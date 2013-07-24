CC = gcc
CFLAGS = `pkg-config --cflags gio-2.0 gio-unix-2.0`
LDFLAGS = `pkg-config --libs gio-2.0 gio-unix-2.0`
SRCS = lineup-parameters.c
OBJS = $(SRCS:.c=.o)
EXECUTABLE = lineup-parameters

.PHONY: clean

$(EXECUTABLE): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(EXECUTABLE)

all: $(EXECUTABLE)

clean:
	rm -f $(OBJS) $(EXECUTABLE)
