// planets.cpp (Minigames)

#ifdef PLANET_FUNC
#undef PLANET_FUNC
// Functions and Classes for Planets Go Here

// Function that selects a frame of a vertical spritesheet
sf::IntRect spritesheet(sf::Texture tex, unsigned int frameheight, unsigned int id, bool flip = false) {
	sf::IntRect ret;
	sf::Vector2u size = tex.getSize();
	ret.left = 0;
	ret.width = size.x;
	ret.height = frameheight;
	ret.top = id*frameheight;

	// Flip
	if (flip) {
		ret.left = ret.width;
		ret.width = -ret.width;
	}

	return ret;
}
// Returns TextureRect for current animation frame
sf::IntRect animateframe(sf::Texture tex, unsigned int frames, unsigned int fps, float time, bool flip = false, float start = 0) {
	unsigned int frame = (unsigned int)((time-start)*fps) % frames;
	sf::Vector2u size = tex.getSize();
	unsigned int frameheight = size.y/frames;
	return spritesheet(tex,frameheight,frame,flip);
}
// Tests if the animation has played at least once
bool animationdone(unsigned int frames, unsigned int fps, float time, float start = 0) {
	unsigned int tick = (unsigned int)((time-start)*fps);
	if ( tick >= frames )
		return true;
	return false;
}

#include "grid.hpp"	// Map Grid
#include "tilemap.hpp"	// Tilemap Renderer

// Deltatime Vector
sf::Vector2f smooth(float deltatime, sf::Vector2f vec) {
	vec.x * deltatime;
	vec.y * deltatime;
	return vec;
}
	

// Check if position collides with block and return block id (id=0 = air)
uint8_t checkblock(Grid<uint8_t>* map, float blocksize, sf::Vector2f pos) {
	unsigned int x = pos.x/blocksize;
	unsigned int y = pos.y/blocksize;
	return map->get(x,y);
}

float drag(float value, float drag) {
	if (value < 0) value += drag;
	if (value > 0) value -= drag;
	if (value < drag && value > -drag) value = 0;	// Close Enough
	return value;
}

float setng(float value, float threshold) {	// SET if Not Greater
	if (threshold > 0 && value < threshold) value = threshold;
	else if (threshold < 0 && value > threshold) value = threshold;
	return value;
}

#endif

#ifdef PLANET_ASSETS
#undef PLANET_ASSETS
// Load Assets (Textures/Sound/Etc) for Planet Minigames

// Mario ( 'm_' prefix )
sf::Texture m_landinganim;	// Spaceship landing animation
if (!m_landinganim.loadFromFile("textures/landing.anim.png"))	// '.anim.png' for spritesheet animations
	std::cout << "Falied to Load Texture 'landing.anim'!" << std::endl;
// Player Assets
sf::Texture m_playerwalk0anim;
if (!m_playerwalk0anim.loadFromFile("textures/playerwalk0.anim.png"))
	std::cout << "Failed to Load Texture 'playerwalk0.anim'!" << std::endl;
sf::Texture m_playerwalk1anim;
if (!m_playerwalk1anim.loadFromFile("textures/playerwalk1.anim.png"))
	std::cout << "Failed to Load Texture 'playerwalk1.anim'!" << std::endl;
sf::Texture m_player0;
if (!m_player0.loadFromFile("textures/player0.png"))
	std::cout << "Failed to Load Texture 'player0'!" << std::endl;
sf::Texture m_player1;
if (!m_player1.loadFromFile("textures/player1.png"))
	std::cout << "Failed to Load Texture 'player1'!" << std::endl;
// Enemy Assets
sf::Texture m_enemywalkanim;
if (!m_enemywalkanim.loadFromFile("textures/enemywalk.anim.png"))
	std::cout << "Failed to Load Texture 'enemywalk.anim'!" << std::endl;

// Game Map
Grid<uint8_t> m_map;
{
	std::ifstream file("assets/mario.map.txt");
	if (file.is_open()) {
		std::string line;
		unsigned int linenumber = 0;
		while (getline(file, line)) {
			std::string block;
			unsigned int blocknumber = 0;
			std::stringstream linebuffer(line);
			while (getline(linebuffer, block, ' ')) {
				m_map.set(blocknumber, linenumber, (unsigned int) std::stoul(block,nullptr,0) );	// Read Number and convert to unsigned long and then to unsigned int
				blocknumber++;
			}
			linenumber++;
		}
	}
	else std::cout << "Failed to Load Map 'mario.map'!" << std::endl;
	file.close();
}




// End Mario

#endif

