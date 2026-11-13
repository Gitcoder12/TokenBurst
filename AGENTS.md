# Agent Instructions — TokenBurst

Read README.md, CONTRIBUTING.md and FEATURE_UPGRADE_PLAN.md first.

- Inspect the actual CLI and tokenization call graph before changing claims.
- Preserve vocabulary/data compatibility or document breaking changes.
- Add token ID correctness tests against named reference fixtures.
- Keep CUDA status explicit; do not claim GPU acceleration unless end-to-end path uses it.
- Never invent performance figures. Run reproducible benchmarks or mark them not run.
- Work on a branch and open a PR; do not merge or publish packages autonomously.
