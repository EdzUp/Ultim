/*
    EAudio.h - Copyright (C)EdzUp
    Programmed by Ed 'EdzUp' Upton
*/

#ifndef EAUDIOSYSTEM
    #define EAUDIOSYSTEM

    #include <AL/al.h>
    #include <AL/alc.h>
    #include <iostream>
    #include <string>
    #include <stdio.h>
    #include <sstream>
    #include <vector>

    class UAudio {
        //this is the framework audio class for all the framework audio system
        ALuint          source;
        ALuint          soundbuffer;
        ALuint          frequency;

        ALenum          format;

        float           CurrentVolume = 1.0f;
        float           CurrentPitch = 1.0f;

    public:
        //audio loader
        void LoadWAV( std::string filename );
        void Free( void );
        void Loop( bool looped );
        void Pause( void );
        void Resume( void );
        void Play( void );
        void Stop( void );
        void SetVolume( float value );
        float GetVolume( void );
        void SetPitch( float value );
        float GetPitch( void );
        ALuint GetSource( void );
        void CreateSample( unsigned char* buffer, long freq );
    };
#endif // EAUDIOSYSTEM
