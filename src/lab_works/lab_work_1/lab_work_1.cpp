#include "lab_work_1.hpp"
#include "imgui.h"
#include "utils/read_file.hpp"
#include <iostream>

namespace M3D_ISICG
{
	const std::string LabWork1::_shaderFolder = "src/lab_works/lab_work_1/shaders/";

	LabWork1::~LabWork1() {
		glDeleteProgram(program);
		glDeleteBuffers(1, &vbo);
		glDisableVertexArrayAttrib(vao, 0);
		glDeleteVertexArrays(1, &vao);
	}

	bool LabWork1::init()
	{
		std::cout << "Initializing lab work 1..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor(_bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w);

		const std::string vertexShaderStr = readFile(_shaderFolder + "lw1.vert");
		const std::string fragmentShaderStr = readFile(_shaderFolder + "lw1.frag");
		GLuint			  vertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLuint			  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		const GLchar* vSrc = vertexShaderStr.c_str();
		const GLchar* fSrc = fragmentShaderStr.c_str();
		glShaderSource(vertexShader, 1, &vSrc, NULL);
		glShaderSource(fragmentShader, 1, &fSrc, NULL);
		glCompileShader(vertexShader);
		glCompileShader(fragmentShader);
		GLint compiled;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			GLchar log[1024];
			glGetShaderInfoLog(vertexShader, sizeof(log), NULL, log);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			std::cerr << " Error compiling vertex shader : " << log << std::endl;
			return false;
		}
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			GLchar log[1024];
			glGetShaderInfoLog(vertexShader, sizeof(log), NULL, log);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			std::cerr << " Error compiling vertex shader : " << log << std::endl;
			return false;
		}
		program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);
		GLint linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if (!linked)
		{
			GLchar log[1024];
			glGetProgramInfoLog(program, sizeof(log), NULL, log);
			std::cerr << " Error linking program : " << log << std::endl;
			return false;
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		pointsTriangle = { Vec2f(-0.5, 0.5), Vec2f(0.5, 0.5), Vec2f(0.5, -0.5)};
		glCreateBuffers(1, &vbo);
		glNamedBufferData(vbo, pointsTriangle.size() * sizeof(Vec2f), pointsTriangle.data(), GL_STATIC_DRAW);
		glCreateVertexArrays(1, &vao);
		glEnableVertexArrayAttrib(vao, 0);
		glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vec2f));
		glVertexArrayAttribBinding(vao, 0, 0);
		glUseProgram(program);
		std::cout << "Done!" << std::endl;
		std::cout << "good!" << std::endl;

		return true;
	}

	void LabWork1::animate(const float p_deltaTime) {}

	void LabWork1::render() {
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0,pointsTriangle.size());
		glBindVertexArray(0);
	}

	void LabWork1::handleEvents(const SDL_Event& p_event)
	{}

	void LabWork1::displayUI()
	{
		ImGui::Begin("Settings lab work 1");
		ImGui::Text("No setting available!");
		ImGui::End();
	}

} // namespace M3D_ISICG
