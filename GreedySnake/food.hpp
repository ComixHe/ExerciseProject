#pragma once

#ifndef FOOD_H
#define FOOD_H
#endif

#include <random>
#include <ncurses.h>

class food{
    private:
        int food_position_x{0};
        int food_position_y{0};
        int windows_width{0};
        int windows_height{0};
        int food_level{0};
        bool eattenflag{false};
        char food_type;
        void randomfood()
        {
            std::random_device rd;
            std::mt19937 eng(rd());
            std::uniform_int_distribution<int> h_dis(0,windows_height);
            std::uniform_int_distribution<int> w_dis(0,windows_width);
            std::uniform_int_distribution<int> f_dis(1,5);
            food_position_x=w_dis(eng);
            food_position_y=h_dis(eng);
            food_level=f_dis(eng);
        }
    public:
        food(int x,int y,char t='@'):windows_width(x),windows_height(y),food_type(t){};
        food()=default;
        int getx() const;
        int gety() const;
        int getflevel() const;
        void createfood();
        void seteatten(bool status);
        bool iseat() const;
        void paintfood() const;
        void clearfood() const;
};

void food::createfood()
{
    randomfood();
}

int food::getx() const
{
    return food_position_x;
}

int food::gety() const
{
    return food_position_y;
}

int food::getflevel() const
{
    return food_level;
}

void food::seteatten(bool status)
{
    eattenflag=status;
}

bool food::iseat() const
{
    return eattenflag;
}

void food::clearfood() const
{
    mvaddch(food_position_y,food_position_x,' ');
    refresh();
}

void food::paintfood() const
{
    mvaddch(food_position_y,food_position_x,food_type);
    refresh();
}