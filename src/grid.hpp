#ifndef GRID_H
#define GRID_H
// Perfect Grid for Maps
template <typename T> class Grid {
	private:
		std::vector<std::vector<T>> data;
		unsigned int width = 0;
		unsigned int height = 0;
	public:
		Grid() {}
		void set(unsigned int x, unsigned int y, T value) {
			// Resize Grid
			while (x >= width) {
				data.emplace_back();
				for (int i = 0; i < height; i++) data.at(width).push_back(static_cast<T>(0));
				width++;
			};
			while (y >= height) {
				for (int i = 0; i < width; i++) data.at(i).push_back(static_cast<T>(0));
				height++;
			}
			// Set Point
			data.at(x).at(y) = value;
		}
		T get(unsigned int x, unsigned int y) {
			if (x >= width || y >= height) return static_cast<T>(0);
			return data.at(x).at(height-1-y);
		}
		sf::Vector2u getsize() {
			sf::Vector2u size;
			size.x = width;
			size.y = height;
			return size;
		}
		void clear() {
			data.clear();
			width = 0;
			height = 0;
		}
};
#endif
