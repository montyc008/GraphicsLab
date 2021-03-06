
/* Contains Abstract Shape class and corresponding Rect and cube classes
 * Author:Mahendra Chouhan(14CS60R12)
 */ 

#ifndef __SHAPE__
#define __SHAPE__
 
#include <iostream>
#include <math.h>
#include <fstream>
#include <stdlib.h>
#include <vector>

#include "Point3D.h"
#include "Smasher.h"
/*Parent Shape class for all shapes
 * any new shape must inherit this
 * Note: All movement must be relative to origin
 * if absolute values are needed than compute using getAbs()
 * */
class Shape
{
      public:
      Point3D origin;
      int degX,degY,degZ;
      //generate absolute values of shape (if required)  
      virtual void getAbsolutes(Shape &) = 0;
      virtual void Rotate(int,int,int)= 0;
      virtual void Scale(float)= 0;
      virtual void Move(Point3D &)= 0;
      virtual bool Collision(Shape &)= 0;
      virtual void Write(std::ofstream &out) = 0;
      Point3D getOrigin() { return origin; }
};

class Rect:private Shape
{
      //Point3D origin;

      Point3D Points[4];

      public:
      friend class Cube;
      friend class Smasher;
      friend bool CheckCollision(std::vector<class Cube> &,class Cube ,std::vector<class Cube> &);
      friend Point2D HandleCollision(std::vector<class Cube> &,class Cube ,std::vector<class Cube> &);

      Rect() {}
      Rect(Point3D origin,Point3D p0,Point3D p1,Point3D p2,Point3D p3)
      {
        this->origin = origin;
            Points[0] = p0;Points[1] = p1;
        Points[2] = p2;Points[3] = p3;
      }
      Point3D getOrigin() { return origin;}
      void setOrigin(Point3D P) { origin = P; }
      Point3D * getPoints() { return Points;}
      //@not implemented
      //~ Rect(Point3D origin,int orientation,int length,int breadth)
      /*use the file extension in write to call different write functions*/
      //void Write(char *file) {}
      void getAbsolutes(Shape &S) {}
      void Write(std::ofstream &out);    
      bool isAbove(Rect &R2)
      {
        return (R2.origin.z < origin.z);
      }
      //Not implemented
      void Rotate(int Degx,int Degy,int Degz) {}
      void Scale(float S){}
      void Move(Point3D &P){}

      Rect Project(Point3D viewPoint);      
      bool Collision(Shape &S)
      {
            //@if( dynamic_case<Cube
            return false;
      }
      bool CollisionSelf(Rect &R2);    
      void Translate(Point3D P)
      {
        for(int i = 0;i<4;++i) Points[i] = Points[i] + P;
        origin = origin + P;
      }

      bool check_inside(Point3D q);
};

class Cube:public Shape
{
	//@Point3D origin;
	Rect Faces[6];
	Shape::origin;
	public:

	friend class Smasher;
	friend bool CheckCollision(std::vector<Cube>&, Cube, std::vector<Cube> &);
	friend Point2D HandleCollision(std::vector<Cube> &,Cube ,std::vector<Cube> &);

	//length - along x axis
	//bredth - along y axis , depth - along z axis
	Cube(Point3D orig,int length,int breadth,int depth);
	Cube(const char *objfile)
	{
		std::ifstream in(objfile);
		int x=0,y=0,z=0;
		int l=0,b=0,h=0;
		in>>l>>b>>h>>x>>y>>z;
		std::cout<<"\nCube{"<<l<<b<<h<<x<<y<<z<<"}\n";

		*this = Cube(Point3D(x,y,z),l,b,h);
		in.close();	
	}
	Cube(int l)
	{
		*this = Cube(Point3D(),l,l,l);
	}
	Cube()
	{
		*this = Cube(Point3D(),0,0,0);
	}
	void Scale(float S);
	void Rotate(int Degx,int Degy,int Degz);
      
	//@Inline so that can be executed faster
	void getAbsolutes(Shape &S);      
	//~ A relative movement
	void Translate(Point3D &P)
	{
		origin = origin + P;
	}
	//~ Absolute movement of cube to that point
	void Move(Point3D &P)
	{
		origin = P;    
	}

	bool Collision(Shape &S){ return false;}
	//@doesnot projects hidden space
	//@standard write function inherited from shape class
	void Write(std::ofstream &out);
      	
      void write(const char *svgFile);
	//projects hidden faces
	void writeHidden(const char*svgFile);
	//~ returns projection at z = 0,
      //~ *stores initial z ordering of cube 
      Cube Project(Point3D viewPoint);
	//@Writes to ofstream and uses the id to determine vertex position
	void writeObj(std::ofstream &out,int ID);
};

#endif

