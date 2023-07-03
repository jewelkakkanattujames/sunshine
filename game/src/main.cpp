#include "raylib.h"
#include <vector>
#include <queue>
#include <iostream>
#include <sstream>
#include <raymath.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
const int TILE_SIZE = 75;
struct Tile
{
    Vector2 position;
    Color color;
    bool wallPresent;
    std::vector<Tile*> connectedTiles;
    float tilesToarrive;
    Tile* lastNode;
    Rectangle bounds;
    bool visited;
    bool operator==(const Tile& other) const
    {
        if (this == &other)
            return true;

        return position.x == other.position.x && position.y == other.position.y;
    }

};

class Character
{
public:
    Vector2 position;
    Rectangle bounds;
    float speed;
    Texture2D sprite;

    Character()
    {
        
        position = { 0, 0 };
        bounds = { 0, 0, 0, 0 };
        speed = 0.0f;
        sprite = { 0 };
    }

    Character(Vector2 pos, float spd, Texture2D tex)
        : position(pos), speed(spd), sprite(tex)
    {
        bounds = { pos.x, pos.y, static_cast<float>(tex.width), static_cast<float>(tex.height) };
    }
};


std::vector<Tile> tiles;
Character character;
Tile* startNode;
Tile* goalNode;
Tile* currentNode;
std::vector<Vector2> playerPath; 

std::string intToString(int value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

void generateTiles()
{
    const int rows = 9;
    const int cols = 9;

    int totalTiles = rows * cols;

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            Vector2 position = { 100 + col * TILE_SIZE, 100 + row * TILE_SIZE };
            Color color = { static_cast<unsigned char>(GetRandomValue(50, 205)),
                            static_cast<unsigned char>(GetRandomValue(50, 205)),
                            static_cast<unsigned char>(GetRandomValue(50, 205)),
                            255 };
            tiles.push_back({ position, color, false, {}, FLT_MAX, nullptr, false });
        }
    }


    
    int i = 0;
    while (i < totalTiles)
    {
        int row = i / cols;
        int col = i % cols;

        Tile& currentTile = tiles[i];

        
        int topIndex = i - cols;
        if (topIndex >= 0)
        {
            currentTile.connectedTiles.push_back(&tiles[topIndex]);
        }

        
        int bottomIndex = i + cols;
        if (bottomIndex < totalTiles)
        {
            currentTile.connectedTiles.push_back(&tiles[bottomIndex]);
        }

       
        int leftIndex = i - 1;
        if (col > 0)
        {
            currentTile.connectedTiles.push_back(&tiles[leftIndex]);
        }

        
        int rightIndex = i + 1;
        if (col < cols - 1)
        {
            currentTile.connectedTiles.push_back(&tiles[rightIndex]);
        }

        
        ++i;
    }

}


