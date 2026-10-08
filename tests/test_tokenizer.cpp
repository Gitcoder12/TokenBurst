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
    // ---------- setup ----------
    Tokenizer tok;
    if (!tok.load("data/vocab.json")) {
        printf("error: cannot load data/vocab.json — run from repo root\n");
        return 1;
    }
    printf("\n");

    // ---------- vocab tests ----------
    {
        CHECK(tok.vocab().size() > 0, "vocab loaded with tokens");
    }
    {
        CHECK(tok.vocab().get_id("Tokenburst") >= 0, "vocab has 'Tokenburst'");
    }
    {
        CHECK(tok.vocab().get_token(0) == "T", "token ID 0 is 'T'");
    }

    // ---------- single char ----------
    {
        auto ids = tok.encode("T");
        CHECK(ids.size() == 1, "single char 'T' -> 1 token");
    }
    {
        auto ids = tok.encode("o");
        CHECK(ids.size() == 1, "single char 'o' -> 1 token");
    }
    {
        auto ids = tok.encode(" ");
        CHECK(ids.size() == 1, "single space -> 1 token");
    }

    // ---------- known words ----------
    {
        auto ids = tok.encode("Token");
        CHECK(!ids.empty(), "'Token' -> non-empty");
    }
    {
        auto ids = tok.encode("Burst");
        CHECK(!ids.empty(), "'Burst' -> non-empty");
    }
    {
        auto ids = tok.encode("Tokenburst");
        CHECK(!ids.empty(), "'Tokenburst' -> non-empty");
    }
    {
        auto ids = tok.encode("burns");
        CHECK(!ids.empty(), "'burns' -> non-empty");
    }
    {
        auto ids = tok.encode("fast");
        CHECK(!ids.empty(), "'fast' -> non-empty");
    }
    {
        auto ids = tok.encode("its");
        CHECK(!ids.empty(), "'its' -> non-empty");
    }
    {
        auto ids = tok.encode("working");
        CHECK(!ids.empty(), "'working' -> non-empty");
    }

    // ---------- phrases ----------
    {
        auto ids = tok.encode("Tokenburst burns");
        CHECK(!ids.empty(), "'Tokenburst burns' -> non-empty");
    }
    {
        auto ids = tok.encode("burns fast");
        CHECK(!ids.empty(), "'burns fast' -> non-empty");
    }
    {
        auto ids = tok.encode("its working");
        CHECK(!ids.empty(), "'its working' -> non-empty");
    }

    // ---------- signature sentence ----------
    {
        std::string s = "Tokenburst burns fast, its working.";
        auto ids = tok.encode(s);
        printf("  signature: '%s' -> %zu tokens\n", s.c_str(), ids.size());
        CHECK(!ids.empty(), "signature sentence -> non-empty");
        CHECK(ids.size() < s.size(), "signature sentence compresses vs raw chars");
    }

    // ---------- round-trip ----------
    {
        std::string s = "Tokenburst burns fast, its working.";
        std::string decoded = tok.decode(tok.encode(s));
        CHECK(decoded == s, "round-trip: signature sentence");
    }
    {
        std::string s = "Token";
        CHECK(tok.decode(tok.encode(s)) == s, "round-trip: 'Token'");
    }
    {
        std::string s = "Burst";
        CHECK(tok.decode(tok.encode(s)) == s, "round-trip: 'Burst'");
    }
    {
        std::string s = "burns";
        CHECK(tok.decode(tok.encode(s)) == s, "round-trip: 'burns'");
    }
    {
        std::string s = "fast";
        CHECK(tok.decode(tok.encode(s)) == s, "round-trip: 'fast'");
    }

    // ---------- determinism ----------
    {
        std::string s = "Tokenburst burns fast, its working.";
        auto a = tok.encode(s);
        auto b = tok.encode(s);
        CHECK(a == b, "encode is deterministic");
    }

    // ---------- edge cases ----------
    {
        auto ids = tok.encode("");
        CHECK(ids.empty(), "empty input -> empty tokens");
    }
    {
        auto ids = tok.encode("zzz");
        CHECK(ids.empty(), "unknown chars -> empty tokens");
    }
    {
        auto ids = tok.encode("!!!");
        CHECK(ids.empty(), "punctuation not in vocab -> empty");
    }
    {
        std::vector<int64_t> empty;
        CHECK(tok.decode(empty).empty(), "decode empty -> empty string");
    }

    // ---------- unknown id ----------
    {
        std::string s = tok.vocab().get_token(999999);
        CHECK(s.empty(), "unknown token ID -> empty string");
    }

    printf("\n%d passed, %d failed\n", passed, failed);
    return failed == 0 ? 0 : 1;
}
