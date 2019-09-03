/*
The Free Sound of Self-Organisation: Swarm Music
Author: Fernando Ferreira

OpenGL code from Toby Howard, bla bla and Fernando Ferreira from COMP37111
coursework.
*/

#include "triplet.h"
#include "agent.h"
#include "swarm.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <GL/glut.h>
#include "AntTweakBar.h"

#define PI                    3.14159265f
#define GRAV                 -9.8f
#define MAX_GRAV             -15.8f
#define MIN_GRAV             -3.8f
#define POINT_SIZE            6.66f
#define MIN_RESTITUTION       0.27f
#define MAX_RESTITUTION       0.97f
#define MIN_PARTICLES         100
#define DEFAULT_PARTICLES     500
#define MAX_PARTICLES         500000
#define MAX_EMITTERS          3
#define VELOCITY_FACTOR       1.0f
#define MAX_VELOCITY_FACTOR   5
#define MIN_LIFETIME          5
#define MAX_LIFETIME          15
// render modes
// #define POINTS                0
// #define QUADS                 1
// colour modes
#define RAINBOW               0
#define BLOOD                 1
#define SWEAT                 2
#define TEARS                 3
#define BRUISE                4
#define BONES                 5
#define VOID                  6

Swarm swarm;
TwBar *leftBar;
float theta = 0.0;
int frameCount = 0;
float framerate = 0.0;
int timeCurrent = 0;
int timeLastUpdate = 0;
int timeLastFrame = 0;
float interval = 0.01;
float pointSize = POINT_SIZE;
float grav = GRAV;
float restitution = 0.47;
// Emitter emitters[MAX_EMITTERS];
// int noOfEmitters = MAX_EMITTERS;
// int renderMode = POINTS;
float velFactor = VELOCITY_FACTOR;
int colourMode = RAINBOW;
int lifetime = 10;
int noOfParticles = DEFAULT_PARTICLES;

// Display list for coordinate axis
GLuint axisList;

int AXIS_SIZE= 200;
int axisEnabled= 1;

void defineUI()
{
  // global ui properties
  TwDefine("GLOBAL help='The Free Sound of Self-Organisation' fontsize=1");
  // left sidebar
  leftBar = TwNewBar("Swarm Properties");
  TwDefine("'Swarm Properties' position='0 0' size='200 120' valueswidth=25 \
            color='193 131 159' alpha=255 movable=false resizable=false");
  // control radiuses
  TwAddVarRW(leftBar, "Repulsion Radius", TW_TYPE_FLOAT,&swarm.radiusR,"min=10 \
             max=40 step=1 keyIncr=r keyDecr=R help='Set repulsion radius.'");
  TwAddVarRW(leftBar,"Orientation Radius",TW_TYPE_FLOAT,&swarm.radiusO,"min=41 \
             max=70 step=1 keyIncr=o keyDecr=O help='Set orientation radius.'");
  TwAddVarRW(leftBar, "Attraction Radius",TW_TYPE_FLOAT,&swarm.radiusA,"min=71 \
             max=150 step=1 keyIncr=a keyDecr=A help='Set repulsion radius.'");

  // control blind angle, speed and maximum force
  TwAddVarRW(leftBar, "Blind angle", TW_TYPE_FLOAT,&swarm.blindAngle, "min=0 \
             max=45 step=1 keyIncr=b keyDecr=B help='Set blind angle.'");
  TwAddVarRW(leftBar, "Speed", TW_TYPE_FLOAT, &swarm.speed,"min=0.5 max=3.5 \
             step=0.1 keyIncr=s keyDecr=S help='Set speed.'");
  TwAddVarRW(leftBar, "Maximum Force", TW_TYPE_FLOAT, &swarm.maxForce,"min=0.1 \
             max=2.0 step=0.1 keyIncr=f keyDecr=F help='Set maximum force.'");
  // control theta manually/check value
  TwAddVarRW(leftBar, "View Angle", TW_TYPE_FLOAT, &theta,"min=0.0 \
             max=360.0 help='Set view angle.'");
} // defineUI

///////////////////////////////////////////////

double myRandom()
//Return random double within range [0,1]
{
  return (rand()/(double)RAND_MAX);
}

///////////////////////////////////////////////

