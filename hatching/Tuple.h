// ------------------------------------------------------
// LibSL::Math::Tuple
// ------------------------------------------------------
//
// Tuple class
// 
// => modified version for ECP OpenGL TPs
//
// ------------------------------------------------------
// Sylvain Lefebvre - 2006-03-07
// ------------------------------------------------------

#pragma once

typedef unsigned int uint;
#include <sstream>

#include <math.h>

#define ForIndex(i,n) for (int i=0;i<n;i++)
#define sl_assert(x)

//namespace LibSL {
//  namespace Math {

    template<typename T_Type, int T_N>
    class Tuple 
    {
    protected:

      T_Type m_Values[T_N];

    public:

      typedef T_Type t_Element;
      enum           {e_Size = T_N};

      Tuple()  { 

      }

      ~Tuple() { }

      Tuple(const Tuple<T_Type,T_N>& tup) {
        ForIndex(i,T_N) {
          m_Values[i]=tup[i]; 
        }
      }

      template <typename T_Type2,uint T_N2>
      explicit Tuple(const Tuple<T_Type2,T_N2>& tup) {
        fill(T_Type(0));
        ForIndex(i,( T_N < T_N2 ? T_N : T_N2) ) {
          m_Values[i]=T_Type(tup[i]);
        }
      }

      Tuple(const Tuple<T_Type,T_N-1>& tup,const T_Type& v) {
        ForIndex(i,T_N-1) {
          m_Values[i]=tup[i]; 
        }
        m_Values[T_N-1]=v;
      }

      Tuple(const T_Type& v,const Tuple<T_Type,T_N-1>& tup) {
        m_Values[0]=v;
        ForIndex(i,T_N-1) {
          m_Values[i+1]=tup[i]; 
        }
      }

      Tuple(const T_Type& v)
      {
        ForIndex(i,T_N) { 
          m_Values[i]=v;
        }
      }

      void fill(const T_Type& v) 
      {
        ForIndex(i,T_N) { 
          m_Values[i]=v;
        }
      }

      Tuple<T_Type,T_N>& operator=(const Tuple<T_Type,T_N>& tup) 
      {
        ForIndex(i,T_N) { 
          m_Values[i]=tup[i]; 
        }
        return *this;
      }

      Tuple<T_Type,T_N>& operator=(const T_Type& v) 
      {
        ForIndex(i,T_N) { 
          m_Values[i]=v;
        }
        return *this;
      }

      bool operator == (const Tuple<T_Type,T_N>& tup) const {
        ForIndex(i,T_N) { 
          if (m_Values[i]!=tup[i]) 
            return false; 
        }
        return true;
      }

      bool operator != (const Tuple<T_Type,T_N>& tup) const {
        ForIndex(i,T_N) { 
          if (m_Values[i]!=tup[i]) 
            return true; 
        }
        return false;
      }

      // bool operator < (const Tuple<T_Type,T_N>& tup) const

      const T_Type& operator[](int i) const {
//        sl_dbg_assert((i>=0) && (i<T_N));
        return m_Values[i];
      }

      T_Type& operator[](int i) {
//        sl_dbg_assert((i>=0) && (i<T_N));
        return m_Values[i];
      }

    };

    // Tuple operators (add,sub,dot,length,dist)

    // add

    template<class T_Type, int T_N> Tuple<T_Type,T_N> 
    operator + (const Tuple<T_Type,T_N>& t0,const Tuple<T_Type,T_N>& t1)
    {
      Tuple<T_Type,T_N> r;
      ForIndex(i,T_N) {
        r[i]=t0[i]+t1[i];
      }
      return (r);
    }

    template<class T_Type, int T_N> Tuple<T_Type,T_N> 
    add(const Tuple<T_Type,T_N>& t0,const Tuple<T_Type,T_N>& t1)
    {
      Tuple<T_Type,T_N> r;
      ForIndex(i,T_N) {
        r[i]=t0[i]+t1[i];
      }
      return (r);
    }

    template<class T_Type, int T_N> void addEq(Tuple<T_Type,T_N>& t0,const Tuple<T_Type,T_N>& t1)
    {
      ForIndex(i,T_N) {
        t0[i]+=t1[i];
      }
    }

    // sub

    template<class T_Type, int T_N> Tuple<T_Type,T_N> 
    operator - (const Tuple<T_Type,T_N>& t0,const Tuple<T_Type,T_N>& t1)
    {
      Tuple<T_Type,T_N> r;
      ForIndex(i,T_N) {
        r[i]=t0[i]-t1[i];
      }
      return (r);
    }

    template<class T_Type, int T_N> Tuple<T_Type,T_N> 
    sub(const Tuple<T_Type,T_N>& t0,const Tuple<T_Type,T_N>& t1)
    {
      Tuple<T_Type,T_N> r;
      ForIndex(i,T_N) {
        r[i]=t0[i]-t1[i];
      }
      return (r);
    }

    template<class T_Type, int T_N> void 
      subEq(Tuple<T_Type,T_N>& t0,const Tuple<T_Type,T_N>& t1)
    {
      ForIndex(i,T_N) {
        t0[i]-=t1[i];
      }
    }

    // negate

    template<class T_Type, int T_N> Tuple<T_Type,T_N> 
    operator - (const Tuple<T_Type,T_N>& t)
    {
      Tuple<T_Type,T_N> r;
      ForIndex(i,T_N) {
        r[i]=-t[i];
      }
      return (r);
    }

    template<class T_Type, int T_N> Tuple<T_Type,T_N> 
    negate(const Tuple<T_Type,T_N>& t)
    {
      Tuple<T_Type,T_N> r;
      ForIndex(i,T_N) {
        r[i]=-t[i];
      }
      return (r);
    }

    template<class T_Type, int T_N> void
    negateEq(Tuple<T_Type,T_N>& t)
    {
      ForIndex(i,T_N) {
        t[i]=-t[i];
      }
    }

    // dot

    template<class T_Type, int T_N> T_Type 
      dot(const Tuple<T_Type,T_N>& t0,const Tuple<T_Type,T_N>& t1)
    {
      T_Type r(0);
      ForIndex(i,T_N) {
        r += t0[i]*t1[i];
      }
      return (r);
    }

    // mul tuple,tuple

    template<class T_Type, int T_N> Tuple<T_Type,T_N> 
    operator * (const Tuple<T_Type,T_N>& t0,const Tuple<T_Type,T_N>& t1)
    {
      Tuple<T_Type,T_N> r;
      ForIndex(i,T_N) {
        r[i]=t0[i]*t1[i];
      }
      return (r);
    }

    template<class T_Type, int T_N> Tuple<T_Type,T_N> 
    mul(const Tuple<T_Type,T_N>& t0,const Tuple<T_Type,T_N>& t1)
    {
      Tuple<T_Type,T_N> r;
      ForIndex(i,T_N) {
        r[i]=t0[i]*t1[i];
      }
      return (r);
    }

    template<class T_Type, int T_N> void 
      mulEq(Tuple<T_Type,T_N>& t0,const Tuple<T_Type,T_N>& t1)
    {
      ForIndex(i,T_N) {
        t0[i]*=t1[i];
      }
    }

    // mul tuple,scalar

    template<class T_Type, int T_N> Tuple<T_Type,T_N>
    operator * (const Tuple<T_Type,T_N>& t,const T_Type& s)
    {
      Tuple<T_Type,T_N> r;
      ForIndex(i,T_N) {
        r[i]=t[i]*s;
      }
      return (r);
    }

    template<class T_Type, int T_N> Tuple<T_Type,T_N> 
    mul(const Tuple<T_Type,T_N>& t,const T_Type& s)
    {
      Tuple<T_Type,T_N> r;
      ForIndex(i,T_N) {
        r[i]=t[i]*s;
      }
      return (r);
    }

    template<class T_Type, int T_N> void 
      mulEq(Tuple<T_Type,T_N>& t,const T_Type& s)
    {
      ForIndex(i,T_N) {
        t[i]*=s;
      }
    }

    // mul scalar,tuple

    template<class T_Type, int T_N> Tuple<T_Type,T_N> 
    operator * (const T_Type& s,const Tuple<T_Type,T_N>& t)
    {
      return (t*s);
    }

    template<class T_Type, int T_N> Tuple<T_Type,T_N> 
    mul(const T_Type& s,const Tuple<T_Type,T_N>& t)
    {
      return (mul(t,s));
    }

    template<class T_Type, int T_N> void 
      mulEq(const T_Type& s,Tuple<T_Type,T_N>& t)
    {
      mulEq(t,s);
    }

    // div tuple,tuple

    template<class T_Type, int T_N> Tuple<T_Type,T_N>
    operator / (const Tuple<T_Type,T_N>& t0,const Tuple<T_Type,T_N>& t1)
    {
      Tuple<T_Type,T_N> r;
      ForIndex(i,T_N) {
        r[i]=t0[i]/t1[i];
      }
      return (r);
    }

    template<class T_Type, int T_N> Tuple<T_Type,T_N> 
    div(const Tuple<T_Type,T_N>& t0,const Tuple<T_Type,T_N>& t1)
    {
      Tuple<T_Type,T_N> r;
      ForIndex(i,T_N) {
        r[i]=t0[i]/t1[i];
      }
      return (r);
    }

    template<class T_Type, int T_N> void 
      divEq(Tuple<T_Type,T_N>& t0,const Tuple<T_Type,T_N>& t1)
    {
      ForIndex(i,T_N) {
        t0[i]/=t1[i];
      }
    }

    // div tuple,scalar

    template<class T_Type, int T_N> Tuple<T_Type,T_N>
    operator / (const Tuple<T_Type,T_N>& t,const T_Type& s)
    {
      Tuple<T_Type,T_N> r;
      ForIndex(i,T_N) {
        r[i]=t[i]/s;
      }
      return (r);
    }

    template<class T_Type, int T_N> Tuple<T_Type,T_N> 
    div(const Tuple<T_Type,T_N>& t,const T_Type& s)
    {
      Tuple<T_Type,T_N> r;
      ForIndex(i,T_N) {
        r[i]=t[i]/s;
      }
      return (r);
    }

    template<class T_Type, int T_N> void 
      divEq(Tuple<T_Type,T_N>& t,const T_Type& s)
    {
      sl_assert(abs(s) > 0);
      ForIndex(i,T_N) {
        t[i]/=s;
      }
    }

    // div scalar,tuple

    template<class T_Type, int T_N> Tuple<T_Type,T_N> 
    operator / (const T_Type& s,const Tuple<T_Type,T_N>& t)
    {
      Tuple<T_Type,T_N> r;
      ForIndex(i,T_N) {
        r[i]=s/t[i];
      }
      return (r);
    }

    template<class T_Type, int T_N> Tuple<T_Type,T_N> 
    div(const T_Type& s,const Tuple<T_Type,T_N>& t)
    {
      Tuple<T_Type,T_N> r;
      ForIndex(i,T_N) {
        r[i]=s/t[i];
      }
      return (r);
    }

    // length (norm L2)

    template<class T_Type, int T_N> 
    float sqLength(const Tuple<T_Type,T_N>& t)
    {
      float sqlength=0;
      ForIndex(i,T_N) {
        sqlength += float(t[i]*t[i]);
      }
      return (sqlength);
    }

    template<class T_Type, int T_N> 
    float length(const Tuple<T_Type,T_N>& t)
    {
      return (sqrt(sqLength(t)));
    }    

    // normalize

    template<class T_Type, int T_N> Tuple<T_Type,T_N> 
    normalize(const Tuple<T_Type,T_N>& t)
    {
      T_Type len=length(t);
      sl_assert(len > 0);
      Tuple<T_Type,T_N> r;
      r = t/len;
      return (r);
    }

    template<class T_Type, int T_N> void
      normalizeEq(Tuple<T_Type,T_N>& t)
    {
      divEq(t,length(t));
    }


    // distance (norm L2)

    template<class T_Type, int T_N> T_Type 
      sqDist(const Tuple<T_Type,T_N>& t0,const Tuple<T_Type,T_N>& t1)
    {
      T_Type sqdist=0;
      ForIndex(i,T_N) {
        T_Type d = (t0[i]-t1[i]);
        sqdist += (d*d);
      }
      return (sqdist);
    }

    template<class T_Type, int T_N> T_Type 
      dist(const Tuple<T_Type,T_N>& t0,const Tuple<T_Type,T_N>& t1)
    {
      return (sqrt(sqDist(t0,t1)));
    }

    // stream

    template<class T_Type, int T_N>
    std::ostream& operator<<(std::ostream& s,const Tuple<T_Type,T_N>& t)
    {
      s << '(';
      ForIndex(i,T_N) { s << t[i]; if (i < T_N-1) s << ','; }
      s << ')';
      return (s);
    }

    /*
    template<class T_Type, int T_N>
    std::istream& operator>>(std::istream& s,Tuple<T_Type,T_N>& t)
    {

    }
    */

    // iterator (on integers only)

    template<int T_N>
    Tuple<uint,T_N>& inc(Tuple<uint,T_N>& i,const Tuple<uint,T_N>& s)
    {
      ForIndex(d,T_N) {
        if (i[d] == s[d]-1) {
          // max value reached
          if (d == T_N-1) {
            i[d]++;
          } else {
            // zero, next will be incremented
            i[d]=0;
          }
        } else {
          i[d]++;
          // no need to go further
          return i;
        }
      }
      return i;
    }

    template<int T_N>
    bool done(Tuple<uint,T_N>& i,const Tuple<uint,T_N>& s)
    {
      return (i[T_N-1]>=s[T_N-1]);
    }

//  }; // namespace LibSL::Math
//}; // namespace LibSL
