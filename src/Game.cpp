#include "Game.hpp"

#define WIDTH 600U
#define HEIGHT 480U

namespace lf
{
	Game::Game() : 
	m_window(sf::VideoMode(WIDTH, HEIGHT), "Game of Life"),
	m_cells(30, std::vector<Cell>(30)) // Should be the whole screen?
	{
		m_window.setVerticalSyncEnabled(true);

		// Init vertices 
		m_vertices.push_back(sf::Vertex(sf::Vector2f(10.f, 10.f), sf::Color::Red));
		m_vertices.push_back(sf::Vertex(sf::Vector2f(40.f, 30.f), sf::Color::Blue));
		m_vertices.push_back(sf::Vertex(sf::Vector2f(70.f, 20.f), sf::Color::Green));
		m_vertices.push_back(sf::Vertex(sf::Vector2f(50.f, 5.f), sf::Color::Yellow));

		// Init the cells
		for (uint i = 0; i < 30; ++i)
		{
			for (uint j = 0; j < 30; ++j)
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
		for (uint i = 0; i < 30; ++i)
		{
			for (uint j = 0; j < 30; ++j)
			{
				auto count = getLivingNeighbors(sf::Vector2u(i, j));

				// Apply rules to the cell
				if (m_cells[i][j].isAlive)
				{
					if (count < 2)
					{
						m_cells[i][j].isAlive = false;
					}
					else if (count == 2 || count == 3)
					{
						m_cells[i][j].isAlive = true;
					}
					else if (count > 3)
					{
						m_cells[i][j].isAlive = false;
					}
				}
				else
				{
					if (count == 3)
					{
						m_cells[i][j].isAlive = true;
					}
				}
			}
		}
	}

	// Do this cleaner with a direction for loop, and also first check the boundaries of the index?
	uint Game::getLivingNeighbors(const sf::Vector2u& index)
	{
		uint count = 0;

		// TODO: Check neighbors...

		return count;
	}

	void Game::render()
	{
		m_window.clear();

		// TODO: Draw cells
		m_window.draw(m_vertices.data(), m_vertices.size(), sf::Points);
		
		m_window.display();		
	}
}