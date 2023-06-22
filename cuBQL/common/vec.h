// ======================================================================== //
// Copyright 2023-2023 Ingo Wald                                            //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#pragma once

#include "cuBQL/common/common.h"

namespace cuBQL {

  template<typename T, int N>
  struct vec_t_data {
    inline __both__ T  operator[](int i) const { return v[i]; }
    inline __both__ T &operator[](int i)       { return v[i]; }
    T v[N];
  };

  struct invalid_t {};
  /*! definesthe "cuda equivalent type" for a given vector type; i.e.,
      a vec3f=vec_t<float,3> has a equivalent cuda built-in type of
      float3. to also allow vec_t's that do not have a cuda
      equivalent, let's also create a 'invalid_t' to be used by
      default */ 
  template<typename T, int N> struct cuda_eq_t { using type = invalid_t; };
  template<> struct cuda_eq_t<float,2> { using type = float2; };
  template<> struct cuda_eq_t<float,3> { using type = float3; };

  template<typename T>
  struct vec_t_data<T,2> {
    using cuda_t = typename cuda_eq_t<T,2>::type;
    inline __both__ T  operator[](int i) const { return (&x)[i]; }
    inline __both__ T &operator[](int i)       { return (&x)[i]; }
    /*! auto-cast to equivalent cuda type */
    inline __both__ operator cuda_t() { cuda_t t; t.x = x; t.y = y; return t; }
    T x, y;
  };
  template<typename T>
  struct vec_t_data<T,3> {
    using cuda_t = typename cuda_eq_t<T,3>::type;
    inline __both__ T  operator[](int i) const { return (&x)[i]; }
    inline __both__ T &operator[](int i)       { return (&x)[i]; }
    /*! auto-cast to equivalent cuda type */
    inline __both__ operator cuda_t() { cuda_t t; t.x = x; t.y = y; return t; }
    T x, y, z;
  };
  
  template<typename T, int N>
  struct vec_t : public vec_t_data<T,N> {
    enum { numDims = N };
    using scalar_t = T;

    using cuda_t = typename cuda_eq_t<T,N>::type;
    inline __both__ vec_t &operator=(cuda_t v)
    {
#pragma unroll
      for (int i=0;i<numDims;i++) (*this)[i] = (&v.x)[i]; 
      return *this;
    }
  };

  /*! traits for a vec_t */
  template<typename vec_t>
  struct our_vec_t_traits {
    enum { numDims = vec_t::numDims };
    using scalar_t = typename vec_t::scalar_t;
  };


  /*! vec_traits<T> describe the scalar type and number of dimensions
    of whatever wants to get used as a vector/point type in
    cuBQL. By default cuBQL will use its own vec_t<T,N> for that,
    but this should allow to also describe the traits of external
    types such as CUDA's float3 */
  template<typename T> struct vec_traits : public our_vec_t_traits<T> {};
  
  template<> struct vec_traits<float3> { enum { numDims = 3 }; using scalar_t = float; };



  template<typename vec_t>
  inline __both__ vec_t make(typename vec_t::scalar_t v)
  {
    vec_t r;
#pragma unroll
    for (int i=0;i<vec_t::numDims;i++)
      r[i] = v;
    return r;
  }

