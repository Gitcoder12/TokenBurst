# TokenBurst — Activity Log

**Snapshot:** 2026-11-13

## Verified findings

Issue #1 documents CLI/docs mismatch, unverified throughput claims and an unintegrated CUDA kernel. No files were changed in that issue: https://github.com/Gitcoder12/TokenBurst/issues/1

## Current scope and limitations

Current audit found the README CLI examples omit a required vocabulary argument, output-format claims don't match the implementation, the CUDA kernel is a vocabulary-vector copy not integrated into tokenization, and published CPU/GPU performance figures lack a reproducible benchmark harness. Until corrected, describe it as an experimental CPU tokenizer prototype with an isolated CUDA proof of concept.

## Append-only record

| Date | Contributor / agent | Issue / PR | Files / benchmark | Compiler/hardware/data | Result | Evidence / caveats |
|---|---|---|---|---|---|---|
| YYYY-MM-DD | name | link | paths and benchmark | exact versions and dataset | pass / fail / not run | evidence |

Never publish speedup numbers without exact hardware, dataset, warmup, repetition count, correctness test and reproducible command.

## Portfolio documentation rollout — 2026-11-13

- Documentation PR: https://github.com/Gitcoder12/TokenBurst/pull/2
- Added project-specific feature plan, company-fit mapping, beginner/technical README explanation and README checklist.
- Added scoped agent/skills guidance and contributor workflow where appropriate.
- README now explains the user-facing workflow and the technical/research scope while retaining current limitations.
- Validation: documentation/source review only; no application runtime test or deployment is claimed by this docs PR.
- Status: draft PR, not merged.
