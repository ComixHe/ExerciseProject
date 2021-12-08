#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

mode_t oct_pri(const char *mode);
mode_t char_pri(char *mode);
char *regular_string(char *string);
void mode_change(mode_t *new_mode,char *string);
void mode_append(mode_t *new_mode,const char *stringl,const char *stringr);
void mode_revoke(mode_t *new_mode,const char *stringl,const char *stringr);
void mode_reset(mode_t *new_mode,const char *stringl,const char *stringr);
int pri_analysis(const char *string);
int check_X(void);

mode_t privilege[3][8]={{0,S_IXUSR,S_IWUSR,S_IXUSR|S_IWUSR,S_IRUSR,S_IRUSR|S_IXUSR,S_IRUSR|S_IWUSR,S_IRUSR|S_IWUSR|S_IXUSR},
                        {0,S_IXGRP,S_IWGRP,S_IXGRP|S_IWGRP,S_IRGRP,S_IRGRP|S_IXGRP,S_IRGRP|S_IWGRP,S_IRGRP|S_IWGRP|S_IXGRP},
                        {0,S_IXOTH,S_IWOTH,S_IXOTH|S_IWOTH,S_IROTH,S_IROTH|S_IXOTH,S_IROTH|S_IWOTH,S_IROTH|S_IWOTH|S_IXOTH}};
mode_t old_mode=0;

int main(int argc,char *argv[])
{
  int file_count=0;
  char *files[argc-2];
  for(int i=2;i<argc;++i)
    files[i-2]=argv[i];
  if(argc<3)
  {
    fprintf(stderr,"need more info.");
    exit(EXIT_FAILURE);
  }
  while(file_count<argc-2)
  {
    char *file_path=files[file_count++];
    if(access(file_path,F_OK)==-1)
    {
      fprintf(stderr,"file or folder does not exist.");
      return errno;
    }
    struct stat buf;
    int ret=stat(file_path,&buf);
    if(ret==-1)
    {
      fprintf(stderr,"can't get file mode.");
      return errno;
    }
    mode_t new_mode;
    if(*argv[1]>='0'&&*argv[1]<='9')
      new_mode=oct_pri(argv[1]);
    else
    {
      old_mode=buf.st_mode;
      new_mode=char_pri(argv[1]);
    }
    ret=chmod(file_path,new_mode);
    if(ret==-1)
    {
      fprintf(stderr,"can't change privilege.");
      return errno;
    }
  }
  return 0;
}

mode_t oct_pri(const char *mode)
{
  int mode_int=atoi(mode);
  int user=mode_int/100;
  int group=(mode_int/10)%10;
  int others=mode_int%10;
  if(user>7||group>7||others>7||user<0||group<0||others<0){
    fprintf(stderr,"invalid privilege bit.\n");
    exit(EXIT_FAILURE);
  }
  mode_t new_mode=0;
  new_mode=privilege[0][user]|privilege[1][group]|privilege[2][others];
  return new_mode;
}

mode_t char_pri(char *mode)
{
  int i=0;
  char *string[4]={NULL};
  mode_t new_mode=old_mode;
  string[i]=strtok(mode,",");
  while(string[i]!=NULL)
  {
    if(i>3)
    {
      fprintf(stderr,"param length exceeds limit");
      exit(EXIT_FAILURE);
    }
    string[++i]=strtok(NULL,",");
  }
  i=0;
  while(string[i]!=NULL)
  {
    string[i]=regular_string(string[i]);
    if(string[i]==NULL||i>3)
    {
      fprintf(stderr,"invalid mode string");
      exit(EXIT_FAILURE);
    }
    mode_change(&new_mode,string[i]);
    free(string[i++]);
  }
  return new_mode;
}

void mode_change(mode_t *new_mode,char *string)
{
  char *split=string;
  int mal_flag=0;
  while(*split>='a'&&*split<='z')
    ++split;
  if(split==string)
  {
    char *tmp=(char *)malloc(strlen(string)+2);
    char *tmp_p=tmp;
    *(tmp_p++)='a';
    while(*string!='\0')
      *(tmp_p++)=*(string++);
    *(tmp_p+1)='\0';
    split=tmp+1;
    string=tmp;
    mal_flag=1;
  }
  switch(*split)
  {
    case '+':mode_append(new_mode,string,split); break;
    case '-':mode_revoke(new_mode,string,split); break;
    case '=':mode_reset(new_mode,string,split); break;
    default:fprintf(stderr,"invalid mode('+','-','=')"); exit(EXIT_FAILURE);
  }
  if(mal_flag)
    free(string);
}

