#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

struct Highscore {
  int highscore = 0;
  int twoplayerhighscore = 0;
};
struct Highscore readhighscore() {
  struct Highscore highscores;
  std::ifstream file("highscore.txt");
  std::string linestr;
  while (getline(file,linestr)) {
    std::stringstream line;
    line.str(linestr);
    std::string key;
    line >> key;
    int value;
    line >> value;
    if (key == "highscore") {
      highscores.highscore = value;
    }
    else if (key == "twoplayerhighscore") {
      highscores.twoplayerhighscore = value;
    }
  }
  file.close();
  return highscores;
}
void writehighscore(struct Highscore highscores) {
  std::ofstream file("highscore.txt");
  file << "highscore " << highscores.highscore << std::endl;
  file << "twoplayerhighscore " << highscores.twoplayerhighscore << std::endl;
  file.close();
}
