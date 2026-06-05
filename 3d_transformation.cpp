#include <graphics.h>
#include <iostream>
#include <cmath>
#include <conio.h>

// Function to multiply a 3D row vector [x, y, z, 1] by a 4x4 transformation matrix
void multiplyMatrix3D(double vertex[4], double matrix[4][4], double result[4]) {
    for (int i = 0; i < 4; i++) {
        result[i] = 0;
        for (int j = 0; j < 4; j++) {
            result[i] += vertex[j] * matrix[j][i];
        }
    }
}

// Function to draw a 3D tetrahedron by linking its 4 vertices (ignoring Z for 2D projection)
void drawTetrahedron(double points[4][4], int color) {
    setcolor(color);
    // Base Triangle (Points 0, 1, 2)
    line(points[0][0], points[0][1], points[1][0], points[1][1]);
    line(points[1][0], points[1][1], points[2][0], points[2][1]);
    line(points[2][0], points[2][1], points[0][0], points[0][1]);

    // Top Apex (Point 3) connected to the base vertices
    line(points[0][0], points[0][1], points[3][0], points[3][1]);
    line(points[1][0], points[1][1], points[3][0], points[3][1]);
    line(points[2][0], points[2][1], points[3][0], points[3][1]);
}

int main() {
    // Original 3D Tetrahedron vertices in Row Vector format: [x, y, z, 1]
    double tetrahedron[4][4] = {
        {100, 200, 50,  1}, // Base corner 1
        {200, 200, 50,  1}, // Base corner 2
        {150, 250, 150, 1}, // Base corner 3
        {150, 100, 80,  1}  // Top Apex
    };
    
    double transformedTetrahedron[4][4];
    
    // Initialize as a 4x4 Identity Matrix
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
            
            transformMatrix[3][0] = tx; // Bottom Row, Col 0
            transformMatrix[3][1] = ty; // Bottom Row, Col 1
            transformMatrix[3][2] = tz; // Bottom Row, Col 2
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
            
            // Rotation around Z affects X and Y elements
            transformMatrix[0][0] = cos(rad);
            transformMatrix[0][1] = sin(rad);
            transformMatrix[1][0] = -sin(rad);
            transformMatrix[1][1] = cos(rad);
            break;
        }
        case 4: {
            double shx;
            std::cout << "Enter X-shearing factor relative to Z (shx): ";
            std::cin >> shx;
            
            // Modifies X based on its Z position
            transformMatrix[2][0] = shx; 
            break;
        }
        default:
            std::cout << "Invalid choice! Exiting...\n";
            return 1;
    }

    // Perform Vector x Matrix multiplication for each 3D vertex
    for (int i = 0; i < 4; i++) {
        multiplyMatrix3D(tetrahedron[i], transformMatrix, transformedTetrahedron[i]);
    }

    // Initialize graphics window
    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);

    // Draw original 3D object in WHITE
    drawTetrahedron(tetrahedron, WHITE);
    outtextxy(100, 260, (char*)"Original 3D");

    // Draw transformed 3D object in YELLOW
    drawTetrahedron(transformedTetrahedron, YELLOW);
    outtextxy(transformedTetrahedron[0][0], transformedTetrahedron[0][1] + 10, (char*)"Transformed 3D");

    getch();
    closegraph();
    return 0;
}