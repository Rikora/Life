#include "Game.hpp"

namespace lf
{
	Game::Game() : 
	m_window(sf::VideoMode(600, 480), "SFML works!"),
	m_shape(100.f)
	{
		m_shape.setFillColor(sf::Color::Green);
	}

	void Game::run()
	{
		while (m_window.isOpen())
		{
			pollEvents();
			update(1.0 / 60.0);
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
		// Do nothing for now...
	}

	void Game::render()
	{
		m_window.clear();

		m_window.draw(m_shape);
		
		m_window.display();		
	}
}