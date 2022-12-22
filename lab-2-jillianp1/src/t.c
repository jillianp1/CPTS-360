#include <stdio.h>              // for I/O
#include <stdlib.h>             // for exit()
#include <libgen.h>             // for dirname()/basename()
#include <string.h>             // for string functions

typedef struct node{
        char  name[64];         // node's name string
        char  type;             // type = 'D' or 'F'  
   struct node *child, *sibling, *parent;
}NODE;


NODE *root, *cwd, *start;       // root->/, cwd->CWD, start->start_node
char line[128];                 // user input line
char command[16], pathname[64]; // command pathname strings

// you need these for dividing pathname into token strings
char gpath[128];                // token string holder
char *name[32];                 // token string pointers
int  n;                         // number of token strings

//               0       1      2       3         4       5     6     7
char *cmd[] = {"mkdir", "ls", "quit", "rmdir", "creat", "cd", "pwd", "rm", 0};

int tokenize(char *pathname)
{
  int i;
  char *s;
  strcpy(gpath, pathname);
  printf("gpath=%s\n", gpath); 
  n = 0;
  s = strtok(gpath, "/"); //first call to strtok)
  while (s)
  {
    name[n++] = s;
    s = strtok(0, "/"); // successive calls to strtok()
  }
  for (i = 0; i < n; i++)
  {
    printf("name[%d]=%s\n", i, name[i]);
  }
  printf("gpath=%s\n", gpath);
}

int findCmd(char *command)
{
   int i = 0;
   while(cmd[i]){
     if (strcmp(command, cmd[i])==0)
         return i;
     i++;
   }
   return -1;
}

NODE *search_child(NODE *parent, char *name)
{
  NODE *p;
  printf("search for %s in parent DIR %s\n", name, parent->name);
  p = parent->child;
  if (p==0)
    return 0;
  while(p){
    if (strcmp(p->name, name)==0)
      return p;
    p = p->sibling;
  }
  return 0;
}

//returns a pointer to the node of pathname
NODE *path2node(char *pathname)
{
  NODE *start, *p;
  int i; 
  if (pathname[0]=='/')
    start = root;
  else
    start = cwd;
  //if its . return cwd 
  if(strcmp(pathname, ".")==0)
  {
      return cwd;
  }
  tokenize(pathname);
  p = start;
  for (i=0; i < n; i++)
  {
    if(strcmp(name[i], "..")==0)
    {
        p = p->parent;
    }
    else
    {
       p = search_child(p, name[i]);
    }
    printf("name[i]= %s\n", name[i]);
    if (p==0)
    { 
        printf("error in search child in path2node\n");
        return 0;
    }
  }
  return p;
}			 

int insert_child(NODE *parent, NODE *q)
{
  NODE *p;
  printf("insert NODE %s into parent's child list\n", q->name);
  p = parent->child;
  if (p==0)
    parent->child = q;
  else{
    while(p->sibling)
      p = p->sibling;
    
    p->sibling = q;
  }
  q->parent = parent;
  q->child = 0;
  q->sibling = 0;
}

