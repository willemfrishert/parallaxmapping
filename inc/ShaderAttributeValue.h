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

	void setValue(char* name, T value);
	
	virtual void use() const;

	// attributes
private:
	T value;
};

template <class T>
ShaderAttributeValue<T>::ShaderAttributeValue(void)
{
	this->name = NULL;
	this->location = -1;
}

template <class T>
ShaderAttributeValue<T>::~ShaderAttributeValue(void)
{
	free( this->name );
}

/**
* @param name the uniform attribute name
* @param value the attribute value
*/
template <class T>
void ShaderAttributeValue<T>::setValue(char* name, T value)
{
	this->value = value;
	size_t len = strlen(name) + 1;
	this->name = (char*) malloc(sizeof(char) * len);
#ifdef WIN32
	strcpy_s(this->name, len, name);
#else
	strcpy(this->name, len, name);
#endif
}

template <class T>
void ShaderAttributeValue<T>::use() const
{
	setUniform(this->location, this->value);
}

