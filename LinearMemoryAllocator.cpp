// Copyright (c) 2022 Vladimir Popov zor1994@gmail.com https://github.com/ZorPastaman/Linear-Memory-Allocator

#include "LinearMemoryAllocator.h"

#include <new>
#include <memory>

namespace Zor {
namespace MemoryAllocators
{
	LinearMemoryAllocator::LinearMemoryAllocator(const size_t bufferSize) :
		bufferSize(bufferSize)
	{
		m_buffer = static_cast<char*>(malloc(bufferSize));
		if (!m_buffer)
		{
			throw std::bad_alloc();
		}

		m_nextPlace = m_buffer;
	}

	LinearMemoryAllocator::LinearMemoryAllocator(LinearMemoryAllocator&& other) noexcept :
		bufferSize(other.bufferSize),
		m_buffer(other.m_buffer),
		m_nextPlace(other.m_nextPlace)
	{
		other.m_buffer = nullptr;
	}

	LinearMemoryAllocator::LinearMemoryAllocator(const size_t bufferSize, char* const buffer) noexcept :
		bufferSize(bufferSize),
		m_buffer(buffer),
		m_nextPlace(m_buffer)
	{
	}

	LinearMemoryAllocator::~LinearMemoryAllocator() noexcept
	{
		free(m_buffer);
	}

	void* LinearMemoryAllocator::Allocate(const size_t alignment, const size_t size)
	{
		void* place = m_nextPlace;
		size_t remainingSize = getRemainingSize();

		if (std::align(alignment, size, place, remainingSize))
		{
			m_nextPlace = static_cast<char*>(place) + size;
			return place;
		}

		throw std::bad_alloc();
	}

	void* LinearMemoryAllocator::AllocateTight(const size_t size)
	{
		if (getRemainingSize() >= size)
		{
			void* place = m_nextPlace;
			m_nextPlace += size;

			return place;
		}

		throw std::bad_alloc();
	}

	bool LinearMemoryAllocator::Has(const void* const pointer) const noexcept
	{
		return pointer >= m_buffer && pointer < m_buffer + bufferSize;
	}
}}
