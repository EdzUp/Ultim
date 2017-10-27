//
//  Debugger.cpp - Copyright (C)EdzUp
//  Programmed by Ed 'EdzUp' Upton
//

#include "Debugger.h"       //link to debugger header file

bool DebugClass::StartDebugging( std::string filename ){
    DebuggerHandle = fopen( filename.c_str(), "wt" );

    if ( DebuggerHandle != NULL ) {
        fclose( DebuggerHandle );
        DebugStarted = true;
        DebugFilename = filename;
        return( true );
    }

    return( false );
}

bool DebugClass::SetDebugFile( std::string filename ) {
    FILE* tester;

    tester = fopen( filename.c_str(), "rt" );

    if ( tester != NULL ) {
        //make sure file exists before setting the details
        DebugStarted = true;
        DebugFilename = filename;
        fclose( tester );

        return( true );
    }

    DebugStarted = false;
    DebugFilename = "";

    return( false );
}

void DebugClass::DebugLog( std::string text ) {
    if ( DebugStarted == true ) {
        DebuggerHandle = fopen( DebugFilename.c_str(), "at" );
        fwrite( text.c_str(), 1, text.length(), DebuggerHandle );
        fclose( DebuggerHandle );
    }
}

void DebugClass::EOL( void ) {
    if ( DebugStarted == true ) {
        DebuggerHandle = fopen( DebugFilename.c_str(), "at" );
        fputc( 10, DebuggerHandle );
        fputc( 13, DebuggerHandle );
        fclose( DebuggerHandle );
    }
}

void DebugClass::EndDebugging( void ) {
    DebugStarted = false;
}
