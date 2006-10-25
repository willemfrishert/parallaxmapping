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

	void setValues(char* name, T* values);

	virtual void use() const;

	// attributes
private:
	T* values;
	VECSIZE size;
};

template <class T>
ShaderAttributeVector<T>::ShaderAttributeVector(VECSIZE size)
{
	this->name = NULL;
	this->location = -1;
	this->size = size;
	values = new T[ size ];
}

template <class T>
ShaderAttributeVector<T>::~ShaderAttributeVector(void)
{
	free( this->name );
	delete values;
}

/**
* @param name the uniform attribute name
* @param value the attribute value
*/
template <class T>
void ShaderAttributeVector<T>::setValues(char* name, T* values)
{
	size_t len = strlen(name) + 1;
	this->name = (char*) malloc(sizeof(char) * len);
#ifdef WIN32
	strcpy_s(this->name, len, name);
#else
	strcpy(this->name, len, name);
#endif

	for (int i = 0; i < this->size; i++)
	{
		this->values[ i ] = values[ i ];
	}
}

template <class T>
void ShaderAttributeVector<T>::use() const
{
	setUniform(this->location, this->values, this->size);
}

