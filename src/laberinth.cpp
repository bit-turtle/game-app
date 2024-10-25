// Laberinth Minigame with a Procedural Maze
#include "grid.hpp"
//
#ifdef LABERINTH_FUNC
#undef LABERINTH_FUNC
enum MazeTile {		// Representation of the top right corner of walls on a cell, maze is an array of these
	EMPTY = 0b00,		// * * * * @ @ @ @ & & & & @ @ @ @       &       @ & & & & @ @ @ @       &
	RIGHT = 0b01,		//       *       @       &               &       @               @       &
	BOTTOM = 0b10,		//       *       @       &               &       @               @       &
	BOTTOM_RIGHT = 0b11,	//       *       @       &               &       @               @       &
};
enum MazeSide {
	RIGHT_SIDE,
	BOTTOM_SIDE,
	LEFT_SIDE,
	TOP_SIDE,
};
void connectMaze(Grid<MazeTile>* maze, int x, int y, MazeSide side) {
	bool left = (x == 0) ? true : false;
	bool top = (y == 0) ? true : false;
	switch (side) {
		case RIGHT_SIDE:
			if (maze->get(x,y) == BOTTOM || maze->get(x,y) == BOTTOM_RIGHT)
				maze->set(x,y, BOTTOM_RIGHT);
			else
				maze->set(x,y, RIGHT);
			break;
		case BOTTOM_SIDE:
			if (maze->get(x,y) == RIGHT || maze->get(x,y) == BOTTOM_RIGHT)
				maze->set(x,y, BOTTOM_RIGHT);
			else
				maze->set(x,y, BOTTOM);
			break;
		case LEFT_SIDE:
			if (left) break;
			if (maze->get(x-1,y) == BOTTOM || maze->get(x-1,y) == BOTTOM_RIGHT)
				maze->set(x-1,y, BOTTOM_RIGHT);
			else
				maze->set(x-1,y, RIGHT);
			break;
		case TOP_SIDE:
			if (top) break;
			if (maze->get(x,y-1) == RIGHT || maze->get(x,y-1) == BOTTOM_RIGHT)
				maze->set(x,y-1, BOTTOM_RIGHT);
			else
				maze->set(x,y-1, BOTTOM);
			break;
		
	}
}
#endif

#ifdef LABERINTH_ASSET
#undef LABERINTH_ASSET
sf::Texture l_p0tex;	// Player 0 Texture
if (!l_p0tex.loadFromFile("player0.laberinth.png"))
	std::cout << "Failed to load texture 'player0.laberinth'" << std::endl;
sf::Texture l_p1tex;	// Player 1 Texture
if (!l_p0tex.loadFromFile("player1.laberinth.png"))
	std::cout << "Failed to load texture 'player1.laberinth'" << std::endl;
sf::Texture l_enemytex;	// Enemy Texture
if (!l_p0tex.loadFromFile("enemy.laberinth.png"))
	std::cout << "Failed to load texture 'enemy.laberinth'" << std::endl;
// Wall Textures
sf::Texture l_fillwalltex;	// Filler texture to not leave gaps in Wall
if (!l_p0tex.loadFromFile("fill.wall.laberinth.png"))
	std::cout << "Failed to load texture 'fill.wall.laberinth'" << std::endl;
sf::Texture l_litwalltex;	// Lit Wall Texture
if (!l_p0tex.loadFromFile("lit.wall.laberinth.png"))
	std::cout << "Failed to load texture 'lit.wall.laberinth'" << std::endl;
sf::Texture l_shadowwalltex;	// Shadowed Wall Texture
if (!l_p0tex.loadFromFile("shadow.wall.laberinth.png"))
	std::cout << "Failed to load texture 'shadow.wall.laberinth'" << std::endl;
// End of wall textures
sf::Texture l_cointex;	// Coin Texture
if (!l_p0tex.loadFromFile("coin.laberinth.png"))
	std::cout << "Failed to load texture 'coin.laberinth'" << std::endl;
// Initialize Maze
// TODO
#endif

#ifdef LABERINTH_VARS
#undef LABERINTH_VARS
sf::Vector2u l_p0pos;
sf::Vector2u l_p1pos;
std::vector<sf::Vector2u> l_enpos;
Grid<MazeTile> maze;
#endif

#ifdef LABERINTH_CODE
#undef LABERINTH_CODE
// Controls

// Movement

// Render

#endif
