/**
 * @description Represents a uniform shader attribute
 * TODO: support for matrices
 * 
 * @file ShaderAttributeValue.h
 * @author João Pedro Jorge
 */
#pragma once

#include "ShaderAttributeObject.h"

template <class T>
class ShaderAttributeValue: public ShaderAttributeObject
{
public:
	ShaderAttributeValue(void);
	virtual ~ShaderAttributeValue(void);

	void setValue(T value);
	
	virtual void use() const;

	// attributes
private:
	T value;
};

template <class T>
ShaderAttributeValue<T>::ShaderAttributeValue(void)
{
}

template <class T>
ShaderAttributeValue<T>::~ShaderAttributeValue(void)
{
}

/**
 * @param value the attribute value
 */
template <class T>
void ShaderAttributeValue<T>::setValue(T value)
{
	this->value = value;

	// NOTE: we must call the upper class and MARK the
	// value as CHANGED
	this->setHasChanged( true );
}

template <class T>
void ShaderAttributeValue<T>::use() const
{
	if ( this->getHasChanged() )
	{
		setUniform(this->location, this->value);
	}
}

