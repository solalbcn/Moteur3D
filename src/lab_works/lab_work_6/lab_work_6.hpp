#pragma once
#pragma once
#pragma once
#ifndef __LAB_WORK_6_HPP__
#define __LAB_WORK_6_HPP__

#include "GL/gl3w.h"
#include "common/BaseCamera.hpp"
#include "common/base_lab_work.hpp"
#include "common/models/triangle_mesh_model.hpp"
#include "define.hpp"
#include <vector>

namespace M3D_ISICG
{
	class LabWork6 : public BaseLabWork
	{
	public:
		LabWork6() : BaseLabWork() {}
		~LabWork6();
		bool init() override;
		void animate(const float p_deltaTime) override;
		void render() override;
		void handleEvents(const SDL_Event& p_event) override;
		void displayUI() override;
		GLuint _initProgram(std::string vertexShaderPath, std::string fragmentShaderPath);
		GLuint _initProgram(std::string fragmentShaderPath);
		void _createQuad();
		void _shadingPass();
		void _initCamera();
		void _updateViewMatrix();
		void _updateProjectionMatrix();
		void _prepareBuffer();

	private:
		// ================ Scene data.
		// ================

		// ================ GL data.
		GLuint geometryPassProgram = GL_INVALID_VALUE;
		GLuint _shadingPassProgram = GL_INVALID_VALUE;
		GLuint lightLoc;
		float  _light = 1;
		float  _fovy = 60;
		GLuint matTransfoLoc;
		GLuint viewMatrixLoc;
		GLuint projectionMatrixLoc;
		GLuint mvpMatLoc;
		GLuint mMatLoc;
		GLuint mvMatLoc;
		GLuint normalMatLoc;
		GLuint _gBufferTextures[6];
		GLenum _drawBuffers[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
		GLuint fboId;
		const char* textures[5] = { "Position","normal", "ambiant", "diffuse", "specular" };
		int current_texture_id = 0;

		GLuint vbo = GL_INVALID_VALUE;
		GLuint vao = GL_INVALID_VALUE;
		GLuint ebo = GL_INVALID_VALUE;
		std::vector<int> indiceSommet;

		Mat4f	   _matTransfo;
		Mat4f	   _projectionMatrix;
		Mat4f	   _viewMatrix;
		Mat3f	   _normalMatrix;
		Mat4f	   mvpMatrix;
		BaseCamera camera;
		// ================
		TriangleMeshModel _bunny;
		float _cameraSpeed = 0.1f;
		float _cameraSensitivity = 0.1f;
		bool  _ortho = false;

		// ================ Settings.
		Vec4f _bgColor = Vec4f(0.8f, 0.8f, 0.8f, 1.f); // Background color
		// ================
		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_1_HPP__
