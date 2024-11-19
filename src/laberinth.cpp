// Laberinth Minigame with a Procedural Maze
#define MAZE_SIZE_X 64
#define MAZE_SIZE_Y 32
#define MAZE_TILE_SIZE 16
#define MAZE_GEN_RATE 16
//
#define MOVE_DELAY 0.1	// Delay in seconds
//
#ifdef LABERINTH_FUNC
#include "maze.hpp"
#undef LABERINTH_FUNC
#endif

#ifdef LABERINTH_ASSET
#undef LABERINTH_ASSET
sf::Texture l_p0tex;	// Player 0 Texture
if (!l_p0tex.loadFromFile("textures/player0.laberinth.png")) {
	std::cout << "Failed to load texture 'player0.laberinth'" << std::endl;
	ok = false;
}
sf::Texture l_p1tex;	// Player 1 Texture
if (!l_p1tex.loadFromFile("textures/player1.laberinth.png")) {
	std::cout << "Failed to load texture 'player1.laberinth'" << std::endl;
	ok = false;
}
sf::Texture l_enemytex;	// Enemy Texture
if (!l_enemytex.loadFromFile("textures/enemy.laberinth.png")) {
	std::cout << "Failed to load texture 'enemy.laberinth'" << std::endl;
	ok = false;
}
// Wall Textures
sf::Texture l_fillwalltex;	// Filler texture to not leave gaps in Wall
if (!l_fillwalltex.loadFromFile("textures/fill.wall.laberinth.png")) {
	std::cout << "Failed to load texture 'fill.wall.laberinth'" << std::endl;
	ok = false;
}
sf::Texture l_litwalltex;	// Lit Wall Texture
if (!l_litwalltex.loadFromFile("textures/lit.wall.laberinth.png")) {
	std::cout << "Failed to load texture 'lit.wall.laberinth'" << std::endl;
	ok = false;
}
sf::Texture l_shadowwalltex;	// Shadowed Wall Texture
if (!l_shadowwalltex.loadFromFile("textures/shadow.wall.laberinth.png")) {
	std::cout << "Failed to load texture 'shadow.wall.laberinth'" << std::endl;
	ok = false;
}
// End of wall textures
sf::Texture l_cointex;	// Coin Texture
if (!l_cointex.loadFromFile("textures/coin.laberinth.png")) {
	std::cout << "Failed to load texture 'coin.laberinth'" << std::endl;
	ok = false;
}
#endif

#ifdef LABERINTH_VARS
#undef LABERINTH_VARS
unsigned l_scale = 64;
sf::Vector2u l_p0pos(0,0);
float l_p0delay = 0;
sf::Vector2u l_p1pos(0,0);
float l_p1delay = 0;
sf::Vector2u l_enpos = sf::Vector2u(rand()%(MAZE_SIZE_X/2) + (MAZE_SIZE_X/4),rand()%(MAZE_SIZE_Y/2) + (MAZE_SIZE_Y/4));
float l_endelay = 0;
Maze l_maze(MAZE_SIZE_X, MAZE_SIZE_Y);
int l_genguess = MAZE_SIZE_X * MAZE_SIZE_Y * 2 - 1;
#endif

#ifdef LABERINTH_RESET
#undef LABERINTH_RESET
l_p0pos = sf::Vector2u(0,0);
l_p0delay = 0;
l_p1pos = sf::Vector2u(0,0);
l_p1delay = 0;
l_enpos = sf::Vector2u(rand()%(MAZE_SIZE_X/2) + (MAZE_SIZE_X/4),rand()%(MAZE_SIZE_Y/2) + (MAZE_SIZE_Y/4));
l_endelay = 0;
l_maze.init(MAZE_SIZE_X, MAZE_SIZE_Y);
#endif

#ifdef LABERINTH_INST
#undef LABERINTH_INST
message = "Controls";
tiptext = "Press [Enter] or Click to Continue";
text.setString("Move: [WASD] or [^<v>]");
if (player2mode) text.setString("Player 1: Move: [WASD]\nPlayer 2: Move: [^<v>]");
text.setOrigin(text.getLocalBounds().width/2,text.getLocalBounds().height/2);
text.setPosition(windowsize.x*0.5,windowsize.y*0.5);	// 50% from top centered
window.draw(text);

// Continue
if (click || enter) {
	minigame = 6;
	nextminigame = 6;	// Skip animation
	clock.restart();
#define LABERINTH_RESET
#include "laberinth.cpp"	// This is cursed on another level (Including the current file in that file)
}

#endif

