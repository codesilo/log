default:all

all:test_main

test_main:test_main.o log.o
	@$(CC) $^ -o $@  -g

%.o:%.c
	@$(CC) -c $^ -o $@  -g

clean:
	@$(RM) -rf *.o test_main

.PHONY:
	clean

