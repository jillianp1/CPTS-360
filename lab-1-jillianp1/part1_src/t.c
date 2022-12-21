#include <stdio.h>
#include <stdlib.h>

typedef unsigned int u32;

char *ctable = "0123456789ABCDEF";
int  BASE = 10; 

// please put your main and code in this file. All contained.
// Use the provided mk script to compile your code. You are welcome to change the mk script if you'd like
// but know that is how we will be batch compiling code using mk on the back end so your code will need to be able
// to run when that bash script is ran.
// Most code can be grabbed from the .html file in the root.

int main(int argc, char *argv[ ], char *env[ ])
{
    printf("Print argc, argv, env: \n");
    myprintf("argc = %d\n", argc);
    for (int i = 0; i < argc; i++)
    {
        myprintf("argv[%d] = %s\n", i, argv[i]);
    }
    for (int i = 0; i<argc; i++)
    {
        myprintf("env[%d] = %s\n", i, env[i]);
    }
    printf("Testing my printf:\n");
    myprintf("char=%c string=%s       dec=%u  hex=%x neg=%d\n", 
	        'A', "this is a test",  100,   100,   -100);
}

//write function to print a string 
prints(char *s)
{
    while(*s != '\0')
    {
        putchar(*s);
        s++;
    }
}

int rpu(u32 x)
{  
    char c;
    if (x){
       c = ctable[x % BASE];   
       rpu(x / BASE);
       putchar(c);           
    }
}
//prints unsigned int
int printu(u32 x)
{
   (x==0)? putchar('0') : rpu(x);
   putchar(' ');
}

//prints in hex
int printx(u32 x) //which print x in HEX   (prefixed with 0x )
{
    BASE = 16;
    (x==0)? putchar('0') : rpu(x);
    putchar(' ');
}

//prints an int
int printd(int x)
{
    if (x >= 0)
    {
        printu(x);
    }
    else if(x < 0)
    {
        // just add '-'
        putchar('-');
        rpu(-1*x);
    }
}

void myprintf(char *fmt, ...)
{
    char * cp = fmt;
    int * ip = (int *)&fmt; //address of fmt + 1
    ip++;
    while(*cp != '\0')
    {
        if(*cp == '%')
        {
            cp++;
            switch(*cp)
            {
                case 'c':
                    putchar(*ip);
                    break;

                case 's':
                    prints(*ip);
                    break;

                case 'u':
                    printu(*ip);
                    break;

                case 'x':
                    printx(*ip);
                    break;

                case 'd':
                    printd(*ip);
                    break;
            }
            ip++;
        }
        else if (*cp == '\n')
        {
            putchar('\n');
            putchar('\r');
        }
        else 
        {
            putchar(*cp);
        }
        cp++;
    }
}





