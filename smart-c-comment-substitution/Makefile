CC = gcc
CFLAGS = -Wall -g `pkg-config --cflags gtksourceview-3.0`
LDFLAGS = `pkg-config --libs gtksourceview-3.0`
SRCS = smart-c-comment-substitution.c
OBJS = $(SRCS:.c=.o)
EXECUTABLE = smart-c-comment-substitution

.PHONY: clean

$(EXECUTABLE): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(EXECUTABLE)

all: $(EXECUTABLE)

clean:
	rm -f $(OBJS) $(EXECUTABLE)
