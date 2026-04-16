#include <graphics.h>
#include <iostream>
#include <conio.h>

void drawCircle(int x_center, int y_center, int x, int y)
{
    putpixel(x_center + x, y_center + y, WHITE);
    putpixel(x_center - x, y_center + y, WHITE);
    putpixel(x_center + x, y_center - y, WHITE);
    putpixel(x_center - x, y_center - y, WHITE);
    putpixel(x_center + y, y_center + x, WHITE);
    putpixel(x_center - y, y_center + x, WHITE);
    putpixel(x_center + y, y_center - x, WHITE);
    putpixel(x_center - y, y_center - x, WHITE);
}

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);

    int x_center, y_center, r;

    std::cout << "Enter center: ";
    std::cin >> x_center >> y_center;

    std::cout << "Enter radius: ";
    std::cin >> r;

    int x = 0;
    int y = r;

    int p = 3-2*r;

    drawCircle(x_center, y_center, x, y);

    while (x <= y){
        x += 1;

        if (p < 0){
            p = p + 4*x + 6;
        }
        else{
            y -= 1;
            p = p + 4*(x-y) + 10;
        }

        drawCircle(x_center, y_center, x, y);
    }

    getch();
    closegraph();
    return 0;
}