/**
* @description Represents a uniform shader attribute
* TODO: support for matrices
* 
* @file ShaderUniformVector.h
* @author João Pedro Jorge
*/
#pragma once

#include "ShaderUniformObject.h"


// specifies the type of uniform variable
enum VECSIZE {VEC3 = 3, VEC4 = 4};

template <class T>
class ShaderUniformVector: public ShaderUniformObject
{
public:
	ShaderUniformVector(VECSIZE size);
	virtual ~ShaderUniformVector(void);

	void setValues(T* values);

	virtual void use() const;

	// attributes
private:
	T* values;
	VECSIZE size;
};

template <class T>
ShaderUniformVector<T>::ShaderUniformVector(VECSIZE size)
{
	this->size = size;
	values = new T[ size ];
}

template <class T>
ShaderUniformVector<T>::~ShaderUniformVector(void)
{
	delete values;
}

/**
* @param name the uniform attribute name
* @param value the attribute value
*/
template <class T>
void ShaderUniformVector<T>::setValues(T* values)
{
	for (int i = 0; i < this->size; i++)
	{
		this->values[ i ] = values[ i ];
	}

	// NOTE: we must call the upper class and MARK the
	// value as CHANGED
	this->setHasChanged( true );
}

template <class T>
void ShaderUniformVector<T>::use() const
{
	if ( this->getHasChanged() )
	{
		setUniform(this->location, this->values, this->size);
	}
}

