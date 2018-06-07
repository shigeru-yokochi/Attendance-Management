all:
	gcc -g tactsw-led.c -o tactsw-led -L/usr/local/lib -I/usr/local/include -lwiringPi
clean:
	rm -f  tactsw-led
