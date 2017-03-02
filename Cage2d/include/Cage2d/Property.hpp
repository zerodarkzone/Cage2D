#ifndef PROPERTY_HPP_INCLUDED
#define PROPERTY_HPP_INCLUDED
#include <functional>

template <typename _Type, class... _Object>
class Property;

template <typename _Type, class... _Object>
class ReadOnlyProperty;

template <typename _Type, class _Object>
class Property<_Type, _Object>
{
public:
	typedef _Type type;
protected:
	mutable type _var;
	type& _stvar;
	const _Object *_obj;
	std::function<void( type )> _Set;
	std::function<type( void )> _Get;
public:
	Property( ) : _var( ), _stvar( _var ), _obj( nullptr ) {}
	Property( type& var ) : _var( ), _stvar( var ), _obj( nullptr ) {}
	Property( const type& var ) : _var( var ), _stvar( _var ), _obj( nullptr ) {}

	Property( type& var, void ( _Object::*_set )( type ), _Object *obj ) : _var( ), _stvar( var ), _obj( obj )
	{
		if( obj )
		{
			_Set = std::bind( _set, obj, std::placeholders::_1 );
		}
	}

	Property( const type& var, void ( _Object::*_set )( type ), _Object *obj ) : _var( var ), _stvar( _var ), _obj( obj )
	{
		if( obj )
		{
			_Set = std::bind( _set, obj, std::placeholders::_1 );
		}
	}

	Property( type& var, const std::function<void( type )>& _set ) : _var( ), _stvar( var ), _obj( nullptr ), _Set( _set )
	{}

	Property( const type& var, const std::function<void( type )>& _set ) : _var( var ), _stvar( _var ), _obj( nullptr ), _Set( _set )
	{}

	Property( type& var, type( _Object::*_get )( void ), _Object *obj ) : _var( ), _stvar( var ), _obj( obj )
	{
		if( obj )
		{
			_Get = std::bind( _get, obj );
		}
	}

	Property( const type& var, type( _Object::*_get )( void ), _Object *obj ) : _var( var ), _stvar( _var ), _obj( obj )
	{
		if( obj )
		{
			_Get = std::bind( _get, obj );
		}
	}

	Property( type& var, const std::function<type( void )>& _get ) : _var( ), _stvar( var ), _obj( nullptr ), _Get( _get )
	{}

	Property( const type& var, const std::function<type( void )>& _get ) : _var( var ), _stvar( _var ), _obj( nullptr ), _Get( _get )
	{}

	Property( type& var, void ( _Object::*_set )( type ), type( _Object::*_get )( void ), _Object *obj ) : _var( ), _stvar( var ), _obj( obj )
	{
		if( obj )
		{
			_Set = std::bind( _set, obj, std::placeholders::_1 );
			_Get = std::bind( _get, obj );
		}
	}

	Property( const type& var, void ( _Object::*_set )( type ), type( _Object::*_get )( void ), _Object *obj ) : _var( var ), _stvar( _var ), _obj( obj )
	{
		if( obj )
		{
			_Set = std::bind( _set, obj, std::placeholders::_1 );
			_Get = std::bind( _get, obj );
		}
	}

	Property( type& var, type( _Object::*_get )( void ), void ( _Object::*_set )( type ), _Object *obj ) : _var( ), _stvar( var ), _obj( obj )
	{
		if( obj )
		{
			_Set = std::bind( _set, obj, std::placeholders::_1 );
			_Get = std::bind( _get, obj );
		}
	}

	Property( const type& var, type( _Object::*_get )( void ), void ( _Object::*_set )( type ), _Object *obj ) : _var( var ), _stvar( _var ), _obj( obj )
	{
		if( obj )
		{
			_Set = std::bind( _set, obj, std::placeholders::_1 );
			_Get = std::bind( _get, obj );
		}
	}

	Property( type& var, const std::function<void( type )>& _set, const std::function<type( void )>& _get ) : _var( ), _stvar( var ), _obj( nullptr ), _Set( _set ), _Get( _get )
	{}

	Property( const type& var, const std::function<void( type )>& _set, const std::function<type( void )>& _get ) : _var( var ), _stvar( _var ), _obj( nullptr ), _Set( _set ), _Get( _get )
	{}

	Property( type& var, const std::function<type( void )>& _get, const std::function<void( type )>& _set ) : _var( ), _stvar( var ), _obj( nullptr ), _Set( _set ), _Get( _get )
	{}

	Property( const type& var, const std::function<type( void )>& _get, const std::function<void( type )>& _set ) : _var( var ), _stvar( _var ), _obj( nullptr ), _Set( _set ), _Get( _get )
	{}

	virtual ~Property( ) {}
	Property<_Type, _Object>& operator = ( const Property<_Type, _Object>& rhs )
	{
		if ( this == &rhs )
			return *this;
		if( _Set ){
			if( rhs._Get )
				_Set( rhs._Get( ) );
			else
				_Set( rhs._stvar );
		}else{
			if( rhs._Get )
				_stvar = rhs._Get( );
			else
				_stvar = rhs._stvar;
		}
		return *this;
	}
	Property<_Type, _Object>& operator = ( const ReadOnlyProperty<_Type, _Object>& rhs ) = delete;

	type& operator = ( const type& val )
	{
		if( _Set )
		{
			_Set( val );
			return _stvar;
		}
		return ( _stvar = val );
	}

