/**
 * @description Represents a uniform shader attribute
 * TODO: support for matrices
 * 
 * @file ShaderUniformValue.h
 * @author João Pedro Jorge
 */
#pragma once

#include "ShaderUniformObject.h"

template <class T>
class ShaderUniformValue: public ShaderUniformObject
{
public:
	ShaderUniformValue(void);
	virtual ~ShaderUniformValue(void);

	void setValue(T value);
	
	virtual void use();

	// attributes
private:
	T value;
};

template <class T>
ShaderUniformValue<T>::ShaderUniformValue(void)
{
}

template <class T>
ShaderUniformValue<T>::~ShaderUniformValue(void)
{
}

/**
 * @param value the attribute value
 */
template <class T>
void ShaderUniformValue<T>::setValue(T value)
{
	this->value = value;

	// NOTE: we must call the upper class and MARK the
	// value as CHANGED
	this->setHasChanged( true );
}

template <class T>
void ShaderUniformValue<T>::use()
{
	if ( this->getHasChanged() )
	{
		setUniform(this->location, this->value);
	}
}

