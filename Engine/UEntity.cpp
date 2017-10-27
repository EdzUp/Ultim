//
//  UEntity.cpp - Copyright (C)EdzUp
//  Programmed by Ed 'EdzUp' Upton
//

#include "UEntity.h"

long UEntityClass::CreateSurface( void ) {
    //this will create the surface for the engine
    USurface Temp;

    Temp.CreateSurface( NextSurfaceID );

    Surface.push_back( Temp );

    NextSurfaceID ++;

    return( NextSurfaceID -1 );     //return the previous ID so they can adjust the newly created surface
}
