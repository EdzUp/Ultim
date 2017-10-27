/*
    USurface.h - Copyright (C) EdzUp
    Programmed by Ed 'EdzUp' Upton
*/

#ifndef USURFACECLASS
    #define USURFACECLASS
    #include <vector>
    #include "Vec3.h"

    class USurface {
    private:
        long ID;
        std::vector <VecVertex>     Vertices;
        long                        NextVertexID =1;

    public:
        USurface CreateSurface( long NId );
        long SurfaceID( void );

        long AddVertex( float x, float y, float z );
        void VertexColor( int id, float r, float g, float b );

        bool RemoveVertex( int id );
    };
#endif // USURFACECLASS
