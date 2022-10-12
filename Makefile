CFLAGS = -std=c99 -Wall -Wextra -Werror

make: 
	gcc $(CFLAGS) -o t9search src/t9search.c

clean:
	rm t9search
