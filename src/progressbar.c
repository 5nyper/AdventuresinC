#include <stdio.h>
#include <windows.h>

#define BUFFER 21

char empty[BUFFER];

int main() {
	int i = 1;
	float perc;
	empty[0] = '[';
	for (;i<BUFFER; i++) {
		empty[i] = '-';
	}
	empty[BUFFER] = ']';
 	puts("begin download...\n");
 	printf("%s %5.1f\r", empty, perc);
 	for(i = 1;i<BUFFER; i++) {
 		empty[i] = '#';
 		perc = i/(float)BUFFER;
 		printf("%s %5.0f0 %%\r", empty, perc*10);
 		fflush(stdout);
 		usleep(70000);
 	}
    return 0;
}
