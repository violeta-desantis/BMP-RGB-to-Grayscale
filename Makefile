all:
	gcc -o gray rgb_to_gray.c -std=c99

clean:
	rm -f gray
