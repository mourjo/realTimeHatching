// ------------------------------------------------------
// LibSL::Math::Quaternion
// ------------------------------------------------------
//
// Quaternion class
// 
// => modified version for ECP OpenGL TPs
//
// ------------------------------------------------------
// Sylvain Lefebvre - 2006-05-09
// ------------------------------------------------------

#pragma once

#include "Tuple.h"
#include "Matrix4x4.h"

//namespace LibSL {
//  namespace Math {

    template <typename T_Type>
    class Quaternion : public Tuple<T_Type,4>
    {
    protected:

      typedef Tuple<T_Type,3> v3;

    public:

      Quaternion() 
      {
        (*this)[0]=0; (*this)[1]=0; (*this)[2]=0; (*this)[3]=1;
      }

      Quaternion(const Tuple<T_Type,4>& t)
      {
        (*this)[0]=t[0]; (*this)[1]=t[1]; (*this)[2]=t[2]; (*this)[3]=t[3];
      }

      Quaternion(T_Type x,T_Type y,T_Type z,T_Type w)
      {
        (*this)[0]=x; (*this)[1]=y; (*this)[2]=z; (*this)[3]=w;
      }

      Quaternion(const v3& axis,T_Type angle)
      {
        (*this)[3] = cos(angle/2); 
        v3 ax      = normalize(axis);
        T_Type s   = sin(angle/2);
        (*this)[0] = ax[0]*s;
        (*this)[1] = ax[1]*s;
        (*this)[2] = ax[2]*s;
      }

      Quaternion(const v3& axis,T_Type cos_half_angle,T_Type sin_half_angle)
      {
        (*this)[0] = axis[0]*sin_half_angle;
        (*this)[1] = axis[1]*sin_half_angle;
        (*this)[2] = axis[2]*sin_half_angle;
        (*this)[3] = cos_half_angle;
      }

      //Quaternion(const matrix4x4& )

      //Quaternion(const v3& u,const v3& v,const v3& w);

      Quaternion(const Quaternion& q)
      {
        ForIndex(i,4) {
          (*this)[i]=q[i];
        }
      }

      ~Quaternion() {}

      void inverseEq()
      {
        (*this)[0]=-(*this)[0]; (*this)[1]=-(*this)[1]; (*this)[2]=-(*this)[2];
      }

      Quaternion inverse() const
      {
        return Quaternion(-(*this)[0],-(*this)[1],-(*this)[2],(*this)[3]);
      }

      Matrix4x4<T_Type> toMatrix() const
      {
        T_Type xx = (*this)[0]*(*this)[0];
        T_Type xy = (*this)[0]*(*this)[1];
        T_Type xz = (*this)[0]*(*this)[2];
        T_Type xw = (*this)[0]*(*this)[3];

        T_Type yy = (*this)[1]*(*this)[1];
        T_Type yz = (*this)[1]*(*this)[2];
        T_Type yw = (*this)[1]*(*this)[3];

        T_Type zz = (*this)[2]*(*this)[2];
        T_Type zw = (*this)[2]*(*this)[3];

        Matrix4x4<T_Type> m;
        m.eqIdentity();
        m.at(0,0) = 1 - 2*( yy + zz );
        m.at(1,0) =     2*( xy - zw );
        m.at(2,0) =     2*( xz + yw );
        m.at(0,1) =     2*( xy + zw );
        m.at(1,1) = 1 - 2*( xx + zz );
        m.at(2,1) =     2*( yz - xw );
        m.at(0,2) =     2*( xz - yw );
        m.at(1,2) =     2*( yz + xw );
        m.at(2,2) = 1 - 2*( xx + yy );
        return (m);
      }

      Quaternion exp() const
      {
        v3 ax;
        ax[0]=(*this)[0]; ax[1]=(*this)[1]; ax[2]=(*this)[2]; 
        T_Type fAngle = length(ax);
        T_Type fSin   = sin(fAngle);

        Quaternion kResult;
        kResult[3]=cos(fAngle);
        if ( fabs(fSin) > 0 ) {
          T_Type fCoeff = fSin/fAngle;
          kResult[0]=fCoeff*(*this)[0];
          kResult[1]=fCoeff*(*this)[1];
          kResult[2]=fCoeff*(*this)[2];
        } else {
          kResult[0] = (*this)[0];
          kResult[1] = (*this)[1];
          kResult[2] = (*this)[2];
        }
        return kResult;	  
      }

      Quaternion log() const
      {
        Quaternion kResult;
        kResult[3]=0;
        if ( abs((*this)[3]) < 1.0 ) {
          T_Type fAngle = acos((*this)[3]);
          T_Type fSin   = sin(fAngle);
          if ( abs(fSin) > 0 ) {
            T_Type fCoeff = fAngle/fSin;
            kResult[0]=fCoeff*(*this)[0];
            kResult[1]=fCoeff*(*this)[1];
            kResult[2]=fCoeff*(*this)[2];
            return kResult;
          }
        }
        kResult[0] = (*this)[0];
        kResult[1] = (*this)[1];
        kResult[2] = (*this)[2];
        return kResult;
      }

      static Quaternion slerp(T_Type t,
        const Quaternion& from,
        const Quaternion& to)
      {
        if (sqLength(from-to) < 1e-6)
          return from;

        T_Type fCos   = dot(from,to);
        T_Type fAngle = acos(fCos);

        if (abs(fAngle) < 0)
          return (from);

        T_Type fSin    = T_Type(sin(fAngle));
        T_Type fInvSin = T_Type(1.0/fSin);
        T_Type fCoeff0 = T_Type(sin((1.0f-t)*fAngle)*fInvSin);
        T_Type fCoeff1 = T_Type(sin(t*fAngle)*fInvSin);
        return Quaternion((fCoeff0*from) + (fCoeff1*to));	    
      }

      static Quaternion slerp_shortest(T_Type t,
        const Quaternion& from,
        const Quaternion& to)
      {
        Quaternion res,sum,dif;
        T_Type omega, cosom, sinom, scale0, scale1;

        T_Type _x = from[0];
        T_Type _y = from[1];
        T_Type _z = from[2];
        T_Type _w = from[3];

        T_Type tx = to[0];
        T_Type ty = to[1];
        T_Type tz = to[2];
        T_Type tw = to[3];

        // find shortest path on unit quaternion sphere
        if (to.dot(from) < 0.0) {
          tx=-tx; 
          ty=-ty;
          tz=-tz; 
          tw=-tw;
        }
        // calc cosine
        cosom = _x * tx + _y * ty + _z * tz + _w * tw;
        // calculate coefficients
        if ( (1.0 + cosom) > 0) {
          if ( (1.0 - cosom) > 0) {
            // standard case (slerp)
            omega = acos(cosom);
            sinom = sin(omega);
            scale0 = sin((1.0 - t) * omega) / sinom;
            scale1 = sin(t * omega) / sinom;
          } else  {        
            // "from" and "to" quaternions are very close 
            //  ... so we can do a linear interpolation
            scale0 = 1.0 - t;
            scale1 = t;
          }
        } else {
          scale0 = sin ((1.0 - t) * M_PI/2.0);
          scale1 = sin (t * M_PI/2.0);      
        }
        // calculate final values
        res[0]=scale0 * _x + scale1 * tx;
        res[1]=scale0 * _y + scale1 * ty;
        res[2]=scale0 * _z + scale1 * tz;
        res[3]=scale0 * _w + scale1 * tw;
        return (res);	    
      }

      static void squad_intermediate (const Quaternion& rkQ0,
        const Quaternion& rkQ1, 
        const Quaternion& rkQ2, 
        Quaternion& rkA)
      {
        Quaternion kQ1inv = rkQ1.inverse();
        Quaternion rkP2   = kQ1inv*rkQ2;
        Quaternion rkP0   = kQ1inv*rkQ0;
        Quaternion kArg   = (-0.25)*(rkP2.log()+rkP0.log());

        rkA = rkQ1*(kArg.exp());
      }

      static Quaternion squad(T_Type fT, 
        const Quaternion& rkP,
        const Quaternion& rkA, 
        const Quaternion& rkB,
        const Quaternion& rkQ)
      {
        T_Type     fSlerpT = 2.0*fT*(1.0-fT);
        Quaternion kSlerpP = slerp(fT,rkP,rkQ);
        Quaternion kSlerpQ = slerp(fT,rkA,rkB);
        return slerp(fSlerpT,kSlerpP,kSlerpQ);
      }

      template<typename U_Type> Tuple<U_Type,3>
      transform(const Tuple<U_Type,3>& p) const
      {
        U_Type t[4];

        t[0] =  p[0]*(*this)[0] + p[1]*(*this)[1] + p[2]*(*this)[2];
        t[1] =  p[0]*(*this)[3] - p[1]*(*this)[2] + p[2]*(*this)[1];
        t[2] =  p[0]*(*this)[2] + p[1]*(*this)[3] - p[2]*(*this)[0];
        t[3] = -p[0]*(*this)[1] + p[1]*(*this)[0] + p[2]*(*this)[3];

        Tuple<U_Type,3> r;
        r[0]=(*this)[3]*t[1] + (*this)[0]*t[0] + (*this)[1]*t[3] - (*this)[2]*t[2];
        r[1]=(*this)[3]*t[2] - (*this)[0]*t[3] + (*this)[1]*t[0] + (*this)[2]*t[1];
        r[2]=(*this)[3]*t[3] + (*this)[0]*t[2] - (*this)[1]*t[1] + (*this)[2]*t[0];
        return (r);
      }

      template<typename U_Type> Tuple<U_Type,4>
      transform(const Tuple<U_Type,4>& p) const
      {
        U_Type t[4];

        t[0] =  p[0]*(*this)[0] + p[1]*(*this)[1] + p[2]*(*this)[2];
        t[1] =  p[0]*(*this)[3] - p[1]*(*this)[2] + p[2]*(*this)[1];
        t[2] =  p[0]*(*this)[2] + p[1]*(*this)[3] - p[2]*(*this)[0];
        t[3] = -p[0]*(*this)[1] + p[1]*(*this)[0] + p[2]*(*this)[3];

        Tuple<U_Type,4> r;
        r[0]=(*this)[3]*t[1] + (*this)[0]*t[0] + (*this)[1]*t[3] - (*this)[2]*t[2];
        r[1]=(*this)[3]*t[2] - (*this)[0]*t[3] + (*this)[1]*t[0] + (*this)[2]*t[1];
        r[2]=(*this)[3]*t[3] + (*this)[0]*t[2] - (*this)[1]*t[1] + (*this)[2]*t[0];
        r[3]=p[3];
        return (r);
      }

    };

    // operators

    template<typename T_Type> Quaternion<T_Type> 
    operator * (const Quaternion<T_Type>& q0,const Quaternion<T_Type>& q1)
    {
      return Quaternion<T_Type>(
        q0[3]*q1[0]+q0[0]*q1[3]+q0[1]*q1[2]-q0[2]*q1[1],
        q0[3]*q1[1]+q0[1]*q1[3]+q0[2]*q1[0]-q0[0]*q1[2],
        q0[3]*q1[2]+q0[2]*q1[3]+q0[0]*q1[1]-q0[1]*q1[0],
        q0[3]*q1[3]-q0[0]*q1[0]-q0[1]*q1[1]-q0[2]*q1[2]);
    }

    template<typename T_Type> Tuple<T_Type,3>
    operator * (const Quaternion<T_Type>& q,const Tuple<T_Type,3>& p)
    {
      return (q.transform(p));
    }

    template<typename T_Type> Tuple<T_Type,4>
    operator * (const Quaternion<T_Type>& q,const Tuple<T_Type,4>& p)
    {
      return (q.transform(p));
    }

    // typedefs

    typedef Quaternion<float>  quatf;
    typedef Quaternion<double> quatd;

//  }; // namespace LibSL::Math
//}; // namespace LibSL
