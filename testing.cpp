/*
 * 3D Cube Rotation — BGI graphics.h (WinBGIm / TDM-GCC)
 *
 * Rotate a filled cube about X, Y, or Z axis continuously.
 * Each face is filled with a distinct color; edges are drawn in CYAN.
 *
 * Controls (keyboard):
 *   X / x  — rotate around X axis (+/-)
 *   Y / y  — rotate around Y axis (+/-)
 *   Z / z  — rotate around Z axis (+/-)
 *   Q / q  — quit
 *
 * Compile (TDM-GCC + WinBGIm):
 *   g++ rotate3d.cpp -o rotate3d.exe -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
 */

#include <graphics.h>
#include <conio.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>

// ─── Math helpers ────────────────────────────────────────────────────────────

const double PI = 3.14159265358979323846;

struct Vec3 { double x, y, z; };

// Rotate a point about the X axis by angle (radians)
Vec3 rotateX(Vec3 p, double a) {
    return { p.x,
             p.y * cos(a) - p.z * sin(a),
             p.y * sin(a) + p.z * cos(a) };
}

// Rotate a point about the Y axis
Vec3 rotateY(Vec3 p, double a) {
    return { p.x * cos(a) + p.z * sin(a),
             p.y,
            -p.x * sin(a) + p.z * cos(a) };
}

// Rotate a point about the Z axis
Vec3 rotateZ(Vec3 p, double a) {
    return { p.x * cos(a) - p.y * sin(a),
             p.x * sin(a) + p.y * cos(a),
             p.z };
}

// Simple perspective projection → 2D screen coords
// Camera is at z = -distance looking toward +z
struct Point2D { int x, y; };

Point2D project(Vec3 p, int cx, int cy, double fov) {
    double z = p.z + fov;           // shift so object doesn't go behind camera
    if (z == 0) z = 0.001;
    double scale = fov / z;
    return { (int)(cx + p.x * scale),
             (int)(cy - p.y * scale) };   // y flipped: screen y grows downward
}

// ─── Scan-line fill for a convex polygon (2D) ────────────────────────────────

void fillPolygon(std::vector<Point2D>& pts, int fillColor, int borderColor) {
    int n = pts.size();
    if (n < 2) return;

    // Find vertical extent
    int yMin = pts[0].y, yMax = pts[0].y;
    for (auto& p : pts) {
        yMin = std::min(yMin, p.y);
        yMax = std::max(yMax, p.y);
    }

    setcolor(fillColor);
    for (int y = yMin; y <= yMax; y++) {
        std::vector<int> xs;
        for (int i = 0; i < n; i++) {
            Point2D a = pts[i];
            Point2D b = pts[(i + 1) % n];
            if (a.y == b.y) continue;
            if (y >= std::min(a.y, b.y) && y < std::max(a.y, b.y)) {
                int x = a.x + (y - a.y) * (b.x - a.x) / (b.y - a.y);
                xs.push_back(x);
            }
        }
        std::sort(xs.begin(), xs.end());
        for (int i = 0; i + 1 < (int)xs.size(); i += 2)
            line(xs[i], y, xs[i + 1], y);
    }

    // Draw border on top
    setcolor(borderColor);
    for (int i = 0; i < n; i++) {
        Point2D a = pts[i];
        Point2D b = pts[(i + 1) % n];
        line(a.x, a.y, b.x, b.y);
    }
}

// ─── Cube definition ─────────────────────────────────────────────────────────
//
//  8 vertices of a cube centered at origin, side length = 2*SIZE
//
//       6 ──── 7
//      /|     /|
//     2 ──── 3 |
//     | 4 ──|─ 5
//     |/    | /
//     0 ──── 1
//

const double CUBE_SIZE = 80.0;

