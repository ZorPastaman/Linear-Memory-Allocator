// Copyright (c) 2022 Vladimir Popov zor1994@gmail.com https://github.com/ZorPastaman/Linear-Memory-Allocator

#include "DynamicLinearMemoryAllocator.h"

#include <algorithm>
#include <memory>

namespace Zor {
namespace MemoryAllocators
{
	void DynamicLinearMemoryAllocator::Reset() noexcept
	{
		std::fill(m_buffer, m_buffer + bufferSize, (uint8_t)0);
		m_remainingSize = bufferSize;
		m_nextPlace = m_buffer;
	}

	void* DynamicLinearMemoryAllocator::Allocate(const size_t alignment, const size_t size)
	{
		void* place = m_nextPlace;

		if (std::align(alignment, size, place, m_remainingSize))
		{
			m_nextPlace = (uint8_t*)place + size;
			m_remainingSize -= size;

			return place;
		}

		throw std::bad_alloc();
	}

	void* DynamicLinearMemoryAllocator::AllocateTight(const size_t size)
	{
		if (m_remainingSize >= size)
		{
			void* place = m_nextPlace;
			m_nextPlace += size;
			m_remainingSize -= size;

			return place;
		}

		throw std::bad_alloc();
	}

	bool DynamicLinearMemoryAllocator::Has(const void* const pointer) const noexcept
	{
		return pointer >= m_buffer && pointer < m_buffer + bufferSize;
	}
}}
