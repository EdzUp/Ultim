

#include "Win32.h"
//#include "Windows.h"

void UltimWin32::PassValues( HINSTANCE hin, int CShow, std::string Title ) {
    //needed for all windows related calls and values
    Hinst = hin;
    CMDShow = CShow;
    WinTitle = Title;
}


void UltimWin32::AddMode( int ID, int Width, int Height, int Depth, int Frequency, int Orientation, int FixedOutput, int Flags ) {
    //this will add the mode to the game engine list allow for all different things to be done
    ModesClass Temp;

    Temp.ID = ID;
    Temp.Width = Width;
    Temp.Height = Height;
    Temp.Depth = Depth;
    Temp.Frequency = Frequency;
    Temp.Orientation = Orientation;
    Temp.FixedOutput = FixedOutput;
    Temp.Flags = Flags;

    Modes.push_back( Temp );            //add it to the back of the list
}

void UltimWin32::GetGraphicsModes( void ) {
    DEVMODE dm = { 0 };
    dm.dmSize = sizeof(dm);
    for( int iModeNum = 0; EnumDisplaySettings( NULL, iModeNum, &dm ) != 0; iModeNum++      )
    {
        //dmDisplayOrientation, dmDisplayFixedOutput, dmDisplayFlags
       std::cout << "Mode #" << iModeNum << " = " << dm.dmPelsWidth << "x" << dm.dmPelsHeight << " bpp:" << dm.dmBitsPerPel <<" o:" << dm.dmDisplayOrientation << " f:" << dm.dmDisplayFixedOutput << " df:" << dm.dmDisplayFlags;
       std::cout << " Freq:" << dm.dmDisplayFrequency << std::endl;
       AddMode( iModeNum, dm.dmPelsWidth, dm.dmPelsHeight, dm.dmBitsPerPel, dm.dmDisplayFrequency, dm.dmDisplayOrientation, dm.dmDisplayFixedOutput, dm.dmDisplayFlags );
    }
}

bool UltimWin32::CheckMode( int width, int height, int depth, int hertz =60 ) {
    unsigned int curcheck =0;

    for ( curcheck =0; curcheck<Modes.size(); curcheck++ ) {
        if ( Modes[ curcheck ].Width == width && Modes[ curcheck ].Height == height && Modes[ curcheck ].Depth == depth && Modes[ curcheck ].Frequency == hertz ) {
            return( true );
        }
    }

    return( false );            //FAIL
}

bool UltimWin32::SetupGraphicsWindow( int width, int height ) {
    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = Hinst;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "Ultim";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    Win32Width = width;
    Win32Height = height;

    if (!RegisterClassEx(&wcex))
        return(false );

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "Ultim",
                          WinTitle.c_str(),
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          width,
                          height,
                          NULL,
                          NULL,
                          Hinst,
                          NULL);

    ShowWindow(hwnd, CMDShow);

    RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	Win32DesktopWidth = desktop.right;
	Win32DesktopHeight = desktop.bottom;

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    return( true );
}

int UltimWin32::MouseXPosition( void ) {
    return MouseXPos;
}

int UltimWin32::MouseYPosition( void ) {
    return MouseYPos;
}

void UltimWin32::DisplayError( std::string title, std::string message ) {
	MessageBox( hwnd, message.c_str(), title.c_str(), MB_OK );
}

void UltimWin32::SetCursor( bool hidden ) {
	if ( hidden == true ){
		while(ShowCursor(false)>=0);
	} else {
		ShowCursor( true );
	}
}

void UltimWin32::MoveMouse( int x, int y ) {
    POINT pt;
    // you should set pt as a position in your Application
    ClientToScreen(hwnd, &pt);
    // now pt is a screen position
    pt.x =x;
    pt.y =y;
    SetCursorPos(pt.x, pt.y);
}

void UltimWin32::CloseGraphicsWindow( void ) {
    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);
}

void UltimWin32::FlipBuffer( void ) {
    SwapBuffers(hDC);
}

