// =============================================================================
// cuda_add.cu — CUDA Array Addition, fully annotated
// =============================================================================
//
// WHAT IS CUDA?
//   CUDA lets you run code on the GPU in parallel.
//   The GPU has thousands of tiny cores — ideal for data-parallel tasks
//   like adding two arrays element-by-element.
//
// COMPILE:
//   nvcc -o cuda_add cuda_add.cu
// RUN:
//   ./cuda_add
//
// MEMORY MODEL (crucial mental model):
//
//   CPU (Host)          GPU (Device)
//   ──────────          ────────────
//   RAM (Host Memory)   VRAM (Device Memory)
//       A[], B[], C[]       d_A[], d_B[], d_C[]
//
//   cudaMalloc  → allocate on GPU VRAM
//   cudaMemcpy  → transfer bytes between CPU RAM ↔ GPU VRAM
//   cudaFree    → release GPU VRAM
//
// CPU and GPU cannot directly read each other's memory.
// You MUST copy data across the PCIe bus.
// =============================================================================

#include <iostream>
#include <cuda_runtime.h>

// =============================================================================
// CUDA ERROR-CHECKING HELPER
// Every CUDA call returns cudaError_t. In production code, always check it.
// =============================================================================
#define CUDA_CHECK(call) \
    do { \
        cudaError_t err = (call); \
        if (err != cudaSuccess) { \
            std::cerr << "CUDA error at " << __FILE__ << ":" << __LINE__ \
                      << " — " << cudaGetErrorString(err) << "\n"; \
            std::exit(EXIT_FAILURE); \
        } \
    } while (0)

// =============================================================================
// KERNEL — runs on the GPU
//
//   __global__    → called from CPU, runs on GPU
//   __device__    → called from GPU, runs on GPU
//   __host__      → called from CPU, runs on CPU (the default)
// =============================================================================

/// @brief Add two integer arrays element-wise on the GPU.
///
/// Each GPU thread handles ONE element, identified by its global index idx.
///
/// @param A  Input array 1  (device pointer)
/// @param B  Input array 2  (device pointer)
/// @param C  Output array   (device pointer)
/// @param N  Number of elements
__global__ void add(int* A, int* B, int* C, int N)
{
    // ── THREAD INDEX CALCULATION ─────────────────────────────────────────────
    // Threads are organised as:   Grid → Blocks → Threads
    //
    //  add<<<2, 4>>> means:
    //    gridDim.x  = 2   (number of blocks in X dimension)
    //    blockDim.x = 4   (threads per block in X dimension)
    //    Total threads launched = 2 × 4 = 8
    //
    //  Block 0: threadIdx.x = 0,1,2,3  →  idx = 0,1,2,3
    //  Block 1: threadIdx.x = 0,1,2,3  →  idx = 4,5,6,7
    //
    //  Formula: idx = blockIdx.x * blockDim.x + threadIdx.x

    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    // ── BOUNDS CHECK — THE KEY QUESTION ──────────────────────────────────────
    //
    //  WHY do we write:  if (idx < N)  ?
    //
    //  GPU thread counts must be multiples of the block size.
    //  In real programs you rarely have exactly N = blocks × threadsPerBlock.
    //
    //  Example: N = 9 elements, blockDim = 4
    //    → You'd need ceil(9/4) = 3 blocks → 3×4 = 12 threads launched
    //    → Threads with idx = 9, 10, 11 have NO corresponding array slot!
    //    → Without the guard, those threads write to C[9], C[10], C[11]
    //      which is beyond the allocated memory → UNDEFINED BEHAVIOUR / crash
    //
    //  This example uses N=8 = 2×4 exactly, so no "extra" threads exist here.
    //  But the guard is written anyway as defensive, portable practice.

    if (idx < N)
        C[idx] = A[idx] + B[idx];
}

// =============================================================================
// HOST (CPU) CODE
// =============================================================================
int main()
{
    // ── Setup ─────────────────────────────────────────────────────────────────
    const int N    = 8;
    const int size = N * sizeof(int);   // byte count for cudaMalloc/Memcpy

    // Host arrays (live in CPU RAM)
    int A[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    int B[8] = {10,20,30,40,50,60,70,80};
    int C[8] = {};   // results will be copied here from GPU

    // ── 1. Allocate GPU memory ────────────────────────────────────────────────
    // d_ prefix = "device" — convention to distinguish from host pointers
    int *d_A, *d_B, *d_C;
    CUDA_CHECK(cudaMalloc(&d_A, size));
    CUDA_CHECK(cudaMalloc(&d_B, size));
    CUDA_CHECK(cudaMalloc(&d_C, size));

    // ── 2. Copy inputs CPU → GPU ──────────────────────────────────────────────
    // cudaMemcpyHostToDevice: source is in CPU RAM, destination is in GPU VRAM
    CUDA_CHECK(cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice));
    // We do NOT copy d_C — it will be written by the kernel

    // ── 3. Launch the kernel ──────────────────────────────────────────────────
    //
    //   add  <<<  numBlocks, threadsPerBlock  >>>  (args...)
    //         ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    //         CUDA execution configuration — unique to CUDA .cu files
    //
    //   numBlocks        = 2   (how many thread blocks)
    //   threadsPerBlock  = 4   (threads per block)
    //   Total threads    = 8   (matches N exactly here)
    //
    //   Rule of thumb for threadsPerBlock: use 128, 256, or 512
    //   (multiples of 32 = one "warp", the GPU's execution unit)

    int threadsPerBlock = 4;
    int numBlocks = (N + threadsPerBlock - 1) / threadsPerBlock;  // ceil(N/T)
    add<<<numBlocks, threadsPerBlock>>>(d_A, d_B, d_C, N);

    // ── 4. Wait for GPU to finish, check for kernel errors ───────────────────
    CUDA_CHECK(cudaDeviceSynchronize());

    // ── 5. Copy result GPU → CPU ──────────────────────────────────────────────
    // cudaMemcpyDeviceToHost: source is GPU VRAM, destination is CPU RAM
    CUDA_CHECK(cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost));

    // ── 6. Use the result ─────────────────────────────────────────────────────
    std::cout << "A + B = ";
    for (int i = 0; i < N; i++)
        std::cout << C[i] << " ";
    // Expected: 11 22 33 44 55 66 77 88
    std::cout << "\n";

    // ── 7. Free GPU memory ────────────────────────────────────────────────────
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}

// =============================================================================
// SUMMARY TABLE — CUDA VOCABULARY
//
//  Term                 Meaning
//  ───────────────────  ──────────────────────────────────────────────
//  __global__           kernel: launched from CPU, runs on GPU
//  <<<blocks, threads>>>  execution configuration
//  threadIdx.x          thread's local index within its block (0..blockDim-1)
//  blockIdx.x           which block this thread belongs to    (0..gridDim-1)
//  blockDim.x           threads per block
//  gridDim.x            blocks per grid
//  idx = blockIdx.x * blockDim.x + threadIdx.x   global unique thread index
//  cudaMalloc           allocate GPU VRAM
//  cudaMemcpy           transfer data between CPU RAM and GPU VRAM
//  cudaFree             release GPU VRAM
//  cudaDeviceSynchronize  wait for all GPU work to complete
//
// MENTAL MODEL:
//   Think of the GPU as a city of thread-workers.
//   Each worker (thread) has a street address (idx).
//   The kernel is the job description.
//   Every worker runs the same code but on different data (idx is their ID).
//   if (idx < N) makes sure workers beyond the array boundary do nothing.
// =============================================================================
