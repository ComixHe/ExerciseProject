#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2);

int main(void)
{
    struct ListNode a1,a2,b1,b2,b3,*p;
    a1.next=NULL;
    a1.val=1;
    b1.next=&b2,b2.next=NULL;
    b1.val=9,b2.val=9;
    p=addTwoNumbers(&a1,&b1);
    while(p!=NULL)
    {
        printf("%d ",p->val);
        p=p->next;
    }
    return 0;
}

struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2){
    struct ListNode *sum,*s;
    bool flag=false,first=true;
    int num=-1;
    while(l1!=NULL||l2!=NULL)
    {
        struct ListNode *temp=malloc(sizeof(struct ListNode));
        if(l1!=NULL)
        {
            temp->val=l1->val;
            if(l2!=NULL)
                temp->val+=l2->val;
        }
        else
            temp->val=l2->val;
        if(flag)
        {
            temp->val+=1;
            flag=false;
        }
        if(temp->val>9)
        {
            if(l1!=NULL&&l2!=NULL&&l1->next==NULL&&l2->next==NULL)
                num=temp->val/10;
            if((l1==NULL&&l2!=NULL&&l2->next==NULL)||(l2==NULL&&l1!=NULL&&l1->next==NULL))
                num=temp->val/10;
            temp->val-=10;
            flag=true;
        }
        if(l1!=NULL)
            l1=l1->next;
        if(l2!=NULL)
            l2=l2->next;
        if(first)
        {
            sum=temp;
            s=temp;
            first=false;
        }
        else
        {
            s->next=temp;
            s=s->next;
        }
    }
    if(num!=-1)
    {
        struct ListNode *temp=malloc(sizeof(struct ListNode));
        temp->val=num;
        s->next=temp;
        s=s->next;
    }
    s->next=NULL;
    return sum;
}