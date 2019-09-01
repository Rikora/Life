#include "Game.hpp"

namespace lf
{
	Game::Game() : 
	m_window(sf::VideoMode(static_cast<uint>(WIDTH), static_cast<uint>(HEIGHT)), "Game of Life", sf::Style::Close),
	m_cells(CELLS_X, std::vector<Cell>(CELLS_Y)),
	m_nextCellStates(CELLS_X, std::vector<bool>(CELLS_Y, false))
	{
		m_window.setVerticalSyncEnabled(true);

		// Init grid
		createGrid();

		// Init directions of neighbors
		m_directions = { sf::Vector2i(-1, 0), sf::Vector2i(1, 0), sf::Vector2i(0, 1), sf::Vector2i(0, -1),
						 sf::Vector2i(-1, -1), sf::Vector2i(1, 1), sf::Vector2i(-1, 1), sf::Vector2i(1, -1) };

		// Init the cells
		for (int i = 0; i < CELLS_X; ++i)
		{
			for (int j = 0; j < CELLS_Y; ++j)
			{
				m_cells[i][j] = Cell(sf::Vector2f(static_cast<float>(i), static_cast<float>(j)));
			}
		}
	}

	void Game::run()
	{
		while (m_window.isOpen())
		{
			pollEvents();
			//update();
			render();
		}
	}

	void Game::pollEvents()
	{
		sf::Event event;

		while (m_window.pollEvent(event))
		{
			if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
			{
				// Set the intersected cell to alive
				updateCell(static_cast<sf::Vector2f>(sf::Mouse::getPosition(m_window)), true);	
			}

			if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Right)
			{
				// Set the intersected cell to dead
				updateCell(static_cast<sf::Vector2f>(sf::Mouse::getPosition(m_window)), false);
			}

			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
			{
				m_window.close();
			}
		}
	}

	void Game::update()
	{
		// Check the cells
		for (int i = 0; i < CELLS_X; ++i)
		{
			for (int j = 0; j < CELLS_Y; ++j)
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

		setNextState(); // Could be this one...
	}


	void Game::render()
	{
		m_window.clear(sf::Color::White);

		// Draw cells
		for (int i = 0; i < CELLS_X; ++i)
		{
			for (int j = 0; j < CELLS_Y; ++j)
			{
				if (m_cells[i][j].alive)
				{
					m_window.draw(m_cells[i][j].body);
				}
			}
		}

		// Draw grid
		m_window.draw(m_verticalLines.data(), m_verticalLines.size(), sf::Lines);
		m_window.draw(m_horizontalLines.data(), m_horizontalLines.size(), sf::Lines);

		m_window.display();
	}

	void Game::setNextState()
	{
		// Update the state of the cells
		for (int i = 0; i < CELLS_X; ++i)
		{
			for (int j = 0; j < CELLS_Y; ++j)
			{
				m_cells[i][j].alive = m_nextCellStates[i][j];
			}
		}
	}

	uint Game::getLivingNeighbors(const sf::Vector2i& index) const
	{
		uint count = 0;

		// Crash here...
		for (const auto& direction : m_directions)
		{
			const auto neighborIndex = index + direction;

			if (isValid(neighborIndex) && m_cells[neighborIndex.x][neighborIndex.y].alive)
			{
				count++;
			}
		}

		return count;
	}

	bool Game::isValid(const sf::Vector2i& index) const
	{
		return (index.x > 0) && (index.x < WIDTH - 1) && 
			   (index.y > 0) && (index.y < HEIGHT - 1);
	}

	void Game::updateCell(const sf::Vector2f& point, bool alive)
	{
		for (int i = 0; i < CELLS_X; ++i)
		{
			for (int j = 0; j < CELLS_Y; ++j)
			{
				if (m_cells[i][j].body.getGlobalBounds().contains(point))
				{
					m_cells[i][j].alive = alive;
				}
			}
		}
	}

	void Game::createGrid()
	{
		// Vertical grid lines
		const int multiple = 2;
		const auto grayColor = sf::Color(75, 75, 75);
		auto size = static_cast<uint>(std::round(HEIGHT / CELL_SIZE) * multiple);
		auto remainder = size % multiple;

		// Make sure the grid expands the whole window
		(remainder != 0) ? m_verticalLines.resize(size + (multiple * multiple) - remainder) : m_verticalLines.resize(size + multiple);

		float tileSize = 0.f;
		for (uint i = 0; i < m_verticalLines.size(); i += 2, tileSize += CELL_SIZE)
		{
			m_verticalLines[i] = sf::Vertex(sf::Vector2f(0.f, static_cast<float>(HEIGHT - tileSize)), grayColor);
			m_verticalLines[i + 1] = sf::Vertex(sf::Vector2f(static_cast<float>(WIDTH), static_cast<float>(HEIGHT - tileSize)), grayColor);
		}

		// Horizontal grid lines
		tileSize = 0.f;
		size = static_cast<uint>(std::round(WIDTH / CELL_SIZE)) * multiple;
		remainder = size % multiple;

		// Make sure the grid expands the whole scene window
		(remainder != 0) ? m_horizontalLines.resize(size + (multiple * multiple) - remainder) : m_horizontalLines.resize(size + multiple);

		for (uint i = 0; i < m_horizontalLines.size(); i += 2, tileSize += CELL_SIZE)
		{
			m_horizontalLines[i] = sf::Vertex(sf::Vector2f(tileSize, static_cast<float>(HEIGHT)), grayColor);
			m_horizontalLines[i + 1] = sf::Vertex(sf::Vector2f(tileSize, 0.f), grayColor);
		}
	}
}