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
#include <stdlib.h>

int main(int argc, char *argv[]) {

  double sd;
  int n, seed;
  int i;

  if (argc < 4) {
    printf("Too few arguments. Usage:\n%s [n] [seed] [sd]\n", argv[0]);
    return -1;
  }

  n = atoi(argv[1]);
  seed = atoi(argv[2]);
  sd = atof(argv[3]);

  snrand(seed);

  for (i = 0; i < n; ++i) {
    printf("%f\n", (double)nrandsd(sd));
  }

  return 0;
}
