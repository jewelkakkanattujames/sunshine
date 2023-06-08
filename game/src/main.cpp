#include <iostream>
#include <vector>
#include "raylib.h"
#include <raymath.h>

struct Circle
{
    Vector2 center;
    float radius;
};

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
    Vector2 leftWhisker; // Added left whisker
    Vector2 rightWhisker; // Added right whisker

    void obstacleAvoidance(const Circle& obstacle, float avoidanceForce)
    {
        Vector2 ahead = Vector2Add(rigidbody.position, Vector2Scale(Vector2Normalize(rigidbody.velocity), 50.0f));
        Vector2 ahead2 = Vector2Add(rigidbody.position, Vector2Scale(Vector2Normalize(rigidbody.velocity), 25.0f));

        // Additional left and right whiskers
        Vector2 leftWhisker = Vector2Rotate(Vector2Normalize(rigidbody.velocity), -45.0f);
        Vector2 rightWhisker = Vector2Rotate(Vector2Normalize(rigidbody.velocity), 45.0f);// whisker vectors

        Vector2 avoidance = { 0, 0 };
        bool collisionDetected = false;

        if (CheckCollisionCircleRec(obstacle.center, obstacle.radius, rectangle))
        {
            avoidance = Vector2Subtract(rigidbody.position, obstacle.center);
            collisionDetected = true;
        }
        else if (CheckCollisionCircleRec(obstacle.center, obstacle.radius, { ahead.x, ahead.y, rectangle.width, rectangle.height }))
        {
            avoidance = Vector2Subtract(rigidbody.position, obstacle.center);
            collisionDetected = true;
        }
        else if (CheckCollisionCircleRec(obstacle.center, obstacle.radius, { ahead2.x, ahead2.y, rectangle.width, rectangle.height }))
        {
            avoidance = Vector2Subtract(rigidbody.position, obstacle.center);
            collisionDetected = true;
        }
        else if (CheckCollisionCircleRec(obstacle.center, obstacle.radius, { ahead.x + leftWhisker.x, ahead.y + leftWhisker.y, rectangle.width, rectangle.height }))
        {
            avoidance = Vector2Subtract(rigidbody.position, obstacle.center);
            collisionDetected = true;
        }
        else if (CheckCollisionCircleRec(obstacle.center, obstacle.radius, { ahead.x + rightWhisker.x, ahead.y + rightWhisker.y, rectangle.width, rectangle.height }))
        {
            avoidance = Vector2Subtract(rigidbody.position, obstacle.center);
            collisionDetected = true;
        }

        if (collisionDetected)
        {
            avoidance = Vector2Normalize(avoidance);
            avoidance = Vector2Scale(avoidance, avoidanceForce);

            rigidbody.velocity = Vector2Add(rigidbody.velocity, avoidance);
            float speed = Vector2Length(rigidbody.velocity);
            if (speed > maxSpeed)
            {
                rigidbody.velocity = Vector2Scale(rigidbody.velocity, maxSpeed / speed);
            }
        }
    }
};

int main(void)
{
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Obstacle Avoidance - AI Lab 3");
    SetTargetFPS(60);

    std::vector<Agent> agents;

    // Create and add agents
    for (int i = 0; i < 3; i++)
    {
        Agent agent;
        agent.rigidbody.position.x = 100.0f + i * 100.0f;
        agent.rigidbody.position.y = 100.0f + i * 100.0f;
        agent.rigidbody.velocity = { 2, 2 };
        agent.maxSpeed = 9;
        agent.maxAcceleration = 0.2f;
        agent.rectangle = { agent.rigidbody.position.x, agent.rigidbody.position.y, 40, 40 };

        // Additional left and right whiskers
        agent.leftWhisker = Vector2Rotate(Vector2Normalize(agent.rigidbody.velocity), -30.0f);
        agent.rightWhisker = Vector2Rotate(Vector2Normalize(agent.rigidbody.velocity), 30.0f);

        agents.push_back(agent);
    }

    std::vector<Circle> obstacles;

    while (!WindowShouldClose())
    {
        // Update
        float deltaTime = GetFrameTime();

        // Place obstacles with mouse click
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mousePos = GetMousePosition();
            Circle obstacle = { mousePos, 50 };
            obstacles.push_back(obstacle);
        }

        for (Agent& agent : agents)
        {
            for (const Circle& obstacle : obstacles)
            {
                agent.obstacleAvoidance(obstacle, 0.5f);
            }

            agent.rigidbody.position.x += agent.rigidbody.velocity.x * deltaTime;
            agent.rigidbody.position.y += agent.rigidbody.velocity.y * deltaTime;
            agent.rectangle.x = agent.rigidbody.position.x;
            agent.rectangle.y = agent.rigidbody.position.y;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Render agents
        for (const Agent& agent : agents)
        {
            DrawRectangleRec(agent.rectangle, GREEN);

            // Draw whiskers
            DrawLine(agent.rigidbody.position.x, agent.rigidbody.position.y,
                agent.rigidbody.position.x + agent.leftWhisker.x * 60,
                agent.rigidbody.position.y + agent.leftWhisker.y * 60, BLACK); 
            DrawLine(agent.rigidbody.position.x, agent.rigidbody.position.y,
                agent.rigidbody.position.x + agent.rightWhisker.x * 60,
                agent.rigidbody.position.y + agent.rightWhisker.y * 60, BLACK); 
        }

        // Render obstacles
        for (const Circle& obstacle : obstacles)
        {
            DrawCircleV(obstacle.center, obstacle.radius, RED);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
