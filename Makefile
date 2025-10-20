.PHONY: all
all :
	gcc src/main.c src/BBQ.c -Iinclude/ -o bin/bbq_test.out

.PHONY: clean
clean :
	rm bin/*.out
