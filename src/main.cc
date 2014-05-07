#include "Simulator.h"

#include <string>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc < 2 || argc > 3) {
    printf("usage: %s <filename> [maxIterations]\n", argv[0]);
    return -1;
  }

  string filename(argv[1]);
  Simulator simulator(filename);

  int maxIterations = 10;
  if (argc == 3) {
    maxIterations = atoi(argv[2]);
  }

  printf("i");
  printf("\tbx\tby\tbh"); // true bond position
  printf("\tg1x\tg1y\tg2x\tg2y\tg3x\tg3y\tg4x\tg4y"); // true guard positions
  printf("\t~bx-\t~by-\t~bh-"); // estimated bond position pre-update
  printf("\t~bx+\t~by+\t~bh+"); // estimated bond position post-update
  printf("\t~o1x\t~o1y\t~o2x\t~o2y\t~o3x\t~o3y\t~o4x\t~o4y\t~o5x\t~o5y\t~o6x\t~o6y\t~o7x\t~o7y\t~o8x\t~o8y\t~o9x\t~o9y\t~o10x\t~o10y"); // estimated obstacle positions
  printf("\t~g1x\t~g1y\t~g2x\t~g2y\t~g3x\t~g3y\t~g4x\t~g4y"); // predicted guard positions
  printf("\n");

  int iterations = 0;
  while (!simulator.terminated() && iterations < maxIterations) {
    printf("%d", iterations);
    simulator.advance();
    iterations++;
    printf("\n");
  }

  if (!simulator.terminated()) {
    printf("max iterations exceeded\n");
    return -1;
  }

  return (simulator.success() ? 0 : -1);
}
