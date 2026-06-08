#include "Player.hpp"

Player::Player()
{
}

Player::~Player()
{
}

void Player::update(float ts, GameContext& context)
{
    if (SDL_GetTicks() - startTime > m_animSpeedMs)
    {
        currentFrame = (currentFrame + 1) % m_numFrames;
        startTime = SDL_GetTicks();
    }

    body->inputX = 0.0f;
    if (context.inputManager.IsActionHeld("move_left"))
        body->inputX = -1.0f;
    if (context.inputManager.IsActionHeld("move_right"))
        body->inputX = 1.0f;

    if (context.inputManager.IsActionPressed("move_up") && body->isGrounded)
        body->velocity.y = -800.0f;
    // need to send to the server the inputX to know the direction and if he jumped
}

void Player::render(Renderer& renderer)
{
    if (!playerTexture)
        return;

    float texWidth = 0.0f;
    float texHeight = 0.0f;
    SDL_GetTextureSize(playerTexture, &texWidth, &texHeight);

    float frameWidth = texWidth / m_numFrames;

    SDL_FRect src = {currentFrame * frameWidth, 0.0f, frameWidth, texHeight};
    SDL_FRect dst = {body->bounds.pos.x, body->bounds.pos.y, 64.0f, 64.0f};

    renderer.DrawTexture(playerTexture, &src, &dst);
}
// a getter that returns a reference to the player texture pointer, so it can be modified outside the class
SDL_Texture*& Player::GetPlayerTexture()
{
    return playerTexture;
}

PhysicsBody*& Player::GetPhysicsBody()
{
    return body;
}
