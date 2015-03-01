
#include "Point3D.h"
#include "Dcel.hpp"
#include "Segments.hpp"

int HalfEdge::counter = 0;
int Face::counter = 0;
HEdgeList Edge::HEDGE_LIST;
int Vertex::counter = 0;

bool HalfEdge::intersect(HalfEdge *e)
{
      if( this->twin == e || this == e ) return true;
      
      Line L1(*origin,*dest()),L2(*e->origin,*e->dest());

      Point2D p;
      if( L1.intersect(L2,p))
      {
            if( p == (origin->origin) || p == (dest()->origin) || p == e->origin->origin || p == e->dest()->origin  )
                  return false;                  
            return true;
      }
      return false;
}

HalfEdge *Vertex::searchEdge(Face *f)
{
      for(unsigned int i = 0;i<out_edges.size();++i)
            if( f == out_edges[i]->face ) return out_edges[i];
      return NULL;      
}
Face *Vertex::getCommonFace(Vertex *v)
{
      for(unsigned int i = 0;i<out_edges.size();++i)
            for( unsigned int  j = 0; j < v->out_edges.size() ; ++j)
            { 
                  if(out_edges[i]->face == out_edges[j]->face)
                        return out_edges[i]->face;
            }
      return NULL;
}

int Vertex::getType()
{
      switch(out_edges.size())
      {
            case 0: return ORPH;
            case 1: return TERM;
            default: return LINK;
      } 
}

bool Vertex::IsLinked(Vertex *v2)
{
      HalfEdge *e = out_edges[0];
      while( e->next != NULL && e->next != out_edges[0]) 
      {
            //~ Careful!! can lead to infinite loop
            e = e->next;
            if( e->origin == v2 ) return true;
      }
      return false;
}
Vertex & Vertex::operator >> ( Vertex &V2 )
{
      /*Idiot proofing *
      for(int i = 0;i<V2.out_edges.size();++i)
      {     
            if( V2.out_edges[i]->twin->origin == this )
            {      
                  std::cout<<"\nAlready Connected\n";
                  return V2;
            }
      }*/
      if( getType() == ORPH && V2.getType() == TERM )
            Edge(&V2,this);
      else  Edge(this,&V2);
      
      return V2;
}

Edge::Edge(Vertex *v1,Vertex *v2)
{
      HalfEdge *e1 = new HalfEdge();
      HalfEdge *e2 = new HalfEdge();
      memset(e1,sizeof(HalfEdge),0);
      memset(e2,sizeof(HalfEdge),0);
      
      e1->twin = e2;e2->twin = e1;
      e1->origin = v1;e2->origin = v2;
      //Assume v2 is a orphan 
      if( v1->out_edges.size() == 0 ) //both are orphans no need to assign pre/next
      {
            Face *f = new Face();
            e1->face = e2->face = f;
            f->edge = e1;     
      }
      else if ( v1->out_edges.size() == 1 && v2->out_edges.size() == 0 )
      {
            HalfEdge *e = v1->out_edges[0];
            e->prev = e2;e2->next = e;
            e = e->twin;
            e->next = e1;e1->prev = e;
            
            e1->face = e2->face = e->face;
      }
      else //link - link handling 
      {
            Face *f = v1->getCommonFace(v2);
            assert(f != NULL);
            
            e1->next = v2->searchEdge(f);
            e2->next = v1->searchEdge(f);
            
            if(v1->out_edges.size() == 1 && v2->out_edges.size() == 1) 
            {
                  e1->prev = e2->next->twin;
                  e2->prev = e1->next->twin;
            }
            else 
            {
                  e1->prev = e2->next->prev;
                  e2->prev = e1->next->prev;
            }
            e2->next->prev = e2->prev->next = e2;
            e1->next->prev = e1->prev->next = e1;

            e1->face = f;                        
            /*Assign new face to dcel elements */
            f = new Face();
            for( HalfEdge *e = e2->next;e != e2;e = e->next)
                  e->face = f;
            e2->face = f;
      }
            
      v1->out_edges.push_back(e1);
      v2->out_edges.push_back(e2);
            
      HEDGE_LIST.push_back(e1);
      //HEDGE_LIST.push_back(e2);
}      

void writeDcel(VerList &list,std::ofstream &out,const char * color )
{
      for(int i = 0;i<list.size();++i)
      {
            for(int j = 0; j<list[i].out_edges.size();++j)
            {
                  HalfEdge  *e = list[i].out_edges[j];
                  writeLine(e->origin->origin,e->dest()->origin,out,color);
            }
      }
}
void writeDcel(HEdgeList &list,std::ofstream &out,const char * color)
{
      for(int i = 0;i<list.size();++i)
      {
            HalfEdge  *e = list[i];
            writeLine(e->origin->origin,e->dest()->origin,out,color);
      }

}