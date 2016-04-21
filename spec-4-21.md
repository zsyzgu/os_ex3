####在 ucore 中，目前Stride是采用无符号的32位整数表示。则BigStride应该取多少，才能保证比较的正确性？

由于STRIDE_MAX - STRIDE_MIN <= PASS_MAX，而PASS_MAX <= BigStride。所以保证int(BigStride) > 0即可，也就是BigStride<=INT_MAX(0x7fffffff)。
