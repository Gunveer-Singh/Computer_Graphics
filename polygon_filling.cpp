#include <graphics.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <conio.h>

struct Point {
    int x, y;
};

std::vector<int> getScanlineIntersections(std::vector<Point>& poly, int y) {
    std::vector<int> intersections;
    int n = poly.size();

    for (int i = 0; i < n; i++) {
        Point p1 = poly[i];
        Point p2 = poly[(i + 1) % n];

        if (p1.y == p2.y) continue;

        if (y >= std::min(p1.y, p2.y) && y < std::max(p1.y, p2.y)) {
            int x = p1.x + (y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
            intersections.push_back(x);
        }
    }

    std::sort(intersections.begin(), intersections.end());
    return intersections;
}

void drawPolygon(std::vector<Point>& poly, int color) {
    setcolor(color);
    int n = poly.size();
    for (int i = 0; i < n; i++) {
        Point p1 = poly[i];
        Point p2 = poly[(i + 1) % n];
        line(p1.x, p1.y, p2.x, p2.y);
    }
}

void scanLineFill(std::vector<Point>& poly, int fillColor, int borderColor) {
    int yMin = poly[0].y, yMax = poly[0].y;
    for (auto& p : poly) {
        yMin = std::min(yMin, p.y);
        yMax = std::max(yMax, p.y);
    }

    // Scan each horizontal line with a small delay so fill is visible
    for (int y = yMin; y <= yMax; y++) {
        std::vector<int> intersections = getScanlineIntersections(poly, y);

        setcolor(fillColor);
        for (int i = 0; i + 1 < (int)intersections.size(); i += 2) {
            line(intersections[i], y, intersections[i + 1], y);
        }

        // Redraw border after every scanline so it doesn't get overwritten
        drawPolygon(poly, borderColor);

        delay(10); // 10ms per line — adjust for faster/slower animation
    }
}

int main() {
    int choice;
    std::cout << "==== Scan Line Polygon Fill ====\n";
    std::cout << "1. Triangle\n";
    std::cout << "2. Rectangle\n";
    std::cout << "3. Pentagon\n";
    std::cout << "4. Custom Polygon (enter your own vertices)\n";
    std::cout << "Enter your choice (1-4): ";
    std::cin >> choice;

    std::vector<Point> polygon;

    switch (choice) {
        case 1:
            polygon = {{150, 50}, {250, 200}, {50, 200}};
            break;
        case 2:
            polygon = {{50, 50}, {250, 50}, {250, 200}, {50, 200}};
            break;
        case 3:
            polygon = {{150, 40}, {260, 120}, {220, 230}, {80, 230}, {40, 120}};
            break;
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

    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);

    // Draw outline first so shape is visible before fill starts
    drawPolygon(polygon, CYAN);

    // Small pause so user sees the outline before animation begins
    delay(500);

    // Animate the fill line by line
    scanLineFill(polygon, LIGHTRED, CYAN);

    getch();
    closegraph();
    return 0;
}