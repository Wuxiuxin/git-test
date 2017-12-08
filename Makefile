all:
	arm-linux-gcc *.c -o project -lpthread
clean:
	rm -f *.o
