/*
    URandom.cpp - Copyright (C)EdzUp
    Programmed by Ed 'EdzUp' Upton
*/

#include "URandom.h"

void URandomClass::Initialise( void ) {
    iScanUp = iSCANUPSTART;
    iSeed = 0;
    iValue = 0;
}

//seed the random number generator
void URandomClass::Seed( long iRNDSeed, long iStartScanUp ) {
    if ( iStartScanUp == 1 ) iStartScanUp = iSCANUPSTART;
    iValue = ( iRNDSeed & i31BITSIGN );
    iSeed = ( iRNDSeed & i31BITSIGN );
    iScanUp = ( iStartScanUp & i31BITSIGN );
}

void URandomClass::Randomize( void ) {
    struct timeval t;
    gettimeofday(&t, NULL);
    Seed( (t.tv_sec + (t.tv_usec / 1000000.0)) * 1000.0 );
}

long URandomClass::Generate( long iMaxValue ) {
    //Step up
    if ( iMaxValue ==0 ) return( 0 );			//if the values is 0-0 then just return 0

    iScanUp = (iScanUp + 1);                        //Increase by one
    iScanUp = (iScanUp & i31BITSIGN);               //Make sure the sign value does Not get used (wraps it)
    //Re-seed self with new value
    iSeed = ((iSeed+(iSeed >> 8))-(iSLIP ^iScanUp) ) & i31BITSIGN; //Calculate new seed for next time
    //Wrap to get correct value
    iValue = (iSeed % iMaxValue);  //Just in case we need it
    return( iValue );
}
