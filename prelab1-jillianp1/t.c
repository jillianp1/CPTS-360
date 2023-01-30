#include <stdio.h>
#include <stdlib.h>

int *FP;

int main(int argc, char *argv[ ], char *env[ ])
{
    int a,b,c;
    printf("enter main\n");
    printf("&argc=%x argv=%x env=%x\n", &argc, argv, env);
    printf("&a=%8x &b=%8x &c=%8x\n", &a, &b, &c);

//(1). Write C code to print values of argc and argv[] entries
    printf("argc= %d", argc);
    int i;
    for(i=0; i<argc; i++)
    {
        printf("argv[%d] = %d\n", i, argv[i]);
    }
    
    a=1; b=2; c=3;
    A(a,b);
    printf("exit main\n");
}
	      
int A(int x, int y)
{ 
    int d,e,f; 
    printf("enter A\n");
    // write C code to PRINT ADDRESS OF d, e, f
    //printf("&d = %p\n", &d);
    printf("&d=%x &e=%x &f=%x\n", &d, &e, &f);
    d=4; e=5; f=6;
    B(d,e);
    printf("exit A\n");
}

int B(int x, int y)
{
    int g,h,i;
    printf("enter B\n");
    // write C code to PRINT ADDRESS OF g,h,i
    printf("&g=%x &h=%x &i=%x\n", &g, &h, &i);
    g=7; h=8; i=9;
    C(g,h);
    printf("exit B\n");
}

int C(int x, int y)
{
    int u, v, w, i, *p;
    printf("enter C\n");
    // write C cdoe to PRINT ADDRESS OF u,v,w,i,p;
    printf("&u=%x &v=%x &w=%x &i=%x &p=%x\n", &u, &v, &w, &i, p);
    printf("\n");
    u=10; v=11; w=12; i=13;
//======================================================================
    FP = (int *)getebp();  // FP = stack frame pointer of the C() function

  // FP is a 32-bit ADDRESS. Write C code to print its value in HEX 

  // (2). Write C code to print the stack frame link list as

  // FP -> previousFP -> previousFP -> .... -> NULL      (all in Hex)

    printf("FP = ebp = %x\n", FP);
    printf("Stack frame linked list\n");
    p = (int *)FP;
    while (p)
    {
      printf("%8x -> ", p);
      p = *p;
    }
    printf("\n");

//(3). Let p = &u;
     //Print the stack contents from p to the frame of main().
     //YOU MAY JUST PRINT 128 entries of the stack contents.

    printf("\nStack contents\n");
    p = &u;
    i = 0;
    while(i < 128)
    {
        printf("%8x %x\n", p, *p);
        p++;
        i++;
    }

/*
(4). On a hard copy of the print out, identify the stack contents
     as LOCAL VARIABLES, PARAMETERS, stack frame pointer of each function.
     */

}

   