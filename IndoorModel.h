#pragma once
class IndoorModel : public Model
{
protected:
	bool _switcher;

public:
	IndoorModel(void);

	IndoorModel(char* fileName, 
		stack<mat4> *mvStack, GLuint program,
		vec4 transVec, vec3 rotVec, vec3 scalVec,
		bool texFlag, char* fileTextrue, int txWidth, int txHeight );
	
	using Model::render;
	using Model::updateSwitcher;
	void updateSwitcher(bool switcher);
	void render();

	~IndoorModel(void);
};

