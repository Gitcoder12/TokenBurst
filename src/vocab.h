// src/vocab.h
// TokenBurst — Vocabulary loader.

#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>

namespace tokenburst {

class Vocab {
public:
    Vocab() = default;

    // Load vocab from JSON file. Format: {"token": id, ...}
    bool load(const std::string& path);

    // Look up ID for token string. Returns -1 if not found.
    int64_t get_id(const std::string& token) const;

    // Look up string for ID. Returns empty if not found.
    std::string get_token(int64_t id) const;

    // Total tokens in vocab.
    size_t size() const { return id_to_token_.size(); }

private:
    std::unordered_map<std::string, int64_t> token_to_id_;
    std::vector<std::string> id_to_token_;
};

}  // namespace tokenburst
