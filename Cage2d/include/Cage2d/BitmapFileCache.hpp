#pragma once
class BitmapFileCache
{
	BitmapFileCache( );
public:
	INSTANCE( BitmapFileCache )
		alx::Bitmap LoadBitmap( const std::string& filename );
	void ClearCache( );
	~BitmapFileCache( );
private:
	std::map<std::string, alx::Bitmap> _bitmapChache;
};


class BitmapLoadException : public std::runtime_error
{
public:
	explicit BitmapLoadException( const std::string& msg ) : std::runtime_error( msg ) {}
};

class InvalidBitmapException : public std::runtime_error
{
public:
	explicit InvalidBitmapException( const std::string& msg ) : std::runtime_error( msg ) {}
};
