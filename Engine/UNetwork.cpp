/*
    ENetwork.cpp - Copyright (C)EdzUp
    Programmed by Ed 'EdzUp' Upton
*/

#include "UNetwork.h"       //network header file
#include <stdio.h>

//#include "enet-1.3.13/include/enet/enet.h"

bool UNetwork::Initialise( void ) {
    NetworkOk = false;
    Hosting = false;

    if ( enet_initialize () != 0 ) {
        return( false );
    }

    NetworkOk = true;
    atexit (enet_deinitialize);
    return( true );
}

void UNetwork::Shutdown( void ) {
    if ( Server != NULL ) enet_host_destroy( Server );
    if ( Client != NULL ) enet_host_destroy( Client );
    enet_deinitialize();
    NetworkOk = false;
}

bool UNetwork::StartServer( long port, long MaxConnections ) {
    NetworkAddress.host = ENET_HOST_ANY;
    NetworkAddress.port = port;

    MaximumConnections = MaxConnections;
    CurrentConnections = 0;

    Server = enet_host_create( &NetworkAddress, 32, 2, 0, 0 );
    if ( Server == NULL ) {
        return( false );
    }

    Hosting = true;
    return( true );
}

bool UNetwork::StartClient( long mode = 0 ) {
    //57600 /8 is 56k bandwidth 56k downstream, 14400 /8 is 14kbs upstream
    long UpStream;
    long DownStream;

    switch ( mode ) {
    default:
        DownStream = BANDWIDTH_56KDown;
        UpStream = BANDWIDTH_56KUp;
        break;
    }
    Client = enet_host_create( NULL, 1, 2, DownStream, UpStream );
    if ( Client == NULL ) {
        return( false );
    }

    return( true );
}

void UNetwork::Update( void ) {
    if ( Hosting == true ) {
        //if server is here then check that
        while ( enet_host_service( Server, &Event, 1 ) >0 ) {
            switch( Event.type ) {
            case ENET_EVENT_TYPE_CONNECT:
                printf ("A new client connected from %x:%u.\n",
                Event.peer -> address.host,
                Event.peer -> address.port);
                // Store any relevant client information here.
                Event.peer -> data = (void*)"Client information";
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                printf ("A packet of length %uc containing %s was received from %s on channel %uc.\n",
                    (unsigned int) Event.packet -> dataLength,
                    (char*)Event.packet -> data,
                    (char*)Event.peer -> data,
                    (unsigned int) Event.channelID);
                // Clean up the packet now that we're done using it.
                enet_packet_destroy (Event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                printf ("%s disconnected.\n", (char*)Event.peer -> data);
                // Reset the peer's client information.
                Event.peer -> data = NULL;
                break;
            case ENET_EVENT_TYPE_NONE:
                break;
            }
        }
    }

    while ( enet_host_service( Client, &Event, 1 ) >0 ) {
    }
}

bool UNetwork::ClientConnect( char* HostIP, long port ) {
    enet_address_set_host( &NetworkAddress, HostIP );
    NetworkAddress.port = port;
    Peer = enet_host_connect( Client, &NetworkAddress, 2, 0 );
    if ( Peer == NULL ) {
        return( false );
    }
    if ( enet_host_service( Client, &Event, 5000 ) >0 && Event.type == ENET_EVENT_TYPE_CONNECT ) {
        return( true );
    }
    enet_peer_reset( Peer );
    return( false );
}

void UNetwork::ClientDisconnect( void ) {
    enet_peer_disconnect( Peer, 0 );

    while( enet_host_service( Client, &Event, 3000 ) >0 ) {
        if ( Event.type == ENET_EVENT_TYPE_RECEIVE ) {
            enet_packet_destroy( Event.packet );
        }
        if ( Event.type == ENET_EVENT_TYPE_DISCONNECT ) {
            return;
        }
    }

    //force close the connection
    enet_peer_reset (Peer);
}

void UNetwork::BroadcastPacket( char* PacketData, long reliability ) {
    Packet = enet_packet_create( PacketData, sizeof( PacketData )+1, reliability );
    enet_host_broadcast( Client, 0, Packet );
}

void UNetwork::SendPacket( char* PacketData, long reliability ) {
    Packet = enet_packet_create( PacketData, sizeof( PacketData )+1, reliability );
    enet_peer_send( Peer, 0, Packet );
}
