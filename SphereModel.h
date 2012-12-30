#pragma once

class SphereModel : public Model
{

private:
	GLubyte *_frontTex, *_backTex, *_leftTex, *_rightTex, *_topTex, *_bottomTex;
	GLuint _cubemap;
	GLfloat _RotateAngle;
	GLuint _program;
	stack<mat4> *_mvStack;
	vec4 _transVec; 
	bool _rotateFlag;
	vec3 _rotVec, _scalVec;
	int _txWidth;
	int _txHeight;
	bool _indoorFlag, _switcherFlag;

public:
	SphereModel(void);
	SphereModel(char* frontName, char* BackName, 
		char* LeftName, char* RightName, 
		char* UpperName, char* BelowName, 
		int txWidth, int txHeight,
		stack<mat4> *mvStack, GLuint program, GLuint cubemap,
		bool rotateFlag, bool indoorFlag,
		vec4 transVec, vec3 rotVec, GLfloat ScaleSize);
	~SphereModel(void);
	void render();
	void updateSwitcher(bool switcher);
};

