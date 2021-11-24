#pragma once

#ifndef TERMINAL_H
#define TERMINAL_H
#endif

#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string>

class terminal{
    private:
    struct winsize size;
    unsigned int score{0};
    public:
    terminal(){ioctl(STDIN_FILENO,TIOCGWINSZ,&size);};
    unsigned short gettw() const;
    unsigned short getth() const;
    void update();
    unsigned keyboardspy();
    void init() const;
    void paintgg() const;
    void scoreinc(unsigned int point);
};

void terminal::init() const
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    attrset(A_NORMAL);
}

unsigned short terminal::gettw() const
{
    return size.ws_col;
}

unsigned short terminal::getth() const
{
    return size.ws_row;
}

void terminal::update()
{
    ioctl(STDIN_FILENO,TIOCGWINSZ,&size);
}

unsigned terminal::keyboardspy()
{
    fd_set rfds;
    struct timeval tv;
    unsigned ch=0;
    FD_ZERO(&rfds);
    FD_SET(0,&rfds);
    tv.tv_sec=0;
    tv.tv_usec=10;
    if(select(1,&rfds,NULL,NULL,&tv)>0)
        ch=static_cast<unsigned>(getchar());
    return ch;
}

void terminal::paintgg() const
{
    clear();
    mvaddstr(getth()/3,gettw()/3,"Game Over!");
    mvprintw((getth()+1)/3,gettw()/3,"Your Score Is %u",score);
    mvaddstr((getth()+2)/3,gettw()/3,"Press q to quit....");
    refresh();
}

void terminal::scoreinc(unsigned int point)
{
    score+=point;
}