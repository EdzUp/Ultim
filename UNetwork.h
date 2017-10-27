/*
    UNetwork.h - Copyright (C)EdzUp
    Programmed by Ed 'EdzUp' Upton
*/

#ifndef ULTIMNETWORK
    #define ULTIMNETWORK
    #include "enet-1.3.13/include/enet/enet.h"

    class UNetwork {
        bool                NetworkOk;
        bool                Hosting;
        ENetAddress         NetworkAddress;
        ENetHost*           Server;
        ENetHost*           Client;
        ENetPeer*           Peer;

        ENetEvent           Event;
        ENetPacket*         Packet;

        long                MaximumConnections;
        long                CurrentConnections;

    public:
        const long  BANDWIDTH_56K       =0;
        const long  BANDWIDTH_56KDown   =7200;
        const long  BANDWIDTH_56KUp     =1800;

        bool Initialise( void );
        void Shutdown( void );
        bool StartServer( long port, long MaxConnections );
        bool StartClient( long mode );
        void Update( void );
        bool ClientConnect( char* HostIP, long port );
        void ClientDisconnect( void );
        void BroadcastPacket( char* PacketData, long reliability = ENET_PACKET_FLAG_RELIABLE );
        void SendPacket( char* PacketData, long reliability = ENET_PACKET_FLAG_RELIABLE );
    };
#endif // ULTIMNETWORK
