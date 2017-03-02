#ifndef LINKED_PTR_HPP_INCLUDED
#define LINKED_PTR_HPP_INCLUDED

namespace Smr_ptr
{
template<typename T> class linked_ptr
{

public:
	template<class Y> friend class linked_ptr;

	linked_ptr( T *p = nullptr ) throw()
		: ptr( p )
	{
		prev = next = this;
	}

	~linked_ptr()
	{
		release();
	}

	linked_ptr( const linked_ptr &rhs ) throw()
	{
		acquire( rhs );
	}

	template<class Y> linked_ptr( const linked_ptr<Y> &rhs ) throw()
	{
		acquire( rhs );
	}

	bool operator==( const linked_ptr &rhs ) const
	{
		return this->ptr == rhs.ptr;
	}

	template<class Y> bool operator==( const linked_ptr<Y> &rhs )
	{
		return this->ptr == rhs.ptr;
	}

	bool operator!=( const linked_ptr &rhs ) const
	{
		return this->ptr != rhs.ptr;
	}

	template<class Y> bool operator!=( const linked_ptr<Y> &rhs )
	{
		return this->ptr != rhs.ptr;
	}

	bool operator <( const linked_ptr &rhs ) const
	{
		return this->ptr < rhs.ptr;
	}

	template<class Y> bool operator <( const linked_ptr<Y> &rhs ) const
	{
		return this->ptr < rhs.ptr;
	}

	bool operator >( const linked_ptr &rhs ) const
	{
		return this->ptr > rhs.ptr;
	}

	template<class Y> bool operator >( const linked_ptr<Y> &rhs ) const
	{
		return this->ptr > rhs.ptr;
	}

	friend std::ostream &operator<< ( std::ostream &out, const linked_ptr &rhs )
	{
		out << rhs.ptr;
		return out;
	}

	linked_ptr &operator=( const linked_ptr &rhs )
	{
		if ( this != &rhs )
		{
			release();
			acquire( rhs );
		}
		return *this;
	}

	template<class Y> linked_ptr &operator=( const linked_ptr<Y> &rhs )
	{
		if ( this != &rhs )
		{
			release();
			acquire( rhs );
		}
		return *this;
	}

	T &operator* () const throw()
	{
		return *ptr;
	}

	T *operator-> () const throw()
	{
		return ptr;
	}

	T *get() const throw()
	{
		return ptr;
	}

	operator bool() const
	{
		return ptr != nullptr;
	}

	bool unique() const throw()
	{
		return prev ? prev == this : true;
	}

private:
	T *ptr;
	const mutable linked_ptr *prev;
	const mutable linked_ptr *next;

	void release()
	{
		if ( unique() )
		{
			delete ptr;
		}
		else
		{
			this->next->prev = this->prev;
			this->prev->next = this->next;
			next = prev = nullptr;
		}
		ptr = nullptr;
	}

	void acquire( const linked_ptr &rhs ) throw()
	{
		this->ptr = rhs.ptr;
		this->next = rhs.next;
		this->next->prev = this;
		this->prev = &rhs;
		rhs.next = this;
	}

	template<class Y> void acquire( const linked_ptr<Y> &rhs ) throw()
	{
		this->ptr = rhs.ptr;
		this->next = reinterpret_cast<const linked_ptr<T> *>( rhs.next );
		this->next->prev = this;
		this->prev = reinterpret_cast<const linked_ptr<T> *>( &rhs );
		rhs.next = reinterpret_cast<const linked_ptr<Y> *>( this );
	}
};

}
#endif // LINKED_PTR_HPP_INCLUDED
