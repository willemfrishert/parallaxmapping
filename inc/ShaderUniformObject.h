#pragma once
#include "Basic.h"

class ShaderUniformObject
{
public:
	ShaderUniformObject(void);
	virtual ~ShaderUniformObject(void);

	int assignLocation(GLuint shaderProgram);
	
	/**
	 * binds the value to the location in memory.
	 * Equivalent to a glUniform* call
	 */
	virtual void use() const = 0;

	void setName(char* name);

	void setHasChanged(bool changed);
	bool getHasChanged() const;

	/*
	 *	Wrapping functions
	 */
protected:
	void setUniform(GLint location, float value);
	void setUniform(GLint location, int value);
	void setUniform(GLint location, float* value, int count);
	void setUniform(GLint location, GLint* value, GLsizei count );

protected:
	GLint location;
	char* name;
	bool hasChanged;
};

inline
void ShaderUniformObject::setHasChanged(bool changed)
{
	this->hasChanged = changed;
}

inline
bool ShaderUniformObject::getHasChanged() const
{
	return this->hasChanged;
}