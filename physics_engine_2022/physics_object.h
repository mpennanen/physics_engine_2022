#pragma once

#include <iostream>
#include <vector>
#include "vectors.h"
#include <GLFW/glfw3.h>
#include <glad.c>
#include <glad/glad.h>
class PhysicsObject
{
public:
	vec3 Position;
	vec3 Velocity;
	vec3 Force;
	PhysicsObject(float _x, float _y)
	{
		Position.x = _x;
		Position.y = _y;
		Position.z = 0.0f;
	};

	void Draw()
	{


	}
private:

};


class PhysicsWorld
{
private:
	vec3 m_gravity = vec3(0.0f,-9.81f,0.0f);
public:
	std::vector<PhysicsObject*> m_objects;
	void AddObject(PhysicsObject* object) {/*   */ };
	void RemoveObject(PhysicsObject* object) {/*   */ };

	void Step(float dt)
	{
		for (PhysicsObject* obj : m_objects)
		{


		};
	};
	void Draw(float dt)
	{
		for (PhysicsObject* obj : m_objects)
		{

			float quadVertices[] = {
				// positions     // colors
				-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
				 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
				-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

				-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
				 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
				 0.05f,  0.05f,  0.0f, 1.0f, 1.0f
			};
			GLuint vbo = 0;
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), quadVertices, GL_STATIC_DRAW);

			GLuint vao = 0;
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);


			const char* vertexShaderSource = "#version 330 core\n"
				"layout (location = 0) in vec3 aPos;\n"
				"void main()\n"
				"{\n"
				"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
				"}\0";
			unsigned int vertexShader;
			vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
			glCompileShader(vertexShader);


			const char* fragmentShaderSource = "#version 330 core\n"
				"out vec4 FragColor;\n"
				"void main()\n"
				"{\n"
				"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
				"}\0";
			unsigned int fragmentShader;
			fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
			glCompileShader(fragmentShader);

			GLuint shaderProgram = glCreateProgram();
			glAttachShader(shaderProgram, vertexShader);
			glAttachShader(shaderProgram, fragmentShader);
			glLinkProgram(shaderProgram);

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			glVertexAttribPointer(0, 6, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, 6);

		};
	};
};



