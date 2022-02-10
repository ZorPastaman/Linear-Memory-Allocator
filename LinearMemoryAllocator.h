// Copyright (c) 2022 Vladimir Popov zor1994@gmail.com https://github.com/ZorPastaman/Linear-Memory-Allocator

#pragma once

#include <utility>

namespace Zor {
namespace MemoryAllocators
{
	/// <summary>
	/// Preallocates a buffer of a specified size and linearly allocates objects into it.
	/// </summary>
	class LinearMemoryAllocator
	{
	public:
		const size_t bufferSize;

	private: 
		char* m_buffer;
		char* m_nextPlace;

	public:
		/// <param name="bufferSize"></param>
		/// <exception cref="std::bad_alloc">Thrown when it's impossible to allocate a desired buffer.</exception>
		LinearMemoryAllocator(const size_t bufferSize = 1024);
		LinearMemoryAllocator(LinearMemoryAllocator&& other) noexcept;

	protected:
		/// <summary>
		/// Creates an allocator with an external buffer supplied by a derived class.
		/// </summary>
		/// <param name="bufferSize"></param>
		/// <param name="buffer"></param>
		/// <remarks>
		/// <para>The <paramref name="buffer"/> must be of size of the <paramref name="bufferSize"/>.</para>
		/// <para>If the <paramref name="buffer"/> mustn't be destroyed on destruction, call <see cref="KillBufferPointer"/> in a derived destructor.</para>
		/// </remarks>
		LinearMemoryAllocator(const size_t bufferSize, char* const buffer) noexcept;

	private:
		LinearMemoryAllocator(const LinearMemoryAllocator&) = delete;

	public:
		virtual ~LinearMemoryAllocator() noexcept;

	public:
		/// <summary>
		/// Clears the buffer and starts object allocations from the begin.
		/// </summary>
		void Reset() noexcept;

		/// <summary>
		/// Allocates a placement in the buffer for an object of the specified parameters with alignment.
		/// </summary>
		/// <param name="alignment">Alignment of the object.</param>
		/// <param name="size">Size of the object.</param>
		/// <returns>Placement for an object of the specified parameters.</returns>
		/// <exception cref="std::bad_alloc">Thrown when it's impossible to allocate a desired placement in the buffer.</exception>
		/// <seealso cref="AllocateTight"/>
		void* Allocate(const size_t alignment, const size_t size);
		/// <summary>
		/// Allocates an object of type <typeparamref name="T"/> in the buffer with alignment.
		/// </summary>
		/// <typeparam name="T">Allocated object type.</typeparam>
		/// <typeparam name="...TArgs">Constructor argument types.</typeparam>
		/// <param name="...arguments">Constructor arguments.</param>
		/// <returns>Allocated object.</returns>
		/// <exception cref="std::bad_alloc">Thrown when it's impossible to allocate a desired placement in the buffer.</exception>
		/// <seealso cref="AllocateTight"/>
		template<typename T, typename... TArgs>
		T* Allocate(TArgs&&... arguments);

		/// <summary>
		/// Allocates a placement in the buffer for an object of the specified parameters without alignment.
		/// </summary>
		/// <param name="size">Size of the object.</param>
		/// <returns>Placement for an object of the specified parameters.</returns>
		/// <exception cref="std::bad_alloc">Thrown when it's impossible to allocate a desired placement in the buffer.</exception>
		/// <seealso cref="Allocate"/>
		void* AllocateTight(const size_t size);
		/// <summary>
		/// Allocates an object of type <typeparamref name="T"/> in the buffer without alignment.
		/// </summary>
		/// <typeparam name="T">Allocated object type.</typeparam>
		/// <typeparam name="...TArgs">Constructor argument types.</typeparam>
		/// <param name="...arguments">Constructor arguments.</param>
		/// <returns>Allocated object.</returns>
		/// <exception cref="std::bad_alloc">Thrown when it's impossible to allocate a desired placement in the buffer.</exception>
		/// <seealso cref="Allocate"/>
		template<typename T, typename... TArgs>
		T* AllocateTight(TArgs&&... arguments);

		/// <summary>
		/// Checks if the <paramref name="pointer"/> points to the memory in the buffer.
		/// </summary>
		/// <param name="pointer">Checked pointer</param>
		/// <returns><see langword="True"/> if the <paramref name="pointer"/> points to the memory in the buffer; <see langword="false"/> otherwise.</returns>
		bool Has(const void* const pointer) const noexcept;

		inline size_t getRemainingSize() const noexcept { return static_cast<size_t>(m_buffer + bufferSize - m_nextPlace); }

	protected:
		/// <summary>
		/// Kills the pointer to the buffer. It may be used in a derived destructor to prevent a destruction of the buffer in this class destructor.
		/// </summary>
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
