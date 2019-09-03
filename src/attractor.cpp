/*
  @file     attractor.cpp
  @author   Fernando Ferreira

  The Free Sound of Self-Organisation: Swarm Music
  A particle pleasant to the agents, calling to them
*/

#include <attractor.h>
#include <triplet.h>
#include <cmath>
#include <vector>
#include <random>
#include <GL/glut.h>

const float MAUVE_R = 0.75686274509;
const float MAUVE_G = 0.51372549019;
const float MAUVE_B = 0.62352941176;
const float FREEFORM = 0;
const float SCALES = 1;
const float I = 0, ii = 1, iii = 2, IV = 3, V = 4, vi = 5, vii = 6;

float attractRand()
{
  std::random_device randomDevice;
  std::mt19937 generator(randomDevice());
  std::uniform_int_distribution<> distribution(0, 800);

  float randNo = (float) distribution(generator);
  return randNo;
} // attractRand

Attractor::Attractor(long pitch)
{
  mode = FREEFORM;

  colour = Triplet(MAUVE_R, MAUVE_G, MAUVE_B);

  float xCoord = (float) pitch - 72;

  xCoord *= 800;
  xCoord /= 24;
  xCoord -= 400;
  position = Triplet(xCoord, attractRand() - 400.0, attractRand() - 400.0);
  strength = 2;
} // Attractor

Attractor::Attractor(long pitch, int givenTone)
{
  tone = givenTone;
  mode = SCALES;

  float xCoord = (float) pitch - 72;

  xCoord *= 800;
  xCoord /= 24;
  xCoord -= 400;
  position = Triplet(xCoord, attractRand() - 400.0, attractRand() - 400.0);
  if (tone == I || tone == V)
  {
    strength = 3;
  } // if
  else if (tone == ii || tone == IV || tone == vii)
  {
    strength = 2;
  } // else if
  else if (tone == iii || tone == vi)
  {
    strength = 1;
  } // else if

  switch (strength)
  {
    case 1: colour = Triplet(MAUVE_R-0.2, MAUVE_G-0.2, MAUVE_B-0.2);
            break;
    case 2: colour = Triplet(MAUVE_R, MAUVE_G, MAUVE_B);
            break;
    case 3: colour = Triplet(MAUVE_R+0.2, MAUVE_G+0.2, MAUVE_B+0.2);
            break;
  } // switch
} // Attractor

void Attractor::draw()
{
  glColor4f(colour.getX(), colour.getY(), colour.getZ(), 1.0);
  glPushMatrix();
    glTranslatef(position.getX(), position.getY(), position.getZ());
    glutWireSphere(5, 10, 10);
  glPopMatrix();
} // draw
