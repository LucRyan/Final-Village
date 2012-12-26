#include "StdAfx.h"
#include "Models.h"

#include "myCamera.h"
#include "Skybox.h"


Skybox::Skybox(char* fileName, char* fileTextrue, int txWidth, int txHeight,
			   stack<mat4> *mvStack, 
			   GLuint program, GLuint skyRotate, myCamera *camera)
{
	_model = glmReadOBJ(fileName); //Loading the model
	if (!_model) exit(0); // See if it success

	glmUnitize(_model);	// Normalize vertices
	glmFacetNormals(_model); // Compute facet normals
	glmVertexNormals(_model, 90.0); // Compute vertex normals
	glmSpheremapTexture(_model); //Map the texture to Model
	glmLoadInVBO(_model); // Load the model (vertices and normals) into a vertex buffer

	_texture = glmReadPPM(fileTextrue, txWidth, txHeight);

	glActiveTexture( GL_TEXTURE0 ); 
	glBindTexture( GL_TEXTURE_2D, _textures[0] );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

	_group = _model->groups;
	_mvStack = mvStack;
	_program = program;
	_skyRotate = skyRotate;
	_camera = camera;
}

Skybox::~Skybox(void)
{
}


void Skybox::render()
{
	glUseProgram( _program );
	glBindVertexArray( _model->vao[0] );
	glBindBuffer( GL_ARRAY_BUFFER, _model->vbo[0] );
	GLuint vPosition = glGetAttribLocation( _program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0) );
	GLuint vTexCoord = glGetAttribLocation( _program, "vTexCoord" );
	glEnableVertexAttribArray( vTexCoord );
	glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(GLfloat)*4*_model->numPointsInVBO 
		+ sizeof(GLfloat)*4*_model->numPointsInVBO) );
	glUniform1i( glGetUniformLocation(_program, "texture"), 0 );

	glBindTexture( GL_TEXTURE_2D, _textures[0] );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0,
		GL_RGB, GL_UNSIGNED_BYTE, _texture );

	_mvStack->top() *= Angel::Translate( Angel::vec4( _camera->getEye().x, _camera->getEye().y-0.3, _camera->getEye().z, 0.0 ) );
	_mvStack->top() *= Angel::RotateZ( 90.0 );
	_mvStack->top() *= Angel::RotateX( _skyRotate );
	_mvStack->top() *= Angel::Scale( 2.0, 2.0, 2.0 );
	glUniformMatrix4fv(glGetUniformLocation( _program, "ModelView" ), 1, GL_TRUE, _mvStack->top());

	glDisable( GL_DEPTH_TEST );
	glmDrawVBO(_model);
	glEnable( GL_DEPTH_TEST );
}
