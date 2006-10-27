#pragma once
#include "Basic.h"

/**
 * @description Represents a vertex attribute object.
 * 
 * @file ShaderProgram.h
 * @author João Pedro Jorge
 */
class ShaderAttributeObject
{
public:
	ShaderAttributeObject(char* name);
	virtual ~ShaderAttributeObject(void);

	int assignLocation(GLuint shaderProgram);

	GLuint getIndex() const;

protected:
	GLuint index;
	char* name;
};

inline
GLuint ShaderAttributeObject::getIndex() const
{
	return this->index;
}