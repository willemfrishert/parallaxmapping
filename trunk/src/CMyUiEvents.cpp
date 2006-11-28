
//INCLUDE FILES
#include "CMyUiEvents.h"

//CONSTANTS
#ifdef USE_RADIANS
const float KAngleChangeAmount = 0.001f;
#elif defined USE_DEGREES
const float KAngleChangeAmount = 0.05f;
#endif

//INIT STATIC DATA
CMyUiEvents* CMyUiEvents::iCurrentUi = 0;

//Constructor, saves a pointer to the renderer
CMyUiEvents::CMyUiEvents( CMyRenderer* aRenderer )
{
	iRenderer = aRenderer;
	CMyUiEvents::iCurrentUi = this;
}

//Destructor
CMyUiEvents::~CMyUiEvents()
{
}

//Used to handle the keyboard input (ASCII Characters)
void CMyUiEvents::ProcessNormalKeys(unsigned char key, int x, int y)
{

}

// Used to handle the keyboard input (not ASCII Characters)
void CMyUiEvents::ProcessCursorKeys(int key, int x, int y)
{
	switch (key)
	{
		//MODIFY X
	case GLUT_KEY_UP:
		break;
	case GLUT_KEY_DOWN:
		break;
		//MODIFY Y
	case GLUT_KEY_LEFT:
		break;
	case GLUT_KEY_RIGHT:
		break;
		//MODIFY Z
	case GLUT_KEY_PAGE_UP:
		break;
	case GLUT_KEY_PAGE_DOWN:
		break;
		//RESET Base angle as well
	case GLUT_KEY_HOME:
		break;
		//FREEZE
	default:
		break;
	}
}

void CMyUiEvents::ProcessMouseEvent( int button, int state, int x, int y )
{
	iMouseY = y;
	iMouseX = x;

	switch (button)
	{
		//Set point of rotation
	case GLUT_LEFT_BUTTON:
		if(GLUT_DOWN == state)
		{
			iMouseButtonDown = EMouseDownLeft;
			this->iRenderer->SetOldXRotation( this->iRenderer->GetXRotation() );
			this->iRenderer->SetOldYRotation( this->iRenderer->GetYRotation() );
		}
		else
		{
			iMouseButtonDown = EMouseUp;
		}
		break;
		//Set mesh position
	case GLUT_RIGHT_BUTTON:
		if(GLUT_UP == state)
		{
			iMouseButtonDown = EMouseUp;
		}
		else
		{
			iMouseButtonDown = EMouseDownRight;
		}
		break;
	default:
		break;
	}

}

void CMyUiEvents::ProcessMouseMotionEvent( int x, int y )
{
	if( EMouseDownLeft == iMouseButtonDown)
	{
		this->iRenderer->SetXRotation( this->iRenderer->GetOldXRotation() 
			+ (float)(y - this->iMouseY) / 4.0);
		this->iRenderer->SetYRotation( this->iRenderer->GetOldYRotation() 
			+ (float)(x - this->iMouseX) / 4.0);
	}
	else if( EMouseDownRight == iMouseButtonDown)
	{
		this->iRenderer->SetZoom(this->iRenderer->GetZoom()+(((float)y - (float)this->iMouseY) / this->iRenderer->GetScreenHeightInPixels()) * 20);
		this->iMouseY = y;
	}
}






//EXTERNAL FUNCTIONS TO USE GLUT CALLBACKS
void ProcessNormalKeysWithUi( unsigned char key, int x, int y )
{
	if (CMyUiEvents::iCurrentUi != 0 )
		CMyUiEvents::iCurrentUi->ProcessNormalKeys( key, x, y  );
}

void ProcessCursorKeysWithUi( int key, int x, int y )
{
	if (CMyUiEvents::iCurrentUi != 0 )
		CMyUiEvents::iCurrentUi->ProcessCursorKeys( key, x, y );
}
void ProcessMouseEventWithUi( int button, int state, int x, int y)
{
	if (CMyUiEvents::iCurrentUi != 0 )
		CMyUiEvents::iCurrentUi->ProcessMouseEvent( button, state, x, y );
}

void ProcessMouseMotionEventWithUi( int x, int y )
{
	if (CMyUiEvents::iCurrentUi != 0 )
		CMyUiEvents::iCurrentUi->ProcessMouseMotionEvent( x, y );
}