#ifdef PLANET_VARS
#undef PLANET_VARS
// Global Variables for Planets Go Here

// Minigame 1 Mario Style Moving and Combat Variables ( 'm_' prefix )
// Visual
float m_offset = 0;
float m_scale = 8;
sf::Vector2f m_playersize(5*m_scale,9*m_scale);
float m_blockwidth = 11;
float m_blocksize = m_blockwidth*m_scale;
// Physic
float m_speed = 100*m_scale;
float m_airspeed = 75*m_scale;
float m_jump = 200.f*m_scale;
float m_gravity = 10.f*m_scale;
float m_drag = 400*m_scale;
float m_friction = 600*m_scale;
// Players
sf::Vector2f m_p0pos;
sf::Vector2f m_p0vel;
bool m_p0land = false;
sf::Vector2f m_p1pos;
sf::Vector2f m_p1vel;
bool m_p1land = false;
// Enemies
std::vector<sf::Vector2f> m_enemypos;
std::vector<sf::Vector2f> m_enemyvel;
// Level
TileMap m_level;
m_level.load("textures/blocks.png",sf::Vector2u(11,11), m_scale, window.getSize().y, m_map);

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
		animation.setTextureRect(animateframe(m_landinganim,22,13,time)); // 22 frames long at 13 fps using animation 'm_landinganim'

		// Move on if done
		if (animationdone(22,13,time)) {
			animation.setTextureRect(animateframe(m_landinganim,22,1,21)); // Freeze on last frame
			nextminigame = 3;
			if (planetanimtime >= PLANETANIMLENGTH) planetanimtime = 0;
		}

		window.draw(animation);

		message = "";
		tiptext = "";
} break;

