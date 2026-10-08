// src/vocab.cpp
// TokenBurst — Vocabulary loader implementation.

#include "vocab.h"

#include <fstream>
#include <sstream>
#include <iostream>

namespace tokenburst {

bool Vocab::load(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Vocab: cannot open " << path << "\n";
        return false;
    }

    std::stringstream buf;
    buf << file.rdbuf();
    std::string content = buf.str();

    // Minimal JSON parse: {"key": id, "key": id, ...}
    size_t i = 0;
    size_t n = content.size();

    while (i < n && content[i] != '{') i++;
    if (i == n) return false;
    i++;

    while (i < n) {
        while (i < n && (content[i] == ' ' || content[i] == '\n' ||
                         content[i] == '\t' || content[i] == ',')) i++;
        if (i >= n || content[i] == '}') break;

        if (content[i] != '"') return false;
        i++;

        std::string key;
        while (i < n && content[i] != '"') {
            if (content[i] == '\\' && i + 1 < n) {
                char esc = content[i + 1];
                if (esc == '"') key += '"';
                else if (esc == '\\') key += '\\';
                else if (esc == 'n') key += '\n';
                else if (esc == 't') key += '\t';
                else if (esc == 'r') key += '\r';
                else if (esc == 'u') {
                    if (i + 5 < n) {
                        unsigned cp = 0;
                        for (int k = 2; k <= 5; k++) {
                            char c = content[i + k];
                            cp <<= 4;
                            if (c >= '0' && c <= '9') cp |= (c - '0');
                            else if (c >= 'a' && c <= 'f') cp |= (c - 'a' + 10);
                            else if (c >= 'A' && c <= 'F') cp |= (c - 'A' + 10);
                        }
                        if (cp < 0x80) key += (char)cp;
                        else if (cp < 0x800) {
                            key += (char)(0xC0 | (cp >> 6));
                            key += (char)(0x80 | (cp & 0x3F));
                        } else {
                            key += (char)(0xE0 | (cp >> 12));
                            key += (char)(0x80 | ((cp >> 6) & 0x3F));
                            key += (char)(0x80 | (cp & 0x3F));
                        }
                        i += 4;
                    }
                } else key += esc;
                i += 2;
            } else {
                key += content[i++];
            }
        }
        if (i >= n) return false;
        i++;

        while (i < n && (content[i] == ' ' || content[i] == '\n' ||
                         content[i] == '\t' || content[i] == ':')) i++;

        int64_t id = 0;
        bool neg = false;
        if (i < n && content[i] == '-') { neg = true; i++; }
        while (i < n && content[i] >= '0' && content[i] <= '9') {
            id = id * 10 + (content[i] - '0');
            i++;
        }
        if (neg) id = -id;

        token_to_id_[key] = id;
        if ((size_t)id >= id_to_token_.size()) {
            id_to_token_.resize(id + 1);
        }
        id_to_token_[id] = key;
    }

    std::cerr << "Vocab: loaded " << id_to_token_.size() << " tokens\n";
    return true;
}

int64_t Vocab::get_id(const std::string& token) const {
    auto it = token_to_id_.find(token);
    return (it != token_to_id_.end()) ? it->second : -1;
}

std::string Vocab::get_token(int64_t id) const {
    if (id < 0 || (size_t)id >= id_to_token_.size()) return "";
    return id_to_token_[id];
}

}  // namespace tokenburst
