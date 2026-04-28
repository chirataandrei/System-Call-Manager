CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

SRCS = main.c data_struct_functions.c operations.c
OBJS = data_struct_functions.o operations.o main.o

TARGET = tema1

build: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(CFLAGS)

pack:
	zip -FSr 313CA_ChirataAndrei_Tema1.zip README Makefile *.c *.h

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: pack clean