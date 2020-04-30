PROG = autosacikstes-server
CC = gcc
OBJS = main.o router.o networking/socket.o state.o routes/lobby.o logic/lobby.o
OBJS += routes/ping.o routes/error.o ../common/networking/translate.o
OBJS += ../common/protocol/protocol_messages.o ../common/networking/fetch.o
OBJS += ../common/logic/math.o
CFLAGS = -g

# Make actions
all: ${PROG}
clean:
	find ../ -type f -name '*.o' -exec rm {} + 
	rm -f $(PROG)

# Compile object files
%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS) 
	
# Linking the program.
${PROG}: $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS)
