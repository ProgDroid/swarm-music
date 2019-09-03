/*
  @file     agent.cpp
  @author   Fernando Ferreira

  The Free Sound of Self-Organisation: Swarm Music
  An agent of the swarm, with its local rules for movement
*/

#include <agent.h>
#include <attractor.h>
#include <triplet.h>
#include <cmath>
#include <vector>
#include <random>
#include <GL/glut.h>

const float TO_DEGREES = 57.295779524;
const float BLUE_R = 0.03137254901;
const float BLUE_G = 0.49411764705;
const float BLUE_B = 0.54509803921;
const float RED_R = 1.0;
const float RED_G = 0.35294117647;
const float RED_B = 0.3725490196;
const float DEFAULT_WHITE = 0.96078431372;

// random number utility
// generates numbers from 0 to 800 which then can be used for the position
//   coordinates directly by adjusting to -400 to 400
// for the direction, adjusting the same way and dividing by 400
float myRand()
{
  std::random_device randomDevice;
  std::mt19937 generator(randomDevice());
  std::uniform_int_distribution<> distribution(0, 800);

  float randNo = (float) distribution(generator);
  return randNo;
} // myRand

Agent::Agent()
{
  blue = Triplet(BLUE_R, BLUE_G, BLUE_B);
  red = Triplet(RED_R, RED_G, RED_B);
  colour = Triplet(0.0, 0.0, 0.0);

  // assign random colour
  switch ((int) myRand())
  {
    case 0 ... 400:   colour = blue;
                      break;
    case 401 ... 800: colour = red;
                      break;
    // just in case
    default:          colour=Triplet(DEFAULT_WHITE,DEFAULT_WHITE,DEFAULT_WHITE);
  } // switch

  // agent properties
  acceleration = Triplet(0.0, 0.0, 0.0);
  position = Triplet(myRand() - 400.0, myRand() - 400.0, myRand() - 400.0);
  direction = Triplet((myRand() - 400.0) / 400.0,
                      (myRand() - 400.0) / 400.0,
                      (myRand() - 400.0) / 400.0);
} // Agent

// change colour if near enough of the other colour
void Agent::setColour(int colourCount)
{
  if (colourCount > 0)
  {
    colour = blue;
  } // if
  else if (colourCount < 0)
  {
    colour = red;
  } // else if
} // setColour

Triplet Agent::repulsion(std::vector<Agent> &agents, float radiusR,
                         float blindAngle, float maxForce)
{
  Triplet newDirection(0.0, 0.0, 0.0);
  int count = 0;

  for (std::size_t i = 0, j = agents.size(); i < j; ++i)
  {
    // calculate distance to another agent
    float distance = position.distance(agents.at(i).position);
    // if not itself and within radius
    if (distance > 0 && distance <= radiusR)
    {
      // check if not behind
      Triplet vecBetweenPoints = agents.at(i).position - position;
      float angle = direction.angle(vecBetweenPoints);
      // if not, consider it
      if (angle <= (180 - blindAngle))
      {
        vecBetweenPoints.scalarMul(-1);
        vecBetweenPoints.normalise();
        vecBetweenPoints.scalarDiv(distance);
        newDirection = newDirection + vecBetweenPoints;
        ++count;
      } // if
    } // if
  } // for

  // average and add to acceleration
  if (count > 0)
  {
    newDirection.computeChange(count, direction, maxForce);
  } // if

  return newDirection;
} // repulsion

Triplet Agent::orientation(std::vector<Agent> &agents, float radiusR,
                           float radiusO, float blindAngle, float maxForce)
{
  Triplet newDirection(0.0, 0.0, 0.0);
  int count = 0;
  int colourCount = 0;

  for (std::size_t i = 0, j = agents.size(); i < j; ++i)
  {
    // calculate distance
    float distance = position.distance(agents.at(i).position);
    // if within range (also not itself)
    if (distance > 0 && distance <= radiusO)
    {
      // check if behind
      Triplet vecBetweenPoints = agents.at(i).position - position;
      float angle = direction.angle(vecBetweenPoints);
      if (angle <= (180 - blindAngle))
      {
        newDirection = newDirection + agents.at(i).direction;

        // check colours
        // if red take 1, else add 1
        if (agents.at(i).colour.getX() == 1.0)
        {
          --colourCount;
        } // if
        // if red take one
        else
        {
          ++colourCount;
        } // else
        ++count;
      } // if
    } // if
  } // for

  if (count > 0)
  {
    // average and add to acceleration
    newDirection.computeChange(count, direction, maxForce);
  } // if

  setColour(colourCount);

  return newDirection;
} // orientation

