#pragma once

#include "Basic.h"
#include "CMesh.h"
#include "ShaderObject.h"
#include "ShaderProgram.h"
#include "ShaderAttributeObject.h"
#include "ShaderUniformObject.h"
#include "ShaderUniformValue.h"
#include "ShaderUniformVector.h"
#include "Mapping.h"


class TextureMapping: public Mapping {

public:
	TextureMapping();
	~TextureMapping();

	void setup(CMesh* aObject, GLuint* aTextureIds);
	void enable();
	void disable();

protected:
	virtual void resetMultitexturing(GLuint* aTextureIds);

private:
	ShaderUniformValue<int>* iTextureMapUniformObject;
};