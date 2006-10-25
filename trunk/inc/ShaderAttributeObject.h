#pragma once

class ShaderAttributeObject
{
public:
	ShaderAttributeObject(void);
	virtual ~ShaderAttributeObject(void);

	int assignLocation(GLuint shaderProgram);
	
	/**
	 * binds the value to the location in memory.
	 * Equivalent to a glUniform* call
	 */
	virtual void use() const = 0;

	/*
	 *	Wrapping functions
	 */
protected:
	void setUniform(GLint location, float value) const;
	void setUniform(GLint location, int value) const;
	void setUniform(GLint location, float* value, int count) const;
	void setUniform(GLint location, int* value, int count ) const;

protected:
	GLint location;
	char* name;
};