Vec3 baseVerts[8] = {
    {-CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE},  // 0 front-bottom-left
    { CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE},  // 1 front-bottom-right
    {-CUBE_SIZE,  CUBE_SIZE, -CUBE_SIZE},  // 2 front-top-left
    { CUBE_SIZE,  CUBE_SIZE, -CUBE_SIZE},  // 3 front-top-right
    {-CUBE_SIZE, -CUBE_SIZE,  CUBE_SIZE},  // 4 back-bottom-left
    { CUBE_SIZE, -CUBE_SIZE,  CUBE_SIZE},  // 5 back-bottom-right
    {-CUBE_SIZE,  CUBE_SIZE,  CUBE_SIZE},  // 6 back-top-left
    { CUBE_SIZE,  CUBE_SIZE,  CUBE_SIZE},  // 7 back-top-right
};

// 6 faces — each is 4 vertex indices
int faces[6][4] = {
    {0, 1, 3, 2},   // front   (z = -SIZE)
    {5, 4, 6, 7},   // back    (z = +SIZE)
    {4, 0, 2, 6},   // left    (x = -SIZE)
    {1, 5, 7, 3},   // right   (x = +SIZE)
    {2, 3, 7, 6},   // top     (y = +SIZE)
    {0, 4, 5, 1},   // bottom  (y = -SIZE)
};

// Face fill colors (one per face)
int faceColors[6] = {
    LIGHTRED,       // front
    LIGHTGREEN,     // back
    LIGHTBLUE,      // left
    YELLOW,         // right
    LIGHTMAGENTA,   // top
    BROWN,          // bottom
};

// ─── Draw the cube given current rotation angles ──────────────────────────────

void drawCube(double ax, double ay, double az, int cx, int cy, double fov) {
    // 1. Rotate all 8 vertices
    Vec3 verts[8];
    for (int i = 0; i < 8; i++) {
        Vec3 v = baseVerts[i];
        v = rotateX(v, ax);
        v = rotateY(v, ay);
        v = rotateZ(v, az);
        verts[i] = v;
    }

    // 2. Compute average Z of each face for painter's algorithm (back-to-front)
    double faceZ[6];
    for (int f = 0; f < 6; f++) {
        double avgZ = 0;
        for (int k = 0; k < 4; k++)
            avgZ += verts[faces[f][k]].z;
        faceZ[f] = avgZ / 4.0;
    }

    // Sort face indices back-to-front (largest Z drawn first)
    int order[6] = {0, 1, 2, 3, 4, 5};
    std::sort(order, order + 6, [&](int a, int b) {
        return faceZ[a] > faceZ[b];
    });

    // 3. Project and draw each face
    for (int fi = 0; fi < 6; fi++) {
        int f = order[fi];
        std::vector<Point2D> pts;
        for (int k = 0; k < 4; k++)
            pts.push_back(project(verts[faces[f][k]], cx, cy, fov));
        fillPolygon(pts, faceColors[f], CYAN);
    }
}

// ─── Main ────────────────────────────────────────────────────────────────────

int main() {
    initwindow(800, 600, "3D Cube Rotation");

    int cx = 400;
    int cy = 300;
    double fov = 400.0;
    double step = PI / 18.0;  // 10 degrees per keypress

    double ax = 0.3, ay = 0.5, az = 0.0;

    auto redraw = [&]() {
        cleardevice();
        drawCube(ax, ay, az, cx, cy, fov);
        setcolor(WHITE);
        outtextxy(10, 10, (char*)"X/x: rot X  |  Y/y: rot Y  |  Z/z: rot Z  |  Q: quit");
        char info[128];
        sprintf(info, "ax=%.1f  ay=%.1f  az=%.1f  (deg)",
                ax * 180.0 / PI, ay * 180.0 / PI, az * 180.0 / PI);
        outtextxy(10, 25, info);
    };

    redraw();

    while (true) {
        char key = getch();
        if (key == 'q' || key == 'Q') break;

        switch (key) {
            case 'X': ax += step; break;
            case 'x': ax -= step; break;
            case 'Y': ay += step; break;
            case 'y': ay -= step; break;
            case 'Z': az += step; break;
            case 'z': az -= step; break;
            default: continue;
        }
        redraw();
    }

    closegraph();
    return 0;
}