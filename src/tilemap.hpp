class TileMap : public sf::Drawable, public sf::Transformable
{
public:

    void update() {

	    unsigned int width = tiles->getsize().x;
	    unsigned int height = tiles->getsize().y;
        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::Triangles);
        m_vertices.resize(width * height * 6);

	// Adjust Base based on height
	base -= tileSize.y * height * scale;

        // populate the vertex array, with two triangles per tile
        for (unsigned int i = 0; i < width; ++i)
            for (unsigned int j = 0; j < height; ++j)
            {
                // get the current tile number
                int tileNumber = tiles->get(i, height-1 - j);

                // find its position in the tileset texture
                int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
                int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

                // get a pointer to the triangles' vertices of the current tile
                sf::Vertex* triangles = &m_vertices[(i + j * width) * 6];

                // define the 6 corners of the two triangles
                triangles[0].position = sf::Vector2f(i * tileSize.x * scale, base + j * tileSize.y * scale);
                triangles[1].position = sf::Vector2f((i + 1) * tileSize.x * scale, base + j * tileSize.y * scale);
                triangles[2].position = sf::Vector2f(i * tileSize.x * scale, base + (j + 1) * tileSize.y * scale);
                triangles[3].position = sf::Vector2f(i * tileSize.x * scale, base + (j + 1) * tileSize.y * scale);
                triangles[4].position = sf::Vector2f((i + 1) * tileSize.x * scale, base + j * tileSize.y * scale);
                triangles[5].position = sf::Vector2f((i + 1) * tileSize.x * scale, base + (j + 1) * tileSize.y * scale);

                // define the 6 matching texture coordinates
                triangles[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                triangles[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                triangles[2].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
                triangles[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
                triangles[4].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                triangles[5].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
            }
    }

    bool load(const std::string& tileset, sf::Vector2u tilesize, float tilescale, float tilebase, Grid<uint8_t>* tilemap)
    {

        // load the tileset texture
        if (!m_tileset.loadFromFile(tileset)) {
            	std::cout << "Failed to load tileset" << std::endl;
		return false;
	}

	// Set Vars
	scale = tilescale;
	base = tilebase;
	tileSize = tilesize;
	tiles = tilemap;

	// Initial Update
	update();
 	return true;
    }
private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // apply the tileset texture
        states.texture = &m_tileset;

        // draw the vertex array
        target.draw(m_vertices, states);
    }

    sf::Vector2u tileSize;
    float scale;
    float base;
    Grid<uint8_t>* tiles;
    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};
