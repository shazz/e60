CFLAGS=-g

PATH=/usr/local/arm/4.2.2-eabi/usr/bin:$PATH
CC=arm-linux-gcc
CXX=arm-linux-c++
LD=arm-linux-ld
AR=arm-linux-ar
AS=arm-linux-as
NM=arm-linux-nm
OBJDUMP=arm-linux-objdump
STRIP=arm-linux-strip

_OBJ = demo.o
OBJ = $(patsubst %,%,$(_OBJ))
LIBS = -L../gnugl -lgnugl
CPPFLAGS = -Wall -O6 -I..

HEADERS=s3c_epd_userland.h ../gnugl/gnugl.h

%.o: %.cpp $(HEADERS)
	$(CXX) -c -o $@ $< $(CPPFLAGS)

demo: $(OBJ)
	$(CXX) -o $@ $^ $(CPPFLAGS) $(LIBS)

.PHONY: clean all

clean:
	rm -f *.o *~ core $(INCDIR)/*~ 

all: demo

