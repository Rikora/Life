#pragma once

#include <SFML/Graphics.hpp>
#include <array>

namespace lf
{
	struct Cell
	{
		Cell() : alive(true) {}
		Cell(const sf::Vector2f& pos) : pos(pos), alive(true) {}

		sf::Vector2f pos; // Random pos on spawn? How can they move if they are static?
		bool alive;
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

		sf::RenderWindow m_window;
		std::vector<sf::Vertex> m_vertices;
		std::vector<Cell> m_cells;
		std::array<sf::Vector2f, 8> m_directions;
	};
}