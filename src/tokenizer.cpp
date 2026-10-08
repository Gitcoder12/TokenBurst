// src/tokenizer.cpp
// TokenBurst — BPE tokenizer implementation.

#include "tokenizer.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

namespace tokenburst {

bool Tokenizer::load(const std::string& vocab_path) {
    if (!vocab_.load(vocab_path)) {
        return false;
    }

    // Build merge rank table from vocab.
    // In GPT-2 vocab, merged tokens contain the pair concatenated.
    // We derive merges: for each token, its two-child split has lower rank.
    // Simplified: rank = token ID (lower ID = merged earlier).
    for (size_t id = 0; id < vocab_.size(); id++) {
        std::string tok = vocab_.get_token(static_cast<int64_t>(id));
        if (tok.size() >= 2) {
            merge_rank_[tok] = static_cast<int>(id);
        }
    }

    std::cerr << "Tokenizer: ready with " << vocab_.size() << " tokens\n";
    return true;
}

// Split UTF-8 string into individual character tokens.
std::vector<std::string> Tokenizer::split_utf8(const std::string& text) const {
    std::vector<std::string> out;
    size_t i = 0;
    size_t n = text.size();

    while (i < n) {
        unsigned char c = static_cast<unsigned char>(text[i]);
        size_t len = 1;

        if (c < 0x80) len = 1;
        else if ((c & 0xE0) == 0xC0) len = 2;
        else if ((c & 0xF0) == 0xE0) len = 3;
        else if ((c & 0xF8) == 0xF0) len = 4;

        if (i + len > n) len = 1;  // safety

        out.push_back(text.substr(i, len));
        i += len;
    }
    return out;
}

// Find the best merge pair (lowest rank) in current token list.
std::pair<size_t, std::string> Tokenizer::find_best_merge(
    const std::vector<std::string>& tokens) const {

    size_t best_idx = SIZE_MAX;
    int best_rank = INT32_MAX;
    std::string best_pair;

    for (size_t i = 0; i + 1 < tokens.size(); i++) {
        std::string pair = tokens[i] + tokens[i + 1];
        auto it = merge_rank_.find(pair);
        if (it != merge_rank_.end() && it->second < best_rank) {
            best_rank = it->second;
            best_idx = i;
            best_pair = pair;
        }
    }

    return {best_idx, best_pair};
}

// Apply BPE merges repeatedly until no merge possible.
std::vector<std::string> Tokenizer::apply_bpe(
    std::vector<std::string> tokens) const {

    while (tokens.size() > 1) {
        auto [idx, merged] = find_best_merge(tokens);
        if (idx == SIZE_MAX) break;  // no more merges

        // Replace tokens[idx] and tokens[idx+1] with merged
        tokens[idx] = merged;
        tokens.erase(tokens.begin() + idx + 1);
    }

    return tokens;
}

std::vector<int64_t> Tokenizer::encode(const std::string& text) const {
    std::vector<int64_t> ids;

    if (text.empty()) return ids;

    // Split into UTF-8 characters
    std::vector<std::string> chars = split_utf8(text);

    // Apply BPE merges
    std::vector<std::string> merged = apply_bpe(chars);

    // Look up IDs
    for (const auto& tok : merged) {
        int64_t id = vocab_.get_id(tok);
        if (id < 0) {
            // Unknown token — skip (or could use <unk>)
            continue;
        }
        ids.push_back(id);
    }

    return ids;
}

std::string Tokenizer::decode(const std::vector<int64_t>& tokens) const {
    std::string out;
    for (int64_t id : tokens) {
        out += vocab_.get_token(id);
    }
    return out;
}

}  // namespace tokenburst
