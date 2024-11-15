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

sf::IntRect sidesheet(unsigned int framesize, uint8_t frame) {
	sf::IntRect ret;
	ret.left = frame*framesize;
	ret.top = 0;
	ret.width = framesize;
	ret.height = framesize;
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

sf::Vector2u real2tile(sf::Vector2f realpos, float blocksize) {
	sf::Vector2u ret;
	ret.x = realpos.x/blocksize;
	ret.y = realpos.y/blocksize;
	return ret;
}

sf::Vector2f tile2real(sf::Vector2u tilepos, float blocksize) {
	sf::Vector2f ret;
	ret.x = tilepos.x*blocksize;
	ret.y = tilepos.y*blocksize;
	return ret;
}

// Check if position collides with block and return block id (id=0 = air)
uint8_t checkblock(Grid<uint8_t>* map, float blocksize, sf::Vector2f pos) {
	sf::Vector2u tile = real2tile(pos, blocksize);
	return map->get(tile.x,tile.y);
}

bool checkcollision(uint8_t id) {
	switch (id) {
		case 1:	// Block
			return true;
		default:
			return false;
	}
};

float trianglesign(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3) {
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool trianglecollision(sf::Vector2f pt, sf::Vector2f v1, sf::Vector2f v2, sf::Vector2f v3) {
	
	float d1 = trianglesign(pt, v1, v2);
	float d2 = trianglesign(pt, v2, v3);
	float d3 = trianglesign(pt, v3, v1);

	bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

	return !(has_neg && has_pos);
}
bool subrectcollision(sf::Vector2f pt, sf::FloatRect rect) {
	return rect.contains(pt);
}

bool checkdamage(Grid<uint8_t>* map, float blocksize, float blockwidth, sf::Vector2f pos) {
	uint8_t id = checkblock(map, blocksize, pos);
	sf::Vector2u bpos = real2tile(pos, blocksize);
	sf::Vector2f rpos(pos.x/blocksize*blockwidth - bpos.x*blockwidth, pos.y/blocksize*blockwidth+0.1f - bpos.y*blockwidth);
	// Square Collision but Triangle Collision With Spikes
	switch (id) {
		case 2:	// Up Facing Spike
			if (
				trianglecollision(rpos,
					sf::Vector2f(2,0),
					sf::Vector2f(9,0),
					sf::Vector2f(6,6)
				)
			) return true;
			else return false;
		case 3:	// Down Facing Spike
			if (
				trianglecollision(rpos,
					sf::Vector2f(2,11),
					sf::Vector2f(9,11),
					sf::Vector2f(6,6)
				)
			) return true;
			else return false;
		case 4:	// Right Facing Spike
			if (
				trianglecollision(rpos,
					sf::Vector2f(0,2),
					sf::Vector2f(0,9),
					sf::Vector2f(6,6)
				)
			) return true;
			else return false;
		case 5:	// Left Facing Spike
			if (
				trianglecollision(rpos,
					sf::Vector2f(11,2),
					sf::Vector2f(11,9),
					sf::Vector2f(6,6)
				)
			) return true;
			else return false;
		default:
			return false;
	}
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

struct Enemy {
	sf::Vector2f pos;
	sf::Vector2f vel = sf::Vector2f(0,0);
	bool left = false;
	float squishtimer;
	bool squished = false;
};

enum TileEntityTypes {	// Tiles on the map that change are tile entities
	CHEST = 6,		// Chest, ID = 6
	OPENCHEST = 7,		// OpenChest, ID = 7
	LABERINTHDOOR = 8,	// LaberinthDoor, ID = 8
	COIN = 9,		// Coin, ID = 9
};

bool istileentity(uint8_t id) {
	switch(id) {
		case 6:
			return true;
		case 7:
			return true;
		case 8:
			return true;
		case 9:
			return true;
		default:
			return false;
	}
}

struct TileEntity {
	sf::Vector2u pos;
	TileEntityTypes type;
};

enum InteractResultTypes {
	NOTHING,
	SCORE,
	LIFE,
	LABERINTH,
};

struct InteractResult {
	InteractResultTypes type;
	int level;
};

InteractResult tileinteract(std::vector<TileEntity>* tiles, float blocksize, sf::Vector2f pos) {
	InteractResult res;
	res.type = NOTHING;
	sf::Vector2u intpos = real2tile(pos, blocksize);
	int id = 0;
	for (TileEntity tile : *tiles) if (tile.pos.x == intpos.x && tile.pos.y == intpos.y) {
		switch (tile.type) {
			case CHEST:
				tiles->at(id).type = OPENCHEST;
				res.type = LIFE;
				res.level = 1;
				break;
			case LABERINTHDOOR:
				tiles->at(id).type = static_cast<TileEntityTypes>(0);
				res.type = LABERINTH;
				break;
			case COIN:
				tiles->at(id).type = OPENCHEST;
				res.type = SCORE;
				res.level = 5;
			default:
				break;
		}
		break;
	} else id++;
	return res;
}

struct YayText {
	std::string text;
	sf::Vector2f pos;
	float initsize = 35;
	float size = initsize;
	float animtime = 0;
	float animlength = 1;
	float risedist = 150;
};

void yaytext(sf::RenderWindow* window, sf::Font* font, float deltatime, float offset, std::vector<YayText>* texts) {
	for (int i = 0; i < texts->size(); i++) {
		texts->at(i).animtime += deltatime;
		texts->at(i).pos.y += (texts->at(i).risedist / texts->at(i).animlength) * deltatime;
		texts->at(i).size -= (texts->at(i).initsize / texts->at(i).animlength) * deltatime;

		if (texts->at(i).animtime >= texts->at(i).animlength) {
			texts->erase(texts->begin()+i);
			i--;
			continue;
		}

		sf::Text text(texts->at(i).text, *font, texts->at(i).size);
		text.setOrigin(text.getLocalBounds().width/2,text.getLocalBounds().height/2);
		text.setPosition(texts->at(i).pos.x - offset, window->getSize().y - texts->at(i).pos.y);
		window->draw(text);
	}
};

// Laberinth Functions
#define LABERINTH_FUNC
#include "laberinth.cpp"

#endif

#ifdef PLANET_ASSETS
#undef PLANET_ASSETS

// Load Assets (Textures/Sound/Etc) for Planet Minigames

// Ship Land Animation
sf::SoundBuffer whooshbuffer;
if (!whooshbuffer.loadFromFile("sounds/whoosh.wav")) {
	std::cout << "Failed to Load The Whoosh Sound!" << std::endl;
	ok = false;
}
sf::Sound whoosh;
whoosh.setBuffer(whooshbuffer);

// Mario ( 'm_' prefix )
sf::Texture m_landinganim;	// Spaceship landing animation
if (!m_landinganim.loadFromFile("textures/landing.anim.png")) {	// '.anim.png' for spritesheet animations
	std::cout << "Falied to Load Texture 'landing.anim'!" << std::endl;
	ok = false;
}
// Player Assets
sf::Texture m_playerwalk0anim;
if (!m_playerwalk0anim.loadFromFile("textures/playerwalk0.anim.png")) {
	std::cout << "Failed to Load Texture 'playerwalk0.anim'!" << std::endl;
	ok = false;
}
sf::Texture m_playerwalk1anim;
if (!m_playerwalk1anim.loadFromFile("textures/playerwalk1.anim.png")) {
	std::cout << "Failed to Load Texture 'playerwalk1.anim'!" << std::endl;
	ok = false;
}
sf::Texture m_player0;
if (!m_player0.loadFromFile("textures/player0.png")) {
	std::cout << "Failed to Load Texture 'player0'!" << std::endl;
	ok = false;
}
sf::Texture m_playergameover0;
if (!m_playergameover0.loadFromFile("textures/playergameover0.png")) {
	std::cout << "Failed to Load Texture 'playergameover0'!" << std::endl;
	ok = false;
}
sf::Texture m_player1;
if (!m_player1.loadFromFile("textures/player1.png")) {
	std::cout << "Failed to Load Texture 'player1'!" << std::endl;
	ok = false;
}
sf::Texture m_playergameover1;
if (!m_playergameover1.loadFromFile("textures/playergameover1.png")) {
	std::cout << "Failed to Load Texture 'playergameover1'!" << std::endl;
	ok = false;
}
// Enemy Assets
sf::Texture m_enemywalkanim;
if (!m_enemywalkanim.loadFromFile("textures/enemywalk.anim.png")) {
	std::cout << "Failed to Load Texture 'enemywalk.anim'!" << std::endl;
	ok = false;
}
sf::Texture m_enemysquish;
if (!m_enemysquish.loadFromFile("textures/enemysquish.png")) {
	std::cout << "Failed to Load Texture 'enemysquish.png'" << std::endl;
	ok = false;
}

// Sounds
sf::SoundBuffer m_jumpsoundbuffer;
if (!m_jumpsoundbuffer.loadFromFile("sounds/jump.wav")) {
	std::cout << "Failed to load sound effect 'jump'!" << std::endl;
	ok = false;
}
sf::Sound m_jumpsound;
m_jumpsound.setBuffer(m_jumpsoundbuffer);
m_jumpsound.setLoop(false);

// Block Texture
sf::Texture m_blocktexture;
if (!m_blocktexture.loadFromFile("textures/blocks.png")) {
	std::cout << "Failed to load texture 'blocks'!" << std::endl;
	ok = false;
}

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
				if (block == "") continue;	// Skip empty values
				m_map.set(blocknumber, linenumber, (unsigned int) std::stoul(block,nullptr,0) );	// Read Number and convert to unsigned long and then to unsigned int
				blocknumber++;
			}
			linenumber++;
		}
	}
	else {
		std::cout << "Failed to Load Map 'mario.map'!" << std::endl;
		ok = false;
	}
	file.close();
}




