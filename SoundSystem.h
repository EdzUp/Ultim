/*
    SoundSystem.h - Copyright (C)EdzUp
    Programmed by Ed 'EdzUp' Upton
*/

/*
    THIS IS THE ULTIM SOUND SYSTEM FOR SETTING UP OPENAL FOR AUDIO
    IT MUST BE INITIALISED BEFORE ANYTHING CAN BE USED.

    ALL FUNCTION HERE ARE FOR GLOBAL ADJUSTMENT AND SYSTEM USE FOR EACH
    INDIVIDUAL SOUND GO TO THE UAUDIO CLASS
*/

#ifndef USOUNDSYSTEM
    #define USOUNDSYSTEM
    #include <AL/al.h>
    #include <AL/alc.h>

    class SoundSystemClass {
        float           CurrentVolume;
        int             SampleRate = 44100;
        int             SampleSize = 1024;
        ALbyte          buffer[22050];
        ALint           recordSample;
        ALCdevice       *recordingDevice;
        ALCdevice* device;
        ALCcontext* context;

    public:
        void SetVolume( float level );
        float GetVolume( void );
        bool StartRecording( void );
        void UpdateRecording( void );
        void StopRecording( void );
        ALbyte* GetRecording( void );
        bool Initialise( void );
        void Shutdown( void );
    };
#endif // USOUNDSYSTEM
