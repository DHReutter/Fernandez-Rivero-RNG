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

#include <stdlib.h>
#include <math.h>

#include <stdio.h>

#ifndef PI
#define PI 3.141592654
#endif

// Declaration of opaque function.
double _OPAQUE_nrand(int reset, unsigned int rehash, double sd);


void snrand(unsigned int seed, double sd)
{
	_OPAQUE_nrand(1, seed, sd);
}

double nrand()
{
	return _OPAQUE_nrand(0, 0, 0);
}

double nrandsd(double sd)
{
	return _OPAQUE_nrand(0, 0, sd);
}

// Now opaque definitions follow.

// Generates a 32 bit hash of a 32 bit number.
inline void _OPAQUE_hash(unsigned int * const a) {
    *(a) = (*(a) ^ 61) ^ (*(a) >> 16);
    *(a) = *(a) + (*(a) << 3);
    *(a) = *(a) ^ (*(a) >> 4);
    *(a) = *(a) * 0x27d4eb2d;
    *(a) = *(a) ^ (*(a) >> 15);
    return;
}

// A Fernandez-Rivero random number generator which works by rotating two coordinates by random angles.
// See http://www.springerlink.com/content/yj82465n0q755572/
double _OPAQUE_nrand(int reset, unsigned int rehash, double sd) {
	const unsigned int N = 2048;  // Array size of r.
	const unsigned int K = 1024;  // Array size of c and s.
	static double rho = 0.1;      // rho of the normal distribution.
	static double *c = NULL;      // Array of cosinus for predefined angles.
	static double *s = NULL;      // Array of sinus for predefined angles.
	static double *r = NULL;      // Array of normally distributed numbers.
	static char secondp = 0;      // Switch to determine whether the second half of a pair is still unused.
	static unsigned int m = 0;    // Counter of current r position.
	static unsigned int ihash;    // Current uniform randomly distributed hash.
	double x;                     // Temporal variable.
	unsigned int n, k;            // Random numbers between 0 and N resp. K.
	const double SD_ACCURACY = 0.001; // Maximal deviance of the square of the difference before reset is triggered.

	if (sd) {
		if ((rho - sd) * (rho - sd) > SD_ACCURACY) {
			reset = 1;
		}
	}

	if (reset || r == NULL) {
		unsigned int i = 0;
		double theta;
		const int modulo = 1048576;

		// Randomness is generated by a hash function. To initialize, a random number is taken out of the standard rng.
		ihash = rehash?rehash:ihash; // Don't change ihash if there is no value for rehash.
		rho = sd?sd:rho; // Don't change rho if there is no value for sd.

		// Allocate the random, cosinus and sinus arrays.
		if (c == NULL) {
			c = (double *)malloc(K * sizeof(double));
		}
		if (s == NULL) {
			s = (double *)malloc(K * sizeof(double));
		}
		if (r == NULL) {
			r = (double *)malloc(N * sizeof(double));
		}

		// Randomly choose K angles and precompute cosinus and sinus for it.  Store in c, s.
		for (i = 0; i < K; ++i) {
			_OPAQUE_hash(&ihash);
			theta = (double)(ihash % modulo) * 2 * PI / (double)modulo;
			c[i] = cos(theta);
			s[i] = sin(theta);
		}

		// Initialize r with rho.  A normal distribution of given rho is not proven to work better.
		for (i = 0; i < N; ++i) {
			r[i] = rho;
		}

		// Initialize by rotating every point at least once.
		for (i = 0; i < N; ++i) {
			// Randomly select an n different from m.
			do {
				_OPAQUE_hash(&ihash);
				n = ihash % N;
			} while (n == i);
			// Randomly select one of the predefined angles.
			_OPAQUE_hash(&ihash);
			k = ihash % K;
			// Rotate the x/y position by the given angle. New position is normally distributed.
			x = r[i] * c[k] + r[n] * s[k];
			r[n] = -r[i] * s[k] + r[n] * c[k];
			r[i] = x;
		}

		// Reset the counters.
		n = 0;
		m = 0;
		secondp = 0;
	}

	if (secondp == 0) {
		m = (m + 1) % N;

		// Randomly select an n different from m.
		do {
			_OPAQUE_hash(&ihash);
			n = ihash % N;
		} while (n == m);

		// Randomly select one of the predefined angles.
		_OPAQUE_hash(&ihash);
		k = ihash % K;

		// Rotate the x/y position by the given angle. New position is normally distributed.
		x = r[m] * c[k] + r[n] * s[k];
		r[n] = -r[m] * s[k] + r[n] * c[k];
		r[m] = x;

		secondp = 1;
		return r[n];
	} else {
		secondp = 0;
		return r[m];
	}
}
