CFLAGS=-Os
#CC=diet -Os gcc
CC=gcc
INCLUDES=-I.
LIBS=-Liksemel -Llibowfat -liksemel -lowfat

SHA1_SRC=$(wildcard sha1/*.c)
SHA1_OBJ=$(SHA1_SRC:.c=.o)

STACK_SRC=$(wildcard stack/*.c)
STACK_OBJ=$(STACK_SRC:.c=.o)

MW_SRC=$(wildcard mw/*.c)
MW_OBJ=$(MW_SRC:.c=.o)

default: xml3


.c.o:
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

sha1sum.o: sha1sum.c

sha1sum: sha1sum.o $(SHA1_OBJ)
	$(CC) -o $@ $< $(SHA1_OBJ) $(LIBS)


xml3.o: xml3.c

xml3: xml3.o $(MW_OBJ) $(STACK_OBJ)
	$(CC) -o $@ $< $(STACK_OBJ) $(MW_OBJ) $(LIBS)

clean:
	rm -f *.o xml3 sha1sum $(SHA1_OBJ) $(STACK_OBJ) $(MW_OBJ)
