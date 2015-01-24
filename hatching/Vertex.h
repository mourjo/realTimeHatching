// ------------------------------------------------------
// LibSL::Math
// ------------------------------------------------------
//
// Vertex class
//
// => modified version for ECP OpenGL TPs
//
// ------------------------------------------------------
// Sylvain Lefebvre - 2006-04-28
// ------------------------------------------------------

#pragma once

#include "Tuple.h"
#include <vector>

//namespace LibSL {
//  namespace Math {

//    typedef Tuple<half,  2> v2h;
    typedef Tuple<float, 2> v2f;
    typedef Tuple<double,2> v2d;

//    typedef Tuple<half,  3> v3h;
    typedef Tuple<float, 3> v3f;
    typedef Tuple<double,3> v3d;
    
//    typedef Tuple<half,  4> v4h;
    typedef Tuple<float, 4> v4f;
    typedef Tuple<double,4> v4d;

    //! constructors

    v2f V2F(float x,float y);        

    v2d V2D(double x,double y);

    v3f V3F(float x,float y,float z);

    v3d V3D(double x,double y,double z);

    v4f V4F(float x,float y,float z,float w);

    v4d V4D(double x,double y,double z,double w);

    //! vertex operators

    template<class T_Type> Tuple<T_Type,3> cross
      (const Tuple<T_Type,3>& t0,const Tuple<T_Type,3>& t1)
    {
      Tuple<T_Type,3> r;
      r[0]=t0[1]*t1[2] - t0[2]*t1[1];
      r[1]=t0[2]*t1[0] - t0[0]*t1[2];
      r[2]=t0[0]*t1[1] - t0[1]*t1[0];
      return (r);
    } 

    template<class T_Type> std::pair<Tuple<T_Type,3>,Tuple<T_Type,3> > frame
      (const Tuple<T_Type,3>& n)
    {
      Tuple<T_Type,3> u,v;
      u[0]=-n[1];
      u[1]= n[0];
      u[2]= 0;
      if (sqLength(u) < 1e-5) {
        u[0]= n[2];
        u[1]= 0;
        u[2]=-n[0];
        if (sqLength(u) < 1e-5) {
         // throw LibSL::Errors::Fatal("LibSL::Math::Vector::frame - cannot create frame!");
        }
      }
      u=normalize(u);
      v=cross(n,u);
      v=normalize(v);
      return (std::make_pair(u,v));
    } 

//  } // namespace LibSL::Math
//} // namespace LibSL
