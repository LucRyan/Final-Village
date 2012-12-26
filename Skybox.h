#pragma once

class Skybox : public Model
{
private:
	GLMmodel* _model;
	GLMgroup* _group;
	GLMmaterial* _material;
	GLuint _program;
	stack<mat4> *_mvStack;
	bool _texFlag;
	GLubyte *_texture;
	vec4 _transVec; 
	vec3 _rotVec, _scalVec;
	GLuint _textures[2];
	GLuint _skyRotate;
	myCamera *_camera;

public:
	Skybox(char* fileName, char* fileTextrue, int txWidth, int txHeight,
		   stack<mat4> *mvStack, 
		   GLuint program, GLuint skyRotate, myCamera *camera);
	using Model::render;
	void render();
	~Skybox(void);
};

