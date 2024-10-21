// Laberinth Minigame
#ifdef LABERINTH_FUNC
#undef LABERINTH_FUNC
#endif

#ifdef LABERINTH_ASSET
#undef LABERINTH_ASSET
sf::Texture l_p0tex;	// Player 0 Texture
sf::Texture l_p1tex;	// Player 1 Texture
sf::Texture l_entex;	// Enemy Texture
sf::Texture l_bltex;	// Block Texture
#endif

#ifdef LABERINTH_VARS
#undef LABERINTH_VARS
sf::Vector2u l_p0pos;
sf::Vector2u l_p1pos;
std::vector<sf::Vector2u> l_enpos;
#endif

#ifdef LABERINTH_CODE
#undef LABERINTH_CODE
// Controls

// Movement

// Render

#endif
