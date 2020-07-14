#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <errno.h>

// push

int main() {
	printf("errno: %d\n", errno);
	printf("malloc: %s\n", strerror(errno));

	return 0;
}