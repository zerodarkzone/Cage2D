#ifndef DICTIONARY_HPP_INCLUDED
#define DICTIONARY_HPP_INCLUDED
#include "co\edu\unal\list\Exception\Exception.hpp"
#include "co\edu\unal\comp\Comp.hpp"
namespace JDict
{

	template <typename K, class E> class Dictionary
	{
		virtual E& get (const K& key) = 0;
		virtual E put (const K& key, const E& element) = 0;
		virtual E remove (const K& key) = 0;
	};
	template <typename K, class E> class Dictionary<K, E*>
	{
		virtual E* get (const K& key) = 0;
		virtual E* put (const K& key,  E* element) = 0;
		virtual E* remove (const K& key) = 0;
	};

	class Key_not_found: public std::exception
	{
	public:
		explicit Key_not_found (const char* _arg) _GLIBCXX_USE_NOEXCEPT
		{
			ex_arg = _arg;
		}
		virtual const char* what() const throw()
		{
			return ex_arg;
		}
		virtual ~Key_not_found() _GLIBCXX_USE_NOEXCEPT {}
	protected:
		const char* ex_arg;
	};
}
#endif // DICTIONARY_HPP_INCLUDED
