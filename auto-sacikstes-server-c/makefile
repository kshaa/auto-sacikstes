PROG = autosacikstes-server
CC = gcc
OBJS = main.o sockets.o traffic.o routes/ping.o routes/error.o
CFLAGS =  

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
