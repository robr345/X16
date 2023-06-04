#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// emulator - add -rtc to command line


int main()
{
    time_t seconds;
    
    time(&seconds);
    printf("Seconds since January 1, 1970 = %ld\n", seconds);

    return EXIT_SUCCESS;

}
