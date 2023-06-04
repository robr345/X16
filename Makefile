.PHONY: all clean

LIBRARIES := -L ./lib
INCLUDE := -I ./include

all: INTERUPTS.PRG SPRITE.PRG BALL.PRG SECONDS.PRG RANDOM.PRG

INTERUPTS.PRG: interupts.c
	cl65 -t cx16 -O -o INTERUPTS.PRG $(INCLUDE) $(LIBRARIES) interupts.c libX16.lib cx16.lib

SPRITE.PRG: sprite.c
	cl65 -t cx16 -O -o SPRITE.PRG $(INCLUDE) $(LIBRARIES) sprite.c libX16.lib cx16.lib
	
BALL.PRG: ball.c
	cl65 -t cx16 -O -o BALL.PRG $(INCLUDE) $(LIBRARIES) ball.c libX16.lib cx16.lib
	
SECONDS.PRG: seconds.c
	cl65 -t cx16 -O -o SECONDS.PRG seconds.c
	
RANDOM.PRG: random.c
	cl65 -t cx16 -O -o RANDOM.PRG random.c

clean:
	rm -r *.PRG
