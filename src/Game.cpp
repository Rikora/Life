#include "Game.hpp"

#define WIDTH 600
#define HEIGHT 480
#define CELLS 30

namespace lf
{
	Game::Game() : 
	m_window(sf::VideoMode(static_cast<uint>(WIDTH), static_cast<uint>(HEIGHT)), "Game of Life"),
	m_cells(CELLS, std::vector<Cell>(CELLS)),
	m_nextCellStates(CELLS, std::vector<bool>(CELLS, false))
	{
		m_window.setVerticalSyncEnabled(true);

		// Init vertices 
		m_vertices.push_back(sf::Vertex(sf::Vector2f(10.f, 10.f), sf::Color::Red));
		m_vertices.push_back(sf::Vertex(sf::Vector2f(40.f, 30.f), sf::Color::Blue));
		m_vertices.push_back(sf::Vertex(sf::Vector2f(70.f, 20.f), sf::Color::Green));
		m_vertices.push_back(sf::Vertex(sf::Vector2f(50.f, 5.f), sf::Color::Yellow));

		// Init directions of neighbors
		m_directions = { sf::Vector2i(-1, 0), sf::Vector2i(1, 0), sf::Vector2i(0, 1), sf::Vector2i(0, -1),
						 sf::Vector2i(-1, -1), sf::Vector2i(1, 1), sf::Vector2i(-1, 1), sf::Vector2i(1, -1) };

		// Init the cells
		for (int i = 0; i < CELLS; ++i)
		{
			for (int j = 0; j < CELLS; ++j)
			{
				m_cells[i][j] = { sf::Vector2f(static_cast<float>(i), static_cast<float>(j)) }; // Note: Is this the correct coordinate system?
			}
		}
	}

	void Game::run()
	{
		while (m_window.isOpen())
		{
			pollEvents();
			//update(1.0 / 60.0);
			render();
		}
	}

	void Game::pollEvents()
	{
		sf::Event event;

		while (m_window.pollEvent(event))
		{
			// TODO: check if mouse is down and if the mouse intersects a cell -> alive/dead

			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
			{
				m_window.close();
			}
		}
	}

	void Game::update(double dt)
	{
		// Check the cells
		for (int i = 0; i < CELLS; ++i)
		{
			for (int j = 0; j < CELLS; ++j)
			{
				auto count = getLivingNeighbors(sf::Vector2i(i, j));

				// Apply rules to the cell
				if (m_cells[i][j].alive)
				{
					if (count < 2)
					{
						m_cells[i][j].alive = false;
					}
					else if (count == 2 || count == 3)
					{
						m_cells[i][j].alive = true;
					}
					else if (count > 3)
					{
						m_cells[i][j].alive = false;
					}
				}
				else
				{
					if (count == 3)
					{
						m_cells[i][j].alive = true;
					}
				}
			}
		}

		setNextState();
	}

	void Game::setNextState()
	{
		for (int i = 0; i < CELLS; ++i)
		{
			for (int j = 0; j < CELLS; ++j)
			{
				m_cells[i][j].alive = m_nextCellStates[i][j];
			}
		}
	}

	uint Game::getLivingNeighbors(const sf::Vector2i& index)
	{
		uint count = 0;

		for (const auto& direction : m_directions)
		{
			auto neighborIndex = index + direction;

			if (isValid(neighborIndex) && m_cells[neighborIndex.x][neighborIndex.y].alive)
			{
				count++;
			}
		}

		return count;
	}

	// Note: This may not be correct
	bool Game::isValid(const sf::Vector2i& index)
	{
		return (index.x >= 0) && (index.x < WIDTH - 1) && 
			   (index.y >= 0) && (index.y < HEIGHT - 1);
	}

	void Game::render()
	{
		m_window.clear();

		// TODO: Draw cells
		m_window.draw(m_vertices.data(), m_vertices.size(), sf::Points);
		
		m_window.display();		
	}
}