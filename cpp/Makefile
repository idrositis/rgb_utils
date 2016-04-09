#
#  RGB Manipulation Utilities
#  Simple makefile for compiling with EasyBMP library
#
# TODOs
# - reduce the (default) verbosity
#   http://stackoverflow.com/questions/9314322/controlling-verbosity-of-make
#
CC = g++
EASYBMP_PATH = ./EasyBMP
C_INCLUDE_PATH = $(EASYBMP_PATH)

# Safe optimization flags
CFLAGS = -O3 -pipe -fomit-frame-pointer -s -funroll-all-loops -I $(C_INCLUDE_PATH)

#all: EasyBMP.o rgb2cmyk.cpp rgb2yuv.cpp rgb_split.cpp
#	$(CC) $(CFLAGS) EasyBMP.o rgb2cmyk.cpp 

all: rgb2cmyk rgb2yuv rgb_split clean

rgb2cmyk: rgb2cmyk.o EasyBMP.o
	$(CC) $(CFLAGS) rgb2cmyk.o EasyBMP.o -o rgb2cmyk
	/bin/chmod 755 rgb2cmyk

rgb2yuv: rgb2yuv.o EasyBMP.o
	$(CC) $(CFLAGS) rgb2yuv.o EasyBMP.o -o rgb2yuv
	/bin/chmod 755 rgb2yuv

rgb_split: rgb_split.o EasyBMP.o
	$(CC) $(CFLAGS) rgb_split.o EasyBMP.o -o rgb_split
	/bin/chmod 755 rgb_split

rgb2cmyk.o: rgb2cmyk.cpp $(EASYBMP_PATH)/EasyBMP.h
	$(CC) $(CFLAGS) -c rgb2cmyk.cpp

rgb2yuv.o: rgb2yuv.cpp $(EASYBMP_PATH)/EasyBMP.h
	$(CC) $(CFLAGS) -c rgb2yuv.cpp

rgb_split.o: rgb_split.cpp $(EASYBMP_PATH)/EasyBMP.h
	$(CC) $(CFLAGS) -c rgb_split.cpp

EasyBMP.o: $(EASYBMP_PATH)/EasyBMP.cpp $(EASYBMP_PATH)/EasyBMP*.h
	$(CC) $(CFLAGS) -c EasyBMP/EasyBMP.cpp -o EasyBMP.o

clean: 
	@echo "Cleaning up..."
	/bin/rm -f ./*.o $(EASYBMP_PATH)/*.o
