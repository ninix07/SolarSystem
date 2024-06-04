#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <ctime>
#include <stb_image.h>

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

// Draw a filled circle with texture
void drawTexturedCircle(float x, float y, float radius, int segments, GLuint texture)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.5f, 0.5f);
    glVertex2f(x, y); // Center of the circle
    for (int i = 0; i <= segments; i++)
    {
        float angle = i * 2.0f * 3.14159265358979323846f / segments;
        float dx = cos(angle) * radius;
        float dy = sin(angle) * radius;
        glTexCoord2f((cos(angle) + 1.0f) * 0.5f, (sin(angle) + 1.0f) * 0.5f);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

// Load texture from file
GLuint loadTexture(const char *filename)
{
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (!data)
    {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    return texture;
}

struct Planet
{
    float distanceFromSun;
    float radius;
    float orbitSpeed;
    float angle;
    GLuint texture;
};

struct Moon
{
    float distanceFromPlanet;
    float radius;
    float orbitSpeed;
    float angle;
    GLuint texture;
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

    // Load textures
    GLuint earthTexture = loadTexture("earth_texture.jpg");
    if (!earthTexture)
    {
        std::cerr << "Failed to load textures" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Define planets
    Planet planets[] = {
        {100.0f, 15.0f, 0.0025f, 0.0f, 0},            // Mercury
        {150.0f, 18.0f, 0.002f, 0.0f, 0},             // Venus
        {200.0f, 20.0f, 0.0015f, 0.0f, earthTexture}, // Earth
        {250.0f, 18.0f, 0.00085f, 0.0f, 0},           // Mars
        {350.0f, 25.0f, 0.00055f, 0.0f, 0},           // Jupiter
        {450.0f, 23.0f, 0.00045f, 0.0f, 0},           // Saturn
        {550.0f, 20.0f, 0.00035f, 0.0f, 0},           // Uranus
        {650.0f, 19.0f, 0.00025f, 0.0f, 0}            // Neptune
    };

    const int numPlanets = sizeof(planets) / sizeof(Planet);

    // Define moon
    Moon moon = {30.0f, 5.0f, 0.01f, 0.0f, 0}; // Moon orbiting Earth

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the sun
        glColor3f(1.0f, 1.0f, 0.0f); // Yellow
        drawCircle(800.0f, 800.0f, 50.0f, 50);

        // Draw planets and update their positions
        for (int i = 0; i < numPlanets; ++i)
        {
            Planet &planet = planets[i];
            glColor3f(1.0f, 1.0f, 1.0f); // White color for orbits
            drawCircleLine(800.0f, 800.0f, planet.distanceFromSun, 100);

            float planetX = 800.0f + planet.distanceFromSun * cos(planet.angle);
            float planetY = 800.0f + planet.distanceFromSun * sin(planet.angle);

            if (planet.texture)
            {
                drawTexturedCircle(planetX, planetY, planet.radius, 50, planet.texture);
            }
            else
            {
                glColor3f(planet.r, planet.g, planet.b);
                drawCircle(planetX, planetY, planet.radius, 50);
            }

            // Draw moon orbiting Earth
            if (i == 2)
            {                                // Earth
                glColor3f(1.0f, 1.0f, 1.0f); // White color for moon orbit
                drawCircleLine(planetX, planetY, moon.distanceFromPlanet, 100);

                float moonX = planetX + moon.distanceFromPlanet * cos(moon.angle);
                float moonY = planetY + moon.distanceFromPlanet * sin(moon.angle);
                glColor3f(0.8f, 0.8f, 0.8f); // Light gray color for moon
                drawCircle(moonX, moonY, moon.radius, 30);

                moon.angle += moon.orbitSpeed; // Update moon's angle for animation
            }

            // Draw ring around Saturn
            if (i == 5)
            {                                // Saturn
                glColor3f(0.7f, 0.7f, 0.7f); // Gray
                drawCircleLine(planetX, planetY, planet.radius * 1.5f, 100);
                drawCircleLine(planetX, planetY, planet.radius * 1.45f, 100);
                drawCircleLine(planetX, planetY, planet.radius * 1.4f, 100);
            }

            planet.angle += planet.orbitSpeed; // Update planet's angle for animation
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
