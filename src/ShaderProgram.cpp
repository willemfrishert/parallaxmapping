#ifdef WIN32
#include <windows.h>
#include <gl/glew.h>
#endif

#include "ShaderObject.h"
#include "ShaderAttributeObject.h"
#include "ShaderProgram.h"
#include "ExtensionsLoader.h"

ShaderProgram::ShaderProgram(void)
{
	this->program = glCreateProgram();
}

ShaderProgram::~ShaderProgram(void)
{
}

/**
 * @param obj
 */
void ShaderProgram::attachShader(const ShaderObject& obj)
{
	glAttachShader( this->program, obj.getShaderId() );
}

void ShaderProgram::addUniformObject(ShaderAttributeObject* obj)
{
	//int status = obj->assignLocation( this->program );

	//if (status != -1)
	{
		this->uniformObjects.push_back( obj );
	}
}

/**
 */
void ShaderProgram::buildProgram()
{
	// Link the program object and print out the info log.
	glLinkProgram( this->program );
	glGetProgramiv( this->program, GL_LINK_STATUS, &this->shadersLinked );

	if( this->shadersLinked == GL_FALSE )
	{
		char statusString[4096];
		glGetProgramInfoLog( this->program, sizeof(statusString), NULL, statusString );
		ExtensionsLoader::printError("Program object linking error", statusString);
	}
	else
	{
		list< ShaderAttributeObject* >::iterator it = this->uniformObjects.begin();
		for (; it != this->uniformObjects.end(); it++)
		{
			(*it)->assignLocation( this->program );
		}
	}
}

/**
 */
void ShaderProgram::useProgram()
{
	glUseProgram( this->program );

	list< ShaderAttributeObject* >::const_iterator uniformIter = this->uniformObjects.begin();

	for (; uniformIter != this->uniformObjects.end() ; uniformIter++)
	{
		(*uniformIter)->use();
	}
}

/**
 */
void ShaderProgram::disableProgram()
{
	glUseProgram( 0 );
}