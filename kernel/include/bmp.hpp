#pragma once

#include <stddef.h>
#include <stdint.h>
#include <algorithm>

/*
    http://www.daubnet.com/en/file-format-bmp
*/

enum BI_: uint32_t
{
    BI_RGB       = 0,  /* No compression - straight BGR data */
    BI_RLE8      = 1,  /* 8-bit run-length compression */
    BI_RLE4      = 2,  /* 4-bit run-length compression */
    BI_BITFIELDS = 3,  /* RGB bitmap with RGB masks */
};

static inline
const char *BI_string( uint32_t compression )
{
    switch (compression)
    {
        default:            return "BI_INVALID";
        case BI_RGB:        return "BI_RGB";
        case BI_RLE8:       return "BI_RLE8";
        case BI_RLE4:       return "BI_RLE4";
        case BI_BITFIELDS:  return "BI_BITFIELDS";
    }
}


struct BMP_infoheader
{
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bpp;           // bits per pixel
    uint32_t compression;   // 0, 1, 2, 3 --> none, 8bit run length, 4bit run length, mask
    uint32_t imagesize;

    // Entire field can be set to 0 --> zero compression
    uint32_t ppm_x;             // pixels per meter.
    uint32_t ppm_y;             // pixels per meter.
    uint32_t colors_used;       // number of used colors.
    uint32_t colors_important;  // 0 == all
    uint8_t  color_table[4];    // present only if bpp <= 8

} __attribute__((packed));


struct BMP_header
{
    uint16_t signature;
    uint32_t filesize;
    uint32_t reserved;
    uint32_t offset; // offset to image data

    uint32_t infosize; // should be sizeof(infoheader)
    BMP_infoheader infoheader;

    uint8_t rest[8];

} __attribute__((packed));



struct BMP_File
{
    uint8_t *pixels;
    int w, h, bpp;
    size_t nbytes;

    BMP_File( void *addr )
    {
        auto *header = (BMP_header*)addr;
        auto &info   = header->infoheader;
    
        this->w      = int(info.width);
        this->h      = int(info.height);
        this->bpp    = int(info.bpp);
        this->nbytes = header->filesize;
        this->pixels = (uint8_t*)header + header->offset;

        // Flip vertically
        auto *buf = (uint8_t*)pixels;
        for (int i=0; i<h/2; i++)
        {
            for (int j=0; j<w; j++)
            {
                for (int k=0; k<4; k++)
                {
                    int idx0 = w*4*(h-1 - i) + 4*j + k;
                    int idx1 = w*4*i + 4*j + k;
                    std::swap(buf[idx0], buf[idx1]);
                }
            }
        }
    }

};


// void test()
// {
//     sizeof(BMP_infoheader);
// }



// unsigned char bmpData[] = // All values are little-endian
// {
//     0x42, 0x4D,             // Signature 'BM'
//     0xaa, 0x00, 0x00, 0x00, // Size: 170 bytes
//     0x00, 0x00,             // Unused
//     0x00, 0x00,             // Unused
//     0x8a, 0x00, 0x00, 0x00, // Offset to image data

//     0x7c, 0x00, 0x00, 0x00, // DIB header size (124 bytes)
//     0x04, 0x00, 0x00, 0x00, // Width (4px)
//     0x02, 0x00, 0x00, 0x00, // Height (2px)
//     0x01, 0x00,             // Planes (1)
//     0x20, 0x00,             // Bits per pixel (32)
//     0x03, 0x00, 0x00, 0x00, // Format (bitfield = use bitfields | no compression)
//     0x20, 0x00, 0x00, 0x00, // Image raw size (32 bytes)
//     0x13, 0x0B, 0x00, 0x00, // Horizontal print resolution (2835 = 72dpi * 39.3701)
//     0x13, 0x0B, 0x00, 0x00, // Vertical print resolution (2835 = 72dpi * 39.3701)
//     0x00, 0x00, 0x00, 0x00, // Colors in palette (none)
//     0x00, 0x00, 0x00, 0x00, // Important colors (0 = all)
//     0x00, 0x00, 0xFF, 0x00, // R bitmask (00FF0000)
//     0x00, 0xFF, 0x00, 0x00, // G bitmask (0000FF00)
//     0xFF, 0x00, 0x00, 0x00, // B bitmask (000000FF)
//     0x00, 0x00, 0x00, 0xFF, // A bitmask (FF000000)
//     0x42, 0x47, 0x52, 0x73, // sRGB color space
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Unused R, G, B entries for color space
//     0x00, 0x00, 0x00, 0x00, // Unused Gamma X entry for color space
//     0x00, 0x00, 0x00, 0x00, // Unused Gamma Y entry for color space
//     0x00, 0x00, 0x00, 0x00, // Unused Gamma Z entry for color space

//     0x00, 0x00, 0x00, 0x00, // Unknown
//     0x00, 0x00, 0x00, 0x00, // Unknown
//     0x00, 0x00, 0x00, 0x00, // Unknown
//     0x00, 0x00, 0x00, 0x00, // Unknown

//     // Image data:
//     0xFF, 0x00, 0x00, 0x7F, // Bottom left pixel
//     0x00, 0xFF, 0x00, 0x7F,
//     0x00, 0x00, 0xFF, 0x7F,
//     0xFF, 0xFF, 0xFF, 0x7F, // Bottom right pixel
//     0xFF, 0x00, 0x00, 0xFF, // Top left pixel
//     0x00, 0xFF, 0x00, 0xFF,
//     0x00, 0x00, 0xFF, 0xFF,
//     0xFF, 0xFF, 0xFF, 0xFF  // Top right pixel
// };
