#pragma once

#include "raylib.h"
#include "Rigidbody.h"
#include <list>

class IBehaviour;

class Agent
{
public:
    float maxSpeed{ 50.0f };
    Vector2 acceleration;
    Rigidbody* rigidbody;

    Agent();
    Agent(Vector2 position, float radius, Color colour);

    virtual ~Agent() {};

    // update the agent and its behaviours
    virtual void Update(float deltaTime);
    // draw the agent
    virtual void Draw();
    // add a behaviour to the agent
    void AddBehaviour(IBehaviour* behaviour);

    void SetPosition(Vector2 position);
    Vector2 GetPosition();

private:
    float radius{ 5.0f };
    Color colour{ BLUE };
    std::list<IBehaviour*> behaviours;
};