#pragma once
#pragma once
#pragma once
#ifndef __LAB_WORK_5_HPP__
#define __LAB_WORK_5_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "define.hpp"
#include <vector>
#include "common/BaseCamera.hpp"
#include "common/models/triangle_mesh_model.hpp"

namespace M3D_ISICG
{
	class LabWork5 : public BaseLabWork
	{
	public:
		LabWork5() : BaseLabWork() {}
		~LabWork5();
		bool init() override;
		void animate(const float p_deltaTime) override;
		void render() override;

		void handleEvents(const SDL_Event& p_event) override;
		void displayUI() override;
		void _initCamera();
		void _updateViewMatrix();
		void _updateProjectionMatrix();



	private:

		// ================ Scene data.
		// ================

		// ================ GL data.
		GLuint program = GL_INVALID_VALUE;
		GLuint lightLoc;
		float _light = 1;
		float _fovy = 60;
		GLuint matTransfoLoc;
		GLuint viewMatrixLoc;
		GLuint projectionMatrixLoc;
		GLuint mvpMatLoc;
		GLuint mMatLoc;
		GLuint mvMatLoc;
		GLuint normalMatLoc;

		Mat4f _matTransfo;
		Mat4f _projectionMatrix;
		Mat4f _viewMatrix;
		Mat3f _normalMatrix;
		Mat4f mvpMatrix;
		BaseCamera camera;
		// ================
		TriangleMeshModel _bunny;
		float _cameraSpeed = 0.1f;
		float _cameraSensitivity = 0.1f;
		bool _ortho = false;

		// ================ Settings.
		Vec4f _bgColor = Vec4f(0.8f, 0.8f, 0.8f, 1.f); // Background color
		// ================
		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_1_HPP__
