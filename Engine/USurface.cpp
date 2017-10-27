/*
    USurface.cpp - Copyright (C)EdzUp
    Programmed by Ed 'EdzUp' Upton
*/

#include "USurface.h"

USurface USurface::CreateSurface( long NId ) {
    //basically make a empty surface and set its ID then return it to the host function
    USurface temp;      //the temp handle for this surface

    temp.ID = NId;

    return( temp );
}

long USurface::SurfaceID( void ) {
    //return the surface ID
    return ( ID );
}

long USurface::AddVertex( float x, float y, float z ) {
    //add a vertex to the surface
    VecVertex Entry;

    Entry.Id = NextVertexID;

    Entry.X = x;
    Entry.Y = y;
    Entry.Z = z;

    Entry.Red = 1.0f;
    Entry.Green = 1.0f;
    Entry.Blue = 1.0f;
    Entry.Alpha = 1.0f;

    Vertices.push_back( Entry );

    NextVertexID++;

    return( NextVertexID -1 );
}

void USurface::VertexColor( int id, float r, float g, float b ) {
    //change a vertex colour of a surface
    int Vert;

    for ( Vert =0; Vertices.size(); Vert++ ) {
        if ( Vertices[ Vert ].Id == id ) {
            Vertices[ Vert ].Red = r;
            Vertices[ Vert ].Green = g;
            Vertices[ Vert ].Blue = b;
        }
    }
}

bool USurface::RemoveVertex( int id ) {
    //this will run through the vector and remove an entry from the render list
    unsigned long pos;

    for ( pos=0; pos<Vertices.size(); pos++ ) {
        if ( Vertices[ pos ].Id == id ) {
            Vertices.erase( Vertices.begin() +pos );
            return( true );
        }
    }

    return( false );
}