#ifdef LABERINTH_GEN
#undef LABERINTH_GEN
// Generate Maze
for (int i = 0; i < MAZE_GEN_RATE; i++) if(l_maze.gen()) break;
// Loader
switch ((int)(clock.getElapsedTime().asSeconds()*8)%4) {
	case 0:
		message = "Generating Maze";
		break;
	case 1:
		message = "Generating Maze.";
		break;
	case 2:
		message = "Generating Maze..";
		break;
	default:
		message = "Generating Maze...";
} 
std::stringstream percentagedone;
if (l_genguess > 0) percentagedone << (l_maze.iter() / l_genguess * 100);
else percentagedone << "NaN";
percentagedone << "% Generated (";
percentagedone << l_maze.iter();
percentagedone << "/";
percentagedone << l_genguess;
percentagedone << ")";
tiptext = percentagedone.str();

if (l_maze.done()) {
	nextminigame = 7;
	minigame = 7;
	clock.restart();
}
#endif

#ifdef LABERINTH_CODE
#undef LABERINTH_CODE
// Controls
// Player 0
int l_p0mx = 0;
if (
	!player2mode && sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
	sf::Keyboard::isKeyPressed(sf::Keyboard::D)
) l_p0mx += 1;
if (
	!player2mode && sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
	sf::Keyboard::isKeyPressed(sf::Keyboard::A)
) l_p0mx -= 1;
int l_p0my = 0;
if (
	!player2mode && sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
	sf::Keyboard::isKeyPressed(sf::Keyboard::W)
) l_p0my -= 1;
if (
	!player2mode && sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
	sf::Keyboard::isKeyPressed(sf::Keyboard::S)
) l_p0my += 1;
// Player 1
int l_p1mx = 0;
if ( player2mode && sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
) l_p1mx += 1;
if ( player2mode && sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
) l_p1mx -= 1;
int l_p1my = 0;
if ( player2mode && sf::Keyboard::isKeyPressed(sf::Keyboard::Up)
) l_p1my -= 1;
if ( player2mode && sf::Keyboard::isKeyPressed(sf::Keyboard::Down)
) l_p1my += 1;
// Movement
if (!player1gameover) {
	// Player 0
	l_p0delay -= deltatime;
	if (l_p0delay <= 0) {
		l_p0delay = 0;
		if (l_p0mx != 0 && l_maze.testx(l_p0pos, l_p0mx)) {
			l_p0pos.x += l_p0mx;
			l_p0delay = MOVE_DELAY;
		}
		if (l_p0my != 0 && l_maze.testy(l_p0pos, l_p0my)) {
			l_p0pos.y += l_p0my;
			l_p0delay = MOVE_DELAY;
		}
	}
}
if (!player2gameover && player2mode) {
	// Player 1
	l_p1delay -= deltatime;
	if (l_p1delay <= 0) {
		l_p1delay = 0;
		if (l_p1mx != 0 && l_maze.testx(l_p1pos, l_p1mx)) {
			l_p1pos.x += l_p1mx;
			l_p1delay = MOVE_DELAY;
		}
		if (l_p1my != 0 && l_maze.testy(l_p1pos, l_p1my)) {
			l_p1pos.y += l_p1my;
			l_p1delay = MOVE_DELAY;
		}
	}
}
// Render
{	// Player 0
	sf::Sprite p0(l_p0tex);
	p0.setScale(l_scale/9,l_scale/9);
	p0.setPosition(l_p0pos.x*l_scale, l_p0pos.y*l_scale);
	window.draw(p0);
}
{	// Player 1
	sf::Sprite p1(l_p1tex);
	p1.setScale(l_scale/9,l_scale/9);
	p1.setPosition(l_p1pos.x*l_scale, l_p1pos.y*l_scale);
	window.draw(p1);
}
// Wall Test
{	// Right
	sf::Sprite wall(l_shadowwalltex);
	wall.setScale(l_scale/9, l_scale/9);
	wall.setRotation(0);
	wall.setPosition(l_scale*2, l_scale*2 -l_scale/9);
	window.draw(wall);
}
{	// Down
	sf::Sprite wall(l_shadowwalltex);
	wall.setScale(l_scale/9, l_scale/9);
	wall.setRotation(90);
	wall.setPosition(l_scale*2, l_scale*2);
	window.draw(wall);
}
{	// Left
	sf::Sprite wall(l_shadowwalltex);
	wall.setScale(l_scale/9, l_scale/9);
	wall.setRotation(180);
	wall.setPosition(l_scale*2 -l_scale/9, l_scale*2);
	window.draw(wall);
}
{	// Up 
	sf::Sprite wall(l_shadowwalltex);
	wall.setScale(l_scale/9, l_scale/9);
	wall.setRotation(270);
	wall.setPosition(l_scale*2 -l_scale/9, l_scale*2 -l_scale/9);
	window.draw(wall);
}
{	// Center
	sf::Sprite wall(l_fillwalltex);
	wall.setScale(l_scale/9, l_scale/9);
	wall.setPosition(l_scale*2 -l_scale/9, l_scale*2 -l_scale/9);
	window.draw(wall);
}
#endif
