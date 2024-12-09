// Laberinth Minigame with a Procedural Maze
#define MAZE_SIZE_X 16
#define MAZE_SIZE_Y 8
#define MAZE_GEN_RATE 16
//
#define MOVE_DELAY 0.1	// Delay in seconds
//
#ifdef LABERINTH_FUNC
// Maze Library
#include "maze.hpp"
// Maze Render Functions
void verticalwall(sf::RenderWindow& window, sf::Texture& texture, float l_scale, int x, int y, bool flip = false) {	// Create a rectangle shape at the location of the x wall
	if (!flip) {	// Up
		sf::Sprite wall(texture);
		wall.setScale(l_scale/9, l_scale/9);
		wall.setRotation(90);
		wall.setPosition(l_scale*(x+1), l_scale*(y));
		window.draw(wall);
	}
	else if (flip) {	// Down
		sf::Sprite wall(texture);
		wall.setScale(l_scale/9, l_scale/9);
		wall.setRotation(270);
		wall.setPosition(l_scale*(x+1) -l_scale/9, l_scale*(y+1) -l_scale/9);
		window.draw(wall);
	}
}
void horizontalwall(sf::RenderWindow& window, sf::Texture& texture, float l_scale, int x, int y, bool flip = false) {	// Create a rectangle shape at the location of the y wall
	if (!flip) {	// Right
		sf::Sprite wall(texture);
		wall.setScale(l_scale/9, l_scale/9);
		wall.setRotation(0);
		wall.setPosition(l_scale*(x), l_scale*(y+1) -l_scale/9);
		window.draw(wall);
	}
	else if (flip) {	// Left
		sf::Sprite wall(texture);
		wall.setScale(l_scale/9, l_scale/9);
		wall.setRotation(180);
		wall.setPosition(l_scale*(x+1) -l_scale/9, l_scale*(y+1));
		window.draw(wall);
	}
}
void centerwall(sf::RenderWindow& window, sf::Texture& texture, float l_scale, int x, int y) {	// Fill the gap between the walls
	{	// Center
		sf::Sprite wall(texture);
		wall.setScale(l_scale/9, l_scale/9);
		wall.setPosition(l_scale*(x+1) -l_scale/9, l_scale*(y+1) -l_scale/9);
		window.draw(wall);
	}
}
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
sf::Texture l_entex;	// Enemy Texture
if (!l_entex.loadFromFile("textures/enemy.laberinth.png")) {
	std::cout << "Failed to load texture 'enemy.laberinth'" << std::endl;
	ok = false;
}
sf::Texture l_flagtex;	// Flag Texture
if (!l_flagtex.loadFromFile("textures/flag.laberinth.png")) {
	std::cout << "Failed to load texture 'flag.laberinth'" << std::endl;
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
float l_scale = std::min(window.getSize().x/MAZE_SIZE_X,window.getSize().y/MAZE_SIZE_Y);
sf::Vector2u l_p0pos(0,0);
float l_p0delay = 0;
sf::Vector2u l_p1pos(0,0);
float l_p1delay = 0;
sf::Vector2u l_enpos(rand()%(MAZE_SIZE_X/2) + (MAZE_SIZE_X/4),rand()%(MAZE_SIZE_Y/2) + (MAZE_SIZE_Y/4));
float l_endelay = 0;
sf::Vector2u l_flag(MAZE_SIZE_X-1, MAZE_SIZE_Y-1);
Maze l_maze(MAZE_SIZE_X, MAZE_SIZE_Y);
int l_genguess = MAZE_SIZE_X * MAZE_SIZE_Y * 2 - 1;
unsigned l_coinscore = 5;
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
if (player2mode) text.setString("Player 1: Move: [WASD]\n\nPlayer 2: Move: [^<v>]");
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
for (int i = 0; i < MAZE_GEN_RATE; i++) {
	l_maze.gen();
	if(l_maze.done()) break;
}
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
message = "";
tiptext = "";
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
// Enemy
l_endelay -= deltatime;
if (l_endelay <= 0) {
	l_endelay = 0;
	int l_enmx = rand()%2*2-1;
	int l_enmy = rand()%2*2-1;
	if (l_enmx != 0 && l_maze.testx(l_enpos, l_enmx)) {
		l_enpos.x += l_enmx;
		l_endelay = MOVE_DELAY;
	}
	if (l_enmy != 0 && l_maze.testy(l_enpos, l_enmy)) {
		l_enpos.y += l_enmy;
		l_endelay = MOVE_DELAY;
	}

}
// Enemy Gameover TODO
// Coin pickup
{
	if (l_maze.coin(l_p0pos.x,l_p0pos.y)) {
		score += l_coinscore;
		l_maze.pickupcoin(l_p0pos.x,l_p0pos.y);
	}
	if (l_maze.coin(l_p1pos.x,l_p1pos.y)) {
		score += l_coinscore;
		l_maze.pickupcoin(l_p1pos.x,l_p1pos.y);
	}
}
// Render
if (!player1gameover) {	// Player 0
	sf::Sprite p0(l_p0tex);
	p0.setScale(l_scale/9,l_scale/9);
	p0.setPosition(l_p0pos.x*l_scale, l_p0pos.y*l_scale);
	window.draw(p0);
}
if (!player2gameover && player2mode) {	// Player 1
	sf::Sprite p1(l_p1tex);
	p1.setScale(l_scale/9,l_scale/9);
	p1.setPosition(l_p1pos.x*l_scale, l_p1pos.y*l_scale);
	window.draw(p1);
}
{	// Enemy
	sf::Sprite enemy(l_entex);
	enemy.setScale(l_scale/9,l_scale/9);
	enemy.setPosition((l_enpos.x)*l_scale, (l_enpos.y)*l_scale);
	window.draw(enemy);
}
{	// Flag
	sf::Sprite flag(l_flagtex);
	flag.setScale(l_scale/9,l_scale/9);
	flag.setPosition((l_flag.x)*l_scale, (l_flag.y)*l_scale);
	window.draw(flag);
}
// Wall Test
/* Example
verticalwall(window, l_shadowwalltex, l_scale, l_p0pos.x, l_p0pos.y, false);
horizontalwall(window, l_shadowwalltex, l_scale, l_p0pos.x, l_p0pos.y, false);
centerwall(window, l_fillwalltex, l_scale, l_p0pos.x, l_p0pos.y);
wow */
// Render Walls
if (!player1gameover) { // Player 0
	// Lit Walls
	if ( !(l_maze[l_p0pos.x][l_p0pos.y] & RIGHT) )	// Right Wall
		verticalwall(window, l_litwalltex, l_scale, l_p0pos.x, l_p0pos.y, false);
	if ( !(l_maze[l_p0pos.x][l_p0pos.y] & BOTTOM) )	// Bottom Wall
		horizontalwall(window, l_litwalltex, l_scale, l_p0pos.x, l_p0pos.y, false);
	if (l_p0pos.x==0 || !(l_maze[l_p0pos.x-1][l_p0pos.y] & RIGHT) )	// Left Wall
		verticalwall(window, l_litwalltex, l_scale, l_p0pos.x-1, l_p0pos.y, false);
	if (l_p0pos.y==0 || !(l_maze[l_p0pos.x][l_p0pos.y-1] & BOTTOM) )	// Top Wall
		horizontalwall(window, l_litwalltex, l_scale, l_p0pos.x, l_p0pos.y-1, false);
	// Shadow Walls
	// Top
	if (l_p0pos.x>0 && l_p0pos.y>0 && !(l_maze[l_p0pos.x-1][l_p0pos.y-1] & RIGHT) )	// Top Left
		verticalwall(window, l_shadowwalltex, l_scale, l_p0pos.x-1, l_p0pos.y-1, false);
	if (l_p0pos.y>0 && !(l_maze[l_p0pos.x][l_p0pos.y-1] & RIGHT) )	// Top Right
		verticalwall(window, l_shadowwalltex, l_scale, l_p0pos.x, l_p0pos.y-1, false);
	// Bottom
	if (l_p0pos.x>0 && l_p0pos.y<l_maze.height()-1 && !(l_maze[l_p0pos.x-1][l_p0pos.y+1] & RIGHT) )	// Bottom Left
		verticalwall(window, l_shadowwalltex, l_scale, l_p0pos.x-1, l_p0pos.y+1, true);
	if (l_p0pos.y<l_maze.height()-1 && !(l_maze[l_p0pos.x][l_p0pos.y+1] & RIGHT) )	// Bottom Right
		verticalwall(window, l_shadowwalltex, l_scale, l_p0pos.x, l_p0pos.y+1, true);
	// Left
	if (l_p0pos.x>0 && l_p0pos.y>0 && !(l_maze[l_p0pos.x-1][l_p0pos.y-1] & BOTTOM) )	// Left Top
		horizontalwall(window, l_shadowwalltex, l_scale, l_p0pos.x-1, l_p0pos.y-1, false);
	if (l_p0pos.x>0 && !(l_maze[l_p0pos.x-1][l_p0pos.y] & BOTTOM) )	// Left Bottom
		horizontalwall(window, l_shadowwalltex, l_scale, l_p0pos.x-1, l_p0pos.y, false);
	// Right
	if (l_p0pos.x<l_maze.width()-1 && l_p0pos.y>0 && !(l_maze[l_p0pos.x+1][l_p0pos.y-1] & BOTTOM) )	// Right Top
		horizontalwall(window, l_shadowwalltex, l_scale, l_p0pos.x+1, l_p0pos.y-1, true);
	if (l_p0pos.x<l_maze.width()-1 && !(l_maze[l_p0pos.x+1][l_p0pos.y] & BOTTOM) )	// Right Bottom
		horizontalwall(window, l_shadowwalltex, l_scale, l_p0pos.x+1, l_p0pos.y, true);
	// Center Walls
	if (	// Bottom Right
		(l_p0pos.y==l_maze.height()-1) || (l_p0pos.x==l_maze.width()-1) ||	// Edge
		!(l_maze[l_p0pos.x][l_p0pos.y] & RIGHT) && !(l_maze[l_p0pos.x][l_p0pos.y+1] & RIGHT) ||	// Vertical
		!(l_maze[l_p0pos.x][l_p0pos.y] & BOTTOM) && !(l_maze[l_p0pos.x+1][l_p0pos.y] & BOTTOM)	// Horizontal
	)	centerwall(window, l_fillwalltex, l_scale, l_p0pos.x, l_p0pos.y);	// Render
	if (	// Bottom Left
		(l_p0pos.y==l_maze.height()-1) || (l_p0pos.x==0) ||	// Edge
		!(l_maze[l_p0pos.x-1][l_p0pos.y] & RIGHT) && !(l_maze[l_p0pos.x-1][l_p0pos.y+1] & RIGHT) ||	// Vertical
		!(l_maze[l_p0pos.x-1][l_p0pos.y] & BOTTOM) && !(l_maze[l_p0pos.x][l_p0pos.y] & BOTTOM)		// Horizontal
	)	centerwall(window, l_fillwalltex, l_scale, l_p0pos.x-1, l_p0pos.y);	// Render
	if (	// Top Right
		(l_p0pos.y==0) || (l_p0pos.x==l_maze.width()-1) ||	// Edge
		!(l_maze[l_p0pos.x][l_p0pos.y-1] & RIGHT) && !(l_maze[l_p0pos.x][l_p0pos.y] & RIGHT) ||		// Vertical
		!(l_maze[l_p0pos.x][l_p0pos.y-1] & BOTTOM) && !(l_maze[l_p0pos.x+1][l_p0pos.y-1] & BOTTOM)	// Horizontal
	)	centerwall(window, l_fillwalltex, l_scale, l_p0pos.x, l_p0pos.y-1);	// Render
	if (	// Bottom Right
		(l_p0pos.y==0) || (l_p0pos.x==0) ||	// Edge
		!(l_maze[l_p0pos.x-1][l_p0pos.y-1] & RIGHT) && !(l_maze[l_p0pos.x-1][l_p0pos.y] & RIGHT) ||	// Vertical
		!(l_maze[l_p0pos.x-1][l_p0pos.y-1] & BOTTOM) && !(l_maze[l_p0pos.x][l_p0pos.y-1] & BOTTOM)	// Horizontal
	)	centerwall(window, l_fillwalltex, l_scale, l_p0pos.x-1, l_p0pos.y-1);	// Render
}
if (!player2gameover && player2mode) { // Player 2
	// Lit Walls
	if ( !(l_maze[l_p1pos.x][l_p1pos.y] & RIGHT) )	// Right Wall
		verticalwall(window, l_litwalltex, l_scale, l_p1pos.x, l_p1pos.y, false);
	if ( !(l_maze[l_p1pos.x][l_p1pos.y] & BOTTOM) )	// Bottom Wall
		horizontalwall(window, l_litwalltex, l_scale, l_p1pos.x, l_p1pos.y, false);
	if (l_p1pos.x==0 || !(l_maze[l_p1pos.x-1][l_p1pos.y] & RIGHT) )	// Left Wall
		verticalwall(window, l_litwalltex, l_scale, l_p1pos.x-1, l_p1pos.y, false);
	if (l_p1pos.y==0 || !(l_maze[l_p1pos.x][l_p1pos.y-1] & BOTTOM) )	// Top Wall
		horizontalwall(window, l_litwalltex, l_scale, l_p1pos.x, l_p1pos.y-1, false);
	// Shadow Walls
	// Top
	if (l_p1pos.x>0 && l_p1pos.y>0 && !(l_maze[l_p1pos.x-1][l_p1pos.y-1] & RIGHT) )	// Top Left
		verticalwall(window, l_shadowwalltex, l_scale, l_p1pos.x-1, l_p1pos.y-1, false);
	if (l_p1pos.y>0 && !(l_maze[l_p1pos.x][l_p1pos.y-1] & RIGHT) )	// Top Right
		verticalwall(window, l_shadowwalltex, l_scale, l_p1pos.x, l_p1pos.y-1, false);
	// Bottom
	if (l_p1pos.x>0 && l_p1pos.y<l_maze.height()-1 && !(l_maze[l_p1pos.x-1][l_p1pos.y+1] & RIGHT) )	// Bottom Left
		verticalwall(window, l_shadowwalltex, l_scale, l_p1pos.x-1, l_p1pos.y+1, true);
	if (l_p1pos.y<l_maze.height()-1 && !(l_maze[l_p1pos.x][l_p1pos.y+1] & RIGHT) )	// Bottom Right
		verticalwall(window, l_shadowwalltex, l_scale, l_p1pos.x, l_p1pos.y+1, true);
	// Left
	if (l_p1pos.x>0 && l_p1pos.y>0 && !(l_maze[l_p1pos.x-1][l_p1pos.y-1] & BOTTOM) )	// Left Top
		horizontalwall(window, l_shadowwalltex, l_scale, l_p1pos.x-1, l_p1pos.y-1, false);
	if (l_p1pos.x>0 && !(l_maze[l_p1pos.x-1][l_p1pos.y] & BOTTOM) )	// Left Bottom
		horizontalwall(window, l_shadowwalltex, l_scale, l_p1pos.x-1, l_p1pos.y, false);
	// Right
	if (l_p1pos.x<l_maze.width()-1 && l_p1pos.y>0 && !(l_maze[l_p1pos.x+1][l_p1pos.y-1] & BOTTOM) )	// Right Top
		horizontalwall(window, l_shadowwalltex, l_scale, l_p1pos.x+1, l_p1pos.y-1, true);
	if (l_p1pos.x<l_maze.width()-1 && !(l_maze[l_p1pos.x+1][l_p1pos.y] & BOTTOM) )	// Right Bottom
		horizontalwall(window, l_shadowwalltex, l_scale, l_p1pos.x+1, l_p1pos.y, true);
	// Center Walls
	if (	// Bottom Right
		(l_p1pos.y==l_maze.height()-1) || (l_p1pos.x==l_maze.width()-1) ||	// Edge
		!(l_maze[l_p1pos.x][l_p1pos.y] & RIGHT) && !(l_maze[l_p1pos.x][l_p1pos.y+1] & RIGHT) ||	// Vertical
		!(l_maze[l_p1pos.x][l_p1pos.y] & BOTTOM) && !(l_maze[l_p1pos.x+1][l_p1pos.y] & BOTTOM)	// Horizontal
	)	centerwall(window, l_fillwalltex, l_scale, l_p1pos.x, l_p1pos.y);	// Render
	if (	// Bottom Left
		(l_p1pos.y==l_maze.height()-1) || (l_p1pos.x==0) ||	// Edge
		!(l_maze[l_p1pos.x-1][l_p1pos.y] & RIGHT) && !(l_maze[l_p1pos.x-1][l_p1pos.y+1] & RIGHT) ||	// Vertical
		!(l_maze[l_p1pos.x-1][l_p1pos.y] & BOTTOM) && !(l_maze[l_p1pos.x][l_p1pos.y] & BOTTOM)		// Horizontal
	)	centerwall(window, l_fillwalltex, l_scale, l_p1pos.x-1, l_p1pos.y);	// Render
	if (	// Top Right
		(l_p1pos.y==0) || (l_p1pos.x==l_maze.width()-1) ||	// Edge
		!(l_maze[l_p1pos.x][l_p1pos.y-1] & RIGHT) && !(l_maze[l_p1pos.x][l_p1pos.y] & RIGHT) ||		// Vertical
		!(l_maze[l_p1pos.x][l_p1pos.y-1] & BOTTOM) && !(l_maze[l_p1pos.x+1][l_p1pos.y-1] & BOTTOM)	// Horizontal
	)	centerwall(window, l_fillwalltex, l_scale, l_p1pos.x, l_p1pos.y-1);	// Render
	if (	// Bottom Right
		(l_p1pos.y==0) || (l_p1pos.x==0) ||	// Edge
		!(l_maze[l_p1pos.x-1][l_p1pos.y-1] & RIGHT) && !(l_maze[l_p1pos.x-1][l_p1pos.y] & RIGHT) ||	// Vertical
		!(l_maze[l_p1pos.x-1][l_p1pos.y-1] & BOTTOM) && !(l_maze[l_p1pos.x][l_p1pos.y-1] & BOTTOM)	// Horizontal
	)	centerwall(window, l_fillwalltex, l_scale, l_p1pos.x-1, l_p1pos.y-1);	// Render
}
// Coins
{	// Player 0
	for (int x = l_p0pos.x -1; x<=l_p0pos.x+1; x++)
		for (int y = l_p0pos.y -1; y<=l_p0pos.y+1; y++)
			if (x>0 && x<l_maze.width() && y>0 && y<l_maze.height())
				if (l_maze.coin(x,y)) {
					sf::Sprite coin(l_cointex);
					coin.setScale(l_scale/9,l_scale/9);
					coin.setPosition(x*l_scale, y*l_scale);
					window.draw(coin);
				}
}
{	// Player 1
	for (int x = l_p1pos.x -1; x<=l_p1pos.x+1; x++)
		for (int y = l_p1pos.y -1; y<=l_p1pos.y+1; y++)
			if (x>0 && x<l_maze.width() && y>0 && y<l_maze.height())
				if (l_maze.coin(x,y)) {
					sf::Sprite coin(l_cointex);
					coin.setScale(l_scale/9,l_scale/9);
					coin.setPosition(x*l_scale, y*l_scale);
					window.draw(coin);
				}
}
#endif // Game
