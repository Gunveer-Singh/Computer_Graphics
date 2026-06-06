#include <graphics.h>
#include <iostream>
#include <cmath>
#include <cstring>
#include <conio.h>

// Function to multiply a 2D row vector [x, y, 1] by a 3x3 transformation matrix
void multiplyMatrix(double vertex[3], double matrix[3][3], double result[3]) {
    for (int i = 0; i < 3; i++) {
        result[i] = 0;
        for (int j = 0; j < 3; j++) {
            result[i] += vertex[j] * matrix[j][i];
        }
    }
}

// Function to draw a triangle given its 3 vertices (ignoring Z, which is always 1)
void drawTriangle(double points[3][3], int color) {
    setcolor(color);
    line(points[0][0], points[0][1], points[1][0], points[1][1]);
    line(points[1][0], points[1][1], points[2][0], points[2][1]);
    line(points[2][0], points[2][1], points[0][0], points[0][1]);
}

int main() {
    // Original triangle vertices in Row Vector format: [x, y, 1]
    double triangle[3][3] = {
        {100, 150, 1},
        {200, 150, 1},
        {150, 50,  1}
    };

    double transformedTriangle[3][3];

    // Initialize as a 3x3 Identity Matrix
    double transformMatrix[3][3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };

    int choice;
    std::cout << "==== 2D Transformations Menu (Row Vector Form) ====\n";
    std::cout << "1. Translation\n";
    std::cout << "2. Scaling\n";
    std::cout << "3. Rotation\n";
    std::cout << "4. Shearing\n";
    std::cout << "Enter your choice (1-4): ";
    std::cin >> choice;

    switch (choice) {
        case 1: {
            double tx, ty;
            std::cout << "Enter translation factors (tx ty): ";
            std::cin >> tx >> ty;
            transformMatrix[2][0] = tx;
            transformMatrix[2][1] = ty;
            break;
        }
        case 2: {
            double sx, sy;
            std::cout << "Enter scaling factors (sx sy): ";
            std::cin >> sx >> sy;
            transformMatrix[0][0] = sx;
            transformMatrix[1][1] = sy;
            break;
        }
        case 3: {
            double angle;
            std::cout << "Enter rotation angle in degrees: ";
            std::cin >> angle;

            double rad = angle * M_PI / 180.0;

            // Calculate centroid of the triangle to rotate around it
            double cx = (triangle[0][0] + triangle[1][0] + triangle[2][0]) / 3.0;
            double cy = (triangle[0][1] + triangle[1][1] + triangle[2][1]) / 3.0;

            // Step 1: Translate centroid to origin
            double T1[3][3] = {
                {1,   0,   0},
                {0,   1,   0},
                {-cx, -cy, 1}
            };

            // Step 2: Rotate around origin (which is now the centroid)
            double R[3][3] = {
                { cos(rad), sin(rad), 0},
                {-sin(rad), cos(rad), 0},
                {0,         0,        1}
            };

            // Step 3: Translate centroid back to its original position
            double T2[3][3] = {
                {1,  0,  0},
                {0,  1,  0},
                {cx, cy, 1}
            };

            // Combine: T1 * R -> temp
            double temp[3][3] = {};
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    for (int k = 0; k < 3; k++)
                        temp[i][j] += T1[i][k] * R[k][j];

            // Combine: temp * T2 -> transformMatrix (zero it out first to avoid identity bleed-in)
            memset(transformMatrix, 0, sizeof(transformMatrix));
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    for (int k = 0; k < 3; k++)
                        transformMatrix[i][j] += temp[i][k] * T2[k][j];

            break;
        }
        case 4: {
            int shearChoice;
            std::cout << "1. X-Axis Shearing\n";
            std::cout << "2. Y-Axis Shearing\n";
            std::cout << "Select shearing type (1-2): ";
            std::cin >> shearChoice;

            if (shearChoice == 1) {
                double shx;
                std::cout << "Enter X-shear factor (shx): ";
                std::cin >> shx;
                transformMatrix[1][0] = shx;
            } else if (shearChoice == 2) {
                double shy;
                std::cout << "Enter Y-shear factor (shy): ";
                std::cin >> shy;
                transformMatrix[0][1] = shy;
            } else {
                std::cout << "Invalid shearing choice!\n";
                return 1;
            }
            break;
        }
        default:
            std::cout << "Invalid choice! Exiting...\n";
            return 1;
    }

    for (int i = 0; i < 3; i++) {
        multiplyMatrix(triangle[i], transformMatrix, transformedTriangle[i]);
    }

    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);

    // Draw the original triangle in LIGHTGREEN
    drawTriangle(triangle, LIGHTGREEN);
    outtextxy(100, 160, (char*)"Original");

    // Draw the transformed triangle in LIGHTRED
    drawTriangle(transformedTriangle, LIGHTRED);
    outtextxy(transformedTriangle[0][0], transformedTriangle[0][1] + 10, (char*)"Transformed");

    getch();
    closegraph();
    return 0;
}