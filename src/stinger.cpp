// Final Minigame

// Functions
#ifdef STINGER_FUNC
#undef STINGER_FUNC

class s_enemy {
public:
	sf::Vector2f pos;
	sf::FloatRect hitbox;
	s_enemy(sf::Vector2f startpos, sf::FloatRect hitboxoverride = sf::FloatRect(
		sf::Vector2f(0,0),
		sf::Vector2f(10,10))
	) {
		pos = startpos;
		hitbox = hitboxoverride;
	}
	void update(float delta) {
		pos.x -= 1 * delta;
	}
	void render(sf::RenderWindow* window) {
		return;
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
	s_player(int variant) {
		var = variant;
		reset();
	}
	void reset() {
		height = 0;
		vel = 0;
		accel = 100;
		lives = 3;
		invtimer = 0;
		hitbox.width = 50;
		hitbox.height = 50;
		hitbox.top = height;
		hitbox.left = 10;

		// Drawable init
		rect.setSize(sf::Vector2f(hitbox.width, hitbox.height));
		rect.setFillColor(sf::Color::White);
	}
	void render(sf::RenderWindow* window) {
		if (lives <= 0)
			return;
		rect.setPosition(sf::Vector2f(hitbox.left, hitbox.top));
		window->draw(rect);
	}
	void update(float delta, bool input) {
		invtimer -= delta;
		vel += ( (input) ? (accel) : (-accel) ) * delta;
		height += vel * delta;
		hitbox.top = height;
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

s_player s_p0(0);
s_player s_p1(1);

std::vector<s_enemy> s_enemies;

#endif

// Reset
#ifdef STINGER_RESET
#undef STINGER_RESET

s_p0.reset();
s_p1.reset();

s_enemies.clear();

#endif

// Minigame Code
#ifdef STINGER_CODE
#undef STINGER_CODE

// Update players
if (player2mode) {
	s_p0.update(deltatime,
// 2 Player, Player 0 controls
	(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) ? true : false);
	s_p1.update(deltatime,
// 2 Player, Player 1 controls
	(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) ? true : false);
}
else {
	s_p0.update(deltatime,
// 1 Player, Player 0 Controls
	(sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) ? true : false);
	s_p1.die();
}

// Update and render enemies and process collisions
for (s_enemy& enemy : s_enemies) {
	enemy.update(deltatime);
	enemy.render(&window);
	if (s_p0.alive())
		s_p0.collide(enemy.hitbox);
	if (s_p1.alive())
		s_p1.collide(enemy.hitbox);
}

// Render
s_p0.render(&window);
s_p1.render(&window);

#endif
