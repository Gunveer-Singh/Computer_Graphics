#include <graphics.h>
#include <iostream>
#include <conio.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);
    int x1,y1,x2,y2;
    
    std::cout << "Enter x1 and y1: ";
    std::cin >> x1 >> y1;
    std::cout << "Enter x2 and y2: ";
    std::cin >> x2 >> y2;

    int dx = x2 - x1;
    int dy = y2 - y1;

    int p = 2*dy-dx;  

    int x = x1;
    int y = y1;

    putpixel(x, y, WHITE);

    if (dx == 0){
        for (int i = y1; i < y2; i++){
            putpixel(x1,i,WHITE);
        }
    }

    for(int i = 0; i < dx; i++){
        x = x + 1;

        if(p < 0){
            p = p + 2*dy;
        }
        else{
            y = y + 1;
            p = p + 2*dy - 2*dx;
        }
        putpixel(x, y, WHITE);
    }

    getch();
    closegraph();
    return 0;
}
