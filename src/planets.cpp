// planets.cpp (Minigames)

#ifdef PLANET_FUNC
#undef PLANET_FUNC
// Functions for Planets Go Here

// Returns TextureRect for current animation frame
sf::IntRect animateframe(sf::Texture tex, unsigned int frames, unsigned int fps, float time, float start = 0) {
	sf::IntRect ret;
	sf::Vector2u size = tex.getSize();
	unsigned int frameheight = size.y/frames;
	ret.left = 0;
	ret.width = size.x;
	ret.height = frameheight;
	unsigned int tick = (unsigned int)((time-start)*fps) % frames;
	ret.top = tick/frameheight;
	return ret;
}
// Tests if the animation has played at least once
bool animationdone(unsigned int frames, unsigned int fps, float time, float start = 0) {
	unsigned int frame = (time-start)*fps;
	if ( frame > frames )
		return true;
	return false;
}

#endif

#ifdef PLANET_ASSETS
#undef PLANET_ASSETS
// Load Assets (Textures/Sound/Etc) for Planet Minigames

// Mario ( 'm_' prefix )
sf::Texture m_landinganim;	// Spaceship landing animation
if (!m_landinganim.loadFromFile("textures/landing.anim.png")) {	// '.anim.png' for spritesheet animations
	std::cout << "Falied to Load Texture 'landing.anim'!" << std::endl;
}
// End Mario

#endif

#ifdef PLANET_VARS
#undef PLANET_VARS
// Global Variables for Planets Go Here

// Minigame 1 Mario Style Moving and Combat Variables ( 'm_' prefix )
// Players
sf::Vector2f m_p1pos;
sf::Vector2f m_p1vel;
sf::Vector2f m_p2pos;
sf::Vector2f m_p2vel;
// Enemies
std::vector<sf::Vector2f> m_enemypos;
std::vector<sf::Vector2f> m_enemyvel;

// End Mario

#endif

#ifdef PLANET_CODE
#undef PLANET_CODE
// Code for Planets Goes Here

// Set Message Text
std::string message = "Bob the fish"; // Default Text

// Set Tiptext
std::string tiptext = "Bob is a very good fish"; // Default Text

// Reusable text sprite
sf::Text text = sf::Text("",roboto,50);

// Locations
sf::Vector2f windowsize = sf::Vector2f(window.getSize().x,window.getSize().y);

// Time in seconds
float time = clock.getElapsedTime().asSeconds();

// If Both Players Gameover Minigame = 0
if (player1gameover && (player2mode && player2gameover)) minigame = 0;

switch (minigame) {

case 1: { // Mario Mode Controls
	message = "Controls";
	tiptext = "Press [Enter] or Click to Continue";
	text.setString("Movement: [a][d] or [<][>]");
	text.setOrigin(text.getLocalBounds().width/2,text.getLocalBounds().height/2);
	text.setPosition(windowsize.x*0.5,windowsize.y*0.30);	// 30% from top centered
	window.draw(text);
	text.setString("Jump: [w] or [^]");
	text.setOrigin(text.getLocalBounds().width/2,text.getLocalBounds().height/2);
	text.setPosition(windowsize.x*0.5,windowsize.y*0.50);	// 50% from top centered
	window.draw(text);
	text.setString("Attack: [s] or [v]");
	text.setOrigin(text.getLocalBounds().width/2,text.getLocalBounds().height/2);
	text.setPosition(windowsize.x*0.5,windowsize.y*0.70);	// 70% from top centered
	window.draw(text);

	// Continue
	if (click || enter) {
		minigame = 2;
		nextminigame = 2;	// Skip animation
		clock.restart();
	}
} break;

case 2: {	// Animation of landing
		sf::RectangleShape animation = sf::RectangleShape(sf::Vector2f(windowsize));
		animation.setTexture(&m_landinganim);
		animation.setTextureRect(animateframe(m_landinganim,25,12,time)); // 25 frames long at 12 fps using animation 'm_landinganim'
		window.draw(animation);

		// Move on if done
		if (animationdone(10,12,clock.getElapsedTime().asSeconds())) minigame = 3;
} break;

case 3: {	// Mario Mode
	// Player Controls
	message = "Mario!";
	tiptext = "TODO";
} break;

default:  // Minigame does not exist
	minigame = 0;
}

// Bob the messenger
sf::Text bob = sf::Text(message, roboto, 60);
bob.setOrigin(bob.getLocalBounds().width/2,bob.getLocalBounds().height/2);	// Center
bob.setPosition(window.getSize().x/2, window.getSize().y*0.05); // 5% from top
window.draw(bob);

// Tip the text
sf::Text tip = sf::Text(tiptext, roboto, 30);
tip.setOrigin(tip.getLocalBounds().width/2,tip.getLocalBounds().height/2);
tip.setPosition(windowsize.x/2,windowsize.y*0.9);	// 10% from bottom
window.draw(tip);

#endif
