# Linear Memory Allocator

Usual operators malloc and new are awesome but they have some disadvantages: 
memory allocation is random and memory fragmentation is very possible.
One of the solutions is a linear memory allocator. Such an allocator preallocates a buffer of a linear memory segment 
and allocates all the object linearly into the buffer.

A linear memory allocator is especially useful if you want to keep pointers to different objects (especially of different size)
and you know that you access them in a special order.

## Setup

This repo is just a code. Download it and include the headers.

## Usage

### [LinearMemoryAllocator](LinearMemoryAllocator.h)

This is a basic linear memory allocator. Its constructor takes an argument of a buffer size and dynamically allocates a buffer.
It's your responsibility to set the size that can contain all the object allocated into it.

### [TLinearMemoryAllocator](TLinearMemoryAllocator.h)

This a class derived from [LinearMemoryAllocator](LinearMemoryAllocator.h). 
It takes a buffer size argument in its template parameter and 
statically allocates a buffer right after all the other fields in the allocator.
It may make a memory usage even more efficient but the compiler must know its size in advance.
