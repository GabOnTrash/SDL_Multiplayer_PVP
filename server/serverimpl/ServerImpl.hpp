#pragma once

#include <chrono>

// #include "../../client/player/Player.hpp"
#include "../../common/network/Server.hpp"
#include "../../common/map/MapGenerator.hpp"
#include "../../common/physics/PhysicsEngine.hpp"
#include "../../common/network/packet/PacketType.hpp"
#include "../../common/network/packet/PacketStructure.hpp"

class Player
{
public:
    Player(){}
    ~Player(){}

    PhysicsBody*& GetPhysicsBody(){return body;}

    uint32_t id = 0;

private:
    PhysicsBody* body = nullptr;

    int currentFrame = 0;
    uint32_t startTime = 0; // Modificato in Uint32, il tipo corretto per SDL_GetTicks()

    int m_numFrames = 11;
    uint32_t m_animSpeedMs = 100; // Millisecondi tra un frame e l'altro
};


class ServerImpl : public network::serverInterface<pvpPacket>
{
public:
    ServerImpl(uint16_t port);
    ~ServerImpl() final;

    void UpdatePhysics(float ts);
    bool ListPlayers();
    bool KickPlayer();
    void OnClientValidated(std::shared_ptr<network::connection<pvpPacket>> client) final;

protected:
    bool OnClientConnect(std::shared_ptr<network::connection<pvpPacket>> client) final;
    void OnClientDisconnect(std::shared_ptr<network::connection<pvpPacket>> client) final;
    void OnMessage(std::shared_ptr<network::connection<pvpPacket>> client, network::message<pvpPacket> &msg) final;

private:
    void FindSpawnPoints(const AdvancedMap& map);
    void CreatePlayers(uint32_t id);

    PhysicsEngine m_engine;
    MapGenerator m_mapGen;

    std::vector<Player> m_players;

    bool m_mapGenerated = false;
    uint32_t m_mapSeed = 0;
    std::vector<vec2> m_spawnPoints;
};