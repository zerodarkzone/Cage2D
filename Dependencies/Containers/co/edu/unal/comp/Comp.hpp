#ifndef COMP_HPP_INCLUDED
#define COMP_HPP_INCLUDED

template <typename T> struct Comp
{
	bool operator() (const T& f, const T& s)
	{
		return f < s;
	}
};
template <typename T> struct Comp<T*>
{
	bool operator() (const T* f, const T* s)
	{
		return *f < *s;
	}
};
#ifndef EQUALS_HPP_INCLUDED
#define EQUALS_HPP_INCLUDED

template <typename T> struct Equals
{
	bool operator() (const T& f, const T& s)
	{
		return f == s;
	}
};
template <typename T> struct Equals<T*>
{
	bool operator() (T* f, const T* s)
	{
		return *f == *s;
	}
};

#endif // EQUALS_HPP_INCLUDED

#endif // COMP_HPP_INCLUDED
