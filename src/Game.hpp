#pragma once

#include <SFML/Graphics.hpp>

namespace lf
{
	using uint = unsigned int;

	struct Cell
	{
		Cell() = default;
		Cell(const sf::Vector2f& pos) : pos(pos), isAlive(false) {}

		sf::RectangleShape body;
		sf::Vector2f pos;
		bool isAlive;
	};

	class Game
	{
	public:
		Game();

		void run();

	private:
		void render();
		void pollEvents();
		void update(double dt);
		uint getLivingNeighbors(const sf::Vector2u& index);

		sf::RenderWindow m_window;
		std::vector<sf::Vertex> m_vertices;
		std::vector<std::vector<Cell>> m_cells; // Could be 1D as well for better performance
		//sf::Vector2f m_dimensions;
	};
}