/***************************************************************
 This mkdir(char *name) makes a DIR in the current directory
 You MUST improve it to mkdir(char *pathname) for ANY pathname
****************************************************************/
int mkdir(char *pathname)
{
  NODE *p, *q;
  printf("mkdir: pathname=%s\n", pathname);
  if (!strcmp(pathname, "/") || !strcmp(pathname, ".") || !strcmp(pathname, "..")){
    printf("can't mkdir with %s\n", pathname);
    return -1;
  }
  char temp[100];
  char temp2[100];
  char dirName[100];
  char base[100];
  //get basename
  strcpy(temp2, pathname);
  printf("temp = %s\n", temp2);
  strcpy(base, basename(temp2));
  printf("basename = %s\n", base);
  //get dir name
  strcpy(temp, pathname);
  printf("temp = %s\n", temp);
  strcpy(dirName, dirname(temp));
  printf("dirname = %s\n", dirName);
  //search for dir name node 
  p = path2node(dirName);
  //if exist but not dir error message return fail
  if (p)
  {
      printf("path exists\n");
      if(p->type != 'D')
      {
          printf("error type not D Failed\n");
          return -1;
      }
  // if exist and is dir
      else
      {
          //search for base name in (under) dir name node 
          q = search_child(p, base);
          // if already exists error
          if(q)
          {
              printf("error path already exists\n");
              return -1;
          }
          //else does not exist add a new dir
          else
          {
              printf("--------------------------------------\n");
              printf("ready to mkdir %s\n", pathname);
              q = (NODE *)malloc(sizeof(NODE));
              q->type = 'D';
              strcpy(q->name, base);
              insert_child(p, q);
              printf("mkdir %s OK\n", pathname);
              printf("--------------------------------------\n");
              return 0;
          }
      }
    return 0;
  }
  //if nonexist error message return Fail
  else
  {
      printf("error pathname does not exist\n");
      return -1;
  }
  return 0;
}

int rmdir(char *pathname)
{
    NODE *p;
    NODE *q;
    //search for pathname node 
    p = path2node(pathname);
    //if pathname exists 
    if(p)
    {
      //check its dir type
        if(p->type == 'D')
        {
            //check if empty meaning has no children
            if(p->child)
            {
                printf("Dir not empty cannot remove\n");
                return -1;
            }
            else
            {
              printf("Dir is empty\n");
              printf("--------------------------------------\n");
              printf("ready to remove dir\n");
              NODE *curr;
              NODE *prev;
              q = p->parent; 
              if(q->child == p)
              {
                  q->child = p->sibling;
                  free(p);
                  return 0;
              }
              curr = q->child; //starts at sib 1
              prev = q->child;
              while(curr)
              {
                prev = curr;
                curr = curr->sibling;
                if(curr==p)
                {
                    break;
                }
              }
              prev->sibling = curr->sibling;
              free(curr);
              printf("rmdir OK\n");
              printf("--------------------------------------\n");
              return 0;
            }
        }
        else
        {
          printf("error not D type\n");
          return -1;
        }
    }
    else
    {
        printf("error path2node does not exist\n");
        return -1;
    }
    return 0;
}

int creat (char *pathname)
{
  NODE *p, *q;
  printf("creat: pathname=%s\n", pathname);
  if (!strcmp(pathname, "/") || !strcmp(pathname, ".") || !strcmp(pathname, "..")){
    printf("can't creat with %s\n", pathname);
    return -1;
  }
  char temp[100];
  char temp2[100];
  char dirName[100];
  char base[100];
  //get basename
  strcpy(temp2, pathname);
  printf("temp = %s\n", temp2);
  strcpy(base, basename(temp2));
  printf("basename = %s\n", base);
  //get dir name
  strcpy(temp, pathname);
  printf("temp = %s\n", temp);
  strcpy(dirName, dirname(temp));
  printf("dirname = %s\n", dirName);
  //search for dir name node 
  p = path2node(dirName);
  //if exist but not dir error message return fail
  if (p)
  {
      printf("path exists\n");
      printf("type %c\n", p->type);
      if(p->type != 'D')
      {
          printf("error type not F Failed\n");
          return -1;
      }
  // if exist and is dir
      else
      {
          //search for base name in (under) dir name node 
          q = search_child(p, base);
          // if already exists error
          if(q)
          {
              printf("error path already exists\n");
              return -1;
          }
          //else does not exist add a new dir
          else
          {
              printf("--------------------------------------\n");
              printf("ready to creat %s\n", pathname);
              q = (NODE *)malloc(sizeof(NODE));
              q->type = 'F';
              strcpy(q->name, base);
              insert_child(p, q);
              printf("creat %s OK\n", pathname);
              printf("--------------------------------------\n");
              return 0;
          }
      }
    return 0;
  }
  //if nonexist error message return Fail
  else
  {
      printf("error pathname does not exist\n");
      return -1;
  }
  return 0;
}

