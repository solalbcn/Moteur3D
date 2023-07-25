#pragma once
#ifndef __LAB_WORK_3_HPP__
#define __LAB_WORK_3_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "define.hpp"
#include <vector>
#include "common/BaseCamera.hpp"

namespace M3D_ISICG
{
	class LabWork3 : public BaseLabWork
	{
		struct Mesh {
			std::vector<Vec3f>	sommets;
			std::vector<Vec3f> couleurs;
			std::vector<unsigned int> indices;
			Mat4f matTransfo;
			GLuint vbo = GL_INVALID_VALUE;
			GLuint vboCouleur = GL_INVALID_VALUE;
			GLuint vao = GL_INVALID_VALUE;
			GLuint ebo = GL_INVALID_VALUE;

		};
	public:
		LabWork3() : BaseLabWork() {}
		~LabWork3();
		bool init() override;
		void animate(const float p_deltaTime) override;
		void render() override;

		void handleEvents(const SDL_Event& p_event) override;
		void displayUI() override;
		void _initCamera();
		void _updateViewMatrix();
		void _updateProjectionMatrix();

		

	private:
		Mesh _createCube();
		
		// ================ Scene data.
		// ================

		// ================ GL data.
		GLuint program = GL_INVALID_VALUE;
		GLuint lightLoc;
		float _light=1;
		float _fovy = 60;
		GLuint matTransfoLoc;
		GLuint viewMatrixLoc;
		GLuint projectionMatrixLoc;
		GLuint mvpMatLoc;

		Mat4f _matTransfo;
		Mat4f _projectionMatrix;
		Mat4f _viewMatrix;
		Mat4f mvpMatrix;
		BaseCamera camera;
		// ================
		Mesh _cube1;
		Mesh _cube2;
		float _cameraSpeed = 0.1f;
		float _cameraSensitivity = 0.1f;
		bool _ortho=false;

		// ================ Settings.
		Vec4f _bgColor = Vec4f(0.8f, 0.8f, 0.8f, 1.f); // Background color
		// ================
		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_1_HPP__
