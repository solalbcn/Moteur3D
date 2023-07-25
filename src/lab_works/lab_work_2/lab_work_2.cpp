#include "lab_work_2.hpp"
#include "imgui.h"
#include "utils/read_file.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "utils/random.hpp"
#include <iostream>

namespace M3D_ISICG
{
	const std::string LabWork2::_shaderFolder = "src/lab_works/lab_work_2/shaders/";

	LabWork2::~LabWork2() {
		glDeleteProgram(program);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &vboCouleur);
		glDeleteBuffers(1, &ebo);
		glDisableVertexArrayAttrib(vao, 0);
		glDeleteVertexArrays(1, &vao);
	}
	void LabWork2::exoOpt(Vec2f centre, int nbTriangles, float rayon, std::vector<Vec2f>pointsTriangle, std::vector<Vec4f> couleurs, std::vector<int> indiceSommet)
	{
		pointsTriangle = { Vec2f(centre.x,centre.y) };
		couleurs = { Vec4f(getRandomVec3f(),1) };
		Vec2f premierPoint = Vec2f(centre.x + rayon, centre.y);
		pointsTriangle.push_back(premierPoint);
		couleurs.push_back(Vec4f(getRandomVec3f(), 1));

		for (int i = 1; i < nbTriangles; i++)
		{
			pointsTriangle.push_back(Vec2f(centre.x+ rayon * glm::cos(i * 2 * glm::pi<float>() / nbTriangles),centre.y+rayon * glm::cos(i * 2 * glm::pi<float>() / nbTriangles)));
			couleurs.push_back(Vec4f(getRandomVec3f(), 1));
		}
		for (int i = 1; i < pointsTriangle.size(); i += 1)
		{
			indiceSommet.push_back(0);
			indiceSommet.push_back(i);
			if (i + 1 < pointsTriangle.size())
				indiceSommet.push_back(i + 1);
			else
				indiceSommet.push_back(1);
		}
		for (int i = 0; i < indiceSommet.size(); i++)
		{
			std::cout << indiceSommet[i] << std::endl;
		}
	}

	bool LabWork2::init()
	{
		std::cout << "Initializing lab work 1..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor(_bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w);

		const std::string vertexShaderStr = readFile(_shaderFolder + "lw2.vert");
		const std::string fragmentShaderStr = readFile(_shaderFolder + "lw2.frag");
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
		/*pointsTriangle = {Vec2f(-0.5, 0.5), Vec2f(0.5, 0.5), Vec2f(0.5, -0.5), Vec2f(-0.5, -0.5),};
		indiceSommet = { 0,1,2,0,3,2 };
		couleurs = { Vec4f(1,0,0,1),Vec4f(0,1,0,1),Vec4f(0,0,1,1),Vec4f(1,0,1,1) };
		*/

		Vec2f centre = Vec2f(0.3, -0.2);
		int nbTriangles = 64;
		float rayon = 0.5;

		exoOpt(centre, nbTriangles, rayon, pointsTriangle, couleurs, indiceSommet);
		pointsTriangle = {Vec2f(centre.x,centre.y)};
		couleurs = { Vec4f(getRandomVec3f(),1) };
		Vec2f premierPoint = Vec2f(centre.x+rayon, centre.y);
		pointsTriangle.push_back(premierPoint);
		couleurs.push_back(Vec4f(getRandomVec3f(), 1));

		for (int i = 1; i < nbTriangles; i++)
		{
			float angle = (i * 2 * glm::pi<float>()) / nbTriangles;
			pointsTriangle.push_back(Vec2f((centre.x + rayon * glm::cos(angle)), (centre.y + rayon * glm::sin(angle))));
			couleurs.push_back(Vec4f(getRandomVec3f(), 1));
		}
		for (int i = 1; i < pointsTriangle.size(); i ++)
		{
			indiceSommet.push_back(0);
			indiceSommet.push_back(i);
			if (i + 1 < pointsTriangle.size())
				indiceSommet.push_back(i + 1);
			else
				indiceSommet.push_back(1);
		}
		



		translationXLoc = glGetUniformLocation(program,"uTranslationX");
		lightLoc = glGetUniformLocation(program, "light");
		glProgramUniform1f(program, lightLoc, _light);
		
		//Création vbo sommets
		glCreateBuffers(1, &vbo);
		glNamedBufferData(vbo, pointsTriangle.size() * sizeof(Vec2f), pointsTriangle.data(), GL_STATIC_DRAW);

		//Création vbo Couleurs
		glCreateBuffers(1, &vboCouleur);
		glNamedBufferData(vboCouleur, couleurs.size() * sizeof(Vec4f), couleurs.data(), GL_STATIC_DRAW);

		glCreateVertexArrays(1, &vao);

		glEnableVertexArrayAttrib(vao, 0);
		glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vec2f));
		glVertexArrayAttribBinding(vao, 0, 0);

		glEnableVertexArrayAttrib(vao, 1);
		glVertexArrayAttribFormat(vao, 1, 4, GL_FLOAT, GL_TRUE, 0);
		glVertexArrayVertexBuffer(vao, 1, vboCouleur, 0, sizeof(Vec4f));
		glVertexArrayAttribBinding(vao, 1, 1);

		glCreateBuffers(1, &ebo);
		glNamedBufferData(ebo, indiceSommet.size() * sizeof(int), indiceSommet.data(), GL_STATIC_DRAW);
		glVertexArrayElementBuffer(vao, ebo);

		glUseProgram(program);
		std::cout << "Done!" << std::endl;

		return true;
	}

	void LabWork2::animate(const float p_deltaTime) {
		_time += p_deltaTime;
		glProgramUniform1f(program, translationXLoc, glm::sin(_time));
	}

	void LabWork2::render() {
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, indiceSommet.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void LabWork2::handleEvents(const SDL_Event& p_event)
	{}

	void LabWork2::displayUI()
	{
		ImGui::Begin("Settings lab work 1");
		if (ImGui::SliderFloat("light", &_light, 0, 1))
			glProgramUniform1f(program, lightLoc, _light);

		if(ImGui::ColorEdit3("background color", glm::value_ptr(_bgColor)))
			glClearColor(_bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w);

		ImGui::End();
	}

} // namespace M3D_ISICG
