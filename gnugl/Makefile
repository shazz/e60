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

_OBJ = face.o  file.o linepo.o  matrix.o  object.o  pcxbmp.o  point.o  primitiv.o  rest.o  scena.o  spline.o  sweeping.o  texture.o  zbuffer.o
OBJ = $(patsubst %,%,$(_OBJ))
CPPFLAGS = -Wall -O6

HEADERS=gnugl.h

%.o: %.cpp $(HEADERS)
	$(CXX) -c -o $@ $< $(CPPFLAGS)

libgnugl.a: $(OBJ)
	$(AR) -rv libgnugl.a $@ $^

.PHONY: clean all

clean:
	rm -f *.o *~ core $(INCDIR)/*~ 

all: libgnugl.a

