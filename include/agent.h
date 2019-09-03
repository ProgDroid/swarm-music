/*
  @file     agent.cpp
  @author   Fernando Ferreira

  The Free Sound of Self-Organisation: Swarm Music
  An agent of the swarm, with its local rules for movement
*/

#ifndef AGENT_H_
#define AGENT_H_

#include <triplet.h>
#include <attractor.h>
#include <vector>

class Agent {
public:
  Triplet position; // random
  Triplet direction; // also random, always needs to be normalised
  Triplet acceleration;

  // the different colours it can be
  Triplet blue;
  Triplet red;
  // its actual colour
  Triplet colour;

  float speed;
  float blindAngle;
  float maxForce;

// public:

  Agent();
  // Agent(Triplet pos);

  void setColour(int colourCount);

  Triplet repulsion(std::vector<Agent> &agents, float radiusR,float blindAngle,
                    float maxForce);
  Triplet orientation(std::vector<Agent> &agents, float radiusR, float radiusO,
                      float blindAngle, float maxForce);
  Triplet attraction(std::vector<Agent> &agents, float radiusO, float radiusA,
                     float blindAngle, float maxForce);

  Triplet bounding(Triplet min, Triplet max);
  void draw();
  void move(float speed, std::vector<Attractor> attractors);
  void step(std::vector<Agent> &agents, float radiusR, float radiusO,
            float radiusA, float angle, float maxForce);
}; // class Agent

#endif
