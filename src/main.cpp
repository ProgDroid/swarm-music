/*
  @file     main.cpp
  @author   Fernando Ferreira

  The Free Sound of Self-Organisation: Swarm Music
  OpenGL code adapted from COMP37111 coursework
  Music thread code adapted from STK tutorials
*/

#include <triplet.h>
#include <agent.h>
#include <swarm.h>
#include <stdio.h>
#include <math.h>
#include <GL/freeglut.h>
#include <AntTweakBar.h>

#include <stk/Plucked.h>
#include <stk/Skini.h>
#include <stk/RtAudio.h>
#include <algorithm>
#include <thread>
#include <random>

struct TickData
{
  stk::Instrmnt *instrument;
  stk::StkFloat frequency;
  long counter;
  bool done;

  TickData()
    : instrument(0), counter(0), done(false) {}
}; // struct TickData

const float PI = 3.14159265;
const float OLIVEBLACK = 0.23529411764;
const int FIREANDFLAMES = 100;
const int METAL = 80;
const int JAZZ = 60;
const int BLUES = 40;
const int DOOM = 20;
Swarm swarm;
TwBar *leftBar;
TwBar *rightBar;
float theta = 0.0;
long pitch;
float velocity, noteLength;
bool canExit = false;
int style = JAZZ;
int positionX;
int GUIpitch = 72;
int pitchMax = 96;
int GUIscale = 0;
const int MAJOR = 0;
const int MINOR = 1;
const int PENTATONIC = 2;
const int BLUES_SCALE = 3;
bool mute = false;

float mainRand()
{
  std::random_device randomDevice;
  std::mt19937 generator(randomDevice());
  std::uniform_int_distribution<> distribution(0, 100);

  float randNo = (float) distribution(generator);
  return randNo;
} // mainRand

void TW_CALL addAttr(void * /*clientData*/)
{
  swarm.addAttractor(GUIpitch, -1);
} // addAttr

void TW_CALL muteSound(void * /*clientData*/)
{
  mute = !mute;
} // addAttr

void TW_CALL makeScale(void * /*clientData*/)
{
  swarm.resetAttractors();
  int root = GUIpitch;
  int scaleType = GUIscale;
  if (root >= 84)
  {
    root -= 12;
  } // if
  int j = 14;
  std::vector<int> intervals;
  intervals.reserve(14);
  // if major or minor
  switch (scaleType)
  {
    case MAJOR:       j = 14;
                      intervals.push_back(2);
                      intervals.push_back(2);
                      intervals.push_back(1);
                      intervals.push_back(2);
                      intervals.push_back(2);
                      intervals.push_back(2);
                      intervals.push_back(1);
                      break;
    case MINOR:       j = 14;
                      intervals.push_back(2);
                      intervals.push_back(1);
                      intervals.push_back(2);
                      intervals.push_back(2);
                      intervals.push_back(1);
                      intervals.push_back(2);
                      intervals.push_back(2);
                      break;
    case PENTATONIC:  j = 10;
                      intervals.push_back(3);
                      intervals.push_back(2);
                      intervals.push_back(2);
                      intervals.push_back(3);
                      intervals.push_back(2);
                      break;
    case BLUES:       j = 12;
                      intervals.push_back(3);
                      intervals.push_back(2);
                      intervals.push_back(1);
                      intervals.push_back(1);
                      intervals.push_back(3);
                      intervals.push_back(2);
                      break;
  } // switch

  for (int i = 0; i < j; i++)
  {
    int half = j / 2;
    int index = i % half;
    swarm.addAttractor(root + intervals[index], index);
  } // for
} // makeScale

