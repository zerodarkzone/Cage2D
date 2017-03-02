#ifndef LINEARLIST_HPP_INCLUDED
#define LINEARLIST_HPP_INCLUDED
#include <iostream>
#include <cstdint>
#include <stdexcept>
#include <cstdlib>
#include <type_traits>
#include "jiterator\JIterator.hpp"
#include "Exception\Exception.hpp"
namespace JList
{
	/**
	*   class Linearlist
	*   Abstract class with necessary methods
	*/
	template <typename P>class LinearList
	{
	public:
		virtual bool isEmpty() = 0;
		virtual int getSize() = 0;
		virtual P& get (int index) = 0;
		virtual int indexOf (const P& elem) = 0;
		virtual P remove (int index) = 0;
		virtual void add (int index, const P& elem) = 0;
		virtual void clear() = 0;
		virtual P pop_back() = 0;
		virtual P pop_front() = 0;
		virtual void push_back (const P& elem) = 0;
		virtual void push_front (const P& elem) = 0;

		virtual ~LinearList() {}
	};
	template <typename P>class LinearList<P*>
	{
	public:
		virtual bool isEmpty() = 0;
		virtual int getSize() = 0;
		virtual P* get (int index) = 0;
		virtual int indexOf (P* elem) = 0;
		virtual P* remove (int index) = 0;
		virtual void add (int index, P* elem) = 0;
		virtual void clear() = 0;
		virtual P* pop_back() = 0;
		virtual P* pop_front() = 0;
		virtual void push_back (P* elem) = 0;
		virtual void push_front (P* elem) = 0;

		virtual ~LinearList() {}
	};
#ifndef OPERATOR_OVERLOADED_
#define OPERATOR_OVERLOADED_
	inline std::ostream& operator<< (std::ostream& out, int* i)
	{
		out << *i;
		return out;
	}
	inline std::ostream& operator<< (std::ostream& out, double* i)
	{
		out << *i;
		return out;
	}
	inline std::ostream& operator<< (std::ostream& out, float* i)
	{
		out << *i;
		return out;
	}
	inline std::ostream& operator<< (std::ostream& out, char* a)
	{
		out << *a;
		return out;
	}
	inline std::ostream& operator<< (std::ostream& out, std::string* str)
	{
		out << str->c_str();
		return out;
	}
#endif // OPERATOR_OVERLOADED_
}

#endif // LINEARLIST_HPP_INCLUDED
