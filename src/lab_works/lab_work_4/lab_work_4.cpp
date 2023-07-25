#include "lab_work_4.hpp"
#include "imgui.h"
#include "utils/read_file.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "utils/random.hpp"
#include <iostream>

namespace M3D_ISICG
{
	const std::string LabWork4::_shaderFolder = "src/lab_works/lab_work_4/shaders/";

	LabWork4::~LabWork4() {
		glDeleteProgram(program);
	}

	bool LabWork4::init()
	{
		glEnable(GL_DEPTH_TEST);
		std::cout << "Initializing lab work 1..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor(_bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w);

		const std::string vertexShaderStr = readFile(_shaderFolder + "mesh.vert");
		const std::string fragmentShaderStr = readFile(_shaderFolder + "mesh.frag");
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

		_bunny.load("bunny", "data/models/bunny/bunny.obj");
		//_bunny._transformation = glm::scale(_bunny._transformation, Vec3f(0.003, 0.003, 0.003));

		_initCamera();

		lightLoc = glGetUniformLocation(program, "light");
		glProgramUniform1f(program, lightLoc, _light);
		mvpMatLoc = glGetUniformLocation(program, "uMVPMatrix");

		mMatLoc = glGetUniformLocation(program, "uMMatrix");
		mvMatLoc = glGetUniformLocation(program, "uMVMatrix");
		normalMatLoc = glGetUniformLocation(program, "normalMatrix");

		glUseProgram(program);
		std::cout << "Done!" << std::endl;

		return true;
	}
	void LabWork4::_updateViewMatrix()
	{
		_viewMatrix = camera.getViewMatrix();
	}
	void LabWork4::_updateProjectionMatrix()
	{
		_projectionMatrix = camera.getProjectionMatrix();
	}
	void LabWork4::_initCamera()
	{
		camera.setScreenSize(1280, 720);
		camera.setPosition(Vec3f(0, 1,3));
		camera.setLookAt(Vec3f(0, 0, 0));
		_updateProjectionMatrix();
		_updateViewMatrix();
	}
	void LabWork4::animate(const float p_deltaTime) {
	}

	void LabWork4::render() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mvpMatrix = _projectionMatrix * _viewMatrix * _bunny._transformation;
		_normalMatrix = glm::transpose(glm::inverse(_viewMatrix * _bunny._transformation));
		glProgramUniformMatrix3fv(program, normalMatLoc, 1, GL_FALSE, glm::value_ptr(_normalMatrix));
		glProgramUniform3fv(program, glGetUniformLocation(program, "lightPos"), 1, glm::value_ptr(_viewMatrix*Vec4f(camera._position,0.f)));
		glProgramUniformMatrix4fv(program, mMatLoc, 1, GL_FALSE, glm::value_ptr(_bunny._transformation));
		glProgramUniformMatrix4fv(program, mvMatLoc, 1, GL_FALSE, glm::value_ptr(_viewMatrix*_bunny._transformation));
		glProgramUniformMatrix4fv(program, mvpMatLoc, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
		_bunny.render(program);

	}

	void LabWork4::handleEvents(const SDL_Event& p_event)
	{
		if (p_event.type == SDL_KEYDOWN)
		{
			switch (p_event.key.keysym.scancode)
			{
			case SDL_SCANCODE_W: // Front
				camera.moveFront(_cameraSpeed);
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_S: // Back
				camera.moveFront(-_cameraSpeed);
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_A: // Left
				camera.moveRight(-_cameraSpeed);
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_D: // Right
				camera.moveRight(_cameraSpeed);
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_R: // Up
				camera.moveUp(_cameraSpeed);
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_F: // Bottom
				camera.moveUp(-_cameraSpeed);
				_updateViewMatrix();
				break;
			default: break;
			}
		}

		// Rotate when left click + motion (if not on Imgui widget).
		if (p_event.type == SDL_MOUSEMOTION && p_event.motion.state & SDL_BUTTON_LMASK
			&& !ImGui::GetIO().WantCaptureMouse)
		{
			camera.rotate(p_event.motion.xrel * _cameraSensitivity, p_event.motion.yrel * _cameraSensitivity);
			_updateViewMatrix();
		}
	}

	void LabWork4::displayUI()
	{
		ImGui::Begin("Settings lab work 4");
		if (ImGui::SliderFloat("light", &_light, 0, 1))
			glProgramUniform1f(program, lightLoc, _light);
		if (ImGui::SliderFloat("fovy", &_fovy, 0, 180))
		{
			camera.setFovy(_fovy);
			_updateProjectionMatrix();
		}
		ImGui::Checkbox("ortho", &_ortho);


		if(ImGui::ColorEdit3("background color", glm::value_ptr(_bgColor)))
			glClearColor(_bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w);

		ImGui::End();
	}

} // namespace M3D_ISICG
