#include "Engine.h"

Engine::Engine()
{
	m_Window.create(VideoMode::getDesktopMode());		
}

void Engine::run()
{
	Clock c;

	cout << "Starting Particle unit tests..." << endl;
	Particle p(m_Window, 4, { (int)m_Window.getSize().x / 2, (int)m_Window.getSize().y / 2 });
	p.unitTests();
	cout << "Unit tests complete.  Starting engine..." << endl;

	while (m_Window.isOpen())
	{
		Time t = c.restart();
		float seconds = t.asSeconds();

		input();
		update();
		draw();
	}
}

void Engine::input()
{

}

void Engine::update(float dtAsSeconds)
{

}

void Engine::draw()
{

}
