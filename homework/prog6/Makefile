TARGET1 = lc1
TARGET2 = lc2
TARGET3 = lc3
OBJS1 = lc1.o
OBJS2 = lc2.o
OBJS3 = lc3.o
CC = gcc
CFLAGS = -Wall -O2
all: $(TARGET1) $(TARGET2) $(TARGET3)

$(TARGET1): $(OBJS1)
		$(CC) -o $(TARGET1) $(OBJS1) $(CFLAGS)

$(TARGET2): $(OBJS2)
		$(CC) -o $(TARGET2) $(OBJS2) $(CFLAGS)

$(TARGET3): $(OBJS3)
		$(CC) -o $(TARGET3) $(OBJS3) $(CFLAGS) -pthread

clean:
		rm -f $(TARGET1) $(OBJS1)
		rm -f $(TARGET2) $(OBJS2)
		rm -f $(TARGET3) $(OBJS3)

.PHONY: clean all
