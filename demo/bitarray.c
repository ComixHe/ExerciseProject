#include <stdio.h>

void setbit(__uint8_t bitarray[],unsigned bitnumber);
void clearbit(__uint8_t bitarray[],unsigned bitnumber);
void assignbit(__uint8_t bitarray[],unsigned bitnumber,int value);
int testbit(__uint8_t bitarray[],unsigned bitnumber);

int main(void)
{
    __uint8_t a[10];
    return 0;
}

void setbit(__uint8_t bitarray[],unsigned bitnumber)
{
    int index=bitnumber/8;
    int temp=bitnumber%8-1;
    if(temp==-1)
        temp=7;
    if(bitnumber%8==0)
        index--;
    bitarray[index]|=((__uint8_t)1<<temp);
}

void clearbit(__uint8_t bitarray[],unsigned bitnumber)
{
    int index=bitnumber/8;
    int temp=bitnumber%8-1;
    if(temp==-1)
        temp=7;
    if(bitnumber%8==0)
        index--;
    bitarray[index]&=(~((__uint8_t)1<<temp));
}

void assignbit(__uint8_t bitarray[],unsigned bitnumber,int value)
{
    if(value==0)
        setbit(bitarray,bitnumber);
    else
        clearbit(bitarray,bitnumber);
}

int testbit(__uint8_t bitarray[],unsigned bitnumber)
{
    int index=bitnumber/8;
    int temp=bitnumber%8-1;
    if(temp==-1)
        temp=7;
    if(bitnumber%8==0)
        index--;
    __uint8_t bit=bitarray[index];
    return ((__uint8_t)1<<temp)&bit;
}