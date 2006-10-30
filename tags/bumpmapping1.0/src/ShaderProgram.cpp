#ifdef WIN32
#include <windows.h>
#include <gl/glew.h>
#endif

#include "ShaderObject.h"
#include "ShaderUniformObject.h"
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

void ShaderProgram::addUniformObject(ShaderUniformObject* obj)
{
	this->uniformObjects.push_back( obj );
}

void ShaderProgram::addAttributeObject(ShaderAttributeObject* obj)
{
	this->attributeObjects.push_back( obj );
}

/**
 * @description links the program and assigns all the necessary locations for
 * the uniform objects connected to it.
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
		// get the locations for the uniform objects
		list< ShaderUniformObject* >::iterator it = this->uniformObjects.begin();
		for (; it != this->uniformObjects.end(); it++)
		{
			(*it)->assignLocation( this->program );
		}

		// get the locations for the attribute objects
		list< ShaderAttributeObject* >::iterator attribtIter = this->attributeObjects.begin();
		for (; attribtIter != this->attributeObjects.end(); attribtIter++)
		{
			(*attribtIter)->assignLocation( this->program );
		}
	}
}

/**
 * @description sets this program to be used as the current shader and 
 * updates all the uniform objects (connected to this program)
 * that need to be updated (i.e., that have been changed since the last call)
 */
void ShaderProgram::useProgram()
{
	glUseProgram( this->program );

	updateProgramUniformObjects();	
}

/**
 * @description updates all the uniform objects (connected to this program)
 * that need to be updated (i.e., that have been changed since the last call)
 */
void ShaderProgram::updateProgramUniformObjects()
{
	list< ShaderUniformObject* >::const_iterator uniformIter = this->uniformObjects.begin();

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