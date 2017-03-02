#pragma once
namespace Cage2d
{
	class MenuItem
	{
		alx::Rect<double> _rect;
		std::function<void( )> _action;
		sp::Sprite _image;
		alx::Font _font;
		alx::Font _selectedFont;
		std::string _text;
		bool _hasImage;
		alx::Color _color;
		bool _isSelected;

		friend class Menu;
	public:
		MenuItem( );
		MenuItem( std::string text );

		void SetAction( const std::function<void( )>& action );
		void SetImage( const sp::Sprite& sprite );
		void SetFont( const alx::Font& font, const alx::Font& sfont );
		void SetFont( const std::string& filename, int size = 34 );
		void SetColor( const alx::Color& color );
		void SetSize( double width, double height );

		bool IsSelected( );
		void Draw( );
		void Action( );

		~MenuItem( );
	private:
		bool InsideBounds( double x, double y );
		void ChangeSelect( bool select );
		void SetPosition( double x, double y );
		void SetBounds( const alx::Rect<double>& rect );
		void SetBounds( double left, double top, double right, double bottom );
		void SetBounds( const alx::Point<double>& topLeft, const alx::Size<double>& size );
	};
}
