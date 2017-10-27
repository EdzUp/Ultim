/*
    Ultim.cpp - Copyright (C)EdzUp
    Programmed by Ed 'EdzUp' Upton
*/

#include "Ultim.h"
#include <GL/gl.h>          //opengl calls
#include <math.h>
#include <vector>
#include <stdio.h>

void UltimClass::SetVirtualResolution( int width, int height ) {
    //this sets the new resolution depending on users requirements.
    virtual_width = width;
    virtual_height = height;
}

void UltimClass::SetVideoAspect( float sVAwidth, float sVAheight, bool threeD ) {
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    //real window size
    int screen_width = sVAwidth;
    int screen_height = sVAheight;

    //figure out the largest area that fits in the resolution
    int width = screen_width;
    int height = (int)( width *AspectRatio +0.5f );

    if ( height >screen_height ){
        //it doesnt fit our height we must switch to pillarbox
        height = screen_height;
        width = (int)(height *AspectRatio +0.5f );
    }
//    int vp_x = (screen_width /2 ) - (width /2 );
//    int vp_y = ( screen_height /2 ) - (height /2 );

    //now we use glOrtho
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();

    if ( threeD == false ) {
        //this function is for mac only and window, if you are using iOS you should use glOrthof instead
        //left right bottom top near far
        glOrtho( 0.0f -AspectRatio, AspectRatio, 1.0f, -1.0f, -1, 1 );
        /* if on iOS */ //glOrthof( 0, screen_width, screen_height, 0, -1, 1 );
    } else {
        glFrustum( 0.0f -AspectRatio, AspectRatio, 1.0f, -1.0f, CameraNear, CameraFar );
    }
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();

    //now we need to push the transformations
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();

    //now to calculate the scale considering the screen size and virtual size
    float scale_x = (float)((float)(screen_width ) /(float)virtual_width);
    float scale_y = (float)((float)(screen_height) /(float)virtual_height );
    glScalef( scale_x, scale_y, 1.0f );

    //ALL DRAWING NOW SHOULD BE SORTED AND WORK AS EXPECTED
}

void UltimClass::ResetAspect( float NewWidth, float NewHeight ) {
    //virtual screen settings
    AspectRatio = (float)NewWidth /(float)NewHeight;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport( -AspectRatio, AspectRatio, NewHeight, -NewHeight );
    SetupAspectRatio( 60.0f, (float)NewWidth /(float)NewHeight);
    glTranslatef( 0.0f, 0.0f, -1.0f );

    glMatrixMode( GL_MODELVIEW );
    VideoWidth = NewWidth;
    VideoHeight = NewHeight;
}

void UltimClass::SetupAspectRatio( double fovy, double aspect ) {
    if ( VideoWidth == 0.0f ) return;
    if ( VideoHeight == 0.0f ) return;

    PixelScaleX = 1.0f;
    PixelScaleY = 1.0f;

    ymax = CameraNear *tan( fovy *Pi /360.0 );
    ymin = -ymax;
    xmin = ymin *aspect;
    xmax = ymax *aspect;
    glFrustum( xmin, xmax, ymin, ymax, CameraNear, CameraFar );
}

bool UltimClass::CheckHostMessages( void ) {
    #ifdef _WIN32
        return( Win32.CheckOSMessages() );          //Windows 32 OS messages
    #endif // _WIN32

    return( false );
}

void UltimClass::Initialise( void ) {
    //main initialiser
    AppTitle = "Untitled";
    CameraNear = 0.1f;
    CameraFar = 100.0f;

    Win32.GetGraphicsModes();                //get all graphics modes for game engine
}

char* UltimClass::GetAppTitle( void ) {
    return( (char*) AppTitle.c_str() );
}

void UltimClass::SetAppTitle( std::string NewTitle ) {
    AppTitle = NewTitle;
}

bool UltimClass::Graphics( int width, int height, int depth ) {
    if ( Win32.CheckMode( width, height, depth, 60 ) == false ) return( false );

    std::cout << "MODE IS A GO" << std::endl;
    #ifdef _WIN32
        //call windows for the setup :)
        Win32.SetupGraphicsWindow( width, height );
    #endif

    VideoWidth = width;
    VideoHeight = height;

    //virtual screen settings
    AspectRatio = width /height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport( -AspectRatio, AspectRatio, height, -height );
    SetupAspectRatio( 60.0f, (float)width /(float)height);
    glTranslatef( 0.0f, 0.0f, -1.0f );

    glMatrixMode( GL_MODELVIEW );

//    SetVideoAspect( (int)width, (int)height, false );
    Camera3d = false;               //camera can only rotate on Z

    return( true );
}

bool UltimClass::Graphics3D( int width, int height, int depth ) {
    if ( Win32.CheckMode( width, height, depth, 60 ) == false ) return( false );

    std::cout << "3D MODE IS A GO" << std::endl;
    #ifdef _WIN32
        //call windows for the setup :)
        Win32.SetupGraphicsWindow( width, height );
    #endif

    VideoWidth = width;
    VideoHeight = height;
    //virtual screen settings
    AspectRatio = (float)width /(float)height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport( -AspectRatio, AspectRatio, height, -height );
    SetupAspectRatio( 60.0f, (float)width /(float)height);
    glTranslatef( 0.0f, 0.0f, -1.0f );

    glMatrixMode( GL_MODELVIEW );

//    SetVideoAspect( (int)width, (int)height, true );

    Camera3d = true;

    return( true );
}

void UltimClass::ClsColor( int red, int green, int blue, int alpha ) {
    glClearColor( 0.00392f *red, 0.00392f *green, 0.00392f *blue, 0.00392f *alpha );
}

void UltimClass::Cls( void ) {
    //standard CLS routine for compatibility with B3d
    glClear(GL_COLOR_BUFFER_BIT);
}

void UltimClass::Flip( void ) {
    Win32.FlipBuffer();
}

void UltimClass::EndGraphics( void ) {
    Win32.CloseGraphicsWindow();
}

float UltimClass::GraphicsWidth( void ) {
    return( VideoWidth );
}

float UltimClass::GraphicsHeight( void ) {
    return( VideoHeight );
}

void UltimClass::SetCameraRange( float camnear, float camfar ) {
    CameraNear = camnear;
    CameraFar = camfar;
}

float UltimClass::GetAspectRatio( void ) {
    return( AspectRatio );
}
/*
Check if 4 * height == 3 * width or 16 * height == 9 * width.

Remember the definition of a rational number: It is an equivalence class of pairs of
integers (m, n) sub­ject to the equivalence (m, n) ≡ (m', n') if and only if n' m = n m'

4:3 CRT
5:4 LCD
16:9 Wide CRT and LCD
16:10 Wide CRT and LCD

shortSideMax = 640;
longSideMax = 960;
function Resize(image)
{
    if (image.width >= image.height)
    {
        if (image.width <= longSideMax && image.height <= shortSideMax)
            return image;  // no resizing required
        wRatio = longSideMax / image.width;
        hRatio = shortSideMax / image.height;
    }
    else
    {
        if (image.height <= longSideMax && image.width <= shortSideMax)
            return image; // no resizing required
        wRatio = shortSideMax / image.width;
        hRatio = longSideMax / image.height;
    }

    // hRatio and wRatio now have the scaling factors for height and width.
    // You want the smallest of the two to ensure that the resulting image
    // fits in the desired frame and maintains the aspect ratio.
    resizeRatio = Min(wRatio, hRatio);

    newHeight = image.Height * resizeRatio;
    newWidth = image.Width * resizeRatio;

    // Now call function to resize original image to [newWidth, newHeight]
    // and return the result.
}
*/