case 3: {	// Mario Mode
		// Player Controls
		// Player 0
		{
			int direction = 0;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || (!player2mode && sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ) ) direction -= 1; 

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || (!player2mode && sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ) ) direction += 1;
			if (m_p0land) m_p0vel.x = setng(m_p0vel.x, direction*m_speed);
			else m_p0vel.x = setng(m_p0vel.x, direction*m_airspeed);

			if ( m_p0land && ( sf::Keyboard::isKeyPressed(sf::Keyboard::W) || (!player2mode && sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ) ) ) m_p0vel.y = m_jump;
			
		}
		// Player 1
		{
			int direction = 0;
			if ( player2mode && sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ) direction -= 1; 

			if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ) direction += 1;
			if (m_p1land) m_p1vel.x = setng(m_p1vel.x, direction*m_speed);
			else m_p1vel.x = setng(m_p1vel.x, direction*m_airspeed);

			if (m_p1land && player2mode && sf::Keyboard::isKeyPressed(sf::Keyboard::Up) )  m_p1vel.y = m_jump;
		}

		// Add Half of Gravity
		m_p0vel.y += (-pow(m_gravity,2.f))*deltatime*0.5f;
		m_p1vel.y += (-pow(m_gravity,2.f))*deltatime*0.5f;
		// Add Drag
		m_p0vel.x = drag(m_p0vel.x, m_drag*deltatime);
		m_p1vel.x = drag(m_p1vel.x, m_drag*deltatime);
		// Add Friction
		if (m_p0land) m_p0vel.x = drag(m_p0vel.x, m_friction*deltatime);
		if (m_p1land) m_p1vel.x = drag(m_p1vel.x, m_friction*deltatime);
		
		// Move Players
		{	// Player 0
			m_p0land = false;
			sf::Vector2f future = m_p0pos;
			sf::Vector2f movement = m_p0vel;

			future.x += movement.x * deltatime;
			// Collision Checks X
			if (	checkblock(&m_map, m_blocksize, future) != 0 ||
				future.x < 0 || 
				checkblock(&m_map, m_blocksize, sf::Vector2f(future.x + m_playersize.x, future.y ) ) != 0 ||
				checkblock(&m_map, m_blocksize, sf::Vector2f(future.x + m_playersize.x, future.y + m_playersize.y ) ) != 0 ||
				checkblock(&m_map, m_blocksize, sf::Vector2f(future.x, future.y + m_playersize.y ) ) != 0
			) {
				future.x = m_p0pos.x;	// Revert if collision
				m_p0vel.x = 0;	// Cancel X momentum
			}

			// Collision Checks Y
			future.y += movement.y * deltatime;
			
			if (	checkblock(&m_map, m_blocksize, future) != 0 ||
				future.y < 0 || 
				checkblock(&m_map, m_blocksize, sf::Vector2f(future.x + m_playersize.x, future.y ) ) != 0 ||
				checkblock(&m_map, m_blocksize, sf::Vector2f(future.x + m_playersize.x, future.y + m_playersize.y ) ) != 0 ||
				checkblock(&m_map, m_blocksize, sf::Vector2f(future.x, future.y + m_playersize.y ) ) != 0
			) {
				future.y = m_p0pos.y;	// Revert if collision
				m_p0vel.y = 0;	// Cancel X momentum
				if (movement.y < 0) m_p0land = true;	// Only lands if going down
			}

			m_p0pos = future;
		}

		{	// Player 1
			m_p1land = false;
			sf::Vector2f future = m_p1pos;
			sf::Vector2f movement = m_p1vel;
			
			// Collision Checks X
			future.x += movement.x * deltatime;
			if (	checkblock(&m_map, m_blocksize, future) != 0 ||
				future.x < 0 || 
				checkblock(&m_map, m_blocksize, sf::Vector2f(future.x + m_playersize.x, future.y ) ) != 0 ||
				checkblock(&m_map, m_blocksize, sf::Vector2f(future.x + m_playersize.x, future.y + m_playersize.y ) ) != 0 ||
				checkblock(&m_map, m_blocksize, sf::Vector2f(future.x, future.y + m_playersize.y ) ) != 0
			) {
				future.x = m_p1pos.x;	// Revert if collision
				m_p1vel.x = 0;	// Cancel X momentum
			}

			// Collision Checks Y
			future.y += movement.y * deltatime;
			
			if (	checkblock(&m_map, m_blocksize, future) != 0 ||
				future.y < 0 || 
				checkblock(&m_map, m_blocksize, sf::Vector2f(future.x + m_playersize.x, future.y ) ) != 0 ||
				checkblock(&m_map, m_blocksize, sf::Vector2f(future.x + m_playersize.x, future.y + m_playersize.y ) ) != 0 ||
				checkblock(&m_map, m_blocksize, sf::Vector2f(future.x, future.y + m_playersize.y ) ) != 0
			) {
				future.y = m_p1pos.y;	// Revert if collision
				m_p1vel.y = 0;	// Cancel X momentum
				if (movement.y < 0) m_p1land = true;	// Only lands if going down
			}

			m_p1pos = future;
		}
		
		// Add Other Half of Gravity
		m_p0vel.y += (-pow(m_gravity,2.f))*deltatime*0.5f;
		m_p1vel.y += (-pow(m_gravity,2.f))*deltatime*0.5f;

		// Scroll Camera
		{
			float future = m_offset;
			if (m_p0pos.x > m_p1pos.x) future = m_p0pos.x - 1*(windowsize.x/2);
			if (m_p1pos.x > m_p0pos.x) future = m_p1pos.x - 1*(windowsize.x/2);
			if (future > 0) m_offset = future;
		}
		// Draw Players
		// Player 0
		if (!player1gameover) {
			sf::Sprite player0;
			if (m_p0vel.x == 0) player0.setTexture(m_player0);
			else {
				player0.setTexture(m_playerwalk0anim);
				if (m_p0vel.x >= 0) player0.setTextureRect(animateframe(m_playerwalk0anim,6,12,time,false));	// 6 Frames, 12 FPS, Not Flipped
				else player0.setTextureRect(animateframe(m_playerwalk0anim,6,12,time,true));	// 6 Frames, 12 FPS, Flipped
			}
			player0.setScale(m_scale,m_scale); // m_scale times Real Size
			player0.setPosition(m_p0pos.x-m_offset,windowsize.y-m_p0pos.y-m_playersize.y);
			// Draw Player
			window.draw(player0);
		}

		// Player 1
		if (player2mode && !player2gameover) {
			sf::Sprite player1;
			if (m_p1vel.x == 0) player1.setTexture(m_player1);
			else {
				player1.setTexture(m_playerwalk1anim);
				if (m_p1vel.x >= 0) player1.setTextureRect(animateframe(m_playerwalk1anim,6,12,time,false));	// 6 Frames, 12 FPS, Not Flipped
				else player1.setTextureRect(animateframe(m_playerwalk1anim,6,12,time,true));	// 6 Frames, 12 FPS, Flipped
			}
			player1.setScale(m_scale,m_scale); // m_scale times Real Size
			player1.setPosition(m_p1pos.x-m_offset,windowsize.y-m_p1pos.y-m_playersize.y);
			// Draw Player
			window.draw(player1);
		}

		// Draw Level
		//m_level.setPosition(0-m_offset,0);
		window.draw(m_level);
		
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
