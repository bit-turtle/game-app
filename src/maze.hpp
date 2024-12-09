// Maze Header
#include <vector>
#include <random>
#define COIN_CHANCE 16	// 1/x chance

enum mazedir {
	RIGHT_SIDE,
	LEFT_SIDE,
	TOP_SIDE,
	BOTTOM_SIDE,
};
enum mazetile {
	DEFAULT = 0b00,	// All Walls
	RIGHT = 0b01,	// No Right Wall
	BOTTOM = 0b10,	// No Bottom Wall
	EMPTY = 0b11,	// No Walls
};
class Maze {
	int iteration = 0;
	std::vector<std::vector<mazetile>> maze;
	std::vector<std::vector<bool>> coins;
	std::vector<std::vector<bool>> visited;
	std::vector<sf::Vector2u> stack;
	sf::Vector2u size;
	public:
	void init(unsigned int sizeX = 16, unsigned int sizeY = 16) {
		size.x = sizeX;
		size.y = sizeY;
		maze.clear();
		visited.clear();
		coins.clear();
		stack.clear();
		maze.reserve(sizeX);
		visited.reserve(sizeX);
		coins.reserve(sizeX);
		for (unsigned int x = 0; x < sizeX; x++) {
			maze.emplace_back();
			visited.emplace_back();
			coins.emplace_back();
			maze.at(maze.size()-1).reserve(sizeY);
			visited.at(visited.size()-1).reserve(sizeY);
			coins.at(coins.size()-1).reserve(sizeY);
			for (unsigned int y = 0; y < sizeY; y++) {
				maze.at(maze.size()-1).push_back(DEFAULT);
				visited.at(visited.size()-1).push_back(false);
				coins.at(coins.size()-1).push_back( (rand()%COIN_CHANCE == 0) ? true : false );	// Randomly place coins
			}
		}
		// Choose random starting point
		stack.emplace_back(rand()%sizeX, rand()%sizeY);
		// Visit starting point
		visited.at(stack.at(0).x).at(stack.at(0).y) = true;
	}
	Maze(unsigned int sizeX = 16, unsigned int sizeY = 16) {init(sizeX,sizeY);}
	bool gen() {
		if (stack.empty()) return true;
		sf::Vector2u pos = stack.at(stack.size()-1);
		iteration++;
		bool found = false;
		mazedir newdir;
		sf::Vector2u newpos = pos;
		std::vector<mazedir> moves;
		// Add possible moves to list
		moves.reserve(4);
		moves.push_back(RIGHT_SIDE);
		moves.push_back(LEFT_SIDE);
		moves.push_back(TOP_SIDE);
		moves.push_back(BOTTOM_SIDE);
		// Loop randomly through moves
		while (moves.size() > 0) {
			int move = rand()%moves.size();
			mazedir side = moves.at(move);
			sf::Vector2u movepos = pos;
			switch (side) {
				case RIGHT_SIDE:
					movepos.x += 1;
					break;
				case LEFT_SIDE:
					movepos.x -= 1;
					break;
				case TOP_SIDE:
					movepos.y -= 1;
					break;
				case BOTTOM_SIDE:
					movepos.y += 1;
					break;
				default:
					break;
			}
			if (
				movepos.x >= 0 &&
				movepos.y >= 0 &&
				movepos.x < size.x &&
				movepos.y < size.y &&
				visited.at(movepos.x).at(movepos.y) == false
			) {
				found = true;
				newpos = movepos;
				newdir = side;
				break;
			}
			else {
				moves.erase(moves.begin()+move);
				continue;
			}
		}
		if (found) {
			visited.at(newpos.x).at(newpos.y) = true;
			stack.push_back(newpos);
			// Modify maze to make path
			switch (newdir) {
				case RIGHT_SIDE:
					maze[pos.x][pos.y] = static_cast<mazetile>(
						static_cast<int>(maze[pos.x][pos.y])
						|	// Or
						static_cast<int>(RIGHT)
					);
					break;
				case LEFT_SIDE:
					maze[pos.x-1][pos.y] = static_cast<mazetile>(
						static_cast<int>(maze[pos.x-1][pos.y])
						|	// Or
						static_cast<int>(RIGHT)
					);
					break;
				case TOP_SIDE:
					maze[pos.x][pos.y-1] = static_cast<mazetile>(
						static_cast<int>(maze[pos.x][pos.y-1])
						|	// Or
						static_cast<int>(BOTTOM)
					);
					break;
				case BOTTOM_SIDE:
					maze[pos.x][pos.y] = static_cast<mazetile>(
						static_cast<int>(maze[pos.x][pos.y])
						|	// Or
						static_cast<int>(BOTTOM)
					);
					break;
			}
		}
		else {
			stack.pop_back();
		}
		return false;
	}
	int iter() {
		return iteration;
	}
	bool done() {
		return stack.empty();
	}
	unsigned int width() {
		return size.x;
	}
	unsigned int height() {
		return size.y;
	}
	std::vector<mazetile> operator[](unsigned int x) {
		return maze.at(x);
	}
	bool testx(sf::Vector2u pos, int dir) {
		if (pos.x>0 && dir<0 && maze.at(pos.x-1).at(pos.y) & RIGHT ) return true;
		if (dir>0 && maze.at(pos.x).at(pos.y) & RIGHT ) return true;
		return false;
	}
	bool testy(sf::Vector2u pos, int dir) {
		if (pos.y>0 && dir<0 && maze.at(pos.x).at(pos.y-1) & BOTTOM ) return true;
		if (dir>0 && maze.at(pos.x).at(pos.y) & BOTTOM ) return true;
		return false;
	}
	bool coin(unsigned x, unsigned y) {
		if (x>=width()||y>=height()) return false;
		return coins.at(x).at(y);
	}
	void pickupcoin(int x, int y) {
		coins.at(x).at(y) = false;
	}
	void debug() {
		std::cout << "|";
		for (unsigned int x = 0; x < width(); x++) std::cout << "----";
		std::cout << "|" << std::endl;
		for (unsigned int y = 0; y < height(); y++) {
			std::cout << "|";
			for (unsigned int x = 0; x < height(); x++) if (maze[x][y] & RIGHT) std::cout << "████";
			else std::cout << "██  ";
			std::cout << "|" << std::endl << "|";
			for (unsigned int x = 0; x < height(); x++) if (maze[x][y] & BOTTOM) std::cout << "██  ";
			else std::cout << "    ";
			std::cout << "|" << std::endl;
		}
		std::cout << "|";
		for (unsigned int x = 0; x < width(); x++) std::cout << "----";
		std::cout << "|" << std::endl;
	}
};
