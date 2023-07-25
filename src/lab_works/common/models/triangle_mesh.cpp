#include "triangle_mesh.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>
#include "glm/gtx/string_cast.hpp"

namespace M3D_ISICG
{
	TriangleMesh::TriangleMesh( const std::string &				  p_name,
								const std::vector<Vertex> &		  p_vertices,
								const std::vector<unsigned int> & p_indices,
								const Material &				  p_material ) :
		_name( p_name ),
		_vertices( p_vertices ), _indices( p_indices ), _material( p_material )
	{
		_vertices.shrink_to_fit();
		_indices.shrink_to_fit();
		_setupGL();
	}

	void TriangleMesh::render( const GLuint p_glProgram ) const
	{
		glProgramUniform3fv(p_glProgram, glGetUniformLocation(p_glProgram, "uAmbientColor"), 1, glm::value_ptr(_material._ambient));
		glProgramUniform3fv(p_glProgram, glGetUniformLocation(p_glProgram, "uDiffuse"), 1,glm::value_ptr(_material._diffuse));
		glProgramUniform3fv(p_glProgram, glGetUniformLocation(p_glProgram, "uSpecular"), 1, glm::value_ptr(_material._specular));
		glProgramUniform1f(p_glProgram, glGetUniformLocation(p_glProgram, "uShiny"), _material._shininess);
		glProgramUniform1i( p_glProgram, glGetUniformLocation( p_glProgram, "uHasDiffuseMap" ), _material._hasDiffuseMap );
		glProgramUniform1i( p_glProgram, glGetUniformLocation( p_glProgram, "uHasAmbiantMap" ), _material._hasAmbientMap );
		glProgramUniform1i( p_glProgram, glGetUniformLocation( p_glProgram, "uHasSpecularMap" ), _material._hasSpecularMap );
		glProgramUniform1i( p_glProgram, glGetUniformLocation( p_glProgram, "uHasShininessMap" ), _material._hasShininessMap );
		glProgramUniform1i( p_glProgram, glGetUniformLocation( p_glProgram, "uHasNormalsMap" ), _material._hasNormalsMap );

		if ( _material._hasAmbientMap )
			glBindTextureUnit( 0, _material._ambientMap._id );
		if (_material._hasDiffuseMap)
			glBindTextureUnit( 1, _material._diffuseMap._id );
		if ( _material._hasSpecularMap )
			glBindTextureUnit( 2, _material._specularMap._id );
		if ( _material._hasShininessMap )
			glBindTextureUnit( 3, _material._shininessMap._id );
		if ( _material._hasNormalsMap )
			glBindTextureUnit( 4, _material._normalsMap._id );

		glBindVertexArray(_vao);
		glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
	}

	void TriangleMesh::cleanGL()
	{
		glDisableVertexArrayAttrib( _vao, 0 );
		glDisableVertexArrayAttrib( _vao, 1 );
		glDisableVertexArrayAttrib( _vao, 2 );
		glDisableVertexArrayAttrib( _vao, 3 );
		glDisableVertexArrayAttrib( _vao, 4 );
		glDeleteVertexArrays( 1, &_vao );
		glDeleteBuffers( 1, &_vbo );
		glDeleteBuffers( 1, &_ebo );
	}

	void TriangleMesh::_setupGL()
	{		
		//Création vbo sommets
		glCreateBuffers(1, &_vbo);
		glNamedBufferData(_vbo, _vertices.size() * sizeof(Vertex), _vertices.data(), GL_STATIC_DRAW);
		glCreateVertexArrays(1, &_vao);

		glEnableVertexArrayAttrib(_vao, 0);
		glVertexArrayAttribFormat(_vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex,_position));
		glVertexArrayAttribBinding(_vao, 0, 0);

		glEnableVertexArrayAttrib(_vao, 1);
		glVertexArrayAttribFormat(_vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, _normal));
		glVertexArrayAttribBinding(_vao, 1, 0);

		glEnableVertexArrayAttrib(_vao, 2);
		glVertexArrayAttribFormat(_vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, _texCoords));
		glVertexArrayAttribBinding(_vao, 2, 0);

		glEnableVertexArrayAttrib(_vao, 3);
		glVertexArrayAttribFormat(_vao, 3, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, _tangent));
		glVertexArrayAttribBinding(_vao, 3, 0);

		glEnableVertexArrayAttrib(_vao, 4);
		glVertexArrayAttribFormat(_vao, 4, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, _bitangent));
		glVertexArrayAttribBinding(_vao, 4, 0);

		glVertexArrayVertexBuffer(_vao, 0, _vbo, 0, sizeof(Vertex));

		glCreateBuffers(1, &_ebo);
		glNamedBufferData(_ebo, _indices.size() * sizeof(unsigned int), _indices.data(), GL_STATIC_DRAW);
		glVertexArrayElementBuffer(_vao, _ebo);
	}
} // namespace M3D_ISICG
