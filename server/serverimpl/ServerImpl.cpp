#include "ServerImpl.hpp"

ServerImpl::ServerImpl(uint16_t port)
    : serverInterface<pvpPacket>(port)
{
    m_players.reserve(2);
}

ServerImpl::~ServerImpl()
{
}

void ServerImpl::UpdatePhysics(float ts)
{
    m_engine.update(ts);
}

bool ServerImpl::ListPlayers()
{
    if (m_players.empty())
        return false;

    for (auto& player : m_players)
    {
        uint32_t id = player.id;
        int x = player.GetPhysicsBody()->bounds.pos.x;
        int y = player.GetPhysicsBody()->bounds.pos.y;

        LOG_INFO_CONSOLE("[Player #" + std::to_string(id) +
        "]: { x: " + std::to_string(x) + ", y: " + std::to_string(y) + " }");
    }

    return true;
}
bool ServerImpl::KickPlayer()
{
    if (!m_mapGenerated)
    {
        m_mapSeed = static_cast<uint32_t>(std::time(nullptr));
        m_mapGen.SetSeed(m_mapSeed);

        AdvancedMap map = m_mapGen.Generate(true);
        m_mapGen.AddPhysicsToMap(map, m_engine);

        FindSpawnPoints(map);
        m_mapGenerated = true;

        LOG_INFO_CONSOLE("Map generated with seed: " + std::to_string(m_mapSeed));
        // debug
        for (auto body : m_engine.GetBodies())
        {
            LOG_INFO_CONSOLE("- Body x: " + std::to_string(body->bounds.size.x)
                + " y: " + std::to_string(body->bounds.size.y)
                + " w: " + std::to_string(body->bounds.size.x)
                + " h: " + std::to_string(body->bounds.size.y));
        }
    }
    return false;
}

bool ServerImpl::OnClientConnect(std::shared_ptr<network::connection<pvpPacket>> client)
{
    return m_players.size() < 3;
}

void ServerImpl::OnClientValidated(std::shared_ptr<network::connection<pvpPacket>> client)
{
    if (!m_mapGenerated)
    {
        m_mapSeed = static_cast<uint32_t>(std::time(nullptr));
        m_mapGen.SetSeed(m_mapSeed);

        AdvancedMap map = m_mapGen.Generate(true);
        m_mapGen.AddPhysicsToMap(map, m_engine);

        FindSpawnPoints(map);
        m_mapGenerated = true;

        LOG_INFO_CONSOLE("Map generated with seed: " + std::to_string(m_mapSeed));
        // debug
        for (auto body : m_engine.GetBodies())
        {
            LOG_INFO_CONSOLE("- Body x: " + std::to_string(body->bounds.size.x)
                + " y: " + std::to_string(body->bounds.size.y)
                + " w: " + std::to_string(body->bounds.size.x)
                + " h: " + std::to_string(body->bounds.size.y));
        }
    }

    network::message<pvpPacket> msg;
    msg.header.id = pvpPacket::MapSeed;
    msg << m_mapSeed;

    MessageClient(client, msg);

    CreatePlayers(client->GetID());
}

void ServerImpl::OnClientDisconnect(std::shared_ptr<network::connection<pvpPacket>> client)
{
    serverInterface<pvpPacket>::OnClientDisconnect(client);
}

void ServerImpl::OnMessage(std::shared_ptr<network::connection<pvpPacket>> client, network::message<pvpPacket> &msg)
{
    switch (msg.header.id)
    {
        case pvpPacket::MapSeed:
            // map seed is only sent by the server by the client
            break;
        case pvpPacket::PlayerJoin:
            break;
        case pvpPacket::PlayerLeave:
            break;
        case pvpPacket::PlayerMove:
            MessageAllClient(msg, client);
            break;
    }
}

void ServerImpl::FindSpawnPoints(const AdvancedMap &map)
{
    const float TILE_SIZE = 64.0f;
    m_spawnPoints.clear();

    int targetRow = map.rows - 4;

    // search left to right
    for (int c = 0; c < map.columns / 2; c++)
    {
        if (map.grid[targetRow][c].isSolid)
        {
            vec2 spawn1 = { c * TILE_SIZE, (targetRow - 1) * TILE_SIZE };
            m_spawnPoints.push_back(spawn1);
            break;
        }
    }

    // right to left
    for (int c = map.columns - 1; c > map.columns / 2; c--)
    {
        if (map.grid[targetRow][c].isSolid)
        {
            vec2 spawn2 = { c * TILE_SIZE, (targetRow - 1) * TILE_SIZE };
            m_spawnPoints.push_back(spawn2);
            break;
        }
    }
}

void ServerImpl::CreatePlayers(uint32_t id)
{
    vec2 spawnPos = m_spawnPoints[m_players.size() % m_spawnPoints.size()];

    auto* pBody = new PhysicsBody();
    pBody->bounds.pos = spawnPos;
    pBody->bounds.size = { 64.0f, 128.0f };
    pBody->isStatic = false;

    m_engine.AddBody(pBody);

    Player newPlayer;
    newPlayer.id = id;
    newPlayer.GetPhysicsBody() = pBody;
    m_players.push_back(newPlayer);

    network::message<pvpPacket> msg;
    msg.header.id = pvpPacket::PlayerJoin;
    msg << spawnPos.x << spawnPos.y << id;

    MessageAllClient(msg, nullptr);
}
