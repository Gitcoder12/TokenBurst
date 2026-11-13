# TokenBurst — Feature Upgrade Plan

## Objective

A C++ tokenizer prototype that turns text into token IDs for language-model pipelines.

## P0 — honest CLI and evidence

- [ ] Fix README build and CLI commands against the actual compiled binary and required arguments.
- [ ] Remove unsupported speedup figures or supply a reproducible benchmark harness with correctness checks.

## P1 — tokenizer correctness

- [ ] Define and test tokenizer behavior against a named reference tokenizer and vocabulary format.
- [ ] Clarify the isolated CUDA kernel status and do not label tokenization GPU-accelerated until integrated end-to-end.

## P2 — performance and CI

- [ ] Add CPU single-thread/multithread benchmarks with dataset, hardware, warmup and repetitions.
- [ ] Add CI for CPU builds and correctness; add CUDA checks only on a supported runner.

## Contributor work packages

1. CLI/docs contract and smoke tests.
2. Vocabulary format and reference-tokenizer compatibility tests.
3. CPU benchmark harness and baseline.
4. CUDA integration proof only after end-to-end correctness validation.

## Definition of done

- [ ] Clean CPU build succeeds and documented commands run.
- [ ] Token IDs match the stated reference behavior on fixtures.
- [ ] Benchmarks are reproducible and not inflated by setup differences.
- [ ] GPU claims correspond to an integrated tested execution path.
