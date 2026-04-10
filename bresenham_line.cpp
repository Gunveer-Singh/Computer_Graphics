#include <graphics.h>
#include <iostream>
#include <conio.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);

    int x1 = 150, y1 = 300;
    int x2 = 220, y2 = 100;

    int dx = x2 - x1;
    int dy = y2 - y1;

    int p = 2 * dy - dx;  

    int x = x1;
    int y = y1;

    putpixel(x, y, WHITE);

    for(int i = 0; i < dx; i++){
        x = x + 1;

        if(p < 0){
            p = p + 2 * dy;
        }
        else{
            y = y + 1;
            p = p + 2 * dy - 2 * dx;
        }
        putpixel(x, y, WHITE);
    }

    getch();
    closegraph();
    return 0;
}
