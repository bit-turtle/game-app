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
};

class s_player {
public:
	int var;
	float height;
	float vel;
	int lives;
	float invtimer;
	sf::FloatRect hitbox;
	s_player(int variant) {
		var = variant;
		reset();
	}
	void update(float delta, bool input) {
		invtimer -= delta;
		vel += ((input) ? 1.0f : -1.0f) * delta;
		height += vel * delta;
		hitbox.top = height;
	}
	void collision(sf::FloatRect enemy) {
		if (invtimer > 0) return;
		if (hitbox.intersects(enemy)) {
			lives --;
			invtimer = 2;
		}
	}
	bool dead() {
		return (lives > 0) ? false : true;
	}
	void reset() {
		height = 0;
		vel = 0;
		lives = 3;
		invtimer = 0;
		hitbox.width = 10;
		hitbox.height = 10;
		hitbox.top = height;
		hitbox.left = 10;
	}
	void die() {
		lives = 0;
	}
	void render(sf::RenderWindow* window) {
		if (lives > 0)
			return;
			// window->draw();
		else
			return;
	}
};

#endif

// Variables
#ifdef STINGER_VARS
#undef STINGER_VARS

s_player s_p0(0);
s_player s_p1(1);

#endif

// Reset
#ifdef STINGER_RESET
#undef STINGER_RESET

s_p0.reset();
s_p1.reset();

#endif

// Minigame Code
#ifdef STINGER_CODE
#undef STINGER_CODE

// Update pos
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
}

// Update enemies

// Process collisions



#endif