// void drawParticle(Particle particle)
// {
//   if (renderMode == POINTS && particle.position.x != 0)
//   {
//     glPointSize(particle.size);
//     glBegin(GL_POINTS);
//       glColor4f(particle.r,
//                 particle.g,
//                 particle.b,
//                 (-particle.timeDead/lifetime)*1.0);
//       glVertex3f(particle.position.x, particle.position.y, particle.position.z);
//     glEnd();
//   } // if
//   // this is awful but I love the effect
//   else if (renderMode == QUADS)
//   {
//     glLineWidth(particle.size);
//     glBegin(GL_QUADS);
//       glColor4f(particle.r,
//                 particle.g,
//                 particle.b,
//                 (-particle.timeDead/lifetime)*1.0);
//       glVertex3f(particle.position.x-6.66,
//                  particle.position.y-6.66,
//                  particle.position.z-6.66);
//       glVertex3f(particle.positionPrevious.x-6.66,
//                  particle.positionPrevious.y-6.66,
//                  particle.positionPrevious.z-6.66);
//       glVertex3f(particle.position.x+6.66,
//                  particle.position.y+6.66,
//                  particle.position.z+6.66);
//       glVertex3f(particle.positionPrevious.x+6.66,
//                  particle.positionPrevious.y+6.66,
//                  particle.positionPrevious.z+6.66);
//     glEnd();
//   } // else if
// } // drawParticle

///////////////////////////////////////////////

// void drawParticles(Emitter emitter)
// {
//   int i;
//   for (i = 0; i < emitter.particleCount; i++)
//   {
//     drawParticle(emitter.particles[i]);
//   } // for
// } // drawParticles

///////////////////////////////////////////////

// void drawEmitters()
// {
//   int i;
//   int j;
//   for (i = 0; i < noOfEmitters; i++)
//   {
//     glPushMatrix();
//     glColor3f(emitters[i].r, emitters[i].g, emitters[i].b);
//     glBegin(GL_TRIANGLE_FAN);
//       glVertex3f(emitters[i].centre.x,emitters[i].centre.y,emitters[i].centre.z);
//       for (j = 0; j <= 25; j++)
//       {
//         glVertex3f(emitters[i].centre.x + (33.666 * cos(j * (2.0 * PI) / 25)),
//                    emitters[i].centre.y,
//                    emitters[i].centre.z + (33.666 * sin(j * (2.0 * PI) / 25)));
//       } // for
//     glEnd();
//     drawParticles(emitters[i]);
//     glPopMatrix();
//   } // for
// } // drawParticles

///////////////////////////////////////////////

void drawText()
{
  // grav, velFactor, lifetime,
  // coefficient of restitution, noOfParticles, framerate,
  char *c;
  char fps[200];
  sprintf(fps, "Gravity: %2.1f, "
          "Velocity Modifier: %1.1f, "
          "Lifetime Modifier: %1d, "
          // "# of particles/emitter: %4i, "
          "coefficient of restitution: %1.2f, "
          "%4.2f", grav,velFactor,lifetime,/*noOfParticles,*/restitution,framerate);
  glColor4f(1.0, 1.0, 1.0, 1.0);
  // glWindowPos2d(6, 6);
  glRasterPos2d(-666, -600);
  for (c = fps; *c != '\0'; c++)
  {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
  } // for
} // drawText

///////////////////////////////////////////////
// light is awful but I tried
void defineLight()
{
  glEnable(GL_LIGHTING);

  float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
  float lightDifAndSpec[] = { 0.5, 0.5, 0.5, 1.0 };
  float globAmb[] = { 0.0, 0.0, 0.0, 1.0 };

  glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);

  glEnable(GL_LIGHT0);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

  float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
  float matShine[] = { 1.0 };

  glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
  glMaterialfv(GL_FRONT, GL_SHININESS, matShine);
} // defineLight

///////////////////////////////////////////////

void drawLight()
{
  // glEnable(GL_CULL_FACE);
  // glCullFace(GL_FRONT);

  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

  float lightPos[] = { 0.0, 100.0, 1000.0, 1.0 }; // Spotlight position.
  float spotDirection[] = {0.0, -1.0, -1.0}; // Spotlight direction.

  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 55.0);
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirection);
  glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 1.0);
}

///////////////////////////////////////////////

