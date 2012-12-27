#pragma once
#include "glm.h"

class Model {
private:
	GLMmodel* _model;
	GLMgroup* _group;
	GLMmaterial* _material;
	GLuint _program;
	stack<mat4> *_mvStack;
	bool _texFlag;
    GLubyte *_textures;
	vec4 _transVec; 
	vec3 _rotVec, _scalVec;
	bool _alphaFlag;
	int _alphaIndex;
	int _txWidth;
	int _txHeight;


public:
	 
	// Initialize the Model by loading it and set shader, Light and other materials.
	//@param program the shader
	//@param texFlag, true if this model has 2D texture, for example .ppm
	//@param fileTextrue the 2D texture filename
	//@param, mvStack the Model View Stack
	//@param, transVec the Translate Vector
	//@param, rotVec the Rotate Vector
	//@param, scalVec the Scale Vector
	Model(char* fileName, 
		  stack<mat4> *mvStack, GLuint program,
		  vec4 transVec, vec3 rotVec, vec3 scalVec,
		  bool texFlag, char* fileTextrue, int txWidth, int txHeight );
	
	//Alpha Constructor
	Model(char* fileName, 
		stack<mat4> *mvStack, 
		vec4 transVec, vec3 rotVec, vec3 scalVec,
		GLuint program, bool alphaFlag, int alphaIndex );
	
	//Non-2DTexture Constructor
	Model::Model(char* fileName, 
		stack<mat4> *mvStack, GLuint program,
		vec4 transVec, vec3 rotVec, vec3 scalVec);

	//Default Constructor
	Model(void);

	//Destructor
	~Model();

	// Render the Model
	virtual void render();

	void setTraslate(vec4 transVec);
	virtual void setRotate(vec3 rotVec);
};

struct Node {
	Model* _model;
	Node* _sibling;
	Node* _child;


	Node() :
	_model(NULL), _sibling(NULL), _child(NULL) {}

	Node( Model *model, Node* sibling, Node* child ) {
		_model = model;
		_sibling = sibling;
		_child = child;
	}
};