Triplet Agent::attraction(std::vector<Agent> &agents, float radiusO,
                          float radiusA, float blindAngle, float maxForce)
{
  Triplet newDirection(0.0, 0.0, 0.0);
  int count = 0;

  for (std::size_t i = 0, j = agents.size(); i < j; ++i)
  {
    // calculate distance
    float distance = position.distance(agents.at(i).position);
    // if within range
    if (distance > radiusO && distance <= radiusA)
    {
      // check if behind
      Triplet vecBetweenPoints = agents.at(i).position - position;
      float angle = direction.angle(vecBetweenPoints);
      if (angle <= (180 - blindAngle))
      {
        newDirection = newDirection + vecBetweenPoints;
        ++count;
      } // if
    } // if
  } // for

  if (count > 0)
  {
    // average and add to acceleration
    newDirection.computeChange(count, direction, maxForce);
  } // if

  return newDirection;
} // attraction

// extra rule to bind the agents to the cube
Triplet Agent::bounding(Triplet min, Triplet max)
{
  Triplet vector(0.0, 0.0, 0.0);

  if (position.getX() <= min.getX())
  {
    vector.setX(0.3);
  } // if
  else if (position.getX() >= max.getX())
  {
    vector.setX(-0.3);
  } // else if

  if (position.getY() <= min.getY())
  {
    vector.setY(0.3);
  } // if
  else if (position.getY() >= max.getY())
  {
    vector.setY(-0.3);
  } // else if

  if (position.getZ() <= min.getZ())
  {
    vector.setZ(0.3);
  } // if
  else if (position.getZ() >= max.getZ())
  {
    vector.setZ(-0.3);
  } // else if

  return vector;
} // bounding

void Agent::draw()
{
  glColor4f(colour.getX(), colour.getY(), colour.getZ(), 1.0);
  glPushMatrix();
    glTranslatef(position.getX(), position.getY(), position.getZ());
    Triplet tempDir = Triplet(direction);
    tempDir.scalarMul(TO_DEGREES);
    glRotatef(tempDir.getX(), 1.0, 0.0, 0.0);
    glRotatef(tempDir.getY(), 0.0, cos(direction.getX()), -sin(direction.getX()));
    glRotatef(tempDir.getZ(), 0.0, 0.0, 1.0);
    // scaling for looks
    glScalef(1.0, 0.25, 0.25);
    glutSolidCone(5.5, 5.5, 5, 1);
  glPopMatrix();
} // draw

void Agent::move(float speed, std::vector<Attractor> attractors)
{
  float minDistance = 100000000;
  size_t index = 0;
  if (attractors.size() != 0)
  {
    for (std::size_t i = 0; i < attractors.size(); i++)
    {
      float distance = position.distance(attractors.at(i).position);
      if (distance < minDistance)
      {
        index = i;
        minDistance = distance;
      } // if
    } // for
    Triplet newTmp = attractors.at(index).position - position;
    newTmp.scalarMul(0.0005);
    acceleration = acceleration + newTmp;
  } // if
  // adjust acceleration if too harsh?
  // acceleration.scalarMul(0.5);

  direction = direction + acceleration;
  direction.normalise();

  Triplet stepDirection = direction;
  stepDirection.scalarMul(speed);
  position = position + stepDirection;

  // reset acceleration
  acceleration.scalarMul(0);
} // move

void Agent::step(std::vector<Agent> &agents, float radiusR, float radiusO,
                 float radiusA, float angle, float maxForce)
{
  // calculate repulsion first as this is the priority
  Triplet repulsionVec = repulsion(agents, radiusR, angle, maxForce);
  // repulsionVec.scalarMul(1);
  // if there's moving away to do, do it
  if (repulsionVec.length() != 0) {
    acceleration = acceleration + repulsionVec;
    return;
  } // if
  // otherwise we can seek others, stay in course or a bit of both
  else {
    Triplet orientationVec = orientation(agents, radiusR, radiusO, angle,
                                         maxForce);
    orientationVec.scalarMul(2.5);
    Triplet attractionVec = attraction(agents, radiusO, radiusA, angle,
                                       maxForce);
    // attractionVec.scalarMul(1);
    // can cast to int since value isn't important, only if it's 0
    switch ((int) orientationVec.length()) {
      case 0:   acceleration = acceleration + attractionVec;
                break;
      default:  if (attractionVec.length() == 0) {
                  acceleration = acceleration + orientationVec;
                  break;
                } // if
                else {
                  Triplet tmpVector = orientationVec + attractionVec;
                  // multiply by factor to weight this rule,
                  //  as we want the swarm to keep moving to generate non-static
                  //  music
                  tmpVector.scalarDiv(2);
                  acceleration = acceleration + tmpVector;
                  break;
                } // else
    } // switch
  } // else
  Triplet min(-400.0, -400.0, -400.0);
  Triplet max(400.0, 400.0, 400.0);
  Triplet boundingVec = bounding(min, max);
  acceleration = acceleration + boundingVec;
} // step
