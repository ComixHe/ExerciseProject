#include <stdio.h>
#include <stdarg.h>
int myprintf(const char *line,...);
int main(void)
{
    myprintf("hello %d %f %c",1,2.0,'A');
    return 0;
}

int myprintf(const char *line,...)
{
    va_list args;
    va_start(args,line);
    int index=0;
    const char *point=line;
    while((*point)!='\0')
    {
        if((*point)!='%')
            printf("%c",(*point));
        else
        {
            char temp=*(point+1);
            switch(temp)
            {
                case 'd':
                {
                    int integer=va_arg(args,int);
                    printf("%d",integer); 
                    break;
                }
                case 'f':
                {
                    float sig=va_arg(args,double);
                    printf("%f",sig); 
                    break;
                }
                case 'c':
                {
                    char c=va_arg(args,int);
                    printf("%c",c); 
                    break;
                }
                case 's':
                {
                    char *s=va_arg(args,char *);
                    printf("%s",args[index]); 
                    break;
                }
                default:printf("undefined!");break;
            }
            point++;
        }
        index++;
        point++;
    }
    va_end(args);
    return 0;
}
