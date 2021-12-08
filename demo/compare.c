#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void insertC(FILE *file,char choice);
int main(int argc,char **argv)
{
    if(argc>3)
    {
        printf("too many arg!",argc);
        exit(1);
    }
    FILE *file_1,*file_2,*file_3;
    file_1=fopen(argv[1],"r");
    file_2=fopen(argv[2],"r");
    file_3=fopen("/home/comix/all.txt","a+");
    if(file_1==NULL||file_2==NULL)
    {
        printf("can't open files!");
        exit(1);
    }
    char cache_longer[100],cache_shorter[100];
    unsigned int count_1=0,count_2=0;
    while(1)
    {
        if(fscanf(file_1,"%[^\n]",cache_longer)==EOF)
            break;
        count_1++;
        fseek(file_1,1,SEEK_CUR);
    }
    while(1)
    {
        if(fscanf(file_2,"%[^\n]",cache_longer)==EOF)
            break;
        count_2++;
        fseek(file_2,1,SEEK_CUR);
    }
    fseek(file_1,0,SEEK_SET);
    fseek(file_2,0,SEEK_SET);
    FILE *longer,*shorter;
    int long_count,short_count;
    if(count_1>count_2)
    {
        longer=file_1,shorter=file_2;
        long_count=count_1,short_count=count_2;
    }
    else
    {
        longer=file_2,shorter=file_1;
        long_count=count_2,short_count=count_1;
    }
    int sum_of_mismatch=0;
    int collection=0;
    char choice;
    printf("Aria format?[Y/N]:");
    scanf("%c",&choice);
    while(fscanf(shorter,"%[^\n]",cache_shorter)!=EOF)
    {
        int temp=0;
        while(fscanf(longer,"%[^\n]",cache_longer)!=EOF)
        {
            if(strcmp(cache_longer,cache_shorter)==0)
                break;
            temp++;
            fseek(longer,1,SEEK_CUR);
        }
        if(temp!=long_count)
        {
            printf("match!longer_file:%s | shorter_file:%s\n",cache_longer,cache_shorter);
            fputs(cache_longer,file_3);
            insertC(file_3,choice);
            collection++;
        }
        else
        { 
            fputs(cache_longer,file_3);
            insertC(file_3,choice);
            fputs(cache_shorter,file_3);
            insertC(file_3,choice);
            sum_of_mismatch++;
            collection+=2;
        }
        fseek(shorter,1,SEEK_CUR);
        fseek(longer,0,SEEK_SET);
    }
    printf("\nthe longer file:%d row,the shorter file:%d row,the collection file:%d row,mismatch row:%d match row:%.1lf\n",
            long_count+1,short_count+1,collection,sum_of_mismatch+1,(double)(long_count+short_count-sum_of_mismatch+1)/2.0);
    fclose(file_1);
    fclose(file_2);
    fclose(file_3);
    return 0;
}

void insertC(FILE *file,char choice)
{
    if(choice=='Y'||choice=='y')
        fputc(',',file);
    else if(choice=='N'||choice=='n')
        fputc('\n',file);
    else
    {
        printf("invalid choice!");
        exit(1);
    }
    return;
}