	operator type const& ( ) const
	{
		if( _Get )
		{
			return ( _var = _Get( ) );
		}
		return ( _var = _stvar );
	}
};


template <typename _Type, class _Object>
class ReadOnlyProperty<_Type, _Object> : Property<_Type, _Object>
{
public:
	typedef _Type type;
	ReadOnlyProperty( ) : Property<type, _Object>( ) {}
	ReadOnlyProperty( type& var ) : Property<type, _Object>( var ) {}
	ReadOnlyProperty( const type& var ) : Property<type, _Object>( var ) {}
	ReadOnlyProperty( type& var, type( _Object::*_get )( void ), _Object *obj ) : Property<type, _Object>( var, _get, obj ) {}
	ReadOnlyProperty( const type& var, type( _Object::*_get )( void ), _Object *obj ) : Property<type, _Object>( var, _get, obj ) {}
	ReadOnlyProperty( type& var, const std::function<type( void )>& _get ) : Property<type, _Object>( var, _get ) {}
	ReadOnlyProperty( const type& var, const std::function<type( void )>& _get ) : Property<type, _Object>( var, _get ) {}

	~ReadOnlyProperty( ) {}
	ReadOnlyProperty<_Type, _Object>& operator = ( const Property<_Type, _Object>& rhs ) = delete;
	ReadOnlyProperty<_Type, _Object>& operator = ( const ReadOnlyProperty<_Type, _Object>& rhs ) = delete;
	type& operator = ( const type& val ) = delete;
	operator type const& ( ) const
	{
		if( this->_Get )
		{
			return ( this->_var = this->_Get( ) );
		}
		return ( this->_var = this->_stvar );
	}
};

template <typename _Type>
class Property<_Type>
{

public:
	typedef _Type type;
protected:
	mutable type _var;
	type& _stvar;
	std::function<void( type )> _Set;
	std::function<type( void )> _Get;
public:
	Property( ) : _var( ), _stvar( _var ) {}
	Property( type& var ) : _var( ), _stvar( var ) {}
	Property( const type& var ) : _var( var ), _stvar( _var ) {}

	Property( type& var, const std::function<void( type )>& _set ) : _var( ), _stvar( var )
	{
		_Set = _set;
	}

	Property( const type& var, const std::function<void( type )>& _set ) : _var( var ), _stvar( _var )
	{
		_Set = _set;
	}

	Property( type& var, const std::function<type( void )>& _get ) : _var( ), _stvar( var )
	{
		_Get = _get;
	}

	Property( const type& var, const std::function<type( void )>& _get ) : _var( var ), _stvar( _var )
	{
		_Get = _get;
	}

	Property( type& var, const std::function<void( type )>& _set, const std::function<type( void )>& _get ) : _var( ), _stvar( var )
	{
		_Set = _set;
		_Get = _get;
	}

	Property( const type& var, const std::function<void( type )>& _set, const std::function<type( void )>& _get ) : _var( var ), _stvar( _var )
	{
		_Set = _set;
		_Get = _get;
	}

	Property( type& var, const std::function<type( void )>& _get, const std::function<void( type )>& _set ) : _var( ), _stvar( var )
	{
		_Set = _set;
		_Get = _get;
	}

	Property( const type& var, const std::function<type( void )>& _get, const std::function<void( type )>& _set ) : _var( var ), _stvar( _var )
	{
		_Set = _set;
		_Get = _get;
	}

	virtual ~Property( ) {}
	Property<_Type>& operator = ( const Property<_Type>& rhs )
	{
		if ( this == &rhs )
			return *this;
		if( _Set ){
			if( rhs._Get )
				_Set( rhs._Get( ) );
			else
				_Set( rhs._stvar );
		}else{
			if( rhs._Get )
				_stvar = rhs._Get( );
			else
				_stvar = rhs._stvar;
		}
		return *this;
	}
	Property<_Type>& operator = ( const ReadOnlyProperty<_Type>& rhs ) = delete;

	type& operator = ( const type& val )
	{
		if( _Set )
		{
			_Set( val );
			return _stvar;
		}
		return ( _stvar = val );
	}

	operator type const& ( ) const
	{
		if( _Get )
		{
			return ( _var = _Get( ) );
		}
		return ( _var = _stvar );
	}

};

template<typename _Type>
class ReadOnlyProperty<_Type> : Property<_Type>
{

public:
	typedef _Type type;
	ReadOnlyProperty( ) : Property<type>( ) {}
	ReadOnlyProperty( type& var ) : Property<type>( var ) {}
	ReadOnlyProperty( const type& var ) : Property<type>( var ) {}
	ReadOnlyProperty( type& var, const std::function<type( void )>& _get ) : Property<type>( var, _get ) {}
	ReadOnlyProperty( const type& var, const std::function<type( void )>& _get ) : Property<type>( var, _get ) {}

	~ReadOnlyProperty( ) {}
	ReadOnlyProperty<_Type>& operator = ( const Property<_Type>& rhs ) = delete;
	ReadOnlyProperty<_Type>& operator = ( const ReadOnlyProperty<_Type>& rhs ) = delete;
	type& operator = ( const type& val ) = delete;
	operator type const& ( ) const
	{
		if( this->_Get )
		{
			return ( this->_var = this->_Get( ) );
		}
		return ( this->_var = this->_stvar );
	}
};

#endif // PROPERTY_HPP_INCLUDED