void drawWireCube()
{
  // glMatrixMode(GL_MODELVIEW);
  glDisable(GL_LIGHTING);
  glColor4f(0.0, 0.0, 0.0, 1.0);
  // glLoadIdentity();
  glPushMatrix();
    glutWireCube(800.0);
    // glutWireSphere(0.5,10,10);
  // glBegin(GL_QUADS);
  //   glVertex3f(-3000, -250, -3000);
  //   glVertex3f(3000, -250, -3000);
  //   glVertex3f(3000, -250, 3000);
  //   glVertex3f(-3000, -250, 3000);
  // glEnd();
  glPopMatrix();
  glEnable(GL_LIGHTING);
  // glutSwapBuffers();
} // drawWireCube

///////////////////////////////////////////////

void display()
{
  glLoadIdentity();
  gluLookAt(1250*sin(theta*(PI / 180)), 500.0, 1250*cos(theta*(PI / 180)),
            0.0, -100.0, 0.0,
            0.0, 1.0, 0.0);
  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT);
  // If enabled, draw coordinate axis
  glDisable(GL_LIGHTING);
  // if(axisEnabled) glCallList(axisList);
  glEnable(GL_LIGHTING);

  // drawLight();

  swarm.drawAgents();
  drawWireCube();

  TwDraw();

  // drawEmitters();

  // drawText();

  glutSwapBuffers();
}

///////////////////////////////////////////////

void toggleAxis()
{
  axisEnabled = 1 - axisEnabled;
} // toggleAxis

///////////////////////////////////////////////

// void toggleRenderMode()
// {
//   renderMode = 1 - renderMode;
// } // toggleRenderMode

///////////////////////////////////////////////

void reshape(int width, int height)
{
  // glClearColor(0.231, 0.02, 0.02, 1.0);
  glClearColor(0.23529411764, 0.23529411764, 0.23529411764, 1.0);
  glViewport(0, 0, (GLsizei)width, (GLsizei)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 10000.0);
  glMatrixMode(GL_MODELVIEW);
  TwWindowSize(width, height);
}

///////////////////////////////////////////////

void makeAxes() {
// Create a display list for drawing coord axis
  axisList = glGenLists(1);
  glNewList(axisList, GL_COMPILE);
      glLineWidth(2.0);
      glBegin(GL_LINES);
      glColor3f(1.0, 0.0, 0.0);       // X axis - red
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(AXIS_SIZE, 0.0, 0.0);
      glColor3f(0.0, 1.0, 0.0);       // Y axis - green
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(0.0, AXIS_SIZE, 0.0);
      glColor3f(0.0, 0.0, 1.0);       // Z axis - blue
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(0.0, 0.0, AXIS_SIZE);
    glEnd();
  glEndList();
}

///////////////////////////////////////////////

// void colourParticle(Particle* particle)
// {
//   switch (colourMode)
//   {
//     case BLOOD:
//       particle->r = 1.0;
//       particle->g = 0.0;
//       particle->b = 0.0;
//       break;
//     case SWEAT:
//       particle->r = 1.0;
//       particle->g = 1.0;
//       particle->b = 0.0;
//       break;
//     case TEARS:
//       particle->r = 0.0;
//       particle->g = 0.0;
//       particle->b = 1.0;
//       break;
//     case BRUISE:
//       particle->r = 1.0;
//       particle->g = 0.0;
//       particle->b = 1.0;
//       break;
//     case BONES:
//       particle->r = 1.0;
//       particle->g = 1.0;
//       particle->b = 1.0;
//       break;
//     case VOID:
//       particle->r = 0.0;
//       particle->g = 0.0;
//       particle->b = 0.0;
//       break;
//     default: // CASE RAINBOW
//       particle->r = myRandom();
//       particle->g = myRandom();
//       particle->b = myRandom();
//       break;
//   } // switch
// } // colourParticle

///////////////////////////////////////////////

void calcFramerate()
{
  frameCount++;
  timeCurrent = glutGet(GLUT_ELAPSED_TIME);
  float interval = timeCurrent - timeLastUpdate;
  if (interval > 1000)
  {
   framerate = frameCount / (interval / 1000.0f);
   timeLastUpdate = timeCurrent;
   frameCount = 0;
  } // if
  float intervalLastFrame = timeCurrent - timeLastFrame;
  interval = intervalLastFrame / 1000.0f;
  timeLastFrame = timeCurrent;
} // calcFramerate

///////////////////////////////////////////////

