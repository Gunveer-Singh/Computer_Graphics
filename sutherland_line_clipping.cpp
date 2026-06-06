#include <graphics.h>
#include <iostream>
#include <conio.h>

#define INSIDE 0
#define LEFT   1
#define RIGHT  2
#define BOTTOM 4
#define TOP    8

double xMin, yMin, xMax, yMax;

int computeCode(double x, double y) {
    int code = INSIDE;

    if      (x < xMin) code |= LEFT;
    else if (x > xMax) code |= RIGHT;

    if      (y < yMin) code |= BOTTOM;
    else if (y > yMax) code |= TOP;

    return code;
}

void cohenSutherland(double x1, double y1, double x2, double y2) {
    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);

    bool accepted = false;

    // Calculate slope and inverse slope once before the loop
    double m    = (x2 != x1) ? (y2 - y1) / (x2 - x1) : 0; // dy/dx
    double invM = (y2 != y1) ? (x2 - x1) / (y2 - y1) : 0; // dx/dy

    while (true) {
        if (!(code1 | code2)) {
            accepted = true;
            break;
        } else if (code1 & code2) {
            break;
        } else {
            double x, y;
            int codeOut = code1 ? code1 : code2;

            if (codeOut & TOP) {
                // x = x1 + (1/m) * (yMax - y1)
                x = x1 + invM * (yMax - y1);
                y = yMax;
            } else if (codeOut & BOTTOM) {
                // x = x1 + (1/m) * (yMin - y1)
                x = x1 + invM * (yMin - y1);
                y = yMin;
            } else if (codeOut & RIGHT) {
                // y = y1 + m * (xMax - x1)
                y = y1 + m * (xMax - x1);
                x = xMax;
            } else { // LEFT
                // y = y1 + m * (xMin - x1)
                y = y1 + m * (xMin - x1);
                x = xMin;
            }

            if (codeOut == code1) {
                x1 = x; y1 = y;
                code1 = computeCode(x1, y1);
            } else {
                x2 = x; y2 = y;
                code2 = computeCode(x2, y2);
            }
        }
    }

    if (accepted) {
        setcolor(YELLOW);
        line((int)x1, (int)y1, (int)x2, (int)y2);
    } else {
        outtextxy(10, 440, (char*)"Line completely outside window - rejected");
    }
}

int main() {
    std::cout << "==== Cohen-Sutherland Line Clipping ====\n";
    std::cout << "Enter clipping window (xMin yMin xMax yMax): ";
    std::cin >> xMin >> yMin >> xMax >> yMax;

    double x1, y1, x2, y2;
    std::cout << "Enter line endpoints (x1 y1 x2 y2): ";
    std::cin >> x1 >> y1 >> x2 >> y2;

    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);

    setcolor(CYAN);
    rectangle((int)xMin, (int)yMin, (int)xMax, (int)yMax);
    outtextxy((int)xMin, (int)yMax + 10, (char*)"Clipping Window");

    setcolor(RED);
    line((int)x1, (int)y1, (int)x2, (int)y2);
    outtextxy((int)x1, (int)y1 - 15, (char*)"Original");

    delay(1000);

    cohenSutherland(x1, y1, x2, y2);
    outtextxy(10, 460, (char*)"Yellow = Clipped  Red = Original  Cyan = Window");

    getch();
    closegraph();
    return 0;
}