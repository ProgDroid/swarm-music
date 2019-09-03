/*
  @file     triplet.cpp
  @author   Fernando Ferreira

  The Free Sound of Self-Organisation: Swarm Music
  Vectors in 3 dimensions
*/

#include <triplet.h>
#include <cmath>

const float TO_DEGREES = 57.295779524;

Triplet::Triplet()
{
  x = 0;
  y = 0;
  z = 0;
} // Triplet()

Triplet::Triplet(float a, float b, float c)
{
  x = a;
  y = b;
  z = c;
} // Triplet(float X, float Y, float Z)

Triplet::Triplet(const Triplet &v)
{
  x = v.x;
  y = v.y;
  z = v.z;
} // Triplet(Triplet &v)

float Triplet::length()
{
  float length = sqrt((x * x) + (y * y) + (z * z));
  return length;
} // length

Triplet Triplet::operator+(Triplet vector)
{
  Triplet triplet = Triplet((x + vector.x), (y + vector.y), (z + vector.z));
  return triplet;
} // operator+

Triplet Triplet::operator-(Triplet vector)
{
  Triplet triplet = Triplet((x - vector.x), (y - vector.y), (z - vector.z));
  return triplet;
} // operator-

float Triplet::operator*(Triplet vector)
{
  float dotProduct = (x * vector.x) + (y * vector.y) + (z * vector.z);
  return dotProduct;
} // operator*

void Triplet::scalarDiv(float scalar)
{
  x = x / scalar;
  y = y / scalar;
  z = z / scalar;
} // scalarDiv

void Triplet::scalarMul(float scalar)
{
  x = x * scalar;
  y = y * scalar;
  z = z * scalar;
} // scalarMul

void Triplet::normalise()
{
  float mag = length();
  if (mag > 0)
  {
    scalarDiv(mag);
  } // if
} // normalise

float Triplet::distance(Triplet b)
{
  Triplet distV = b - *this;
  float distance = distV.length();
  return distance;
} // distance

float Triplet::angle(Triplet b)
{
  float mag = length();
  float magB = b.length();
  if (mag > 0 && magB > 0) {
    float dotProduct = *this * b;
    float cosTheta = dotProduct / (mag * magB);
    if (cosTheta <= -1)
    {
      return 180;
    } // if
    else if (cosTheta >= 1)
    {
      return 0.0;
    } // else if
    else {
      float theta = acos(cosTheta) * TO_DEGREES;
      return theta;
    } // else
  } // if
  return 0.0;
} // angle

void Triplet::computeChange(int count, Triplet direction, float maxForce)
{
  scalarDiv((float) count);
  normalise();
  *this = *this - direction;
  if (length() > maxForce)
  {
    normalise();
    scalarMul(maxForce);
  } // if
} // computeChange

float Triplet::getX()
{
  float X = x;
  return X;
} // getX

float Triplet::getY()
{
  float Y = y;
  return Y;
} // getY

float Triplet::getZ()
{
  float Z = z;
  return Z;
} // getZ

void Triplet::setX(float X)
{
  x = X;
} // getX

void Triplet::setY(float Y)
{
  y = Y;
} // getY

void Triplet::setZ(float Z)
{
  z = Z;
} // getX
