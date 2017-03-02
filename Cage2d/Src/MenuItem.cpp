#include "Cage2d/stdafx.h"
#include "Cage2d/MenuItem.hpp"

namespace Cage2d
{
	MenuItem::MenuItem( ) : _hasImage( false )
	{
		_rect.set( -1, -1, 1, 1 );
		_font = alx::Font( "Data/Fonts/arial.ttf", 28 );
		_selectedFont = alx::Font( "Data/Fonts/arial.ttf", 38 );
		if( !_font || !_selectedFont )
		{
			std::cerr << "Could not load font" << std::endl;
		}
		_rect.setSize( _font.getHeight( ) + 10, _font.getHeight( ) + 10 );
		_color = al_color_name( "white" );
		_isSelected = false;
	}

	MenuItem::MenuItem( std::string text ) : MenuItem( )
	{
		_text = text;
	}

	void MenuItem::SetAction( const std::function<void( )>& action )
	{
		_action = action;
	}

	void MenuItem::ChangeSelect( bool select )
	{
		_isSelected = select;
	}

	void MenuItem::SetPosition( double x, double y )
	{
		if( _hasImage )
		{
			_image.setPosition( x, y );
		}
		_rect.setCenter( x, y );
	}

	void MenuItem::SetSize( double width, double height )
	{
		if( _hasImage )
		{
			double _w = width / _image.Width;
			double _h = height / _image.Height;
			_image.setScale( _w, _h );
		}
		_rect.setSize( width, height );
	}

	void MenuItem::SetImage( const sp::Sprite& sprite )
	{
		if( sprite )
		{
			_hasImage = true;
			_image = sprite;
			_rect.setSize( _image.Width, _image.Height );
		}
	}

	void MenuItem::SetBounds( const alx::Rect<double>& rect )
	{
		_rect = rect;
		if( _hasImage )
		{
			_image.setPosition( _rect.getCenter( ) );
			double _w = _rect.getSize( ).getWidth( ) / _image.Width;
			double _h = _rect.getSize( ).getHeight( ) / _image.Height;
			_image.setScale( _w, _h );
		}
	}

	void MenuItem::SetBounds( double left, double top, double right, double bottom )
	{
		_rect.set( left, top, right, bottom );
		if( _hasImage )
		{
			_image.setPosition( _rect.getCenter( ) );
			double _w = _rect.getSize( ).getWidth( ) / _image.Width;
			double _h = _rect.getSize( ).getHeight( ) / _image.Height;
			_image.setScale( _w, _h );
		}
	}

	void MenuItem::SetBounds( const alx::Point<double>& topLeft, const alx::Size<double>& size )
	{
		_rect.set( topLeft, size );
		if( _hasImage )
		{
			_image.setPosition( _rect.getCenter( ) );
			double _w = _rect.getSize( ).getWidth( ) / _image.Width;
			double _h = _rect.getSize( ).getHeight( ) / _image.Height;
			_image.setScale( _w, _h );
		}
	}

	void MenuItem::SetFont( const alx::Font& font, const alx::Font& sfont )
	{
		if( font && sfont )
		{
			_font = font;
			_selectedFont = sfont;
			SetSize( _font.getWidth( _text.c_str( ) ) + 20, _font.getHeight( ) + 10 );
		}
		else
		{
			std::cerr << "Couldn't load MenuItem font" << std::endl;
		}
	}

	void MenuItem::SetFont( const std::string& filename, int size )
	{
		auto font = alx::Font( filename, size );
		auto sfont = alx::Font( filename, size + 20 );
		if( !font || !sfont )
		{
			std::cerr << "Could not load font" << std::endl;
		}
		SetFont( font, sfont );
	}

	void MenuItem::SetColor( const alx::Color& color )
	{
		_color = color;
	}

	bool MenuItem::IsSelected( )
	{
		return _isSelected;
	}

	void MenuItem::Draw( )
	{
		if( _hasImage )
		{
			_image.draw( );
			if( _isSelected )
			{
				unsigned char r, g, b;
				al_unmap_rgb( _color, &r, &g, &b );
				alx::Color col( r - 10, g - 10, b - 10 );
				al_draw_rectangle( _rect.getLeft( ), _rect.getTop( ), _rect.getRight( ), _rect.getBottom( ), col, 2 );
			}
		}
		else
		{
			if( _isSelected )
			{
				alx::Rect<double> trect = _rect;
				trect.setSize( _selectedFont.getWidth( _text.c_str( ) ) + 20, _selectedFont.getHeight( ) + 10 );
				trect.setCenter( _rect.getCenter( ) );
				_selectedFont.draw( trect.getCenterX( ), trect.getCenterY( ) - _selectedFont.getHeight( ) / 2, ALLEGRO_ALIGN_CENTRE, _color, _text.c_str( ) );
				unsigned char r, g, b;
				al_unmap_rgb( _color, &r, &g, &b );
				alx::Color col( r - 10, g - 10, b - 10 );
				al_draw_rectangle( trect.getLeft( ), trect.getTop( ), trect.getRight( ), trect.getBottom( ), col, 2 );
			}
			else
			{
				_font.draw( _rect.getCenterX( ), _rect.getCenterY( ) - _font.getHeight( ) / 2, ALLEGRO_ALIGN_CENTRE, _color, _text.c_str( ) );
			}
		}

	}

	void MenuItem::Action( )
	{
		if( _action )
		{
			_action( );
		}
	}

	bool MenuItem::InsideBounds( double x, double y )
	{
		return _rect.intersects( x, y );
	}

	MenuItem::~MenuItem( )
	{}
}
