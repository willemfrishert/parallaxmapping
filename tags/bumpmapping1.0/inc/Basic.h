#ifndef BASIC1_H
#define BASIC1_H

//#include <windows.h>

#include <stdio.h>    // for fopen, FILE & NULL
#include <cstdlib>    // for exit and atoi
#include <iostream>   // for cout and cerr
#include <sstream>    // for istrstream and ostringstream
#include <fstream>    // for fprintf
#include <string>     // for string and strcpy()
#include <iomanip>    // for setw()
//#include <unistd.h>   // for sleep
#include <assert.h>   // for assert (bypass with  -DNDEBUG)
#include <math.h>     // for basic math
#include <ctype.h>
#include <cassert>
#include <vector>
#include <algorithm>
#include <time.h>


// ***** LINUX INCLUDE
//#include <GL/glut.h>    // Header File For The GLUT Library for linux
//#include <GL/gl.h>	// Header File For The OpenGL32 Library for linux
//#include <GL/glu.h>	// Header File For The GLu32 Library for linux

#ifdef __APPLE__
#include <OpenGL/gl.h>		// Header File For The OpenGL32 Library for Mac os x
#include <OpenGL/glu.h>		// Header File For The GLu32 Library for Mac os x
#include <GLUT/glut.h>		// Header File For The GLUT Library for Mac os x

#elif defined _WIN32
#include <GL/glew.h>
#include <GL/glut.h>    // Header File For The GLUT Library
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288   /* pi */
#endif

using namespace std;  // no need for std::cout etc.

#endif
