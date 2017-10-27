/*
    Win32.h - Copyright (C)EdzUp
    Programmed by Ed 'EdzUp' Upton
*/

#ifndef WINDOWSGO
    #define WINDOWSGO

    #include <Windows.h>
    #include <wtypes.h>
    #include <iostream>
    #include <vector>
    #include <GL/gl.h>

    LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
    void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
    void DisableOpenGL(HWND, HDC, HGLRC);

    class ModesClass {
    public:
        int ID;

        int Width;
        int Height;
        int Depth;

        int Frequency;
        int Orientation;
        int FixedOutput;

        int Flags;
    };

    class UltimWin32{
        public:
        std::vector<ModesClass> Modes;
        HINSTANCE               Hinst;
        int                     CMDShow;
        std::string             WinTitle;
        WNDCLASSEX wcex;
        HWND hwnd;
        HDC hDC;
        HGLRC hRC;
        MSG msg;

        int MouseXPos;
        int MouseYPos;
        bool		FullScreenView;
        int			Win32Width;
        int			Win32Height;
        int			Win32DesktopWidth;
        int			Win32DesktopHeight;

        void PassValues( HINSTANCE hin, int CShow, std::string Title );

        //graphics modes stuff
        bool SetupGraphicsWindow( int width, int height );
        void CloseGraphicsWindow( void );
        void AddMode( int ID, int Width, int Height, int Depth, int Frequency, int Orientation, int FixedOutput, int Flags );
        void GetGraphicsModes( void );
        bool CheckMode( int width, int height, int depth, int hertz );
        bool CheckOSMessages( void );
        int MouseXPosition( void );
        int MouseYPosition( void );
        void DisplayError( std::string title, std::string message );
        void SetCursor( bool hidden );
        void MoveMouse( int x, int y );
        bool InFullscreen( void );
        int WindowWidth( void );
        int WindowHeight( void );
        int CurrentWidth( void );
        int CurrentHeight( void );
        int DesktopWidth( void );
        int DesktopHeight( void );
        int Shutdown( void );
        void Fullscreen( void );
        void LeaveFullscreen( void );
        int ChangeResolution( int width, int height, int depth );
        void Windowed( int width, int height, bool resizeable = false );

        void FlipBuffer( void );
    };
#endif // WINDOWSGO
