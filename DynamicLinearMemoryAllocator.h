// Copyright (c) 2022 Vladimir Popov zor1994@gmail.com https://github.com/ZorPastaman/Linear-Memory-Allocator

#pragma once

#include <cstdint>
#include <utility>

namespace Zor {
namespace MemoryAllocators
{
	class DynamicLinearMemoryAllocator
	{
	public:
		const size_t bufferSize;

	private: 
		size_t m_remainingSize;
		uint8_t* m_buffer;
		uint8_t* m_nextPlace;

	public:
		DynamicLinearMemoryAllocator(const size_t bufferSize) noexcept;
		DynamicLinearMemoryAllocator(DynamicLinearMemoryAllocator&& other) noexcept;

		~DynamicLinearMemoryAllocator() noexcept;

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
		DynamicLinearMemoryAllocator(const DynamicLinearMemoryAllocator&) = delete;

		DynamicLinearMemoryAllocator& operator=(const DynamicLinearMemoryAllocator&) = delete;
		DynamicLinearMemoryAllocator& operator=(DynamicLinearMemoryAllocator&& other) = delete;
	};

	template<typename T, typename ...TArgs>
	T* MemoryAllocators::DynamicLinearMemoryAllocator::Allocate(TArgs && ...arguments)
	{
		void* place = Allocate(alignof(T), sizeof(T));
		return new (place) T(std::forward<TArgs>(arguments)...);
	}

	template<typename T, typename ...TArgs>
	T* MemoryAllocators::DynamicLinearMemoryAllocator::AllocateTight(TArgs && ...arguments)
	{
		void* place = AllocateTight(sizeof(T));
		return new (place) T(std::forward<TArgs>(arguments)...);
	}
}
}
