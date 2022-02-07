// Copyright (c) 2022 Vladimir Popov zor1994@gmail.com https://github.com/ZorPastaman/Linear-Memory-Allocator

#include "LinearMemoryAllocator.h"

#include <algorithm>
#include <memory>

namespace Zor {
namespace MemoryAllocators
{
	LinearMemoryAllocator::LinearMemoryAllocator(const size_t bufferSize) noexcept :
		bufferSize(bufferSize),
		m_buffer(new uint8_t[bufferSize]),
		m_remainingSize(bufferSize),
		m_nextPlace(m_buffer)
	{
	}

	LinearMemoryAllocator::LinearMemoryAllocator(LinearMemoryAllocator&& other) noexcept :
		bufferSize(other.bufferSize),
		m_buffer(other.m_buffer),
		m_remainingSize(other.m_remainingSize),
		m_nextPlace(other.m_nextPlace)
	{
		other.m_buffer = nullptr;
	}

	LinearMemoryAllocator::LinearMemoryAllocator(const size_t bufferSize, uint8_t* const buffer) noexcept :
		bufferSize(bufferSize),
		m_buffer(buffer),
		m_remainingSize(bufferSize),
		m_nextPlace(m_buffer)
	{
	}

	LinearMemoryAllocator::~LinearMemoryAllocator() noexcept
	{
		delete[] m_buffer;
	}

	void LinearMemoryAllocator::Reset() noexcept
	{
		std::fill(m_buffer, m_buffer + bufferSize, (uint8_t)0);
		m_remainingSize = bufferSize;
		m_nextPlace = m_buffer;
	}

	void* LinearMemoryAllocator::Allocate(const size_t alignment, const size_t size)
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

	void* LinearMemoryAllocator::AllocateTight(const size_t size)
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

	bool LinearMemoryAllocator::Has(const void* const pointer) const noexcept
	{
		return pointer >= m_buffer && pointer < m_buffer + bufferSize;
	}
}}
