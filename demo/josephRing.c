#include <stdio.h>
#include <stdlib.h>
typedef struct node{
	int value;
	struct node *next;
}node;
node *create(int v);
node *delete(node* p);
int main()
{
	node *head,*itr;
	head=malloc(sizeof(node));
	if(head==NULL)
	{
		printf("malloc error!");
		exit(EXIT_FAILURE);
	}
	int m,n,count=1;
	itr=head;
	printf("please input the number of people and distance:");
	scanf("%d%d",&m,&n);
	for(int temp=1;temp<=m;temp++)
	{
		itr->next=create(temp);
		itr=itr->next;
	}
	itr->next=head->next;
	itr=itr->next;
	while(m!=1)
	{
		if(count==n-1)
		{
			printf("%d is dead\n",itr->next->value);
			itr->next=delete(itr);
			itr=itr->next;
			m--;
			count=1;
		}
		else
		{
			count++;
			itr=itr->next;
		}
	}
	printf("%d is still alive",itr->value);
	free(itr);
	return 0;
}

node *create(int v)
{
	node *p;
	p=malloc(sizeof(node));
	if(p==NULL)
	{
		printf("malloc error!");
		exit(EXIT_FAILURE);
	}
	else
		p->value=v;
	return p;
}

node *delete(node *p)
{
	node *a=p->next->next;
	free(p->next);
	return a;
}