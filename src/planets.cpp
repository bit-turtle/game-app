// planets.cpp inserted very hackily in the middle of main.cpp in int main(void);
// Variables from main.cpp::int main(void); are available here
// No functions are allowed in file, put them in main.cpp (Note to myself)

switch (minigame) {

	case 1: {
		sf::Text bob = sf::Text("Bob the fish", roboto, 80);
		bob.setPosition(500,500);
		window.draw(bob);
	} break;

	case 2: {} break;

	default: { // Minigame does not exist
		minigame = 0;	 
	}


}