void drawAdjacency(const std::vector<Tile>& tiles)
{
    const Color lineColor = GREEN; 
    const Color wallColor = RED;
    const Color startColor = GREEN;
    const Color lastTileColor = RED;
    const Color currentColor = BLUE;
    const Color playerColor = BLUE; 

    for (const Tile& tile : tiles)
    {
        if (tile.wallPresent)
        {
            DrawRectangle(static_cast<int>(tile.position.x), static_cast<int>(tile.position.y), TILE_SIZE, TILE_SIZE, wallColor);
        }
        else
        {
            if (tile == *startNode)
            {
                DrawRectangle(static_cast<int>(tile.position.x), static_cast<int>(tile.position.y), TILE_SIZE, TILE_SIZE, startColor);
            }
            else if (&tile == currentNode)
            {
                DrawRectangle(static_cast<int>(tile.position.x), static_cast<int>(tile.position.y), TILE_SIZE, TILE_SIZE, currentColor);
            }
            else if (CheckCollisionRecs(character.bounds, { tile.position.x, tile.position.y, TILE_SIZE, TILE_SIZE }))
            {
                DrawRectangle(static_cast<int>(tile.position.x), static_cast<int>(tile.position.y), TILE_SIZE, TILE_SIZE, playerColor);
            }
            else
            {
                DrawRectangle(static_cast<int>(tile.position.x), static_cast<int>(tile.position.y), TILE_SIZE, TILE_SIZE, tile.color);
            }
        }

        DrawLine(static_cast<int>(tile.position.x), static_cast<int>(tile.position.y), static_cast<int>(tile.position.x + TILE_SIZE), static_cast<int>(tile.position.y), BLACK);
        DrawLine(static_cast<int>(tile.position.x), static_cast<int>(tile.position.y), static_cast<int>(tile.position.x), static_cast<int>(tile.position.y + TILE_SIZE), BLACK);

        if (tile.lastNode != nullptr)
        {
            Vector2 currentCenter = { tile.position.x + TILE_SIZE / 2, tile.position.y + TILE_SIZE / 2 };
            Vector2 previousCenter = { tile.lastNode->position.x + TILE_SIZE / 2, tile.lastNode->position.y + TILE_SIZE / 2 };
            DrawLineEx(previousCenter, currentCenter, 2, lineColor); 
        }
    }
    
    for (const Tile& tile : tiles)
    {
        Vector2 currentCenter = { tile.position.x + TILE_SIZE / 2, tile.position.y + TILE_SIZE / 2 };
        for (const Tile* connectedTile : tile.connectedTiles)
        {
            Vector2 connectedCenter = { connectedTile->position.x + TILE_SIZE / 2, connectedTile->position.y + TILE_SIZE / 2 };
            DrawLineEx(currentCenter, connectedCenter, 1, lineColor); 
        }
    }

    if (playerPath.size() > 1)
    {
        for (size_t i = 0; i < playerPath.size() - 1; ++i)
        {
            Vector2 currentPos = playerPath[i];
            Vector2 nextPos = playerPath[i + 1];
            Vector2 lineStart = { currentPos.x + TILE_SIZE / 2, currentPos.y + TILE_SIZE / 2 };
            Vector2 lineEnd = { nextPos.x + TILE_SIZE / 2, nextPos.y + TILE_SIZE / 2 };
            DrawLineEx(lineStart, lineEnd, 2, BLUE);
        }
    }
}

void initCharacter()
{
    Vector2 position = tiles[0].position;
    Rectangle bounds = { position.x + 10, position.y + 10, TILE_SIZE - 20, TILE_SIZE - 20 };
    float speed = 2.0f;
    Texture2D sprite = LoadTexture("C:/Ai assignments/AI Lab5/game/assets/textures/doremon.png");

    character = Character(position, speed, sprite);
    character.bounds = bounds;
}

void updateCharacterMovement()
{
    Vector2 newPosition = character.position;

    if (IsKeyDown(KEY_W) && newPosition.y > 100)
        newPosition.y -= character.speed;

    if (IsKeyDown(KEY_S) && newPosition.y < (100 + TILE_SIZE * 7))
        newPosition.y += character.speed;

    if (IsKeyDown(KEY_A) && newPosition.x > 100)
        newPosition.x -= character.speed;

    if (IsKeyDown(KEY_D) && newPosition.x < (100 + TILE_SIZE * 7))
        newPosition.x += character.speed;

    
    bool isColliding = false;
    Rectangle newBounds = { newPosition.x + 10, newPosition.y + 10, TILE_SIZE - 20, TILE_SIZE - 20 };

    for (const Tile& tile : tiles)
    {
        if (tile.wallPresent && CheckCollisionRecs(newBounds, tile.bounds))
        {
            isColliding = true;
            break;
        }
    }

    if (!isColliding)
    {
        character.position = newPosition;
        character.bounds = newBounds;
    }
}



