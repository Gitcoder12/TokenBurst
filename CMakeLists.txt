cmake_minimum_required(VERSION 3.18)
project(TokenBurst LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# CPU-only build (works everywhere)
add_executable(burst_cpu
    src/main.cpp
    src/vocab.cpp
    src/tokenizer.cpp
)

target_include_directories(burst_cpu PRIVATE src)

# CUDA build (requires NVIDIA GPU + CUDA toolkit)
option(TOKENBURST_ENABLE_CUDA "Build with CUDA support" OFF)

if(TOKENBURST_ENABLE_CUDA)
    enable_language(CUDA)
    set(CMAKE_CUDA_STANDARD 17)
    set(CMAKE_CUDA_ARCHITECTURES 75 80 86 89)

    add_executable(burst_gpu
        src/main.cpp
        src/vocab.cpp
        src/tokenizer.cpp
        cuda/kernels.cu
    )

    target_include_directories(burst_gpu PRIVATE src cuda)
    target_compile_definitions(burst_gpu PRIVATE TOKENBURST_CUDA=1)
endif()
