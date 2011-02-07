gcc -c -Wall -O6 -m486 *.cpp
ar -rv libgnugl.a *.o
move libgnugl.a c:\djgpp\lib\
copy gnugl.h c:\djgpp\include
del *.o
