#include <graphics.h>
#include <iostream>
#include <conio.h>
#include <cmath>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);

    float x1 = 150, y1 = 300;
    float x2 = 220, y2 = 100;

    float dx = x2 - x1;
    float dy = y2 - y1;

    int steps = std::max(abs(dx), abs(dy));

    float x_inc = dx / steps;
    float y_inc = dy / steps;

    float x = x1;
    float y = y1;

    for(int i = 0; i <= steps; i++)
    {
        putpixel(round(x), round(y), WHITE);
        x += x_inc;
        y += y_inc;
    }

    getch();
    closegraph();
}


