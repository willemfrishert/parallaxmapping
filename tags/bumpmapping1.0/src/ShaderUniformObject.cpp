#ifdef WIN32
#include <Windows.h>
#include <gl/glew.h>
#endif

#include "ShaderUniformObject.h"

#include "ExtensionsLoader.h"

ShaderUniformObject::ShaderUniformObject(void)
: hasChanged( true )
{
	this->name = NULL;
	this->location = -1;
}

ShaderUniformObject::~ShaderUniformObject(void)
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
int ShaderUniformObject::assignLocation(GLuint shaderProgram)
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
* @param name the uniform attribute name
*/
void ShaderUniformObject::setName(char* name)
{
	size_t len = strlen(name) + 1;
	this->name = (char*) malloc(sizeof(char) * len);
#ifdef WIN32
	strcpy_s(this->name, len, name);
#else
	strncpy(this->name, name, len);
#endif
}

/**
 * @param location openGL variable location
 * @param value the value
 * @param count the number of elements
 */
void ShaderUniformObject::setUniform(GLint location, float value)
{
	glUniform1f(location, value);

	// mark it as not changed because it has been registered now
	this->setHasChanged( false );
}

/**
 * @param location openGL variable location
 * @param value the value
 * @param count the number of elements
 */
void ShaderUniformObject::setUniform(GLint location, int value)
{
	glUniform1i(location, value);

	// mark it as not changed because it has been registered now
	this->setHasChanged( false );
}

/**
 * @param location openGL variable location
 * @param value the value
 * @param count the number of elements
 */
void ShaderUniformObject::setUniform(GLint location, float* value, int count)
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

	// mark it as not changed because it has been registered now
	this->setHasChanged( false );
}

/**
 * @param location openGL variable location
 * @param value the value
 * @param count the number of elements
 */
void ShaderUniformObject::setUniform(GLint location, GLint* value, GLsizei count )
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

	// mark it as not changed because it has been registered now
	this->setHasChanged( false );
}