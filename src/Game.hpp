#pragma once

#include <SFML/Graphics.hpp>
#include <array>

namespace lf
{
	using uint = unsigned int;

	struct Cell
	{
		Cell() : alive(false), neighbors(8) {}
		Cell(const sf::Vector2f& pos) : pos(pos), alive(true), neighbors(8) {}

		sf::Vector2f pos;
		bool alive;
		uint aliveNeighbors;
		std::vector<Cell> neighbors; // Should be array with 8 elements...
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
		void generateNeighbors();

		sf::RenderWindow m_window;
		std::vector<sf::Vertex> m_vertices;
		std::vector<Cell> m_cells;
		std::array<sf::Vector2f, 8> m_directions;
	};
}