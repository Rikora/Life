#pragma once

#include <SFML/Graphics.hpp>

namespace lf
{
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
		sf::CircleShape m_shape;
	};
}