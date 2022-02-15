// Copyright (c) 2022 Vladimir Popov zor1994@gmail.com https://github.com/ZorPastaman/Linear-Memory-Allocator

#pragma once

#include "LinearMemoryAllocator.h"

namespace Zor {
namespace MemoryAllocators
{
	/**
	 * Preallocates a buffer of a specified size inside the allocator and linearly allocates objects into it.
	 * @tparam NBufferSize
	 */
	template<size_t NBufferSize = 1024>
	class TLinearMemoryAllocator : public LinearMemoryAllocator
	{
	public:
		static constexpr size_t bufferSizeValue = NBufferSize;

	private:
		char m_buffer[bufferSizeValue] = {};

	public:
		TLinearMemoryAllocator() noexcept :
			LinearMemoryAllocator(bufferSizeValue, m_buffer)
		{
		}

		TLinearMemoryAllocator(const TLinearMemoryAllocator&) = delete;
		TLinearMemoryAllocator(TLinearMemoryAllocator&&) = delete;

	public:
		~TLinearMemoryAllocator() noexcept override;

	public:
		TLinearMemoryAllocator& operator=(const TLinearMemoryAllocator&) = delete;
		TLinearMemoryAllocator& operator=(TLinearMemoryAllocator&&) = delete;
	};

	template<size_t bufferSizeValue>
	TLinearMemoryAllocator<bufferSizeValue>::~TLinearMemoryAllocator() noexcept
	{
		KillBufferPointer();
	}
}}
