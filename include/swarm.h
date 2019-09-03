/*
  @file     swarm.h
  @author   Fernando Ferreira

  The Free Sound of Self-Organisation: Swarm Music
  A swarm made up of agents
*/

#ifndef SWARM_H_
#define SWARM_H_

#include <agent.h>
#include <triplet.h>
#include <attractor.h>

class Swarm {
public:
  std::vector<Agent> agents;

  std::vector<Attractor> attractors;

  Triplet averagePosition;
  int swarmMode;

  float radiusR;
  float radiusO;
  float radiusA;

  float blindAngle;
  float speed;
  float maxForce;

  Swarm();

  void addAttractor(long pitch, int tone);

  void resetAll();
  void resetAttractors();

  int getSize();
  int getAttractors();

  void swarming();
  void drawAgents();
  void drawAttractors();
}; // class Swarm

#endif
