#ifndef JITERATOR_HPP_INCLUDED
#define JITERATOR_HPP_INCLUDED
//#include <boost/typeof/typeof.hpp>
//#include <boost/scoped_ptr.hpp>
//#include <memory>

namespace ssp
{
	template <typename T> class sm_ptr;
}
template<typename T> inline bool _Assign (T& a, T& b)
{
	a = b;
	return true;
}
#ifndef _for_each

#define _for_each(elem, list) \
	for(ssp::sm_ptr<JIterator::Iterator<decltype(elem)>> _ITER(list->iterator());\
			_ITER->hasNext() && ((elem = _ITER->next()) || true);)\

#endif
//This macro is faster than _for_each and than a range-based for loop
#ifndef _for

#define _for(_elem, _list) \
	for(auto _ITER(_list._iterator());\
			_ITER.hasNext() && (_Assign(_elem, _ITER.next()));)\

#endif

/*#ifndef _for

#define _for(elem, list) \
	for(auto _ITER(list._iterator());\
			_ITER.hasNext() && (_Assign(elem, _ITER.next()));)\

#endif*/

namespace JIterator
{
	template <class T> class Iterator
	{
	public:
		virtual bool hasNext() = 0;
		virtual T& next() = 0;
		virtual T& current() = 0;
		virtual void remove() = 0;
		virtual ~Iterator() {}
	};

	template <class T> class Iterable
	{
	public:
		virtual Iterator<T>* iterator() = 0;
		virtual ~Iterable() {}
	};
}


#endif // JITERATOR_HPP_INCLUDED
