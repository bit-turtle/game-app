// Final Minigame

// Functions
#ifdef STINGER_FUNC
#undef STINGER_FUNC

class s_enemy {
public:
	sf::Vector2f pos;
	sf::Vector2f initpos;
	char variant;
	sf::FloatRect hitbox;

	// Drawables
	sf::RectangleShape rect;
	// Functions
	s_enemy(sf::Vector2f startpos, char var, sf::RenderWindow* window) {
		initpos = sf::Vector2f(startpos.x * window->getSize().y/8, startpos.y * window->getSize().y/8);
		pos = initpos;
		variant = var;
		hitbox.top = pos.y;
		hitbox.left = pos.x;
		hitbox.width = window->getSize().y/8;
		hitbox.height = window->getSize().y/8;

		// Drawable init
		rect.setFillColor(sf::Color::White);
		rect.setSize(sf::Vector2f(hitbox.width, hitbox.height));
	}
	void update(float delta) {
		pos.x -= 200 * delta;
		hitbox.left = pos.x;
	}
	void render(sf::RenderWindow* window) {
		rect.setPosition(sf::Vector2f(hitbox.left, hitbox.top));

		window->draw(rect);
	}
	void reset() {
		pos = initpos;
		hitbox.left = pos.y;
	}
	bool past() {
		if (pos.x - hitbox.width < 0) return true; else return false;
	}
};

class s_player {
public:
	int var;
	float height;
	float vel;
	float accel;
	int lives;
	float invtimer;
	sf::FloatRect hitbox;

	// Drawables
	sf::RectangleShape rect;
	// Functions
	s_player(int variant, sf::RenderWindow* window) {
		var = variant;
		reset(window);
	}
	void reset(sf::RenderWindow* window) {
		height = window->getSize().y / 2.f;
		vel = 0;
		accel = window->getSize().y / 2.f;
		lives = 3;
		invtimer = 0;
		hitbox.width = window->getSize().y/16;
		hitbox.height = window->getSize().y/16;
		hitbox.top = height;
		hitbox.left = 10;

		// Drawable init
		rect.setSize(sf::Vector2f(hitbox.width, hitbox.height));
		rect.setFillColor(sf::Color::White);
		rect.setOrigin(rect.getSize().x/2, rect.getSize().y/2);	
	}
	void render(sf::RenderWindow* window) {
		if (lives <= 0)
			return;
		rect.setPosition(sf::Vector2f(hitbox.left, hitbox.top));
		rect.setRotation(50);
		window->draw(rect);
	}
	void update(float delta, bool input, sf::RenderWindow* window) {
		invtimer -= delta;
		vel += ( (input) ? (-accel) : (accel) ) * delta;
		height += vel * delta;
		hitbox.top = height;

		// Screen Limit
		if (height < 0) {
			height = 0;
			vel = 0;
			height = 0;
		}
		if (height > window->getSize().y-hitbox.height) {
			height = window->getSize().y-hitbox.height;
			vel = 0;
			height = window->getSize().y-hitbox.height;
		}
	}
	void collide(sf::FloatRect enemy) {
		if (invtimer > 0) return;
		if (hitbox.intersects(enemy)) {
			lives --;
			invtimer = 2;
		}
	}
	bool dead() {
		return (lives > 0) ? false : true;
	}
	bool alive() {
		return (lives > 0) ? true : false;
	}
	void die() {
		lives = 0;
	}
};

#endif

// Variables
#ifdef STINGER_VARS
#undef STINGER_VARS

float s_gameovertime = 0;
float s_gameoveranimtime = 5;

s_player s_p0(0, &window);
s_player s_p1(1, &window);

std::vector<s_enemy> s_enemies;

// Load stinger map
{
	std::ifstream file("assets/stinger.map.txt");
	if (file.is_open()) {
		std::string line;
		unsigned int linenumber = 0;
		while (getline(file, line)) {
			std::stringstream sline(line);
			char value;
			unsigned int valuenumber = 0;
			while (sline.get(value)){
				valuenumber++;
				// Add enemy
				if (value == ' ') continue;
				s_enemies.emplace_back(
					sf::Vector2f(
						valuenumber,
						linenumber
					),
					value,
					&window
				);
			}
			linenumber++;
		}
	}
	else {
		std::cout << "Failed to Load Map 'stinger.map'" << std::endl;
		ok = false;
	}
}

#endif

// Reset
#ifdef STINGER_RESET
#undef STINGER_RESET

s_gameovertime = 0;

s_p0.reset(&window);
s_p1.reset(&window);

for (s_enemy& enemy : s_enemies) {
	enemy.reset();
}

#endif

// Minigame Code
#ifdef STINGER_CODE
#undef STINGER_CODE

// Update players
if (player2mode) {
	s_p0.update(deltatime,
// 2 Player, Player 0 controls
	(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) ? true : false,
	&window);
	s_p1.update(deltatime,
// 2 Player, Player 1 controls
	(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) ? true : false,
	&window);
}
else {
	s_p0.update(deltatime,
// 1 Player, Player 0 Controls
	(sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) ? true : false,
	&window);
	s_p1.die();
}

// Update and render enemies and process collisions
bool allpast = true;
for (s_enemy& enemy : s_enemies) {
	enemy.update(deltatime);
	enemy.render(&window);
	if (s_p0.alive())
		s_p0.collide(enemy.hitbox);
	if (s_p1.alive())
		s_p1.collide(enemy.hitbox);
	if (!enemy.past())
		allpast = false;
}

// Render
s_p0.render(&window);
s_p1.render(&window);

// Win fade
if (allpast) {
	s_gameovertime += deltatime;
	extraDelayClock = time-5.f;	// Skip other animation
	sf::RectangleShape fade((sf::Vector2f)windowsize);
	sf::Color fill = sf::Color::Black;
	fill.a = s_gameovertime/s_gameoveranimtime*255;
	fade.setFillColor(fill);
	window.draw(fade);
	if (s_gameovertime >= s_gameoveranimtime) {
		player1gameover = s_p0.dead();
		player2gameover = s_p1.dead();
		nextminigame = 10;
		minigame = 10;
	}
}

// Gameover fade
if ( (s_p0.dead() && !player2mode) || (s_p1.dead() && s_p0.dead()) ) {
	s_gameovertime += deltatime;
	extraDelayClock = time-5.f;	// Skip other animation
	sf::RectangleShape fade((sf::Vector2f)windowsize);
	sf::Color fill = sf::Color::Black;
	fill.a = s_gameovertime/s_gameoveranimtime*255;
	fade.setFillColor(fill);
	window.draw(fade);
	if (s_gameovertime >= s_gameoveranimtime) {
		player1gameover = true;
		player2gameover = true;
		nextminigame = 0;
		minigame = 0;
	}
}


#endif
