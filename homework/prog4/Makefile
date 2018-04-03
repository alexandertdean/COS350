TARGET = scroll
OBJS = scroll.o
CC = gcc
CFLAGS = -Wall
all: $(TARGET)

$(TARGET): $(OBJS)
		$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

clean:
		rm -f $(TARGET) $(OBJS)

.PHONY: clean all
