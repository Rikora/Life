#pragma once

#include <SFML/Graphics.hpp>
#include <array>

namespace lf
{
	using uint = unsigned int;

	struct Cell
	{
		Cell() = default;
		Cell(const sf::Vector2f& pos) : pos(pos), alive(false) {}

		sf::RectangleShape body;
		sf::Vector2f pos;
		bool alive;
	};

	class Game
	{
	public:
		Game();

		void run();

	private:
		void createGrid();
		void render();
		void pollEvents();
		void update(double dt);
		void setNextState();
		uint getLivingNeighbors(const sf::Vector2i& index) const;
		bool isValid(const sf::Vector2i& index) const;
		bool isClicked(const sf::Vector2f& pos, Cell* cell);
		
		sf::RenderWindow m_window;
		std::array<sf::Vector2i, 8> m_directions;
		std::vector<std::vector<Cell>> m_cells;
		std::vector<std::vector<bool>> m_nextCellStates;
		std::vector<sf::Vertex> m_verticalLines;
		std::vector<sf::Vertex> m_horizontalLines;
	};
}