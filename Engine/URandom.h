/*
    ERandom.h - Copyright (C)EdzUp
    Programmed by Ed 'EdzUp' Upton
*/

#ifndef _ERANDOM_
    #define _ERANDOM_ 1

	#include <stdio.h>
	#include <time.h>
	#include <math.h>
	#include <chrono>
	#include <iostream>
	#include <ctime>
	#include <sys/time.h>
	#include <unistd.h>

class URandomClass {
    static const int               iSLIP = 27;
    static const int               i31BITSIGN = 0x7FFFFFFF;
    static const int               iSCANUPSTART =1;

    public:
    int                     iScanUp;
    int                     iSeed;
    int                     iValue;

    void Initialise( void );
    void Seed( long iRNDSeed, long iStartScanUp = 1 );
    void Randomize( void );
    long Generate( long iMaxValue );
};
#endif // _ERANDOM_
