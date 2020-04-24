PROG = autosacikstes
CC = gcc
OBJS = init.o input.o main.o scene.o state.o 
OBJS += queries/game.o networking/socket.o queries/ping.o
OBJS += ../common/networking/translate.o
OBJS += ../common/networking/fetch.o ../common/protocol/protocol_messages.o
LIBS = -l SDL2
CFLAGS = ${LIBS} ${INCLUDES}

# Make actions
all: ${PROG}
clean:
	find ../ -type f -name '*.o' -exec rm {} + 
	rm -f $(PROG)

# Compile object files
%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS) 
	
# linking the program.
${PROG}: $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS)  
