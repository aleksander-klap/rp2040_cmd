CC := gcc
CFLAGS := -Wall -Wextra -O2 -std=c11

all: crc_calc

crc_calc: mpeg2_crc32.o main.o 
	$(CC) $(CFLAGS) mpeg2_crc32.o main.o -o crc_calc

mpeg2_crc32.o: mpeg2_crc32.c mpeg2_crc32.h
	$(CC) $(CFLAGS) -c mpeg2_crc32.c -o mpeg2_crc32.o

main.o: main.c mpeg2_crc32.h
	$(CC) $(CFLAGS) -c main.c -o main.o

clean:
	rm -f main.o mpeg2_crc32.o crc_calc