// void calcParticle(Emitter *emitter, Particle *particle)
// {
//   // ALIVE
//   if (!particle->isDead)
//   {
//     particle->positionPrevious.x = particle->position.x;
//     particle->positionPrevious.y = particle->position.y;
//     particle->positionPrevious.z = particle->position.z;
//
//     particle->position.x = particle->position.x + particle->velocity.x*interval;
//
//     particle->velocity.y = particle->velocity.y + grav * interval;
//     particle->position.y = particle->position.y + particle->velocity.y*interval;
//
//     particle->position.z = particle->position.z + particle->velocity.z*interval;
//
//     if (particle->collisionBool == 0 && particle->position.y <= -25)
//     {
//       particle->velocity.y = particle->velocity.y * -restitution;
//       particle->collisionBool = 1;
//       float x = myRandom() * 50 - 25;
//       float z = myRandom() * 50 - 25;
//       if (abs(x) <= abs(particle->velocity.y))
//       {
//         particle->velocity.x = x;
//       } // if
//       if (abs(z) <= abs(particle->velocity.y))
//       {
//         particle->velocity.z = z;
//       } // if
//     } // if
//     else if (particle->position.y > -25)
//     {
//       particle->collisionBool = 0;
//     } // else if
//     else
//     {
//       if (particle->position.y <= -25 + 0.1 && particle->velocity.y <= 0.1) {
//         particle->isDead = 1;
//       } // if
//     } // else
//   } // if
//   // DEAD
//   else
//   {
//     if (particle->timeDead > lifetime)
//     {
//       particle->position.x = emitter->centre.x + myRandom() * 25 - 12.5;
//       particle->position.y = emitter->centre.y - 6.666;
//       particle->position.z = emitter->centre.z + myRandom() * 25 - 12.5;
//
//       particle->positionPrevious.x = particle->position.x;
//       particle->positionPrevious.y = particle->position.y;
//       particle->positionPrevious.z = particle->position.z;
//
//       particle->velocity.x = (myRandom() * 10 - 5) * velFactor;
//       particle->velocity.y = (myRandom() * 5 - 5) * velFactor;
//       particle->velocity.z = (myRandom() * 10 - 5) * velFactor;
//
//       particle->collisionBool = 0;
//
//       particle->isDead = 0;
//       particle->timeDead = 0;
//
//       colourParticle(particle);
//     } // if
//     else
//     {
//       particle->timeDead += interval;
//     } // else
//   } // else
// } // calcParticle

///////////////////////////////////////////////

// void calcParticleMovement(Emitter *emitter)
// {
//   int i;
//   for (i = 0; i < emitter->particleCount; i++)
//   {
//     calcParticle(emitter, &emitter->particles[i]);
//   } // for
// } // calcParticleMovement

///////////////////////////////////////////////

// void calcEmitters()
// {
//   int i;
//   for (i = 0; i < noOfEmitters; i++)
//   {
//     calcParticleMovement(&emitters[i]);
//   } // for
// } // calcEmitters

///////////////////////////////////////////////

void idle()
{
  // calcFramerate();
  // swarm.swarming();
  // calcEmitters();
  glutPostRedisplay();
} // idle

///////////////////////////////////////////////

// void createEmitters()
// {
//   int i;
//   int j;
//
//   noOfEmitters = 3;
//
//   emitters[0].centre.x = -66.6 * 1.666;
//   emitters[0].centre.y = 66.6;
//   emitters[0].centre.z = 66.6 * 11.666;
//
//   emitters[1].centre.x = 0.666;
//   emitters[1].centre.y = 66.6;
//   emitters[1].centre.z = 666;
//
//   emitters[2].centre.x = 66.6 * 1.666;
//   emitters[2].centre.y = 66.6;
//   emitters[2].centre.z = 66.6 * 11.666;
//
//   for (i = 0; i < noOfEmitters; i++)
//   {
//     emitters[i].r = 0.722;
//     emitters[i].g = 0.847;
//     emitters[i].b = 0.745;
//
//     emitters[i].particles = (Particle*) calloc(noOfParticles, sizeof(Particle));
//     for (j = 0; j < noOfParticles; j++)
//     {
//       emitters[i].particles[j].size = pointSize - (myRandom() * 4);
//       emitters[i].particles[j].isDead = 1;
//       emitters[i].particles[j].timeDead = myRandom() * lifetime;
//     } // for
//     emitters[i].particleCount = noOfParticles;
//   } // for
// } // createEmitters

///////////////////////////////////////////////