void defineUI()
{
  // global ui properties
  TwDefine("GLOBAL help='The Free Sound of Self-Organisation' fontsize=1 \
            fontresizable=false");
  // left sidebar
  leftBar = TwNewBar("Swarm Properties");
  TwDefine("'Swarm Properties' position='0 0' size='200 120' valueswidth=35 \
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

  // right sidebar
  rightBar = TwNewBar("Musical Properties");
  TwDefine("'Musical Properties' position='0 150' size='200 120' valueswidth=35 \
             color='193 131 159' alpha=255 movable=false resizable=false");
  // control chance of note being played
  TwEnumVal styleMode[] =
  {
    {DOOM, "DOOM"},
    {BLUES, "Blues"},
    {JAZZ, "Jazz"},
    {METAL, "Metal"},
    {FIREANDFLAMES, "Insane"}
  }; // styleMode
  TwType modeType = TwDefineEnum("Style", styleMode, 5);
  TwAddVarRW(rightBar, "Style", modeType, &style, "keyIncr=n keyDecr=N \
             help='From a gloomy to non-stop.'");
  // control whether to get random coordinates from agents or average of all
  TwEnumVal swarmMode[] =
  {
    {0, "Random"},
    {1, "Average"},
  }; // swarmMode
  TwType swarmType = TwDefineEnum("Mapping", swarmMode, 2);
  TwAddVarRW(rightBar, "Mapping", swarmType, &swarm.swarmMode, "keyIncr=m \
             keyDecr=M help='Pick random coordinates from swarm agents or average of all.'");
  TwEnumVal pitchMode[] =
  {
    {72, "C4"}, {73, "C#4"}, {74, "D4"}, {75, "D#4"},
    {76, "E4"}, {77, "F4"}, {78, "F#4"}, {79, "G4"},
    {80, "G#4"}, {81, "A4"}, {82, "A#4"}, {83, "B4"},
    {84, "C5"}, {85, "C#5"}, {86, "D5"}, {87, "D#5"},
    {88, "E5"}, {89, "F5"}, {90, "F#5"}, {91, "G5"},
    {92, "G#5"}, {93, "A5"}, {94, "A#5"}, {95, "B5"},
    {96, "C6"}
  }; // swarmMode
  TwType pitchType = TwDefineEnum("Pitches", pitchMode, 25);
  // pick pitch for new attractor
  TwAddVarRW(rightBar, "Pitch", pitchType, &GUIpitch,
             "label='Pitch for attractors or scale' keyIncr='p' keyDecr='P' \
              help='The pitch for attractors to be added.'");
  // const int lowest = 72;
  // TwSetParam(rightBar, "Pitch", "min", TW_PARAM_INT32, 1, &lowest);
  // TwSetParam(rightBar, "Pitch", "max", TW_PARAM_INT32, 1, &pitchMax);
  // add attractor
  TwAddButton(rightBar, "Add Attractor", addAttr, nullptr,
              "label='Add an attractor' help='Add attractor with pitch from above.'");
  // pick scale type
  TwEnumVal scaleMode[] =
  {
    {MAJOR, "Major"},
    {MINOR, "Minor"},
    {PENTATONIC, "Pentatonic Minor"},
    {BLUES, "Blues"},
  }; // scaleMode
  TwType scalesType = TwDefineEnum("Scale Type", scaleMode, 4);
  TwAddVarRW(rightBar, "Scale Type", scalesType, &GUIscale, "keyIncr=t \
             keyDecr=T help='Pick the type of scale to be added with attractors.'");
  // button to make scale based on pitch
  TwAddButton(rightBar, "Add Scale", makeScale, nullptr, "label='Add a scale' \
              help='Add a scale with the pitch above as root.'");
  // button to mute/unmute
  TwAddButton(rightBar, "Mute On/Off", muteSound, nullptr, "label='Mute On/Off' \
              help='Mute sound output.'");
} // defineUI

void drawWireCube()
{
  glColor4f(0.0, 0.0, 0.0, 1.0);
  glPushMatrix();
    glutWireCube(800.0);
  glPopMatrix();
} // drawWireCube

void display()
{
  glLoadIdentity();
  gluLookAt(1250*sin(theta*(PI / 180)), 500.0, 1250*cos(theta*(PI / 180)),
            0.0, -100.0, 0.0,
            0.0, 1.0, 0.0);

  glClear(GL_COLOR_BUFFER_BIT);

  swarm.drawAgents();
  drawWireCube();
  if (swarm.getAttractors() != 0)
  {
    swarm.drawAttractors();
  } // if

  // draw UI
  TwDraw();

  glutSwapBuffers();
} // display

// from coursework + passing the UI the window size
void reshape(int width, int height)
{
  glClearColor(OLIVEBLACK, OLIVEBLACK, OLIVEBLACK, 1.0);
  glViewport(0, 0, (GLsizei)width, (GLsizei)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 10000.0);
  glMatrixMode(GL_MODELVIEW);
  TwWindowSize(width, height);
  positionX = width - 200;
}

void idle()
{
  glutPostRedisplay();
} // idle

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

  // to exit cleanly
  if (key == GLUT_KEY_HOME)
  {
    glutLeaveMainLoop();
    canExit = true;
  } // HOME

  if (key == GLUT_KEY_PAGE_UP)
  {
    swarm.resetAll();
  } // if

  if (key == GLUT_KEY_PAGE_DOWN)
  {
    swarm.resetAttractors();
  } // if

  glutPostRedisplay();
} // special