bool UltimWin32::CheckOSMessages( void ) {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        /* handle or dispatch messages */
        if (msg.message == WM_QUIT) {
            return( true );
        } else {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return( false );
}

bool UltimWin32::InFullscreen( void ) {
	return( FullScreenView );
}

int UltimWin32::WindowWidth( void ){
	return( Win32Width );
}

int UltimWin32::WindowHeight( void ) {
	return( Win32Height );
}

int UltimWin32::CurrentWidth( void ) {
    if ( FullScreenView == true ) {
           return( Win32DesktopWidth );
    } else {
        return( Win32Width );
    }
    return( 0 );
}

int UltimWin32::CurrentHeight( void ) {
    if ( FullScreenView == true ) {
           return( Win32DesktopHeight );
    } else {
        return( Win32Height );
    }
    return( 0 );
}

int UltimWin32::DesktopWidth( void ){
	return( Win32DesktopWidth );
}

int UltimWin32::DesktopHeight( void ) {
    return( Win32DesktopHeight );
}

int UltimWin32::Shutdown( void ) {
    DisableOpenGL( hwnd, hDC, hRC);
		// destroy the window explicitly
	DestroyWindow(hwnd);
	return( msg.wParam );
}

//And to reset the resolution:
void UltimWin32::LeaveFullscreen ( void ){
	// this resets the screen to the registry-stored values
  	ChangeDisplaySettings ( NULL, 0 );
}

void UltimWin32::Fullscreen( void ) {
	//this will allow jumping to fullscreen
	float Aspect;

	FullScreenView = true;
	SetWindowLongPtr(hwnd, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);
	MoveWindow(hwnd, 0, 0, Win32DesktopWidth, Win32DesktopHeight, TRUE);
	Aspect = (float)Win32DesktopWidth / (float)Win32DesktopHeight;
	glViewport( 0, 0, Win32DesktopWidth, Win32DesktopHeight );
}

int UltimWin32::ChangeResolution( int width, int height, int depth ) {
	int success;

	if ( FullScreenView == true ){
		DEVMODE dm;
		dm.dmSize = sizeof(DEVMODE);
		dm.dmPelsWidth = width;
		dm.dmPelsHeight = height;
		dm.dmBitsPerPel = depth;
		dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
		success = ChangeDisplaySettings(&dm, 0) == DISP_CHANGE_SUCCESSFUL;
	} else {
		success = ChangeDisplaySettings(0, 0) == DISP_CHANGE_SUCCESSFUL;
	}

	return( success );
}

void UltimWin32::Windowed( int width, int height, bool resizeable ) {
	//take back from fullscreen and set window size
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = width;
	rect.bottom = height;

	if ( resizeable == true ) {
		SetWindowLongPtr(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
		MoveWindow(hwnd, 0, 0, rect.right-rect.left, rect.bottom-rect.top, TRUE);
	} else {
		SetWindowLongPtr(hwnd, GWL_STYLE, WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE);
		AdjustWindowRect(&rect, WS_CAPTION | WS_POPUPWINDOW, FALSE);
		MoveWindow(hwnd, 0, 0, rect.right-rect.left, rect.bottom-rect.top, TRUE);
	}
	FullScreenView = false;
	glViewport( 0, 0, Win32Width, Win32Height );
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}


/*
class FrameworkWin32 {
	void KeyDown( WPARAM wParam ) {
//	    printf( "%i is pressed", wParam );
        keybank[ wParam ] = true;
	}

	void KeyUp( WPARAM wParam ) {
	    keybank[ wParam ] = false;
	}
};

FrameworkWin32 Entry;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_RBUTTONDOWN:
			MouseRightDown = true;
		break;
		case WM_RBUTTONUP:
			MouseRightDown = false;
		break;
		case WM_MBUTTONDOWN:
			MouseMiddleDown = true;
		break;
		case WM_MBUTTONUP:
			MouseMiddleDown = false;
		break;
		case WM_LBUTTONDOWN:
			MouseLeftDown = true;
		break;
		case WM_LBUTTONUP:
			MouseLeftDown = false;
		break;
		case WM_MOUSEMOVE:
			// get the x and y coordinates
			MouseXPos = LOWORD(lParam);
			MouseYPos = HIWORD(lParam);
		break;
		case WM_CLOSE:
			PostQuitMessage(0);
		break;

		case WM_DESTROY:
			return 0;

        case WM_KEYUP:
            Entry.KeyUp( wParam );
            break;

        case WM_KEYDOWN:
//		case WM_KEYDOWN:
		{
		    Entry.KeyDown( wParam );    //make sure key is setup
		    if ( Input.KeyDown( KEY_ESCAPE ) == true ) {
                PostQuitMessage( 0 );
		    }
		}
		break;

		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

*/


/*
Change display settings:
LONG ChangeDisplaySettings(
  _In_ DEVMODE *lpDevMode,
  _In_ DWORD   dwflags
);

ChangeDisplaySettings function

The ChangeDisplaySettings function changes the settings of the default display device to the specified graphics mode.
To change the settings of a specified display device, use the ChangeDisplaySettingsEx function.
Note  Apps that you design to target Windows 8 and later can no longer query or set display modes that are less than 32 bits per pixel (bpp); these operations will fail. These apps have a compatibility manifest that targets Windows 8. Windows 8 still supports 8-bit and 16-bit color modes for desktop apps that were built without a Windows 8 manifest; Windows 8 emulates these modes but still runs in 32-bit color mode.

Syntax
C++

LONG ChangeDisplaySettings(
  _In_ DEVMODE *lpDevMode,
  _In_ DWORD   dwflags
);

Parameters
lpDevMode [in]
A pointer to a DEVMODE structure that describes the new graphics mode. If lpDevMode is NULL, all the values currently in the registry will be used for the display setting. Passing NULL for the lpDevMode parameter and 0 for the dwFlags parameter is the easiest way to return to the default mode after a dynamic mode change.
The dmSize member of DEVMODE must be initialized to the size, in bytes, of the DEVMODE structure. The dmDriverExtra member of DEVMODE must be initialized to indicate the number of bytes of private driver data following the DEVMODE structure. In addition, you can use any or all of the following members of the DEVMODE structure.
Member	Meaning
dmBitsPerPel	Bits per pixel
dmPelsWidth	Pixel width
dmPelsHeight	Pixel height
dmDisplayFlags	Mode flags
dmDisplayFrequency	Mode frequency
dmPosition	Position of the device in a multi-monitor configuration.

In addition to using one or more of the preceding DEVMODE members, you must also set one or more of the following values in the dmFields member to change the display setting.
Value	Meaning
DM_BITSPERPEL	Use the dmBitsPerPel value.
DM_PELSWIDTH	Use the dmPelsWidth value.
DM_PELSHEIGHT	Use the dmPelsHeight value.
DM_DISPLAYFLAGS	Use the dmDisplayFlags value.
DM_DISPLAYFREQUENCY	Use the dmDisplayFrequency value.
DM_POSITION	Use the dmPosition value.

dwflags [in]
Indicates how the graphics mode should be changed. This parameter can be one of the following values.
Value	Meaning
0
The graphics mode for the current screen will be changed dynamically.
CDS_FULLSCREEN
The mode is temporary in nature.
If you change to and from another desktop, this mode will not be reset.
CDS_GLOBAL
The settings will be saved in the global settings area so that they will affect all users on the machine. Otherwise, only the settings for the user are modified. This flag is only valid when specified with the CDS_UPDATEREGISTRY flag.
CDS_NORESET
The settings will be saved in the registry, but will not take effect. This flag is only valid when specified with the CDS_UPDATEREGISTRY flag.
CDS_RESET
The settings should be changed, even if the requested settings are the same as the current settings.
CDS_SET_PRIMARY
This device will become the primary device.
CDS_TEST
The system tests if the requested graphics mode could be set.
CDS_UPDATEREGISTRY
The graphics mode for the current screen will be changed dynamically and the graphics mode will be updated in the registry. The mode information is stored in the USER profile.

Specifying CDS_TEST allows an application to determine which graphics modes are actually valid, without causing the system to change to that graphics mode.
If CDS_UPDATEREGISTRY is specified and it is possible to change the graphics mode dynamically, the information is stored in the registry and DISP_CHANGE_SUCCESSFUL is returned. If it is not possible to change the graphics mode dynamically, the information is stored in the registry and DISP_CHANGE_RESTART is returned.
If CDS_UPDATEREGISTRY is specified and the information could not be stored in the registry, the graphics mode is not changed and DISP_CHANGE_NOTUPDATED is returned.
Return value
The ChangeDisplaySettings function returns one of the following values.
Return code	Description
DISP_CHANGE_SUCCESSFUL
The settings change was successful.
DISP_CHANGE_BADDUALVIEW
The settings change was unsuccessful because the system is DualView capable.
DISP_CHANGE_BADFLAGS
An invalid set of flags was passed in.
DISP_CHANGE_BADMODE
The graphics mode is not supported.
DISP_CHANGE_BADPARAM
An invalid parameter was passed in. This can include an invalid flag or combination of flags.
DISP_CHANGE_FAILED
The display driver failed the specified graphics mode.
DISP_CHANGE_NOTUPDATED
Unable to write settings to the registry.
DISP_CHANGE_RESTART
The computer must be restarted for the graphics mode to work.
*/

