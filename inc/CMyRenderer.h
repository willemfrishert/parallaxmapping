#ifndef MYRENDERER_JOHANNES_H
#define MYRENDERER_JOHANNES_H

#include "Basic.h"
#include "CMesh.h"
#include "ShaderObject.h"
#include "ShaderProgram.h"
#include "ShaderAttributeObject.h"
#include "ShaderUniformObject.h"
#include "ShaderUniformValue.h"
#include "ShaderUniformVector.h"
#include "ParallaxMapping.h"


//CONSTANTS:
const int KNumberOfColorMaps = 3;
const int KNumberOfShaderPrograms = 3;

//CLASS DECLARATION

/** \brief Rendering class.
*
* This class contains all the functions necessary for rendering.
*/
class CMyRenderer
{
	//PUBLIC FUNCTIONS
	//------------------
public:

	//CONSTRUCTORS
	//------------------

	/**
	* Default constructor.
	* Initializes private data, and sets the window size to ( KScreenWidth, KScreenHeight ).
	*/
	CMyRenderer();

	//DESTRUCTOR
	//------------------
	/**
	* Default constructor.
	* Frees the reserved memory (iMesh).
	*/
	~CMyRenderer();


	//MEMBER FUNCTIONS
	///Renders a scene. This function is called from OpenGL, and renders a scene.
	void RenderScene();

	///Resize the rendering window
	void ResizeScene(const int aWidth, const int aHeight);

	///Creates a Scene
	void CreateScene();

	///Sets the Trackball's X rotation
	void SetXRotation(float aXRotation);

	///Sets the Trackball's Y rotation
	void SetYRotation(float aYRotation);

	///Gets the Trackball's X rotation
	float GetXRotation();

	///Gets the Trackball's Y rotation
	float GetYRotation();

	///Sets the Trackball's X rotation
	void SetOldXRotation(float aXRotation);

	///Sets the Trackball's Y rotation
	void SetOldYRotation(float aYRotation);

	///Gets the Trackball's X rotation
	float GetOldXRotation();

	///Gets the Trackball's Y rotation
	float GetOldYRotation();

	///Sets the Trackball's X position
	void SetXPosition(float aXPosition);

	///Sets the Trackball's Y position
	void SetYPosition(float aYPosition);

	///Gets the Trackball's X position
	float GetXPosition();

	///Gets the Trackball's Y position
	float GetYPosition();

	void SetZoom(float aZoom);
	float GetZoom();

	float GetScreenHeightInPixels();

	void ToggleMapping();

	void ToggleLightMovement();


	//GETTERS
	//---------------------------------------


	//SETTERS
	//---------------------------------------


	//PRIVATE FUNCTIONS
	//------------------
private:
	//Constructors will call this one
	void InitMain();

	// Initializes the shaders
	void InitShaders();

	//Render current object
	void RenderObject( );

	// FPS calculation. Frames drawn in a second.
	void FramesPerSec();

	/// Initializie OpenGL Lights
	void InitLights();

	/// Text output. Prints text on the screen.
	void DrawText() const;

	void ReadPixel (GLubyte *image, GLubyte *pix, int x, int y, int width);

	void WritePixel (GLubyte *image, GLubyte *pix, int x, int y, int width);

	GLuint GenerateDOT3( GLuint aHeightMapId );

	// initializes the textures: texture map, bump map and normal map
	void ResetMultitexturing(GLuint* textureIds);

	// loads the textures
	void LoadTextures();

	// Computes the positional moving light position
	void ComputeLightPosition(float* position, float radius, float alpha, float cosineFreq, float height);

	// Dinamically computes lights positions
	void ComputeLightsPositions();

	void drawPointSprite(GLuint spriteId, GLfloat spriteSize);

	//DRAWING FUNCTIONS
	//------------------

	void drawRoom();

	//PUBLIC STATIC DATA
	//------------------
public:
	/// A static pointer to the current renderer object.
	/// This is used to be able to pass rendering method to OpenGL.
	static CMyRenderer* iCurrentRenderer;


	//PRIVATE DATA
	//------------------
private:
	char  iFpsCountString[ 60 ];	//< String for FPS

	float iCurrentTime;		//< Current time
	float iPreviousFrame;	///< The previous second
	float iPreviousTime;	///< The previous second

	int iFrame;				//< Current frame number (used to approximate FPS)

	// Scenario

	CMesh* wall;
	CMesh* column;
	CMesh* teapot;

	ParallaxMapping* iParallaxMapping;
	Mapping* iMappings[3];

	unsigned int iMappingIndex;

	// Trackball attributes
	float iXRotation;
	float iYRotation;
	float iOldXRotation;
	float iOldYRotation;
	float iXPosition;
	float iYPosition;
	float iZoom;
	float iScreenHeight;

	// 0: texture map; 1: bump map; 2: normal map
	GLuint rockwallTextures[3];
	GLuint rocksTextures[3];
	GLuint redbricksTextures[3];

	GLuint sprite[2];

	// Lights definitions
	GLfloat light1Position[4];
	GLfloat light0Position[4];
	GLfloat light2Position[4];
	bool iFreezeLights;

	GLfloat diffuse0[4];
	GLfloat diffuse1[4];
	GLfloat diffuse2[4];
};


//EXTERNAL FUNCTIONS
/// Reference to rendering function. Used to pass the render object's rendering function to OpenGL.
extern void RenderSceneWithRenderer();
///Reference to resize function. Used to pass the render object's resize function to OpenGL.
extern void ResizeSceneWithRenderer( int aWidth, int aHeight );
/// Reference to timer function.
extern void RenderSceneWithRendererTimerFunc(int timeValue);

#endif
