PROG = autosacikstes-server
CC = gcc
OBJS = main.o socket.o router.o
OBJS += routes/ping.o routes/error.o
OBJS += ../common/protocol_messages.o ../common/fetch.o
CFLAGS = 

# Make actions
all: ${PROG}
clean:
	rm -rf *.o routes/*.o ../common/*.o $(PROG)

# Compile object files
%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS) 
	
# linking the program.
${PROG}: $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS)
