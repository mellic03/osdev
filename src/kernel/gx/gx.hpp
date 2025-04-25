#pragma once
#include <kmath/vec.hpp>
#include "command.hpp"
#include "gxContext.hpp"
#include "gxPrimitive.hpp"

void gx_main();
void gx_rasterize( const gxTrianglePrimitive &P );

void  gxEnable( gxEnum );
void  gxDisable( gxEnum );
void  gxViewport( int x, int y, int w, int h );
void  gxFlush();
float gxDepthSample( int u, int v );

uint32_t gxCreateBuffer( size_t nbytes );
void     gxDeleteBuffer( uint32_t );
void    *gxGetBuffer( uint32_t );

void gxTintColor( float r, float g, float b, float a );
void gxClearColor( float r, float g, float b, float a );
void gxClearDepth( float d );
void gxClear( uint32_t mask );
void gxClearTexture( uint32_t );

void gxBlitTexture( uint32_t dst, uint32_t src, const gxRect &dstrect, const gxRect &srcrect );
void gxBlitTexture( uint32_t src, const gxRect &dstrect );
void gxBlitTexture( uint32_t src, int x, int y );
void gxBlitTexture( uint32_t src, const gxRect &dstrect, const gxRect &srcrect );
void gxBlitTexture( uint32_t dst, uint32_t src, const gxRect &dstrect );

void gxDrawRect( uint32_t dst, const gxRect &rect, bool fill, int stroke,
                 const vec4 &fillColor, const vec4 &borderColor = vec4(0.0f) );

void gxDrawRect( const gxRect &rect, bool fill, int stroke,
                 const vec4 &fillColor, const vec4 &borderColor = vec4(0.0f) );
