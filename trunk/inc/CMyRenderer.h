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



	//DRAWING FUNCTIONS
	//------------------

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

	CMesh* mesh;
	ShaderProgram* iShaderProgram;
	ShaderObject* iVertexShader;
	ShaderObject* iFragmentShader;
	
	float angle;
	GLuint textureMapId;
	GLuint heightMapId;
	GLuint normalMapId;
	ShaderAttributeObject* tangentAttributeObject;
	ShaderAttributeObject* binormalAttributeObject;
	ShaderAttributeObject* tbnNormalAttributeObject;
	ShaderUniformValue<int>* textureMapUniformObject;
	ShaderUniformValue<int>* heightMapUniformObject;	
	ShaderUniformValue<int>* normalMapUniformObject;

	// Trackball attributes
	float iXRotation;
	float iYRotation;
	float iOldXRotation;
	float iOldYRotation;
};


//EXTERNAL FUNCTIONS
/// Reference to rendering function. Used to pass the render object's rendering function to OpenGL.
extern void RenderSceneWithRenderer();
///Reference to resize function. Used to pass the render object's resize function to OpenGL.
extern void ResizeSceneWithRenderer( int aWidth, int aHeight );
/// Reference to timer function.
extern void RenderSceneWithRendererTimerFunc(int timeValue);

#endif
