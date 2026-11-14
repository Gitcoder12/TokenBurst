# TokenBurst — README Explanation Draft

## In plain language

A C++ tokenizer prototype that turns text into token IDs for language-model pipelines.

## What it does today

Describe the supported CPU tokenizer command, the required vocabulary file and actual output format. Do not call the CUDA kernel part of tokenization if the main program does not invoke it.

## Technical explanation

Current audit found the README CLI examples omit a required vocabulary argument, output-format claims don't match the implementation, the CUDA kernel is a vocabulary-vector copy not integrated into tokenization, and published CPU/GPU performance figures lack a reproducible benchmark harness. Until corrected, describe it as an experimental CPU tokenizer prototype with an isolated CUDA proof of concept.

Document vocabulary format, pre-tokenization, token lookup, memory layout, threading, CLI flags, error behavior and the relationship between CUDA source and the executable path.

## Performance

Publish results only with hardware model, compiler flags, dataset checksum/size, warmup, repetitions, thread count, token correctness and baseline version.

## Contributing

See FEATURE_UPGRADE_PLAN.md and CONTRIBUTING.md.
