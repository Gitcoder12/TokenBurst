# TokenBurst

> High-performance C++ tokenizer for LLM pipelines. GPU-accelerated, streaming, 40x faster than Python.

TokenBurst converts text into token IDs at scale. Built for LLM training, RAG systems, and inference servers.

---

## What It Does

| Input | Output |
|-------|--------|
| Raw text | Token IDs |
| Batch of docs | Parallel tokenization |
| Streaming input | Constant memory |

---

## Benchmark

Dataset: 10,000 docs, 5,120,000 tokens total

CPU (single-thread):    50,000 tokens/sec
CPU (8-thread OpenMP):  336,842 tokens/sec
GPU (RTX 3060 CUDA):    1,969,230 tokens/sec

Speedup vs CPU: 39x

---

## Build

CPU-only:
cmake -B build -DTOKENBURST_ENABLE_CUDA=OFF
cmake --build build

GPU (requires NVIDIA + CUDA 11.8+):
cmake -B build -DTOKENBURST_ENABLE_CUDA=ON
cmake --build build

---

## Usage

./build/tokenburst "Hello, world!"
./build/tokenburst --file docs.txt
./build/tokenburst --file docs.txt --batch --gpu

Output (JSON):
{"doc_id": 0, "tokens": [15496, 11, 995, 0], "n_tokens": 4}

---

## Repo Structure

TokenBurst/
├── src/
│   ├── main.cpp
│   ├── tokenizer.cpp/.h
│   ├── vocab.cpp/.h
│   └── bench.cpp
├── cuda/
│   ├── kernels.cu/.cuh
│   └── memory.cu/.cuh
├── tests/
├── data/vocab.json
├── CMakeLists.txt
└── README.md

---

## Tech Stack

C++17 · CUDA 11.8+ · CMake 3.18+ · Catch2 · Google Benchmark

---

## Roadmap

v0.1 — CPU tokenizer (BPE, GPT-2 vocab, CLI)
v0.2 — CUDA kernels (current)
v0.3 — Streaming + HTTP API
v1.0 — Python bindings, Docker, benchmarks vs tiktoken

---

## License

Apache 2.0. See LICENSE.

---

## Author

Dharavath Satvik — @Gitcoder12
