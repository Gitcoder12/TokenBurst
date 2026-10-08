// src/main.cpp
// Burst — CLI entry point.

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>

#ifdef BURST_CUDA
extern "C" void vocab_lookup(
    const int* h_word_ids,
    const float* h_vocab_emb,
    float* h_output,
    int n_tokens,
    int vocab_size,
    int dim
);
#endif

int main(int argc, char** argv) {
    printf("Burst v0.2.0 — CUDA Tokenizer\n");

#ifdef BURST_CUDA
    printf("Build: CUDA enabled\n");

    // Small demo: 10 tokens, 5-word vocab, 4-dim embeddings
    const int n_tokens = 10;
    const int vocab_size = 5;
    const int dim = 4;

    int word_ids[n_tokens] = {0, 1, 2, 0, 3, 4, 1, 2, 3, 0};
    float vocab[vocab_size * dim] = {
        1.0f, 1.1f, 1.2f, 1.3f,   // word 0
        2.0f, 2.1f, 2.2f, 2.3f,   // word 1
        3.0f, 3.1f, 3.2f, 3.3f,   // word 2
        4.0f, 4.1f, 4.2f, 4.3f,   // word 3
        5.0f, 5.1f, 5.2f, 5.3f,   // word 4
    };
    float output[n_tokens * dim];

    vocab_lookup(word_ids, vocab, output, n_tokens, vocab_size, dim);

    printf("\nToken -> Embedding:\n");
    for (int i = 0; i < n_tokens; i++) {
        int wid = word_ids[i];
        printf("  token %d (word %d): [%.1f, %.1f, %.1f, %.1f]\n",
               i, wid,
               output[i*dim+0], output[i*dim+1],
               output[i*dim+2], output[i*dim+3]);
    }
#else
    printf("Build: CPU only (rebuild with -DBURST_ENABLE_CUDA=ON for GPU)\n");
#endif

    return 0;
}