int tick(void *outputBuffer, void *inputBuffer,
         unsigned int nBufferFrames, double streamTime,
         RtAudioStreamStatus status, void *userData)
{
  TickData *data = (TickData *) userData;
  register stk::StkFloat *samples = (stk::StkFloat *) outputBuffer;

  for (unsigned int i = 0; i < nBufferFrames; i++)
  {
    *samples++ = data->instrument->tick();
    if (++data->counter % 2000 == 0)
    {
      data->instrument->setFrequency(data->frequency);
    } // if
  } // for

  int lengthFactor = 110;
  if (style == DOOM || style == BLUES)
  {
    lengthFactor = 200;
  } // if
  else if (style == JAZZ || style == METAL)
  {
    lengthFactor = 100;
  } // else if
  else if (style == FIREANDFLAMES)
  {
    lengthFactor = 50;
  }

  if (data->counter > noteLength*lengthFactor)
  {
    data->done = true;
  } // if

  return 0;
} // tick

void playMusic()
{
  // x coordinate determines pitch between C4=72 and C6=96,
  // for a range of 2 octaves
  pitch = ((swarm.averagePosition.getX() + 400) * 24) / 800;
  if (pitch < 0)
  {
    pitch = 0;
  } // if
  else if (pitch > 24)
  {
    pitch = 24;
  } // else if
  pitch += 72;

  // y coordinate determines velocity ("volume" at which individual
  // note is played), 0.0 to 1.0
  velocity = ((swarm.averagePosition.getY() + 400.0)) / 800.0;
  if (velocity < 0)
  {
    velocity = 0;
  } // if
  else if (velocity > 1.0)
  {
    velocity = 1.0;
  } // else if

  // z coordinate determines note length in ms
  noteLength = ((swarm.averagePosition.getZ() + 400.0)) / 800.0;
  noteLength = noteLength * 250;
} // playMusic

void music()
{
  stk::Stk::setSampleRate(44100.0);
  stk::Stk::setRawwavePath("./stk-4.6.0/rawwaves");

  TickData data;
  RtAudio dac;

  RtAudio::StreamParameters parameters;
  parameters.deviceId = dac.getDefaultOutputDevice();
  parameters.nChannels = 1;
  RtAudioFormat format = (sizeof(stk::StkFloat) == 8) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
  unsigned int bufferFrames = stk::RT_BUFFER_SIZE;

  try
  {
    dac.openStream(&parameters, NULL, format, (unsigned int) stk::Stk::sampleRate(), &bufferFrames, &tick, (void *) &data);
  } // try
  catch (RtAudioError &error)
  {
    error.printMessage();
    return;
  } // catch

  try
  {
    // 261.63
    data.instrument = new stk::Plucked(261.63);
  } // try
  catch (stk::StkError &)
  {
    delete data.instrument;
    return;
  } // catch

  try
  {
    dac.startStream();
  } // try
  catch (RtAudioError &error)
  {
    error.printMessage();
    delete data.instrument;
    return;
  } // catch

  while(!canExit)
  {
    data.frequency = stk::Midi2Pitch[pitch];
    if (mainRand() < (float) style && !mute)
    {
      data.instrument->noteOn(data.frequency, velocity);
    } // if

    while(!data.done)
    {
      stk::Stk::sleep(noteLength);
    } // while

    data.counter = 0;
    data.done = false;
  } // while

  try
  {
    dac.closeStream();
  } // try
  catch (RtAudioError &error)
  {
    error.printMessage();
  } // catch

  delete data.instrument;

  return;
  } // setupMusic()

void timer(int val) {
  glutPostRedisplay();
  glutTimerFunc(16, timer, 0);
  swarm.swarming();
  playMusic();
} // timer

void initGraphics(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitWindowSize(1280, 720);
  glutInitWindowPosition(100, 100);
  positionX = 1280 - 200;
  glutInitDisplayMode(GLUT_DOUBLE);
  glutCreateWindow("Swarm Music");
  glutDisplayFunc(display);
  glutSpecialFunc(special);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);
  glutTimerFunc(0, timer, 0);
}

int main(int argc, char *argv[])
{
  initGraphics(argc, argv);
  TwInit(TW_OPENGL, NULL);
  TwWindowSize(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
  glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);
  glutMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
  glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
  glutKeyboardFunc((GLUTkeyboardfun)TwEventKeyboardGLUT);
  glutSpecialFunc(special);
  TwGLUTModifiersFunc(glutGetModifiers);
  defineUI();
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_POINT_SMOOTH);
  std::thread soundThread(music);
  glutMainLoop();
  soundThread.join();

  return 0;
}
