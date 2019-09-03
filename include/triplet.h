/*
  @file     triplet.h
  @author   Fernando Ferreira

  The Free Sound of Self-Organisation: Swarm Music
  Vectors in 3 dimensions
*/

#ifndef TRIPLET_H_
#define TRIPLET_H_

class Triplet {
private:
  float x, y, z;

public:
  Triplet();

  Triplet(float a, float b, float c);

  Triplet(const Triplet &v);

  float length();

  Triplet operator+(Triplet v);
  Triplet operator-(Triplet v);
  float operator*(Triplet v);

  void scalarDiv(float scalar);
  void scalarMul(float scalar);
  void normalise();
  float distance(Triplet b);
  float angle(Triplet b);
  void computeChange(int count, Triplet direction, float maxForce);

  float getX();
  float getY();
  float getZ();

  void setX(float X);
  void setY(float Y);
  void setZ(float Z);
}; // class Triplet

#endif
