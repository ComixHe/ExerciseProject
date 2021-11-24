#include "food.hpp"
#include "snake.hpp"
#include "terminal.hpp"
#include <time.h>
#include <ncurses.h>
#include <unistd.h>

int main()
{
    unsigned kbhit=0;
    char ex;
    terminal t;
    t.init();
    snake mysnake;
    direction dir;
    mysnake.init();
    food myfood=food(t.gettw()-1,t.getth()-1);
    myfood.createfood();
    myfood.seteatten(false);
    myfood.paintfood();
    while(!mysnake.crash(t.gettw(),t.getth()))
    {
        kbhit=t.keyboardspy();
        t.update();
        if(myfood.iseat())
        {
            myfood.createfood();
            myfood.seteatten(false);
            myfood.paintfood();
        }
        if(kbhit==87||kbhit==119)
            dir=direction::FORWARD;
        else if(kbhit==65||kbhit==97)
            dir=direction::LEFT;
        else if(kbhit==68||kbhit==100)
            dir=direction::RIGHT;
        else if(kbhit==83||kbhit==115)
            dir=direction::BACKWARD;
        mysnake.setdir(dir);
        mysnake.paintbody();
        if(mysnake.eatfood(myfood))
        {
            myfood.seteatten(true);
            mysnake.grow(myfood.getflevel());
            t.scoreinc(myfood.getflevel());
        }
        usleep(150000);
    }
    t.paintgg();
    ex=getchar();
    if(ex=='q')
    {
        endwin();
        return 0;
    }
}