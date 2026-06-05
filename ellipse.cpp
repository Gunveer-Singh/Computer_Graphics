#include <graphics.h>
#include <iostream>
#include <conio.h>

void drawEllipse(int x_center, int y_center, int x, int y)
{
    // 4-way symmetry for an ellipse
    putpixel(x_center + x, y_center + y, WHITE);
    putpixel(x_center - x, y_center + y, WHITE);
    putpixel(x_center + x, y_center - y, WHITE);
    putpixel(x_center - x, y_center - y, WHITE);
}

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);

    int x_center, y_center, rx, ry;

    std::cout << "Enter center (x y): ";
    std::cin >> x_center >> y_center;

    std::cout << "Enter major and minor radii (rx ry): ";
    std::cin >> rx >> ry;

    int x = 0;
    int y = ry;

    // Initial decision parameter for Region 1
    double p1 = (ry * ry) - (rx * rx * ry) + (0.25 * rx * rx);
    
    int dx = 2 * ry * ry * x;
    int dy = 2 * rx * rx * y;

    // Region 1: Slope is less than 1 (dx < dy)
    while (dx < dy) {
        drawEllipse(x_center, y_center, x, y);

        x += 1;
        dx = dx + 2 * ry * ry;

        if (p1 < 0) {
            p1 = p1 + dx + (ry * ry);
        } else {
            y -= 1;
            dy = dy - 2 * rx * rx;
            p1 = p1 + dx - dy + (ry * ry);
        }
    }

    // Initial decision parameter for Region 2
    double p2 = ((ry * ry) * ((x + 0.5) * (x + 0.5))) + 
                ((rx * rx) * ((y - 1) * (y - 1))) - 
                (rx * rx * ry * ry);

    // Region 2: Slope is greater than 1 (y reaches 0)
    while (y >= 0) {
        drawEllipse(x_center, y_center, x, y);

        y -= 1;
        dy = dy - 2 * rx * rx;

        if (p2 > 0) {
            p2 = p2 - dy + (rx * rx);
        } else {
            x += 1;
            dx = dx + 2 * ry * ry;
            p2 = p2 + dx - dy + (rx * rx);
        }
    }

    getch();
    closegraph();
    return 0;
}