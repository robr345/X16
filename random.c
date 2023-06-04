#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// emulator add -rtc to command line
// time function takes about 1.6k of memory


int main()
{

    int i, n;
    time_t t;
    
    n = 5;
    
    // intialize random number generator
    srand((unsigned) time(&t));
    
    // print 5 random numbers from 0 to 49
    for (i=0; i<n; i++) {
        printf("%d\n", rand() % 255);
    }


    return EXIT_SUCCESS;

}
