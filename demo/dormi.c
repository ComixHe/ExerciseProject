#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct info{
    char faculty[10];
    char name[10];
    char id[10];
    char room[10];
}info;

typedef struct list{
    info *stu;
    struct list *next;
}list;

typedef struct warpper{
    int key;
    list *value;
}wapper;

int write_to_file(list *l)
{
    FILE *a=fopen("/home/comix/sushe.txt","w");
    if(a==NULL)
    {
        printf("can't open this file.\n");
        return 0;
    }
    for(;l!=NULL;l=l->next)
    {
        info *i=l->stu;
        fputs(i->id,a);
        fputc(' ',a);
        fputs(i->room,a);
        fputc(' ',a);
        fputs(i->name,a);
        fputc(' ',a);
        fputs(i->faculty,a);
        fputc('\n',a);
    }
    fclose(a);
    return 1;
}

list *process_info(char *t,list *s)
{
    const char *c=" ";
    info *n=s->stu;
    t[strlen(t)-1]='\0';
    strcpy(n->id,strtok(t,c));
    strcpy(n->room,strtok(NULL,c));
    strcpy(n->name,strtok(NULL,c));
    strcpy(n->faculty,strtok(NULL,c));
    return s;
}

list *create_node()
{
    list *new=malloc(sizeof(list));
    if(new==NULL)
    {
        printf("malloc error!\n");
        return NULL;
    }
    new->next=NULL;
    new->stu=malloc(sizeof(info));
    if(new->stu==NULL)
    {
        printf("malloc error!\n");
        return NULL;
    }
    return new;
}

void destory_list(list *l)
{
    list *temp=l;
    while(l!=NULL)
    {
        free(l->stu);
        l=l->next;
        free(temp);
        temp=l;
    }
}

list *read_from_file(char *p)
{
    list *head=NULL,**itr=&head;
    FILE *a=fopen(p,"r+");
    if(a==NULL)
    {
        printf("can't read this file.\n");
        return NULL;
    }
    char temp[50];
    while(fgets(temp,50,a)!=NULL)
    {
        *itr=create_node();
        if(itr==NULL)
        {
            printf("read error!\n");
            break;
        }
        process_info(temp,*itr);
        itr=&((*itr)->next);
    }
    fclose(a);
    return head;
}

list *combine(list *old,list *new)
{
    list *new_itr=new;
    while(new_itr!=NULL)
    {
        list *before=NULL,*old_itr=old,*temp=new_itr;
        new_itr=new_itr->next;
        while(old_itr!=NULL)
        {
            if(atoi(temp->stu->id)==atoi(old_itr->stu->id))
            {
                free(temp);
                break;
            }
            if(atoi(temp->stu->id)<atoi(old_itr->stu->id))
            {
                if(before==NULL)
                {
                    temp->next=old_itr;
                    old=temp;
                }
                else
                {
                    temp->next=before->next;
                    before->next=temp;
                }
                break;
            }
            else
            {
                before=old_itr;
                old_itr=old_itr->next;
            }
        }
        if(old_itr==NULL)
        {
            before->next=temp;
            temp->next=NULL;
        }
    }
    return old;
}

list *insert(list *old)
{
    list *new=create_node(),*itr=new;
    char buffer[10];
    while(1)
    {
        char f;
        printf("input id:");
        scanf("%s",buffer);
        strcpy(itr->stu->id,buffer);
        printf("input room:");
        scanf("%s",buffer);
        strcpy(itr->stu->room,buffer);
        printf("input name:");
        scanf("%s",buffer);
        strcpy(itr->stu->name,buffer);
        printf("input faculty:");
        scanf("%s",buffer);
        strcpy(itr->stu->faculty,buffer);
        printf("continue?[y/n]:");
        getchar();
        scanf("%c",&f);
        if(f=='n')
        {
            itr->next=NULL;
            break;
        }
        itr->next=create_node();
        itr=itr->next;
        printf("\n");
    }
    if(old!=NULL)
        new=combine(old,new);
    return new;
}

