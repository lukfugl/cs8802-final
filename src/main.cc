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
  
  int iterations = 0;
  while (!simulator.terminated() && iterations < maxIterations) {
    simulator.advance();
    iterations++;
  }
  
  return 0;
}
