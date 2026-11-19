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


---

## AI-maintained project notes (secondary — 2026-11-13)

This section supplements the maintainer-authored README above; it does not replace or certify original claims.

- The current source tree, runtime configuration and executed tests are the implementation source of truth. Roadmap items and draft PRs are not shipped functionality.
- Preserve original model configurations, benchmark inputs and user-provided data. Never substitute synthetic or placeholder records silently.
- Record provider/model versions, prompt/configuration, seeds, latency/cost methodology, environment and reproducible test outputs for evaluation claims.
- Mark unverified metrics and unsupported provider capabilities as unknown/unverified.
- See [MODEL_AUDIT_ROUTER.md](MODEL_AUDIT_ROUTER.md) for independent model review instructions.

---

## Engineering Audit & Handover — 2026-11-17

**Reviewed commit:** `b4be2e4bec2610c1427db5b591e77deb51aabb1e` (`main`). Static inspection covered `src/tokenizer.cpp`, `src/vocab.cpp`, and `tests/test_tokenizer.cpp`; tests were not executed.

### Confirmed findings
- The inspected encoder splits input into UTF-8 character substrings and repeatedly merges adjacent strings using token IDs inferred from the vocabulary. It does not show the standard GPT-2 byte-level pre-tokenization and merge-rank implementation.
- Unknown tokens are silently skipped in `Tokenizer::encode()`, so encoding followed by decoding can lose input text.
- The current tests mostly assert that selected strings produce non-empty token lists and that one sample sentence compresses below its character count. They do not establish compatibility with GPT-2/tiktoken token IDs or correctness over arbitrary Unicode input.
- The README's throughput and 39x speedup figures were not reproduced in this audit. CUDA source files exist, but the inspected CPU tokenizer path does not establish that the advertised GPU benchmark uses equivalent tokenization semantics.

### Next actions
1. Implement the exact target tokenizer specification, including pre-tokenization, byte fallback, and canonical merge ranks.
2. Return explicit unknown-token errors or a lossless fallback; never silently drop input bytes.
3. Add golden compatibility tests against a trusted reference tokenizer and publish reproducible benchmark scripts, hardware, compiler flags, and datasets.

**Verification status:** static source/test inspection only; build, tests, reference-tokenizer compatibility, CUDA equivalence, and performance claims remain unverified.

---

## Examples Used
- Tokenizing a short sentence into token IDs
- Processing a large document collection in batches
- Comparing CPU and GPU tokenization throughput


---

## Comparison with Existing Open-Source Projects

TokenBurst is documented as a C++ tokenizer for LLM pipelines, with CPU parallelism, optional CUDA acceleration and streaming ambitions. The following projects are established tokenizer implementations and important correctness/performance baselines.

| Project | Established focus | Relationship to TokenBurst |
|---|---|---|
| [tiktoken](https://github.com/openai/tiktoken) | Fast BPE tokenizer implementation used for OpenAI model tokenization | A key compatibility and performance reference; TokenBurst should compare token IDs only when implementing the same encoding specification and vocabulary. |
| [Hugging Face Tokenizers](https://github.com/huggingface/tokenizers) | High-performance tokenization library with many algorithms and language bindings | A mature general-purpose tokenizer library; TokenBurst's proposed distinction is a C++/CUDA-oriented pipeline and streaming/batch throughput. |
| [SentencePiece](https://github.com/google/sentencepiece) | Language-independent subword tokenizer and training toolkit | A widely used tokenizer framework with different algorithm/configuration choices; it is a reference for tokenizer design, not necessarily a drop-in GPT-2 BPE equivalent. |

### Where TokenBurst is different
- **C++ implementation:** the project aims to keep tokenization close to native code rather than rely on a Python-level orchestration loop.
- **GPU acceleration direction:** the README describes optional CUDA kernels and batch/streaming use cases for LLM data pipelines.
- **Pipeline-oriented CLI:** the tool is designed to tokenize text files and batches as well as short strings.

### Where established projects are stronger
tiktoken, Hugging Face Tokenizers and SentencePiece have mature algorithm implementations, compatibility tests and broad adoption. TokenBurst's current engineering audit found that its inspected encoder does not demonstrate standard GPT-2 byte-level pre-tokenization/merge-rank behavior, and its advertised speedup was not reproduced in that audit.

### Correctness and benchmark requirements
First implement and document the exact tokenizer specification, including byte handling, pre-tokenization, merge ranks and unknown-token behavior. Add golden tests against the reference tokenizer and Unicode/losslessness tests. Benchmark only equivalent outputs with published hardware, compiler flags, dataset, thread settings and warm-up protocol. Do not claim a speedup until correctness and reproducibility are established. This is a scope comparison, not a validated performance ranking.
