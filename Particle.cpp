#include "Particle.h"
#include <cstdlib>
#include <cmath>
#include <ctime>

Particle::Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition)
    : m_ttl(TTL)
    , m_numPoints(numPoints)
    , m_radiansPerSec(static_cast<float>(rand()) / RAND_MAX * M_PI)
    , m_cartesianPlane()
    , m_color1(Color::White)
    , m_A(2, numPoints)
{
    // Seed random once in Engine; no need here
    // Setup Cartesian view
    m_cartesianPlane.setCenter(0, 0);
    m_cartesianPlane.setSize(
        static_cast<float>(target.getSize().x),
        -static_cast<float>(target.getSize().y)
    );

    // Map click to Cartesian center
    m_centerCoordinate = target.mapPixelToCoords(mouseClickPosition, m_cartesianPlane);

    // Initial velocities
    m_vx = 100 + rand() % 401; // [100, 500]
    if (rand() % 2) m_vx = -m_vx;
    m_vy = 100 + rand() % 401;

    // Random outer color
    m_color2 = Color(
        static_cast<Uint8>(rand() % 256),
        static_cast<Uint8>(rand() % 256),
        static_cast<Uint8>(rand() % 256)
    );

    // Generate shape vertices
    float theta = static_cast<float>(rand()) / RAND_MAX * (M_PI / 2.0f);
    float dTheta = 2.0f * M_PI / (numPoints - 1);
    for (int j = 0; j < m_numPoints; ++j)
    {
        float r = 20.0f + rand() % 61; // [20, 80]
        float dx = r * std::cos(theta);
        float dy = r * std::sin(theta);
        m_A(0, j) = m_centerCoordinate.x + dx;
        m_A(1, j) = m_centerCoordinate.y + dy;
        theta += dTheta;
    }
}

void Particle::draw(RenderTarget& target, RenderStates states) const
{
    VertexArray lines(TriangleFan, m_numPoints + 1);

    // Center pixel conversion
    Vector2i pixelCenterI = target.mapCoordsToPixel(m_centerCoordinate, m_cartesianPlane);
    Vector2f pixelCenter(static_cast<float>(pixelCenterI.x), static_cast<float>(pixelCenterI.y));
    lines[0].position = pixelCenter;
    lines[0].color = m_color1;

    // Outer vertices conversion
    for (int j = 1; j <= m_numPoints; ++j)
    {
        Vector2f coord(
            static_cast<float>(m_A(0, j - 1)),
            static_cast<float>(m_A(1, j - 1))
        );
        Vector2i pixelI = target.mapCoordsToPixel(coord, m_cartesianPlane);
        Vector2f pixel(static_cast<float>(pixelI.x), static_cast<float>(pixelI.y));
        lines[j].position = pixel;
        lines[j].color = m_color2;
    }

    target.draw(lines);
}

void Particle::update(float dt)
{
    m_ttl -= dt;
    rotate(dt * m_radiansPerSec);
    scale(SCALE);

    // Translate by velocity and gravity
    float dx = m_vx * dt;
    m_vy -= G * dt;
    float dy = m_vy * dt;
    translate(dx, dy);
}

void Particle::translate(double xShift, double yShift)
{
    TranslationMatrix T(xShift, yShift, m_A.getCols());
    m_A = T + m_A;
    m_centerCoordinate.x += static_cast<float>(xShift);
    m_centerCoordinate.y += static_cast<float>(yShift);
}

void Particle::rotate(double theta)
{
    // Move to origin
    Vector2f temp = m_centerCoordinate;
    translate(-temp.x, -temp.y);

    // Rotate
    RotationMatrix R(theta);
    m_A = R * m_A;

    // Move back
    translate(temp.x, temp.y);
}

void Particle::scale(double c)
{
    // Move to origin
    Vector2f temp = m_centerCoordinate;
    translate(-temp.x, -temp.y);

    // Scale
    ScalingMatrix S(c);
    m_A = S * m_A;

    // Move back
    translate(temp.x, temp.y);
}

bool Particle::almostEqual(double a, double b, double eps)
{
	return fabs(a - b) < eps;
}

void Particle::unitTests()
{
    int score = 0;

    cout << "Testing RotationMatrix constructor...";
    double theta = M_PI / 4.0;
    RotationMatrix r(M_PI / 4);
    if (r.getRows() == 2 && r.getCols() == 2 && almostEqual(r(0, 0), cos(theta))
        && almostEqual(r(0, 1), -sin(theta))
        && almostEqual(r(1, 0), sin(theta))
        && almostEqual(r(1, 1), cos(theta)))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing ScalingMatrix constructor...";
    ScalingMatrix s(1.5);
    if (s.getRows() == 2 && s.getCols() == 2
        && almostEqual(s(0, 0), 1.5)
        && almostEqual(s(0, 1), 0)
        && almostEqual(s(1, 0), 0)
        && almostEqual(s(1, 1), 1.5))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing TranslationMatrix constructor...";
    TranslationMatrix t(5, -5, 3);
    if (t.getRows() == 2 && t.getCols() == 3
        && almostEqual(t(0, 0), 5)
        && almostEqual(t(1, 0), -5)
        && almostEqual(t(0, 1), 5)
        && almostEqual(t(1, 1), -5)
        && almostEqual(t(0, 2), 5)
        && almostEqual(t(1, 2), -5))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    
    cout << "Testing Particles..." << endl;
    cout << "Testing Particle mapping to Cartesian origin..." << endl;
    if (m_centerCoordinate.x != 0 || m_centerCoordinate.y != 0)
    {
        cout << "Failed.  Expected (0,0).  Received: (" << m_centerCoordinate.x << "," << m_centerCoordinate.y << ")" << endl;
    }
    else
    {
        cout << "Passed.  +1" << endl;
        score++;
    }

    cout << "Applying one rotation of 90 degrees about the origin..." << endl;
    Matrix initialCoords = m_A;
    rotate(M_PI / 2.0);
    bool rotationPassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), -initialCoords(1, j)) || !almostEqual(m_A(1, j), initialCoords(0, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            rotationPassed = false;
        }
    }
    if (rotationPassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Applying a scale of 0.5..." << endl;
    initialCoords = m_A;
    scale(0.5);
    bool scalePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), 0.5 * initialCoords(0,j)) || !almostEqual(m_A(1, j), 0.5 * initialCoords(1, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            scalePassed = false;
        }
    }
    if (scalePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Applying a translation of (10, 5)..." << endl;
    initialCoords = m_A;
    translate(10, 5);
    bool translatePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), 10 + initialCoords(0, j)) || !almostEqual(m_A(1, j), 5 + initialCoords(1, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            translatePassed = false;
        }
    }
    if (translatePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Score: " << score << " / 7" << endl;
}
