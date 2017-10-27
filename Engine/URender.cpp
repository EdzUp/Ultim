/*
    URender.cpp - Copyright (C)EdzUp
    Written by Ed 'EdzUp' Upton
*/

#include "URender.h"            //header declarations
#include "iostream"

UEntityClass URenderClass::CreateEntity( void ) {
    UEntityClass Temp;

    Temp.ID = NextID;
    Temp.Name = "Untitled";

    NextID++;

    EntityList.push_back( Temp );

    return( Temp );
}

void URenderClass::Update( UEntityClass Entry ) {
    //this will run through the vector and update the render list
    //if the item isnt in the render list then its created
    unsigned long pos;

    for ( pos=0; pos<EntityList.size(); pos++ ) {
        if ( EntityList[ pos ].ID == Entry.ID ) {
            EntityList[ pos ] = Entry;      //update the whole thing so that its all in there.
            return;
        }
    }

    //if its not in there add a new entry
    EntityList.push_back( Entry );
}

void URenderClass::Remove( UEntityClass Entry ) {
    //this will run through the vector and remove an entry from the render list
    unsigned long pos;

    for ( pos=0; pos<EntityList.size(); pos++ ) {
        if ( EntityList[ pos ].ID == Entry.ID ) {
            EntityList.erase( EntityList.begin() +pos );
            return;
        }
    }
}

void URenderClass::ShowEm( void ) {
    unsigned long pos;

    for ( pos=0; pos<EntityList.size(); pos++ ) {
        std::cout << pos << ":" << EntityList[ pos ].Name << std::endl;
    }
}

void URenderClass::Render( void ) {
    //this will run through all the entities and render the surfaces that need rendering allowing the whole
    //thing to go to the screen for display to the user.
    unsigned long pos;

    for ( pos =0; pos<EntityList.size(); pos++ ) {
        //this will then render each entity in the list for the user to peruse :D
    }
}