int cd(char *pathname)
{
    NODE *p;
    p = path2node(pathname);
    //path exists 
    if(p)
    {
      //check its dir
      if (p->type == 'D')
      {
        //change cwd to point at dir
        cwd = p;
        return 0;
      }
      //not dir type
      else
      {
        printf("error type not D\n");
        return -1;
      }
    }
    else
    {
      printf("path2node fail\n");
    }
    return 0;
}

//remove a file 
int rm (char *pathname)
{
    NODE *p;
    NODE *q;
    p = path2node(pathname);
    //if pathname exists 
    if(p)
    { 
      //check its file type
        if(p->type == 'F')
        {
              printf("--------------------------------------\n");
              printf("ready to remove file\n");
              NODE *curr;
              NODE *prev;
              q = p->parent; 
              if(q->child == p)
              {
                  q->child = p->sibling;
                  printf("rm OK\n");
                  free(p);
                  return 0;
              }
              curr = q->child; //starts at sib 1
              prev = q->child;
              //p is what i want to delete 
              // if the dir to delete is the last one in pathname its the base name 
              while(curr)
              {
                //prev = curr sibling
                prev = curr;
                curr = curr->sibling;
                if(curr==p)
                {
                    break;
                }
              }
              prev->sibling = curr->sibling;
              printf("rm OK\n");
              printf("--------------------------------------\n");
              free(curr);
              return 0;
          }
          else
          {
            printf("error not F type\n");
            return -1;
          }
    }
    else
    {
        printf("error path2node does not exist\n");
        return -1;
    }
    return 0;
}

// This ls() list CWD. You MUST improve it to ls(char *pathname)
// shows current nodes children
int ls(char *pathname)
{
  NODE *p;
  if(pathname[0] == 0)
  {
      p = cwd;
  }
  else
  {
     p = path2node(pathname);
     printf("p name = %s\n", p->name);
  }
  if(p)
  {
    p = p->child;
    printf("--------------------------------------\n");
    printf("contents = ");
    while(p){
      printf("[%c %s] ", p->type, p->name);
      p = p->sibling;
    }
    printf("\n");
  }
  return 0;
}

//print working directory helper recursion function 
void pwdrecur(NODE *p)
{    
    if(p == root)
    {
      return;
    }
    pwdrecur(p->parent);
    printf("/%s", p->name);
}

//print working directory 
void pwd()
{
    NODE *p;
    p = cwd;
    printf("cwd = ");
    pwdrecur(p);
    if (p == root)
    {
        printf("/");
    }
    printf("\n");
}

int quit()
{
  printf("Program exit\n");
  exit(0);
}

int initialize()
{
    root = (NODE *)malloc(sizeof(NODE));
    strcpy(root->name, "/");
    root->parent = root;
    root->sibling = 0;
    root->child = 0;
    root->type = 'D';
    cwd = root;
    printf("Root initialized OK\n");
}

int main()
{
  int index;

  initialize();

  while(1){
     printf("Enter command line [mkdir|ls|quit|rmdir|creat|cd|pwd|rm] : ");
     fgets(line, 128, stdin);     // get input line
     line[strlen(line)-1] = 0;    // kill \n at end
     
     strcpy(pathname, "");
     sscanf(line, "%s %s", command, pathname); // extract command pathname
     printf("command=%s pathname=%s\n", command, pathname);
      
     if (command[0]==0) 
         continue;

     index = findCmd(command);

     switch (index){
        case 0: mkdir(pathname); break;
        case 1: ls(pathname);    break;
        case 2: quit();          break;
        case 3: rmdir(pathname); break;
        case 4: creat(pathname); break;
        case 5: cd(pathname);    break;
        case 6: pwd();           break;
        case 7: rm(pathname);    break;
     }
  }
}
