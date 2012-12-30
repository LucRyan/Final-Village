#include "StdAfx.h"
#include "Models.h"
#include "SphereModel.h"
typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

extern point4 distant_light_position;
extern color4 distant_light_ambient;
extern color4 distant_light_diffuse;
extern color4 distant_light_specular;



SphereModel::SphereModel(void)
{
}

SphereModel::SphereModel(char* frontName, char* BackName, 
						 char* LeftName, char* RightName, 
						 char* UpperName, char* BelowName, 
						 int txWidth, int txHeight,
					     stack<mat4> *mvStack, GLuint program, GLuint cubemap,
						 bool rotateFlag, bool indoorFlag,
						 vec4 transVec, vec3 rotVec, GLfloat ScaleSize){

	    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		glGenTextures(1, &cubemap);

		_frontTex = glmReadPPM(frontName, txWidth, txHeight);
		_backTex = glmReadPPM(BackName, txWidth, txHeight);
		_leftTex = glmReadPPM(LeftName, txWidth, txHeight);
		_rightTex = glmReadPPM(RightName, txWidth, txHeight);
		_topTex = glmReadPPM(UpperName, txWidth, txHeight);
		_bottomTex = glmReadPPM(BelowName, txWidth, txHeight);

		_RotateAngle = 0;
		_mvStack = mvStack;
		_program = program;
		_txWidth = txWidth;
		_txHeight = txHeight;
		_transVec = transVec;
		_rotVec = rotVec;
		_scalVec.x = ScaleSize;
		_rotateFlag = rotateFlag;
		_indoorFlag = indoorFlag;
}


SphereModel::~SphereModel(void)
{
}


void SphereModel::render()
{
	if(!_switcherFlag && _indoorFlag)
		return;

	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubemap);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X ,0,GL_RGB,
		_txWidth,_txWidth,0,GL_RGB,GL_UNSIGNED_BYTE, _rightTex);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X ,0,GL_RGB,
		_txWidth,_txWidth,0,GL_RGB,GL_UNSIGNED_BYTE, _leftTex);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y ,0,GL_RGB,
		_txWidth,_txWidth,0,GL_RGB,GL_UNSIGNED_BYTE, _topTex);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y ,0,GL_RGB,
		_txWidth,_txWidth,0,GL_RGB,GL_UNSIGNED_BYTE, _bottomTex);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z ,0,GL_RGB,
		_txWidth,_txWidth,0,GL_RGB,GL_UNSIGNED_BYTE, _frontTex);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z ,0,GL_RGB,
		_txWidth,_txWidth,0,GL_RGB,GL_UNSIGNED_BYTE, _backTex);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_NEAREST);


	_RotateAngle -= 0.5;
	glUseProgram( _program );
	//_mvStack->push(_mvStack->top());
	_mvStack->top() *= Angel::Translate(_transVec);
	_mvStack->top() *= Angel::RotateX(_rotVec.x);
	_mvStack->top() *= Angel::RotateZ(_rotVec.z);
	_mvStack->top() *= Angel::RotateY(_rotateFlag?_RotateAngle:_rotVec.y);
	glUniformMatrix4fv(glGetUniformLocation( _program, "ModelView" ), 1, GL_TRUE, _mvStack->top());
	glUniform1ui(glGetUniformLocation(_program, "cubeMap"), _cubemap);
	glutSolidSphere(_scalVec.x, 200, 200);
	//_mvStack->pop();
}


void SphereModel::updateSwitcher(bool switcher){
	_switcherFlag = switcher;
}