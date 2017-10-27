/*
    SoundSystem.cpp - Copyright (C)EdzUp
    Programmed by Ed 'EdzUp' Upton
*/

#include "SoundSystem.h"
#include "stdlib.h"
#include <AL/al.h>
#include <AL/alc.h>

void SoundSystemClass::SetVolume( float level ) {
    alListenerf( AL_GAIN, level );
    CurrentVolume = level;
}

float SoundSystemClass::GetVolume( void ) {
    //used to get the current volume level
    return( CurrentVolume );
}

bool SoundSystemClass::StartRecording( void ) {
    alGetError();
    recordingDevice = alcCaptureOpenDevice( NULL, SampleRate, AL_FORMAT_STEREO16, SampleSize );
    if (alGetError() != AL_NO_ERROR) {
        return( false );
    }
    alcCaptureStart( recordingDevice );

    return( true );
}

void SoundSystemClass::UpdateRecording( void ){
    alcGetIntegerv( recordingDevice, ALC_CAPTURE_SAMPLES, ( ALCsizei )sizeof( ALint ), &recordSample );
    alcCaptureSamples( recordingDevice, (ALCvoid *)buffer, recordSample );
}

void SoundSystemClass::StopRecording( void ) {
    alcCaptureStop( recordingDevice );
    alcCaptureCloseDevice( recordingDevice );
}

ALbyte* SoundSystemClass::GetRecording( void ) {
    return( buffer );
}

bool SoundSystemClass::Initialise( void ) {
    //this will start up the audio system for us
    ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };
    ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };
    ALfloat ListenerOri[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };

    device = alcOpenDevice( NULL );
    if (!device ) exit( 4 );
    context = alcCreateContext( device, NULL );
    if (!context ) exit( 5 );
    alcMakeContextCurrent( context );

    alListenerfv( AL_POSITION, ListenerPos );
    alListenerfv( AL_VELOCITY, ListenerVel );
    alListenerfv( AL_ORIENTATION, ListenerOri );
    CurrentVolume = 1.0;

    return( true );
}

void SoundSystemClass::Shutdown( void ) {
    //this will shutdown OpenAL
    alcMakeContextCurrent( NULL );
    alcDestroyContext( context );
    alcCloseDevice( device );
}
