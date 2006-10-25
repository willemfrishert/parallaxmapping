#pragma once

#include <list>

class ShaderObject;
class ShaderAttributeObject;

using namespace std;

class ShaderProgram
{
public:
	ShaderProgram(void);
	~ShaderProgram(void);

	void attachShader(const ShaderObject& obj);

	void addUniformObject(ShaderAttributeObject* obj);

	void buildProgram();

	void useProgram();

	void disableProgram();

private:
	GLuint program;
	GLint shadersLinked;

	list< ShaderAttributeObject* > uniformObjects;
};
