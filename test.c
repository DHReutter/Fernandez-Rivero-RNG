#include "nrand.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

inline double sqr(double x) {
	return x * x;
}

int main(int argc, char *argv[]) {

	double rn = 0;

	double A = 0, Q = 0;

	clock_t c0, c1;

	double sd, calcsd;
	int n, seed;
	int i;

	if (argc < 4) {
		printf("Too few arguments. Usage:\n%s [n] [seed] [sd]\n", argv[0]);
		return -1;
	}

	n = atoi(argv[1]);
	seed = atoi(argv[2]);
	sd = atof(argv[3]);

	snrand(seed, sd);

	c0 = clock();

	for (i = 0; i < n; ++i) {
		rn = 5 + nrand();
		if (i) {
			Q = Q + (i * sqr(rn - A)) / (i+1);
			A = A + (rn - A)/(i+1);
		} else {
			A = rn;
			Q = 0;
		}
	}

	c1 = clock();

	printf("Last number was %f\n", rn);
	printf("Mean is %f\n", A);
	calcsd = sqrt(Q / n);
	printf("Standard deviation is %f (deviates %f from %f)\n", calcsd, sd - calcsd, sd);

	printf("\nThe calculation took %d clock cycles (%.3f milliseconds; %.3f microsec/call).\n", (int)(c1-c0), (c1-c0)/(double)CLOCKS_PER_SEC*1000, (c1-c0)/(double)CLOCKS_PER_SEC*1000000/n);

	return 0;
}
