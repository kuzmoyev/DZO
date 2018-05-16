

#ifndef MI_PRC_CLION_CUDA_STUB_H
#define MI_PRC_CLION_CUDA_STUB_H



#ifdef __JETBRAINS_IDE__
#define __CUDACC__ 1
#define __host__
#define __device__
#define __global__
#define __forceinline__
#define __shared__
inline void __syncthreads() {}
inline void __threadfence_block() {}
template<class T> inline T __clz(const T val) { return val; }
struct __cuda_fake_struct { unsigned int x, y, z; };
extern __cuda_fake_struct threadIdx;
extern __cuda_fake_struct blockIdx;
extern __cuda_fake_struct blockDim;

template <typename Num>
unsigned int atomicAdd(Num* address, Num val);

#endif

#endif //MI_PRC_CLION_CUDA_STUB_H
