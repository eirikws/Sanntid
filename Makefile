TARGET=/export/nfs/root/main
CFLAGS=-g -Wall -D_GNU_SOURCE
LDFLAGS=-g
SOURCES=main.c udp.h udp.c pid.h pid.c
all: $(TARGET)
OBJECTS=$(SOURCES:.c=.o)

export PATH = $PATH:/home/student/Desktop/G17_ex10/buildroot-avr32-v3.0.0_TTK4147/output/staging/usr/bin
$(TARGET): $(OBJECTS)
	avr32-linux-gcc $(LDFLAGS) -o $@ $^

%.o : %.c
	avr32-linux-gcc  $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJECTS)
