// Copyright (c) 2022 Vladimir Popov zor1994@gmail.com https://github.com/ZorPastaman/Linear-Memory-Allocator

#pragma once

#include <cstdlib>
#include <utility>

namespace Zor {
namespace MemoryAllocators
{
	/**
	 * Preallocates a buffer of a specified size and linearly allocates objects into it.
	 */
	class LinearMemoryAllocator
	{
	public:
		const size_t bufferSize;

	private: 
		char* m_buffer;
		char* m_nextPlace;

	public:
		/**
		 * @param bufferSize
		 * @throws std::bad_alloc Thrown when it's impossible to allocate a desired buffer.
		 */
		LinearMemoryAllocator(size_t bufferSize = 1024);
		LinearMemoryAllocator(LinearMemoryAllocator&& other) noexcept;

		LinearMemoryAllocator(const LinearMemoryAllocator&) = delete;

	protected:
		/**
		 * Creates an allocator with an external buffer supplied by a derived class.
		 * @param bufferSize
		 * @param buffer
		 * @remark The @a buffer must be of size of the @a bufferSize.
		 * @remark If the @a buffer mustn't be destroyed on destruction,
		 * call @a KillBufferPointer() in a derived destructor.
		 */
		LinearMemoryAllocator(size_t bufferSize, char* buffer) noexcept;

	public:
		virtual ~LinearMemoryAllocator() noexcept;

	public:
		/**
		 * Allocates a placement in the buffer for an object of the specified parameters with alignment.
		 * @param alignment Alignment of the object.
		 * @param size Size of the object.
		 * @returns Placement for an object of the specified parameters.
		 * @throws std::bad_alloc Thrown when it's impossible to allocate a desired placement in the buffer.
		 * @see AllocateTight(size_t)
		 */
		void* Allocate(size_t alignment, size_t size);
		/**
		 * Allocates an object of type @a T in the buffer with alignment.
		 * @tparam T Allocated object type.
		 * @tparam TArgs Constructor argument types.
		 * @param arguments Constructor arguments.
		 * @returns Allocated object.
		 * @throws std::bad_alloc Thrown when it's impossible to allocate a desired placement in the buffer.
		 * @see AllocateTight(TArgs&&...)
		 */
		template<typename T, typename... TArgs>
		T* Allocate(TArgs&&... arguments);

		/**
		 * Allocates a placement in the buffer for an object of the specified parameters without alignment.
		 * @param size Size of the object.
		 * @returns Placement for an object of the specified parameters.
		 * @throws std::bad_alloc Thrown when it's impossible to allocate a desired placement in the buffer.
		 * @see Allocate(size_t, size_t)
		 */
		void* AllocateTight(size_t size);
		/**
		 * Allocates an object of type @a T in the buffer without alignment.
		 * @tparam T Allocated object type.
		 * @tparam TArgs Constructor argument types.
		 * @param arguments Constructor arguments.
		 * @returns Allocated object.
		 * @throws std::bad_alloc Thrown when it's impossible to allocate a desired placement in the buffer.
		 * @see Allocate(TArgs&&...)
		 */
		template<typename T, typename... TArgs>
		T* AllocateTight(TArgs&&... arguments);

		/**
 		* Starts object allocations from the begin.
 		*/
		inline void Reset() noexcept { m_nextPlace = m_buffer; };

		/**
		 * Checks if the @a pointer points to the memory in the buffer.
		 * @param pointer Checked pointer.
		 * @returns @p True if the @a pointer points to the memory in the buffer; @p false otherwise.
		 */
		bool Has(const void* pointer) const noexcept;

		inline size_t GetRemainingSize() const noexcept
		{
			return static_cast<size_t>(m_buffer + bufferSize - m_nextPlace);
		}

	protected:
		/**
		 * Kills the pointer to the buffer.
		 * It may be used in a derived destructor to prevent a destruction of the buffer in this class destructor.
		 */
		inline void KillBufferPointer() noexcept { m_buffer = nullptr; };

	public:
		LinearMemoryAllocator& operator=(const LinearMemoryAllocator&) = delete;
		LinearMemoryAllocator& operator=(LinearMemoryAllocator&&) = delete;
	};

	template<typename T, typename... TArgs>
	T* LinearMemoryAllocator::Allocate(TArgs&&... arguments)
	{
		void* const place = Allocate(alignof(T), sizeof(T));
		return new (place) T(std::forward<TArgs>(arguments)...);
	}

	template<typename T, typename... TArgs>
	T* LinearMemoryAllocator::AllocateTight(TArgs&&... arguments)
	{
		void* const place = AllocateTight(sizeof(T));
		return new (place) T(std::forward<TArgs>(arguments)...);
	}
}}
