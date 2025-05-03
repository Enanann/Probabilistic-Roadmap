#include <chrono> // for execution time
using namespace std::chrono;

#include "common.h"
#include "node.h"
#include "map.h"
#include "graph.h"
#include "astar.h"
#include "performance.h"

#define MOUSE_SCALE_MARK_SIZE   12

enum inputStatus {
    nodes_count,
    neighbor_count,
    done
};

int main(void) {
    // Flags
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    
    // Initialize window
    const int w = 1980; // min
    const int h = 1080; // min
    InitWindow(w, h, "balls");
    
    MaximizeWindow();
    // SetWindowState(FLAG_FULLSCREEN_MODE);

    // Set up map size
    Rectangle scr = { 50, 50, 1000, 900 };

    Vector2 mousePosition = { 0 };

    bool mouseScaleReady = false;
    bool mouseScaleMode = false;
    
    // Map
    // m1
    Rectangle box1 = {600, 400, 100, 300};
    Rectangle box2 = {200, 100, 300, 200};
    Rectangle box3 = {50, 750, 600, 100};
    std::vector<Rectangle> obstaclesRec {box1, box2, box3};

    // m2
    Rectangle box4 = {500, 350, 90, 110}; 
    Rectangle box5 = {600, 450, 80, 90};
    Rectangle box6 = {800, 550, 70, 100};
    Rectangle box7 = {800, 650, 60, 110};
    Rectangle box8 = {350, 400, 50, 75};
    Rectangle box9 = {450, 500, 100, 80};
    Rectangle box10 = {600, 250, 55, 85};
    Rectangle box11 = {650, 300, 40, 60};
    Rectangle box12 = {700, 200, 120, 95};
    Rectangle box13 = {550, 150, 100, 120};
    Rectangle box14 = {300, 600, 75, 55};
    Rectangle box15 = {400, 550, 60, 70};
    Rectangle box16 = {250, 450, 80, 100};
    Rectangle box17 = {350, 550, 90, 80};
    Rectangle box18 = {200, 300, 55, 65};
    Rectangle box19 = {750, 400, 70, 100};
    Rectangle box20 = {850, 500, 60, 80};
    Rectangle box21 = {100, 400, 90, 100};
    Rectangle box22 = {150, 550, 70, 60};
    Rectangle box23 = {350, 670, 50, 300};
    std::vector<Rectangle> obstaclesRec2 {box4, box5, box6, box7, box8, box9, box10, box11, box12, 
        box13, box14, box15, box16, box17, box18, box19, box20, box21, box22, box23};

    // m3
    Rectangle box24 = {330, 50, 50, 600};
    Rectangle box25 = {330, 720, 50, 430};
    Rectangle box26 = {660, 50, 50, 260};
    Rectangle box27 = {660, 380, 50, 600};
    std::vector<Rectangle> obstaclesRec3 {box24, box25, box26, box27};

    Map map1(obstaclesRec2);

    // Input variable
    inputStatus IStatus = nodes_count;
    std::string nCount{};
    int nodesCount{};
    std::string kCount{};
    int k{};

    // Set up start, goal point
    Vector2 start = {(float)-1, (float) -1};
    Vector2 end = {(float)-1, (float) -1};
    Node* root = new Node(start);
    Node* goal = new Node(end);

    // Create points for graph
    std::vector<Node*> points;
    points.push_back(root);
    points.push_back(goal);
    
    bool shouldDraw = true;
    float Alpha = 1.0f;

    // A*
    AStarData asd;
    std::vector<Node*> path;
    
    // Performance stat
    std::vector<TimeData> td_Ve;
    int td_maxsize = 5;
    TimeData td_temp;
    Color td_Col = BLACK;

    // Performance Table
    int cols = 5;
    int rows = 6;  // header + 5 data rows
    int cellWidth = 120;
    int cellHeight = 50;
    int startX = GetScreenWidth() - (cols * cellWidth) - 20;  // Right align with 20px margin
    int startY = 150;

    const char* headers[5] = { "Sample", "NodesGen", "KNN", "A*", "PRM" };

    // Initialize FPS
    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        // Set map size
        mousePosition = GetMousePosition();

        if (CheckCollisionPointRec(mousePosition, Rectangle{ scr.x + scr.width - MOUSE_SCALE_MARK_SIZE, scr.y + scr.height - MOUSE_SCALE_MARK_SIZE, MOUSE_SCALE_MARK_SIZE, MOUSE_SCALE_MARK_SIZE }))
        {
            mouseScaleReady = true;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) mouseScaleMode = true;
        }
        else mouseScaleReady = false;

        if (mouseScaleMode)
        {
            mouseScaleReady = true;

            scr.width = (mousePosition.x - scr.x);
            scr.height = (mousePosition.y - scr.y);

            // Check minimum rec size
            if (scr.width < MOUSE_SCALE_MARK_SIZE) scr.width = MOUSE_SCALE_MARK_SIZE;
            if (scr.height < MOUSE_SCALE_MARK_SIZE) scr.height = MOUSE_SCALE_MARK_SIZE;
            
            // Check maximum rec size
            if (scr.width > (GetScreenWidth() - scr.x - 700)) scr.width = GetScreenWidth() - scr.x - 700;
            if (scr.height > (GetScreenHeight() - scr.y)) scr.height = GetScreenHeight() - scr.y;

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) mouseScaleMode = false;
        }

        // Getting Input
        if (IStatus != done) {
            for (int key = KEY_ZERO; key <= KEY_NINE; key++) {
                if (IsKeyPressed(key)) {
                    if (IStatus == nodes_count) {
                        nCount += '0' + (key - KEY_ZERO);
                    } else {
                        kCount += '0' + (key - KEY_ZERO);
                    }
                }

            }
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (IStatus == nodes_count) {
                nCount.pop_back();
            } else if (IStatus == neighbor_count) {
                kCount.pop_back();
            }
        }

        if (IsKeyPressed(KEY_ENTER)) {
            if (IStatus == nodes_count) {
                if (nCount.empty()) {
                    std::cout << "Nodes count is empty!\n";
                } else {
                    IStatus = neighbor_count;
                }
            } else if (IStatus == neighbor_count) {
                if (kCount.empty()) {
                    std::cout << "Neighbors count is empty!\n";
                } else {
                    IStatus = done;
                
                    nodesCount = std::stoi(nCount);
                    k = std::stoi(kCount);

                    td_temp.Nodes = nodesCount;
                    td_temp.K = k;
                    
                    auto t_Process = high_resolution_clock::now();
                    auto t_NodeGeneration = high_resolution_clock::now();
                    
                    for (int i = 0; i < nodesCount; i++) {
                        createSamplePoint(points, map1, scr);
                    } 
                    auto t_NodeGenerationStop = high_resolution_clock::now();
                    std::cout << "Time for nodes generation: " << (duration<double, std::milli>(t_NodeGenerationStop - t_NodeGeneration).count()) << "ms\n";
                    td_temp.NG = duration<double, std::milli>(t_NodeGenerationStop - t_NodeGeneration).count();
                    
                    auto t_Neighbor = high_resolution_clock::now();
                    kNearestNeighbor(points, k, map1);
                    auto t_NeighborStop = high_resolution_clock::now();
                    std::cout << "Time for KNN: " << (duration<double, std::milli>(t_NeighborStop - t_Neighbor).count()) << "ms\n";
                    td_temp.KNN = duration<double, std::milli>(t_NeighborStop - t_Neighbor).count();

                    auto t_Astar = high_resolution_clock::now();
                    asd.initialize(points, root, goal);
                    aStar(asd, root, goal, points);
                    if (asd.cameFrom.find(goal) != asd.cameFrom.end()) {
                        path = reconstructPath(root, goal, asd.cameFrom);
                    }
                    auto t_AstarStop = high_resolution_clock::now();
                    std::cout << "Time for A*: " << (duration<double, std::milli>(t_AstarStop - t_Astar).count()) << "ms\n";
                    td_temp.AS = duration<double, std::milli>(t_AstarStop - t_Astar).count();

                    auto t_ProcessStop = high_resolution_clock::now();
                    std::cout << "Time for PRM: " << (duration<double, std::milli>(t_ProcessStop - t_Process).count()) << "ms\n";
                    td_temp.PRM = duration<double, std::milli>(t_ProcessStop - t_Process).count();

                    td_Ve.insert(td_Ve.begin(), td_temp);
                    if (td_Ve.size() > td_maxsize) td_Ve.pop_back();
                }
            }
        }

        // Start / Goal input
        if (IsKeyPressed(KEY_Z)) {
            if (start.x == -1 && start.y == -1) {
                start = GetMousePosition();
            } else {
                start = GetMousePosition();
            }
            
            if (!checkCollisionPointMap(start, map1)) {
                root->pos = start;
            }
        }

        if (IsKeyPressed(KEY_X)) {
            if (end.x == -1 && end.y == -1) {
                end = GetMousePosition();
            } else {
                end = GetMousePosition();
            }

            if (!checkCollisionPointMap(end, map1)) {
                goal->pos = end;
            }
        }

        // Reset
        if (IsKeyPressed(KEY_R) || mouseScaleMode) {
            deletePoints(points, root, goal);

            path.clear();
            IStatus = nodes_count;
            shouldDraw = true;
            nCount.clear();
            kCount.clear();
        }

        // Enable/Disable path drawing
        if (IsKeyPressed(KEY_UP)) {
            if (shouldDraw) {
                shouldDraw = false;
            } else {
                shouldDraw = true;
            }
        }
        
        BeginDrawing();
            
            ClearBackground(RAYWHITE);
            
            DrawRectangle( GetScreenWidth() - 270, 50, 250, 70, Fade(SKYBLUE, 0.5f));
            DrawRectangleLines( GetScreenWidth() - 270, 50, 250, 70, BLUE);

            DrawText("Z to spawn Start", GetScreenWidth() - 260, 60, 20, BLACK);
            DrawText("X to spawn Goal", GetScreenWidth() - 260, 90, 20, BLACK);


            for (int row = 0; row < rows; row++) {
                for (int col = 0; col < cols; col++) {
                    int x = startX + col * cellWidth;
                    int y = startY + row * cellHeight;
            
                    DrawRectangle( x, y, cellWidth, cellHeight, Fade(Color{129, 231, 175, 1}, 0.5f));
                    DrawRectangleLines(x, y, cellWidth, cellHeight, BLUE);

                    // Header row
                    if (row == 0) {
                        DrawText(headers[col], x + 5, y + 8, 20, BLACK);
                    } else {
                        if (row - 1 < td_Ve.size()) {
                            const TimeData& data = td_Ve[row - 1];
                            std::string content;
                            
                            switch (col) {
                                case 0: content = TextFormat("%d-%d", data.Nodes, data.K); break;
                                case 1: content = TextFormat("%.4fms", data.NG); break;
                                case 2: content = TextFormat("%.4fms", data.KNN); break;
                                case 3: content = TextFormat("%.4fms", data.AS); break;
                                case 4: content = TextFormat("%.4fms", data.PRM); break;
                            }
            
                            DrawText(content.c_str(), x + 5, y + 8, 20, BLACK);
                        }
                    }
                }
            }

            DrawRectangleLinesEx(scr, 2, Fade(GREEN, 0.5f));

            if (mouseScaleReady) {
                DrawRectangleLinesEx(scr, 1, RED);
                DrawTriangle(Vector2{ scr.x + scr.width - MOUSE_SCALE_MARK_SIZE, scr.y + scr.height },
                             Vector2{ scr.x + scr.width, scr.y + scr.height },
                             Vector2{ scr.x + scr.width, scr.y + scr.height - MOUSE_SCALE_MARK_SIZE }, RED);
            }

            BeginScissorMode((int)scr.x, (int)scr.y, (int)scr.width, (int)scr.height);

            if (start.x != -1) DrawCircle(root->pos.x, root->pos.y, 5, ORANGE);
            if (end.x != -1) DrawCircle(goal->pos.x, goal->pos.y, 5, RED);

            // draw map
            for (Rectangle box : map1.obstaclesRec) {
                DrawRectangle(box.x, box.y, box.width, box.height, GRAY);
            }

            EndScissorMode();
            

            if (IStatus == done) {
                Alpha = (shouldDraw) ? 0.05f  : 1.0f;

                // Reset
                DrawText("Press R to reset! Press up arrow to enable/disable path!", 5, 0, 20, BLACK);

                BeginScissorMode((int)scr.x, (int)scr.y, (int)scr.width, (int)scr.height);
    
                // Draw graph & path found
                drawGraph(points, Alpha);
                if (shouldDraw && !path.empty()) {
                    drawPath(path);
                }
    
                // Draw start, goal point
                DrawCircle(root->pos.x, root->pos.y, 5, ORANGE);
                DrawCircle(goal->pos.x, goal->pos.y, 5, RED);

                EndScissorMode();
            } else {
                if (IStatus == nodes_count) {
                    DrawText(TextFormat("Enter how many nodes to generate: %s", nCount.c_str()), 10, 0, 30, BLACK);
                } else {
                    DrawText(TextFormat("Enter how many neighbors to connect: %s", kCount.c_str()), 10, 0, 30, BLACK);
                }
            }

        EndDrawing();
    }

    // Clean up memory
    // Maybe refactor to use smart pointer
    deletePoints(points);

    CloseWindow();

    return 0;
}