  template<typename vec_t>
  inline __both__ vec_t make(typename cuda_eq_t<typename vec_t::scalar_t,vec_t::numDims>::type v)
  {
    vec_t r;
#pragma unroll
    for (int i=0;i<vec_t::numDims;i++)
      r[i] = (&v.x)[i];
    return r;
  }
  
#define CUBQL_OPERATOR(long_op, op)                     \
  /* vec:vec */                                         \
  template<typename T, int D>                           \
  inline __both__                                       \
  vec_t<T,D> long_op(vec_t<T,D> a, vec_t<T,D> b)        \
  {                                                     \
    vec_t<T,D> r;                                       \
    _Pragma("unroll")                                   \
      for (int i=0;i<D;i++) r[i] = a[i] op b[i];        \
    return r;                                           \
  }                                                     \
  /* scalar-vec */                                      \
  template<typename T, int D>                           \
  inline __both__                                       \
  vec_t<T,D> long_op(T a, vec_t<T,D> b)                 \
  {                                                     \
    vec_t<T,D> r;                                       \
    _Pragma("unroll")                                   \
      for (int i=0;i<D;i++) r[i] = a op b[i];           \
    return r;                                           \
  }                                                     \
  /* vec:scalar */                                      \
  template<typename T, int D>                           \
  inline __both__                                       \
  vec_t<T,D> long_op(vec_t<T,D> a, T b)                 \
  {                                                     \
    vec_t<T,D> r;                                       \
    _Pragma("unroll")                                   \
      for (int i=0;i<D;i++) r[i] = a[i] op b;           \
    return r;                                           \
  }                                                     \
  /* cudaVec:vec */                                      \
  template<typename T, int D>                           \
  inline __both__                                       \
  vec_t<T,D> long_op(typename cuda_eq_t<T,D>::type a, vec_t<T,D> b)  \
  {                                                     \
    vec_t<T,D> r;                                       \
    _Pragma("unroll")                                   \
      for (int i=0;i<D;i++) r[i] = (&a.x)[i] op b[i];      \
    return r;                                           \
  }                                                     \
  /* vec:cudaVec */                                      \
  template<typename T, int D>                           \
  inline __both__                                       \
  vec_t<T,D> long_op(vec_t<T,D> a,  typename cuda_eq_t<T,D>::type b)                 \
  {                                                     \
    vec_t<T,D> r;                                       \
    _Pragma("unroll")                                   \
      for (int i=0;i<D;i++) r[i] = a[i] op (&b.x)[i];      \
    return r;                                           \
  }                                                     \

  CUBQL_OPERATOR(operator+,+)
  CUBQL_OPERATOR(operator-,-)
  CUBQL_OPERATOR(operator*,*)
  CUBQL_OPERATOR(operator/,/)
#undef CUBQL_OPERATOR
  
#define CUBQL_BINARY(op)                                \
  template<typename T, int N>                           \
  inline __both__                                       \
  vec_t<T,N> op(vec_t<T,N> a, vec_t<T,N> b)             \
  {                                                     \
    vec_t<T,N> r;                                       \
    _Pragma("unroll")                                   \
      for (int i=0;i<N;i++) r[i] = op(a[i],b[i]);       \
    return r;                                           \
  }

  using ::min;
  CUBQL_BINARY(min)
  using ::max;
  CUBQL_BINARY(max)
#undef CUBQL_FUNCTOR

  template<typename T> struct dot_result_t;
  template<> struct dot_result_t<float> { using type = float; };
  template<> struct dot_result_t<int32_t> { using type = int64_t; };

  template<typename T, int D> inline __both__
  typename dot_result_t<T>::type dot(vec_t<T,D> a, vec_t<T,D> b)
  {
    typename dot_result_t<T>::type result = 0;
#pragma unroll
    for (int i=0;i<D;i++)
      result += a[i]*b[i];
    return result;
  }


    
  /*! accurate square-distance between two points; due to the 'square'
      involved in computing the distance this may need to change the
      type from int to long, etc - so a bit less rounding issues, but
      a bit more complicated to use with the right typenames */
  template<typename T, int D> inline __both__
  typename dot_result_t<T>::type sqrDistance(vec_t<T,D> a, vec_t<T,D> b)
  {
    vec_t<T,D> diff = a - b;
    return dot(diff,diff);
  }

  /*! approximate-conservative square distance between two
      points. whatever type the points are, the result will be
      returned in floats, including whatever rounding error that might
      incur. we will, however, always round downwars, so if this is
      used for culling it will, if anything, under-estiamte the
      distance to a subtree (and thus, still traverse it) rather than
      wrongly skipping it*/
  template<typename T> inline __device__ float fSqrLength(T v);
  template<> inline __device__ float fSqrLength<float>(float v)
  { return v*v; }
  template<> inline __device__ float fSqrLength<int>(int _v)
  { float v = __int2float_rz(_v); return v*v; }

  /*! approximate-conservative square distance between two
      points. whatever type the points are, the result will be
      returned in floats, including whatever rounding error that might
      incur. we will, however, always round downwars, so if this is
      used for culling it will, if anything, under-estiamte the
      distance to a subtree (and thus, still traverse it) rather than
      wrongly skipping it*/
  template<typename T, int D> inline __both__
  float fSqrDistance(vec_t<T,D> a, vec_t<T,D> b)
  {
    float sum = 0.f;
    for (int i=0;i<D;i++)
      sum += fSqrLength(a[i]-b[i]);
    return sum;
  }
  
  using vec3f = vec_t<float,3>;
}

