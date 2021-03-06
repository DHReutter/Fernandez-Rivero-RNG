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

// Pi definition.
#ifndef PI
#ifdef M_PII
#define PI (float_type)M_PII
#else
#ifdef M_PI
#define PI (float_type)M_PI
#else
// 26 numbers of pi.
#define PI (float_type)3.1415926535897932384626434
#endif
#endif
#endif

// If NO_HASH is defined, the hash function is replaced with the normal RNG.
#ifdef NO_HASH
#endif

// Declaration of opaque function.
float_type _OPAQUE_nrand(int reset, unsigned int rehash);

void snrand(unsigned int seed)
{
	_OPAQUE_nrand(1, seed);
}

float_type nrand()
{
	return _OPAQUE_nrand(0, 0);
}

float_type nrandsd(float_type sd)
{
	return _OPAQUE_nrand(0, 0) * sd;
}

// Now opaque definitions follow.

// Generates a 32 bit hash of a 32 bit number.
inline void _OPAQUE_hash(unsigned int * const a) {
#ifdef NO_HASH
	*(a) = (unsigned int)rand();
#else
    *(a) = (*(a) ^ 61) ^ (*(a) >> 16);
    *(a) = *(a) + (*(a) << 3);
    *(a) = *(a) ^ (*(a) >> 4);
    *(a) = *(a) * 0x27d4eb2d;
    *(a) = *(a) ^ (*(a) >> 15);
#endif
    return;
}

// A Fernandez-Rivero random number generator which works by rotating two coordinates by random angles.
// See http://www.springerlink.com/content/yj82465n0q755572/
float_type _OPAQUE_nrand(int reset, unsigned int rehash) {
	const unsigned int N = 2048;  // Array size of r.
	const unsigned int K = 1024;  // Array size of c and s.
	const float_type rho = 1.0;      // rho of the normal distribution.
	static float_type *c = NULL;      // Array of cosinus for predefined angles.
	static float_type *s = NULL;      // Array of sinus for predefined angles.
	static float_type *r = NULL;      // Array of normally distributed numbers.
	static char secondp = 0;      // Switch to determine whether the second half of a pair is still unused.
	static unsigned int m = 0;    // Counter of current r position.
	static unsigned int ihash;    // Current uniform randomly distributed hash.
	float_type x;                     // Temporal variable.
	unsigned int n, k;            // Random numbers between 0 and N resp. K.

	if (reset || r == NULL) {
		unsigned int i = 0;
		float_type theta;
		const int modulo = 1048576;

#ifdef NO_HASH
		// Randomness is generated by a hash function. To initialize, a random number is taken out of the standard rng.
		// If rehash is 0, take a random value from /dev/random.
		if (rehash) {
			srand(rehash);
		} else {
			sranddev();
		}
#else
		ihash = rehash?rehash:ihash; // Don't change ihash if there is no value for rehash.
#endif

		// Allocate the random, cosinus and sinus arrays.
		if (c == NULL) {
			c = (float_type *)malloc(K * sizeof(float_type));
		}
		if (s == NULL) {
			s = (float_type *)malloc(K * sizeof(float_type));
		}
		if (r == NULL) {
			r = (float_type *)malloc(N * sizeof(float_type));
		}

		// Randomly choose K angles and precompute cosinus and sinus for it.  Store in c, s.
		for (i = 0; i < K; ++i) {
			_OPAQUE_hash(&ihash);
			theta = (float_type)(ihash % modulo) * 2 * PI / (float_type)modulo;
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