// End Mario

// Laberinth Assets
#define LABERINTH_ASSET
#include "laberinth.cpp"

#endif

#ifdef PLANET_VARS
#undef PLANET_VARS
// Laberinth Variables
#define LABERINTH_VARS
#include "laberinth.cpp"
// Global Variables for Planets Go Here
bool whooshed = false;
// Remember to copy values to PLANET_RESET
// Minigame 1 Mario Style Moving and Combat Variables ( 'm_' prefix )
// Visual
std::vector<YayText> m_yaytext;
float m_gameoveranimtime = 5.f;	// 5 second fade
float m_offset = 0;
float m_scale = window.getSize().y/128;
sf::Vector2f m_playersize(5*m_scale,9*m_scale);
sf::Vector2f m_enemysize(7*m_scale,7*m_scale);
float m_blockwidth = 11;
float m_blocksize = m_blockwidth*m_scale;
float m_damageblinkrate = 15;	// 15 blinks per second
// Timing
float m_damagetime = 2.f;	// 2 seconds of damage immunity (if you have more lives)
// Physic
float m_speed = 100*m_scale;
float m_airspeed = 75*m_scale;
float m_airtime = 0.05f;	// 0.05 seconds until not able to jump
float m_enemyspeed = 50*m_scale;
float m_jump = 200.f*m_scale;
float m_hitjump = m_jump / 2;	// Defeat enemy bounce
float m_gravity = 10.f*m_scale;
float m_drag = 400*m_scale;
float m_friction = 600*m_scale;
unsigned int m_initlives = 2;	// Extra lives: 2 = 3 total
// Players
unsigned int m_p0lives = m_initlives;
bool m_p0gameover = false;
float m_p0damagetime = 100;
float m_p0gameovertime = 0;
unsigned int m_p1lives = m_initlives;
bool m_p1gameover = false;
float m_p1damagetime = 100;
float m_p1gameovertime = 0;
sf::Vector2f m_p0pos;
sf::Vector2f m_p0vel;
bool m_p0interact = false;
bool m_p1interact = false;
bool m_p0land = false;
long double m_p0airtime = 0;
sf::Vector2f m_p1pos;
bool m_p1land = false;
long double m_p1airtime = 0;
sf::Vector2f m_p1vel;
// Enemies
std::vector<Enemy> m_enemies;
// Load in enemies
for (unsigned int x = 0; x < m_map.getsize().x; x++) for (unsigned int y = 0; y < m_map.getsize().y; y++) if (m_map.get(x,y) == 255) {	// 255 on map = enemy && air
	Enemy enemy;
	enemy.pos = sf::Vector2f(x*m_blocksize,y*m_blocksize);
	m_enemies.push_back(enemy);
	m_map.set(x,m_map.getsize().y-1-y,(uint8_t)0);	// Set to air
}
std::vector<Enemy> m_initenemies = m_enemies;
// Tile Entities (Like Chests)
std::vector<TileEntity> m_tileentities;
// Load tile entities
for (unsigned int x = 0; x < m_map.getsize().x; x++) for (unsigned int y = 0; y < m_map.getsize().y; y++) if (istileentity(m_map.get(x,y)) != 0) {	// 255 on map = enemy && air
	TileEntity tileentity;
	tileentity.type = (TileEntityTypes) m_map.get(x,y);
	tileentity.pos = sf::Vector2u(x,y);
	m_tileentities.push_back(tileentity);
	m_map.set(x,m_map.getsize().y-1-y,(uint8_t)0);	// Set to air
}
std::vector<TileEntity> m_inittileentities = m_tileentities;
// Level
Grid m_initmap = m_map;
TileMap m_level;
m_level.load("textures/blocks.png",sf::Vector2u(11,11), m_scale, window.getSize().y, &m_map);
TileMap m_initlevel = m_level;
// End Mario

