#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define CSH_RL_BUFSIZE 1024
#define CSH_TOK_BUFSIZE 64
#define CSH_TOK_DELIM " \t\r\n\a"


void csh_loop(void);
char *csh_read_line(void);
char **csh_split_line(char *line);
int csh_launch(char **args);
int csh_cd(char **args);
int csh_exit(char **args);
int csh_help(char **args);
int csh_num_builtins();
int csh_execute(char **args);

char *builtin_str[]={
    "cd",
    "help",
    "exit"
};
int (*builtin_fun[])(char **)={
    &csh_cd,
    &csh_help,
    &csh_exit
};

int csh_execute(char **args)
{
    int i;
    if(args[0]==NULL)
        return 1;
    for(i=0;i<csh_num_builtins();i++)
    {
        if(strcmp(args[0],builtin_str[i])==0)
            return (*builtin_fun[i])(args);
    }
    return csh_launch(args);
}

int csh_cd(char **args)
{
    if(args[1]==NULL)
    {
        fprintf(stderr,"csh: please input path\n");
    }
    else
    {
        if(chdir(args[1])!=0)
            perror("csh");
    }
    return 1;
}

int csh_exit(char **args)
{
    return 0;
}

int csh_help(char **args)
{
    int i;
    printf("A SImple Shell in C\n");
    printf("The Following are built in:\n");
    for(i=0;i<csh_num_builtins();i++)
    {
        printf("%s\n",builtin_str[i]);
    }
    printf("Please use 'man' in other command.\n");
    return 1;
}

int csh_num_builtins(void)
{
    return sizeof(builtin_str)/sizeof(char *);
}

int csh_launch(char **args) 
{
    pid_t pid;
    int status;

    pid=fork();
    if(pid==0)
    {
        if(execvp(args[0],args)==-1)
        {
            perror("csh");
            exit(EXIT_FAILURE);
        }
    }
    else if(pid<0)
    {
        perror("csh");
    }
    else
    {
        do{
            waitpid(pid,&status,WUNTRACED);
        }while(!WIFEXITED(status)&&!WIFSIGNALED(status));
    }
    return 1;
}

char **csh_split_line(char *line)
{
    int bufsize=CSH_TOK_BUFSIZE;
    int position=0;
    char **tokens=malloc(bufsize*sizeof(char *));
    char *token,**tokens_backup;

    if(!tokens)
    {
        fprintf(stderr,"csh:allocation error!\n");
        exit(EXIT_FAILURE);
    }

    token=strtok(line,CSH_TOK_DELIM);
    while(token!=NULL)
    {
        tokens[position]=token;
        position++;

        if(position>=bufsize)
        {
            bufsize+=CSH_TOK_BUFSIZE;
            tokens_backup=tokens;
            tokens=realloc(tokens,bufsize*sizeof(char *));
            if(!tokens)
            {
                free(tokens_backup);
                fprintf(stderr,"csh:allocation error!\n");
                exit(EXIT_FAILURE);
            }   
        }
        token=strtok(NULL,CSH_TOK_DELIM);
    }
    tokens[position]=NULL;
    return tokens;
}

char *csh_read_line(void)
{
    int ch;
    int bufsize=CSH_RL_BUFSIZE;
    int position=0;
    char *buffer=malloc(sizeof(char)*bufsize);
    if(!buffer)
    {
        fprintf(stderr,"csh:allocation error!\n");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        ch=getchar();
        if(ch==EOF)
        {
            exit(EXIT_SUCCESS);
        }
        else if(ch=='\n')
        {
            buffer[position]='\0';
            return buffer;
        }
        else
        {
            buffer[position]=ch;
        }
        position++;

        if(position>=bufsize)
        {
            bufsize+=CSH_RL_BUFSIZE;
            buffer=realloc(buffer,bufsize);
            if(!buffer)
            {
                fprintf(stderr, "csh:allocation error!\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void csh_loop(void)
{
    char *line;
    char **args;
    int status;

    do{
        printf("-->");
        line=csh_read_line();
        args=csh_split_line(line);
        status=csh_execute(args);

        free(line);
        free(args);
    }while(status);
}

int main(int argc, char *argv[])
{
    //加载自定义配置开始

    //加载自定义配置结束

    csh_loop();     //运行循环命令

    return EXIT_SUCCESS;        //循环结束退出程序
}