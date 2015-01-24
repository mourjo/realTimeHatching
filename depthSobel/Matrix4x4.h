// ------------------------------------------------------

// LibSL::Math

// ------------------------------------------------------

//

// Matrix 4x4

//

// Layout is row major

// Access with at(column,row)

//

// => modified version for ECP OpenGL TPs

//

// ------------------------------------------------------

// Sylvain Lefebvre - 2006-06-13

// ------------------------------------------------------



#pragma once







#include "Math.h"

#include "Tuple.h"

#define M_PI 3.14

extern double cot(double i);


//namespace LibSL {

//  namespace Math {



#define M(IJ) const T_Type& m##IJ=(*this)[_##IJ]; // helper to map matrix entries to mij variables



    template <typename T_Type> class Quaternion;



    //! Matrix 4x4 class

    template <typename T_Type>

    class Matrix4x4 : public Tuple<T_Type,16>

    {

    public:



      enum { // column/row

        _00 = 0,

        _10 = 1,

        _20 = 2,

        _30 = 3,

        _01 = 4,

        _11 = 5,

        _21 = 6,

        _31 = 7,

        _02 = 8,

        _12 = 9,

        _22 =10,

        _32 =11,

        _03 =12,

        _13 =13,

        _23 =14,

        _33 =15};



        //! Undefined matrix

        Matrix4x4()

        {

        }



        //! Matrix from table

        explicit Matrix4x4(T_Type *p)

        {

          ForIndex(n,16) {

            (*this)[n]=p[n];

          }

        }



        //! Matrix from Tuple

        Matrix4x4(const Tuple<T_Type,16>& t)

        {

          ForIndex(n,16) {

            (*this)[n]=t[n];

          }

        }



        //! Matrix from Tuple

        Matrix4x4(const Matrix4x4& t)

        {

          ForIndex(n,16) {

            (*this)[n]=t[n];

          }

        }



        //! Matrix from quaternion, scaling and translation

        Matrix4x4(const Quaternion<T_Type>& q,const Tuple<T_Type,3>& s,const Tuple<T_Type,3>& t)

        {

          (*this)=q.toMatrix();

          ForIndex(i,3) {

            at(i,i)*=s[i];

          }

          ForIndex(i,3) {

            at(3,i)=t[i];

          }

        }



        //! Matrix from values

        Matrix4x4(

          T_Type m00,T_Type m10,T_Type m20,T_Type m30,

          T_Type m01,T_Type m11,T_Type m21,T_Type m31,

          T_Type m02,T_Type m12,T_Type m22,T_Type m32,

          T_Type m03,T_Type m13,T_Type m23,T_Type m33)

        {

          (*this)[_00]=m00; (*this)[_10]=m10; (*this)[_20]=m20; (*this)[_30]=m30;

          (*this)[_01]=m01; (*this)[_11]=m11; (*this)[_21]=m21; (*this)[_31]=m31;

          (*this)[_02]=m02; (*this)[_12]=m12; (*this)[_22]=m22; (*this)[_32]=m32;

          (*this)[_03]=m03; (*this)[_13]=m13; (*this)[_23]=m23; (*this)[_33]=m33;

        }

        


        //! Set to identity matrix

        void eqIdentity()

        {

          this->fill(T_Type(0));

          ForIndex(i,4) {

            at(i,i)=T_Type(1);

          }

        }



        //! Matrix determinant

        T_Type det() const

        {

          M(00); M(10); M(20); M(30);

          M(01); M(11); M(21); M(31);

          M(02); M(12); M(22); M(32);

          M(03); M(13); M(23); M(33);

          return (

            m00*m33*m11*m22 - m00*m23*m11*m32 + m00*m23*m12*m31 + m00*m13*m21*m32

            - m00*m33*m12*m21 - m00*m13*m31*m22 + m23*m02*m30*m11 + m11*m03*m20*m32

            - m11*m33*m02*m20 - m03*m30*m11*m22 - m13*m30*m02*m21 - m13*m01*m20*m32

            - m03*m10*m21*m32 - m23*m12*m01*m30 + m33*m02*m10*m21 + m33*m12*m01*m20

            - m33*m22*m01*m10 + m03*m21*m12*m30 + m13*m31*m02*m20 + m13*m01*m30*m22

            + m23*m32*m01*m10 + m03*m10*m31*m22 - m03*m20*m12*m31 - m23*m02*m10*m31);

        }



        //! Computes inverse matrix

        Matrix4x4 inverse() const

        {

          M(00); M(10); M(20); M(30); 

          M(01); M(11); M(21); M(31); 

          M(02); M(12); M(22); M(32); 

          M(03); M(13); M(23); M(33); 

          float d=det();

          sl_assert(LibSL::Math::abs(d) > 1e-8);

          Matrix4x4 i;



          i[_00] =   ( m33*m11*m22 - m23*m11*m32 + m23*m12*m31 + m13*m21*m32 - m33*m12*m21  - m13*m31*m22);

          i[_10] = - ( m13*m20*m32 + m23*m12*m30 - m33*m12*m20 + m33*m22*m10 - m13*m30*m22 - m23*m32*m10);

          i[_20] =   ( m23*m30*m11 - m11*m33*m20 - m13*m30*m21 + m33*m10*m21 + m13*m31*m20 - m23*m10*m31);

          i[_30] = - (-m11*m20*m32 + m30*m11*m22 + m10*m21*m32 - m21*m12*m30 - m10*m31*m22 + m20*m12*m31);



          i[_01] = - ( m33*m22*m01 - m23*m32*m01 + m03*m21*m32 - m33*m02*m21 + m23*m02*m31 - m03*m31*m22);

          i[_11] =   ( m00*m33*m22 - m00*m23*m32 + m23*m02*m30 + m03*m20*m32 - m33*m02*m20 - m03*m30*m22);

          i[_21] = - ( m00*m33*m21 - m00*m23*m31 - m03*m21*m30 + m03*m20*m31 - m33*m01*m20 + m23*m01*m30);

          i[_31] =   ( m00*m21*m32 - m00*m31*m22 - m30*m02*m21 - m01*m20*m32 + m31*m02*m20 + m01*m30*m22);	  



          i[_02] =   ( m03*m11*m32 - m33*m02*m11 - m13*m01*m32 + m33*m12*m01 + m13*m02*m31 - m03*m31*m12);

          i[_12] = - (-m00*m13*m32 + m00*m33*m12 - m33*m02*m10 + m03*m10*m32 - m03*m12*m30 + m13*m30*m02);

          i[_22] =   ( m33*m00*m11 - m33*m01*m10 - m03*m30*m11 - m13*m00*m31 + m13*m01*m30 + m03*m10*m31);

          i[_32] = - ( m32*m00*m11 - m32*m01*m10 - m02*m30*m11 - m12*m00*m31 + m12*m01*m30 + m02*m10*m31);	  



          i[_03] = - (-m23*m02*m11 + m03*m11*m22 + m23*m12*m01 - m03*m21*m12 - m13*m01*m22 + m13*m02*m21);

          i[_13] =   ( m00*m23*m12 - m00*m13*m22 + m13*m02*m20 + m03*m10*m22 - m03*m20*m12 - m23*m02*m10);

          i[_23] = - ( m23*m00*m11 - m23*m01*m10 - m03*m20*m11 - m13*m00*m21 + m13*m01*m20 + m03*m10*m21);

          i[_33] =   ( m22*m00*m11 - m22*m01*m10 - m02*m20*m11 - m12*m00*m21 + m12*m01*m20 + m02*m10*m21);



          i = i / d;



          return (i);

        }



        Matrix4x4 transpose() const

        {

          Matrix4x4 r;

          ForIndex(i,4) {

            ForIndex(j,4) {

              r.at(i,j) = at(j,i);

            }

          }

          return (r);

        }



        const T_Type& at(uint c,uint r) const

        {

          return (*this)[c+r*4];

        }



        T_Type& at(uint c,uint r)

        {

          return (*this)[c+r*4];

        }

        Matrix4x4 operator *(const Matrix4x4& m) const

        {

          Matrix4x4 r;

          r.fill(T_Type(0));

          ForIndex (i,4) {

            ForIndex (j,4) {

              ForIndex (k,4) {

                r.at(j,i)+=at(k,i)*m.at(j,k);

              }

            }

          }

          return (r);

        }



        Tuple<T_Type,4> operator *(const Tuple<T_Type,4>& v) const

        {

          Tuple<T_Type,4> r;

          r.fill(T_Type(0));

          ForIndex (i,4) {

            ForIndex (k,4) {

              r[i]+=at(k,i)*v[k];

            }

          }

          return (r);

        }



        Tuple<T_Type,3> mulPoint(const Tuple<T_Type,3>& p) const

        {

          Tuple<T_Type,3> r;

          r.fill(T_Type(0));

          ForIndex (i,3) {

            ForIndex (k,3) {

              r[i]+=at(k,i)*p[k];

            }

            r[i]+=at(3,i);

          }

          return (r);

        }



        Tuple<T_Type,3> mulVector(const Tuple<T_Type,3>& v) const

        {

          Tuple<T_Type,3> r;

          r.fill(T_Type(0));

          ForIndex (i,3) {

            ForIndex (k,3) {

              r[i]+=at(k,i)*v[k];

            }

          }

          return (r);

        }



    };

      

    // typedefs



    typedef Matrix4x4<float>  m4x4f;

    typedef Matrix4x4<double> m4x4d;



    // translation matrix

    template <typename T_Type>

    Matrix4x4<T_Type> translationMatrix(const Tuple<T_Type,3>& t)

    {

      Matrix4x4<T_Type> m; m.eqIdentity();

      ForIndex(i,3) {

        m.at(3,i)=t[i];

      }

      return (m);

    }



    // scale matrix

    template <typename T_Type>

    Matrix4x4<T_Type> scaleMatrix(const Tuple<T_Type,3>& s)

    {

      Matrix4x4<T_Type> m; m.eqIdentity();

      ForIndex(i,3) {

        m.at(i,i)=s[i];

      }

      return (m);

    }



    // lookat matrix (RH)

    template <typename T_Type>

    Matrix4x4<T_Type> lookatMatrix(

      const Tuple<T_Type,3>& eye,

      const Tuple<T_Type,3>& at,

      const Tuple<T_Type,3>& up)

    {

      Tuple<T_Type,3> zaxis = normalize(eye - at); // revert for LH

      Tuple<T_Type,3> xaxis = normalize(cross(up,zaxis));

      Tuple<T_Type,3> yaxis = cross(zaxis, xaxis);

      return Matrix4x4<T_Type>(

        xaxis[0],           xaxis[1],           xaxis[2],          -dot(xaxis, eye),

        yaxis[0],           yaxis[1],           yaxis[2],          -dot(yaxis, eye),

        zaxis[0],           zaxis[1],           zaxis[2],          -dot(zaxis, eye),

        0,                  0,                  0,                  1);

    }



    // perspective matrix from frustum (RH)

    template <typename T_Type>

    Matrix4x4<T_Type> perspectiveMatrix(

      float l,float r,

      float b,float t,

      float zn,float zf)

    {

      return Matrix4x4<T_Type>(

        2*zn/(r-l),          0, (l+r)/(r-l),             0,

        0,          2*zn/(t-b), (t+b)/(t-b),             0,

        0,                   0,  zf/(zn-zf), zn*zf/(zn-zf),

        0,                   0,           -1,            0);

    }



    // perspective matrix from FOV (RH)

    template <typename T_Type>

    Matrix4x4<T_Type> perspectiveMatrix(

      float fov,float aspect,

      float zn,float zf)

    {

      //float yScale = cot(fov/2.0);
      float yScale = tan(M_PI/2.0 - fov/2.0);

      float xScale = yScale / aspect;

      return Matrix4x4<T_Type>(

        xScale,    0,          0,             0,

        0,    yScale,          0,             0,

        0,         0, zf/(zn-zf), zn*zf/(zn-zf),

        0,         0,         -1,             0);

    }
    
    // orthographic matrix

    template <typename T_Type>

    Matrix4x4<T_Type> orthographicMatrix(

      float l,float r,

      float t,float b,
      
      float n, float f	)

    {

      return Matrix4x4<T_Type>(

        2./(r-l),    0,          0,             -(r+l)/(r-l),

        0,    2./(t-b),          0,             -(t+b)/(t-b),

        0,         0, 		2./(f-n), 	-(f+n)/(f-n),

        0,         0,         0,             1);

    }



    // print matrix

    template <typename T_Type>

    std::ostream& operator<<(std::ostream& s,const Matrix4x4<T_Type>& m)

    {

      ForIndex(r,4) {

        s << '[';

        ForIndex(c,4) {

          s << m.at(c,r);

          if (c < 3) s << ',';

        }

        s << ']';

        if (r < 3) s << std::endl;

      }

      return (s);

    }



//  }; // namespace LibSL::Math

//}; // namespace LibSL

