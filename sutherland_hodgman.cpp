#include <graphics.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <conio.h>

struct Point {
    double x, y;
};

double xMin, yMin, xMax, yMax;

// Check if point is inside the given boundary (0=LEFT, 1=RIGHT, 2=BOTTOM, 3=TOP)
bool isInside(Point p, int boundary) {
    switch (boundary) {
        case 0: return p.x >= xMin;
        case 1: return p.x <= xMax;
        case 2: return p.y >= yMin;
        case 3: return p.y <= yMax;
    }
    return false;
}

// Compute intersection of edge (p1->p2) with the given boundary using m and invM
Point computeIntersection(Point p1, Point p2, int boundary) {
    Point inter;
    double m    = (p2.x != p1.x) ? (p2.y - p1.y) / (p2.x - p1.x) : 0;
    double invM = (p2.y != p1.y) ? (p2.x - p1.x) / (p2.y - p1.y) : 0;

    switch (boundary) {
        case 0: inter.x = xMin; inter.y = p1.y + m    * (xMin - p1.x); break; // LEFT
        case 1: inter.x = xMax; inter.y = p1.y + m    * (xMax - p1.x); break; // RIGHT
        case 2: inter.y = yMin; inter.x = p1.x + invM * (yMin - p1.y); break; // BOTTOM
        case 3: inter.y = yMax; inter.x = p1.x + invM * (yMax - p1.y); break; // TOP
    }
    return inter;
}

// Clip polygon against a single boundary — applies the 4-case Sutherland-Hodgman logic
std::vector<Point> clipAgainstBoundary(std::vector<Point>& poly, int boundary) {
    std::vector<Point> output;
    int n = poly.size();

    for (int i = 0; i < n; i++) {
        Point current  = poly[i];
        Point previous = poly[(i + n - 1) % n];

        bool currentInside  = isInside(current,  boundary);
        bool previousInside = isInside(previous, boundary);

        if (currentInside) {
            if (!previousInside)
                output.push_back(computeIntersection(previous, current, boundary)); // Outside -> Inside: add intersection
            output.push_back(current); // Inside -> Inside: add current
        } else if (previousInside) {
            output.push_back(computeIntersection(previous, current, boundary));     // Inside -> Outside: add intersection only
        }
        // Outside -> Outside: add nothing
    }

    return output;
}

// Apply clipping against all 4 boundaries sequentially
std::vector<Point> sutherlandHodgman(std::vector<Point>& poly) {
    std::vector<Point> output = poly;
    for (int boundary = 0; boundary < 4; boundary++) {
        if (output.empty()) break;
        output = clipAgainstBoundary(output, boundary);
    }
    return output;
}

void drawPolygon(std::vector<Point>& poly, int color) {
    if (poly.empty()) return;
    setcolor(color);
    int n = poly.size();
    for (int i = 0; i < n; i++) {
        Point p1 = poly[i];
        Point p2 = poly[(i + 1) % n];
        line((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y);
    }
}

int main() {
    std::cout << "==== Sutherland-Hodgman Polygon Clipping ====\n";
    std::cout << "Enter clipping window (xMin yMin xMax yMax): ";
    std::cin >> xMin >> yMin >> xMax >> yMax;

    int choice;
    std::cout << "\nSelect polygon:\n";
    std::cout << "1. Triangle\n";
    std::cout << "2. Rectangle\n";
    std::cout << "3. Pentagon\n";
    std::cout << "4. Custom Polygon\n";
    std::cout << "Enter choice (1-4): ";
    std::cin >> choice;

    std::vector<Point> polygon;

    switch (choice) {
        case 1: polygon = {{100, 50}, {300, 250}, {50, 250}}; break;
        case 2: polygon = {{50, 50}, {300, 50}, {300, 250}, {50, 250}}; break;
        case 3: polygon = {{175, 40}, {300, 130}, {255, 260}, {95, 260}, {50, 130}}; break;
        case 4: {
            int n;
            std::cout << "Enter number of vertices: ";
            std::cin >> n;
            for (int i = 0; i < n; i++) {
                Point p;
                std::cout << "Enter vertex " << (i + 1) << " (x y): ";
                std::cin >> p.x >> p.y;
                polygon.push_back(p);
            }
            break;
        }
        default:
            std::cout << "Invalid choice! Exiting...\n";
            return 1;
    }

    std::vector<Point> clipped = sutherlandHodgman(polygon);

    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);

    // Draw clipping window in CYAN
    setcolor(CYAN);
    rectangle((int)xMin, (int)yMin, (int)xMax, (int)yMax);
    outtextxy((int)xMin, (int)yMax + 10, (char*)"Clipping Window");

    // Draw original polygon in RED
    drawPolygon(polygon, RED);
    outtextxy((int)polygon[0].x, (int)polygon[0].y - 15, (char*)"Original");

    delay(1000);

    // Draw clipped polygon boundary in YELLOW (no fill)
    if (!clipped.empty()) {
        drawPolygon(clipped, YELLOW);
        outtextxy(10, 10, (char*)"Clipped polygon");
    } else {
        outtextxy(10, 10, (char*)"Polygon completely outside - rejected");
    }

    outtextxy(10, 460, (char*)"Red = Original  Yellow = Clipped  Cyan = Window");

    getch();
    closegraph();
    return 0;
}