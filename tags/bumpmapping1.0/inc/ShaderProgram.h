#pragma once

#include <list>

class ShaderObject;
class ShaderUniformObject;
class ShaderAttributeObject;

using namespace std;

class ShaderProgram
{
public:
	ShaderProgram(void);
	~ShaderProgram(void);

	void attachShader(const ShaderObject& obj);

	void addUniformObject(ShaderUniformObject* obj);

	void addAttributeObject(ShaderAttributeObject* obj);

	void buildProgram();

	void useProgram();

	void disableProgram();

	void updateProgramUniformObjects();

private:
	GLuint program;
	GLint shadersLinked;

	list< ShaderUniformObject* > uniformObjects;
	list< ShaderAttributeObject* > attributeObjects;
};
