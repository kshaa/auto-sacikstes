PROG = autosacikstes
CC = gcc
OBJS = draw.o init.o input.o main.o 
LIBS = -l SDL2 -l SDL2_image
CFLAGS = ${LIBS} ${INCLUDES}

# Make actions
all: ${PROG}
clean:
	rm -rf *.o $(PROG)

# Compile object files
%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS) 
	
# linking the program.
${PROG}: $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS)  
