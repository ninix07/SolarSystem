#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <ctime>

// Function to initialize GLFW
bool initGLFW()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }
    return true;
}

// Error callback function for GLFW
void error_callback(int error, const char *description)
{
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

// Key callback function for GLFW
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

// Draw a filled circle
void drawCircle(float x, float y, float radius, int segments)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= segments; i++)
    {
        float angle = i * 2.0f * 3.14159265358979323846f / segments;
        float dx = cos(angle) * radius;
        float dy = sin(angle) * radius;
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}

// Draw a circle line
void drawCircleLine(float x, float y, float radius, int segments)
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; i++)
    {
        float angle = i * 2.0f * 3.14159265358979323846f / segments;
        float dx = cos(angle) * radius;
        float dy = sin(angle) * radius;
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}

struct Planet
{
    float distanceFromSun;
    float radius;
    float orbitSpeed;
    float angle;
    float r, g, b;
};

struct AsteroidBelt
{
    float distanceFromSun;
    float angle;
    float radius;
};

struct Circle
{
    float x;
    float y;
    float radius;
    float r, g, b;
};
struct Moon
{
    float distanceFromPlanet;
    float radius;
    float orbitSpeed;
    float angle;
};

int main()
{
    if (!initGLFW())
    {
        return -1;
    }

    glfwSetErrorCallback(error_callback);

    GLFWwindow *window = glfwCreateWindow(1600, 1600, "2D Solar System", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    // Set up OpenGL
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1600.0, 0.0, 1600.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    // Define planets with more realistic colors
    Planet planets[] = {
        {100.0f, 15.0f, 0.0025f, 0.0f, 0.7f, 0.4f, 0.2f},  // Mercury (Grayish-brown)
        {150.0f, 18.0f, 0.002f, 0.0f, 0.8f, 0.5f, 0.2f},   // Venus (Brownish-yellow)
        {200.0f, 20.0f, 0.0015f, 0.0f, 0.2f, 0.5f, 0.2f},  // Earth (Blue for water, Green for land)
        {250.0f, 18.0f, 0.00085f, 0.0f, 0.7f, 0.2f, 0.1f}, // Mars (Reddish-brown)
        {350.0f, 25.0f, 0.00055f, 0.0f, 0.9f, 0.8f, 0.7f}, // Jupiter (Cream)
        {450.0f, 23.0f, 0.00045f, 0.0f, 0.8f, 0.7f, 0.5f}, // Saturn (Pale yellow)
        {550.0f, 20.0f, 0.00035f, 0.0f, 0.5f, 0.7f, 0.9f}, // Uranus (Pale blue)
        {650.0f, 19.0f, 0.00025f, 0.0f, 0.2f, 0.4f, 0.8f}  // Neptune (Deep blue)
    };
    // Moon
    Moon moon = {30.0f, 8.0f, 0.005f, 0.0f};
    const int numPlanets = sizeof(planets) / sizeof(Planet);

    // Define asteroid belt between Mars and Jupiter
    std::vector<AsteroidBelt> asteroidBelt;
    const int numAsteroids = 1000;
    for (int i = 0; i < numAsteroids; ++i)
    {
        float distance = 250.0f + (rand() % 150);          // Random distance between 250 and 400
        float angle = static_cast<float>(rand() % 360);    // Random angle
        float radius = static_cast<float>(rand() % 3 + 1); // Random radius between 1 and 3
        asteroidBelt.push_back({distance, angle, radius});
    }

    // Random asteroids
    std::vector<Circle> asteroids;
    const int numRandAsteroids = 1000;
    for (int i = 0; i < numRandAsteroids; ++i)
    {
        float x = static_cast<float>(rand() % 1600);       // Random x position
        float y = static_cast<float>(rand() % 1600);       // Random y position
        float radius = static_cast<float>(rand() % 2 + 1); // Random radius between 1 and 3
        float r = 0.5f;                                    // Grey color
        float g = 0.5f;                                    // Grey color
        float b = 0.5f;                                    // Grey color
        asteroids.push_back({x, y, radius, r, g, b});
    }
    // Random Stars
    std::vector<Circle> Stars;
    const int numRandStars = 500;
    for (int i = 0; i < numRandStars; ++i)
    {
        float x = static_cast<float>(rand() % 1600);       // Random x position
        float y = static_cast<float>(rand() % 1600);       // Random y position
        float radius = static_cast<float>(rand() % 2 + 1); // Random radius between 1 and 3
        float r = static_cast<float>(rand()) / RAND_MAX;   // Random red component
        float g = static_cast<float>(rand()) / RAND_MAX;   // Random green component
        float b = static_cast<float>(rand()) / RAND_MAX;   // Random blue component
        asteroids.push_back({x, y, radius, r, g, b});
    }

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        // Draw asteroids
        for (const auto &asteroid : asteroids)
        {
            glColor3f(asteroid.r, asteroid.g, asteroid.b);
            drawCircle(asteroid.x, asteroid.y, asteroid.radius, 10);
        }
        // Draw Stars
        for (const auto &asteroid : Stars)
        {
            glColor3f(asteroid.r, asteroid.g, asteroid.b);
            drawCircle(asteroid.x, asteroid.y, asteroid.radius, 10);
        }
        // Sun
        glColor3f(1.0f, 1.0f, 0.0f); // Yellow
        drawCircle(800.0f, 800.0f, 50.0f, 50);

        // Draw planets
        for (int i = 0; i < numPlanets; ++i)
        {
            Planet &planet = planets[i];
            glColor3f(1.0f, 1.0f, 1.0f); // White color for orbits
            drawCircleLine(800.0f, 800.0f, planet.distanceFromSun, 100);

            float planetX = 800.0f + planet.distanceFromSun * cos(planet.angle);
            float planetY = 800.0f + planet.distanceFromSun * sin(planet.angle);
            glColor3f(planet.r, planet.g, planet.b);
            drawCircle(planetX, planetY, planet.radius, 50);
            // Draw moon orbiting Earth
            if (i == 2)
            {

                float moonX = planetX + moon.distanceFromPlanet * cos(moon.angle);
                float moonY = planetY + moon.distanceFromPlanet * sin(moon.angle);
                glColor3f(0.8f, 0.8f, 0.8f); // Light gray color for moon
                drawCircle(moonX, moonY, moon.radius, 30);

                moon.angle += moon.orbitSpeed;
            }
            // Draw ring around Saturn
            if (i == 5)
            {                                // Saturn
                glColor3f(0.7f, 0.7f, 0.7f); // Gray
                drawCircleLine(planetX, planetY, planet.radius * 1.5f, 100);
                drawCircleLine(planetX, planetY, planet.radius * 1.45f, 100);
                drawCircleLine(planetX, planetY, planet.radius * 1.4f, 100);
            }

            planet.angle += planet.orbitSpeed;
        }

        // Draw asteroid belt between Mars and Jupiter
        glColor3f(0.5f, 0.5f, 0.5f); // Gray
        for (size_t i = 0; i < asteroidBelt.size(); ++i)
        {
            // Calculate asteroid position
            float asteroidX = 800.0f + asteroidBelt[i].distanceFromSun * cos(asteroidBelt[i].angle);
            float asteroidY = 800.0f + asteroidBelt[i].distanceFromSun * sin(asteroidBelt[i].angle);

            // Check if the asteroid is within the orbit range of Mars and Jupiter
            if (asteroidBelt[i].distanceFromSun >= 250.0f && asteroidBelt[i].distanceFromSun <= 450.0f)
            {
                // Check if the asteroid overlaps with any planet
                bool overlap = false;
                for (int j = 0; j < numPlanets; ++j)
                {
                    float distanceToPlanet = sqrt(pow(800.0f + planets[j].distanceFromSun * cos(planets[j].angle) - asteroidX, 2) +
                                                  pow(800.0f + planets[j].distanceFromSun * sin(planets[j].angle) - asteroidY, 2));
                    if (distanceToPlanet <= planets[j].radius + asteroidBelt[i].radius)
                    {
                        overlap = true;
                        break;
                    }
                }

                // If no overlap, draw the asteroid
                if (!overlap)
                {
                    drawCircle(asteroidX, asteroidY, asteroidBelt[i].radius, 10);
                }
            }

            asteroidBelt[i].angle += 0.001f;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