void show_info(list *i)
{
    if(i!=NULL)
    {
        printf("found!\n");
        list *q=i;
        while(q!=NULL)
        {
            printf("id:%s ",q->stu->id);
            printf("name:%s ",q->stu->name);
            printf("room:%s ",q->stu->room);
            printf("faculty:%s ",q->stu->faculty);
            printf("\n");
            q=q->next;
        }
        destory_list(i);
    }
    else
        printf("not found!");
}

int binary_search(const wapper *b, int search_key, int low, int high)
{
    while (low<=high)
    {
      int mid=(low+high)/2;
      if(search_key==b[mid].key)
        return mid;
      else if(search_key<b[mid].key)
        high=mid-1;
      else
        low=mid+1;
    }
   return -1;
}

list *search_by_id(list *file,const char *tar)
{
    int count=0,key=atoi(tar);
    list *t=NULL,*result=NULL;
    for(t=file;t!=NULL;t=t->next)
        count++;
    wapper id_list[count];
    for(int i=0;i<count;i++)
    {
        id_list[i].key=-1;
        id_list[i].value=NULL;
    }
    count=0;
    for(t=file;t!=NULL;t=t->next)
    {
        id_list[count].key=atoi(t->stu->id);
        id_list[count].value=t;
        ++count;
    }
    int get=binary_search(id_list,key,0,count);
    if(get!=-1)
    {
        result=create_node();
        *(result->stu)=*(id_list[get].value->stu);
        result->next=NULL;
    }
    return result;
}

list *search_by_name(list *file,const char *tar)
{
    list *temp=NULL,**itr=&temp;
    while(file!=NULL)
    {
        if(strcmp(tar,file->stu->name)==0)
        {
            *itr=create_node();
            *((*itr)->stu)=*(file->stu);
            (*itr)->next=NULL;
            itr=&((*itr)->next);
        }
        file=file->next;
    }
    return temp;
}

list *search_by_room(list *file,const char *tar)
{
    list *temp=NULL,**itr=&temp;
    while(file!=NULL)
    {
        if(strcmp(tar,file->stu->room)==0)
        {
            *itr=create_node();
            *((*itr)->stu)=*(file->stu);
            (*itr)->next=NULL;
            itr=&((*itr)->next);
        }
        file=file->next;
    }
    return temp;
}

list *search_by_faculty(list *file,const char *tar)
{
    list *temp=NULL,**itr=&temp;
    while(file!=NULL)
    {
        if(strcmp(tar,file->stu->faculty)==0)
        {
            *itr=create_node();
            *((*itr)->stu)=*(file->stu);
            (*itr)->next=NULL;
            itr=&((*itr)->next);
        }
        file=file->next;
    }
    return temp;
}

list *search(list *file,int mode)
{

    char temp[10];
    char *op[4]={"name","id","room","faculty"};
    list *n=NULL;
    printf("input %s:",op[mode-1]);
    scanf("%s",temp);
    while(file!=NULL)
    {
        switch(mode)
        {
            case 1:{
                return search_by_name(file,temp);
            }break;
            case 2:{
                return search_by_id(file,temp);
            }break;
            case 3:{
                return search_by_room(file,temp);
            }break;
            case 4:{
                return search_by_faculty(file,temp);
            }break;
        }
        file=file->next;
    }
    return n;
}

int main()
{
    int flag,choose;
    list *file=file=read_from_file("/home/comix/sushe.txt");
    start:
    flag=0;
    printf(" \n\
    |       dormitory management system       |\n\
    ===========================================\n\
    |please choose function:                  |\n\
    |    1.insert infomation.                 |\n\
    |    2.search infomation.                 |\n\
    |    3.quit.                              |\n\
    ===========================================\n");
    scanf("%d",&flag);
    switch(flag){
        case 1:{
            file=insert(file);
            write_to_file(file);
            printf("insert done!\n");
        }break;
        case 2:{
            again:
            printf("please choose the way of search\n\
            1.by name.\n\
            2.by id.\n\
            3.by room.\n\
            4.by faculty.\n\
            5.back\n");
            scanf("%d",&choose);
            if(choose==5)
                goto start;
            else
                show_info(search(file,choose));
        }break;
        case 3:{
            destory_list(file);
            goto end;
        }break;
        default:{
            printf("this function not found.please retry!\n");
        }
    }
    goto start;
    end:
    printf("bye!");
    return 0;
}