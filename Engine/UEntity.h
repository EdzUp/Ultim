//
//  UEntity.h - Copyright (C)EdzUp
//  Programmed by Ed 'EdzUp' Upton
//

#ifndef UENTITY
    #define UENTITY

    #include "USurface.h"
    #include <string>
    #include <vector>

    class UEntityClass {
    private:
        std::vector <USurface>      Surface;        //this is the surface list for this items rendering
        long                        NextSurfaceID =1;
    public:
        std::string                 Name;           //its name for locating information about it etc
        long                        ID;             //its machine ID

        long CreateSurface( void );

        //Surfaces will be in here for rendering
    };
#endif // UENTITY
