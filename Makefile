CC	= gcc
AS	= as
CFLAGS	= -g -O -Wall
ASFLAGS	= -g
LDFLAGS	= 
RM	= rm

CSRCS_CPD	= compress_main.c compress.c LZSS.c huffman.c heap.c tree.c substitution.c
CSRCS_DCPD	= decompress_main.c compress.c LZSS.c huffman.c heap.c tree.c substitution.c

ASRCS	= assemble.s
TARGET	= compress
TARGET2 = decompress
OBJECTS_CPD	= $(CSRCS_CPD:.c=.o) $(ASRCS:.s=.o)
OBJECTS_DCPD	= $(CSRCS_DCPD:.c=.o) $(ASRCS:.s=.o)

all: $(TARGET)
all: $(TARGET2)

$(TARGET): $(OBJECTS_CPD)
	$(CC) $(LDFLAGS) $(OBJECTS_CPD) -o $@

$(TARGET2): $(OBJECTS_DCPD)
	$(CC) $(LDFLAGS) $(OBJECTS_DCPD) -o $@
	
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.s.o:
	$(AS) $(ASFLAGS) $< -o $@

clean:
	$(RM) -f $(OBJECTS_CPD) $(TARGET)
	$(RM) -f $(OBJECTS_DCPD) $(TARGET2)
