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

/*! instantiates the GPU builder(s) */
#define CUBQL_GPU_BUILDER_IMPLEMENTATION 1
#include "cuBQL/bvh.h"

CUBQL_INSTANTIATE_BINARY_BVH(float,2)
CUBQL_INSTANTIATE_BINARY_BVH(float,4)

CUBQL_INSTANTIATE_BINARY_BVH(float,3)
CUBQL_INSTANTIATE_WIDE_BVH(float,3,4)
CUBQL_INSTANTIATE_WIDE_BVH(float,3,8)

CUBQL_INSTANTIATE_BINARY_BVH(float,CUBQL_TEST_N)

namespace cuBQL {

  // template void gpuBuilder(BinaryBVH<float,CUBQL_TEST_N>   &bvh,
  //                          const box_t<float,CUBQL_TEST_N> *boxes,
  //                          uint32_t     numBoxes,
  //                          BuildConfig  buildConfig,
  //                          cudaStream_t s);
  
  // template void gpuBuilder(WideBVH<float,CUBQL_TEST_N,4>   &bvh,
  //                          const box_t<float,CUBQL_TEST_N> *boxes,
  //                          uint32_t     numBoxes,
  //                          BuildConfig  buildConfig,
  //                          cudaStream_t s);
  // template void gpuBuilder(WideBVH<float,CUBQL_TEST_N,8>   &bvh,
  //                          const box_t<float,CUBQL_TEST_N> *boxes,
  //                          uint32_t     numBoxes,
  //                          BuildConfig  buildConfig,
  //                          cudaStream_t s);
  // template void free(WideBVH<float,CUBQL_TEST_N,4>  &bvh, cudaStream_t s);
  // template void free(WideBVH<float,CUBQL_TEST_N,8>  &bvh, cudaStream_t s);
}
