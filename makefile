all: control.c client.c
	gcc -o control control.c
	gcc -o client client.c
