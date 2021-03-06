# Design by Rocky
# Compiler all .c source

COBJS	+= main.o socket.o parser.o
COBJS	+= http.o downloader.o
COBJS	+= cJSON.o jsonbuf.o weatherData.o
COBJS	+= getAK.o cgic.o

CFLAGS	+= -O2 -Wall
CFLAGS	+= -I./inc
CFLAGS	+= -DDEBUG

LDFLAGS	+= -lm

CROSS_COMPILE	?= /opt/FriendlyARM/toolschain/4.5.1/bin/arm-linux-

CC	= $(CROSS_COMPILE)gcc

TARGET = getWeather.cgi

all:$(TARGET)
$(TARGET):$(COBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
.PHONY:clean
clean:
	rm -f $(COBJS) $(TARGET)
