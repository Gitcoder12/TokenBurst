# Agent Skills — TokenBurst

## C++ tokenizer audit
Trace CLI, vocabulary loading, preprocessing, token lookup and output.

## Reference correctness
Build fixtures and compare token IDs against a named compatible tokenizer.

## CUDA integration
Trace host/device data flow and verify that the production tokenization path invokes the kernel.

## Benchmark engineering
Record hardware, compiler, flags, dataset, warmup, repetitions, throughput and correctness.
