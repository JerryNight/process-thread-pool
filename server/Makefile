srcs:=$(wildcard *.c)
objs:=$(patsubst %.c,%.o, $(srcs))

%.o:%.c
	gcc -c $^ -o $@ -g

main:$(objs)
	gcc $^ -o $@ -lpthread

clean:
	$(RM) $(objs) main

rebuild: clean main