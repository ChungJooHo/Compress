CC	= gcc
AS	= as
CFLAGS	= -g -O -Wall
ASFLAGS	= -g
LDFLAGS	= 
RM	= rm

CSRCS	= main.c compress.c LZ77.c huffman.c heap.c tree.c substitution.c runlength.c
ASRCS	= 
TARGET	= compress
OBJECTS	= $(CSRCS:.c=.o) $(ASRCS:.s=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.s.o:
	$(AS) $(ASFLAGS) $< -o $@

clean:
	$(RM) -f $(OBJECTS) $(TARGET) 
