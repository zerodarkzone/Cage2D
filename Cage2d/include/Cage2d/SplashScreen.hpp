#pragma once
namespace Cage2d
{
	class SplashScreen
	{
		alx::Bitmap _image;
	public:
		SplashScreen( );
		void Show( alx::Display& renderWindow );
	};
}
