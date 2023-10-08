CC = gcc
CFLAGS = -g -Wall -std=gnu99
LDFLAGS = -L.
AR = ar
ARFLAGS = rcs

TARGET = run

# List of source files
SRCS = ls.c library.c

# Generate a list of object files from source files
OBJS = $(SRCS:.c=.o)

# Rules to build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

# Rules to compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean up
clean:
	rm -f $(TARGET) $(OBJS)

