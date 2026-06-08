#pragma once

#include "../../common/network/Server.hpp"
#include "../../common/map/MapGenerator.hpp"
#include "../../common/physics/PhysicsEngine.hpp"
#include "../../common/network/packettype/PacketType.hpp"

class ServerImpl : public network::serverInterface<PVP_PACKET>
{
public:
    ServerImpl(uint16_t port);
    virtual ~ServerImpl();


private:

    PhysicsEngine m_engine;
    MapGenerator m_mapGen;
};