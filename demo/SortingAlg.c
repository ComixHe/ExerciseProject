#include <stdio.h>

void bubble_sort(int b);
void selection_sort(int b);
void insert_sort(int b);
void quick_sort(int b);
void quick_sort_method(int *s,int low,int high);
void shell_sort(int b);

int main(void)
{
    int n;
    printf("please input the number of data:");
    scanf("%d",&n);
    if(n==0)
        return 0;
    printf("which fuction is you want to use?\n\
            1.bubble_sore.\n\
            2.insert_sort.\n\
            3.selection_sort.\n\
            4.quick_sort.\n\
            5.shell_sort\n\
    ");
    printf("your choice is:");
    int c;
    scanf("%d",&c);
    switch (c)
    {
        case 1:bubble_sort(n);break;
        case 2:insert_sort(n);break;
        case 3:selection_sort(n);break;
        case 4:quick_sort(n);break;
        case 5:shell_sort(n);break;
    }
    return 0;
}

void bubble_sort(int b)
{ 
    int a[b],temp;
    printf("please input all number:");
    for(int i=0;i<b;i++)
        scanf("%d",&a[i]);
    for(int i=0;i<b;i++)
    {
        for(int j=0;j<b-i-1;j++)
        {
            if(a[j]>a[j+1])
            {
                temp=a[j];
                a[j]=a[j+1];
                a[j+1]=temp;
            }
        }
    }
    printf("after sorted:");
    for(int i=0;i<b;i++)
        printf("%d ",a[i]);
    printf("\n");
    return;
}

void insert_sort(int b)
{
    int a[b];
    printf("please input all number:");
    for(int i=0;i<b;i++)
        scanf("%d",&a[i]);
    for(int i=1;i<b;i++)
        {
            if(a[i]<a[i-1])
            {
                int j=i-1;
                int x=a[i];
                while(j>=0&&x<a[j])
                {
                    a[j+1]=a[j];
                    j--;
                }
                a[j+1]=x;
            }
         }
         printf("after sorted:");
    for(int i=0;i<b;i++)
        printf("%d ",a[i]);
    printf("\n");
    return;
}

void selection_sort(int b)
{
    int a[b],temp,x;
    printf("please input all number:");
    for(int i=0;i<b;i++)
        scanf("%d",&a[i]);
    for(int i=0;i<b;i++,temp=0)
    {
        x=0;
        for(int j=0;j<b-i;j++)
        {
            if(a[j]>a[x])
              x=j;
        }
        temp=a[x];
        a[x]=a[b-1-i];
        a[b-1-i]=temp;
    }
    printf("after sorted:");
    for(int i=0;i<b;i++)
        printf("%d ",a[i]);
    printf("\n");
    return;
}

void quick_sort(int b)
{
    int a[b];
    printf("please input all number:");
    for(int i=0;i<b;i++)
        scanf("%d",&a[i]);
    quick_sort_method(a,0,b-1);
    printf("after sorted:");
    for(int i=0;i<b;i++)
        printf("%d ",a[i]);
    printf("\n");
    return;
}

void quick_sort_method(int *a, int low, int high)
{
    if(low >= high)
        return ;
    int i = low;
    int j = high;
    int key = a[low];
    while(i < j)
    {
        while(i < j && key <= a[j])
            j--;
        a[i] = a[j];
        while(i < j && key >= a[i])
            i++;
        a[j] = a[i];
    }
    a[i] = key;
    quick_sort_method(a, low, i - 1);
    quick_sort_method(a, i + 1, high);
    //待优化
}

void shell_sort(int b)
{
    int a[b],temp,j;
    printf("please input all number:");
    for(int i=0;i<b;i++)
        scanf("%d",&a[i]);
    for(int gap=b/2;gap>=1;gap/=2)
    {
        for(int i=gap;i<b;i++)
        {
            temp=a[i];
            j=i-gap;
            while(j>=0&&temp<a[j])
            {
                a[j+gap]=a[j];
                j-=gap;
            }
            a[j+gap]=temp;
        }
    }
    printf("after sorted:");
    for(int i=0;i<b;i++)
        printf("%d ",a[i]);
    printf("\n");
}