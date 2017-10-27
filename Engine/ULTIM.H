/*
    Ultim.h - Copyright (C)EdzUp
    Programmed by Ed 'EdzUp' Upton
*/

/*
Required to run Ultim:
Project>>Build Options >> Linker Settings >> Link Libraries:
opengl32, glu32, gdi32, OpenAL32.lib, enet.lib, libws2_32.a, Winmm

Other Linker options:
-lwinmm
-lws2_32
*/

#ifndef ULTIMENGINE
    #define ULTIMENGINE

    #define Pi 3.14159265               //for the new perspective system

    #include <string>
    #include <GL/gl.h>                  //opengl calls
    #include "Win32.h"                  //Ultim win32 class that contains all windows related functions
    #include "UTexture.h"               //ETexture hook for loading png textures
    #include "UAudio.h"                 //EAudio hook for loading wav files
    #include "SoundSystem.h"            //OpenAL initialisers
    #include "URandom.h"                //Random number generator originally by Shagwana
    #include "UNetwork.h"               //Winsock networking library using ENet
    #include "URender.h"                //Ultim Render class

    class UltimClass {
    private:
        std::string         AppTitle;
        float               PixelScaleX;
        float               PixelScaleY;

        int                 virtual_width = 640;
        int                 virtual_height = 480;

        double              xmin, xmax, ymin, ymax;
        double              CameraNear, CameraFar;

        bool                Camera3d = false;

        float               AspectRatio;

        void SetupAspectRatio( double fovy, double aspect );

    public:
        UltimWin32          Win32;
        SoundSystemClass    SoundSystem;
        URandomClass        Random;
        URenderClass        Render;

        float               VideoWidth;
        float               VideoHeight;

        bool                ResetAspectRatio = false;

        void Initialise( void );
        char* GetAppTitle( void );
        void SetAppTitle( std::string NewTitle );

        //OS Specific
        bool Graphics( int width, int height, int depth );
        bool Graphics3D( int width, int height, int depth );
        void ClsColor( int red, int green, int blue, int alpha );
        void Cls( void );
        void Flip( void );
        void EndGraphics( void );
        bool CheckHostMessages( void );
        float GraphicsWidth( void );
        float GraphicsHeight( void );
        void SetVirtualResolution( int width, int height );
        void SetVideoAspect( float width, float height, bool threeD );
        void SetCameraRange( float camnear, float camfar );

        float GetAspectRatio( void );
        void ResetAspect( float NewWidth, float NewHeight );
    };
#endif // ULTIMENGINE
