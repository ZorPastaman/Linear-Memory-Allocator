// Copyright (c) 2022 Vladimir Popov zor1994@gmail.com https://github.com/ZorPastaman/Linear-Memory-Allocator

#pragma once

#include "LinearMemoryAllocator.h"

namespace Zor {
namespace MemoryAllocators
{
	template<size_t bufferSizeValue = 1024>
	class TLinearMemoryAllocator : public LinearMemoryAllocator
	{
	public:
		static constexpr size_t bufferSizeValue = bufferSizeValue;

	private:
		uint8_t m_buffer[bufferSizeValue] = {};

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
