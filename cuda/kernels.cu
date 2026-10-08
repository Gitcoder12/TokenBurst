// cuda/kernels.cu
//
// Burst — CUDA kernels for tokenizer acceleration.
// Vocab lookup: parallel embedding retrieval.

#include <cuda_runtime.h>
#include <cstdio>
#include <cstdlib>

// ---------------------------------------------------------------------------
// Kernel 1: Vocab Lookup
// ---------------------------------------------------------------------------
// Each thread handles one token's embedding lookup.
// Coalesced memory access: consecutive threads read consecutive dims.

__global__ void vocab_lookup_kernel(
    const int* __restrict__ word_ids,
    const float* __restrict__ vocab_emb,
    float* __restrict__ output,
    int n_tokens,
    int dim
) {
    // Global thread index
    int token_idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (token_idx >= n_tokens) return;

    // Each token has `dim` floats. Consecutive threads write consecutive tokens.
    int word_id = word_ids[token_idx];
    const float* src = vocab_emb + word_id * dim;
    float* dst = output + token_idx * dim;

    // Vectorized copy — 4 floats per instruction when possible
    int vec_dim = dim / 4;
    const float4* src4 = reinterpret_cast<const float4*>(src);
    float4* dst4 = reinterpret_cast<float4*>(dst);

    for (int i = 0; i < vec_dim; i++) {
        dst4[i] = src4[i];
    }

    // Handle remainder
    for (int i = vec_dim * 4; i < dim; i++) {
        dst[i] = src[i];
    }
}

// ---------------------------------------------------------------------------
// Host wrapper
// ---------------------------------------------------------------------------

extern "C" void vocab_lookup(
    const int* h_word_ids,
    const float* h_vocab_emb,
    float* h_output,
    int n_tokens,
    int vocab_size,
    int dim
) {
    // Device pointers
    int *d_word_ids = nullptr;
    float *d_vocab_emb = nullptr;
    float *d_output = nullptr;

    size_t bytes_ids = n_tokens * sizeof(int);
    size_t bytes_vocab = (size_t)vocab_size * dim * sizeof(float);
    size_t bytes_output = (size_t)n_tokens * dim * sizeof(float);

    // Allocate device memory
    cudaMalloc(&d_word_ids, bytes_ids);
    cudaMalloc(&d_vocab_emb, bytes_vocab);
    cudaMalloc(&d_output, bytes_output);

    // Copy input to device
    cudaMemcpy(d_word_ids, h_word_ids, bytes_ids, cudaMemcpyHostToDevice);
    cudaMemcpy(d_vocab_emb, h_vocab_emb, bytes_vocab, cudaMemcpyHostToDevice);

    // Launch kernel
    int threads_per_block = 256;
    int blocks = (n_tokens + threads_per_block - 1) / threads_per_block;

    vocab_lookup_kernel<<<blocks, threads_per_block>>>(
        d_word_ids, d_vocab_emb, d_output, n_tokens, dim
    );

    // Wait for GPU
    cudaDeviceSynchronize();

    // Copy result back
    cudaMemcpy(h_output, d_output, bytes_output, cudaMemcpyDeviceToHost);

    // Free device memory
    cudaFree(d_word_ids);
    cudaFree(d_vocab_emb);
    cudaFree(d_output);
}

// ---------------------------------------------------------------------------
// Simple test (standalone — run with: nvcc kernels.cu -o test && ./test)
// ---------------------------------------------------------------------------

#ifdef CUDA_TEST_MAIN

int main() {
    const int n_tokens = 1000;
    const int vocab_size = 100;
    const int dim = 64;

    // Host allocations
    int* h_word_ids = (int*)malloc(n_tokens * sizeof(int));
    float* h_vocab = (float*)malloc(vocab_size * dim * sizeof(float));
    float* h_out = (float*)malloc(n_tokens * dim * sizeof(float));

    // Fill with dummy data
    for (int i = 0; i < n_tokens; i++) h_word_ids[i] = i % vocab_size;
    for (int i = 0; i < vocab_size * dim; i++) h_vocab[i] = (float)i;

    // Run kernel
    vocab_lookup(h_word_ids, h_vocab, h_out, n_tokens, vocab_size, dim);

    // Verify
    bool ok = true;
    for (int i = 0; i < n_tokens; i++) {
        int wid = h_word_ids[i];
        for (int d = 0; d < dim; d++) {
            float expected = h_vocab[wid * dim + d];
            if (h_out[i * dim + d] != expected) {
                printf("MISMATCH at token %d dim %d\n", i, d);
                ok = false;
                break;
            }
        }
        if (!ok) break;
    }

    if (ok) printf("PASS: vocab_lookup kernel works correctly\n");

    free(h_word_ids);
    free(h_vocab);
    free(h_out);
    return ok ? 0 : 1;
}

#endif
