// A spaceship game where you try fly through an asteroid field while being
// chased by enemy ships as you struggle to keep your ship under control
#define GAMENAME "Amazing Game!"
#include "highscore.h"

// SFML Library for Rendering (apt-get libsfml-dev)
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <sstream>
#include <string>

#include <cstdlib>

#include <cmath>
#define PI 3.14159265

#include <ctime>

#define MOVEMENTVEL 400
#define ROTATIONVEL 400

#define ASTEROIDPOINTS 1
#define ENEMYPOINTS 5
#define BOMBSCORE 10 // Points for each unused bomb
#define SCOREANIMATIONSTARTTIME                                                \
	2 // Start x seconds after game over screen appears
#define SCOREANIMATIONDELAY 0.05 // Seconds between each increment

#define BOMBCOOLDOWN 1
#define INITIALBOMBS 1

// Powerups
#define PLANETANIMLENGTH 2
#define PLANET_SIZE 80
#define PLANETSPAWNCHANCE 16 // 1/x chance
#define COLLECTRADIUS 40
#define POWERUPCHANCE 20 // 1/ (x/2) chance
#define BOMBCHANCE 5	 // 1/x chance
#define COLLECTSHEILDMESSAGE "Sheild!"
#define COLLECTBOMBMESSAGE "Bomb!"
#define POWERSCALETIME                                                         \
	1 // Time for powerups to get to full size and to shrink to nothing

#define LAZERSPEED 1000 // Speed of lazers
#define LAZERCOOLDOWN 0.25
#define LAZERDESPAWNDISTANCE 100 // Lazer despawn distance
#define HITRADIUS 40 // Player and enemy hit radius for lazers in pixels
#define LAZERINVULNERABILITYPERIOD                                             \
	0.15 // Seconds after shooting a lazer that the player or enemy cannot
	     // be hit you can fire at the last second to survive

#define EXPLOSIONTIME 1
#define EXPLOSIONSIZE 10
#define PLAYEREXPLOSIONSIZE 100

#define EDGEBOUNCESCALE                                                        \
	0.7 // This value is multiplied against movement when a bounce happens
	    // on the edge of screen (does not apply to asteroids)

#define ENEMYDELAYBASE 20
#define ENEMYDELAYSCALE 0.05
#define BASEVELOCITY 100
#define ASTEROIDSPEEDSCALE 2
#define MAXASTEROIDSIZE 800
#define BASEASTEROIDSIZE 100
#define ASTEROIDSIZESCALE 1
#define MINASTEROIDSIZE 25
#define ASTEROIDBOUNCESPEEDSCALE                                               \
	1.00 // Multiplied against asteroid speed after every colision with
	     // another asteroid to prevent crazy speed
#define ENEMYSPAWNCHANCE                                                       \
	8 // 1/x chance for an enemy to apear instead of an asteroid
#define ENEMYMAXROTATIONVELOCITY 200
#define ENEMYMAXVELOCITY 400
#define ENEMYINITIALCOOLDOWN 2 // Seconds before shooting for the first time

#define SHEILDSIZE 30
#define SHEILDRANGE 10 // Ammount the sheild grows in animation
#define SHEILDSPACE 15 // Space between multiple sheilds
#define SHEILDANIMATIONLENGTH 1
#define SHEILDREPELSTRENGTH 1
#define SHEILDSPEEDMULTIPLIER 1
#define SHEILDINVULNERABILITYPERIOD                                            \
	1 // Seconds after losing sheild that the sheild will still activate

#define TEXTTIME 5 // Text animation length
#define TEXTSIZE 40
#define TEXTRISE 200 // Rise animation by x pixels (Goes down btw)

#define GAMEOVERMESSAGE "Game Over!" // Game over floating text
#define SHEILDPOPMESSAGE "Pop!"	     // FLoating text when sheild pops
#define BOUNCEMESSAGE                                                          \
	"Bounce!" // Floating text when you bounce off the edge of the screen

#define GAMEOVERTIME                                                           \
	5 // Time after game over camera continues to show world before gameover
	  // screen

float square(float value) { return value * value; }

float distance(float x, float y) { // Pythagorean Theorem
	return sqrt(square(x) + square(y));
}

#include "font.h"
#include "logo.h"

enum powertypes { SHEILD, BOMB, PLANET };

// Functions For Planets
#define PLANET_FUNC
#include "planets.cpp"

