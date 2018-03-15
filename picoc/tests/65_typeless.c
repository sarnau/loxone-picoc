#include "stdio.h"

void main()
{
    float x = 3.9;
    long y = 4;
    long *z = &y;
    char *msg = "hi there";
	int i;

    printf("%d %d %d %s\n", (int) (x*2), y*2, *z, msg);
    printf("%d %d %d\n", sizeof(x), sizeof(y), sizeof(msg));
    for (i = 1; i <= 3; i++)
        printf("%d\n", i);

    /* this should fail
    { 
        int q = 5;
    }
    q = 3.14; // should say error
    */
}
