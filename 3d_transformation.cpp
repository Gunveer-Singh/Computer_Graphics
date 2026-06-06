#include <graphics.h>
#include <iostream>
#include <cmath>
#include <cstring>
#include <conio.h>

void multiplyMatrix3D(double vertex[4], double matrix[4][4], double result[4]) {
    for (int i = 0; i < 4; i++) {
        result[i] = 0;
        for (int j = 0; j < 4; j++) {
            result[i] += vertex[j] * matrix[j][i];
        }
    }
}

void drawTetrahedron(double points[4][4], int color) {
    setcolor(color);
    line(points[0][0], points[0][1], points[1][0], points[1][1]);
    line(points[1][0], points[1][1], points[2][0], points[2][1]);
    line(points[2][0], points[2][1], points[0][0], points[0][1]);
    line(points[0][0], points[0][1], points[3][0], points[3][1]);
    line(points[1][0], points[1][1], points[3][0], points[3][1]);
    line(points[2][0], points[2][1], points[3][0], points[3][1]);
}

int main() {
    double tetrahedron[4][4] = {
        {100, 200, 50,  1},
        {200, 200, 50,  1},
        {150, 250, 150, 1},
        {150, 100, 80,  1}
    };

    double transformedTetrahedron[4][4];

    double transformMatrix[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };

    int choice;
    std::cout << "==== 3D Transformations Menu (Row Vector Form) ====\n";
    std::cout << "1. Translation\n";
    std::cout << "2. Scaling\n";
    std::cout << "3. Rotation (around Z-Axis)\n";
    std::cout << "4. Shearing (X-Axis Shear relative to Z)\n";
    std::cout << "Enter your choice (1-4): ";
    std::cin >> choice;

    switch (choice) {
        case 1: {
            double tx, ty, tz;
            std::cout << "Enter 3D translation factors (tx ty tz): ";
            std::cin >> tx >> ty >> tz;
            transformMatrix[3][0] = tx;
            transformMatrix[3][1] = ty;
            transformMatrix[3][2] = tz;
            break;
        }
        case 2: {
            double sx, sy, sz;
            std::cout << "Enter 3D scaling factors (sx sy sz): ";
            std::cin >> sx >> sy >> sz;
            transformMatrix[0][0] = sx;
            transformMatrix[1][1] = sy;
            transformMatrix[2][2] = sz;
            break;
        }
        case 3: {
            double angle;
            std::cout << "Enter Z-axis rotation angle in degrees: ";
            std::cin >> angle;

            double rad = angle * M_PI / 180.0;

            // Calculate centroid (x and y only; z doesn't affect 2D projection)
            double cx = (tetrahedron[0][0] + tetrahedron[1][0] + tetrahedron[2][0] + tetrahedron[3][0]) / 4.0;
            double cy = (tetrahedron[0][1] + tetrahedron[1][1] + tetrahedron[2][1] + tetrahedron[3][1]) / 4.0;

            // Step 1: Translate centroid to origin
            double T1[4][4] = {
                {1,   0,   0, 0},
                {0,   1,   0, 0},
                {0,   0,   1, 0},
                {-cx, -cy, 0, 1}
            };

            // Step 2: Rotate around Z-axis
            double R[4][4] = {
                { cos(rad), sin(rad), 0, 0},
                {-sin(rad), cos(rad), 0, 0},
                {0,         0,        1, 0},
                {0,         0,        0, 1}
            };

            // Step 3: Translate back
            double T2[4][4] = {
                {1,  0,  0, 0},
                {0,  1,  0, 0},
                {0,  0,  1, 0},
                {cx, cy, 0, 1}
            };

            // T1 * R -> temp
            double temp[4][4] = {};
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    for (int k = 0; k < 4; k++)
                        temp[i][j] += T1[i][k] * R[k][j];

            // temp * T2 -> transformMatrix
            memset(transformMatrix, 0, sizeof(transformMatrix));
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    for (int k = 0; k < 4; k++)
                        transformMatrix[i][j] += temp[i][k] * T2[k][j];

            break;
        }
        case 4: {
            double shx;
            std::cout << "Enter X-shearing factor relative to Z (shx): ";
            std::cin >> shx;
            transformMatrix[2][0] = shx;
            break;
        }
        default:
            std::cout << "Invalid choice! Exiting...\n";
            return 1;
    }

    for (int i = 0; i < 4; i++) {
        multiplyMatrix3D(tetrahedron[i], transformMatrix, transformedTetrahedron[i]);
    }

    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);

    
    // Draw the original tetrahedron in LIGHTGREEN
    drawTetrahedron(tetrahedron, LIGHTGREEN);
    outtextxy(100, 260, (char*)"Original 3D");

    // Draw the transformed tetrahedron in LIGHTRED
    drawTetrahedron(transformedTetrahedron, LIGHTRED);
    outtextxy(transformedTetrahedron[0][0], transformedTetrahedron[0][1] + 10, (char*)"Transformed 3D");

    getch();
    closegraph();
    return 0;
}