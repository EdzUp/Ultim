#include <windows.h>
#include <gl/gl.h>

#include "../Engine/Ultim.h"            //main entry for Ultim engine
#include "../Engine/Debugger.h"

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    UltimClass      Ultim;                  //hook into engine
    DebugClass      Debug;                  //Debugging system for log file
    UTexture        ImageTest;              //Texture system ETexture
    UAudio          SoundTest;              //Audio system
    UNetwork        NetworkTest;            //Network system handle
    UEntityClass    EntityTest;             //Entity system handle
    UAudio          RecordTest;             //this will test the recording device

    UAudio          Thunder[ 3 ];

    //this bit works as expected :D
    Debug.StartDebugging( "DebugLog.txt" );
    Debug.DebugLog( "This is a test" );
    Debug.EOL();
    Debug.DebugLog( "So is this " );
    Debug.DebugLog( ":)" );
    Debug.EOL();


    Ultim.Initialise();
    Ultim.Random.Initialise();
    Ultim.SetAppTitle( "Ultim Test 1" );
    Ultim.Win32.PassValues( hInstance, nCmdShow, Ultim.GetAppTitle() );           //pass values for Win32 calls
    Debug.DebugLog( "::Initialising graphics window...." );
    Ultim.SetVirtualResolution( 640, 480 );
    Ultim.SetCameraRange( 0.1f, 1000.0f );
    if ( Ultim.Graphics3D( 1366, 768, 32 ) == false ) exit( 1 );

    Ultim.Win32.ChangeResolution( 1366, 768, 32);

    Debug.DebugLog( "Complete" );
    Debug.EOL();

    EntityTest = Ultim.Render.CreateEntity();           //create a entity
    EntityTest.Name = "Woohoo";                         //change its name to "Woohoo"
    Debug.DebugLog( EntityTest.Name );                  //debug its new name
    Ultim.Render.Update( EntityTest );                  //commit the update to the render list
    Ultim.Render.ShowEm();

    //now do a network test
    if ( NetworkTest.Initialise() == false )
        exit( 1 );

    NetworkTest.StartServer( 10248, 32 );

    ImageTest.LoadPNG( "EdzUpLogo.png" );

    Ultim.SoundSystem.Initialise();
    SoundTest.LoadWAV( "rain.wav" );
    SoundTest.SetVolume( 0.5f );
    SoundTest.SetPitch( 1.0f );
    SoundTest.Loop( true );
    SoundTest.Play();

    Thunder[ 0 ].LoadWAV( "Thunder1.wav" );
    Thunder[ 1 ].LoadWAV( "Thunder2.wav" );
    Thunder[ 2 ].LoadWAV( "Thunder3.wav" );

    BOOL bQuit = FALSE;
    float theta = 0.0f;

    Ultim.ClsColor( 0, 0, 0, 0 );

    printf( "Aspect ratio is : %f\n", Ultim.GetAspectRatio() );

    if ( Ultim.SoundSystem.StartRecording() == true ) {
        Sleep( 1024 );
        RecordTest.CreateSample( (unsigned char* )Ultim.SoundSystem.GetRecording(), 44100 );
        Ultim.SoundSystem.StopRecording();
        RecordTest.Play();
    } else {
        printf( "AUDIO::NO RECORDING DEVICE FOUND\n\n" );
    }

    /* program main loop */
    while (!bQuit)
    {
        if ( Ultim.Random.Generate( 300 ) <1 ) {
            switch( Ultim.Random.Generate( 3 ) ) {
            case 1:
                Thunder[ 1 ].Play();
                break;
            case 2:
                Thunder[ 2 ].Play();
                break;
            default:
                Thunder[ 0 ].Play();
                break;
            }
        }


        /* check for messages */
        if ( Ultim.CheckHostMessages() == true ) {
            bQuit = TRUE;
        } else {
            /* OpenGL animation code goes here */
            Ultim.Cls();

            glLoadIdentity();                       //reset all the matrix's
            glPushMatrix();
            glRotatef(theta, 0.0f, 0.0f, 1.0f);
            glEnable( GL_TEXTURE_2D );
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBindTexture( GL_TEXTURE_2D, ImageTest.Texture );

            glBegin(GL_TRIANGLES);
                glTexCoord2f( 0.0f, 0.0f ); glColor3f(1.0f, 1.0f, 1.0f);   glVertex3f(100.0f,   100.0f,     -209.0f);
                glTexCoord2f( 1.0f, 0.0f ); glColor3f(1.0f, 1.0f, 1.0f);   glVertex3f(100.0f,  -100.0f,    -209.0f);
                glTexCoord2f( 1.0f, 1.0f ); glColor3f(1.0f, 1.0f, 1.0f);   glVertex3f(-100.0f, -100.0f,    -209.0f);

                glTexCoord2f( 1.0f, 1.0f ); glColor3f(1.0f, 1.0f, 1.0f);   glVertex3f(-100.0f,   -100.0f,     -209.0f);
                glTexCoord2f( 0.0f, 1.0f ); glColor3f(1.0f, 1.0f, 1.0f);   glVertex3f(-100.0f,  100.0f,    -209.0f);
                glTexCoord2f( 0.0f, 0.0f ); glColor3f(1.0f, 1.0f, 1.0f);   glVertex3f(100.0f, 100.0f,    -209.0f);
            glEnd();

            glPopMatrix();
            glDisable( GL_TEXTURE_2D );

            Ultim.Render.Render();              //render everything to the screen
            Ultim.Flip();                       //flip to front buffer

            theta += 1.0f;
            Sleep (1);
        }
    }

    Ultim.EndGraphics();
    Ultim.SoundSystem.Shutdown();

    return Ultim.Win32.msg.wParam;
}


