// program to demo a moving animated sprite


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <cbm.h>
#include <time.h>

#include <vload.h>



void fileNotFoundError(char *filename)
{
  // change VERA config so the error message can be displayed
  VERA.layer1.config    = 0b01100000;
  VERA.layer1.tilebase  = 0b11111000;

  bgcolor(COLOR_BLUE);
  textcolor(COLOR_WHITE);
  videomode(VIDEOMODE_80COL);

  printf("error, %s file not found", filename);
  printf("\nprogram exiting, cannot continue");
}

int main(void)
{
    signed char DeltaX=1;
    signed char DeltaY=-1;
    uint16_t SpriteX;
    uint16_t SpriteY;
    uint16_t spriteIndex=0;
    uint16_t size;
    bool update=false;
    time_t t;

    // switch back to the uppercase character set
    cbm_k_bsout(CH_FONT_UPPER);
    
    // change video mode 320x240
    videomode(VIDEOMODE_40COL);
    
    // intialize random number generator
    srand((unsigned) time(&t));
    
    // generate random start position
    SpriteX = rand() % 287;
    SpriteY = rand() % 207;
    
    // load the sprite into the VERA
    size = vload_host("sprite.bin",0x04000);
    if (size==0) {
      fileNotFoundError("sprite.bin");
      return 1;
    }
    
    // VERA sprite config
    VERA.address_hi = 0x11;                     // inc by 1, memory start at 0x10000
    VERA.address = 0xFC00;                      // 1st sprite register at 0x1FC00
    VERA.data0 = 0x0000;                        // 1st sprite memory address
    VERA.data0 = 0b10000010;                    // 256 color mode address at 0x04000
    VERA.data0 = SpriteX & 0xFF;                // X position
    VERA.data0 = SpriteX >> 8;                  // X position
    VERA.data0 = SpriteY & 0xFF;                // Y position
    VERA.data0 = SpriteY >> 8;                  // Y position
    VERA.data0 = 0b00001100;                    // in front
    VERA.data0 = 0b10100000;                    // 32x32 pixel pallette offset 0
    

    // enable sprites
    vera_sprites_enable(true);
    
    puts("\npress any key to stop.");
    do {
        // wait until the start of the next video frame
        waitvsync();
    
        if (DeltaX == 1 && SpriteX == 287) DeltaX = -1;
        if (DeltaX == -1 && SpriteX == 0) DeltaX = 1;
            
        SpriteX += DeltaX;
            
        if (DeltaY == 1 && SpriteY == 207) DeltaY = -1;
        if (DeltaY == -1 && SpriteY == 0) DeltaY = 1;
            
        SpriteY += DeltaY;
        
        // animate the sprite
        if (update=!update) {   // every other vsync
            spriteIndex=spriteIndex+32;
            if (spriteIndex==256) spriteIndex=0;
        }
    
        // move the sprite
        VERA.address = 0xFC00;
        VERA.data0 = spriteIndex;
        VERA.data0 = 0b10000010;
        VERA.data0 = SpriteX & 0xFF;
        VERA.data0 = SpriteX >> 8;
        VERA.data0 = SpriteY & 0xFF;
        VERA.data0 = SpriteY >> 8;                  
            
             
        
    } while (!kbhit());
    cgetc();

    // disable sprites
    vera_sprites_enable(false);
    
    videomode(VIDEOMODE_80COL);
    
    
    return EXIT_SUCCESS;
    
}