void mode_append(mode_t *new_mode,const char *stringl,const char *stringr)
{
  int pri_flag=pri_analysis(stringr+1);
  while(stringl!=stringr)
  {
    switch(*stringl)
    {
      case 'u':*new_mode|=privilege[0][pri_flag]; break;
      case 'g':*new_mode|=privilege[1][pri_flag]; break;
      case 'o':*new_mode|=privilege[2][pri_flag]; break;
      case 'a':*new_mode|=(privilege[0][pri_flag]|privilege[1][pri_flag]|privilege[2][pri_flag]); return;
      default: fprintf(stderr,"role does not exist.\n"); exit(EXIT_FAILURE);
    }
    ++stringl;
  }
}

void mode_revoke(mode_t *new_mode,const char *stringl,const char *stringr)
{
  int pri_flag=pri_analysis(stringr+1);
  while(stringl!=stringr)
  {
    switch(*stringl)
    {
      case 'u':*new_mode&=(~privilege[0][pri_flag]); break;
      case 'g':*new_mode&=(~privilege[1][pri_flag]); break;
      case 'o':*new_mode&=(~privilege[2][pri_flag]); break;
      case 'a':*new_mode&=(~(privilege[0][pri_flag]|privilege[1][pri_flag]|privilege[2][pri_flag])); return;
      default: fprintf(stderr,"role does not exist.\n"); exit(EXIT_FAILURE);
    }
    ++stringl;
  }
}

void mode_reset(mode_t *new_mode,const char *stringl,const char *stringr)
{
  int pri_flag=pri_analysis(stringr+1);
  while(stringl!=stringr)
  {
    switch(*stringl)
    {
      case 'u':{
        *new_mode&=(~privilege[0][7]);
        *new_mode|=privilege[0][pri_flag];
      } break;
      case 'g':{
        *new_mode&=(~privilege[1][7]);
        *new_mode|=privilege[1][pri_flag];
      } break;
      case 'o':{
        *new_mode&=(~privilege[2][7]);
        *new_mode|=privilege[2][pri_flag];
      } break;
      case 'a':{
        *new_mode&=0;
        *new_mode|=(privilege[0][pri_flag]|privilege[1][pri_flag]|privilege[2][pri_flag]);
      } return;
      default: fprintf(stderr,"role does not exist.\n"); exit(EXIT_FAILURE);
    }
    ++stringl;
  }
}

int pri_analysis(const char *string)
{
  int pri=0;
  while(*string!='\0')
  {
    switch(*string)
    {
      case 'r':pri+=4; break;
      case 'w':pri+=2; break;
      case 'x':pri+=1; break;
      case 'X':{
          if(check_X())
            pri+=1;
          else
          {
            fprintf(stderr,"Does not match X permission.");
            exit(EXIT_FAILURE);
          }
        }break;
      default:fprintf(stderr,"invalid privilege flag %c.",*string); exit(EXIT_FAILURE);
    }
    ++string;
  }
  if(pri>7)
  {
    fprintf(stderr,"privilege overflow.");
    exit(EXIT_FAILURE);
  }
  return pri;
}

int check_X(void)
{
  if(S_ISDIR(old_mode)||old_mode&(unsigned short)1)
    return 1;
  return 0;
}

char* regular_string(char *string)
{
  short alphabet[26]={0};
  char *new_string=(char *)malloc(7*sizeof(char));
  char *string_point=new_string;
  while(*string>='a'&&*string<='z')
  {
    int ch=*string-97;
    if(ch<0||ch>=26)
      return NULL;
    if(alphabet[ch]==0)
    {
      *string_point=*string;
      alphabet[ch]=1;
      ++string_point;
    }
    ++string;
  }
  *(string_point++)=*(string++);
  while(*string<='a'&&*string>='z'&&*string!='X')
    ++string;
  memset(alphabet,0,26);
  while(*string!='\0')
  {
    int ch=0;
    if(*string>='a'&&*string<='z')
      ch=*string-97;
    else if(*string=='X')
      ch=23;
    else
      ch=-1;
    if(ch<0||ch>=26)
      return NULL;
    if(alphabet[ch]==0)
    {
      *string_point=*string;
      alphabet[ch]=1;
      ++string_point;
    }
    ++string;
  }
  *string_point='\0';
  return new_string;
}
