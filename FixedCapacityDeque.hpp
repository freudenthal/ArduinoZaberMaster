#ifndef ZABER_FIXED_CAPACITY_DEQUE_HPP // check for multiple inclusions
#define ZABER_FIXED_CAPACITY_DEQUE_HPP

#include <array>
#include <optional>
#include <type_traits>

namespace Zaber
{
	template<bool C, typename T, size_t S>
	class LoopIterator
	{
		// if type of elements are const, make array const to prevent compiler fussing over const iterators
		template<typename U>
		using add_const_if_c_t = std::conditional_t<C, const U, U>;
		using array_type = add_const_if_c_t<std::array<T, S>>;
		array_type& _array;
		size_t _index;
	public:
		// iterator traits
		using iterator_category = std::forward_iterator_tag;
		using value_type = T;
		using difference_type = long long;
		using pointer = add_const_if_c_t<T>*;
		using reference = add_const_if_c_t<T>&;

		// impl
		LoopIterator(array_type& array, size_t index) :
			_array(array), _index(index) {}

		LoopIterator& operator++() // prefix
		{
			_index++;
			return *this;
		}

		LoopIterator operator++(int) // postfix
		{
			LoopIterator retval = *this;
			++(*this); // call prefix definition for consistency
			return retval; // return copy of old value
		}

		bool operator==(LoopIterator other) const
		{
			return (&_array == &(other._array)) && (_index == other._index);
		}

		bool operator!=(LoopIterator other) const
		{
			return !(*this == other);
		}

		reference operator*()
		{
			return _array[_index % S];
		}

		long long operator-(LoopIterator other)
		{
			return _index - other._index;
		}
	};

	template <typename T, size_t S>
	class FixedCapacityDeque
	{
	private:
		std::array<T, S> _array = {};
		size_t _count = 0;
		size_t _indexFront = 0;
		size_t _indexBack = 0;

	public:
		FixedCapacityDeque() {};

		bool push_front(const T& t)
		{
			if (!full())
			{
				_count++;
				_array[preDecrementIndex(_indexFront)] = t;
				return true;
			}
			return false;
		};

		std::optional<T> pop_front()
		{
			if (!empty())
			{
				_count--;
				return _array[postIncrementIndex(_indexFront)];
			}
			return std::nullopt;
		};

		bool push_back(const T& t)
		{
			if (!full())
			{
				_count++;
				_array[postIncrementIndex(_indexBack)] = t;
				return true;
			}
			return false;
		};

		std::optional<T> pop_back()
		{
			if (!empty())
			{
				_count--;
				return _array[preDecrementIndex(_indexBack)];
			}
			return std::nullopt;
		};

		T& peek_front()
		{
			return _array[_indexFront];
		}

		T& peek_back()
		{
			return _array[(_indexBack == 0) ? (S - 1) : (_indexBack - 1)];
		}

		void clear()
		{
			_array = {};
			_count = 0;
			_indexFront = 0;
			_indexBack = 0;
		}

		size_t count() const { return _count; };
		bool full() const { return _count == S; };
		bool empty() const { return _count == 0; };

		// container traits
		using value_type = T;
		using reference = T&;
		using const_reference = const T&;
		using difference_type = long long;
		using size_type = size_t;
		using iterator = LoopIterator<false, T, S>;
		using const_iterator = LoopIterator<true, T, S>;

		// iterators allowing for-each
		iterator begin() { return iterator(_array, _indexFront); }
		iterator end() { return iterator(_array, _indexFront + _count); }
		const_iterator begin() const { return cbegin(); }
		const_iterator end() const { return cend(); }
		const_iterator cbegin() const { return const_iterator(_array, _indexFront); }
		const_iterator cend() const { return const_iterator(_array, _indexFront + _count); }


	private:
		size_t preDecrementIndex(size_t& index)
		{
			if (index != 0)
			{
				index--;
			}
			else
			{
				index = S - 1;
			}
			return index;
		};

		size_t postIncrementIndex(size_t& index)
		{
			size_t value = index;
			index++;
			if (index >= S) index -= S;
			return value;
		};
	};
}

#endif // ZABER_FIXED_CAPACITY_DEQUE_HPP
