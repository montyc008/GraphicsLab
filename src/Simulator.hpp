#ifndef __SIMULATION__
#define __SIMULATION__

#include <stdlib.h>
#include <GL/glut.h>

#include "Shape.h"
#include "Sphere.hpp"
#include "Cube.hpp"


            int Width = 100,Height = 100;
            ShapeList World;

            void Reshape(int w, int h) ;
            void RenderScene(void);
            void NormalKeyEvent(unsigned char key, int x, int y) ;
            void SpecialKeyEvent(int key, int x, int y) ;
            void MouseEvent(int button, int state, int x, int y);
            void drawSphere(Sphere &s);
            void drawCube(Cube &c);

            void SimulatorInit(int argc, char *argv[],int W,int H);
            

#endif
