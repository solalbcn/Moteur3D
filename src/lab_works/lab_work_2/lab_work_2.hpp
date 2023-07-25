#pragma once
#ifndef __LAB_WORK_2_HPP__
#define __LAB_WORK_2_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "define.hpp"
#include <vector>

namespace M3D_ISICG
{
	class LabWork2 : public BaseLabWork
	{
	public:
		LabWork2() : BaseLabWork() {}
		~LabWork2();
		void exoOpt(Vec2f centre, int nbTriangles, float rayon, std::vector<Vec2f>pointsTriangle, std::vector<Vec4f> couleurs, std::vector<int> indiceSommet);
		bool init() override;
		void animate(const float p_deltaTime) override;
		void render() override;

		void handleEvents(const SDL_Event& p_event) override;
		void displayUI() override;

		

	private:
		
		// ================ Scene data.
		// ================

		// ================ GL data.
		GLuint program = GL_INVALID_VALUE;
		GLuint vbo = GL_INVALID_VALUE;
		GLuint vboCouleur = GL_INVALID_VALUE;
		GLuint vao = GL_INVALID_VALUE;
		GLuint ebo = GL_INVALID_VALUE;
		GLuint translationXLoc;
		GLuint lightLoc;
		float _light=1;
		float	_time=0;
		// ================

		// ================ Settings.
		Vec4f _bgColor = Vec4f(0.8f, 0.8f, 0.8f, 1.f); // Background color
		// ================
		std::vector<Vec2f>		 pointsTriangle;
		std::vector<Vec4f> couleurs;
		std::vector<int> indiceSommet;

		//std::vector<float>		 pointsTriangle;
		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_1_HPP__
