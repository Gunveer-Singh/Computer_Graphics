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

    int p = 1-r;

    while (x <= y){
        drawCircle(x_center, y_center, x, y);
        x += 1;

        if (p < 0){
            p = p + 2*x + 1;
        }
        else{
            y -= 1;
            p = p + 2*(x-y) + 1;
        }
    }

    getch();
    closegraph();
    return 0;
}