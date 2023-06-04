// program to demo moving sprites


#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <conio.h>
#include <cbm.h>

#include <vload.h>


#define SPRITE_COUNT 16
#define SPRITE_X_SPACING 30
#define SPRITE_X_START 20


static const int sin[] = {
    0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 29, 32, 34, 36, 39, 40, 42,
    44, 45, 46, 48, 48, 49, 50, 50, 50, 50, 50, 49, 48, 48, 46, 45,
    44, 42, 40, 39, 36, 34, 32, 29, 27, 24, 21, 18, 15, 12, 9, 6, 3,
    0, -2, -5, -8, -11, -14, -17, -20, -23, -26, -28, -31, -33, -35,
    -38, -39, -41, -43, -44, -45, -47, -47, -48, -49, -49, -49, -49,
    -49, -48, -47, -47, -45, -44, -43, -41, -39, -38, -35, -33, -31,
    -28, -26, -23, -20, -17, -14, -11, -8, -5, -2
};

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
    uint16_t i, x;
    uint8_t j, ofs = 0;
    uint16_t size;

    // Switch back to the uppercase character set.
    cbm_k_bsout(CH_FONT_UPPER);
    
    size = vload_host("balloon.bin",0x04000);
    if (size==0) {
      fileNotFoundError("balloon.bin");
      return 1;
    }
    
    VERA.address_hi = 0x11;   // inc by 1, memory start at 0x10000

    // Initialize the sprites.
    for (i = 0; i < SPRITE_COUNT; i++) {
        x = i * SPRITE_X_SPACING + SPRITE_X_START;
        
        VERA.address = 0xFC00 + i * 8;      // 1st sprite at address 0x1FC00
        VERA.data0 = 0x0000;                // %00000000
        VERA.data0 = 0b10000010;            // 256 color mode address at 0x04000
        VERA.data0 = x & 0xFF;              // X position
        VERA.data0 = x >> 8;                // X position
        VERA.data0 = 0 & 0xFF;              // Y position
        VERA.data0 = 0;                     // Y position
        VERA.data0 = 0b00001100;            // in front
        VERA.data0 = 0b11110000;            // 64x64 pixel pallette offset 0
        
    }

    // Enable the sprites.
    vera_sprites_enable(true);

    // Animate those sprites.
    puts("\npress any key to stop.");
    do {
        j = ofs;

        // Wait until the start of the next video frame.
        waitvsync();

        // Update the sprites' y co-ordinates.
        for (i = 0; i < SPRITE_COUNT * 8; i += 8) {
            vpoke(80 + sin[j], 0x1FC04 + i);
            j += 4;
            if (j > 99)
                j -= 100;
        }

        // Update the start offset.
        if (++ofs == 100)
            ofs = 0;
    } while (!kbhit());
    cgetc();

    // Disable the sprites.
    vera_sprites_enable(false);
}
