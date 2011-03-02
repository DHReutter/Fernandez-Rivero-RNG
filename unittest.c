/*
 * Fernandez-Rivero-RNG - A fast normal distribution pseudo-random number generator
 * Copyright (C) 2009-2010 by Daniel Reutter <reutter@psy.lmu.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "nrand.h"
#include <stdio.h>
#include <math.h>

#define SEED1 0x23
#define SEED2 0x00

#define SD1 0.1
#define SD2 1000

#define N 2e6

#define MAXDEV 0.05

inline double sqr(double x) {
	return x * x;
}

double testdeviation(int seed, double sd) {
	int i;
	double A, Q, rn;
	double calcsd, deviation;

	A = 0;
	Q = 0;
	snrand(seed);
	for (i = 0; i < N; ++i) {
		rn = nrandsd(sd);
		if (i) {
			Q = Q + (i * sqr(rn - A)) / (i+1);
			A = A + (rn - A)/(i+1);
		} else {
			A = rn;
			Q = 0;
		}
	}
	calcsd = sqrt(Q / N);
	deviation = fabs(1 - (calcsd / sd));
	printf("[%s] - Calculated deviation %f is %2.2f%% from given deviation %f.\n", (deviation < MAXDEV)?" OK ":"FAIL", calcsd, deviation*100, sd);
	return rn;
}

int main() {
	double rn1, rn2;

	rn1 = testdeviation(SEED1, SD1);
	testdeviation(SEED2, SD1);
	testdeviation(SEED2, SD2);
	rn2 = testdeviation(SEED1, SD1);

	printf("[%s] - Same seed returns same random number (%f and %f).\n", (rn1 == rn2)?" OK ":"FAIL", rn1, rn2);

	return 0;
}