int main() {
	// Create Window
	sf::RenderWindow window(sf::VideoMode(800, 600, 32), GAMENAME,
				sf::Style::Fullscreen);

	// Load Files From Memory

	// Logo Texture
	sf::Texture logo;
	if (!logo.loadFromMemory(logoPtr, LOGOSIZE,
				 sf::IntRect(0, 0, 14, 15))) {
		std::cout << "Failed To Load BitTurtle Logo From Memory: Game "
			     "Corrupted, Reinstall Necessary"
			  << std::endl;
		return EXIT_FAILURE;
	}

	// Roboto Mono Font
	sf::Font roboto;
	if (!roboto.loadFromMemory(fontPtr, FONTSIZE)) {
		std::cout << "Failed To Load Font From Memory: Game Corrupted, "
			     "Reinstall Necessarry"
			  << std::endl;
		return EXIT_FAILURE;
	}

	// Load Files From Filesystem
	sf::SoundBuffer dangerdetectedbuffer;
	if (!dangerdetectedbuffer.loadFromFile("sounds/dangerdetected.wav")) {
		std::cout << "Failed to load sound effect 'dangerdetected'!"
			  << std::endl;
	}
	sf::Sound dangerdetected;
	dangerdetected.setBuffer(dangerdetectedbuffer);

	sf::SoundBuffer lazerfirebuffer;
	if (!lazerfirebuffer.loadFromFile("sounds/lazerfire.wav")) {
		std::cout << "Failed to load sound effect 'lazerfire'!"
			  << std::endl;
	}
	sf::Sound lazerfire;
	lazerfire.setBuffer(lazerfirebuffer);

	sf::SoundBuffer explosionbuffer;
	if (!explosionbuffer.loadFromFile("sounds/explosion.wav")) {
		std::cout << "Failed to load sound effect 'explosion'!"
			  << std::endl;
	}
	sf::Sound explosionsound;
	explosionsound.setBuffer(explosionbuffer);

	sf::SoundBuffer destroybuffer;
	if (!destroybuffer.loadFromFile("sounds/destroy.wav")) {
		std::cout << "Failed to load sound effect 'destroy'!"
			  << std::endl;
	}
	sf::Sound destroysound;
	destroysound.setBuffer(destroybuffer);

	sf::SoundBuffer powerupbuffer;
	if (!powerupbuffer.loadFromFile("sounds/powerup.wav")) {
		std::cout << "Failed to load sound effect 'powerup'!"
			  << std::endl;
	}
	sf::Sound powerupsound;
	powerupsound.setBuffer(powerupbuffer);

	sf::SoundBuffer powerdownbuffer;
	if (!powerdownbuffer.loadFromFile("sounds/powerdown.wav")) {
		std::cout << "Failed to load sound effect 'powerdown'!"
			  << std::endl;
	}
	sf::Sound powerdownsound;
	powerdownsound.setBuffer(powerdownbuffer);

	sf::SoundBuffer buttonclickbuffer;
	if (!buttonclickbuffer.loadFromFile("sounds/buttonclick.wav")) {
		std::cout << "Failed to load sound effect 'buttonclick'!"
			  << std::endl;
	}
	sf::Sound buttonclicksound;
	buttonclicksound.setBuffer(buttonclickbuffer);

	sf::SoundBuffer scorecountbuffer;
	if (!scorecountbuffer.loadFromFile("sounds/scorecount.wav")) {
		std::cout << "Failed to load sound effect 'scorecount'!"
			  << std::endl;
	}
	sf::Sound scorecountsound;
	scorecountsound.setBuffer(scorecountbuffer);

	sf::SoundBuffer bombbuffer;
	if (!bombbuffer.loadFromFile("sounds/bombexplosion.wav")) {
		std::cout << "Failed to load sound effect 'bombexplosion'!"
			  << std::endl;
	}
	sf::Sound bombexplosionsound;
	bombexplosionsound.setBuffer(bombbuffer);

	sf::SoundBuffer bitturtlebuffer;
	if (!bitturtlebuffer.loadFromFile("sounds/bit-turtle.wav")) {
		std::cout << "Failed to load the bit turtle sound!"
			  << std::endl;
	}
	sf::Sound bitturtlesound;
	bitturtlesound.setBuffer(bitturtlebuffer);
	sf::Sound highscoresound;
	highscoresound.setBuffer(bitturtlebuffer);

	sf::Music backgroundmusic;
	if (!backgroundmusic.openFromFile("sounds/backgroundmusic.wav")) {
		std::cout << "Failed to load Background Music!";
	}

// Import Planet Minigame Assets
#define PLANET_ASSETS
#include "planets.cpp"

	backgroundmusic.setLoop(true);
	backgroundmusic.play();

	// Set Random Seed based on Time
	srand((unsigned int)time(0));

	// Home screen animation variables
	float animtime = 0;
	int animstage = 0;

	// Planet Animation Variables
	unsigned int nextminigame = 0;
	float planetanimtime = 0;

	// Create Clock
	sf::Clock clock;
	float delayClock = 0;
	float extraDelayClock = 0;
	float enemyDelayClock = 100000;

	// Tooltip Variables
	bool showtooltip = false;
	std::string tooltiptext = "";
	sf::Vector2f tooltipvec = sf::Vector2f(0, 0);

	// Create DeltaClock
	sf::Clock deltaClock;

	// Variables
	unsigned int screen = 0;
	unsigned int minigame = 0; // 0: main game, 1: game 1, etc
	bool click = false;
	bool player2mode = false;

	// Game
	bool newhighscore = false;
	struct highscore highscores = readhighscore();
	int score = 0;
	int finalscore = 0;  // Counter for final score animation
	float scoretime = 0; // Timer for final score animation

	// Bombs ( [Space] to clear the screen )
	int bombs = INITIALBOMBS; // Start with INITIALBOMBS bombs
	float bombcooldown = 0;

	// Player 1
	bool player1gameover = false;
	int player1sheilds = 0; // Extra sheilds
	bool player1sheild = true;
	float player1time =
	    0; // Time when event happened, clock - time = time since
	sf::Vector2f player1position(0, 0);
	float player1rotation = 0;
	sf::Vector2f player1velocity(0, 0);
	float player1rotationVelocity = 0;
	float player1lazercooldown = 0;

	// Player 2
	bool player2gameover = false;
	int player2sheilds = 0; // Extra sheilds
	bool player2sheild = true;
	float player2time = 0;
	sf::Vector2f player2position(0, 0);
	float player2rotation = 0;
	sf::Vector2f player2velocity(0, 0);
	float player2rotationVelocity = 0;
	float player2lazercooldown = 0;

	// Lazers
	int lazers = 0;
	std::vector<sf::Vector2f> lazerpositions;
	std::vector<float> lazerrotation;
	std::vector<sf::Vector2f> lazervelocity;

	// Explosions
	int explosions = 0;
	std::vector<sf::Vector2f> explosionpositions = {};
	std::vector<float> explosiontime = {};
	std::vector<float> explosionsize = {};

	// Text
	int texts = 0;
	std::vector<sf::Vector2f> textpositions = {};
	std::vector<std::string> textstring = {};
	std::vector<float> texttime = {};

	// Asteroids
	int asteroids = 0;
	std::vector<int> asteroidsize = {};
	std::vector<sf::Vector2f> asteroidpositions = {};
	std::vector<float> asteroidrotation = {};
	std::vector<sf::Vector2f> asteroidvelocity = {};
	std::vector<float> asteroidrotationvelocity = {};

	// Enemies
	int enemies = 0;
	std::vector<sf::Vector2f> enemypositions = {};
	std::vector<sf::Vector2f> enemyvelocity = {};
	std::vector<float> enemyrotation = {};
	std::vector<float> enemyrotationvelocity = {};
	std::vector<float> enemylazercooldown = {};

	// Powerups && Planets (Big powerups that trigger minigames)
	bool planetexists = false;
	int powerups = 0;
	std::vector<sf::Vector2f> poweruppositions = {};
	std::vector<enum powertypes> poweruptype = {};
	std::vector<float> powerupsize = {};
	std::vector<bool> powerupcollected = {};

	bool enter = false;
	bool tabused = false;
	bool tab = false;

	// Planet Variables
#define PLANET_VARS
#include "planets.cpp"

	// Bit turtle sound
	bitturtlesound.play();

	while (window.isOpen()) {
		showtooltip = false;
		enter = false;
		// Process Events
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				// Close Window if [X] Pressed
				window.close();
			if (event.type == sf::Event::KeyPressed)
				switch (event.key.code) {
				case sf::Keyboard::Key::Escape: {
					// Close Window if [esc] Pressed
					window.close();
				} break;
				case sf::Keyboard::Key::Enter: {
					enter = true;
				} break;
				case sf::Keyboard::Key::Tab: {
					tabused = true;
					tab = !tab;
				} break;
				default:
					break;
				}
			if (event.type == sf::Event::MouseButtonReleased)
				switch (event.mouseButton.button) {
				case sf::Mouse::Left: {
					// Log Click For Frame If Clicked
					click = true;
				} break;
				default:
					break;
				}
		}
		// Clear Window
		window.clear();

		float deltatime = deltaClock.getElapsedTime().asSeconds();
		deltaClock.restart();

// Debug Mode FPS counter
#ifdef DEBUG
		{
			int fps = (int)(1.f / deltatime);
			std::stringstream fpsbuf;
			fpsbuf << "FPS: ";
			fpsbuf << fps;
			std::string fpsstring = fpsbuf.str();
			sf::Text fpstext(fpsstring, roboto, 40);
			fpstext.setPosition(0, 0);
			fpstext.setFillColor(sf::Color::White);
			window.draw(fpstext);
		}
#endif

		// Draw To Window
		switch (screen) {
		// Startup Animation
		case 0: {
			sf::Sprite logosprite(logo);
			sf::Time time = clock.getElapsedTime();
			sf::Color logocolor = sf::Color::White;
			if (time.asSeconds() < 1)
				// Fade in
				logocolor.a = time.asSeconds() * 256;
			else if (time.asSeconds() < 3)
				// Visible
				logocolor.a = 255;
			else if (time.asSeconds() < 4)
				// Fade out
				logocolor.a =
				    256 - (time.asSeconds() - 3) * 256;
			else
				// Not visible
				logocolor.a = 0;
			logosprite.setColor(logocolor);
			sf::Vector2u scale = window.getSize();
			scale.x /= 2;
			scale.y /= 2;
			if (scale.x > scale.y)
				scale.x = scale.y;
			else if (scale.y > scale.x)
				scale.y = scale.x;
			// Set a size proportional to 1/2 of the window
			logosprite.setScale(
			    scale.x / logosprite.getLocalBounds().width,
			    scale.y / logosprite.getLocalBounds().height);
			// Center Logo Vertically and Horizontaly
			logosprite.setPosition(
			    window.getSize().x / 2.f - scale.x / 2.f,
			    window.getSize().y / 2.f - scale.y / 2.f);
			window.draw(logosprite);
			// Animation Length
			if (time.asSeconds() >= 4) {
				screen = 1; // Home Screen
				clock.restart();
			}
		} break;
		// Home Screen
		case 1: {
			// Title
			sf::Text title(GAMENAME, roboto, 80);
			// Center Title, 10% from top
			sf::FloatRect titlesize = title.getLocalBounds();
			title.setPosition(sf::Vector2f(
			    window.getSize().x / 2.f - titlesize.width / 2.f,
			    window.getSize().y * 0.1));
			title.setFillColor(sf::Color::White);
			window.draw(title);
			// Play Button
			sf::RectangleShape playbutton(sf::Vector2f(400, 100));
			// Center Play Button, 50% from top
			sf::FloatRect buttonsize = playbutton.getLocalBounds();
			playbutton.setPosition(sf::Vector2f(
			    window.getSize().x / 2.f - buttonsize.width / 2.f,
			    window.getSize().y * 0.5));
			// Detect Mouse Colision
			sf::FloatRect playhitbox = playbutton.getGlobalBounds();
			if (enter ||
			    playhitbox.contains(window.mapPixelToCoords(
				sf::Mouse::getPosition(window)))) {
				// Outline On Hover
				playbutton.setOutlineColor(sf::Color::White);
				playbutton.setOutlineThickness(10);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					// Black On Press
					playbutton.setFillColor(
					    sf::Color::Black);
				if (click || enter) {
					buttonclicksound.play();
					screen = 2; // Player Select
					clock.restart();
				}
			} else
				// White Otherwise
				playbutton.setFillColor(sf::Color::White);
			window.draw(playbutton);
			// Play Button Text
			sf::Text playtext("Play Game", roboto, 50);
			// Center Text In Button
			sf::FloatRect textsize = playtext.getLocalBounds();
			playtext.setPosition(sf::Vector2f(
			    window.getSize().x / 2.f - textsize.width / 2.f,
			    (playhitbox.top + playhitbox.height / 2.f) -
				textsize.height / 2.f));
			if (playbutton.getFillColor() != sf::Color::White)
				playtext.setFillColor(sf::Color::White);
			else
				playtext.setFillColor(sf::Color::Black);
			window.draw(playtext);
			// Tip Text
			sf::Text tiptext("Press [esc] To Exit", roboto, 30);
			// Center Tip Text, 10% from bottom
			sf::FloatRect tipsize = tiptext.getLocalBounds();
			tiptext.setPosition(
			    window.getSize().x / 2.f - tipsize.width / 2.f,
			    window.getSize().y - window.getSize().y * 0.1);
			tiptext.setFillColor(sf::Color::White);
			window.draw(tiptext);

			// Asteroid animation

			// Asteroid Shape
			sf::CircleShape asteroid = sf::CircleShape(50, 4);
			asteroid.setFillColor(sf::Color::Transparent);
			asteroid.setRotation(
			    clock.getElapsedTime().asSeconds());
			asteroid.setOutlineThickness(2);
			asteroid.setOutlineColor(sf::Color::White);

			animtime += deltatime;
			switch (animstage) {
			case 0: { // 5 Second Delay
				if (animtime > 5) {
					animstage = 1;
					animtime = 0;
				}
			} break;
			case 1: {
				sf::Vector2f pos =
				    sf::Vector2f(-100 + animtime * 50,
						 window.getSize().y * 0.7);
				asteroid.setPosition(pos);
				asteroid.setRotation(animtime * 20);
				window.draw(asteroid);
				// Offscreen continue
				if (pos.x > window.getSize().x + 100) {
					animstage = 2;
					animtime = 0;
				}
			} break;
			case 2: {
				// Out of control player crashes into title
				sf::ConvexShape player(
				    6); // Triangle shaped player with
					// rectangular tip
				player.setPoint(0,
						sf::Vector2f(-2, 10)); //     **
				player.setPoint(1,
						sf::Vector2f(-2, 15)); //     **
				player.setPoint(2,
						sf::Vector2f(2, 15)); //     **
				player.setPoint(3,
						sf::Vector2f(2, 10)); //    *  *
				player.setPoint(4,
						sf::Vector2f(10, -10)); //   * *
				player.setPoint(
				    5, sf::Vector2f(-10, -10)); //  ********
				player.setFillColor(
				    sf::Color::Transparent); // Transparent Fill
				player.setOutlineColor(
				    sf::Color::White); // White Outline
				player.setOutlineThickness(2);

				player.setRotation(animtime * 500);
				sf::Vector2f startpos =
				    sf::Vector2f(window.getSize().x + 20,
						 window.getSize().y / 2.f);
				sf::Vector2f endpos = sf::Vector2f(
				    window.getSize().x / 2.f,
				    title.getGlobalBounds().top +
					title.getGlobalBounds().height);

				float animlength = 2;
				sf::Vector2f pos = sf::Vector2f(
				    startpos.x + (endpos.x - startpos.x) *
						     (animtime / animlength),
				    startpos.y + (endpos.y - startpos.y) *
						     (animtime / animlength));
				player.setPosition(pos);

				window.draw(player);

				// Contiue on time up
				if (animtime >= animlength) {
					animtime = 0;
					animstage = 3;
				}
			} break;
			case 3: { // Explosion
				sf::Vector2f pos = sf::Vector2f(
				    window.getSize().x / 2.f,
				    title.getGlobalBounds().top +
					title.getGlobalBounds().height);

				float animlength = EXPLOSIONTIME;
				sf::CircleShape c(0, 16);
				c.setRadius(
				    PLAYEREXPLOSIONSIZE /
				    (EXPLOSIONTIME /
				     (animlength - animtime))); // Grows to size
				c.setPosition(pos);
				c.setFillColor(sf::Color::Transparent);
				c.setOutlineThickness(2);
				c.setOutlineColor(sf::Color::White);
				c.setOrigin(c.getLocalBounds().width / 2,
					    c.getLocalBounds().height / 2);
				window.draw(c);

				// Continue when time over
				if (animtime >= animlength) {
					animtime = 0;
					animstage = 4;
				}
			} break;
			case 4: { // 5s delay
				if (animtime > 5) {
					animtime = 0;
					animstage = 5;
				}
			} break;
			case 5: {
				// Enemy wobbling
				// Draw Enemy
				sf::ConvexShape enemy(
				    8); // Square shaped enemy with rectangular
					// tip
				enemy.setPoint(0,
					       sf::Vector2f(-2, 10)); //     **
				enemy.setPoint(1,
					       sf::Vector2f(-2, 15)); //     **
				enemy.setPoint(
				    2, sf::Vector2f(2, 15)); //  ********
				enemy.setPoint(3, sf::Vector2f(2, 10));	 //  * *
				enemy.setPoint(4, sf::Vector2f(10, 10)); //  * *
				enemy.setPoint(5,
					       sf::Vector2f(10, -10)); //  * *
				enemy.setPoint(
				    6, sf::Vector2f(-10, -10)); //  ********
				enemy.setPoint(7, sf::Vector2f(-10, 10));
				enemy.setFillColor(
				    sf::Color::Transparent); // Black Fill
				enemy.setOutlineColor(
				    sf::Color::White); // White Outline
				enemy.setOutlineThickness(2);

				enemy.setRotation((sin(animtime) * (PI / 2)) *
						  (180 / PI));
				sf::Vector2f startpos =
				    sf::Vector2f(window.getSize().x + 20,
						 window.getSize().y * 0.3);
				sf::Vector2f endpos =
				    sf::Vector2f(-20, window.getSize().y * 0.3);

				float animlength = 5;
				sf::Vector2f pos = sf::Vector2f(
				    startpos.x + (endpos.x - startpos.x) *
						     (animtime / animlength),
				    startpos.y + (endpos.y - startpos.y) *
						     (animtime / animlength));
				enemy.setPosition(pos);

				window.draw(enemy);

				if (animtime >= animlength) {
					animtime = 0;
					animstage = 6;
				}

			} break;
			default: {
				animtime = 0;
				animstage = 0;
			} break;
			}
		} break;
		// Player Select [1P or 2P]
		case 2: {
			// Reset Game Variables
			nextminigame = 0;
			minigame = 0;
			newhighscore = false;

			// Texts
			texts = 0;
			textpositions.clear();
			texttime.clear();
			textstring.clear();

			// Bombs
			bombs = INITIALBOMBS;
			bombcooldown = 0;

			// Powerups
			planetexists = false;
			powerups = 0;
			poweruppositions.clear();
			poweruptype.clear();
			powerupsize.clear();
			powerupcollected.clear();

			// Enemy Timer
			enemyDelayClock = 10000;

			// Enemies
			enemies = 0;
			enemypositions.clear();
			enemyvelocity.clear();
			enemyrotation.clear();
			enemyrotationvelocity.clear();
			enemylazercooldown.clear();

			// Players
			player1sheilds = 0;
			player2sheilds = 0;
			player1gameover = false;
			player2gameover = false;
			player1time = 0;
			player2time = 0;
			// Delete all explosions
			explosions = 0;
			explosionsize.clear();
			explosiontime.clear();
			explosionpositions.clear();
			// Delete all asteroids
			asteroids = 0;
			asteroidpositions.clear();
			asteroidsize.clear();
			asteroidvelocity.clear();
			asteroidrotation.clear();
			asteroidrotationvelocity.clear();
			// Delete all lazers
			lazers = 0;
			lazerpositions.clear();
			lazervelocity.clear();
			lazerrotation.clear();
			// Title
			sf::Text title("Select Players", roboto, 80);
			// Center Title, 10% from top
			sf::FloatRect titlesize = title.getLocalBounds();
			title.setPosition(sf::Vector2f(
			    window.getSize().x / 2.f - titlesize.width / 2,
			    window.getSize().y * 0.1));
			title.setFillColor(sf::Color::White);
			window.draw(title);
			// 1 Player Button
			sf::RectangleShape button1(sf::Vector2f(400, 100));
			// Center 1 Player Button, 50% - button height from top
			sf::FloatRect button1size = button1.getLocalBounds();
			button1.setPosition(sf::Vector2f(
			    window.getSize().x / 2.f - button1size.width / 2.f,
			    window.getSize().y * 0.5 -
				button1size.height * 1.15));
			// Detect Mouse Colision
			sf::FloatRect button1hitbox = button1.getGlobalBounds();
			if ((tabused && !tab) || (enter && !tab) ||
			    button1hitbox.contains(window.mapPixelToCoords(
				sf::Mouse::getPosition(window)))) {

				// Tooltip Highscore on hover
				{
					std::stringstream text;
					text << "Highscore: ";
					text << highscores.highscore;
					tooltiptext = text.str();
					showtooltip = true;
				}
				// Outline On Hover
				button1.setOutlineColor(sf::Color::White);
				button1.setOutlineThickness(10);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					// Black On Press
					button1.setFillColor(sf::Color::Black);
				if ((click && button1hitbox.contains(
						  window.mapPixelToCoords(
						      sf::Mouse::getPosition(
							  window)))) ||
				    (enter && !tab)) {
					buttonclicksound.play();
					player2mode = false;
					screen = 3; // One Player Controls
					score = 0;
					player1position = sf::Vector2f(
					    window.getSize().x / 2.f,
					    window.getSize().y / 2.f);
					player1sheild = true;
					player1rotation = 180;
					player1velocity = sf::Vector2f(0, 0);
					player1rotationVelocity = 0;
					lazerpositions.clear();
					lazerrotation.clear();
					lazervelocity.clear();
					clock.restart();
				}
			} else
				// White Otherwise
				button1.setFillColor(sf::Color::White);
			window.draw(button1);
			// 1 Player Button Text
			sf::Text button1text("1 Player", roboto, 50);
			// Center Text In Button
			sf::FloatRect button1textsize =
			    button1text.getLocalBounds();
			button1text.setPosition(sf::Vector2f(
			    window.getSize().x / 2.f -
				button1textsize.width / 2.f,
			    (button1hitbox.top + button1hitbox.height / 2.f) -
				button1textsize.height / 2.f));
			if (button1.getFillColor() != sf::Color::White)
				button1text.setFillColor(sf::Color::White);
			else
				button1text.setFillColor(sf::Color::Black);
			window.draw(button1text);
			// 2 Player Button
			sf::RectangleShape button2(sf::Vector2f(400, 100));
			// Center 2 Player Button, 50% + button height from top
			sf::FloatRect button2size = button2.getLocalBounds();
			button2.setPosition(sf::Vector2f(
			    window.getSize().x / 2.f - button2size.width / 2,
			    window.getSize().y * 0.5 +
				button2size.height * 1.15));
			// Detect Mouse Colision
			sf::FloatRect button2hitbox = button2.getGlobalBounds();
			if ((tabused && tab) || (enter && tab) ||
			    button2hitbox.contains(window.mapPixelToCoords(
				sf::Mouse::getPosition(window)))) {
				// Tooltip Highscore
				{
					std::stringstream text;
					text << "Highscore: ";
					text << highscores.twoplayerhighscore;
					tooltiptext = text.str();
					showtooltip = true;
				}
				// Outline On Hover
				button2.setOutlineColor(sf::Color::White);
				button2.setOutlineThickness(10);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					// Black On Press
					button2.setFillColor(sf::Color::Black);
				if ((click && button2hitbox.contains(
						  window.mapPixelToCoords(
						      sf::Mouse::getPosition(
							  window)))) ||
				    (enter && tab)) {
					buttonclicksound.play();
					player2mode = true;
					screen = 4; // 2 - Player Controls
					score = 0;
					player1position = sf::Vector2f(
					    window.getSize().x / 4.f,
					    window.getSize().y / 2.f);
					player1sheild = true;
					player1rotation = 180;
					player1velocity = sf::Vector2f(0, 0);
					player1rotationVelocity = 0;
					player2position = sf::Vector2f(
					    window.getSize().x -
						window.getSize().x / 4.f,
					    window.getSize().y / 2.f);
					player2sheild = true;
					player2rotation = 180;
					player2velocity = sf::Vector2f(0, 0);
					player2rotationVelocity = 0;
					lazerpositions.clear();
					lazerrotation.clear();
					lazervelocity.clear();
					clock.restart();
				}
			} else
				// White Otherwise
				button2.setFillColor(sf::Color::White);
			window.draw(button2);
			// 2 Player Button Text
			sf::Text button2text("2 Player", roboto, 50);
			// Center Text In Button
			sf::FloatRect button2textsize =
			    button2text.getLocalBounds();
			button2text.setPosition(sf::Vector2f(
			    window.getSize().x / 2.f -
				button2textsize.width / 2.f,
			    (button2hitbox.top + button2hitbox.height / 2.f) -
				button2textsize.height / 2.f));
			if (button2.getFillColor() != sf::Color::White)
				button2text.setFillColor(sf::Color::White);
			else
				button2text.setFillColor(sf::Color::Black);
			window.draw(button2text);
		} break;
		// 1 Player Controls
		case 3: {
			// Title
			sf::Text title("Controls", roboto, 80);
			// Center Title, 10% from top
			sf::FloatRect titlesize = title.getLocalBounds();
			title.setPosition(sf::Vector2f(
			    window.getSize().x / 2.f - titlesize.width / 2.f,
			    window.getSize().y * 0.1));
			title.setFillColor(sf::Color::White);
			window.draw(title);
			// Engine Controls 30% from top
			sf::Text enginetext("Engine: [W] Or [^]", roboto, 50);
			enginetext.setFillColor(sf::Color::White);
			enginetext.setPosition(
			    window.getSize().x / 2.f -
				enginetext.getLocalBounds().width / 2.f,
			    window.getSize().y * 0.3 -
				enginetext.getLocalBounds().height / 2.f);
			window.draw(enginetext);
			// Steering Controls 45% from top
			sf::Text steeringtext("Steering: [A][S] Or [<][>]",
					      roboto, 50);
			steeringtext.setFillColor(sf::Color::White);
			steeringtext.setPosition(
			    window.getSize().x / 2.f -
				steeringtext.getLocalBounds().width / 2.f,
			    window.getSize().y * 0.45 -
				steeringtext.getLocalBounds().height / 2.f);
			window.draw(steeringtext);
			// Firing Controls 60% from top
			sf::Text firingtext("Firing: [S] Or [v]", roboto, 50);
			firingtext.setFillColor(sf::Color::White);
			firingtext.setPosition(
			    window.getSize().x / 2.f -
				firingtext.getLocalBounds().width / 2.f,
			    window.getSize().y * 0.6 -
				firingtext.getLocalBounds().height / 2.f);
			window.draw(firingtext);
			// Bomb Controls 75% from top
			sf::Text bombtext("Bomb: [Space]", roboto, 50);
			bombtext.setFillColor(sf::Color::White);
			bombtext.setPosition(
			    window.getSize().x / 2.f -
				bombtext.getLocalBounds().width / 2.f,
			    window.getSize().y * 0.75 -
				bombtext.getLocalBounds().height / 2.f);
			window.draw(bombtext);
			// Tip Text
			sf::Text tiptext("Click or Press [Enter] To Continue",
					 roboto, 30);
			// Center Tip Text, 10% from bottom
			sf::FloatRect tipsize = tiptext.getLocalBounds();
			tiptext.setPosition(
			    window.getSize().x / 2.f - tipsize.width / 2.f,
			    window.getSize().y - window.getSize().y * 0.1);
			tiptext.setFillColor(sf::Color::White);
			window.draw(tiptext);
			if (enter || click) {
				// Go to game when clicked
				screen = 6; // Game
				clock.restart();
			}
		} break;
		// 2 Player Player 1 Controls
		case 4: {
			// Title
			sf::Text title("Player 1 Controls", roboto, 80);
			// Center Title, 10% from top
			sf::FloatRect titlesize = title.getLocalBounds();
			title.setPosition(sf::Vector2f(
			    window.getSize().x / 2.f - titlesize.width / 2.f,
			    window.getSize().y * 0.1));
			title.setFillColor(sf::Color::White);
			window.draw(title);
			// Engine Controls 30% from top
			sf::Text enginetext("Engine: [W]", roboto, 50);
			enginetext.setFillColor(sf::Color::White);
			enginetext.setPosition(
			    window.getSize().x / 2.f -
				enginetext.getLocalBounds().width / 2.f,
			    window.getSize().y * 0.3 -
				enginetext.getLocalBounds().height / 2.f);
			window.draw(enginetext);
			// Steering Controls 45% from top
			sf::Text steeringtext("Steering: [A][D]", roboto, 50);
			steeringtext.setFillColor(sf::Color::White);
			steeringtext.setPosition(
			    window.getSize().x / 2.f -
				steeringtext.getLocalBounds().width / 2.f,
			    window.getSize().y * 0.45 -
				steeringtext.getLocalBounds().height / 2.f);
			window.draw(steeringtext);
			// Firing Controls 60% from top
			sf::Text firingtext("Firing: [S]", roboto, 50);
			firingtext.setFillColor(sf::Color::White);
			firingtext.setPosition(
			    window.getSize().x / 2.f -
				firingtext.getLocalBounds().width / 2.f,
			    window.getSize().y * 0.6 -
				firingtext.getLocalBounds().height / 2.f);
			window.draw(firingtext);
			// Bomb Controls 75% from top
			sf::Text bombtext("Bomb: [Space]", roboto, 50);
			bombtext.setFillColor(sf::Color::White);
			bombtext.setPosition(
			    window.getSize().x / 2.f -
				bombtext.getLocalBounds().width / 2,
			    window.getSize().y * 0.75 -
				bombtext.getLocalBounds().height / 2.f);
			window.draw(bombtext);
			// Tip Text
			sf::Text tiptext("Click or Press [Enter] To Continue",
					 roboto, 30);
			// Center Tip Text, 10% from bottom
			sf::FloatRect tipsize = tiptext.getLocalBounds();
			tiptext.setPosition(
			    window.getSize().x / 2.f - tipsize.width / 2.f,
			    window.getSize().y - window.getSize().y * 0.1);
			tiptext.setFillColor(sf::Color::White);
			window.draw(tiptext);
			if (click || enter) {
				buttonclicksound.play();
				// Go to player 2 controls when clicked
				screen = 5; // Player 2 Controls
				clock.restart();
			}
		} break;
		// 2 Player Player 2 Controls
		case 5: {
			// Title
			sf::Text title("Player 2 Controls", roboto, 80);
			// Center Title, 10% from top
			sf::FloatRect titlesize = title.getLocalBounds();
			title.setPosition(sf::Vector2f(
			    window.getSize().x / 2.f - titlesize.width / 2.f,
			    window.getSize().y * 0.1));
			title.setFillColor(sf::Color::White);
			window.draw(title);
			// Engine Controls 30% from top
			sf::Text enginetext("Engine: [^]", roboto, 50);
			enginetext.setFillColor(sf::Color::White);
			enginetext.setPosition(
			    window.getSize().x / 2.f -
				enginetext.getLocalBounds().width / 2.f,
			    window.getSize().y * 0.3 -
				enginetext.getLocalBounds().height / 2.f);
			window.draw(enginetext);
			// Steering Controls 45% from top
			sf::Text steeringtext("Steering: [<][>]", roboto, 50);
			steeringtext.setFillColor(sf::Color::White);
			steeringtext.setPosition(
			    window.getSize().x / 2.f -
				steeringtext.getLocalBounds().width / 2.f,
			    window.getSize().y * 0.45 -
				steeringtext.getLocalBounds().height / 2.f);
			window.draw(steeringtext);
			// Firing Controls 60% from top
			sf::Text firingtext("Firing: [v]", roboto, 50);
			firingtext.setFillColor(sf::Color::White);
			firingtext.setPosition(
			    window.getSize().x / 2.f -
				firingtext.getLocalBounds().width / 2.f,
			    window.getSize().y * 0.6 -
				firingtext.getLocalBounds().height / 2.f);
			window.draw(firingtext);
			// Bomb Controls 75% from top
			sf::Text bombtext("Bomb: [Space]", roboto, 50);
			bombtext.setFillColor(sf::Color::White);
			bombtext.setPosition(
			    window.getSize().x / 2.f -
				bombtext.getLocalBounds().width / 2.f,
			    window.getSize().y * 0.75 -
				bombtext.getLocalBounds().height / 2.f);
			window.draw(bombtext);
			// Tip Text
			sf::Text tiptext("Click or Press [Enter] To Continue",
					 roboto, 30);
			// Center Tip Text, 10% from bottom
			sf::FloatRect tipsize = tiptext.getLocalBounds();
			tiptext.setPosition(
			    window.getSize().x / 2.f - tipsize.width / 2.f,
			    window.getSize().y - window.getSize().y * 0.1);
			tiptext.setFillColor(sf::Color::White);
			window.draw(tiptext);
			if (click || enter) {
				// Go to game when clicked
				screen = 6; // Game
				clock.restart();
			}
		} break;
		// Game
		case 6: {
			// Display Score 5% from top on left
			std::stringstream scorebuffer;
			scorebuffer << "Score: " << score;
			std::string scorestring = scorebuffer.str();
			sf::Text scoretext(scorestring.c_str(), roboto, 50);
			scoretext.setFillColor(sf::Color::White);
			scoretext.setPosition(
			    window.getSize().y * 0.05,
			    window.getSize().y * 0.05 -
				scoretext.getLocalBounds().height / 2);
			window.draw(scoretext);

			// Display Bombs 5% from top on right
			std::stringstream bombbuffer;
			bombbuffer << "Bombs: " << bombs;
			std::string bombstring = bombbuffer.str();
			sf::Text bombtext(bombstring.c_str(), roboto, 50);
			bombtext.setFillColor(sf::Color::White);
			bombtext.setPosition(
			    window.getSize().x -
				bombtext.getLocalBounds().width -
				window.getSize().y * 0.05,
			    window.getSize().y * 0.05 -
				bombtext.getLocalBounds().height / 2);
			window.draw(bombtext);

			switch (minigame) {
			case 0: { // Main Game

				// Spawn Enemies / Asteroids

				// Update Enemy Delay Clock
				enemyDelayClock += (deltatime);

				// Spawn if no enemies/asteroids or timer over
				if (enemies == 0 && asteroids == 0)
					enemyDelayClock = 10000;
				if (enemyDelayClock >
				    ENEMYDELAYBASE /
					(clock.getElapsedTime().asSeconds() *
					 ENEMYDELAYSCALE)) {
					// Play Enemy Spawn Sound
					dangerdetected.play();

					// Spawning
					sf::Vector2f location = sf::Vector2f(
					    rand() % window.getSize().x,
					    rand() % window.getSize()
							 .y); // Random Location
					float size =
					    rand() %
						(int)(BASEASTEROIDSIZE +
						      (clock.getElapsedTime()
							   .asSeconds() *
						       ASTEROIDSIZESCALE)) +
					    MINASTEROIDSIZE;
					size = (int)size %
					       MAXASTEROIDSIZE; // Keep size
								// reasonable
					switch (rand() % 4) {
					case 0:
						location.x = -size;
						break;
					case 1:
						location.y = -size;
						break;
					case 2:
						location.x =
						    window.getSize().x + size;
						break;
					case 3:
						location.y =
						    window.getSize().y + size;
					}
					if (rand() % (int)PLANETSPAWNCHANCE ==
						0 &&
					    !planetexists) {
						planetexists = true;
						powerups++;
						poweruppositions.push_back(
						    sf::Vector2f(
							window.getSize().x / 2,
							window.getSize().y /
							    2));
						poweruptype.push_back(PLANET);
						powerupsize.push_back(0);
						powerupcollected.push_back(
						    false);
					} else if (rand() %
						       (int)ENEMYSPAWNCHANCE ==
						   0) {
						// Spawn enemy
						enemies++;
						enemypositions.push_back(
						    location);
						enemyvelocity.push_back(
						    sf::Vector2f(0, 0));
						enemyrotation.push_back(0);
						enemyrotationvelocity.push_back(
						    0);
						enemylazercooldown.push_back(
						    ENEMYINITIALCOOLDOWN);
					} else {
						sf::Vector2f velocity = sf::Vector2f(
						    (rand() %
						     (int)(clock
							       .getElapsedTime()
							       .asSeconds() +
							   1)) *
							    ASTEROIDSPEEDSCALE +
							BASEVELOCITY,
						    (rand() %
						     (int)(clock
							       .getElapsedTime()
							       .asSeconds() +
							   1)) *
							    ASTEROIDSPEEDSCALE +
							BASEVELOCITY);
						asteroids++;
						asteroidsize.push_back(size);
						asteroidpositions.push_back(
						    location);
						asteroidvelocity.push_back(
						    velocity);
						asteroidrotation.push_back(
						    rand() % 360);
						asteroidrotationvelocity.push_back(
						    (rand() %
						     (int)(clock
							       .getElapsedTime()
							       .asSeconds() +
							   1)) +
						    BASEVELOCITY);
						if (rand() % 2 == 0)
							asteroidrotationvelocity
							    .at(asteroids -
								1) *= -1;
					}

					enemyDelayClock = 0;
				}

				// Player 1
				if (!player1gameover) {
					// Player Controls
					player1lazercooldown -= deltatime;
					bool engine = false;
					if (!player2mode) {
						// 1-Player Mode Player 1
						// Controls WASD or arrow
						if (sf::Keyboard::isKeyPressed(
							sf::Keyboard::W) |
						    sf::Keyboard::isKeyPressed(
							sf::Keyboard::Up)) {
							// Go Forwards
							// Acceleration
							engine = true;
							player1velocity.x -=
							    sin(player1rotation *
								PI / 180) *
							    (deltatime)*MOVEMENTVEL;
							player1velocity.y +=
							    cos(player1rotation *
								PI / 180) *
							    (deltatime)*MOVEMENTVEL;
						}
						float direction = 0;
						if (sf::Keyboard::isKeyPressed(
							sf::Keyboard::A) |
						    sf::Keyboard::isKeyPressed(
							sf::Keyboard::Left)) {
							direction -= 1;
						}
						if (sf::Keyboard::isKeyPressed(
							sf::Keyboard::D) |
						    sf::Keyboard::isKeyPressed(
							sf::Keyboard::Right)) {
							direction += 1;
						}
						if (sf::Keyboard::isKeyPressed(
							sf::Keyboard::A) |
							sf::Keyboard::
							    isKeyPressed(
								sf::Keyboard::
								    Left) &&
						    sf::Keyboard::isKeyPressed(
							sf::Keyboard::D) |
							sf::Keyboard::
							    isKeyPressed(
								sf::Keyboard::
								    Right)) {
							if (player1rotationVelocity >
							    0)
								direction = -1;
							else if (
							    player1rotationVelocity <
							    0)
								direction = 1;
						}
						player1rotationVelocity +=
						    direction *
						    (deltatime)*ROTATIONVEL;
						// Shoot Lazer
						if (player1lazercooldown <= 0 &&
						    (sf::Keyboard::isKeyPressed(
							 sf::Keyboard::S) |
						     sf::Keyboard::isKeyPressed(
							 sf::Keyboard::Down))) {
							lazerfire.play();
							lazers++;
							player1lazercooldown =
							    LAZERCOOLDOWN;
							lazerpositions.push_back(
							    player1position);
							lazerrotation.push_back(
							    player1rotation);
							lazervelocity.push_back(
							    sf::Vector2f(
								sin(-player1rotation *
								    PI / 180) *
								    LAZERSPEED,
								cos(-player1rotation *
								    PI / 180) *
								    LAZERSPEED));
							lazervelocity.at(
							    lazervelocity
								.size() -
							    1) +=
							    player1velocity; // Adds
									     // player
									     // speed to
									     // lazer
									     // velocity
							if (distance(
								lazervelocity
								    .at(lazervelocity
									    .size() -
									1)
								    .x,
								lazervelocity
								    .at(lazervelocity
									    .size() -
									1)
								    .y) <
							    LAZERSPEED)
								lazervelocity.at(
								    lazervelocity
									.size() -
								    1) -=
								    player1velocity;
							lazerpositions.at(
							    lazerpositions
								.size() -
							    1) +=
							    sf::Vector2f(
								sin(-player1rotation *
								    PI / 180),
								cos(-player1rotation *
								    PI / 180)) *
							    (float)
								HITRADIUS; // Get
									   // Lazer
									   // out
									   // of
									   // hit
									   // radius
						}
					} else {
						// 2-player mode player 1
						// controls WASD
						if (sf::Keyboard::isKeyPressed(
							sf::Keyboard::W)) {
							// Go Forwards
							// Acceleration
							engine = true;
							player1velocity.x -=
							    sin(player1rotation *
								PI / 180) *
							    (deltatime)*MOVEMENTVEL;
							player1velocity.y +=
							    cos(player1rotation *
								PI / 180) *
							    (deltatime)*MOVEMENTVEL;
						}
						float direction = 0;
						if (sf::Keyboard::isKeyPressed(
							sf::Keyboard::A)) {
							direction -= 1;
						}
						if (sf::Keyboard::isKeyPressed(
							sf::Keyboard::D)) {
							direction += 1;
						}
						if (sf::Keyboard::isKeyPressed(
							sf::Keyboard::A) &&
						    sf::Keyboard::isKeyPressed(
							sf::Keyboard::D)) {
							if (player1rotationVelocity >
							    0)
								direction = -1;
							else if (
							    player1rotationVelocity <
							    0)
								direction = 1;
						}
						player1rotationVelocity +=
						    direction *
						    (deltatime)*ROTATIONVEL;
						// Shoot Lazer
						if (player1lazercooldown <= 0 &&
						    sf::Keyboard::isKeyPressed(
							sf::Keyboard::S)) {
							lazerfire.play();
							lazers++;
							player1lazercooldown =
							    LAZERCOOLDOWN;
							lazerpositions.push_back(
							    player1position);
							lazerrotation.push_back(
							    player1rotation);
							lazervelocity.push_back(
							    sf::Vector2f(
								sin(-player1rotation *
								    PI / 180) *
								    LAZERSPEED,
								cos(-player1rotation *
								    PI / 180) *
								    LAZERSPEED));
							lazervelocity.at(
							    lazervelocity
								.size() -
							    1) +=
							    player1velocity;
							if (distance(
								lazervelocity
								    .at(lazervelocity
									    .size() -
									1)
								    .x,
								lazervelocity
								    .at(lazervelocity
									    .size() -
									1)
								    .y) <
							    LAZERSPEED)
								lazervelocity.at(
								    lazervelocity
									.size() -
								    1) -=
								    player1velocity;
						}
					}
					// Update Position & Rotation
					player1position +=
					    player1velocity * (deltatime);
					player1rotation +=
					    player1rotationVelocity *
					    (deltatime);

					// Use next sheild if timer over
					if (!player1sheild &&
					    player1sheilds > 0 &&
					    clock.getElapsedTime().asSeconds() -
						    player1time >=
						SHEILDINVULNERABILITYPERIOD) {
						player1sheild = true;
						player1sheilds--;
					}

					// GameOver if colision
					for (int a = 0; a < asteroids; a++) {
						sf::Vector2f apos =
						    asteroidpositions.at(a);
						float size = asteroidsize.at(a);
						if (distance(player1position.x -
								 apos.x,
							     player1position.y -
								 apos.y) <
						    size) {
							// Collision!
							if (player1sheild ||
							    clock.getElapsedTime()
									.asSeconds() -
								    player1time <
								SHEILDINVULNERABILITYPERIOD) { // Bounce with sheild
								// Get New Angle
								float movangle =
								    atan2(
									player1velocity
									    .x,
									player1velocity
									    .y);
								float posangle =
								    atan2(
									player1position
										.x -
									    apos.x,
									player1position
										.y -
									    apos.y) -
								    PI /
									2; // Get
									   // Perpendicular
								float newangle =
								    2 * posangle -
								    movangle; // New
									      // angle
									      // after
									      // bounce
								float velocity =
								    distance(
									player1velocity
									    .x,
									player1velocity
									    .y) *
								    SHEILDSPEEDMULTIPLIER;

								// Bounce
								player1velocity
								    .x =
								    sin(newangle) *
								    velocity;
								player1velocity
								    .y =
								    cos(newangle) *
								    velocity;

								// Repel
								player1velocity += sf::Vector2f(
								    (player1position
									 .x -
								     apos.x) /
									asteroidsize
									    .at(a) *
									SHEILDREPELSTRENGTH,
								    (player1position
									 .y -
								     apos.y) /
									asteroidsize
									    .at(a) *
									SHEILDREPELSTRENGTH);

								// Push out if
								// in asteroid
								if (distance(
									player1position
										.x -
									    apos.x,
									player1position
										.y -
									    apos.y) <
								    asteroidsize
									.at(a)) {
									player1position
									    .x +=
									    (player1position
										 .x -
									     apos.x) /
									    asteroidsize
										.at(a) *
									    (asteroidsize
										 .at(a) -
									     distance(
										 player1position
											 .x -
										     apos.x,
										 player1position
											 .y -
										     apos.y));
									player1position
									    .y +=
									    (player1position
										 .y -
									     apos.y) /
									    asteroidsize
										.at(a) *
									    (asteroidsize
										 .at(a) -
									     distance(
										 player1position
											 .x -
										     apos.x,
										 player1position
											 .y -
										     apos.y));
								}

								// Push if in
								// tragectory of
								// asteroid
								float amovangle =
								    atan2(
									asteroidvelocity
									    .at(a)
									    .x,
									asteroidvelocity
									    .at(a)
									    .y);
								if (posangle >=
									amovangle -
									    PI /
										2 &&
								    posangle <=
									amovangle +
									    PI /
										2)
									player1velocity +=
									    asteroidvelocity
										.at(a);

								// Text
								if (player1sheild) {
									texts++;
									textpositions
									    .push_back(
										player1position);
									textstring
									    .push_back(
										SHEILDPOPMESSAGE);
									texttime
									    .push_back(
										0);
								}
								// Pop Sheild
								player1sheild =
								    false;
								powerdownsound
								    .play();
								player1time =
								    clock
									.getElapsedTime()
									.asSeconds();
								;
							} else {
								player1gameover =
								    true;
								// Explosion
								explosionsound
								    .play();
								explosions++;
								explosionpositions
								    .push_back(
									player1position);
								explosiontime
								    .push_back(
									EXPLOSIONTIME);
								explosionsize
								    .push_back(
									PLAYEREXPLOSIONSIZE);

								// Text
								texts++;
								textpositions
								    .push_back(
									player1position);
								textstring.push_back(
								    GAMEOVERMESSAGE);
								texttime
								    .push_back(
									0);
							}
							break;
						}
					}
					// Gameover if lazer hit
					if (player1lazercooldown <
					    LAZERCOOLDOWN -
						LAZERINVULNERABILITYPERIOD)
						for (int l = 0; l < lazers;
						     l++) {
							if (distance(
								lazerpositions
									.at(l)
									.x -
								    player1position
									.x,
								lazerpositions
									.at(l)
									.y -
								    player1position
									.y) <=
							    HITRADIUS) {

								// Delete Lazer
								lazerpositions.erase(
								    lazerpositions
									.begin() +
								    l);
								lazerrotation.erase(
								    lazerrotation
									.begin() +
								    l);
								lazervelocity.erase(
								    lazervelocity
									.begin() +
								    l);
								lazers--;
								l--;

								if (player1sheild ||
								    clock.getElapsedTime()
										.asSeconds() -
									    player1time <
									SHEILDINVULNERABILITYPERIOD) { // Bounce with sheild
									// Pop
									// Sheild
									player1sheild =
									    false;
									powerdownsound
									    .play();
									player1time =
									    clock
										.getElapsedTime()
										.asSeconds();
									// Text
									texts++;
									textpositions
									    .push_back(
										player1position);
									textstring
									    .push_back(
										SHEILDPOPMESSAGE);
									texttime
									    .push_back(
										0);
								} else {
									player1gameover =
									    true;
									// Explosion
									explosionsound
									    .play();
									explosions++;
									explosionpositions
									    .push_back(
										player1position);
									explosiontime
									    .push_back(
										EXPLOSIONTIME);
									explosionsize
									    .push_back(
										PLAYEREXPLOSIONSIZE);

									// Text
									texts++;
									textpositions
									    .push_back(
										player1position);
									textstring
									    .push_back(
										GAMEOVERMESSAGE);
									texttime
									    .push_back(
										0);
								}
								break;
							}
						}

					bool bounce = false;
					// Bounce on edge of screen
					if (player1position.x < 0 &&
						player1velocity.x < 0 ||
					    player1position.x >
						    window.getSize().x &&
						player1velocity.x > 0) {
						player1velocity.x *=
						    -1 * EDGEBOUNCESCALE;
						bounce = true;
					}
					if (player1position.y < 0 &&
						player1velocity.y < 0 ||
					    player1position.y >
						    window.getSize().y &&
						player1velocity.y > 0) {
						player1velocity.y *=
						    -1 * EDGEBOUNCESCALE;
						bounce = true;
					}
					if (bounce) {
						// Text
						texts++;
						textpositions.push_back(
						    player1position);
						textstring.push_back(
						    BOUNCEMESSAGE);
						texttime.push_back(0);
					}

					// Draw Player
					sf::ConvexShape player(
					    6); // Triangle shaped player with
						// rectangular tip
					player.setPoint(
					    0, sf::Vector2f(-2, 10)); //     **
					player.setPoint(
					    1, sf::Vector2f(-2, 15)); //     **
					player.setPoint(
					    2, sf::Vector2f(2, 15)); //     **
					player.setPoint(
					    3, sf::Vector2f(2, 10)); //    *  *
					player.setPoint(
					    4, sf::Vector2f(10, -10)); //   * *
					player.setPoint(
					    5, sf::Vector2f(-10,
							    -10)); //  ********
					player.setFillColor(
					    sf::Color::
						Transparent); // Transparent
							      // Fill
					player.setOutlineColor(
					    sf::Color::White); // White Outline
					player.setOutlineThickness(2);

					player.setPosition(
					    player1position); // Position
					player.setRotation(
					    player1rotation); // Rotate

					// Draw
					window.draw(player);

					// Draw Fire
					sf::ConvexShape fire(5);
					fire.setPoint(0, sf::Vector2f(-8, -15));
					fire.setPoint(1, sf::Vector2f(-5, -20));
					fire.setPoint(2, sf::Vector2f(0, -18));
					fire.setPoint(3, sf::Vector2f(5, -20));
					fire.setPoint(4, sf::Vector2f(8, -15));

					fire.setFillColor(
					    sf::Color::Transparent);
					fire.setOutlineColor(sf::Color::White);
					fire.setOutlineThickness(2);

					fire.setPosition(
					    player1position); // Position
					fire.setRotation(
					    player1rotation); // Rotate

					if (engine)
						window.draw(fire);

					if (player2mode) {
						// Player indicator
						sf::RectangleShape indicator(
						    sf::Vector2f(4, 4));

						indicator.setOrigin(
						    indicator.getLocalBounds()
							    .width /
							2,
						    indicator.getLocalBounds()
								.height /
							    2 +
							2); // Shift 2px down
							    // from center
						indicator.setPosition(
						    player1position);
						indicator.setRotation(
						    player1rotation);

						indicator.setFillColor(
						    sf::Color::Black); // Black
								       // Fill
						indicator.setOutlineColor(
						    sf::Color::
							White); // White Outline
						indicator.setOutlineThickness(
						    2);

						// Draw
						window.draw(indicator);
					}

					// Draw sheild
					float ptime =
					    clock.getElapsedTime().asSeconds() -
					    player1time;
					sf::CircleShape sheild;
					float sheildThickness = 2;
					for (int i = 0; i < player1sheilds;
					     i++) {
						sheild = sf::CircleShape(
						    SHEILDSIZE +
							SHEILDSPACE * i,
						    16);
						sheild.setFillColor(
						    sf::Color::Transparent);
						sheild.setOutlineThickness(
						    sheildThickness);
						sheild.setOutlineColor(
						    sf::Color::White);
						sheild.setOrigin(
						    sheild.getLocalBounds()
							    .width /
							2,
						    sheild.getLocalBounds()
							    .height /
							2);
						sheild.setPosition(
						    player1position);
						window.draw(sheild);
					}
					if (player1sheild) {
						if (ptime >
						    SHEILDANIMATIONLENGTH) {
							sheild = sf::CircleShape(
							    SHEILDSIZE +
								(SHEILDSPACE *
								 player1sheilds),
							    16);
						} else {
							sheild = sf::CircleShape(
							    SHEILDSIZE +
								(SHEILDSPACE *
								 player1sheilds) +
								SHEILDRANGE *
								    ((SHEILDANIMATIONLENGTH -
								      ptime) /
								     SHEILDANIMATIONLENGTH),
							    16);
							sheildThickness =
							    ptime /
							    SHEILDANIMATIONLENGTH *
							    2;
						}
						sheild.setFillColor(
						    sf::Color::Transparent);
						sheild.setOutlineThickness(
						    sheildThickness);
						sheild.setOutlineColor(
						    sf::Color::White);
						sheild.setOrigin(
						    sheild.getLocalBounds()
							    .width /
							2,
						    sheild.getLocalBounds()
							    .height /
							2);
						sheild.setPosition(
						    player1position);
						window.draw(sheild);
					} else {
						if (ptime <=
						    SHEILDANIMATIONLENGTH) {
							sheild = sf::CircleShape(
							    SHEILDSIZE +
								SHEILDSPACE *
								    player1sheilds +
								SHEILDRANGE *
								    (ptime /
								     SHEILDANIMATIONLENGTH),
							    16);
							sheildThickness =
							    (SHEILDANIMATIONLENGTH -
							     ptime) /
							    SHEILDANIMATIONLENGTH *
							    2;
							sheild.setFillColor(
							    sf::Color::
								Transparent);
							sheild
							    .setOutlineThickness(
								sheildThickness);
							sheild.setOutlineColor(
							    sf::Color::White);
							sheild.setOrigin(
							    sheild.getLocalBounds()
								    .width /
								2,
							    sheild.getLocalBounds()
								    .height /
								2);
							sheild.setPosition(
							    player1position);
							window.draw(sheild);
						}
					}
				}
				// Player 2
				if (!player2gameover && player2mode) {
					bool engine = false;
					player2lazercooldown -= deltatime;
					if (player2mode) {
						// 2-player mode player 2
						// controls arrow
						if (sf::Keyboard::isKeyPressed(
							sf::Keyboard::Up)) {
							// Go Forwards
							// Acceleration
							engine = true;
							player2velocity.x -=
							    sin(player2rotation *
								PI / 180) *
							    (deltatime)*MOVEMENTVEL;
							player2velocity.y +=
							    cos(player2rotation *
								PI / 180) *
							    (deltatime)*MOVEMENTVEL;
						}
						float direction = 0;
						if (sf::Keyboard::isKeyPressed(
							sf::Keyboard::Left)) {
							direction -= 1;
						}
						if (sf::Keyboard::isKeyPressed(
							sf::Keyboard::Right)) {
							direction += 1;
						}
						if (sf::Keyboard::isKeyPressed(
							sf::Keyboard::Left) &&
						    sf::Keyboard::isKeyPressed(
							sf::Keyboard::Right)) {
							if (player2rotationVelocity >
							    0)
								direction = -1;
							else if (
							    player2rotationVelocity <
							    0)
								direction = 1;
						}
						player2rotationVelocity +=
						    direction *
						    (deltatime)*ROTATIONVEL;
						// Shoot Lazer
						if (player2lazercooldown <= 0 &&
						    sf::Keyboard::isKeyPressed(
							sf::Keyboard::Down)) {
							lazerfire.play();
							lazers++;
							player2lazercooldown =
							    LAZERCOOLDOWN;
							lazerpositions.push_back(
							    player2position);
							lazerrotation.push_back(
							    player2rotation);
							lazervelocity.push_back(
							    sf::Vector2f(
								sin(-player2rotation *
								    PI / 180) *
								    LAZERSPEED,
								cos(-player2rotation *
								    PI / 180) *
								    LAZERSPEED));
							lazervelocity.at(
							    lazervelocity
								.size() -
							    1) +=
							    player2velocity; // Adds
									     // player
									     // speed to
									     // lazer
									     // velocity
							if (distance(
								lazervelocity
								    .at(lazervelocity
									    .size() -
									1)
								    .x,
								lazervelocity
								    .at(lazervelocity
									    .size() -
									1)
								    .y) <
							    LAZERSPEED)
								lazervelocity.at(
								    lazervelocity
									.size() -
								    1) -=
								    player2velocity;
							lazerpositions.at(
							    lazerpositions
								.size() -
							    1) +=
							    sf::Vector2f(
								sin(-player2rotation *
								    PI / 180),
								cos(-player2rotation *
								    PI / 180)) *
							    (float)
								HITRADIUS; // Get
									   // Lazer
									   // out
									   // of
									   // hit
									   // radius
						}
					}

					// Update Position & Rotation
					player2position +=
					    player2velocity * (deltatime);
					player2rotation +=
					    player2rotationVelocity *
					    (deltatime);

					// Use next sheild if timer over
					if (!player2sheild &&
					    player2sheilds > 0 &&
					    clock.getElapsedTime().asSeconds() -
						    player2time >=
						SHEILDINVULNERABILITYPERIOD) {
						player2sheild = true;
						player2sheilds--;
					}

					// GameOver if colision
					for (int a = 0; a < asteroids; a++) {
						sf::Vector2f apos =
						    asteroidpositions.at(a);
						float size = asteroidsize.at(a);
						if (distance(player2position.x -
								 apos.x,
							     player2position.y -
								 apos.y) <
						    size) {
							// Collision!
							if (player2sheild ||
							    clock.getElapsedTime()
									.asSeconds() -
								    player2time <
								SHEILDINVULNERABILITYPERIOD) { // Bounce with sheild
								// Get New Angle
								float movangle =
								    atan2(
									player2velocity
									    .x,
									player2velocity
									    .y);
								float posangle =
								    atan2(
									player2position
										.x -
									    apos.x,
									player2position
										.y -
									    apos.y) -
								    PI /
									2; // Get
									   // Perpendicular
								float newangle =
								    2 * posangle -
								    movangle; // New
									      // angle
									      // after
									      // bounce
								float velocity =
								    distance(
									player2velocity
									    .x,
									player2velocity
									    .y) *
								    SHEILDSPEEDMULTIPLIER;

								// Bounce
								player2velocity
								    .x =
								    sin(newangle) *
								    velocity;
								player2velocity
								    .y =
								    cos(newangle) *
								    velocity;

								// Repel
								player2velocity += sf::Vector2f(
								    (player2position
									 .x -
								     apos.x) /
									asteroidsize
									    .at(a) *
									SHEILDREPELSTRENGTH,
								    (player2position
									 .y -
								     apos.y) /
									asteroidsize
									    .at(a) *
									SHEILDREPELSTRENGTH);

								// Push out if
								// in asteroid
								if (distance(
									player2position
										.x -
									    apos.x,
									player2position
										.y -
									    apos.y) <
								    asteroidsize
									.at(a)) {
									player2position
									    .x +=
									    (player2position
										 .x -
									     apos.x) /
									    asteroidsize
										.at(a) *
									    (asteroidsize
										 .at(a) -
									     distance(
										 player2position
											 .x -
										     apos.x,
										 player2position
											 .y -
										     apos.y));
									player2position
									    .y +=
									    (player2position
										 .y -
									     apos.y) /
									    asteroidsize
										.at(a) *
									    (asteroidsize
										 .at(a) -
									     distance(
										 player2position
											 .x -
										     apos.x,
										 player2position
											 .y -
										     apos.y));
								}
								// Push if in
								// tragectory of
								// asteroid
								float amovangle =
								    atan2(
									asteroidvelocity
									    .at(a)
									    .x,
									asteroidvelocity
									    .at(a)
									    .y);
								if (posangle >=
									amovangle -
									    PI /
										2 &&
								    posangle <=
									amovangle +
									    PI /
										2)
									player2velocity +=
									    asteroidvelocity
										.at(a);
								// Text
								if (player2sheild) {
									texts++;
									textpositions
									    .push_back(
										player2position);
									textstring
									    .push_back(
										SHEILDPOPMESSAGE);
									texttime
									    .push_back(
										0);
								}
								// Pop Sheild
								player2sheild =
								    false;
								powerdownsound
								    .play();
								player2time =
								    clock
									.getElapsedTime()
									.asSeconds();
							} else {
								player2gameover =
								    true;
								// Explosion
								explosionsound
								    .play();
								explosions++;
								explosionpositions
								    .push_back(
									player2position);
								explosiontime
								    .push_back(
									EXPLOSIONTIME);
								explosionsize
								    .push_back(
									PLAYEREXPLOSIONSIZE);

								// Text
								texts++;
								textpositions
								    .push_back(
									player2position);
								textstring.push_back(
								    GAMEOVERMESSAGE);
								texttime
								    .push_back(
									0);
							}
							break;
						}
					}

					// Gameover if lazer hit
					if (player2lazercooldown <
					    LAZERCOOLDOWN -
						LAZERINVULNERABILITYPERIOD)
						for (int l = 0; l < lazers;
						     l++) {
							if (distance(
								lazerpositions
									.at(l)
									.x -
								    player2position
									.x,
								lazerpositions
									.at(l)
									.y -
								    player2position
									.y) <=
							    HITRADIUS) {

								// Delete Lazer
								lazerpositions.erase(
								    lazerpositions
									.begin() +
								    l);
								lazerrotation.erase(
								    lazerrotation
									.begin() +
								    l);
								lazervelocity.erase(
								    lazervelocity
									.begin() +
								    l);
								lazers--;
								l--;

								if (player2sheild ||
								    clock.getElapsedTime()
										.asSeconds() -
									    player2time <
									SHEILDINVULNERABILITYPERIOD) { // Bounce with sheild

									// Text
									if (player2sheild) {
										texts++;
										textpositions
										    .push_back(
											player2position);
										textstring
										    .push_back(
											SHEILDPOPMESSAGE);
										texttime
										    .push_back(
											0);
									}

									// Pop
									// Sheild
									player2sheild =
									    false;
									powerdownsound
									    .play();
									player2time =
									    clock
										.getElapsedTime()
										.asSeconds();
								} else {
									player2gameover =
									    true;
									// Explosion
									explosionsound
									    .play();
									explosions++;
									explosionpositions
									    .push_back(
										player2position);
									explosiontime
									    .push_back(
										EXPLOSIONTIME);
									explosionsize
									    .push_back(
										PLAYEREXPLOSIONSIZE);

									// Text
									texts++;
									textpositions
									    .push_back(
										player2position);
									textstring
									    .push_back(
										GAMEOVERMESSAGE);
									texttime
									    .push_back(
										0);
								}
								break;
							}
						}

					bool bounce = false;
					// Bounce on edge
					if (player2position.x < 0 &&
						player2velocity.x < 0 ||
					    player2position.x >
						    window.getSize().x &&
						player2velocity.x > 0) {
						player2velocity.x *=
						    -1 * EDGEBOUNCESCALE;
						bounce = true;
					}
					if (player2position.y < 0 &&
						player2velocity.y < 0 ||
					    player2position.y >
						    window.getSize().y &&
						player2velocity.y > 0) {
						player2velocity.y *=
						    -1 * EDGEBOUNCESCALE;
						bounce = true;
					}
					if (bounce) {
						// Text
						texts++;
						textpositions.push_back(
						    player2position);
						textstring.push_back(
						    BOUNCEMESSAGE);
						texttime.push_back(0);
					}
					// Draw Player
					sf::ConvexShape player(
					    6); // Triangle shaped player with
						// rectangular tip
					player.setPoint(
					    0, sf::Vector2f(-2, 10)); //     **
					player.setPoint(
					    1, sf::Vector2f(-2, 15)); //     **
					player.setPoint(
					    2, sf::Vector2f(2, 15)); //     **
					player.setPoint(
					    3, sf::Vector2f(2, 10)); //    *  *
					player.setPoint(
					    4, sf::Vector2f(10, -10)); //   * *
					player.setPoint(
					    5, sf::Vector2f(-10,
							    -10)); //  ********
					player.setFillColor(
					    sf::Color::Transparent); // Black
								     // Fill
					player.setOutlineColor(
					    sf::Color::White); // White Outline
					player.setOutlineThickness(2);

					player.setPosition(
					    player2position); // Position
					player.setRotation(
					    player2rotation); // Rotate

					// Draw
					window.draw(player);

					// Draw Fire
					sf::ConvexShape fire(5);
					fire.setPoint(0, sf::Vector2f(-8, -15));
					fire.setPoint(1, sf::Vector2f(-5, -20));
					fire.setPoint(2, sf::Vector2f(0, -18));
					fire.setPoint(3, sf::Vector2f(5, -20));
					fire.setPoint(4, sf::Vector2f(8, -15));

					fire.setFillColor(
					    sf::Color::Transparent);
					fire.setOutlineColor(sf::Color::White);
					fire.setOutlineThickness(2);

					fire.setPosition(
					    player2position); // Position
					fire.setRotation(
					    player2rotation); // Rotate

					if (engine)
						window.draw(fire);

					// Player indicator
					sf::RectangleShape indicator(
					    sf::Vector2f(2, 8));

					indicator.setOrigin(
					    indicator.getLocalBounds().width /
						2,
					    indicator.getLocalBounds().height /
						    2 +
						2); // Shift 2px down from
						    // center
					indicator.setPosition(player2position);
					indicator.setRotation(player2rotation);

					// Draw
					window.draw(indicator);

					// Draw sheild
					float ptime =
					    clock.getElapsedTime().asSeconds() -
					    player2time;
					sf::CircleShape sheild;
					float sheildThickness = 2;
					for (int i = 0; i < player2sheilds;
					     i++) {
						sheild = sf::CircleShape(
						    SHEILDSIZE +
							SHEILDSPACE * i,
						    16);
						sheild.setFillColor(
						    sf::Color::Transparent);
						sheild.setOutlineThickness(
						    sheildThickness);
						sheild.setOutlineColor(
						    sf::Color::White);
						sheild.setOrigin(
						    sheild.getLocalBounds()
							    .width /
							2,
						    sheild.getLocalBounds()
							    .height /
							2);
						sheild.setPosition(
						    player2position);
						window.draw(sheild);
					}
					if (player2sheild) {
						if (ptime >
						    SHEILDANIMATIONLENGTH) {
							sheild = sf::CircleShape(
							    SHEILDSIZE +
								(SHEILDSPACE *
								 player2sheilds),
							    16);
						} else {
							sheild = sf::CircleShape(
							    SHEILDSIZE +
								(SHEILDSPACE *
								 player2sheilds) +
								SHEILDRANGE *
								    ((SHEILDANIMATIONLENGTH -
								      ptime) /
								     SHEILDANIMATIONLENGTH),
							    16);
							sheildThickness =
							    ptime /
							    SHEILDANIMATIONLENGTH *
							    2;
						}
						sheild.setFillColor(
						    sf::Color::Transparent);
						sheild.setOutlineThickness(
						    sheildThickness);
						sheild.setOutlineColor(
						    sf::Color::White);
						sheild.setOrigin(
						    sheild.getLocalBounds()
							    .width /
							2,
						    sheild.getLocalBounds()
							    .height /
							2);
						sheild.setPosition(
						    player2position);
						window.draw(sheild);
					} else {
						if (ptime <=
						    SHEILDANIMATIONLENGTH) {
							sheild = sf::CircleShape(
							    SHEILDSIZE +
								SHEILDSPACE *
								    player2sheilds +
								SHEILDRANGE *
								    (ptime /
								     SHEILDANIMATIONLENGTH),
							    16);
							sheildThickness =
							    (SHEILDANIMATIONLENGTH -
							     ptime) /
							    SHEILDANIMATIONLENGTH *
							    2;
							sheild.setFillColor(
							    sf::Color::
								Transparent);
							sheild
							    .setOutlineThickness(
								sheildThickness);
							sheild.setOutlineColor(
							    sf::Color::White);
							sheild.setOrigin(
							    sheild.getLocalBounds()
								    .width /
								2,
							    sheild.getLocalBounds()
								    .height /
								2);
							sheild.setPosition(
							    player2position);
							window.draw(sheild);
						}
					}
				}

				// Lazers
				for (int i = 0; i < lazers; i++) {
					// Update Positions
					lazerpositions.at(i) +=
					    lazervelocity.at(i) * (deltatime);
					// Render Lazer
					sf::RectangleShape lazer(
					    sf::Vector2f(2, 10));
					lazer.setOrigin(
					    lazer.getLocalBounds().width / 2,
					    lazer.getLocalBounds().height / 2);
					lazer.setPosition(lazerpositions.at(i));
					lazer.setRotation(lazerrotation.at(i));
					lazer.setFillColor(sf::Color::White);
					window.draw(lazer);

					// Delete offscreen lazers
					if (lazerpositions.at(i).x <
						0 - LAZERDESPAWNDISTANCE ||
					    lazerpositions.at(i).x >
						window.getSize().x +
						    LAZERDESPAWNDISTANCE ||
					    lazerpositions.at(i).y <
						0 - LAZERDESPAWNDISTANCE ||
					    lazerpositions.at(i).y >
						window.getSize().y +
						    LAZERDESPAWNDISTANCE) {
						lazerrotation.erase(
						    lazerrotation.begin() + i);
						lazervelocity.erase(
						    lazervelocity.begin() + i);
						lazerpositions.erase(
						    lazerpositions.begin() + i);
						i--;
						lazers--;
						continue;
					}

					sf::Vector2f pos = lazerpositions.at(i);
					// Colide with Asteroids = Split
					// asteroid + 5 point
					for (int a = 0; a < asteroids; a++) {
						sf::Vector2f apos =
						    asteroidpositions.at(a);
						float size = asteroidsize.at(a);
						if (distance(pos.x - apos.x,
							     pos.y - apos.y) <
						    size) { // Pythagorean
							    // Theorem
							// Collision!
							score += ASTEROIDPOINTS;
							// Text
							texts++;
							textpositions.push_back(
							    pos);
							std::stringstream
							    buffer;
							buffer << "+";
							buffer
							    << ASTEROIDPOINTS;
							std::string text =
							    buffer.str();
							textstring.push_back(
							    text);
							texttime.push_back(0);

							float speed = distance(
							    asteroidvelocity
								.at(a)
								.x,
							    asteroidvelocity
								.at(a)
								.y);
							float angle =
							    lazerrotation.at(
								i) *
							    PI / 180;
							sf::Vector2f
							    vel; // Velocity
							vel.x -= sin(angle);
							vel.x *= speed;
							vel.y = cos(angle);
							vel.y *= speed;

							// Split Asteroid
							asteroids++; // 2
								     // Asteroids
								     // appear
								     // from
								     // split -
								     // original
								     // asteroid
							asteroidvelocity.push_back(
							    sf::Vector2f(
								-vel.x,
								vel.y)); // Right
									 // of
									 // lazer
							asteroidvelocity.push_back(
							    sf::Vector2f(
								vel.x,
								-vel.y)); // Left
									  // of
									  // Lazer
							asteroidpositions.push_back(
							    apos); // Both
								   // asteroids
								   // are at
								   // the
								   // original
								   // location
							asteroidpositions
							    .push_back(
								apos); // For
								       // now
							asteroidrotationvelocity.push_back(
							    -asteroidrotationvelocity
								 .at(a)); // Rotate
									  // opposite
									  // the
									  // original
									  // rotation
							asteroidrotationvelocity
							    .push_back(
								-asteroidrotationvelocity
								     .at(a));
							asteroidrotation
							    .push_back(
								0); // Start at
								    // 0 degrees
								    // rotation
							asteroidrotation
							    .push_back(0);
							asteroidsize.push_back(
							    asteroidsize.at(a) /
							    2); // Each asteroid
								// is half the
								// size
							asteroidsize.push_back(
							    asteroidsize.at(a) /
							    2);
							// Delete Original
							// Asteroid
							asteroidpositions.erase(
							    asteroidpositions
								.begin() +
							    a);
							asteroidvelocity.erase(
							    asteroidvelocity
								.begin() +
							    a);
							asteroidrotation.erase(
							    asteroidrotation
								.begin() +
							    a);
							asteroidrotationvelocity
							    .erase(
								asteroidrotationvelocity
								    .begin() +
								a);
							asteroidsize.erase(
							    asteroidsize
								.begin() +
							    a);
							// Create Explosion
							destroysound.play();
							explosions++;
							explosionpositions
							    .push_back(pos);
							explosionsize.push_back(
							    EXPLOSIONSIZE);
							explosiontime.push_back(
							    EXPLOSIONTIME);
							// Delete Lazer
							lazerrotation.erase(
							    lazerrotation
								.begin() +
							    i);
							lazervelocity.erase(
							    lazervelocity
								.begin() +
							    i);
							lazerpositions.erase(
							    lazerpositions
								.begin() +
							    i);
							i--;
							lazers--;
							break;
						}
					}
					// Collide with enemy = defeat enemy +
					// some points
				}
				// Explosions
				// Generate Explosion shape
				sf::CircleShape explosion(
				    10.f);		     // Radius 10 circle
				explosion.setPointCount(16); // 16 Points
				explosion.setFillColor(sf::Color::Transparent);
				explosion.setOutlineColor(sf::Color::White);
				explosion.setOutlineThickness(2);
				for (int i = 0; i < explosions; i++) {
					explosiontime.at(i) -= deltatime;
					explosion.setRadius(
					    explosionsize.at(i) /
					    (EXPLOSIONTIME /
					     explosiontime.at(
						 i))); // Grows to size
					explosion.setPosition(
					    explosionpositions.at(i));
					explosion.setOrigin(
					    explosion.getLocalBounds().width /
						2,
					    explosion.getLocalBounds().height /
						2);
					window.draw(explosion);
					// Delete explosion when timer is done
					if (explosiontime.at(i) <= 0) {
						explosionpositions.erase(
						    explosionpositions.begin() +
						    i);
						explosiontime.erase(
						    explosiontime.begin() + i);
						explosionsize.erase(
						    explosionsize.begin() + i);
						i--;
						explosions--;
					}
				}
				// Asteroids
				// Generate Asteroid Shape (spinning Square, aka
				// 4 point circle)
				for (int i = 0; i < asteroids; i++) {
					// Render
					sf::CircleShape asteroid(
					    asteroidsize.at(i), 4);
					asteroid.setPosition(
					    asteroidpositions.at(i));
					asteroid.setRotation(
					    asteroidrotation.at(i));
					asteroid.setFillColor(
					    sf::Color::Transparent);
					asteroid.setOutlineThickness(2);
					asteroid.setOutlineColor(
					    sf::Color::White);
					asteroid.setOrigin(
					    asteroid.getLocalBounds().width / 2,
					    asteroid.getLocalBounds().height /
						2); // Looks funny when this is
						    // not here
					window.draw(asteroid);

					// Move
					asteroidpositions.at(i) +=
					    asteroidvelocity.at(i) *
					    (deltatime);
					asteroidrotation.at(i) +=
					    asteroidrotationvelocity.at(i) *
					    (deltatime);

					// Bounce on edge of screen
					sf::Vector2f pos =
					    asteroidpositions.at(i);
					sf::Vector2f vel =
					    asteroidvelocity.at(i);
					if ((pos.x > window.getSize().x &&
					     vel.x > 0) ||
					    (pos.x < 0 && vel.x < 0))
						asteroidvelocity.at(i).x *= -1;
					if ((pos.y > window.getSize().y &&
					     vel.y > 0) ||
					    (pos.y < 0 && vel.y < 0))
						asteroidvelocity.at(i).y *= -1;

					// Asteroid bounce physics
					sf::Vector2f tapos =
					    asteroidpositions.at(
						i); // This Asteroid POSition
					sf::Vector2f tavel =
					    asteroidvelocity.at(
						i); // This Asteroid VELocity
					float tasize = asteroidsize.at(i);

					for (int a = 0; a < asteroids; a++) {
						if (a == i)
							continue; // Skip
								  // physics
								  // with itself

						sf::Vector2f apos =
						    asteroidpositions.at(a);
						float size = asteroidsize.at(a);

						if (size < tasize)
							continue; // Skip
								  // smaller
								  // asteroids

						if (distance(tapos.x - apos.x,
							     tapos.y - apos.y) <
						    size +
							tasize) { // Radius
								  // of both
								  // asteroids
								  // added
								  // together

							// Collision

							// Get New Angle
							float movangle = atan2(
							    tavel.x, tavel.y);
							float posangle =
							    atan2(tapos.x -
								      apos.x,
								  tapos.y -
								      apos.y) -
							    PI /
								2; // Get
								   // Perpendicular
							float newangle =
							    2 * posangle -
							    movangle; // New
								      // angle
								      // after
								      // bounce
							float velocity =
							    distance(tavel.x,
								     tavel.y) *
							    ASTEROIDBOUNCESPEEDSCALE;

							// Bounce
							tavel.x =
							    sin(newangle) *
							    velocity;
							tavel.y =
							    cos(newangle) *
							    velocity;

							// Push out of asteroid
							tapos.x +=
							    (tapos.x - apos.x) /
							    (size + tasize) *
							    ((size + tasize) -
							     distance(
								 tapos.x -
								     apos.x,
								 tapos.y -
								     apos.y));
							tapos.y +=
							    (tapos.y - apos.y) /
							    (size + tasize) *
							    ((size + tasize) -
							     distance(
								 tapos.x -
								     apos.x,
								 tapos.y -
								     apos.y));

							// break;
						}
					}

					// Save physics
					asteroidpositions.at(i) = tapos;
					asteroidvelocity.at(i) = tavel;
					// Delete small asteroids
					if (asteroidsize.at(i) <
					    MINASTEROIDSIZE) { // Delete
							       // Asteroid if it
							       // is too small

						// Chance to drop powerup
						if (rand() % POWERUPCHANCE ==
						    0) {
							powerups++;
							poweruppositions
							    .push_back(
								asteroidpositions
								    .at(i));
							if (rand() %
								BOMBCHANCE ==
							    0) {
								poweruptype
								    .push_back(
									BOMB);
							} else {
								poweruptype
								    .push_back(
									SHEILD);
							}
							powerupsize.push_back(
							    0);
							powerupcollected
							    .push_back(0);
						}

						// Delete
						asteroids--;
						asteroidpositions.erase(
						    asteroidpositions.begin() +
						    i);
						asteroidvelocity.erase(
						    asteroidvelocity.begin() +
						    i);
						asteroidrotation.erase(
						    asteroidrotation.begin() +
						    i);
						asteroidrotationvelocity.erase(
						    asteroidrotationvelocity
							.begin() +
						    i);
						asteroidsize.erase(
						    asteroidsize.begin() + i);
						i--;
					}
				}

				// Enemies (Basically the player just not a
				// player) BTW the enemies are squares

				for (int i = 0; i < enemies; i++) {
					// Make variables with enemy info
					sf::Vector2f epos =
					    enemypositions.at(i);
					sf::Vector2f evel = enemyvelocity.at(i);
					float erot = enemyrotation.at(i);
					float ervl =
					    enemyrotationvelocity.at(i);
					float lazercooldown =
					    enemylazercooldown.at(i);

					// Choose which player to target
					int closestplayer =
					    0; // 0: No players alive,
					       // 1: Player1, 2: Player2
					{
						float p1dist = distance(
						    player1position.x - epos.x,
						    player1position.x - epos.x);
						float p2dist = distance(
						    player2position.x - epos.x,
						    player2position.y - epos.y);
						if (!player1gameover &&
						    (p1dist < p2dist ||
						     player2gameover))
							closestplayer = 1;
						else if (player2mode &&
							 !player2gameover)
							closestplayer = 2;
					}

					// Get targeted player position
					sf::Vector2f tpos; // Target pos
					switch (closestplayer) {
					case 1:
						tpos = player1position;
						break;
					case 2:
						tpos = player2position;
						break;
					default: // If All players gameover, go
						 // to center
						tpos = sf::Vector2f(
						    window.getSize().x / 2.f,
						    window.getSize().y / 2.f);
					}

					// Check if player is on the left or
					// right
					float steer =
					    0; // 0: Don't steer, -1: steer
					       // left, 1: steer right
					bool engine =
					    false; // Engine off by default
					bool shoot =
					    false; // Autoshoot off by default
					{
						// Enemy AI steering
						float pi2 = PI * 2;
						float angle = fmod(
						    ((erot + 90) * PI / 180),
						    pi2); // Current Angle
						float want = atan(
						    (epos.y - tpos.y) /
						    (epos.x -
						     tpos.x)); // Wanted angle
						if (tpos.x <= epos.x)
							want += PI;
						float distance = fmod(
						    std::min(
							(float)((2 * PI) -
								(want - angle)),
							(float)(want - angle)),
						    pi2); // Distance from
							  // Current Angle to
							  // Wanted Angle
						if (distance < 0)
							steer = -1;
						if (distance > 0)
							steer = 1;

						// Engine
						if (want > angle - PI / 8 &&
						    want < angle + PI / 8) {
							engine =
							    true; // Turn on
								  // engine if
								  // target is
								  // within 45
								  // degrees of
								  // view
						}
						// Disable Engine if already
						// going fast in that direction
						if (evel.x -
							    sin(erot * PI /
								180) *
								(deltatime) *
								(MOVEMENTVEL) >=
							ENEMYMAXVELOCITY ||
						    evel.x -
							    sin(erot * PI /
								180) *
								(deltatime) *
								(MOVEMENTVEL) <=
							-ENEMYMAXVELOCITY)
							engine = false;
						if (evel.y +
							    cos(erot * PI /
								180) *
								(deltatime) *
								(MOVEMENTVEL) >=
							ENEMYMAXVELOCITY ||
						    evel.y +
							    cos(erot * PI /
								180) *
								(deltatime) *
								(MOVEMENTVEL) <=
							-ENEMYMAXVELOCITY)
							engine = false;

						// Lazer
						if (want > angle - PI / 16 &&
						    want < angle + PI / 16) {
							shoot =
							    true; // Shoot if
								  // player
								  // within 22.5
								  // degrees of
								  // view
						}
					}

					// Process controls

					// Add engine acceleration
					if (engine) {
						evel.x -= sin(erot * PI / 180) *
							  (deltatime) *
							  (MOVEMENTVEL);
						evel.y += cos(erot * PI / 180) *
							  (deltatime) *
							  (MOVEMENTVEL);
					}

					// Steer enemy

					if (steer < 0.f &&
					    ervl > -ENEMYMAXROTATIONVELOCITY)
						ervl -=
						    1 * (deltatime)*ROTATIONVEL;
					else if (steer > 0.f &&
						 ervl <
						     ENEMYMAXROTATIONVELOCITY)
						ervl +=
						    1 * (deltatime)*ROTATIONVEL;

					// Update Lazer Cooldown
					enemylazercooldown.at(i) -= deltatime;
					lazercooldown =
					    enemylazercooldown.at(i);

					// Shoot Lazer
					if (shoot && lazercooldown <= 0) {
						lazerfire.play();
						lazers++;
						lazercooldown = LAZERCOOLDOWN;
						lazerpositions.push_back(epos);
						lazerrotation.push_back(erot);
						lazervelocity.push_back(
						    sf::Vector2f(
							sin(-erot * PI / 180) *
							    LAZERSPEED,
							cos(-erot * PI / 180) *
							    LAZERSPEED));
						lazervelocity.at(
						    lazervelocity.size() - 1) +=
						    evel; // Add enemy velocity
							  // to lazer
						if (distance(
							lazervelocity
							    .at(lazervelocity
								    .size() -
								1)
							    .x,
							lazervelocity
							    .at(lazervelocity
								    .size() -
								1)
							    .y) < LAZERSPEED)
							lazervelocity.at(
							    lazervelocity
								.size() -
							    1) -= evel;
						lazerpositions.at(
						    lazerpositions.size() -
						    1) +=
						    sf::Vector2f(
							sin(-erot * PI / 180),
							cos(-erot * PI / 180)) *
						    (float)
							HITRADIUS; // Get Lazer
								   // out of hit
								   // radius
						// Update Lazer Cooldown
						enemylazercooldown.at(i) =
						    lazercooldown;
					}

					// Update position
					epos += evel * (deltatime);
					erot += ervl * (deltatime);

					// Bounce on edge
					if (epos.x < 0 && evel.x < 0 ||
					    epos.x > window.getSize().x &&
						evel.x > 0) {
						evel.x *= -1 * EDGEBOUNCESCALE;
					}
					if (epos.y < 0 && evel.y < 0 ||
					    epos.y > window.getSize().y &&
						evel.y > 0) {
						evel.y *= -1 * EDGEBOUNCESCALE;
					}

					// Update enemy status
					enemypositions.at(i) = epos;
					enemyvelocity.at(i) = evel;
					enemyrotation.at(i) = erot;
					enemyrotationvelocity.at(i) = ervl;

					// Asteroid destroy enemy
					for (int a = 0; a < asteroids; a++) {
						sf::Vector2f apos =
						    asteroidpositions.at(a);
						float size = asteroidsize.at(a);
						if (distance(epos.x - apos.x,
							     epos.y - apos.y) <
						    size) {
							// Collision

							// Explosion
							destroysound.play();
							explosions++;
							explosionpositions
							    .push_back(epos);
							explosiontime.push_back(
							    EXPLOSIONTIME);
							explosionsize.push_back(
							    PLAYEREXPLOSIONSIZE);

							// Delete Enemy
							enemypositions.erase(
							    enemypositions
								.begin() +
							    i);
							enemyvelocity.erase(
							    enemyvelocity
								.begin() +
							    i);
							enemyrotation.erase(
							    enemyrotation
								.begin() +
							    i);
							enemyrotationvelocity
							    .erase(
								enemyrotationvelocity
								    .begin() +
								i);
							enemylazercooldown.erase(
							    enemylazercooldown
								.begin() +
							    i);
							enemies--;
							i--;

							// Increase Score
							score += ENEMYPOINTS;
							// Text
							texts++;
							textpositions.push_back(
							    epos);
							std::stringstream
							    buffer;
							buffer << "+";
							buffer << ENEMYPOINTS;
							std::string text =
							    buffer.str();
							textstring.push_back(
							    text);
							texttime.push_back(0);

							break;
						}
					}

					// Destroy enemy if lazer hit
					if (lazercooldown <
					    LAZERCOOLDOWN -
						LAZERINVULNERABILITYPERIOD)
						for (int l = 0; l < lazers;
						     l++) {
							if (distance(
								lazerpositions
									.at(l)
									.x -
								    epos.x,
								lazerpositions
									.at(l)
									.y -
								    epos.y) <=
							    HITRADIUS) {

								// Delete Lazer
								lazerpositions.erase(
								    lazerpositions
									.begin() +
								    l);
								lazerrotation.erase(
								    lazerrotation
									.begin() +
								    l);
								lazervelocity.erase(
								    lazervelocity
									.begin() +
								    l);
								lazers--;
								l--;

								// Explosion
								destroysound
								    .play();
								explosions++;
								explosionpositions
								    .push_back(
									epos);
								explosiontime
								    .push_back(
									EXPLOSIONTIME);
								explosionsize
								    .push_back(
									PLAYEREXPLOSIONSIZE);

								// Delete Enemy
								enemypositions.erase(
								    enemypositions
									.begin() +
								    i);
								enemyrotation.erase(
								    enemyrotation
									.begin() +
								    i);
								enemyvelocity.erase(
								    enemyvelocity
									.begin() +
								    i);
								enemyrotationvelocity
								    .erase(
									enemyrotationvelocity
									    .begin() +
									i);
								enemylazercooldown
								    .erase(
									enemylazercooldown
									    .begin() +
									i);
								enemies--;
								i--;

								// Increase
								// Score
								score +=
								    ENEMYPOINTS;
								// Text
								texts++;
								textpositions
								    .push_back(
									epos);
								std::
								    stringstream
									buffer;
								buffer << "+";
								buffer
								    << ENEMYPOINTS;
								std::string text =
								    buffer
									.str();
								textstring
								    .push_back(
									text);
								texttime
								    .push_back(
									0);

								break;
							}
						}

					// Render enemy

					// Draw Enemy
					sf::ConvexShape enemy(
					    8); // Square shaped enemy with
						// rectangular tip
					enemy.setPoint(
					    0, sf::Vector2f(-2, 10)); //     **
					enemy.setPoint(
					    1, sf::Vector2f(-2, 15)); //     **
					enemy.setPoint(
					    2,
					    sf::Vector2f(2, 15)); //  ********
					enemy.setPoint(
					    3, sf::Vector2f(2, 10)); //  * *
					enemy.setPoint(
					    4, sf::Vector2f(10, 10)); //  * *
					enemy.setPoint(
					    5, sf::Vector2f(10, -10)); //  * *
					enemy.setPoint(
					    6, sf::Vector2f(-10,
							    -10)); //  ********
					enemy.setPoint(7,
						       sf::Vector2f(-10, 10));
					enemy.setFillColor(
					    sf::Color::Transparent); // Black
								     // Fill
					enemy.setOutlineColor(
					    sf::Color::White); // White Outline
					enemy.setOutlineThickness(2);

					enemy.setPosition(epos); // Position
					enemy.setRotation(erot); // Rotate

					// Draw
					window.draw(enemy);

					// Draw Fire
					sf::ConvexShape fire(5);
					fire.setPoint(0, sf::Vector2f(-8, -15));
					fire.setPoint(1, sf::Vector2f(-5, -20));
					fire.setPoint(2, sf::Vector2f(0, -18));
					fire.setPoint(3, sf::Vector2f(5, -20));
					fire.setPoint(4, sf::Vector2f(8, -15));

					fire.setFillColor(
					    sf::Color::Transparent);
					fire.setOutlineColor(sf::Color::White);
					fire.setOutlineThickness(2);

					fire.setPosition(epos); // Position
					fire.setRotation(erot); // Rotate

					if (engine)
						window.draw(fire);
				}
				// End of enemy code

				// Texts Renderer
				for (int i = 0; i < texts; i++) {
					sf::Text text(textstring.at(i).c_str(),
						      roboto,
						      TEXTSIZE); // Size 20 text
					sf::Vector2f pos = textpositions.at(i);
					pos.y += TEXTRISE *
						 (texttime.at(i) /
						  TEXTTIME); // Rise animation
					text.setPosition(pos);
					text.setOrigin(
					    text.getLocalBounds().width / 2,
					    text.getLocalBounds().height / 2);
					text.setScale(
					    (TEXTTIME - texttime.at(i)) /
						TEXTTIME,
					    (TEXTTIME - texttime.at(i)) /
						TEXTTIME); // Shrink animation

					// Make sure it is on screen
					sf::Vector2f top;
					top.y = text.getGlobalBounds().top;
					top.x = text.getGlobalBounds().left;
					sf::Vector2f bot;
					bot.y = text.getGlobalBounds().top +
						text.getGlobalBounds().height;
					bot.x = text.getGlobalBounds().left +
						text.getGlobalBounds().width;
					sf::Vector2f win = sf::Vector2f(
					    (float)window.getSize().x,
					    (float)window.getSize().y);
					if (top.y < 0)
						text.move(0, -top.y);
					if (top.x < 0)
						text.move(-top.x, 0);
					if (bot.y > win.y)
						text.move(0, win.y - bot.y);
					if (bot.x > win.x)
						text.move(win.x - bot.x, 0);

					text.setFillColor(sf::Color::White);
					window.draw(text);

					// Update timer
					texttime.at(i) += deltatime;
					if (texttime.at(i) > TEXTTIME) {
						texts--;
						textpositions.erase(
						    textpositions.begin() + i);
						textstring.erase(
						    textstring.begin() + i);
						texttime.erase(
						    texttime.begin() + i);
						i--;
					}
				}

				// Powerups
				for (int i = 0; i < powerups; i++) {
					// Scale
					if (powerupcollected.at(i) == false) {
						if (powerupsize.at(i) < 1)
							powerupsize.at(i) +=
							    (1.f /
							     POWERSCALETIME) *
							    deltatime;
						else
							powerupsize.at(i) = 1;
					} else {
						if (powerupsize.at(i) > 0)
							powerupsize.at(i) -=
							    (1.f /
							     POWERSCALETIME) *
							    deltatime;
						else
							powerupsize.at(i) = 0;
					}

					sf::Vector2f pos =
					    poweruppositions.at(i);
					int collect = 0;
					if (distance(pos.x - player1position.x,
						     pos.y -
							 player1position.y) <=
						COLLECTRADIUS ||
					    poweruptype.at(i) == PLANET &&
						distance(
						    pos.x - player1position.x,
						    pos.y -
							player1position.y) <=
						    PLANET_SIZE) {
						collect = 1;
					} else if (distance(
						       pos.x -
							   player2position.x,
						       pos.y -
							   player2position.y) <=
						       COLLECTRADIUS ||
						   poweruptype.at(i) ==
							   PLANET &&
						       distance(
							   pos.x -
							       player2position
								   .x,
							   pos.y -
							       player2position
								   .y) <=
							   PLANET_SIZE) {
						collect = 2;
					}
					if (powerupsize.at(i) != 1)
						collect = 0; // Only collectable
							     // at full size
					sf::CircleShape radius;
					if (poweruptype.at(i) != PLANET)
						radius = sf::CircleShape(
						    COLLECTRADIUS *
							powerupsize.at(i),
						    16);
					else
						radius = sf::CircleShape(
						    PLANET_SIZE *
							powerupsize.at(i),
						    16);
					radius.setFillColor(
					    sf::Color::Transparent);
					radius.setOutlineThickness(2);
					radius.setOutlineColor(
					    sf::Color::White);
					radius.setOrigin(
					    radius.getLocalBounds().width / 2,
					    radius.getLocalBounds().height / 2);
					radius.setPosition(pos);
					window.draw(radius);
					if (powerupcollected.at(i) == false)
						switch (poweruptype.at(i)) {
						case SHEILD: {
							sf::CircleShape sheild =
							    sf::CircleShape(
								COLLECTRADIUS /
								    2.f *
								    powerupsize
									.at(i),
								16);
							sheild.setFillColor(
							    sf::Color::
								Transparent);
							sheild
							    .setOutlineThickness(
								2);
							sheild.setOutlineColor(
							    sf::Color::White);
							sheild.setOrigin(
							    sheild.getLocalBounds()
								    .width /
								2,
							    sheild.getLocalBounds()
								    .height /
								2);
							sheild.setPosition(pos);
							window.draw(sheild);
							if (collect == 1) {
								if (player1sheild)
									player1sheilds++;
								else
									player1sheild =
									    true;
								player1time =
								    clock
									.getElapsedTime()
									.asSeconds();
							} else if (collect ==
								   2) {
								if (player2sheild)
									player2sheilds++;
								else
									player2sheild =
									    true;
								player2time =
								    clock
									.getElapsedTime()
									.asSeconds();
							}

							if (collect != 0) {
								powerupsound
								    .play();
								// Text
								texts++;
								textpositions
								    .push_back(
									pos);
								texttime
								    .push_back(
									0);
								textstring.push_back(
								    COLLECTSHEILDMESSAGE);
							}
						} break;
						case BOMB: {
							sf::CircleShape bomb =
							    sf::CircleShape(
								COLLECTRADIUS /
								    2.f *
								    powerupsize
									.at(i),
								4);
							bomb.setOutlineThickness(
							    2);
							bomb.setOutlineColor(
							    sf::Color::White);
							bomb.setFillColor(
							    sf::Color::
								Transparent);
							bomb.setOrigin(
							    bomb.getLocalBounds()
								    .width /
								2,
							    bomb.getLocalBounds()
								    .height /
								2);
							bomb.setPosition(pos);
							window.draw(bomb);
							if (collect != 0) {
								bombs++;
								powerupsound
								    .play();
								// Text
								texts++;
								textpositions
								    .push_back(
									pos);
								texttime
								    .push_back(
									0);
								textstring.push_back(
								    COLLECTBOMBMESSAGE);
							}
						} break;
						case PLANET: {
							// Minigame Planet
							sf::CircleShape crater1 =
							    sf::CircleShape(
								PLANET_SIZE *
								    powerupsize
									.at(i) /
								    4,
								16);
							crater1
							    .setOutlineThickness(
								2);
							crater1.setOutlineColor(
							    sf::Color::White);
							crater1.setFillColor(
							    sf::Color::
								Transparent);
							crater1.setOrigin(
							    crater1.getLocalBounds()
								    .width /
								2,
							    crater1.getLocalBounds()
								    .height /
								2);
							crater1.setPosition(
							    pos +
							    sf::Vector2f(
								-PLANET_SIZE *
								    powerupsize
									.at(i) /
								    3,
								PLANET_SIZE *
								    powerupsize
									.at(i) /
								    2));
							window.draw(crater1);
							sf::CircleShape crater2 =
							    sf::CircleShape(
								PLANET_SIZE *
								    powerupsize
									.at(i) /
								    3,
								16);
							crater2
							    .setOutlineThickness(
								2);
							crater2.setOutlineColor(
							    sf::Color::White);
							crater2.setFillColor(
							    sf::Color::
								Transparent);
							crater2.setOrigin(
							    crater2.getLocalBounds()
								    .width /
								2,
							    crater2.getLocalBounds()
								    .height /
								2);
							crater2.setPosition(
							    pos -
							    sf::Vector2f(
								PLANET_SIZE *
								    powerupsize
									.at(i) /
								    3,
								PLANET_SIZE *
								    powerupsize
									.at(i) /
								    3));
							window.draw(crater2);
							sf::CircleShape crater3 =
							    sf::CircleShape(
								PLANET_SIZE *
								    powerupsize
									.at(i) /
								    5,
								16);
							crater3
							    .setOutlineThickness(
								2);
							crater3.setOutlineColor(
							    sf::Color::White);
							crater3.setFillColor(
							    sf::Color::
								Transparent);
							crater3.setOrigin(
							    crater3.getLocalBounds()
								    .width /
								2,
							    crater3.getLocalBounds()
								    .height /
								2);
							crater3.setPosition(
							    pos +
							    sf::Vector2f(
								PLANET_SIZE *
								    powerupsize
									.at(i) /
								    2,
								0));
							window.draw(crater3);

							// Enter Planet
							if (collect != 0 &&
							    !powerupcollected
								 .at(i)) {
								powerupcollected
								    .at(i) =
								    true;
								planetexists =
								    false;
								// Text
								texts++;
								textpositions
								    .push_back(
									pos);
								textstring
								    .push_back(
									"Miniga"
									"me "
									"Planet"
									"!");
								texttime
								    .push_back(
									0);
								// Minigame
								nextminigame =
								    1;
								planetanimtime =
								    0;
							}
						} break;
						}

					if (collect != 0) {
						powerupcollected.at(i) = true;
					}

					if (powerupcollected.at(i) &&
					    powerupsize.at(i) == 0) {
						powerups--;
						poweruppositions.erase(
						    poweruppositions.begin() +
						    i);
						poweruptype.erase(
						    poweruptype.begin() + i);
						powerupsize.erase(
						    powerupsize.begin() + i);
						powerupcollected.erase(
						    powerupcollected.begin() +
						    i);
						i--;
					}
				}

				// Bomb Cooldown
				bombcooldown -= deltatime;

				// Check for game over
				if ((player1gameover && player2gameover) ||
				    (player1gameover && !player2mode)) {
					float animationTime =
					    clock.getElapsedTime().asSeconds() -
					    extraDelayClock;
					if (animationTime > GAMEOVERTIME) {
						clock.restart();
						finalscore = 0;
						screen = 7;
						// Test Highscores
						if (player2mode) {
							int totalscore =
							    score +
							    bombs * BOMBSCORE;
							if (totalscore >
							    highscores
								.twoplayerhighscore) {
								newhighscore =
								    true;
								highscores
								    .twoplayerhighscore =
								    totalscore;
								writehighscore(
								    highscores);
							}
						} else {
							int totalscore =
							    score +
							    bombs * BOMBSCORE;
							if (totalscore >
							    highscores
								.highscore) {
								newhighscore =
								    true;
								highscores
								    .highscore =
								    totalscore;
								writehighscore(
								    highscores);
							}
						}
					} // After GAMEOVERTIME seconds go to
					  // game over screen
				} else {
					extraDelayClock =
					    clock.getElapsedTime()
						.asSeconds(); // Keep
							      // extraDelayClock
							      // up to date
							      // until game over
				}

				// Fire Bomb
				if (sf::Keyboard::isKeyPressed(
					sf::Keyboard::Space) &&
				    bombcooldown <= 0 && bombs > 0) {
					bombexplosionsound.play();
					bombcooldown = BOMBCOOLDOWN;
					bombs--;

					// Clear Screen
					asteroids = 0;
					asteroidpositions.clear();
					asteroidrotation.clear();
					asteroidrotationvelocity.clear();
					asteroidvelocity.clear();
					asteroidsize.clear();
					// Enemies
					enemies = 0;
					enemypositions.clear();
					enemyvelocity.clear();
					enemyrotation.clear();
					enemyrotationvelocity.clear();
					enemylazercooldown.clear();
					// Powerups
					powerups = 0;
					poweruppositions.clear();
					poweruptype.clear();
					powerupsize.clear();
					// Lazers
					lazers = 0;
					lazerpositions.clear();
					lazerrotation.clear();
					lazervelocity.clear();
				}
			} break;
			default: { // Minigame Planets
#define PLANET_CODE
#include "planets.cpp"		   // Hacky way to reduce the size of this file
			}
			}

			// Animate Bomb (Flash on screen)
			if (bombcooldown > 0) {
				sf::RectangleShape bombexplosion =
				    sf::RectangleShape(
					sf::Vector2f(window.getSize().x,
						     window.getSize().y));
				sf::Color color = sf::Color::White;
				color.a = bombcooldown / BOMBCOOLDOWN * 256;
				bombexplosion.setFillColor(color);
				bombexplosion.setPosition(sf::Vector2f(0, 0));
				window.draw(bombexplosion);
			}

			// Animate Minigame Switch
			if (nextminigame != minigame) {
				planetanimtime += deltatime;
				if (planetanimtime >= PLANETANIMLENGTH) {
					minigame = nextminigame;
					clock.restart();
				}
				// Fade Animation
				sf::RectangleShape fade = sf::RectangleShape(
				    sf::Vector2f(window.getSize().x,
						 window.getSize().y));
				sf::Color color = sf::Color::Black;
				color.a =
				    (planetanimtime / PLANETANIMLENGTH) * 255;
				fade.setFillColor(color);
				fade.setPosition(sf::Vector2f(0, 0));
				window.draw(fade);
			}

		} break;
		// Game Over
		case 7: {
			// Reset Tabbing
			tab = false;
			tabused = false;
			// Title
			sf::Text title("Game Over", roboto, 80);
			if (newhighscore)
				title.setString("New Highscore!");
			// Center Title, 10% from top
			sf::FloatRect titlesize = title.getLocalBounds();
			title.setPosition(sf::Vector2f(
			    window.getSize().x / 2.f - titlesize.width / 2.f,
			    window.getSize().y * 0.1));
			title.setFillColor(sf::Color::White);
			window.draw(title);
			// Score
			std::stringstream scorebuffer;
			scorebuffer << "Score: " << score;
			std::string scorestring = scorebuffer.str();
			sf::Text scoretext(scorestring.c_str(), roboto, 80);
			// Center Score, 30% from top
			sf::FloatRect scoresize = scoretext.getLocalBounds();
			scoretext.setPosition(sf::Vector2f(
			    window.getSize().x / 2.f - scoresize.width / 2.f,
			    window.getSize().y * 0.3));
			scoretext.setFillColor(sf::Color::White);
			window.draw(scoretext);
			// Bombs
			std::stringstream bombbuffer;
			bombbuffer << "Bombs: " << bombs;
			std::string bombstring = bombbuffer.str();
			sf::Text bombtext(bombstring.c_str(), roboto, 80);
			// Center Bomb, 40% from top
			sf::FloatRect bombsize = scoretext.getLocalBounds();
			bombtext.setPosition(sf::Vector2f(
			    window.getSize().x / 2.f - bombsize.width / 2.f,
			    window.getSize().y * 0.4));
			bombtext.setFillColor(sf::Color::White);
			window.draw(bombtext);
			// Final Score
			std::stringstream finalscorebuffer;
			finalscorebuffer << "Final Score: " << finalscore;
			std::string finalscorestring = finalscorebuffer.str();
			sf::Text finalscoretext(finalscorestring.c_str(),
						roboto, 80);
			// Center Final Score, 60% from top
			sf::FloatRect finalscoresize =
			    finalscoretext.getLocalBounds();
			finalscoretext.setPosition(
			    sf::Vector2f(window.getSize().x / 2.f -
					     finalscoresize.width / 2.f,
					 window.getSize().y * 0.6));
			finalscoretext.setFillColor(sf::Color::White);
			window.draw(finalscoretext);
			// Animate Score
			if (clock.getElapsedTime().asSeconds() >
			    SCOREANIMATIONSTARTTIME) {
				if (scoretime < SCOREANIMATIONDELAY) {
					scoretime += deltatime;
				} else {
					if (score > 0) {
						scorecountsound.play();
						finalscore++;
						score--;
						if (newhighscore &&
						    bombs == 0 && score == 0)
							highscoresound.play();
					} else if (bombs > 0) {
						scorecountsound.play();
						finalscore += BOMBSCORE;
						bombs--;
						if (newhighscore && bombs == 0)
							highscoresound.play();
					}
					scoretime = 0;
				}
			}
			// Play Button
			sf::RectangleShape playbutton(sf::Vector2f(400, 100));
			// Center Play Button, 75% from top
			sf::FloatRect buttonsize = playbutton.getLocalBounds();
			playbutton.setPosition(sf::Vector2f(
			    window.getSize().x / 2.f - buttonsize.width / 2.f,
			    window.getSize().y * 0.75));
			// Detect Mouse Colision
			sf::FloatRect playhitbox = playbutton.getGlobalBounds();
			if (enter ||
			    playhitbox.contains(window.mapPixelToCoords(
				sf::Mouse::getPosition(window)))) {
				// Outline On Hover
				playbutton.setOutlineColor(sf::Color::White);
				playbutton.setOutlineThickness(10);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					// Black On Press
					playbutton.setFillColor(
					    sf::Color::Black);
				if (click || enter) {
					buttonclicksound.play();
					screen = 2; // Player Select
					clock.restart();
				}
			} else
				// White Otherwise
				playbutton.setFillColor(sf::Color::White);
			window.draw(playbutton);
			// Play Button Text
			sf::Text playtext("Play Again", roboto, 50);
			// Center Text In Button
			sf::FloatRect textsize = playtext.getLocalBounds();
			playtext.setPosition(sf::Vector2f(
			    window.getSize().x / 2.f - textsize.width / 2.f,
			    (playhitbox.top + playhitbox.height / 2.f) -
				textsize.height / 2.f));
			if (playbutton.getFillColor() != sf::Color::White)
				playtext.setFillColor(sf::Color::White);
			else
				playtext.setFillColor(sf::Color::Black);
			window.draw(playtext);
			// Tip Text
			sf::Text tiptext("Press [esc] To Exit", roboto, 30);
			// Center Tip Text, 10% from bottom
			sf::FloatRect tipsize = tiptext.getLocalBounds();
			tiptext.setPosition(
			    window.getSize().x / 2.f - tipsize.width / 2.f,
			    window.getSize().y - window.getSize().y * 0.1);
			tiptext.setFillColor(sf::Color::White);
			window.draw(tiptext);
		} break;
		default: {
			sf::Text error("Error: Screen Does Not Exist!!!",
				       roboto, 60);
			error.setFillColor(sf::Color::Red);
			window.draw(error);
			if (clock.getElapsedTime().asSeconds() > 10) {
				screen = 0;
				clock.restart();
			}
		}
		}

		// Tooltip
		if (showtooltip) {
			if (!tabused)
				tooltipvec = window.mapPixelToCoords(
				    sf::Mouse::getPosition(window));
			else
				tooltipvec =
				    sf::Vector2f(window.getSize().x - 200,
						 window.getSize().y - 200);
			sf::Text text(tooltiptext, roboto, 40);
			text.setPosition(tooltipvec);
			text.move(20, 20);
			sf::RectangleShape box(
			    sf::Vector2f(text.getLocalBounds().width + 40,
					 text.getLocalBounds().height + 40));
			box.setPosition(tooltipvec);
			text.setFillColor(sf::Color::White);
			box.setFillColor(sf::Color::Black);
			box.setOutlineThickness(10);
			box.setOutlineColor(sf::Color::White);
			text.move(-box.getLocalBounds().width,
				  -box.getLocalBounds().height);
			box.move(-box.getLocalBounds().width,
				 -box.getLocalBounds().height);
			window.draw(box);
			window.draw(text);
		}

		// Display Window
		window.display();

		// Update Delay Clock
		delayClock = clock.getElapsedTime().asSeconds();

		// Reset Click
		click = false;
	}
	return EXIT_SUCCESS;
}
