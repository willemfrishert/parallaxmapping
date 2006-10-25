#include <Windows.h>
#include <gl/glew.h>
#include "ShaderAttributeObject.h"

#include "ExtensionsLoader.h"

ShaderAttributeObject::ShaderAttributeObject(void)
{

}

ShaderAttributeObject::~ShaderAttributeObject(void)
{
	if ( name != NULL)
	{
		free( name );
	}
}

/**
 * @param name
 * @param shaderProgram
 */
int ShaderAttributeObject::assignLocation(GLuint shaderProgram)
{
	if ( name != NULL)
	{
		this->location = glGetUniformLocation(shaderProgram, name);
	}
	
	if(this->location == -1)
	{
		size_t size = sizeof("Failed to locate uniform variable ");
		size_t total = sizeof(char) * size + strlen(name) + 1;
		char* str = (char*)malloc( total );
#ifdef WIN32
		strcpy_s(str, size, "Failed to locate uniform variable ");
		strcat_s(str, total, name);
#else
		strcpy(str, "Failed to locate uniform variable ");
		strcat(str, name);
#endif
		ExtensionsLoader::printError("Binding warning", str);

		return -1;
	}

	return 0;
}

/**
 * @param location openGL variable location
 * @param value the value
 * @param count the number of elements
 */
void ShaderAttributeObject::setUniform(GLint location, float value) const
{
	glUniform1f(location, value);
}

/**
 * @param location openGL variable location
 * @param value the value
 * @param count the number of elements
 */
void ShaderAttributeObject::setUniform(GLint location, int value) const
{
	glUniform1i(location, value);
}

/**
 * @param location openGL variable location
 * @param value the value
 * @param count the number of elements
 */
void ShaderAttributeObject::setUniform(GLint location, float* value, int count) const
{
	if ( count == 3 )
	{
		glUniform3f(location, value[0], value[1], value[2]);
	} else if (count == 4)
	{
		glUniform4f(location, value[0], value[1], value[2], value[3]);
	} else
	{
		glUniform1fv(location, count, value);
	}
}

/**
 * @param location openGL variable location
 * @param value the value
 * @param count the number of elements
 */
void ShaderAttributeObject::setUniform(GLint location, int* value, int count ) const
{
	if ( count == 3 )
	{
		glUniform3i(location, value[0], value[1], value[2]);
	} else if (count == 4)
	{
		glUniform4i(location, value[0], value[1], value[2], value[3]);
	} else
	{
		glUniform1iv(location, count, value);
	}
}