#endif

#ifdef PLANET_RESET
#undef PLANET_RESET
// Laberinth Reset
#define LABERINTH_RESET
#include "laberinth.cpp"
// Whoosh
whooshed = false;
// Begin Mario
// Visual
m_yaytext.clear();
m_offset = 0;
// Players
m_p0lives = m_initlives;
m_p0gameover = false;
m_p0damagetime = 100;
m_p0gameovertime = 0;
m_p1lives = m_initlives;
m_p1gameover = false;
m_p1damagetime = 100;
m_p1gameovertime = 0;
m_p0pos = sf::Vector2f(0,0);
m_p0vel = sf::Vector2f(0,0);
m_p0interact = false;
m_p1interact = false;
m_p0land = false;
m_p0airtime = 0;
m_p1pos = sf::Vector2f(0,0);
m_p1land = false;
m_p1airtime = 0;
m_p1vel = sf::Vector2f(0,0);
// Enemies
// Re-Load in enemies
m_enemies = m_initenemies;
m_tileentities = m_inittileentities;
m_map = m_initmap;
m_level = m_initlevel;
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

switch (minigame) {

case 1: { // Mario Mode Controls
	// Copy Gameover Vars
	m_p0gameover = player1gameover;
	m_p1gameover = player2gameover;
	//
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
	text.setString("Interact: [s] or [v]");
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
	if (whooshed == false) {
		whoosh.play();
		whooshed = true;
	}
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
		if (!m_p0land) m_p0airtime += deltatime;
		else m_p0airtime = 0;
		if (!m_p1land) m_p1airtime += deltatime;
		else m_p1airtime = 0;
		// Player 0
		if (!m_p0gameover) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || (!player2mode && sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) ) m_p0interact = true;
			else m_p0interact = false;
			int direction = 0;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || (!player2mode && sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ) ) direction -= 1; 

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || (!player2mode && sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ) ) direction += 1;
			if (m_p0land) m_p0vel.x = setng(m_p0vel.x, direction*m_speed);
			else m_p0vel.x = setng(m_p0vel.x, direction*m_airspeed);

			if ( (m_p0land || m_p0airtime <= m_airtime) && ( sf::Keyboard::isKeyPressed(sf::Keyboard::W) || (!player2mode && sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ) ) ) {
				m_p0vel.y = m_jump;
				m_p0airtime = 10;
				m_jumpsound.play();
			}
			
		}
		// Player 1
		if (!m_p1gameover && player2mode) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) m_p1interact = true;
			else m_p1interact = false;
			int direction = 0;
			if ( player2mode && sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ) direction -= 1; 

			if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ) direction += 1;
			if (m_p1land) m_p1vel.x = setng(m_p1vel.x, direction*m_speed);
			else m_p1vel.x = setng(m_p1vel.x, direction*m_airspeed);

			if ( (m_p1land || m_p1airtime <= m_airtime) && player2mode && sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ) {
				m_p1vel.y = m_jump;
				m_p1airtime = 10;
				m_jumpsound.play();
			}
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
			
			// Collision Checks X
			future.x += movement.x * deltatime;
			if (	checkcollision(checkblock(&m_map, m_blocksize, future) ) ||
				future.x < 0 || 
				checkcollision(checkblock(&m_map, m_blocksize, sf::Vector2f(future.x + m_playersize.x, future.y ) ) ) ||
				checkcollision(checkblock(&m_map, m_blocksize, sf::Vector2f(future.x + m_playersize.x, future.y + m_playersize.y ) ) ) ||
				checkcollision(checkblock(&m_map, m_blocksize, sf::Vector2f(future.x, future.y + m_playersize.y ) ) )
			) {
				future.x = m_p0pos.x;	// Revert if collision
				m_p0vel.x = 0;	// Cancel X momentum
			}

			// Collision Checks Y
			future.y += movement.y * deltatime;
			
			if (	checkcollision(checkblock(&m_map, m_blocksize, future) ) ||
				future.y < 0 || 
				checkcollision(checkblock(&m_map, m_blocksize, sf::Vector2f(future.x + m_playersize.x, future.y ) ) ) ||
				checkcollision(checkblock(&m_map, m_blocksize, sf::Vector2f(future.x + m_playersize.x, future.y + m_playersize.y ) ) ) ||
				checkcollision(checkblock(&m_map, m_blocksize, sf::Vector2f(future.x, future.y + m_playersize.y ) ) )
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
			if (	checkcollision(checkblock(&m_map, m_blocksize, future) ) ||
				future.x < 0 || 
				checkcollision(checkblock(&m_map, m_blocksize, sf::Vector2f(future.x + m_playersize.x, future.y ) ) ) ||
				checkcollision(checkblock(&m_map, m_blocksize, sf::Vector2f(future.x + m_playersize.x, future.y + m_playersize.y ) ) ) ||
				checkcollision(checkblock(&m_map, m_blocksize, sf::Vector2f(future.x, future.y + m_playersize.y ) ) )
			) {
				future.x = m_p1pos.x;	// Revert if collision
				m_p1vel.x = 0;	// Cancel X momentum
			}

			// Collision Checks Y
			future.y += movement.y * deltatime;
			
			if (	checkcollision(checkblock(&m_map, m_blocksize, future) ) ||
				future.y < 0 || 
				checkcollision(checkblock(&m_map, m_blocksize, sf::Vector2f(future.x + m_playersize.x, future.y ) ) ) ||
				checkcollision(checkblock(&m_map, m_blocksize, sf::Vector2f(future.x + m_playersize.x, future.y + m_playersize.y ) ) ) ||
				checkcollision(checkblock(&m_map, m_blocksize, sf::Vector2f(future.x, future.y + m_playersize.y ) ) )
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
			if (player2mode && !m_p0gameover && !m_p1gameover) {
				if (m_p0pos.x > m_p1pos.x) future = m_p0pos.x - 1*(windowsize.x/2);
				if (m_p1pos.x > m_p0pos.x) future = m_p1pos.x - 1*(windowsize.x/2);
				if (m_p0pos.x < future) future -= future - m_p0pos.x;
				if (m_p1pos.x < future) future -= future - m_p1pos.x;
			}
			else {
				if (!player2mode) future = m_p0pos.x - 1*(windowsize.x/2);
				else if (!m_p0gameover) future = m_p0pos.x - 1*(windowsize.x/2);
				else if (!m_p1gameover) future = m_p1pos.x - 1*(windowsize.x/2);
				else if (m_p0gameovertime < m_p1gameovertime) future = m_p0pos.x - 1*(windowsize.x/2);
				else if (m_p0gameovertime > m_p1gameovertime) future = m_p1pos.x - 1*(windowsize.x/2);
			}
			// Round the future
			future = std::roundf(future);
			// 
			if (future > 0) m_offset = future;

		}
		// Draw Players
		// Player 0
		m_p0damagetime += deltatime;
		if ( (int)(m_p0damagetime * m_damageblinkrate)%2 == 0 || m_p0damagetime > m_damagetime ) {
		       	if (!m_p0gameover) {
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
			} else {
				sf::Sprite player0;
				player0.setTexture(m_playergameover0);
				player0.setScale(m_scale,m_scale); // m_scale times Real Size
				player0.setPosition(m_p0pos.x-m_offset,windowsize.y-m_p0pos.y-m_playersize.x);
				// Draw Gameover Player
				window.draw(player0);
			}
		}

		// Player 1
		m_p1damagetime += deltatime;
		if ( (int)(m_p1damagetime * m_damageblinkrate)%2 == 0 || m_p1damagetime > m_damagetime ) {
			if (player2mode && !m_p1gameover) {
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
			} else if (player2mode) {
				sf::Sprite player1;
				player1.setTexture(m_playergameover1);
				player1.setScale(m_scale,m_scale); // m_scale times Real Size
				player1.setPosition(m_p1pos.x-m_offset,windowsize.y-m_p1pos.y-m_playersize.x);
				// Draw Gameover Player
				window.draw(player1);
			}
		}

		// Move and Draw enemies
		for (unsigned int i = 0; i < m_enemies.size(); i++) {
			Enemy enemy = m_enemies.at(i);
			// Velocity
			if (enemy.squished) enemy.vel.x = 0;
			else if (enemy.left) enemy.vel.x = -m_enemyspeed;
			else enemy.vel.x = m_enemyspeed;
			// Half of Gravity
			enemy.vel.y += (-pow(m_gravity,2.f))*deltatime*0.5f;

			// Move Enemy
			{
				sf::Vector2f future = enemy.pos;
				sf::Vector2f movement = enemy.vel;
				
				// Collision Checks X
				future.x += movement.x * deltatime;
				if (	checkcollision(checkblock(&m_map, m_blocksize, future) ) ||
					future.x < 0 || 
					checkcollision(checkblock(&m_map, m_blocksize, sf::Vector2f(future.x + m_enemysize.x, future.y ) ) ) ||
					checkcollision(checkblock(&m_map, m_blocksize, sf::Vector2f(future.x + m_enemysize.x, future.y + m_enemysize.y ) ) ) ||
					checkcollision(checkblock(&m_map, m_blocksize, sf::Vector2f(future.x, future.y + m_enemysize.y ) ) )
				) {
					future.x = enemy.pos.x;	// Revert if collision
					enemy.left = !enemy.left;
				}

				// Collision Checks Y
				future.y += movement.y * deltatime;
				
				if (	checkcollision(checkblock(&m_map, m_blocksize, future) ) ||
					future.y < 0 || 
					checkcollision(checkblock(&m_map, m_blocksize, sf::Vector2f(future.x + m_enemysize.x, future.y ) ) ) ||
					checkcollision(checkblock(&m_map, m_blocksize, sf::Vector2f(future.x + m_enemysize.x, future.y + m_enemysize.y ) ) ) ||
					checkcollision(checkblock(&m_map, m_blocksize, sf::Vector2f(future.x, future.y + m_enemysize.y ) ) )
				) {
					future.y = enemy.pos.y;	// Revert if collision
					enemy.vel.y = 0;	// Cancel X momentum
				}

				enemy.pos = future;
			}

			// Other Half of Gravity
			enemy.vel.y += (-pow(m_gravity,2.f))*deltatime*0.5f;

			// Draw Enemy
			sf::Sprite senemy;
			if (!enemy.squished) {
				senemy.setTexture(m_enemywalkanim);
				senemy.setTextureRect(animateframe(m_enemywalkanim,4,12,time,enemy.left));	// 4 Frames, 12 FPS, Flipped if needed
			}
			else {
				senemy.setTexture(m_enemysquish);
				senemy.setOrigin(1,0);
			}
			// Set Scale
			senemy.setScale(m_scale,m_scale); // m_scale times Real Size
			senemy.setPosition(enemy.pos.x-m_offset,windowsize.y-enemy.pos.y-senemy.getGlobalBounds().height);
			// Draw Player
			window.draw(senemy);

			if (!enemy.squished) {
				// Check Player Collision
				sf::FloatRect hitenemy = senemy.getGlobalBounds();
				sf::FloatRect player;
				player.width = m_playersize.x;
				player.height = m_playersize.y;
				// Player 0
				player.top = windowsize.y-m_p0pos.y-m_playersize.y;
				player.left = m_p0pos.x-m_offset;
				if (!m_p0land && hitenemy.intersects(player) && m_p0vel.y < 0) {
					enemy.squished = true;
					m_p0vel.y = m_hitjump;	// Boing
					score += 15;
					// YayText
					YayText text;
					text.text = "+15 Score!";
					text.pos = m_p0pos;
					text.pos.y -= m_playersize.y / 2;
					text.pos.x += m_playersize.x / 2;
					m_yaytext.push_back(text);
				}
				else if (m_p0damagetime > m_damagetime && hitenemy.intersects(player)) {
					m_p0damagetime = 0;
					if (m_p0lives == 0) m_p0gameover = true;
					else m_p0lives--;
				}
				// Player 1
				player.top = windowsize.y-m_p1pos.y-m_playersize.y;
				player.left = m_p1pos.x-m_offset;
				if (!m_p1land && hitenemy.intersects(player) && m_p1vel.y < 0) {
					enemy.squished = true;
					m_p1vel.y = m_hitjump;	// Boing
					score += 15;
					// YayText
					YayText text;
					text.text = "+15 Score!";
					text.pos = m_p0pos;
					text.pos.y -= m_playersize.y / 2;
					text.pos.x += m_playersize.x / 2;
					m_yaytext.push_back(text);
				}
				else if (m_p1damagetime > m_damagetime && hitenemy.intersects(player)) {
					m_p1damagetime = 0;
					if (m_p1lives == 0) m_p1gameover = true;
					else m_p1lives--;
				}
			}
			
			// Update Enemy
			m_enemies.at(i) = enemy;
		}

		// Player Block Damage
		// Player 0
		if (m_p0damagetime > m_damagetime) {
			if (
				checkdamage(&m_map, m_blocksize, m_blockwidth, m_p0pos) ||
				checkdamage(&m_map, m_blocksize, m_blockwidth, sf::Vector2f(m_p0pos.x + m_playersize.x, m_p0pos.y) ) ||
				checkdamage(&m_map, m_blocksize, m_blockwidth, sf::Vector2f(m_p0pos.x, m_p0pos.y + m_playersize.y) ) ||
				checkdamage(&m_map, m_blocksize, m_blockwidth, m_p0pos + m_playersize )
			) {
				m_p0damagetime = 0;
				if (m_p0lives == 0) m_p0gameover = true;
				else m_p0lives--;
			}
		}
		// Player 1
		if (m_p1damagetime > m_damagetime) {
			if (
				checkdamage(&m_map, m_blocksize, m_blockwidth, m_p1pos) ||
				checkdamage(&m_map, m_blocksize, m_blockwidth, sf::Vector2f(m_p1pos.x + m_playersize.x, m_p1pos.y) ) ||
				checkdamage(&m_map, m_blocksize, m_blockwidth, sf::Vector2f(m_p1pos.x, m_p1pos.y + m_playersize.y) ) ||
				checkdamage(&m_map, m_blocksize, m_blockwidth, m_p1pos + m_playersize )
			) {
				m_p1damagetime = 0;
				if (m_p1lives == 0) m_p1gameover = true;
				else m_p1lives--;
			}
		}

		// Draw Level
		m_level.setPosition(0-m_offset,0);
		window.draw(m_level);
		
		// Interact With Tile Entities From Player Center
		{
			sf::Vector2f p0pos = m_p0pos + sf::Vector2f(m_playersize.x/2,m_playersize.y/2);
			if (m_p0interact) {
				InteractResult interact = tileinteract(&m_tileentities, m_blocksize, p0pos);
				switch (interact.type) {
					case LIFE:
						if (player2mode && m_p1gameover) {
							m_p1damagetime = -2;
							m_p1gameover = false;
							m_p1lives += interact.level;

							// Yaytext
							YayText text;
							text.pos = p0pos;
							text.text = "Player 2 Revived!";
							m_yaytext.push_back(text);
						}
						else {
							m_p0lives += interact.level;

							// Yaytext
							YayText text;
							text.pos = p0pos;
							std::stringstream lifecount;
							lifecount << "+" << interact.level << " Lives!";
							text.text = lifecount.str();
							m_yaytext.push_back(text);
						}
						break;
					case SCORE: {
						score += interact.level;
						
						// Yaytext
						YayText text;
						text.pos = p0pos;
						std::stringstream scorecount;
						scorecount << "+" << interact.level << " Score!";
						text.text = scorecount.str();
						m_yaytext.push_back(text);
						} break;
					case LABERINTH: {	// Fall into laberinth
						nextminigame = 4;
						minigame = 4;
						player1gameover = m_p0gameover;
						player2gameover = m_p1gameover;
						
						// Yaytext
						YayText doortext;
						doortext.pos = p0pos;
						doortext.text = "The Door Slowly Opens";
						m_yaytext.push_back(doortext);
						} break;
				}
			}
			sf::Vector2f p1pos = m_p1pos + sf::Vector2f(m_playersize.x/2,m_playersize.y/2);
			if (m_p1interact) {
				InteractResult interact = tileinteract(&m_tileentities, m_blocksize, p1pos);
				switch (interact.type) {
					case LIFE:
						if (player2mode && m_p0gameover) {
							m_p0damagetime = 0;
							m_p0gameover = false;
							m_p0lives += interact.level;
							
							// Yaytext
							YayText text;
							text.pos = p1pos;
							text.text = "Player 1 Revived!";
							m_yaytext.push_back(text);
						}
						else {
							m_p1lives += interact.level;
							
							// Yaytext
							YayText text;
							text.pos = p1pos;
							std::stringstream lifecount;
							lifecount << "+" << interact.level << " Lives!";
							text.text = lifecount.str();
							m_yaytext.push_back(text);
						}
						break;
					case SCORE: {
						score += interact.level;
						
						// Yaytext
						YayText text;
						text.pos = p1pos;
						std::stringstream scorecount;
						scorecount << "+" << interact.level << " Score!";
						text.text = scorecount.str();
						m_yaytext.push_back(text);
						} break;
					case LABERINTH:	{ // Fall into laberinth
						nextminigame = 4;
						minigame = 4;
						player1gameover = m_p0gameover;
						player2gameover = m_p1gameover;
						
						// Yaytext
						YayText doortext;
						doortext.pos = p1pos;
						doortext.text = "The Door Slowly Opens";
						m_yaytext.push_back(doortext);
						} break;
				}
			}
		}
		// Draw Tile Entities
		for (TileEntity tile : m_tileentities) {
			sf::RectangleShape render(sf::Vector2f(m_blocksize,m_blocksize));
			render.setTexture(&m_blocktexture);
			render.setTextureRect(sidesheet(11.f, (uint8_t)tile.type));
			sf::Vector2f pos = tile2real(tile.pos, m_blocksize);
			pos.y = windowsize.y - pos.y - m_blocksize;
			pos.x -= m_offset;
			render.setPosition(pos);
			window.draw(render);
		}

		// Draw Yaytext
		yaytext(&window, &roboto, deltatime, m_offset, &m_yaytext);

		message = "";
		tiptext = "";

		// Update Gameover time
		if (m_p0gameover) m_p0gameovertime += deltatime;
		if (m_p1gameover) m_p1gameovertime += deltatime;
		// Continue if both players gameover after fade
		if ( (m_p0gameover && !player2mode) || (m_p0gameover && m_p1gameover) ) {
			extraDelayClock = time-5.f;	// Skip other animation
			float gt = m_p0gameovertime;
			if (player2mode && m_p1gameovertime < gt) gt = m_p1gameovertime;
			sf::RectangleShape fade((sf::Vector2f)windowsize);
			sf::Color fill = sf::Color::Black;
			fill.a = gt/m_gameoveranimtime*255;
			fade.setFillColor(fill);
			window.draw(fade);
			if (gt >= m_gameoveranimtime) {
				player1gameover = true;
				player2gameover = true;
				minigame = 0;
				nextminigame = 0;
			}
			player1gameover = true;
			player2gameover = true;
		} else {
			if (m_p0gameover) player1gameover = true;
			if (m_p1gameover) player2gameover = true;
		}
} break;

case 4: {	// Animation of falling into laberinth
	if (true) nextminigame = 5;	
} break;
case 5: {
#define LABERINTH_INST
#include "laberinth.cpp"	
} break;
case 6: {
#define LABERINTH_GEN
#include "laberinth.cpp"
} break;
case 7: {
#define LABERINTH_CODE
#include "laberinth.cpp"	
} break;

default:  // Minigame does not exist
	nextminigame = 0;
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
