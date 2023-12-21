#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <string>
#include <iostream>
#include "shader.h"
#include <vector>
#include <glm/glm.hpp>

namespace Trident {

	struct Premake {
		std::vector<glm::vec3> vertices;
		std::vector<int> indices;
	};

	const Premake Square{

		{
		 glm::vec3(1.0f,  1.0f, 0.0f),  // top right
		 glm::vec3(1.0f, -1.0f, 0.0f),  // bottom right
		glm::vec3(-1.0f, -1.0f, 0.0f),  // bottom left
		glm::vec3(-1.0f,  1.0f, 0.0f)   // top left 
		},

		{
		0, 1, 3,
		1, 2, 3
		}
	};
	const Premake Triangle{

		{
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(1.0f, -1.0f, 0.0f),
			glm::vec3(-1.0f, -1.0f, 0.0f)
		},

		{
			0, 1, 2
		}
	};

	class Trident_GX {
	private:
		GLFWwindow* window;
	public:
		Shader* defulatShader;
		int width, height;
		std::string title;

	private:

		void framebuffer_size_callback(GLFWwindow* window, int width, int height)
		{
			glViewport(0, 0, width, height);
		}

		void processInput(GLFWwindow* window)
		{
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);
		}
	public:
		Trident_GX(int Width, int Height, std::string Title) {
			this->width = Width;
			this->height = Height;
			this->title = Title;

			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

			this->window = glfwCreateWindow(width, height, Title.c_str(), NULL, NULL);

			if (window == NULL)
			{
				std::cout << "Failed to create GLFW window" << std::endl;
				glfwTerminate();
			}
			glfwMakeContextCurrent(window);

			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				std::cout << "Failed to initialize GLAD" << std::endl;
			}

			glViewport(0, 0, width, height);

		}
	public:

		void Init() {
			defulatShader = new Shader("src/vertex.shader", "src/fragment.shader");
			defulatShader->use();
			defulatShader->setVec2("resolution", glm::vec2(width, height));
		}
		bool windowShouldClose() {
			return glfwWindowShouldClose(window);
		}
		void Clearup() {
			glfwTerminate();
		}
		void Start(glm::vec3 color) {
			glClearColor(color.r, color.g, color.b, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		void Draw() {
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		void End() {
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		void useDefulatShader() {
			defulatShader->use();
		}
	public:
		bool getInput(int Key) {
			return glfwGetKey(window, Key);
		}
		glm::vec2 getInputDirection() {
			glm::vec2 out(0);
			if (getInput(GLFW_KEY_W))
				out.y += 1;
			if (getInput(GLFW_KEY_S))
				out.y += -1;
			if (getInput(GLFW_KEY_A))
				out.x -= 1;
			if (getInput(GLFW_KEY_D))
				out.x += 1;
			return out;
		}
	};

	class Object {
	public:
		/*
		void SetVerticess(std::vector<glm::vec3> new_vertices) {
			this->vertices = new_vertices;

			glBindVertexArray(this->VAO);

			glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
			glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), this->vertices.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
		}
		void SetIndices(std::vector<int> new_indices) {
			this->indices = new_indices;

			glBindVertexArray(this->VAO);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(int), this->indices.data(), GL_STATIC_DRAW);
		}*/

		void SetColor(glm::vec3 new_color) {
			this->color = new_color;
		}
		void SetColor(float r, float g, float b) {
			this->color.r = r;
			this->color.g = g;
			this->color.b = b;
		}
		void Render() {
			glBindVertexArray(VAO);
			renderer->defulatShader->setVec4("color", glm::vec4(color, 1.0f));
			glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
		}
		void Move(glm::vec2 value) {

			for (int i = 0; i < this->vertices.size(); i++) {
				this->vertices[i] += glm::vec3(value, 0);
			}
			glBindVertexArray(this->VAO);

			glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
			glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), this->vertices.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
		}
		void SetPremake(Premake newPremake) {
			this->indices.clear();
			for (int j = 0; j < newPremake.indices.size(); j++) {
				this->indices.push_back(newPremake.indices[j]);
			}


			this->vertices.clear();
			for (int i = 0; i < newPremake.vertices.size(); i++) {
				glm::vec3 out = newPremake.vertices[i] * glm::vec3(this->size, 0);
				out += glm::vec3(this->position, 0);
				this->vertices.push_back(out);
			}

			glBindVertexArray(this->VAO);

			glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
			glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), this->vertices.data(), GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(int), this->indices.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
		}

		Object(const Trident_GX& renderer, Premake object, glm::vec2 position, glm::vec2 size, glm::vec3 color) {

			/* 
			*  1. Do size
			*  3. Do position
			*/

			this->color = color;
			this->renderer = &renderer;
			for (int j = 0; j < object.indices.size(); j++) {
				this->indices.push_back(object.indices[j]);
			}
			//this->indices = object.indices;
			this->position = position;
			this->size = size;

			for (int i = 0; i < object.vertices.size(); i++) {
				glm::vec3 out = object.vertices[i] * glm::vec3(size, 0);
				out += glm::vec3(position, 0);
				this->vertices.push_back(out);
			}

			Init();
		}
		~Object() {
			Delete();
		}
	private:


		void Init() {
			glGenVertexArrays(1, &this->VAO);
			glGenBuffers(1, &this->VBO);
			glGenBuffers(1, &this->EBO);

			glBindVertexArray(this->VAO);

			glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
			glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), this->vertices.data(), GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(int), this->indices.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
		}

		void Delete() {

		}


		GLuint VBO;
		GLuint VAO;
		GLuint EBO;

		const Trident_GX* renderer;

		std::vector<glm::vec3> vertices;
		glm::vec3 color;
		std::vector<int> indices;

	public:
		glm::vec2 position;
		glm::vec2 size;
	};
}