.PHONY: all clean

CC = gcc

TARGET = train test

RM = rm -f

all: $(TARGET)
# type make/make all to compile test_hmm
train: train.c
	$(CC) -o train train.c

test: test.c
	$(CC) -o test test.c


clean:
	$(RM) $(TARGET) train.o test.o  # type make clean to remove the compiled file
