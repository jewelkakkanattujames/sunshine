#include <iostream>
#include <vector>
#include "raylib.h"
#include <raymath.h>

class Rigidbody 
{
public:
    Vector2 position;
    Vector2 velocity;
};

class Agent 
{
public:
    Rigidbody rigidbody;
    Rectangle rectangle;
    float maxSpeed;
    float maxAcceleration;
};

Vector2 seek(const Vector2& agentPos, const Vector2& targetPos, float maxSpeed) // Velocity for the agent to raech the target position
{
    Vector2 desiredVelocity = Vector2Normalize(Vector2Subtract(targetPos, agentPos));
    desiredVelocity.x *= maxSpeed;
    desiredVelocity.y *= maxSpeed;
    return desiredVelocity;
}

Vector2 flee(const Vector2& agentPos, const Vector2& targetPos, float maxSpeed)  // velocity to move avay from the tarhet position
{
    Vector2 desiredVelocity = Vector2Normalize(Vector2Subtract(agentPos, targetPos));
    desiredVelocity.x *= maxSpeed;
    desiredVelocity.y *= maxSpeed;
    return desiredVelocity;
}

void update(Rigidbody& rb, float deltaTime) // updates the position of the agent.
{
    rb.position.x += rb.velocity.x * deltaTime;
    rb.position.y += rb.velocity.y * deltaTime;
}

int main(void)
{
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Seek and Flee behaviour -AI Lab2");
    SetTargetFPS(60);

    std::vector<Agent> agents;

    Agent agent1;
    agent1.rigidbody.position = { 100, 100 };
    agent1.rigidbody.velocity = { 2, 2 };
    agent1.maxSpeed = 9; // Increase max speed to 9
    agent1.maxAcceleration = 0.2f;
    agent1.rectangle = { agent1.rigidbody.position.x, agent1.rigidbody.position.y, 40, 40 };
    agents.push_back(agent1);

    Agent agent2;
    agent2.rigidbody.position = { 200, 200 };
    agent2.rigidbody.velocity = { -1, -1 };
    agent2.maxSpeed = 8; // Increase max speed to 8
    agent2.maxAcceleration = 0.1f;
    agent2.rectangle = { agent2.rigidbody.position.x, agent2.rigidbody.position.y, 40, 40 };
    agents.push_back(agent2);

    Agent fleeAgent; // Red square agent
    fleeAgent.rigidbody.position = { 400, 300 };
    fleeAgent.rigidbody.velocity = { 0, 0 };
    fleeAgent.maxSpeed = 5;
    fleeAgent.maxAcceleration = 0.1f;
    fleeAgent.rectangle = { fleeAgent.rigidbody.position.x, fleeAgent.rigidbody.position.y, 40, 40 };
    agents.push_back(fleeAgent);

    Vector2 fleeTarget = { 600, 500 };

    while (!WindowShouldClose()) 
    {
        // Update
        float deltaTime = GetFrameTime();

        Vector2 mousePos = GetMousePosition();
        bool isMouseDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);

        // Update flee target position
        if (isMouseDown) 
        {
            fleeTarget = mousePos;
        }

        for (Agent& agent : agents) 
        {
            if (isMouseDown) 
            {
                Vector2 desiredVelocity = seek(agent.rigidbody.position, mousePos, agent.maxSpeed);
                agent.rigidbody.velocity.x += desiredVelocity.x;
                agent.rigidbody.velocity.y += desiredVelocity.y;

                // Limit velocity to the maximum speed
                float speed = Vector2Length(agent.rigidbody.velocity);
                if (speed > agent.maxSpeed)
                {
                    agent.rigidbody.velocity = Vector2Scale(agent.rigidbody.velocity, agent.maxSpeed / speed);
                }
            }
            else 
            {
                Vector2 fleeAcceleration = flee(agent.rigidbody.position, fleeTarget, agent.maxSpeed);
                agent.rigidbody.velocity.x += fleeAcceleration.x;
                agent.rigidbody.velocity.y += fleeAcceleration.y;

                // Limit velocity to the maximum speed
                float speed = Vector2Length(agent.rigidbody.velocity);
                if (speed > agent.maxSpeed)
                {
                    agent.rigidbody.velocity = Vector2Scale(agent.rigidbody.velocity, agent.maxSpeed / speed);
                }
            }

            update(agent.rigidbody, deltaTime);
            agent.rectangle.x = agent.rigidbody.position.x;
            agent.rectangle.y = agent.rigidbody.position.y;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Render agents
        for (const Agent& agent : agents) 
        {
            DrawRectangleRec(agent.rectangle, BLUE);

            // Draw desired velocity vector
            Vector2 agentCenter = { agent.rectangle.x + agent.rectangle.width / 2, agent.rectangle.y + agent.rectangle.height / 2 };
            Vector2 desiredVelocity = isMouseDown ? seek(agent.rigidbody.position, mousePos, agent.maxSpeed) : flee(agent.rigidbody.position, fleeTarget, agent.maxSpeed);
            Vector2 desiredVelocityEnd = Vector2Add(agentCenter, Vector2Scale(desiredVelocity, 50));
            DrawLineEx(agentCenter, desiredVelocityEnd, 2, GREEN);// represents desired velocity of each agent
        }

        // Render flee target
        DrawRectangle(static_cast<int>(fleeTarget.x) - 5, static_cast<int>(fleeTarget.y) - 5, 10, 10, RED);// flee target

        EndDrawing();
    }

    CloseWindow();
    return 0;
}