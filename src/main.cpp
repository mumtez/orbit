#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

double r;
double gravForce;
double angle;

// Define initial position of the fixed sphere
float sphereX = 0.0f;
float sphereY = 0.0f;

double G = 6.7e-10;
float dt = 0.01f;

int spherexMass = 400000000;


// Define initial position of the movable sphere
vector<float> spherePos2 = {0.2f, 0.2f};
vector<float> spherePos1 = {0, 0};


int sphereyMass = 1;

vector<double> sphereyAcceleration = {0,0};
vector<double> sphereyVelocity = {0.2,-0.6};
vector<double> sphereyForce = {0,0};

vector<vector<float>> path;  // Store previous positions
const int maxTrailLength = 1000;  // Limit trail length to avoid performance issues


// Function to draw a circle (representing the sphere) at the center (x, y)
void drawSphere(vector<float>& pos, float radius) {
    int numSegments = 100;  // Number of segments to approximate a circle
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(pos[0], pos[1]);  // Center of the circle

    for (int i = 0; i <= numSegments; ++i) {
        float angle = 2.0f * M_PI * float(i) / float(numSegments);
        float dx = radius * cosf(angle);
        float dy = radius * sinf(angle);
        glVertex2f(pos[0] + dx, pos[1] + dy);
    }

    glEnd();
}

// Function to handle user input and move the sphere
void processInput(GLFWwindow* window) {
    const float moveSpeed = .005;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        sphereyVelocity[1] += moveSpeed;  // Move up
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        sphereyVelocity[1] -= moveSpeed;  // Move down
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        sphereyVelocity[0] -= moveSpeed;  // Move left
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        sphereyVelocity[0] += moveSpeed;  // Move right
}

// Gravity calculation between two objects
void gravityCalc(float& body1X, float& body1Y, vector<float> body2Pos, int& body1Mass, int& body2Mass) {
    r = sqrt((body1X - body2Pos[0]) * (body1X - body2Pos[0]) + (body1Y - body2Pos[1]) * (body1Y - body2Pos[1]));
    std::cout << "Distance: " << r << std::endl;
    gravForce = (body1Mass * body2Mass * G) / (r * r);
    std::cout << "Gravity Force: " << gravForce << std::endl;
}

void angleCalc(vector<float> body1Pos, vector<float> body2Pos, double& theta) {
    theta = atan2(body1Pos[1] - body2Pos[1], body1Pos[0] - body2Pos[0]);
    std::cout << "Angle: " << theta << std::endl;
}

void forceCalc (double& theta, double& gravForce, vector<double>& force) {
    force[1] = sin(theta) * gravForce;
    force[0] = cos(theta) * gravForce;
    std::cout << "xforce: " << force[0] << std::endl;
    std::cout << "yforce: " << force[1] << std::endl;
}

void accelCalc(vector<double>& force, vector<double>& accel, int& mass) {
    for (int i = 0; i < force.size(); ++i) {
        accel[i] = force[i]/mass;
    }
    std::cout << "Acceleration X: " << accel[0] << std::endl;
    std::cout << "Acceleration Y: " << accel[1] << std::endl;
}
void velocityCalc(vector<double>& accel, vector<double>& velocity) {
    for (int i = 0; i < accel.size(); ++i) {
        velocity[i] += accel[i] * dt;
    }
    std::cout << "Velocity X: " << velocity[0] << std::endl;
    std::cout << "Velocity Y: " << velocity[1] << std::endl;
}

void velocityApp(vector<double>& velocity, vector<float>& pos) {
    for (int i = 0; i < velocity.size(); ++i) {
        pos[i] += velocity[i] * dt;
    }
}
void drawTrail(const vector<vector<float>>& path) {
    if (path.size() < 2) return;  // Not enough points to draw a trail

    glBegin(GL_LINE_STRIP);
    for (const auto& point : path) {
        glVertex2f(point[0], point[1]);
    }
    glEnd();
}






int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        return -1;
    }

    // Create windowed mode window and OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 800, "Orbit", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);
    glOrtho(-1, 1, -1, 1, -1, 1); // Set orthogonal projection for 2D

    // Set OpenGL settings
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glPointSize(3);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Handle gravity calculations and inputs
        gravityCalc(sphereX, sphereY, spherePos2, spherexMass, sphereyMass);

        angleCalc(spherePos1,spherePos2, angle);

        forceCalc(angle,gravForce, sphereyForce);

        accelCalc(sphereyForce, sphereyAcceleration, sphereyMass);

        velocityCalc(sphereyAcceleration, sphereyVelocity);

        velocityApp(sphereyVelocity, spherePos2);

        // Add current position to the path
        path.push_back({spherePos2[0], spherePos2[1]});

        // Remove the oldest point if the trail is too long
        if (path.size() > maxTrailLength) {
            path.erase(path.begin());
        }


        // Process input to move the sphere
        processInput(window);

        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the trail
        glColor3f(0.5f, 1.0f, 0.5f);

        drawTrail(path);

        glColor3f(0.5f, 0.5f, 1.0f);
        // Draw the sphere at the current position
        drawSphere(spherePos1, 0.1f);  // 0.1f is the radius of the fixed sphere

        drawSphere(spherePos2, 0.05f);  // 0.1f is the radius of the movable sphere

        // Swap buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();

    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
