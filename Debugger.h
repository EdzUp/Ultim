/*
    Debugger.h - Copyright (C)EdzUp
    Programmed by Ed 'EdzUp' Upton
*/

#ifndef DEBUGGERINC
    #define DEBUGGERINC
    #include <stdio.h>
    #include <string>

    class DebugClass {
        FILE*           DebuggerHandle;
        std::string     DebugFilename;
        bool            DebugStarted = false;

        public:
        bool StartDebugging( std::string filename );
        bool SetDebugFile( std::string filename );
        void DebugLog( std::string text );
        void EOL( void );
        void EndDebugging( void );
    };
#endif // DEBUGGERINC
