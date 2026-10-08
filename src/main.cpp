// src/main.cpp
// TokenBurst — CLI entry point.

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "tokenizer.h"
#include "vocab.h"

using namespace tokenburst;

static void print_usage() {
    printf("TokenBurst v0.2\n");
    printf("Usage:\n");
    printf("  tokenburst --vocab <path> \"text to encode\"\n");
    printf("  tokenburst --vocab <path> --file <path>\n");
    printf("\n");
}

int main(int argc, char** argv) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    std::string vocab_path;
    std::string text;
    std::string file_path;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--vocab" && i + 1 < argc) {
            vocab_path = argv[++i];
        } else if (arg == "--file" && i + 1 < argc) {
            file_path = argv[++i];
        } else {
            text = arg;
        }
    }

    if (vocab_path.empty()) {
        fprintf(stderr, "error: --vocab <path> required\n");
        return 1;
    }

    Tokenizer tok;
    if (!tok.load(vocab_path)) {
        fprintf(stderr, "error: failed to load vocab\n");
        return 1;
    }

    // Read text from file if provided
    if (!file_path.empty()) {
        FILE* f = fopen(file_path.c_str(), "rb");
        if (!f) {
            fprintf(stderr, "error: cannot open %s\n", file_path.c_str());
            return 1;
        }
        fseek(f, 0, SEEK_END);
        long sz = ftell(f);
        fseek(f, 0, SEEK_SET);
        text.resize(sz);
        fread(&text[0], 1, sz, f);
        fclose(f);
    }

    if (text.empty()) {
        fprintf(stderr, "error: no input text\n");
        return 1;
    }

    // Encode
    std::vector<int64_t> tokens = tok.encode(text);

    // Print tokens
    printf("Input length: %zu bytes\n", text.size());
    printf("Token count:  %zu\n", tokens.size());
    printf("Tokens: ");
    for (size_t i = 0; i < tokens.size(); i++) {
        printf("%lld", (long long)tokens[i]);
        if (i + 1 < tokens.size()) printf(", ");
    }
    printf("\n");

    // Round-trip test
    std::string decoded = tok.decode(tokens);
    printf("\nDecoded: %s\n", decoded.c_str());

    return 0;
}
