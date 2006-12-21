#pragma once

#include "Basic.h"
#include "CMesh.h"
#include "ShaderObject.h"
#include "ShaderProgram.h"
#include "ShaderAttributeObject.h"


class Mapping {

public:
	Mapping() {};
	virtual ~Mapping() {};

	virtual void setup(CMesh* aObject, GLuint* aTextureIds) = 0;
	virtual void enable() = 0;
	virtual void disable() = 0;

protected:
	virtual void resetMultitexturing(GLuint* aTextureIds) = 0;

protected:

	ShaderProgram* iShaderProgram;
	ShaderObject* iVertexShader;
	ShaderObject* iFragmentShader;
};