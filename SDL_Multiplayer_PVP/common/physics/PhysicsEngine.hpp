#pragma once 

#include <vector>
#include "PhysicsBody.hpp"

class PhysicsEngine
{
public:
    void AddBody(PhysicsBody* body)
    {
        m_bodies.push_back(body);
    }
    void RemoveBody(PhysicsBody* body)
    {
        m_bodies.erase(std::remove(m_bodies.begin(), m_bodies.end(), body), m_bodies.end());
    }

    void update(float ts)
    {
        // collision with static bodies
        for (auto* body : m_bodies)
        {
            if (body->isStatic) 
                continue;

            // gravity
            body->velocity.y += gravity * ts;
            bool wasGrounded = body->isGrounded;
            body->isGrounded = false;            

            float accel = wasGrounded ? body->groundAccel : body->airAccel;

            if (body->inputX != 0)
            {
                float targetVelX = body->inputX * body->maxSpeed;
                float diff = targetVelX - body->velocity.x;
                float step = accel * ts;
                
                if (std::abs(diff) <= step)
                    body->velocity.x = targetVelX;
                else
                    body->velocity.x += std::copysign(step, diff);
            }
            else
            {
                float friction = wasGrounded ? body->groundFriction : 0.0f;
                float step = friction * ts;

                if (std::abs(body->velocity.x) <= step)
                    body->velocity.x = 0.0f;
                else
                    body->velocity.x -= std::copysign(step, body->velocity.x);
            }

            // separation of the coords to understand where the collision is happening
            body->bounds.pos.x += body->velocity.x * ts;

            for (auto* other : m_bodies)
            {
                if (body == other || !other->isStatic) 
                    continue;
                
                if (other->isOneWay)
                    continue;

                if (body->bounds.Intersects(other->bounds))
                {
                    if (body->velocity.x > 0) // moving right
                    {
                        body->bounds.pos.x = other->bounds.pos.x - body->bounds.size.x;
                        body->velocity.x = 0;
                    }
                    else if (body->velocity.x < 0) // moving left
                    {
                        body->bounds.pos.x = other->bounds.pos.x + other->bounds.size.x;
                        body->velocity.x = 0;
                    }
                }
            }

            body->bounds.pos.y += body->velocity.y * ts;

            for (auto* other : m_bodies)
            {
                if (body == other || !other->isStatic)
                    continue;

                if (body->bounds.Intersects(other->bounds))
                {
                    if (body->velocity.y > 0) // moving down
                    {
                        body->bounds.pos.y = other->bounds.pos.y - body->bounds.size.y;
                        body->velocity.y = 0;
                        body->isGrounded = true;
                    }
                    else if (body->velocity.y < 0) // moving up
                    {
                        body->bounds.pos.y = other->bounds.pos.y + other->bounds.size.y;
                        body->velocity.y = 0;
                    }
                }
            }
        }

        for (size_t i = 0; i < m_bodies.size(); i++)
        {
            for (size_t j = i + 1; j < m_bodies.size(); j++)
            {
                PhysicsBody* bodyA = m_bodies[i];
                PhysicsBody* bodyB = m_bodies[j];

                if (bodyA->bounds.Intersects(bodyB->bounds))
                {
                    // 
                }
            }
        }
    }

private:
    float gravity = 981.0f;
    std::vector<PhysicsBody*> m_bodies;
};