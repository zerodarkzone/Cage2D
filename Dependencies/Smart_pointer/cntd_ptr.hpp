#ifndef CNTD_PTR_HPP_INCLUDED
#define CNTD_PTR_HPP_INCLUDED
namespace Smr_ptr
{
template<typename T> class cntd_ptr;
template<typename T> class Counter
{
public:
	friend class cntd_ptr<T>;
	template<class Y> friend class cntd_ptr;
	~Counter()
	{
		if ( ptr )
		{
			delete ptr;
		}
	}

	friend std::ostream &operator<< ( std::ostream &out, const Counter &rhs )
	{
		out << rhs.ptr;
		return out;
	}

private:
	Counter( T *p = nullptr ) throw()
		: ptr( p ), _count( 0 ) {}

	T *ptr;
	mutable int _count;
};

template<typename T> class cntd_ptr
{
public:
	template<class Y> friend class cntd_ptr;


	cntd_ptr( T *p = nullptr ) throw()
	{
		this->_count = new Smr_ptr::Counter<T>( p );
		++this->_count->_count;
	}


	cntd_ptr( const cntd_ptr &rhs ) throw()
	{
		this->_count = rhs._count;
		if ( this->_count )
		{
			++this->_count->_count;
		}
	}


	~cntd_ptr()
	{
		if ( _count && --_count->_count == 0 )
		{
			delete _count;
		}
	}


	template<class Y> cntd_ptr( const cntd_ptr<Y> &rhs ) throw()
	{
		this->_count = reinterpret_cast<const Counter<T> *>( rhs._count );
		if ( this->_count )
		{
			++this->_count->_count;
		}
	}


	cntd_ptr &operator = ( const cntd_ptr &rhs )
	{
		Counter<T> const *_old = this->_count;
		this->_count = rhs._count;
		if ( this->_count )
		{
			++this->_count->_count;
		}
		if ( _old && --_old->_count == 0 )
		{
			delete _old;
		}
		return *this;
	}


	template<class Y> cntd_ptr &operator = ( const cntd_ptr<Y> &rhs )
	{
		Counter<T> const *_old = this->_count;
		this->_count = reinterpret_cast<const Counter<T> *>( rhs._count );
		if ( this->_count )
		{
			++this->_count->_count;
		}
		if ( _old && --_old->_count == 0 )
		{
			delete _old;
		}
		return *this;
	}

	friend std::ostream &operator<< ( std::ostream &out, const cntd_ptr &rhs )
	{
		out << rhs._count;
		return out;
	}

	bool operator == ( const cntd_ptr &rhs ) const
	{
		return this->_count->ptr == rhs._count->ptr;
	}


	template<class Y> bool operator == ( const cntd_ptr<Y> &rhs ) const
	{
		return this->_count->ptr == rhs._count->ptr;
	}


	bool operator != ( const cntd_ptr &rhs ) const
	{
		return this->_count->ptr != rhs._count->ptr;
	}


	template<class Y> bool operator != ( const cntd_ptr<Y> &rhs ) const
	{
		return this->_count->ptr != rhs._count->ptr;
	}

	bool operator <( const cntd_ptr &rhs ) const
	{
		return this->_count->ptr < rhs._count->ptr;
	}

	template<class Y> bool operator <( const cntd_ptr<Y> &rhs ) const
	{
		return this->_count->ptr < rhs._count->ptr;
	}

	bool operator >( const cntd_ptr &rhs ) const
	{
		return this->_count->ptr > rhs._count->ptr;
	}

	template<class Y> bool operator >( const cntd_ptr<Y> &rhs ) const
	{
		return this->_count->ptr > rhs._count->ptr;
	}

	T &operator* () const throw()
	{
		return *( _count->ptr );
	}


	T *operator-> () const throw()
	{
		return  _count->ptr;
	}


	T *get() const throw()
	{
		return  _count->ptr;
	}

	operator bool() const
	{
		return _count->ptr != nullptr;
	}

	bool unique()
	{
		return _count ? _count->_count == 1 : true;
	}

private:
	const mutable Counter<T> *_count;
};

}
#endif // CNTD_PTR_HPP_INCLUDED
