cmake_minimum_required(VERSION 3.18)
project(Burst LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_ARCHITECTURES 75 80 86 89)

# CPU-only build (works everywhere)
add_executable(burst_cpu
    src/main.cpp
)

# CUDA build (requires NVIDIA GPU + CUDA toolkit)
option(BURST_ENABLE_CUDA "Build with CUDA support" ON)

if(BURST_ENABLE_CUDA)
    add_executable(burst_gpu
        src/main.cpp
        cuda/kernels.cu
    )
    target_compile_definitions(burst_gpu PRIVATE BURST_CUDA=1)
endif()
