// Copyright (c) 2022 Vladimir Popov zor1994@gmail.com https://github.com/ZorPastaman/Linear-Memory-Allocator

#pragma once

#include "LinearMemoryAllocator.h"

namespace Zor {
namespace MemoryAllocators
{
	/// <summary>
	/// Preallocates a buffer of a specified size inside the allocator and linearly allocates objects into it.
	/// </summary>
	template<size_t TBufferSize = 1024>
	class TLinearMemoryAllocator : public LinearMemoryAllocator
	{
	public:
		static constexpr size_t bufferSizeValue = TBufferSize;

	private:
		char m_buffer[bufferSizeValue] = {};

	public:
		TLinearMemoryAllocator() noexcept :
			LinearMemoryAllocator(bufferSizeValue, m_buffer)
		{
		}

	private:
		TLinearMemoryAllocator(const TLinearMemoryAllocator&) = delete;
		TLinearMemoryAllocator(TLinearMemoryAllocator&&) = delete;

	public:
		virtual ~TLinearMemoryAllocator() noexcept override;

	private:
		TLinearMemoryAllocator& operator=(const TLinearMemoryAllocator&) = delete;
		TLinearMemoryAllocator& operator=(TLinearMemoryAllocator&&) = delete;
	};

	template<size_t bufferSizeValue>
	TLinearMemoryAllocator<bufferSizeValue>::~TLinearMemoryAllocator() noexcept
	{
		KillBufferPointer();
	}
}}
