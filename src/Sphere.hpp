
#ifndef __SPHERE__
#define __SPHERE__

#include <fstream>
#include <stdlib.h>

#include "Point3D.h"
#include "Shape.h"
#include "graphics.h"

class Sphere : public Shape
{            
      float radius;
      Shape::origin;
      public:
      Color col;
      //~ bool light;
      
      Sphere() :radius(0) { mass = 0; }
      Sphere(Point3D orig,float rad,float m)       
      {
            //~ light = l;
            Shape::size = radius = rad;
            mass = m;
            origin = orig;
            col.a =  ((float)(rand()%100))/100;
            col.r =  ((float)(rand()%100))/100;
            col.g =  ((float)(rand()%100))/100; 
            col.b =  ((float)(rand()%100))/100;
      }
      
      friend std::ifstream & operator >>(std::ifstream &in,Sphere &S)
      {
            in>>S.origin>>S.radius>>S.mass;
            S.size = S.radius; 
            in>>S.col;
            return in;
      }
      //don't need complicated stuff here :P 
      void getAbsolutes(Shape &s)  { s = *this; }
      void Rotate(int xdeg,int ydeg,int zdeg) { }       
      void Scale(float S) {  radius *= S; }
      
      void Draw() ;      
      bool CollisionSelf( Sphere &s) ;
	bool Collision(Shape &S);
      void Write(std::ofstream &out) ;

      friend class Smasher;
      void setRad(float r) { radius = r;}
      float getRad(void )  { return radius; }
};

#endif 
