
//TODO: define Screen Size in graphics.h

/* Contains defination of Abstract Shape class and corresponding Rect and cube classes
 * Author:Mahendra Chouhan(14CS60R12)
 */ 
#include "graphics.h" 
#include "Shape.h"


Rect Rect::Project(Point3D viewPoint)
{      
	Point3D r,p;
      
      p = origin;
      r = p - viewPoint;
      //r = r/r.mod(); //unit vector^ in our points direction
      origin = viewPoint + r*(-viewPoint.z/r.z);
      //C.origin.z = p.z;            
      for(int j = 0;j<4;++j)
      {
            p = Points[j];
            r = p - viewPoint;
            //r = r/r.mod(); //unit vector^ in our points direction
            Points[j] = viewPoint + r*(-viewPoint.z/r.z) ;
      }
      
      return *this;
}


bool Rect::CollisionSelf(Rect &R2)    
{   
      //@Assuming absolute values
      bool flag = false;
      for(int i = 0;i<4;++i)
      {
      flag = check_inside(R2.Points[i]) or 
            R2.check_inside(Points[i]) or flag;
      }
      flag = check_inside(R2.origin) or flag;
      return flag;
}

bool Rect::check_inside(Point3D q)
{
      Vector prev;
      for(int i = 0;i<4;++i)
      {
            Vector V = Points[(i+1)%4] - Points[i],
            Q = q - Points[i];
            Vector R = V.X(Q);//cross product

            if( i == 0) prev = R;

            if( (prev.z > 0 && R.z > 0) or
            (prev.z < 0 && R.z < 0) or (!prev.z ^ !R.z)) 
            prev = R;
            else return false;
      }	
      return true;
}

void Rect::Write(std::ofstream &out)
{
      //@Add origin to all points
      for(int i = 0;i<3;++i)
          writeLine(Points[i],Points[i+1],out);
      writeLine(Points[0],Points[3],out);
      writePoint(origin,out); 
}

Cube::Cube(Point3D orig,int length,int breadth,int depth)
{	
      Point3D temp;
      Point3D Points[8];

      origin = orig;
      temp.y -= breadth/2;temp.x -= length/2;temp.z += depth/2;
      
      Points[0] = temp;
      temp.x += length;Points[1] = temp;
      temp.y += breadth;Points[2] = temp;
      temp.x -= length;Points[3] = temp;
      temp.z -= depth;Points[4] = temp;
      temp.y -= breadth;Points[5] = temp;
      temp.x += length;Points[6] = temp;
      temp.y += breadth;Points[7] =temp;
      
      temp = Point3D(0,0,depth/2);
      Faces[0] = Rect(temp,
                  Points[0],Points[1],Points[2],Points[3]);                
      temp = Point3D(0,0,-depth/2);
      Faces[1] = Rect(temp,
                  Points[4],Points[5],Points[6],Points[7]);
      temp = Point3D(0,breadth/2,0);
      Faces[2] = Rect(temp,
                  Points[2],Points[3],Points[4],Points[7]);
      temp = Point3D(0,-breadth/2,0);
      Faces[3] = Rect(temp,
                  Points[0],Points[1],Points[6],Points[5]);
      temp = Point3D(length/2,0,0);
      Faces[4] = Rect(temp,
                  Points[1],Points[2],Points[7],Points[6]);
      temp = Point3D(-length/2,0,0);
      Faces[5] = Rect(temp,
                  Points[0],Points[3],Points[4],Points[5]);		
}

 
void Cube::getAbsolutes(Shape &S)
{
      Cube &c = dynamic_cast<Cube&>(S);
      c.origin = origin;
      for(int i = 0;i<6;++i)
            for(int j = 0;j<4;++j)
            {
                  c.Faces[i].Points[j] = 
                  Faces[i].Points[j] + origin;
                  c.Faces[i].origin = Faces[i].origin+origin;
            }      
}

void Cube::Scale(float S)
{
      float MatS[4][4] = { S, 0, 0,  0,
                         0, S, 0,  0,
                         0, 0, S,  0,
                         0, 0, 0,  1 };
	for(int i = 0;i<6;++i)
      {
            for(int j = 0;j<4;++j)
            {
                Faces[i].Points[j].Multiply(MatS);
            }
            Faces[i].origin.Multiply(MatS);
      }    
}

