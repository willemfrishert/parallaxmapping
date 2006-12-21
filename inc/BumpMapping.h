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


class BumpMapping: public Mapping {

public:
	BumpMapping();
	~BumpMapping();

	void setup(CMesh* aObject, GLuint* aTextureIds);
	void enable();
	void disable();

	void setBiasValue(float value);

protected:
	virtual void resetMultitexturing(GLuint* aTextureIds);

private:
	ShaderAttributeObject* iTangentAttributeObject;
	ShaderAttributeObject* iBinormalAttributeObject;
	ShaderAttributeObject* iTbnNormalAttributeObject;
	ShaderUniformValue<int>* iTextureMapUniformObject;
	ShaderUniformValue<int>* iHeightMapUniformObject;	
	ShaderUniformValue<int>* iNormalMapUniformObject;

	ShaderUniformValue<float>* iParallaxBiasUniformObject;
};