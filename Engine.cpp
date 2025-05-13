#include "Engine.h"

Engine::Engine() 
{
    m_Window.create(sf::VideoMode::getDesktopMode(), "Particles");
}

void Engine::input() 
{
    sf::Event event;
    while (m_Window.pollEvent(event)) 
    {
        if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) 
        {
            m_Window.close();  
        }

        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) 
        {
            sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);

            for (int i = 0; i < 5; ++i) 
            {
                int numPoints = rand() % 26 + 25; 
                Particle p(m_Window, numPoints, mousePos);
                m_particles.push_back(p);
            }
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C)
        {
            currtime = time(0);
            celebration_mode = true;
        }
    }
}

void Engine::run() 
{
    Clock m_Clock;
    cout << "Starting Particle unit tests..." << endl;

    Particle p(m_Window, 4, { (int)m_Window.getSize().x / 2, (int)m_Window.getSize().y / 2 });
    p.unitTests();

    cout << "Unit tests complete. Starting engine..." << endl;
    while (m_Window.isOpen()) 
    {
        float dt = m_Clock.restart().asSeconds();
        input(); 
        celebrate(); 
        update(dt);  
        draw(); 
    }
}

void Engine::update(float dtAsSeconds) 
{
    for (auto it = m_particles.begin(); it != m_particles.end();) 
    {
        if (it->getTTL() > 0.0f) 
        {
            it->update(dtAsSeconds);
            ++it;
        }
        else 
        {
            it = m_particles.erase(it); 
        }
    }
}

void Engine::celebrate()
{
	if (celebration_mode && (difftime(time(0), currtime) >= 1.0))
	{
		currtime = time(0);

		sf::Vector2i randPos(rand() % m_Window.getSize().x, rand() % m_Window.getSize().y);

		for (int i = 0; i < 5; ++i)
		{
			int numPoints = rand() % 26 + 25;
			Particle p(m_Window, numPoints, randPos);
			m_particles.push_back(p);
		}
	}
}

void Engine::draw() 
{
    m_Window.clear();  
    for (const auto& particle : m_particles) {
        particle.draw(m_Window, sf::RenderStates::Default); 
    }

    m_Window.display(); 
}