// void reset()
// {
//   int i;
//   for (i = 0; i < MAX_EMITTERS; i++)
//   {
//     free(emitters[i].particles);
//   } // for
//   createEmitters();
// } // reset

///////////////////////////////////////////////

void keyboard(unsigned char key, int x, int y)
{
  if (key == 27) {TwTerminate(); exit(0);} // ESC
  // if (key == 32) reset(); // space bar
  if (key == 97) toggleAxis(); // a
  // if (key == 114) toggleRenderMode(); // r
  if (key == 44) // , comma
  {
    if (velFactor > 1)
      velFactor -= 0.5;
  } // if
  if (key == 46) // . dot
  {
    if (velFactor < MAX_VELOCITY_FACTOR)
      velFactor += 0.5;
  } // if
  if (key == 99) // c
  {
    if (colourMode == VOID)
    {
      colourMode = RAINBOW;
    } // if
    else
    {
      colourMode++;
    } // else
  } // if
  if (key == 91) // [ square bracket left
  {
    if (grav < MIN_GRAV)
      grav += 0.5;
  } // if
  if (key == 93) // ] square bracket right
  {
    if (grav > MAX_GRAV)
      grav -= 0.5;
  } // if
  if (key == 59) // ; semicolon
  {
    if (lifetime > MIN_LIFETIME)
      lifetime--;
  } // if
  if (key == 39) // ' apostrophe
  {
    if (lifetime < MAX_LIFETIME)
      lifetime++;
  } // if
  // if (key == 111) // o
  // {
  //   if (noOfParticles > 100000)
  //   {
  //     noOfParticles -= 10000;
  //   } // if
  //   else if (noOfParticles > 10000)
  //   {
  //     noOfParticles -= 1000;
  //   } // else if
  //   else if (noOfParticles > MIN_PARTICLES)
  //   {
  //     noOfParticles -= 100;
  //   } // else if
  // } // if
  // if (key == 112) // p
  // {
  //   if (noOfParticles < 10000)
  //   {
  //     noOfParticles += 100;
  //   } // if
  //   else if (noOfParticles < 100000)
  //   {
  //     noOfParticles += 1000;
  //   } // else if
  //   else if (noOfParticles < MAX_PARTICLES)
  //   {
  //     noOfParticles += 10000;
  //   } // else if
  // } // if
  // if (key == 100) noOfParticles = DEFAULT_PARTICLES; // d
  if (key == 45) // - minus
  {
    if (restitution > MIN_RESTITUTION)
      restitution -= 0.1;
  } // if
  if (key == 61) // = equals
  {
    if (restitution < MAX_RESTITUTION)
      restitution += 0.1;
  } // if
  glutPostRedisplay();
}

///////////////////////////////////////////////

void special(int key, int x, int y)
{
  if (key == GLUT_KEY_LEFT)
  {
    if (theta <= 0.0)
    {
      theta = 359.5;
    } // if
    else
    {
      theta -= 0.5;
    } // else
  } // LEFT

  if (key == GLUT_KEY_RIGHT)
  {
    if (theta >= 359.5)
    {
      theta = 0.0;
    } // if
    else
    {
      theta += 0.5;
    } // else
  } // RIGHT

  glutPostRedisplay();
}

///////////////////////////////////////////////

void initGraphics(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitWindowSize(1280, 720);
  glutInitWindowPosition(100, 100);
  glutInitDisplayMode(GLUT_DOUBLE);
  glutCreateWindow("Swarm Music");
  // defineLight();
  // createEmitters();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);
  makeAxes();
}

/////////////////////////////////////////////////

void timer(int val) {
  glutPostRedisplay();
  glutTimerFunc(16, timer, 0);
  swarm.swarming();
} // timer

/////////////////////////////////////////////////

int main(int argc, char *argv[])
{
  // double f;
  // srand(time(NULL));
  initGraphics(argc, argv);
  TwInit(TW_OPENGL, NULL);
  TwWindowSize(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
  glEnable(GL_NORMALIZE);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_POINT_SMOOTH);
  glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);
  glutMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
  glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
  glutKeyboardFunc((GLUTkeyboardfun)TwEventKeyboardGLUT);
  // glutSpecialFunc((GLUTspecialfun)TwEventSpecialGLUT);
  TwGLUTModifiersFunc(glutGetModifiers);
  defineUI();
  glutTimerFunc(0, timer, 0);
  glutMainLoop();
  return 0;
}
