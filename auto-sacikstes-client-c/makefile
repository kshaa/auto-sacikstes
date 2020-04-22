PROG = autosacikstes
CC = gcc
OBJS = init.o input.o main.o scene.o state.o
LIBS = -l SDL2
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
