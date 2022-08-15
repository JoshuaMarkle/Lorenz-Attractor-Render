#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <iostream>
#include <vector> //for STL vector

using namespace std;

//The Lorenz equations
float dxdt(float x, float y) {
    return 10.0 * (y - x);
}

float dydt(float x, float y, float z) {
    return -1 * x * z + 28.0 * x - y;
}

float dzdt(float x, float y, float z) {
    return x * y - (8.0 / 3.0) * z;
}

//Fourth order Runge-Kutta discrete integration
Vector3 rk4(Vector3 p, float dt) {
    float k0, k1, k2, k3;
    Vector3 np = p;
    k0 = dt * dxdt(p.x, p.y);
    k1 = dt * dxdt(p.x + k0 / 2.0, p.y);
    k2 = dt * dxdt(p.x + k1 / 2.0, p.y);
    k3 = dt * dxdt(p.x + k2, p.y);
    np.x = p.x + k0 / 6.0 + k1 / 3.0 + k2 / 3.0 + k3 / 6.0;

    k0 = dt * dydt(p.x, p.y, p.z);
    k1 = dt * dydt(p.x, p.y + k0 / 2.0, p.z);
    k2 = dt * dydt(p.x, p.y + k1 / 2.0, p.z);
    k3 = dt * dydt(p.x, p.y + k2, p.z);
    np.y = p.y + k0 / 6.0 + k1 / 3.0 + k2 / 3.0 + k3 / 6.0;

    k0 = dt * dzdt(p.x, p.y, p.z);
    k1 = dt * dzdt(p.x, p.y, p.z + k0 / 2.0);
    k2 = dt * dzdt(p.x, p.y, p.z + k1 / 2.0);
    k3 = dt * dzdt(p.x, p.y, p.z + k2);
    np.z = p.z + k0 / 6.0 + k1 / 3.0 + k2 / 3.0 + k3 / 6.0;

    return np;
}

int main() {
    //Initialize Raylib
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 800, "- Lorenz Attractor -");
    SetWindowPosition(500, 50);
    MaximizeWindow();

    Camera camera = { 0 };
    camera.position = { 80.0, 50.0, 50.0 };
    camera.target = { 0.0,5.0,30.0 };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    SetCameraMode(camera, 2);
    SetTargetFPS(30);

    double numPoints = 0;
    float hx = 0.0, hy = 1.0, hz = 0.0;
    float ix = 0.0, iy = 1.0, iz = 0.0;
    float jx = 0.0, jy = 1.0, jz = 0.0;

    bool complete = false;

    vector<Vector3>vec_list1;
    vector<Vector3>vec_list2;
    vector<Vector3>vec_list3;

    while (!WindowShouldClose()) {
        //Update

        // Initial Drawing Animation
        if (numPoints < 4000 && complete == false) {
            numPoints += 35 * GetFrameTime();
        }
        else if (numPoints >= 4000) {
            complete = true;
        }

        vec_list1.clear();
        vec_list2.clear();
        vec_list3.clear();
        Vector3 pos1 = { hx,hy,hz };
        Vector3 pos2 = { ix,iy,iz };
        Vector3 pos3 = { jx,jy,jz };
        for (int i = 0; i < numPoints; i++) {
            vec_list1.push_back(pos1);
            pos1 = rk4(pos1, 0.01);
            vec_list2.push_back(pos2);
            pos2 = rk4(pos2, 0.01);
            vec_list3.push_back(pos3);
            pos3 = rk4(pos3, 0.01);
        }

        hx = GuiSlider({ 70,50,150,30 }, "X", "", hx, -40.0, 40.0);
        ix = hx + 0.01;
        jx = hx - 0.01;
        hy = GuiSlider({ 250,50,150,30 }, "Y", "", hy, -40.0, 40.0);
        iy = hy + 0.01;
        jy = hy - 0.01;
        hz = GuiSlider({ 430,50,150,30 }, "Z", "", hz, 0.0, 40.0);
        iz = hz + 0.01;
        jz = hz - 0.01;
        numPoints = GuiSlider({ 70,10,510,30 }, "Segments", "", numPoints, 1.0, 8000.0);

        //Draw
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(camera);
        //DrawGrid(100, 1.0f);

        int i = 0;
        while (i < numPoints - 1) {
            DrawLine3D(vec_list1[i], vec_list1[i + 1], BLUE);
            DrawLine3D(vec_list2[i], vec_list2[i + 1], GREEN);
            DrawLine3D(vec_list3[i], vec_list3[i + 1], YELLOW);
            //ColorFromHSV(i, 50, 100)
            i++;
        }

        UpdateCamera(&camera);
        EndMode3D();
        DrawFPS(700, 10);
        EndDrawing();
    }

    return 0;
}
