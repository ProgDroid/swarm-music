/*
  @file     attractor.cpp
  @author   Fernando Ferreira

  The Free Sound of Self-Organisation: Swarm Music
  A particle pleasant to the agents, calling to them
*/

#ifndef ATTRACTOR_H_
#define ATTRACTOR_H_

#include <triplet.h>

class Attractor {
public:
  Triplet position;
  int tone;
  int strength;
  int mode;

  Triplet colour;

  Attractor(long pitch);
  Attractor(long pitch, int givenTone);

  void draw();
}; // class Attractor

#endif
