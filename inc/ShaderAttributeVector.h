/**
* @description Represents a uniform shader attribute
* TODO: support for matrices
* 
* @file ShaderAttributeVector.h
* @author João Pedro Jorge
*/
#pragma once

#include "ShaderAttributeObject.h"


// specifies the type of uniform variable
enum VECSIZE {VEC3 = 3, VEC4 = 4};

template <class T>
class ShaderAttributeVector: public ShaderAttributeObject
{
public:
	ShaderAttributeVector(VECSIZE size);
	virtual ~ShaderAttributeVector(void);

	void setValues(T* values);

	virtual void use() const;

	// attributes
private:
	T* values;
	VECSIZE size;
};

template <class T>
ShaderAttributeVector<T>::ShaderAttributeVector(VECSIZE size)
{
	this->size = size;
	values = new T[ size ];
}

template <class T>
ShaderAttributeVector<T>::~ShaderAttributeVector(void)
{
	delete values;
}

/**
* @param name the uniform attribute name
* @param value the attribute value
*/
template <class T>
void ShaderAttributeVector<T>::setValues(T* values)
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
void ShaderAttributeVector<T>::use() const
{
	if ( this->getHasChanged() )
	{
		setUniform(this->location, this->values, this->size);
	}
}

