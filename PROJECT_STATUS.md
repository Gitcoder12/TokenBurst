# Project Status — TokenBurst

**Snapshot date:** 2026-11-16  
**Status type:** Initial documentation and repository-tree assessment  
**Default branch:** main  
**Review completeness:** Partial — not a code audit or release certification

## Project purpose
C++17 tokenizer with optional CUDA acceleration for batch/streaming text-to-token conversion in LLM pipelines.

## Current maturity (README-declared or upstream description; not independently verified)
Root includes CMakeLists.txt, src/, cuda/, data/, tests/ and README. README describes CPU and CUDA paths and reports 39x GPU speedup; that performance claim is not independently reproduced in this snapshot.

## Observed repository structure
CMakeLists.txt, src/, cuda/, data/, tests/, README.md and project guidance.

## Known issues and risks to investigate
Tokenizer correctness depends on exact vocabulary, normalization, byte-fallback and special-token semantics; compatibility with GPT-2/tiktoken is not established merely by README. Reported throughput numbers need benchmark harness, machine details, warm-up, input distribution and repeat runs. GPU speed comparison must use equivalent outputs and work. No build/tests/benchmarks run.

## Recommended next actions
Run CMake CPU build and tests first; test Unicode, empty strings, invalid UTF-8, special tokens and round-trip/decode semantics; verify vocab provenance/license; run CUDA tests on supported hardware; reproduce the throughput table and report hardware/compiler/build settings and multiple-run statistics.

## Verification record
| Check | Result |
|---|---|
| Repository metadata / fork relationship | Not applicable / not checked |
| Default-branch top-level tree | Inspected |
| README | Inspected |
| Deep source review | Not performed |
| Tests / build / runtime | Not run |
| Security / dependency audit | Not run |
| Independent review | Not performed |

## Change log
- 2026-11-16: Added this initial status snapshot on a review branch. No application code or existing data changed.

> Update this file only after the corresponding inspection or verification has actually been performed. Distinguish observed facts, README claims, hypotheses and unrun checks.