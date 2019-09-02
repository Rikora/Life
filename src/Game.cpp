#include "Game.hpp"

namespace lf
{
	Game::Game() :
	m_cells(CELLS_X, std::vector<Cell>(CELLS_Y)),
	m_nextCellStates(CELLS_X, std::vector<bool>(CELLS_Y, false)),
	m_updateTimer(sf::Time::Zero)
	{
		const auto screenSize = sf::Vector2f(static_cast<float>(CELLS_X), static_cast<float>(CELLS_Y)) * CELL_SIZE;

		m_window.create(sf::VideoMode(static_cast<uint>(screenSize.x), static_cast<uint>(screenSize.y)), "Game of Life", sf::Style::Close);
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
		sf::Clock clock;

		while (m_window.isOpen())
		{
			pollEvents();
			update(clock.restart());
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

	void Game::update(sf::Time dt)
	{
		// Check for intersection with the cells
		updateCell(static_cast<sf::Vector2f>(sf::Mouse::getPosition(m_window)));
		
		// Check the cells
		m_updateTimer += dt;

		if (m_updateTimer.asMilliseconds() > (1000.f / UPS))
		{
			m_updateTimer = sf::Time::Zero;

			for (int i = 0; i < CELLS_X; ++i)
			{
				for (int j = 0; j < CELLS_Y; ++j)
				{
					auto count = getLivingNeighbors(sf::Vector2i(i, j));
					bool result = false;

					// Apply rules to the cell
					if (m_cells[i][j].alive)
					{
						if (count < 2)
						{
							result = false;
						}
						else if (count == 2 || count == 3)
						{
							result = true;
						}
						else if (count > 3)
						{
							result = false;
						}
					}
					else
					{
						if (count == 3)
						{
							result = true;
						}
					}

					m_nextCellStates[i][j] = result;
				}
			}

			setNextState();
		}	
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

		for (const auto& direction : m_directions)
		{
			const auto neighborIndex = index + direction;

			if (isValid(neighborIndex))
			{				
				if (m_cells[neighborIndex.x][neighborIndex.y].alive)
				{
					count++;
				}
			}
		}

		return count;
	}

	bool Game::isValid(const sf::Vector2i& index) const
	{
		return (index.x > 0) && (index.x < CELLS_X - 1) && 
			   (index.y > 0) && (index.y < CELLS_Y - 1);
	}

	void Game::updateCell(const sf::Vector2f& point)
	{
		for (int i = 0; i < CELLS_X; ++i)
		{
			for (int j = 0; j < CELLS_Y; ++j)
			{
				if (m_cells[i][j].body.getGlobalBounds().contains(point))
				{
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						m_cells[i][j].alive = true;
					}
					else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
					{
						m_cells[i][j].alive = false;
					}
				}
			}
		}
	}

	void Game::createGrid()
	{
		// Vertical grid lines
		const int multiple = 2;
		const auto grayColor = sf::Color(75, 75, 75);
		auto size = static_cast<uint>(std::round(m_window.getSize().y / CELL_SIZE) * multiple);
		auto remainder = size % multiple;

		// Allow the grid to expand the whole window
		(remainder != 0) ? m_verticalLines.resize(size + (multiple * multiple) - remainder) : m_verticalLines.resize(size + multiple);

		float tileSize = 0.f;
		for (uint i = 0; i < m_verticalLines.size(); i += 2, tileSize += CELL_SIZE)
		{
			m_verticalLines[i] = sf::Vertex(sf::Vector2f(0.f, static_cast<float>(m_window.getSize().y - tileSize)), grayColor);
			m_verticalLines[i + 1] = sf::Vertex(sf::Vector2f(static_cast<float>(m_window.getSize().x), static_cast<float>(m_window.getSize().y - tileSize)), grayColor);
		}

		// Horizontal grid lines
		tileSize = 0.f;
		size = static_cast<uint>(std::round(m_window.getSize().x / CELL_SIZE)) * multiple;
		remainder = size % multiple;

		// Allow the grid to expand the whole window
		(remainder != 0) ? m_horizontalLines.resize(size + (multiple * multiple) - remainder) : m_horizontalLines.resize(size + multiple);

		for (uint i = 0; i < m_horizontalLines.size(); i += 2, tileSize += CELL_SIZE)
		{
			m_horizontalLines[i] = sf::Vertex(sf::Vector2f(tileSize, static_cast<float>(m_window.getSize().y)), grayColor);
			m_horizontalLines[i + 1] = sf::Vertex(sf::Vector2f(tileSize, 0.f), grayColor);
		}
	}
}