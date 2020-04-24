PROG = autosacikstes
CC = gcc
OBJS = init.o input.o main.o scene.o state.o queries/game.o
OBJS += networking/translate.o networking/socket.o
OBJS += ../common/fetch.o ../common/protocol_messages.o
LIBS = -l SDL2
CFLAGS = ${LIBS} ${INCLUDES}

# Make actions
all: ${PROG}
clean:
	rm -rf *.o networking/*.o ../common/*.o $(PROG)

# Compile object files
%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS) 
	
# linking the program.
${PROG}: $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS)  
