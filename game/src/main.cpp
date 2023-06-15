#include "raylib.h"
#include <vector>

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 900
#define TILE_SIZE 100

struct Tile 
{
    Vector2 position;
    bool isWall;
    std::vector<Tile*> connectedTiles;
};

struct Character 
{
    Texture2D texture;
    Vector2 position;
    Rectangle bounds;
    float speed;
};

std::vector<Tile> tiles;
Character character;

void generateTiles() // creation of tiles 
{
    const int rows = 8;
    const int cols = 8;

    int totalTiles = rows * cols;
    int wallCount = static_cast<int>(totalTiles * 0.2); // 20% of total tiles as walls

    // Create all tiles as floors initially
    for (int i = 0; i < totalTiles; ++i) 
    {
        Vector2 position = { 100 + (i % cols) * TILE_SIZE, 100 + (i / cols) * TILE_SIZE };
        tiles.push_back({ position, false });
    }

    // Randomly assign walls
    for (int i = 0; i < wallCount; ++i) 
    {
        int randomIndex = GetRandomValue(0, totalTiles - 1);
        Tile& tile = tiles[randomIndex];
        if (!tile.isWall) 
        {
            tile.isWall = true;
        }
        else 
        {
            // If the randomly chosen tile is already a wall, try again
            --i;
        }
    }
}

void drawGrid(const std::vector<Tile>& tiles) 
{
    const Color lineColor = GREEN;
    const Color wallColor = BLACK;
    const Color floorColor = WHITE;
    const Color gridLineColor = BLACK;

    for (const Tile& tile : tiles) 
    {
        Vector2 center = { tile.position.x + TILE_SIZE / 2, tile.position.y + TILE_SIZE / 2 };

        if (tile.isWall) 
        {
            DrawRectangle(static_cast<int>(tile.position.x), static_cast<int>(tile.position.y), TILE_SIZE, TILE_SIZE, wallColor);
        }
        else 
        {
            DrawRectangle(static_cast<int>(tile.position.x), static_cast<int>(tile.position.y), TILE_SIZE, TILE_SIZE, floorColor);

            for (const Tile* connectedTile : tile.connectedTiles) 
            {
                Vector2 connectedCenter = { connectedTile->position.x + TILE_SIZE / 2, connectedTile->position.y + TILE_SIZE / 2 };
                DrawLineEx(center, connectedCenter, 2, lineColor);
            }
        }

        DrawCircle(static_cast<int>(center.x), static_cast<int>(center.y), TILE_SIZE / 8, lineColor);
    }

    // Draw thin black lines along rows and columns
    for (int i = 0; i < tiles.size(); ++i) 
    {
        int row = i / 8;
        int col = i % 8;

        DrawLine(col * TILE_SIZE + 100, 100, col * TILE_SIZE + 100, SCREEN_HEIGHT - 100, BLACK);
        DrawLine(100, row * TILE_SIZE + 100, SCREEN_WIDTH - 100, row * TILE_SIZE + 100, BLACK);

    }
}


void initCharacter() 
{
    character.texture = LoadTexture("C:\\Ai assignments\\AI Lab3\\game\\assets\\textures\\doremon.png");
    character.position = tiles[0].position;
    character.bounds = { character.position.x + 10, character.position.y + 10, TILE_SIZE - 20, TILE_SIZE - 20 };
    character.speed = 2.0f;
}

void updateCharacterMovement() 
{
    // Move character based on key inputs (WASD)
    if (IsKeyDown(KEY_W) && character.position.y > 100) 
    {
        character.position.y -= character.speed;
        character.bounds.y -= character.speed;
    }
    else if (IsKeyDown(KEY_S) && character.position.y < SCREEN_HEIGHT - TILE_SIZE - 100)
    {
        character.position.y += character.speed;
        character.bounds.y += character.speed;
    }
    if (IsKeyDown(KEY_A) && character.position.x > 100) 
    {
        character.position.x -= character.speed;
        character.bounds.x -= character.speed;
    }
    else if (IsKeyDown(KEY_D) && character.position.x < SCREEN_WIDTH - TILE_SIZE - 100)
    {
        character.position.x += character.speed;
        character.bounds.x += character.speed;
    }

    // Check collision with walls
    for (const Tile& tile : tiles) 
    {
        if (tile.isWall && CheckCollisionRecs(character.bounds, { tile.position.x, tile.position.y, TILE_SIZE, TILE_SIZE }))
        {
            // Undo the movement if there is a collision with a wall
            if (IsKeyDown(KEY_W)) 
            {
                character.position.y += character.speed;
                character.bounds.y += character.speed;
            }
            else if (IsKeyDown(KEY_S)) 
            {
                character.position.y -= character.speed;
                character.bounds.y -= character.speed;
            }
            if (IsKeyDown(KEY_A)) 
            {
                character.position.x += character.speed;
                character.bounds.x += character.speed;
            }
            else if (IsKeyDown(KEY_D)) 
            {
                character.position.x -= character.speed;
                character.bounds.x -= character.speed;
            }
            break;
        }
    }
}

void drawCharacter() 
{
    Vector2 center = { character.position.x + TILE_SIZE / 2, character.position.y + TILE_SIZE / 2 };
    DrawTexture(character.texture, static_cast<int>(center.x - TILE_SIZE / 2), static_cast<int>(center.y - TILE_SIZE / 2), WHITE);
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "AI LAB 4");
    SetTargetFPS(60);

    generateTiles();
    initCharacter();

    // Define the adjacency connections between tiles
    const int rows = 8;
    const int cols = 8;

    for (int row = 0; row < rows; ++row) 
    {
        for (int col = 0; col < cols; ++col)
        {
            int index = row * cols + col;
            Tile& tile = tiles[index];

            if (col > 0 && !tiles[index - 1].isWall) 
            {
                tile.connectedTiles.push_back(&tiles[index - 1]); // Connect to the left tile
            }
            if (col < cols - 1 && !tiles[index + 1].isWall) 
            {
                tile.connectedTiles.push_back(&tiles[index + 1]); // Connect to the right tile
            }
            if (row > 0 && !tiles[index - cols].isWall) 
            {
                tile.connectedTiles.push_back(&tiles[index - cols]); // Connect to the tile above
            }
            if (row < rows - 1 && !tiles[index + cols].isWall) 
            {
                tile.connectedTiles.push_back(&tiles[index + cols]); // Connect to the tile below
            }
        }
    }

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLUE);

        updateCharacterMovement();
        drawGrid(tiles);
        drawCharacter();

        EndDrawing();
    }

    CloseWindow();
    UnloadTexture(character.texture);

    return 0;
}