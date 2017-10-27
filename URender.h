/*
    URender.h - Copyright (C)EdzUp
    Programmed by Ed 'EdzUp' Upton
*/

#ifndef URENDER
    #define URENDER

    #include "UEntity.h"        //Ultim Entity system
    #include <vector>

    class URenderClass {
    private:
        std::vector <UEntityClass>  EntityList;     //this is the entity list for the entire project
    public:
        long            NextID = 0;
        UEntityClass CreateEntity( void );
        void Update( UEntityClass Entry );
        void Remove( UEntityClass Entry );

        void ShowEm( void );

        void Render( void );
    };
#endif // URENDER
