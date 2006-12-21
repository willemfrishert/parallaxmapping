#include "BumpMapping.h"


BumpMapping::BumpMapping()
{
	iVertexShader = new ShaderObject(GL_VERTEX_SHADER, "shader/bumpmap.vert");
	iFragmentShader = new ShaderObject(GL_FRAGMENT_SHADER, "shader/bumpmap.frag");

	iShaderProgram = new ShaderProgram();

	iShaderProgram->attachShader( *iVertexShader);
	iShaderProgram->attachShader( *iFragmentShader );

	iTangentAttributeObject = new ShaderAttributeObject("tangent");
	iBinormalAttributeObject = new ShaderAttributeObject("binormal");
	iTbnNormalAttributeObject = new ShaderAttributeObject("tbnNormal");

	iTextureMapUniformObject = new ShaderUniformValue<int>();
	iTextureMapUniformObject->setName("textureMap");
	iTextureMapUniformObject->setValue( 0 );

	iNormalMapUniformObject = new ShaderUniformValue<int>();
	iNormalMapUniformObject->setName("normalMap");
	iNormalMapUniformObject->setValue( 1 );

	iShaderProgram->addAttributeObject( iTangentAttributeObject );
	iShaderProgram->addAttributeObject( iBinormalAttributeObject );
	iShaderProgram->addAttributeObject( iTbnNormalAttributeObject );
	iShaderProgram->addUniformObject( iTextureMapUniformObject );
	iShaderProgram->addUniformObject( iNormalMapUniformObject );

	iShaderProgram->buildProgram();
}
BumpMapping::~BumpMapping()
{
	delete iTangentAttributeObject;
	delete iTbnNormalAttributeObject;
	delete iBinormalAttributeObject;
	delete iShaderProgram;
	delete iVertexShader;
	delete iFragmentShader;
	delete iTextureMapUniformObject;
	delete iNormalMapUniformObject;
}

/**
 * NOTE: Call first setBiasValue
 * @param aObject
 * @param aTextureIds
 */
void BumpMapping::setup(CMesh* aObject, GLuint* aTextureIds)
{	
	aObject->setBinormalAttributeObject( iBinormalAttributeObject );
	aObject->setTangentAttributeObject( iTangentAttributeObject );
	aObject->setTBNNormalAttributeObject( iTbnNormalAttributeObject );
	
	resetMultitexturing(aTextureIds);
}

void BumpMapping::enable()
{
	this->iShaderProgram->useProgram();
}

void BumpMapping::disable()
{
	this->iShaderProgram->disableProgram();
}

void BumpMapping::resetMultitexturing(GLuint* aTextureIds)
{
	// initialize multitexturing
	glActiveTexture(GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_2D, aTextureIds[0]);

	glActiveTexture(GL_TEXTURE1 );
	glBindTexture(GL_TEXTURE_2D, aTextureIds[2]);
}