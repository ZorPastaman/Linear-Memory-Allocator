#pragma once

#include <cstdint>
#include <utility>

namespace Zor
{
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

			void Reset() noexcept
			{
				std::fill(std::begin(m_buffer), std::end(m_buffer), (uint8_t)0);
				m_remainingSize = bufferSizeValue;
				m_nextPlace = std::begin(m_buffer);
			}

			void* Allocate(size_t alignment, size_t size)
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

			template<typename T, typename... TArgs>
			T* Allocate(TArgs&&... arguments)
			{
				void* place = Allocate(alignof(T), sizeof(T));
				return new (place) T(std::forward<TArgs>(arguments)...);
			}

			bool Has(void* pointer) const noexcept
			{
				return pointer >= std::begin(m_buffer) && pointer < std::end(m_buffer);
			}

			inline size_t getRemainingSize() const noexcept { return m_remainingSize; }

		private:
			LinearMemoryAllocator(const LinearMemoryAllocator&) = delete;
			LinearMemoryAllocator(LinearMemoryAllocator&&) = delete;

			LinearMemoryAllocator& operator=(const LinearMemoryAllocator&) = delete;
			LinearMemoryAllocator& operator=(LinearMemoryAllocator&&) = delete;
		};
	}
}
