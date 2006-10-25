#include "ExtensionsLoader.h"

#include <windows.h>
#include <stdio.h>
#include <gl/glew.h>

static bool loadedExtensions = false;

/*
* loadExtensions() - Load OpenGL extensions for anything above OpenGL
* version 1.1. (This is a requirement from Windows, not from OpenGL.)
*/
int ExtensionsLoader::loadExtensions() 
{
#if 0
	const char * vendor = glGetString(GL_VENDOR);
	const char * renderer = glGetString(GL_RENDERER);
	const char * version = glGetString(GL_VERSION);
	const char * extensions = glGetString(GL_EXTENSIONS);
	char info[2048];

	sprintf(info, "Vendor.....: %s\n"
		"Renderer...: %s\n"
		"Version....: %s\n",
		vendor, renderer, version);

	printError("GL INFO", info);
#endif
	int cc = 0;

	if( ! loadedExtensions ) {

#ifdef WIN32
		FILE * out;
		fopen_s(&out, "vglinit.log", "w+");
#else
		FILE * out = fopen("vglinit.log", "w+");
#endif

		GLenum err = glewInit();
		if (err != GLEW_OK) {
			printError("Loading Extensions",
				(const char*)glewGetErrorString(err));
			cc = -1;
			loadedExtensions = false;
		} else {
			loadedExtensions = true;
		}

		fclose(out);
	}


	return cc;
}

/*
* printError() - Signal an error. MessageBox() is a Windows-specific function,
* rewrite this to print the error message to the console to make it portable!
*/
void ExtensionsLoader::printError(const char *errtype, const char *errmsg)
{
#ifdef WIN32
	// NOTE: the cast from const char* to LPCWSTR works because I defined in
	// Project: Configuration Properties -> General page, 
	// change the "Character Set" setting from "Use Unicode Character Set" to 
	// "Use Multi-Byte Character Set".
	MessageBox(NULL, errmsg, errtype, MB_OK|MB_ICONEXCLAMATION);
#else
	printf("%s\n", errtype);
	printf("%s\n", errmsg);
#endif
}