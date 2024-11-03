all: myshell mypipe looper

myshell: myshell.c LineParser.c
	gcc -g -m32 -o myshell myshell.c LineParser.c

mypipe: mypipe.c
	gcc -g -m32 -o mypipe mypipe.c

looper: looper.c
	gcc -g -m32 -o looper looper.c

clean:
	rm -f myshell mypipe looper