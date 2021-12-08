#include <stdio.h>

typedef struct traid{
    int row;
    int col;
    int value;
}traid;

int main()
{
    int num_a,num_b,num_c=0,row_a,row_b,col_a,col_b,i,flag=1;
    printf("A's total element:");
    scanf("%d",&num_a);
    printf("B's total element:");
    scanf("%d",&num_b);
    printf("A's row:");
    scanf("%d",&row_a);
    printf("A's col:");
    scanf("%d",&col_a);
    printf("B's row:");
    scanf("%d",&row_b);
    printf("B's col:");
    scanf("%d",&col_b);
    traid a[num_a],ca[num_a],b[num_b],c[num_a+num_b];
    int array[col_a];
    printf("Traid A:\n");
    for(i=0;i<num_a;i++)
    {
        scanf("%d%d%d",&a[i].row,&a[i].col,&a[i].value);
        ca[i]=a[i];
    }
    printf("Traid B:\n");
    for(i=0;i<num_b;i++)
        scanf("%d%d%d",&b[i].row,&b[i].col,&b[i].value);
    for(i=0;i<num_a;i++)
        array[a[i].col-1]++;

    for(i=0;i<num_a;i++)
    {
        a[i].col+=a[i].row;
        a[i].row=a[i].col-a[i].row;
        a[i].col-=a[i].row;
    }
    for(i=0;i<num_a;i++)
    {
        for(int j=0;j<num_a;j++)
        {
            if(ca[j].row>ca[j+1].row)
            {
                traid ex=ca[j];
                ca[j]=ca[j+1];
                ca[j+1]=ex;
            }
            else if(ca[j].row==ca[j+1].row)
            {
                if(ca[j].col>ca[j+1].col)
                {
                    traid ex=ca[j];
                    ca[j]=ca[j+1];
                    ca[j+1]=ex;
                }
            }
        }
    }
    printf("A after convert:\n");
    for(i=0;i<num_a;i++)
        printf("%d %d %d\n",ca[i].row,ca[i].col,ca[i].value);
    for(i=0;i<num_a;i++)
    {
        c[i]=a[i];
        ++num_c;
    }
    for(i=0;i<num_b;i++)
    {
        for(int j=0;j<num_a;j++)
        {
            if(b[i].row==a[j].row)
            {
                if(b[i].col==a[j].col)
                {
                    flag=0;
                    c[j].value+=b[i].value;
                }
            }
        }
        if(flag)
        {
            c[num_c++]=b[i];
            flag=0;
        }
    }
    for(i=0;i<num_c;i++)
    {
        for(int j=0;j<num_c;j++)
        {
            if(c[j].row>c[j+1].row)
            {
                traid ex=c[j];
                c[j]=c[j+1];
                c[j+1]=ex;
            }
            else if(c[j].row==c[j+1].row)
            {
                if(c[j].col>c[j+1].col)
                {
                    traid ex=c[j];
                    c[j]=c[j+1];
                    c[j+1]=ex;
                }
            }
        }
    }
    for(i=0;i<num_c;i++)
        printf("%d %d %d\n",c[i].row,c[i].col,c[i].value);
    return 0;
}
