#include "Game.hpp"

namespace lf
{
	Game::Game() : 
	m_window(sf::VideoMode(600, 480), "Game of Life")
	{
		m_window.setVerticalSyncEnabled(true);

		// TODO: Init start condition for some cells...
		m_vertices.push_back(sf::Vertex(sf::Vector2f(10.f, 10.f), sf::Color::Red));
		m_vertices.push_back(sf::Vertex(sf::Vector2f(40.f, 30.f), sf::Color::Blue));
		m_vertices.push_back(sf::Vertex(sf::Vector2f(70.f, 20.f), sf::Color::Green));
		m_vertices.push_back(sf::Vertex(sf::Vector2f(50.f, 5.f), sf::Color::Yellow));

		// Init directions
		m_directions = { sf::Vector2f(-1.f, 0.f), sf::Vector2f(1.f, 0.f), sf::Vector2f( 0.f, 1.f), sf::Vector2f(0.f, -1.f),
						 sf::Vector2f(-1.f, -1.f), sf::Vector2f(1.f, 1.f), sf::Vector2f(-1.f, 1.f), sf::Vector2f(1.f, -1.f) };
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
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
			{
				m_window.close();
			}
		}
	}

	void Game::update(double dt)
	{
		// How can new cells be created?
		for (auto& cell : m_cells)
		{
			for (size_t i = 0; i < m_directions.size(); ++i)
			{
				// TODO: generate neighbors of the current cell
			}

			// Check status of neighbors
			for (const auto& neighbor : cell.neighbors)
			{
				(neighbor.alive) ? cell.aliveNeighbors++ : cell.aliveNeighbors--;
			}

			if (cell.alive)
			{
				if (cell.aliveNeighbors < 2)
				{
					cell.alive = false;
				}
				else if (cell.aliveNeighbors >= 2 && cell.aliveNeighbors <= 3)
				{
					// Lives on to the next generation
					continue;
				}
				else if (cell.aliveNeighbors > 3)
				{
					cell.alive = false;
				}
			}
			else
			{
				if (cell.aliveNeighbors == 3)
				{
					cell.alive = true;
				}
			}
		}
	}

	void Game::generateNeighbors()
	{


	}

	void Game::render()
	{
		m_window.clear();

		// TODO: don't draw dead cells, alive = false, but don't erase them!
		m_window.draw(m_vertices.data(), m_vertices.size(), sf::Points);
		
		m_window.display();		
	}
}