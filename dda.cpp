#include <graphics.h>
#include <conio.h>


int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);

    int x1, x2, y1, y2;
    x1 = 100;
    y1 = 100;
    x2 = 200;
    y2 = 200;

    float slope = (float)(y2-y1)/(x2-x1);

    for (int i = x1+1; i < x2; i++){
        y1 += slope;
        putpixel(i,y1,WHITE);    
    }


    getch();
    closegraph();
}