#ifdef WIN32
#include <Windows.h>
#include <gl/glew.h>
#endif

#include "ShaderAttributeObject.h"

#include "ExtensionsLoader.h"

ShaderAttributeObject::ShaderAttributeObject(char* name)
{
	assert( name != NULL );
	size_t len = strlen(name) + 1;
	this->name = (char*) malloc(sizeof(char) * len);
#ifdef WIN32
	strcpy_s(this->name, len, name);
#else
	strcpy(this->name, len, name);
#endif
	this->index = 0;
}

ShaderAttributeObject::~ShaderAttributeObject(void)
{
	free( name );
}

/**
* @param name
* @param shaderProgram
*/
int ShaderAttributeObject::assignLocation(GLuint shaderProgram)
{
	// this function returns a GLint because it can be a -1
	// but the index that is passed to the other functions
	// is a GLuint
	GLint loc = glGetAttribLocation(shaderProgram, name);

	if(loc == -1)
	{
		size_t size = sizeof("Failed to locate attribute variable ");
		size_t total = sizeof(char) * size + strlen(name) + 1;
		char* str = (char*)malloc( total );
#ifdef WIN32
		strcpy_s(str, size, "Failed to locate attribute variable ");
		strcat_s(str, total, name);
#else
		strcpy(str, "Failed to locate uniform variable ");
		strcat(str, name);
#endif
		ExtensionsLoader::printError("Binding warning", str);

		return -1;
	}

	this->index = loc;

	return 0;
}