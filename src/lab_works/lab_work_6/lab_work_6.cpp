#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "lab_work_6.hpp"
#include "utils/random.hpp"
#include "utils/read_file.hpp"
#include <iostream>

namespace M3D_ISICG
{
	const std::string LabWork6::_shaderFolder = "src/lab_works/lab_work_6/shaders/";

	LabWork6::~LabWork6()
	{
		glDeleteProgram(geometryPassProgram);
	}

	GLuint LabWork6::_initProgram(std::string vertexShaderPath, std::string fragmentShaderPath)
	{
		const std::string vertexShaderStr = readFile(_shaderFolder + vertexShaderPath);
		const std::string fragmentShaderStr = readFile(_shaderFolder + fragmentShaderPath);
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
		GLuint program = glCreateProgram();
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
		return program;
	}
	GLuint LabWork6::_initProgram(std::string fragmentShaderPath)
	{
		const std::string fragmentShaderStr = readFile(_shaderFolder + fragmentShaderPath);
		GLuint			  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		const GLchar* fSrc = fragmentShaderStr.c_str();
		glShaderSource(fragmentShader, 1, &fSrc, NULL);
		glCompileShader(fragmentShader);
		GLint compiled;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			GLchar log[1024];
			glDeleteShader(fragmentShader);
			std::cerr << " Error compiling frag shader : " << log << std::endl;
			return false;
		}
		GLuint program = glCreateProgram();
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

