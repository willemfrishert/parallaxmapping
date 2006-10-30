/**
 * @description Singleton class that loads openGL extensions
 * 
 * @file ExtensionsLoader.h
 * @author João Pedro Jorge
 */
#pragma once

class ExtensionsLoader
{
public:

	// NOTE: Load the extensions for GLSL - note that this has to be done
	// *after* the window has been opened, or we won't have a GL context
	// to query for those extensions and connect to instances of them.
	static int loadExtensions();

	static void printError(const char *errtype, const char *errmsg);

private:
	ExtensionsLoader(void);
	~ExtensionsLoader(void);
};