void Cube::Rotate(int Degx,int Degy,int Degz)
{
    float   radx = Rad(Degx),
            rady = Rad(Degy),
            radz = Rad(Degz);
	
    float MatZ[4][4] = { cosf(radz), -sinf(radz), 0,  0,
                         sinf(radz), cosf(radz),  0,  0,
                         0       ,  0      ,    1,  0,
                         0       ,  0      ,    0,  1 };
                         
      float MatY[4][4] = { cosf(rady), 0,    sinf(rady),0,
                         0       , 1,       0    ,0,
                         -sinf(rady), 0,   cosf(rady),0,
                         0       ,  0      ,  0  ,1 };
                         
	float MatX[4][4] = { 1       ,  0      ,  0     ,0,
                         0,     cosf(radx), -sinf(radx) ,0,
                         0,     sinf(radx), cosf(radx)  ,0,
                         0,         0   ,       0   ,1 };
	for(int i = 0;i<6;++i)
      {
            for(int j = 0;j<4;++j)
            Faces[i].Points[j].Multiply(MatX)->
                               Multiply(MatY)->
                               Multiply(MatZ);
            Faces[i].origin.Multiply(MatX)->
                            Multiply(MatY)->
                            Multiply(MatZ);                   
      }
}

void Cube::writeHidden(const char *svgFile)
{
	Cube C;
	getAbsolutes(C);
	std::ofstream out(svgFile);
    
	out<<"<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" height=\""<<HEIGHT<<"\" width=\""<<WIDTH<<"\">\n";

      for(int i = 0;i<6;++i)
            C.Faces[i].Write(out);
      out<<"</svg>\n";
    
	out.close();
}

Cube Cube::Project(Point3D viewPoint)
{
      Cube tc = *this;
	Cube C;
      getAbsolutes(C);
      
	Point3D r,p;
      
      p = C.origin;
      r = p - viewPoint;
      
      //~ float xDeg = atan2(r.y,r.z);
      //~ float yDeg = atan2(r.x,r.z);
      //~ tc.Rotate(Deg(xDeg)/2,0,0);
      //~ tc.Rotate(0,yDeg,0);
	//~ tc.getAbsolutes(C);
      //r = r/r.mod(); //unit vector^ in our points direction
      
      C.origin = viewPoint + r*(-viewPoint.z/r.z);
      C.origin.z = p.z;
      
      for(int i = 0;i<6;++i)
      {
            
            for(int j = 0;j<4;++j)
            {
                  p = C.Faces[i].Points[j];
                  r = p - viewPoint;
                  //r = r/r.mod(); //unit vector^ in our points direction
                  C.Faces[i].Points[j] = viewPoint + r*(-viewPoint.z/r.z) - C.origin;
                  //reduce c.origin because all movements are relative to origin 
                  C.Faces[i].Points[j].z = p.z;
            }
            
            p = C.Faces[i].origin;
            r = p - viewPoint;
            //r = r/r.mod(); //unit vector^ in our points direction
            C.Faces[i].origin = viewPoint + r*(-viewPoint.z/r.z) - C.origin;
            C.Faces[i].origin.z = p.z;
      }
      return C;
}


void Cube::Write(std::ofstream &out)
{
    Cube C;
    getAbsolutes(C);
      
    for(int i = 0;i<6;++i)
    {
        bool collision = false;
        for(int j = 0;j<6;++j)
        {
            if( i == j) continue;  
            if(C.Faces[i].CollisionSelf(C.Faces[j]))
            {
                collision = true;
                if(C.Faces[i].isAbove(C.Faces[j]))
                    C.Faces[i].Write(out);
                break;
            }
        
        }
        if(!collision)
            C.Faces[i].Write(out);
    }
        
}

void Cube::write(const char *svgFile)
{
	std::ofstream out(svgFile);    
	out<<"<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" height=\""<<HEIGHT<<"\" width=\""<<WIDTH<<"\">\n";
      Write(out);
      out<<"</svg>\n";
    	out.close();
}

void Cube::writeObj(std::ofstream &out,int ID)
{
      Cube C;
      getAbsolutes(C);
      
      for(int i = 0;i<2;++i)
            for(int j = 0;j<4;++j)
            {
                  out<<"v ";
                  out<<C.Faces[i].Points[j]<<std::endl;
            }
            
      out<<"f "<<ID+0<<" "<<ID+1<<" "<<ID+2<<" "<<ID+3<<std::endl;            
      out<<"f "<<ID+4<<" "<<ID+5<<" "<<ID+6<<" "<<ID+7<<std::endl;            
      out<<"f "<<ID+2<<" "<<ID+3<<" "<<ID+4<<" "<<ID+7<<std::endl;            
      out<<"f "<<ID+0<<" "<<ID+1<<" "<<ID+6<<" "<<ID+5<<std::endl;            
      out<<"f "<<ID+1<<" "<<ID+6<<" "<<ID+7<<" "<<ID+2<<std::endl;            
      out<<"f "<<ID+0<<" "<<ID+3<<" "<<ID+4<<" "<<ID+5<<std::endl;            
      out<<std::endl;
}