		glDeleteShader(fragmentShader);
		return program;
	}
	void LabWork6::_createQuad()
	{
		std::vector<Vec2f>		 pointsTriangle;

		pointsTriangle = { Vec2f(-1, -1), Vec2f(-1, 1), Vec2f(1, 1), Vec2f(1, -1), };
		indiceSommet = { 0,1,2,0,3,2 };
		//Création vbo sommets
		glCreateBuffers(1, &vbo);
		glNamedBufferData(vbo, pointsTriangle.size() * sizeof(Vec2f), pointsTriangle.data(), GL_STATIC_DRAW);

		//Création vbo Couleurs
		glCreateVertexArrays(1, &vao);

		glEnableVertexArrayAttrib(vao, 0);
		glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vec2f));
		glVertexArrayAttribBinding(vao, 0, 0);


		glCreateBuffers(1, &ebo);
		glNamedBufferData(ebo, indiceSommet.size() * sizeof(int), indiceSommet.data(), GL_STATIC_DRAW);
		glVertexArrayElementBuffer(vao, ebo);
	}

	bool LabWork6::init()
	{

		std::cout << "Initializing lab work 1..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor(_bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w);
		glEnable(GL_DEPTH_TEST);

		geometryPassProgram = _initProgram("geometry_pass.vert", "geometry_pass.frag");
		_shadingPassProgram = _initProgram("shading_pass.frag");
		_bunny.load("bunny", "data/models/sponza/sponza.obj");
		//Sponza Scale 
		_bunny._transformation = glm::scale(_bunny._transformation, Vec3f(0.003, 0.003, 0.003));
		_initCamera();
		glUseProgram(geometryPassProgram);
		lightLoc = glGetUniformLocation(geometryPassProgram, "light");
		mvpMatLoc = glGetUniformLocation(geometryPassProgram, "uMVPMatrix");
		mMatLoc = glGetUniformLocation(geometryPassProgram, "uMMatrix");
		mvMatLoc = glGetUniformLocation(geometryPassProgram, "uMVMatrix");
		normalMatLoc = glGetUniformLocation(geometryPassProgram, "normalMatrix");
		glProgramUniform1f(geometryPassProgram, lightLoc, _light);

		_prepareBuffer();
		_createQuad();
		std::cout << "Done!" << std::endl;

		return true;
	}
	void LabWork6::_updateViewMatrix() { _viewMatrix = camera.getViewMatrix(); }
	void LabWork6::_updateProjectionMatrix() { _projectionMatrix = camera.getProjectionMatrix(); }
	void LabWork6::_initCamera()
	{
		camera.setScreenSize(1280, 720);
		camera.setPosition(Vec3f(0, 0, 2));
		camera.setLookAt(Vec3f(0, 0, 0));
		_updateProjectionMatrix();
		_updateViewMatrix();
	}
	void LabWork6::animate(const float p_deltaTime) {}

	void LabWork6::render()
	{
		glUseProgram(geometryPassProgram);
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		mvpMatrix = _projectionMatrix * _viewMatrix * _bunny._transformation;
		_normalMatrix = glm::transpose(glm::inverse(_viewMatrix * _bunny._transformation));
		glProgramUniformMatrix3fv(geometryPassProgram, normalMatLoc, 1, GL_FALSE, glm::value_ptr(_normalMatrix));
		glProgramUniform3fv(geometryPassProgram,glGetUniformLocation(geometryPassProgram, "lightPosition"),1,glm::value_ptr(_viewMatrix * Vec4f(0,1,0, 1.f)));
		glProgramUniformMatrix4fv(geometryPassProgram, mMatLoc, 1, GL_FALSE, glm::value_ptr(_bunny._transformation));
		glProgramUniformMatrix4fv(
			geometryPassProgram, mvMatLoc, 1, GL_FALSE, glm::value_ptr(_viewMatrix * _bunny._transformation));
		glProgramUniformMatrix4fv(geometryPassProgram, mvpMatLoc, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboId);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		_bunny.render(geometryPassProgram);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		_shadingPass();
		//glNamedFramebufferReadBuffer(fboId, _drawBuffers[current_texture_id]);
		//glBlitNamedFramebuffer(fboId, 0, 0, 0, _windowWidth, _windowHeight, 0, 0, _windowWidth, _windowHeight, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	}

	void LabWork6::handleEvents(const SDL_Event& p_event)
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

	void LabWork6::displayUI()
	{
		ImGui::Begin("Settings lab work 4");
		if (ImGui::SliderFloat("light", &_light, 0, 1))
			glProgramUniform1f(geometryPassProgram, lightLoc, _light);
		if (ImGui::SliderFloat("fovy", &_fovy, 0, 180))
		{
			camera.setFovy(_fovy);
			_updateProjectionMatrix();
		}
		ImGui::Checkbox("ortho", &_ortho);

		if (ImGui::ColorEdit3("background color", glm::value_ptr(_bgColor)))
			glClearColor(_bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w);
		if (ImGui::BeginListBox("listbox 1"))
		{
			for (int n = 0; n < 5; n++)
			{
				const bool is_selected = (current_texture_id == n);
				if (ImGui::Selectable(textures[n], is_selected))
					current_texture_id = n;
				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();

			}
			ImGui::EndListBox();

		}
		ImGui::End();
	}

	void LabWork6::_prepareBuffer() {
		glCreateFramebuffers(1, &fboId);
		glCreateTextures(GL_TEXTURE_2D, 6, _gBufferTextures);
		for (int i = 0; i < 5; i++) {
			glTextureStorage2D(_gBufferTextures[i], 1, GL_RGBA32F, _windowWidth, _windowHeight);
			glTextureParameteri(_gBufferTextures[i], GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(_gBufferTextures[i], GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glNamedFramebufferTexture(fboId, _drawBuffers[i], _gBufferTextures[i], 0);
		}
		glTextureStorage2D(_gBufferTextures[5], 1, GL_DEPTH_COMPONENT24, _windowWidth, _windowHeight);
		glTextureParameteri(_gBufferTextures[5], GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(_gBufferTextures[5], GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glNamedFramebufferTexture(fboId, GL_DEPTH_ATTACHMENT, _gBufferTextures[5], 0);

		glNamedFramebufferDrawBuffers(fboId, 5, _drawBuffers);

		if (glCheckNamedFramebufferStatus(fboId, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
			std::cout << "fbo good" << std::endl;
		else
			std::cout << "------------------------------------------------------------------------------------------------------" << std::endl;

	}
	void LabWork6::_shadingPass()
	{
		glUseProgram(_shadingPassProgram);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindTextureUnit(0, _gBufferTextures[0]);
		glBindTextureUnit(1, _gBufferTextures[1]);
		glBindTextureUnit(2, _gBufferTextures[2]);
		glBindTextureUnit(3, _gBufferTextures[3]);
		glBindTextureUnit(4, _gBufferTextures[4]);
		glProgramUniform3fv(geometryPassProgram, glGetUniformLocation(geometryPassProgram, "lightPosition"), 1, glm::value_ptr(_viewMatrix * Vec4f(0, 1, 0, 1.f)));
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, indiceSommet.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		for (size_t i = 0; i < 5; i++)
		{
			glBindTextureUnit(i, 0);
		}

	}

}