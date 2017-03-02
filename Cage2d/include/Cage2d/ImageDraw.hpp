// *** ADDED BY HEADER FIXUP ***
#include <istream>
// *** END ***
#ifndef IMAGEDRAW_HPP_INCLUDED
#define IMAGEDRAW_HPP_INCLUDED
#include <iostream>

#ifndef PI2
#define PI2 6.28318530718
#endif // PI2


/// draw_scaled_rotated_bitmap_region
/*
inline void draw_image ( ALLEGRO_BITMAP *image,	 double sx, double sy, double sw = 0, double sh = 0,
						 double dx = 0, double dy = 0, double angle = 0, double dw = 0, double dh = 0, int flags = 0 )
{
	double scale_w = 0.0;
	double scale_h = 0.0;
	if ( sw == dw )
	{
		scale_w = 1.0;
	}
	else
	{
		scale_w = dw / sw;
	}
	if ( sh == dh )
	{
		scale_h = 1.0;
	}
	else
	{
		scale_h = dh / sh;
	}
	if ( dw == 0 && dh == 0 )
	{
		scale_h = scale_w = 1.0;
		dw = sw;
		dh = sh;
	}
	if ( sw == 0 && sh == 0 )
	{
		al_draw_bitmap ( image, sx - sw / 2, sy - sh / 2, flags );
	}
	else

	{
		ALLEGRO_TRANSFORM old;
		al_copy_transform ( &old, al_get_current_transform() );

		ALLEGRO_TRANSFORM T;
		al_copy_transform(&T, &old);
		al_translate_transform ( &T, -sw / 2, -sh / 2 );
		al_scale_transform ( &T, scale_w, scale_h );
		al_rotate_transform ( &T, angle );
		al_translate_transform ( &T, dx + dw / 2, dy + dh / 2 );
		al_use_transform ( &T );

		al_draw_bitmap_region ( image, sx, sy, sw, sh, 0, 0, flags );
		//al_identity_transform(&T);
		//al_use_transform(&T);


		al_use_transform ( &old );
	}
}
*/

inline void draw_image( ALLEGRO_BITMAP *image, float sx, float sy, float sw = 0, float sh = 0,
						float dx = 0, float dy = 0, float angle = 0, float dw = 0, float dh = 0, int flags = 0 )
{
	float scale_w = 0.0;
	float scale_h = 0.0;
	if( sw == dw )
	{
		scale_w = 1.0;
	}
	else
	{
		scale_w = dw / sw;
	}
	if( sh == dh )
	{
		scale_h = 1.0;
	}
	else
	{
		scale_h = dh / sh;
	}
	if( dw == 0 && dh == 0 )
	{
		scale_h = scale_w = 1.0;
		dw = sw;
		dh = sh;
	}
	if( sw == 0 && sh == 0 )
	{
		al_draw_bitmap( image, sx - sw / 2, sy - sh / 2, flags );
	}
	else if( angle == 0 )
	{
		al_draw_scaled_bitmap( image, sx, sy, sw, sh, dx, dy, dw, dh, flags );
	}
	else

	{
		ALLEGRO_BITMAP *region = al_create_sub_bitmap( image, int( sx ), int( sy ), int( sw ), int( sh ) );
		al_draw_scaled_rotated_bitmap( region, sw / 2, sh / 2, dx + dw / 2, dy + dh / 2, scale_w, scale_h, angle, flags );
		al_destroy_bitmap( region );
	}
}

inline void draw_special_circle( float cx, float cy, float r,
								 ALLEGRO_COLOR outer_color, ALLEGRO_COLOR center_color )
{
	ALLEGRO_VERTEX vertex_cache[ALLEGRO_VERTEX_CACHE_SIZE];
	int num_segments, ii;
	assert( r >= 0 );

	num_segments = int( ALLEGRO_PRIM_QUALITY * sqrtf( r ) );

	if( num_segments < 2 )
		return;

	if( num_segments >= ALLEGRO_VERTEX_CACHE_SIZE )
	{
		num_segments = ALLEGRO_VERTEX_CACHE_SIZE - 1;

	}

	al_calculate_arc( &( vertex_cache[1].x ), sizeof( ALLEGRO_VERTEX ), cx, cy, r, r, 0, static_cast<float>( PI2 ), 0, num_segments );
	vertex_cache[0].x = cx;
	vertex_cache[0].y = cy;

	for( ii = 0; ii < num_segments + 1; ii++ )
	{
		vertex_cache[ii].color = outer_color;
		vertex_cache[ii].z = 0;
	}

	vertex_cache[0].color = center_color;

	al_draw_prim( vertex_cache, 0, 0, 0, num_segments + 1, ALLEGRO_PRIM_TRIANGLE_FAN );
}

inline void draw_special_rough_circle( float cx, float cy, float r,
									   ALLEGRO_COLOR outer_color, ALLEGRO_COLOR center_color )
{
	ALLEGRO_VERTEX vertex_cache[ALLEGRO_VERTEX_CACHE_SIZE];
	int num_segments, ii;
	assert( r >= 0 );

	num_segments = 128;

	al_calculate_arc( &( vertex_cache[1].x ), sizeof( ALLEGRO_VERTEX ), cx, cy, r, r, 0, static_cast<float>( PI2 ), 0, num_segments );
	vertex_cache[0].x = cx;
	vertex_cache[0].y = cy;

	for( ii = 0; ii < num_segments + 1; ii++ )
	{
		vertex_cache[ii].color = outer_color;
		vertex_cache[ii].z = 0;
	}

	vertex_cache[0].color = center_color;

	al_draw_prim( vertex_cache, 0, 0, 0, num_segments + 1, ALLEGRO_PRIM_TRIANGLE_FAN );
}

#endif // IMAGEDRAW_HPP_INCLUDED
