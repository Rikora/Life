#pragma once

#include <SFML/Graphics.hpp>
#include <array>

#define CELLS_X 100
#define CELLS_Y 50
#define CELL_SIZE 10.f
#define UPS 15

namespace lf
{
	using uint = unsigned int;

	struct Cell
	{
		Cell() = default;
		Cell(const sf::Vector2f& pos) : alive(false), body(sf::Vector2f(CELL_SIZE, CELL_SIZE))
		{
			body.setPosition(pos * CELL_SIZE);
			body.setFillColor(sf::Color::Black);
		}

		sf::RectangleShape body;
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
		void update(sf::Time dt);
		void updateCell(const sf::Vector2f& point);
		void setNextState();
		uint getLivingNeighbors(const sf::Vector2i& index) const;
		bool isValid(const sf::Vector2i& index) const;
		
		sf::RenderWindow m_window;
		std::array<sf::Vector2i, 8> m_directions;
		std::vector<std::vector<Cell>> m_cells;
		std::vector<std::vector<bool>> m_nextCellStates;
		std::vector<sf::Vertex> m_verticalLines;
		std::vector<sf::Vertex> m_horizontalLines;
		sf::Time m_updateTimer;
	};
}