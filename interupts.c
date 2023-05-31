#include <cx16.h>
#include <conio.h>
#include <stdio.h>
#include <stdint.h>
#include <6502.h>


// program to test interupt call back function
// output for vsync should be 1x, 2x 3x ...

// cl65 -t cx16 -O -o interupts.prg interupts.c


unsigned char TempStack[64];
uint8_t i=0;


unsigned char IRQ()
{

    // test for VSYNC flag
    if ((VERA.irq_flags & VERA_IRQ_VSYNC) == VERA_IRQ_VSYNC) {
        // first clear the vsync flag
        VERA.irq_flags = VERA.irq_flags | VERA_IRQ_VSYNC;
        
        // then do other things
        i++;
        printf("%d", i);
        if (i==9) i=0;
        
        
        return IRQ_HANDLED; 
             
    } else if ((VERA.irq_flags & VERA_IRQ_RASTER) == VERA_IRQ_RASTER) {
        
        VERA.irq_flags = VERA.irq_flags | VERA_IRQ_RASTER;
        
        printf("r");
        
        return IRQ_HANDLED; 
            
    } else if ((VERA.irq_flags & VERA_IRQ_SPR_COLL) == VERA_IRQ_SPR_COLL) {
        
        VERA.irq_flags = VERA.irq_flags | VERA_IRQ_SPR_COLL;
        
        printf("s");
        
        return IRQ_HANDLED;
    }
    if ((VERA.irq_flags & VERA_IRQ_AUDIO_LOW) == VERA_IRQ_AUDIO_LOW) {
        
        // fill audio FIFO at least 25% to clear flag
        
        printf("a");
        
        return IRQ_HANDLED;
        
    }
    
    return IRQ_NOT_HANDLED;
}
    

int main()
{
    SEI();
    set_irq(&IRQ, TempStack, 64);
    CLI();
    
    // print out the current status of the vsync flag
    if ((VERA.irq_flags & VERA_IRQ_VSYNC) == VERA_IRQ_VSYNC) {
        printf("1");
    } else {
        printf("0");
    }
    
    while (1)
    {
        waitvsync();
        
        printf("x");
        
    }
    
    return 0;
}
