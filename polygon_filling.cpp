#include <graphics.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <conio.h>

// Structure to store edge information
struct Edge {
    int yMin;
    int yMax;
    double xCurrent;
    double inverseSlope; // 1/m = (x2 - x1) / (y2 - y1)
};

int main() {
    int n;
    std::cout << "Enter the number of vertices of the polygon: ";
    std::cin >> n;

    if (n < 3) {
        std::cout << "A polygon must have at least 3 vertices.\n";
        return 1;
    }

    std::vector<int> x(n), y(n);
    int yMinTotal = 10000, yMaxTotal = -10000;

    std::cout << "Enter the coordinates of vertices (x y) in sequence:\n";
    for (int i = 0; i < n; i++) {
        std::cout << "Vertex " << i + 1 << ": ";
        std::cin >> x[i] >> y[i];

        // Track global yMin and yMax to limit our scanlines
        if (y[i] < yMinTotal) yMinTotal = y[i];
        if (y[i] > yMaxTotal) yMaxTotal = y[i];
    }

    // Initialize graphics window
    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);

    // 1. Draw the outline of the original polygon
    setcolor(WHITE);
    for (int i = 0; i < n; i++) {
        int next = (i + 1) % n;
        line(x[i], y[i], x[next], y[next]);
    }

    // 2. Build the Global Edge Table (GET)
    std::vector<Edge> globalEdges;
    for (int i = 0; i < n; i++) {
        int next = (i + 1) % n;

        int x1 = x[i], y1 = y[i];
        int x2 = x[next], y2 = y[next];

        // Ignore perfectly horizontal edges as they don't cross scan-lines
        if (y1 == y2) continue; 

        Edge e;
        if (y1 < y2) {
            e.yMin = y1;
            e.yMax = y2;
            e.xCurrent = x1;
        } else {
            e.yMin = y2;
            e.yMax = y1;
            e.xCurrent = x2;
        }
        e.inverseSlope = static_cast<double>(x2 - x1) / (y2 - y1);
        globalEdges.push_back(e);
    }

    // List to keep track of active edges intersecting the current scan-line
    std::vector<Edge> activeEdgeList;

    // 3. Process every scan-line row by row
    for (int scanlineY = yMinTotal; scanlineY <= yMaxTotal; scanlineY++) {
        
        // Move qualifying edges from Global Edge List to Active Edge List
        for (auto it = globalEdges.begin(); it != globalEdges.end(); ) {
            if (it->yMin == scanlineY) {
                activeEdgeList.push_back(*it);
                it = globalEdges.erase(it); // Remove from global once active
            } else {
                ++it;
            }
        }

        // Remove edges from Active List if the scan-line has passed their yMax
        for (auto it = activeEdgeList.begin(); it != activeEdgeList.end(); ) {
            if (it->yMax <= scanlineY) {
                it = activeEdgeList.erase(it);
            } else {
                ++it;
            }
        }

        // Collect all X-intersections for the current scan-line
        std::vector<int> xIntersections;
        for (const auto& edge : activeEdgeList) {
            xIntersections.push_back(static_cast<int>(edge.xCurrent));
        }

        // Sort X-intersections from left to right
        std::sort(xIntersections.begin(), xIntersections.end());

        // 4. Draw horizontal filling spans between pairs of intersections
        setcolor(YELLOW); // Color used to fill the polygon
        for (size_t i = 0; i < xIntersections.size(); i += 2) {
            if (i + 1 < xIntersections.size()) {
                line(xIntersections[i], scanlineY, xIntersections[i + 1], scanlineY);
            }
        }

        // 5. Update x-coordinates for the next scan-line iteration using 1/m
        for (auto& edge : activeEdgeList) {
            edge.xCurrent += edge.inverseSlope;
        }
    }

    getch();
    closegraph();
    return 0;
}