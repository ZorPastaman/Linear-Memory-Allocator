// Copyright (c) 2022 Vladimir Popov zor1994@gmail.com https://github.com/ZorPastaman/Linear-Memory-Allocator

#pragma once

#include <cstdint>
#include <utility>

namespace Zor {
namespace MemoryAllocators
{
	template<size_t bufferSize = 1024>
	class LinearMemoryAllocator
	{
	public:
		static const size_t bufferSizeValue = bufferSize;

	private:
		uint8_t m_buffer[bufferSize] = {};
		size_t m_remainingSize;
		uint8_t* m_nextPlace;

	public:
		LinearMemoryAllocator() noexcept :
			m_remainingSize(bufferSize),
			m_nextPlace(m_buffer)
		{
		}

		void Reset() noexcept;

		void* Allocate(const size_t alignment, const size_t size);
		template<typename T, typename... TArgs>
		T* Allocate(TArgs&&... arguments);

		void* AllocateTight(const size_t size);
		template<typename T, typename... TArgs>
		T* AllocateTight(TArgs&&... arguments);

		bool Has(const void* const pointer) const noexcept;

		inline size_t getRemainingSize() const noexcept { return m_remainingSize; }

	private:
		LinearMemoryAllocator(const LinearMemoryAllocator&) = delete;
		LinearMemoryAllocator(LinearMemoryAllocator&&) = delete;

		LinearMemoryAllocator& operator=(const LinearMemoryAllocator&) = delete;
		LinearMemoryAllocator& operator=(LinearMemoryAllocator&&) = delete;
	};

	template<size_t bufferSize>
	void LinearMemoryAllocator<bufferSize>::Reset() noexcept
	{
		std::fill(std::begin(m_buffer), std::end(m_buffer), (uint8_t)0);
		m_remainingSize = bufferSize;
		m_nextPlace = std::begin(m_buffer);
	}

	template<size_t bufferSize>
	void* LinearMemoryAllocator<bufferSize>::Allocate(const size_t alignment, const size_t size)
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

	template<size_t bufferSize>
	template<typename T, typename... TArgs>
	T* LinearMemoryAllocator<bufferSize>::Allocate(TArgs&&... arguments)
	{
		void* place = Allocate(alignof(T), sizeof(T));
		return new (place) T(std::forward<TArgs>(arguments)...);
	}

	template<size_t bufferSize>
	void* LinearMemoryAllocator<bufferSize>::AllocateTight(const size_t size)
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

	template<size_t bufferSize>
	template<typename T, typename... TArgs>
	T* LinearMemoryAllocator<bufferSize>::AllocateTight(TArgs&&... arguments)
	{
		void* place = AllocateTight(sizeof(T));
		return new (place) T(std::forward<TArgs>(arguments)...);
	}

	template<size_t bufferSize>
	bool LinearMemoryAllocator<bufferSize>::Has(const void* const pointer) const noexcept
	{
		return pointer >= std::begin(m_buffer) && pointer < std::end(m_buffer);
	}
}}
