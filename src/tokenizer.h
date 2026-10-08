// src/tokenizer.h
// TokenBurst — BPE tokenizer interface.

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

#include "vocab.h"

namespace tokenburst {

class Tokenizer {
public:
    Tokenizer() = default;

    // Load vocab + build merge rank table.
    bool load(const std::string& vocab_path);

    // Encode a single text into token IDs.
    std::vector<int64_t> encode(const std::string& text) const;

    // Decode token IDs back into text.
    std::string decode(const std::vector<int64_t>& tokens) const;

    // Access vocab.
    const Vocab& vocab() const { return vocab_; }

private:
    // Split text into initial character tokens (UTF-8 aware).
    std::vector<std::string> split_utf8(const std::string& text) const;

    // Apply BPE merges to a list of word tokens.
    std::vector<std::string> apply_bpe(std::vector<std::string> tokens) const;

    // Find best merge pair given current tokens.
    // Returns (left_index, pair) or (SIZE_MAX, "") if no merge.
    std::pair<size_t, std::string> find_best_merge(
        const std::vector<std::string>& tokens) const;

    Vocab vocab_;
    // merge_rank_[pair] = rank (lower = higher priority)
    std::unordered_map<std::string, int> merge_rank_;
};

}  // namespace tokenburst
