#include <stdio.h>
#include <windows.h>
#include <io.h>
#include <gl/glew.h>

#include "ShaderObject.h"
#include "ExtensionsLoader.h"

ShaderObject::ShaderObject(GLenum shaderType, char* filename)
{
	init(filename, shaderType);
}

ShaderObject::~ShaderObject(void)
{
}

GLuint ShaderObject::getShaderId() const
{
	return this->shaderId;
}

/************************************************************************/
/* ###### PRIVATE FUNCTIONS ######                                      */
/************************************************************************/

/**
 * @description Creates and initializes the shader
 * @param filename
 * @param shaderType
 */
void ShaderObject::init(char* filename, GLenum shaderType)
{
	unsigned char *shaderAssembly;

	// Create the vertex shader.
	this->shaderId = glCreateShader( shaderType );

	shaderAssembly = readShaderFile( filename );
	
	this->shaderStrings[0] = (char*)shaderAssembly;
	
	// suply the source to openGL
	glShaderSource( this->shaderId, 1, this->shaderStrings, NULL );

	// compile the shader source
	glCompileShader( this->shaderId);
	free((void *)shaderAssembly);

	GLint compiled;

	// get compilation status
	glGetShaderiv( this->shaderId, GL_COMPILE_STATUS, &compiled );

	// if some error occured, print the error msg
	if(compiled == GL_FALSE)
	{
		char statusString[4096];
		glGetShaderInfoLog(this->shaderId, sizeof( statusString ), NULL, statusString);
		ExtensionsLoader::printError("Vertex shader compile error", statusString);
	}
}


/*
 * readShaderFile(filename) - read a shader source string from a file
 */
unsigned char* ShaderObject::readShaderFile(const char *filename)
{
#ifdef WIN32
	FILE * file;
	fopen_s(&file, filename, "r");
#else
	FILE *file = fopen(filename, "r");
#endif
	
	int bytesinfile;
	unsigned char *buffer;
	int bytesread;

	if(file == NULL)
	{
		ExtensionsLoader::printError("ERROR", "Cannot open shader file!");
		return 0;
	}

#ifdef WIN32
	bytesinfile = _filelength(_fileno(file));
#else
	bytesinfile = filelength(fileno(file));
#endif
	
	buffer = (unsigned char*)malloc(bytesinfile);
	bytesread = (int)fread( buffer, 1, bytesinfile, file);
	buffer[bytesread] = 0; // Terminate the string with 0
	fclose(file);

	return buffer;
}