void runDijkstra()
{
    std::priority_queue<std::pair<float, Tile*>, std::vector<std::pair<float, Tile*>>, std::greater<std::pair<float, Tile*>>> pq;
    pq.push({ 0, startNode });

    while (!pq.empty() && currentNode != goalNode) 
    {
        Tile* current = pq.top().second;
        pq.pop();

        current->visited = true;

        for (Tile* neighbor : current->connectedTiles)
        {
            float newCost = current->tilesToarrive + 1;
            if (newCost < neighbor->tilesToarrive) 
            {
                neighbor->tilesToarrive = newCost;
                neighbor->lastNode = current;
                pq.push({ newCost, neighbor });
            }
        }

        currentNode = current;
    }
}

void visualizeDijkstra() 
{
    const Color pathColor = { 0, 0, 255 };       // Blue
    const Color visitedColor = { 255, 255, 255 };   // White
    const Color goalColor = RED;       // Red
    const Color currentColor = BLUE;
    const Color firstTileColor = { 0, 128, 0 };  // Green
    const Color unvisitedColor = { 211, 211, 211 }; // LightGray
    const Color textColor = { 169, 169, 169 };  // DarkGray

    std::vector<Tile>& mutableTiles = const_cast<std::vector<Tile>&>(tiles);
    

    
    if (!mutableTiles.empty()) 
    {
        mutableTiles[0].color = RED;
    }


    Tile* pathNode = goalNode;

    while (pathNode != nullptr)
    {
        mutableTiles[pathNode - &tiles[0]].color = pathColor;
        pathNode = pathNode->lastNode;
    }

    for (Tile& tile : mutableTiles)
    {
        if (!tile.visited)
        {
            std::string costText = intToString(static_cast<int>(tile.tilesToarrive));
            tile.color = unvisitedColor;
            DrawText(costText.c_str(), static_cast<int>(tile.position.x + TILE_SIZE / 2 - MeasureText(costText.c_str(), 20) / 2), static_cast<int>(tile.position.y + TILE_SIZE / 2 - 10), 20, textColor);
        }
        else
        {
            tile.color = visitedColor;
        }

        if (&tile == startNode)
        {
            tile.color = firstTileColor;
        }
        if (&tile == goalNode)
        {
            tile.color = goalColor;
        }
        if (&tile == currentNode)
        {
            tile.color = currentColor;
        }
    }
}


void drawCharacter()
{
    DrawTexture(character.sprite, static_cast<int>(character.position.x), static_cast<int>(character.position.y), WHITE);
}

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Dijkstra's Algorithm");

    generateTiles();

    startNode = nullptr;
    goalNode = nullptr;
    currentNode = nullptr;

    for (Tile& tile : tiles) 
    {
        if (tile.position.x == 100 && tile.position.y == 100) 
        {
            startNode = &tile;
        }
        else if (tile.position.x == 100 + TILE_SIZE * 6 && tile.position.y == 100 + TILE_SIZE * 6) 
        {
            goalNode = &tile;
        }
    }

    if (startNode != nullptr) 
    {
        currentNode = startNode;
    }
    else 
    {
        
    }

    if (goalNode == nullptr) 
    {
        
    }


    initCharacter();

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        updateCharacterMovement();

        for (Tile& tile : tiles)
        {
            if (CheckCollisionRecs(character.bounds, { tile.position.x, tile.position.y, TILE_SIZE, TILE_SIZE }))
            {
                if (tile.wallPresent)
                {
                    character.position = currentNode->position;
                    character.bounds.x = currentNode->position.x + 10;
                    character.bounds.y = currentNode->position.y + 10;
                }
                else
                {
                    currentNode = &tile;
                    playerPath.push_back(currentNode->position); 
                }
            }
        }

        std::for_each(tiles.begin(), tiles.end(), [](Tile& tile) {
    tile.color = WHITE;
});


        for (Tile& tile : tiles)
        {
            tile.visited = false;
            tile.tilesToarrive = FLT_MAX;
            tile.lastNode = nullptr;
        }

        runDijkstra();
        visualizeDijkstra();
        drawAdjacency(tiles);
        drawCharacter(); 

        EndDrawing();
    }

    CloseWindow();
    UnloadTexture(character.sprite); 

    return 0;
}