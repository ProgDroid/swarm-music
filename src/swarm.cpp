/*
  @file     swarm.cpp
  @author   Fernando Ferreira

  The Free Sound of Self-Organisation: Swarm Music
  A swarm made up of agents
*/

#include <swarm.h>
#include <triplet.h>
#include <agent.h>
#include <attractor.h>
#include <vector>
#include <random>

const int MAX_SIZE = 500;
const int MAX_ATTRACTORS = 14;
const int RANDOM = 0;
const int AVERAGE = 1;
const int FREEFORM = 0;
const int SCALES = 1;

// same as in agent, for random positions to be picked for averaging
float swarmRand()
{
  std::random_device randomDevice;
  std::mt19937 generator(randomDevice());
  std::uniform_int_distribution<> distribution(0, 20);

  float randNo = (float) distribution(generator);
  return randNo;
} // swarmRand


Swarm::Swarm()
{
  agents.reserve(MAX_SIZE);

  attractors.reserve(MAX_ATTRACTORS);

  for (int i = 0; i < MAX_SIZE; ++i)
  {
    Agent agent;
    agents.push_back(agent);
  } // for

  radiusR = 20.0;
  radiusO = 50.0;
  radiusA = 80.0;

  blindAngle = 10.0;
  speed = 1.5;
  maxForce = 0.7;

  averagePosition = Triplet(0.0, 0.0, 0.0);
  swarmMode = RANDOM;
} // Swarm

void Swarm::resetAll()
{
  agents.erase(agents.begin(), agents.end());

  for (int i = 0; i < MAX_SIZE; ++i)
  {
    Agent agent;
    agents.push_back(agent);
  } // for
} // resetAll

void Swarm::resetAttractors()
{
  attractors.erase(attractors.begin(), attractors.end());
} // resetAttractors

int Swarm::getSize()
{
  int size = agents.size();
  return size;
} // getSize

int Swarm::getAttractors()
{
  int size = attractors.size();
  return size;
} // getAttractors

void Swarm::addAttractor(long pitch, int tone)
{
  if (pitch < 96)
  {
    Attractor tmp(pitch);
    if (tone != -1)
    {
      tmp = Attractor(pitch, tone);
    } // if
    attractors.push_back(tmp);
  } // if
} // addAttractor

void Swarm::swarming()
{
  for (int i = 0, j = getSize(); i < j; ++i)
  {
    agents[i].step(agents, radiusR, radiusO, radiusA, blindAngle, maxForce);
    if (swarmMode == RANDOM)
    {
      if (swarmRand() < 2)
      {
        averagePosition.setX(agents[i].position.getX());
      } // if
      if (swarmRand() < 2)
      {
        averagePosition.setY(agents[i].position.getY());
      } // if
      if (swarmRand() < 2)
      {
        averagePosition.setZ(agents[i].position.getZ());
      } // if
    } // if
    else if (swarmMode == AVERAGE)
    {
      averagePosition.setX(averagePosition.getX() + agents[i].position.getX());
      averagePosition.setY(averagePosition.getY() + agents[i].position.getY());
      averagePosition.setZ(averagePosition.getZ() + agents[i].position.getZ());
    } // else if
  } // for

  for (int i = 0, j = getSize(); i < j; ++i)
  {
    agents[i].move(speed, attractors);
  } // for

  if (swarmMode == AVERAGE)
  {
    averagePosition.scalarDiv(getSize());
  } // if
} // swarming

void Swarm::drawAgents()
{
  for (int i = 0, j = getSize(); i < j; ++i)
  {
    agents.at(i).draw();
  } // for
} // drawAgents

void Swarm::drawAttractors()
{
  for (int i = 0, j = getAttractors(); i < j; ++i)
  {
    attractors.at(i).draw();
  } // for
} // drawAttractors
