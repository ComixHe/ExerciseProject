#pragma once

#ifndef SNAKE_H
#define SNAKE_H
#endif

#include <vector>
#include <ncurses.h>

enum class direction:int{FORWARD=1,LEFT,BACKWARD,RIGHT};

typedef struct body{
    int cx;
    int cy;
    int status;
}body;

class snake{
    private:
        int length{0};
        char type{'*'};
        int now{4};
        std::vector<body> snakebody;
    public:
        snake():snake('*'){};
        snake(char t):type(t){};
        void grow(int l);
        void setdir(direction dir);
        bool crash(int ww,int wh) const;
        void snakemove();
        void init();
        bool eatfood(food &f) const;
        void paintbody();
        void cleartail() const;
        int getdir() const;
};

void snake::grow(int l)
{
    body tail=snakebody[snakebody.size()-1];
    if(tail.status==3)
    {
        for(int i=1;i<=l;++i)
        {
            body temp;
            temp.cx=tail.cx+i;
            temp.cy=tail.cy;
            temp.status=3;
            snakebody.push_back(temp);
        }
    }
    else if(tail.status==1)
    {
        for(int i=1;i<=l;++i)
        {
            body temp;
            temp.cx=tail.cx;
            temp.cy=tail.cy+i;
            temp.status=1;
            snakebody.push_back(temp);
        }
    }
    else if(tail.status==2)
    {
        for(int i=1;i<=l;++i)
        {
            body temp;
            temp.cx=tail.cx;
            temp.cy=tail.cy-i;
            temp.status=2;
            snakebody.push_back(temp);
        }
    }
    else if(tail.status==4)
    {
        for(int i=1;i<=l;++i)
        {
            body temp;
            temp.cx=tail.cx-i;
            temp.cy=tail.cy;
            temp.status=4;
            snakebody.push_back(temp);
        }
    }
}

void snake::setdir(direction dir)
{
    switch(dir)
    {
        case direction::FORWARD:now=1;break;
        case direction::BACKWARD:now=2;break;
        case direction::LEFT:now=3;break;
        case direction::RIGHT:now=4;break;
    }
}

bool snake::crash(int ww,int wh) const
{
    body head=snakebody[0];
    for(int i=1;i<snakebody.size();i++)
    {
        if(snakebody[i].cx==head.cx&&snakebody[i].cy==head.cy)
            return true;
    }
    if(head.cx<=0||head.cx>=ww||head.cy<=0||head.cy>=wh)
        return true;
    return false;
}

void snake::snakemove()
{
    for(auto &t:snakebody)
    {
        switch(t.status)
        {
            case 1: --t.cy; break;
            case 2: ++t.cy; break;
            case 3: --t.cx; break;
            case 4: ++t.cx; break;
        }
    }
    for(int i=snakebody.size()-1;i>=1;--i)
        snakebody[i].status=snakebody[i-1].status;
    if(now==1) 
        snakebody[0].status=1;
    else if(now==2) 
        snakebody[0].status=2;
    else if(now==3) 
        snakebody[0].status=3;
    else if(now==4) 
        snakebody[0].status=4;
}

void snake::init()
{
    body temp;
    temp.cx=7;
    temp.cy=7;
    temp.status=4;
    snakebody.push_back(temp);
}

bool snake::eatfood(food &f) const
{
    if(snakebody[0].cx==f.getx()&&snakebody[0].cy==f.gety())
        return true;
    else
        return false;
}

int snake::getdir() const
{
    return now;
}

void snake::paintbody()
{
    cleartail();
    snakemove();
    mvaddch(snakebody[0].cy,snakebody[0].cx,type);
    refresh();
}

void snake::cleartail() const
{
    int x=snakebody[snakebody.size()-1].cx;
    int y=snakebody[snakebody.size()-1].cy;
    mvaddch(y,x,' ');   
    refresh();
}