#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "physics.h"

bool mouse_drag = false;
Vec2 mouse_drag_start = Vec2(0, 0);
std::vector<Rectangle*> rectangles;

// Function prototypes
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void drawRectangle(Rectangle* rect);

int main()
{

    rectangles.push_back(new Rectangle(800, 32, 1600, 64,0,0,0,true));
    // Initialize GLFW and create a window
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(1600, 900, "Click to Draw Rectangles", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Set the mouse button callback function
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    // Set the viewport and orthographic projection
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glOrtho(0.0, (double)width, 0.0, (double)height, -1.0, 1.0);


    // Main loop
    while (!glfwWindowShouldClose(window))
    {

        // Clear the screen to white
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw drag line
        if (mouse_drag)
        {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);

            glColor3f(1.0f, 0.5f, 0.5f);
            glLineWidth(2);
            glBegin(GL_LINES);
            glVertex2f(mouse_drag_start.x,mouse_drag_start.y);
            glVertex2f(xpos,height-ypos);
            glEnd();
        }

        // Draw rectangles
        for (std::size_t i = 0; i < rectangles.size(); i++)
        {
            for (std::size_t j = 0; j < rectangles.size(); j++)
            {
                if (j != i)
                {
                    if (sat(rectangles[i], rectangles[j]) == true)
                    {
                        //std::cout << "Collision found!" << std::endl;

                        resolveCollision(rectangles[i], rectangles[j]);
                    }
                }
            }
            rectangles[i]->update();
            drawRectangle(rectangles[i]);

        }
        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    for (std::size_t i = 0; i < rectangles.size(); i++)
    {
        delete rectangles[i];
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

// Mouse button callback function
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    // Add a rectangle at the cursor position
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        // Get the mouse cursor position
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        //rectangles.push_back(new Rectangle(xpos,height - ypos,64,64,0,0.5,0.2,false));
        //std::cout << rectangles.size() << std::endl;
        //std::cout << "x:" << xpos << std::endl;
        //std::cout << "y:" << ypos << std::endl;
        mouse_drag_start.x = xpos;
        mouse_drag_start.y = height-ypos;
        mouse_drag = true;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && mouse_drag)
    {        
        // Get the mouse cursor position
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        Vec2 mouse_drag_delta = Vec2(xpos,height-ypos) - mouse_drag_start;
        rectangles.push_back(new Rectangle(mouse_drag_start.x, mouse_drag_start.y, 64, 64, 0, 1.0, 0.01, false));
        rectangles[rectangles.size() - 1]->velocity = mouse_drag_delta * 0.01;

        mouse_drag = false;
    }
}

// Function to draw a rectangle
void drawRectangle(Rectangle* rect) {
    glColor3f(1.0f, 0.0f, 0.0f);
    glLineWidth(3);
    glBegin(GL_LINES);
    for (size_t i = 0; i < rect->edge_vector.size(); ++i) {
        Vec2 vertex1 = rect->vertex_vector[i];
        Vec2 vertex2 = rect->vertex_vector[(i + 1) % rect->vertex_vector.size()];
        glVertex2f(vertex1.x, vertex1.y);
        glVertex2f(vertex2.x, vertex2.y);

        glVertex2f(vertex1.x, vertex1.y);
        glVertex2f(rect->position.x, rect->position.y);
    }
    glEnd();
}