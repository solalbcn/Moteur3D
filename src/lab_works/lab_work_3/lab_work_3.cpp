#include "lab_work_3.hpp"
#include "imgui.h"
#include "utils/read_file.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "utils/random.hpp"
#include <iostream>

namespace M3D_ISICG
{
	const std::string LabWork3::_shaderFolder = "src/lab_works/lab_work_3/shaders/";

	LabWork3::~LabWork3() {
		glDeleteProgram(program);
		glDeleteBuffers(1, &_cube1.vbo);
		glDeleteBuffers(1, &_cube1.vboCouleur);
		glDeleteBuffers(1, &_cube1.ebo);
		glDisableVertexArrayAttrib(_cube1.vao, 0);
		glDeleteVertexArrays(1, &_cube1.vao);
	}

	LabWork3::Mesh LabWork3::_createCube() {
		Mesh _cube;
		_cube.sommets = { Vec3f(0.5,-0.5,0.5),Vec3f(0.5,-0.5,-0.5), Vec3f(-0.5,-0.5,-0.5), Vec3f(-0.5,-0.5,0.5), Vec3f(0.5,0.5,0.5), Vec3f(0.5,0.5,-0.5), Vec3f(-0.5,0.5,-0.5), Vec3f(-0.5,0.5,0.5) };
		_cube.couleurs = { getRandomVec3f() ,getRandomVec3f() ,getRandomVec3f() ,getRandomVec3f() ,getRandomVec3f() ,getRandomVec3f() ,getRandomVec3f() ,getRandomVec3f() };
		_cube.indices = {0,1,4,4,1,5,1,5,2,2,5,6,2,3,6,7,6,3,3,4,7,3,4,0,4,5,7,7,5,6,0,1,3,1,3,2};

		_cube.matTransfo = Mat4f(1.f);
		_cube.matTransfo = glm::scale(_cube.matTransfo, Vec3f(0.8, 0.8, 0.8));

		//Création _cube.vbo sommets
		glCreateBuffers(1, &_cube.vbo);
		glNamedBufferData(_cube.vbo, _cube.sommets.size() * sizeof(Vec3f), _cube.sommets.data(), GL_STATIC_DRAW);

		//Création _cube.vbo _cube.couleurs
		glCreateBuffers(1, &_cube.vboCouleur);
		glNamedBufferData(_cube.vboCouleur, _cube.couleurs.size() * sizeof(Vec4f), _cube.couleurs.data(), GL_STATIC_DRAW);

		glCreateVertexArrays(1, &_cube.vao);

		glEnableVertexArrayAttrib(_cube.vao, 0);
		glVertexArrayAttribFormat(_cube.vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayVertexBuffer(_cube.vao, 0, _cube.vbo, 0, sizeof(Vec3f));
		glVertexArrayAttribBinding(_cube.vao, 0, 0);

		glEnableVertexArrayAttrib(_cube.vao, 1);
		glVertexArrayAttribFormat(_cube.vao, 1, 3, GL_FLOAT, GL_TRUE, 0);
		glVertexArrayVertexBuffer(_cube.vao, 1, _cube.vboCouleur, 0, sizeof(Vec3f));
		glVertexArrayAttribBinding(_cube.vao, 1, 1);

		glCreateBuffers(1, &_cube.ebo);
		glNamedBufferData(_cube.ebo, _cube.indices.size() * sizeof(unsigned int), _cube.indices.data(), GL_STATIC_DRAW);
		glVertexArrayElementBuffer(_cube.vao, _cube.ebo);

		return _cube;
	}

	bool LabWork3::init()
	{
		glEnable(GL_DEPTH_TEST);
		std::cout << "Initializing lab work 1..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor(_bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w);

		const std::string vertexShaderStr = readFile(_shaderFolder + "lw3.vert");
		const std::string fragmentShaderStr = readFile(_shaderFolder + "lw3.frag");
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

		_cube1=_createCube();
		_cube2 = _createCube();
		_cube2.matTransfo = glm::translate(_cube2.matTransfo, Vec3f(3, 0, 0));

		_initCamera();

		lightLoc = glGetUniformLocation(program, "light");
		glProgramUniform1f(program, lightLoc, _light);
		mvpMatLoc = glGetUniformLocation(program, "mvpMatrix");





		glUseProgram(program);
		std::cout << "Done!" << std::endl;

		return true;
	}
	void LabWork3::_updateViewMatrix()
	{
		_viewMatrix = camera.getViewMatrix();
	}
	void LabWork3::_updateProjectionMatrix()
	{
		_projectionMatrix = camera.getProjectionMatrix();
	}
	void LabWork3::_initCamera()
	{
		camera.setScreenSize(1280, 720);
		camera.setPosition(Vec3f(0, 1,3));
		camera.setLookAt(Vec3f(0, 0, 0));
		_updateProjectionMatrix();
		_updateViewMatrix();

		camera.print();
	}
	void LabWork3::animate(const float p_deltaTime) {
		_cube1.matTransfo=glm::rotate(_cube1.matTransfo, p_deltaTime, Vec3f(0, 1, 1));

		_cube2.matTransfo = glm::translate(_cube2.matTransfo, Vec3f(-3, 0, 0));
		_cube2.matTransfo = glm::rotate(_cube2.matTransfo,p_deltaTime, Vec3f(0, 1, 0));
		_cube2.matTransfo = glm::translate(_cube2.matTransfo, Vec3f(3, 0, 0));
	}

	void LabWork3::render() {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mvpMatrix = _projectionMatrix * _viewMatrix * _cube1.matTransfo;
		glProgramUniformMatrix4fv(program, mvpMatLoc, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
		glBindVertexArray(_cube1.vao);
		glDrawElements(GL_TRIANGLES, _cube1.indices.size(), GL_UNSIGNED_INT, 0);

		mvpMatrix = _projectionMatrix * _viewMatrix * _cube2.matTransfo;
		glProgramUniformMatrix4fv(program, mvpMatLoc, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
		glBindVertexArray(_cube2.vao);
		glDrawElements(GL_TRIANGLES, _cube2.indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void LabWork3::handleEvents(const SDL_Event& p_event)
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

	void LabWork3::displayUI()
	{
		ImGui::Begin("Settings lab work 1");
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
