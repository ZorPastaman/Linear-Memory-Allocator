// Copyright (c) 2022 Vladimir Popov zor1994@gmail.com https://github.com/ZorPastaman/Linear-Memory-Allocator

#pragma once

#include <cstdint>
#include <utility>

namespace Zor {
namespace MemoryAllocators
{
	class LinearMemoryAllocator
	{
	public:
		const size_t bufferSize;

	private: 
		uint8_t* m_buffer;

		size_t m_remainingSize;
		uint8_t* m_nextPlace;

	public:
		LinearMemoryAllocator(const size_t bufferSize = 1024) noexcept;
		LinearMemoryAllocator(LinearMemoryAllocator&& other) noexcept;

	protected:
		LinearMemoryAllocator(const size_t bufferSize, uint8_t* const buffer) noexcept;

	private:
		LinearMemoryAllocator(const LinearMemoryAllocator&) = delete;

	public:
		virtual ~LinearMemoryAllocator() noexcept;

	public:
		void Reset() noexcept;

		void* Allocate(const size_t alignment, const size_t size);
		template<typename T, typename... TArgs>
		T* Allocate(TArgs&&... arguments);

		void* AllocateTight(const size_t size);
		template<typename T, typename... TArgs>
		T* AllocateTight(TArgs&&... arguments);

		bool Has(const void* const pointer) const noexcept;

		inline size_t getRemainingSize() const noexcept { return m_remainingSize; }

	protected:
		inline void KillBufferPointer() noexcept { m_buffer = nullptr; };

	private:
		LinearMemoryAllocator& operator=(const LinearMemoryAllocator&) = delete;
		LinearMemoryAllocator& operator=(LinearMemoryAllocator&& other) = delete;
	};

	template<typename T, typename ...TArgs>
	T* MemoryAllocators::LinearMemoryAllocator::Allocate(TArgs && ...arguments)
	{
		void* place = Allocate(alignof(T), sizeof(T));
		return new (place) T(std::forward<TArgs>(arguments)...);
	}

	template<typename T, typename ...TArgs>
	T* MemoryAllocators::LinearMemoryAllocator::AllocateTight(TArgs && ...arguments)
	{
		void* place = AllocateTight(sizeof(T));
		return new (place) T(std::forward<TArgs>(arguments)...);
	}
}}
