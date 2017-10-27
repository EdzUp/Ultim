/*
    EAudio.cpp - Copyright (C)EdzUp
    Programmed by Ed 'EdzUp' Upton
*/

#include "UAudio.h"
#include "stdlib.h"
#include <AL/al.h>
#include <AL/alc.h>

//audio sample functions
void UAudio::LoadWAV( std::string filename ) {
    FILE *fp = NULL;
    char type[ 4 ];
    long size, chunkSize;
    short formatType, channels;
    long sampleRate, avgBytesPerSec;
    short bytesPerSample, bitsPerSample;
    long dataSize;

    fp = fopen( filename.c_str(), "rb" );
    if (!fp) {
        printf( "::Error loading sound file\n\n" );
        exit( 1 );
    }

    //check to make sure wav is ok
    fread( type, sizeof( char ), 4, fp );

    if ( type[ 0 ] != 'R' || type[ 1 ] != 'I' || type[ 2 ] != 'F' || type[ 3 ] != 'F' ) exit( 64 );

    fread( &size, sizeof( long ), 1, fp );
    fread( type, sizeof( char ), 4, fp );
    if ( type[ 0 ] != 'W' || type[ 1 ] != 'A' || type[ 2 ] != 'V' || type[ 3 ] != 'E' ) exit( 65 );

    fread( type, sizeof( char ), 4, fp );
    if ( type[ 0 ] != 'f' || type[ 1 ] != 'm' || type[ 2 ] != 't' || type[ 3 ] != ' ' ) exit( 66 );

    //now we know the file is a wav file we can load it
    fread( &chunkSize, sizeof( long ), 1, fp );
    fread( &formatType, sizeof( short ), 1, fp );
    fread( &channels, sizeof( short ), 1, fp );
    fread( &sampleRate, sizeof( long ), 1, fp );
    fread( &avgBytesPerSec, sizeof( long ), 1, fp );
    fread( &bytesPerSample, sizeof( short ), 1, fp );
    fread( &bitsPerSample, sizeof( short ), 1, fp );

    fread( type, sizeof( char ), 4, fp );
    if ( type[ 0 ] != 'd' || type[ 1 ] != 'a' || type[ 2 ] != 't' || type[ 3 ] != 'a' ) exit( 67 );

    fread( &dataSize, sizeof( long ), 1, fp );

    //we could display the info if we wanted
    unsigned char* buf= new unsigned char[ dataSize ];
    fread( buf, sizeof( char ), dataSize, fp );

    frequency=sampleRate;
    format =0;

    alGenBuffers( 1, &soundbuffer );
    alGenSources( 1, &source );
    if( alGetError() != AL_NO_ERROR ) exit( 2 );

    if( bitsPerSample == 8 ){
        if ( channels == 1 )
            format = AL_FORMAT_MONO8;
        else if ( channels == 2)
            format = AL_FORMAT_STEREO8;
    }else if ( bitsPerSample == 16 ){
        if ( channels == 1 )
            format = AL_FORMAT_MONO16;
        else if ( channels == 2)
            format = AL_FORMAT_STEREO16;
    }
    if (!format) exit( 2 );

    alBufferData( soundbuffer, format, buf, dataSize, frequency );
    if( alGetError() != AL_NO_ERROR ) exit( 3 );

    ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };
//        ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };
    alSourcei( source, AL_BUFFER, soundbuffer );
    alSourcef( source, AL_PITCH, 1.0f );
    alSourcef( source, AL_GAIN, 1.0f );
    alSourcefv( source, AL_POSITION, SourcePos );
    alSourcei( source, AL_LOOPING, AL_FALSE );

    if ( alGetError() != AL_NO_ERROR ) exit( 4 );

    fclose( fp );
    delete[] buf;
}

void UAudio::Free( void ){
    //this will release all sounds and get rid of it
    alDeleteSources( 1, &source );
    alDeleteBuffers( 1, &soundbuffer );
}

void UAudio::Loop( bool looped ){
    if ( looped == true ){
        alSourcei( source, AL_LOOPING, AL_TRUE );
    } else {
        alSourcei( source, AL_LOOPING, AL_FALSE );
    }
}

void UAudio::Pause( void ) {
    alSourcePause( source );
}

void UAudio::Resume( void ) {
    alSourcePlay( source );
}

void UAudio::Play( void ) {
    alSourcePlay( source );
}

void UAudio::Stop( void ) {
    alSourceStop( source );
}

void UAudio::SetVolume( float value ) {
    alSourcef (source, AL_GAIN, value );
    CurrentVolume = value;
}

float UAudio::GetVolume( void ) {
    return( CurrentVolume );
}

void UAudio::SetPitch( float value ) {
    alSourcef( source, AL_PITCH, value );
    CurrentPitch = value;
}

float UAudio::GetPitch( void ) {
    return( CurrentPitch );
}

ALuint UAudio::GetSource( void ) {
    return( source );
}

void UAudio::CreateSample( unsigned char* buffer, long freq ) {

    frequency=freq;
    format =0;

    alGenBuffers( 1, &soundbuffer );
    alGenSources( 1, &source );
    if( alGetError() != AL_NO_ERROR ) exit( 2 );

    format = AL_FORMAT_STEREO16;
    if (!format) exit( 2 );

    alBufferData( soundbuffer, format, buffer, sizeof( buffer ), frequency );
    if( alGetError() != AL_NO_ERROR ) exit( 3 );

    ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };
//        ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };
    alSourcei( source, AL_BUFFER, soundbuffer );
    alSourcef( source, AL_PITCH, 1.0f );
    alSourcef( source, AL_GAIN, 1.0f );
    alSourcefv( source, AL_POSITION, SourcePos );
    alSourcei( source, AL_LOOPING, AL_FALSE );

    if ( alGetError() != AL_NO_ERROR ) exit( 4 );

}
