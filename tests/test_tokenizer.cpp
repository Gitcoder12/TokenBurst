// tests/test_tokenizer.cpp
// TokenBurst — test suite.

#include <cstdio>
#include <string>
#include <vector>

#include "../src/tokenizer.h"

using namespace tokenburst;

static int passed = 0;
static int failed = 0;

#define CHECK(cond, msg) do { \
    if (cond) { passed++; printf("PASS: %s\n", msg); } \
    else { failed++; printf("FAIL: %s\n", msg); } \
} while (0)

int main() {
    Tokenizer tok;
    if (!tok.load("data/vocab.json")) {
        printf("error: cannot load data/vocab.json — run from repo root\n");
        return 1;
    }

    // Signature sentence
    {
        std::string s = "Tokenburst burns fast, its working.";
        auto ids = tok.encode(s);
        printf("  '%s' -> %zu token(s)\n", s.c_str(), ids.size());
        CHECK(ids.size() == 1, "signature sentence -> 1 token");
    }

    // Round-trip
    {
        std::string s = "Tokenburst burns fast, its working.";
        auto ids = tok.encode(s);
        std::string decoded = tok.decode(ids);
        CHECK(decoded == s, "round-trip preserves sentence");
    }

    // Partial word
    {
        auto ids = tok.encode("Tokenburst");
        CHECK(!ids.empty(), "partial word -> non-empty tokens");
    }

    // Unknown chars
    {
        auto ids = tok.encode("zzz");
        CHECK(ids.empty(), "unknown chars -> empty tokens");
    }

    // Empty
    {
        auto ids = tok.encode("");
        CHECK(ids.empty(), "empty input -> empty tokens");
    }

    printf("\n%d passed, %d failed\n", passed, failed);
    return failed == 0 ? 0 : 1;
}
