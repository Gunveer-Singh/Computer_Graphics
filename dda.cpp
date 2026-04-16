#include <graphics.h>
#include <iostream>
#include <conio.h>
#include <cmath>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);
    int x1,y1,x2,y2;
    
    std::cout << "Enter x1 and y1: ";
    std::cin >> x1 >> y1;
    std::cout << "Enter x2 and y2: ";
    std::cin >> x2 >> y2;

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


