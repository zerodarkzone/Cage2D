#ifndef EXCEPTION_HPP_INCLUDED
#define EXCEPTION_HPP_INCLUDED
#include <exception>


#ifdef _GLIBCXX_USE_NOEXCEPT
#undef _GLIBCXX_USE_NOEXCEPT
#endif // _GLIBCXX_USE_NOEXCEPT
#ifdef __MINGW64__
#define _GLIBCXX_USE_NOEXCEPT noexcept
#else
#ifdef __clang__
#define _GLIBCXX_USE_NOEXCEPT noexcept
#else
#ifdef _MSC_BUILD
#define _GLIBCXX_USE_NOEXCEPT throw()
#endif // _MSC_BUILD
#endif // __clang__
#endif // __MINGW64__
namespace except
{
	class Index_out_of_range: public std::exception
	{
	public:
		explicit Index_out_of_range( const char *_arg )_GLIBCXX_USE_NOEXCEPT
		{
			ex_arg = _arg;
		}
		virtual const char *what() const _GLIBCXX_USE_NOEXCEPT
		{
			return ex_arg;
		}
		Index_out_of_range(const Index_out_of_range& other) = default;
		virtual ~Index_out_of_range()_GLIBCXX_USE_NOEXCEPT{}
	protected:
		const char *ex_arg;
	};

	class No_such_element: public std::exception
	{
	public:
		explicit No_such_element( const char *_arg )_GLIBCXX_USE_NOEXCEPT
		{
			ex_arg = _arg;
		}
		virtual const char *what() const _GLIBCXX_USE_NOEXCEPT
		{
			return ex_arg;
		}
		No_such_element(const No_such_element& other) = default;
		virtual ~No_such_element()_GLIBCXX_USE_NOEXCEPT{}
	protected:
		const char *ex_arg;
	};

	class Unsuported_operation: public std::exception
	{
	public:
		explicit Unsuported_operation( const char *_arg )_GLIBCXX_USE_NOEXCEPT
		{
			ex_arg = _arg;
		}
		virtual const char *what() const _GLIBCXX_USE_NOEXCEPT
		{
			return ex_arg;
		}
		Unsuported_operation(const Unsuported_operation& other) = default;
		virtual ~Unsuported_operation()_GLIBCXX_USE_NOEXCEPT{}
	protected:
		const char *ex_arg;
	};


}


#endif // EXCEPTION_HPP_INCLUDED
