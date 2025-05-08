// #include "texture.hpp"
// #include <kmalloc.h>
// #include <string.h>


// kvideo::Texture_r8u::Texture_r8u( BMP_header *header )
// {
//     auto &info = header->infoheader;
//     uint8_t *data = (uint8_t*)header + header->offset;

//     this->W  = info.width;
//     this->H  = info.height;
//     this->nbytes = W*H * sizeof(uint8_t);
//     pixels = (uint8_t*)kmalloc(nbytes);

//     for (int i=0; i<W*H; i++)
//     {
//         uint16_t r = data[4*i+0];
//         uint16_t g = data[4*i+1];
//         uint16_t b = data[4*i+2];
//         pixels[i] = uint8_t((r+g+b) / 3);
//     }

//     for (int i=0; i<H/2; i++)
//     {
//         for (int j=0; j<W; j++)
//         {
//             auto temp = pixels[W*(H-1-i) + j];
//             pixels[W*(H-1-i) + j] = pixels[W*i + j];
//             pixels[W*i + j] = temp;
//         }
//     }
// }



// kvideo::Texture_rgb8u::Texture_rgb8u( BMP_header *header )
// {
//     auto &info = header->infoheader;
//     uint8_t *data = (uint8_t*)header + header->offset;

//     this->W  = info.width;
//     this->H  = info.height;
//     this->nbytes = W*H * sizeof(u8vec3);
//     pixels = (u8vec3*)kmalloc(nbytes);

//     for (int i=0; i<W*H; i++)
//     {
//         uint8_t r = data[4*i+0];
//         uint8_t g = data[4*i+1];
//         uint8_t b = data[4*i+2];
//         pixels[i] = u8vec3(r, g, b);
//     }

//     for (int i=0; i<H/2; i++)
//     {
//         for (int j=0; j<W; j++)
//         {
//             auto temp = pixels[W*(H-1-i) + j];
//             pixels[W*(H-1-i) + j] = pixels[W*i + j];
//             pixels[W*i + j] = temp;
//         }
//     }
// }



// kvideo::Texture_rgba8u::Texture_rgba8u( BMP_header *header )
// {
//     auto &info = header->infoheader;
//     uint8_t *data = (uint8_t*)header + header->offset;

//     this->W  = info.width;
//     this->H  = info.height;
//     this->nbytes = W*H * sizeof(uint32_t);
//     pixels = (u8vec4*)kmalloc(nbytes);

//     for (int i=0; i<W*H; i++)
//     {
//         uint8_t r = data[4*i+0];
//         uint8_t g = data[4*i+1];
//         uint8_t b = data[4*i+2];
//         uint8_t a = data[4*i+3];
//         pixels[i] = u8vec4(r, g, b, a);
//     }

//     for (int i=0; i<H/2; i++)
//     {
//         for (int j=0; j<W; j++)
//         {
//             auto temp = pixels[W*(H-1-i) + j];
//             pixels[W*(H-1-i) + j] = pixels[W*i + j];
//             pixels[W*i + j] = temp;
//         }
//     }
// }


// kvideo::Texture_r32f::Texture_r32f( BMP_header *header )
// {
//     auto &info = header->infoheader;
//     uint8_t *data = (uint8_t*)header + header->offset;

//     this->W  = info.width;
//     this->H  = info.height;
//     this->nbytes = W*H * sizeof(float);
//     pixels = (float*)kmalloc(nbytes);

//     for (int i=0; i<W*H; i++)
//     {
//         float r = float(data[4*i+0]) / 255.0f;
//         float g = float(data[4*i+1]) / 255.0f;
//         float b = float(data[4*i+2]) / 255.0f;
//         pixels[i] = (r+g+b) / 3.0f;
//     }

//     for (int i=0; i<H/2; i++)
//     {
//         for (int j=0; j<W; j++)
//         {
//             auto temp = pixels[W*(H-1-i) + j];
//             pixels[W*(H-1-i) + j] = pixels[W*i + j];
//             pixels[W*i + j] = temp;
//         }
//     }
// }


// kvideo::Texture_rgb32f::Texture_rgb32f( BMP_header *header )
// {
//     auto &info = header->infoheader;
//     uint8_t *data = (uint8_t*)header + header->offset;

//     this->W  = info.width;
//     this->H  = info.height;
//     this->nbytes = W*H * sizeof(vec3);
//     pixels = (vec3*)kmalloc(nbytes);

//     for (int i=0; i<W*H; i++)
//     {
//         float r = float(data[4*i+0]) / 255.0f;
//         float g = float(data[4*i+1]) / 255.0f;
//         float b = float(data[4*i+2]) / 255.0f;
//         pixels[i] = vec3(r, g, b);
//     }

//     for (int i=0; i<H/2; i++)
//     {
//         for (int j=0; j<W; j++)
//         {
//             auto temp = pixels[W*(H-1-i) + j];
//             pixels[W*(H-1-i) + j] = pixels[W*i + j];
//             pixels[W*i + j] = temp;
//         }
//     }
// }


// kvideo::Texture_rgba32f::Texture_rgba32f( BMP_header *header )
// {
//     auto &info = header->infoheader;
//     uint8_t *data = (uint8_t*)header + header->offset;

//     this->W  = info.width;
//     this->H  = info.height;
//     this->nbytes = W*H * sizeof(vec4);
//     pixels = (vec4*)kmalloc(nbytes);

//     for (int i=0; i<W*H; i++)
//     {
//         float r = float(data[4*i+0]) / 255.0f;
//         float g = float(data[4*i+1]) / 255.0f;
//         float b = float(data[4*i+2]) / 255.0f;
//         float a = float(data[4*i+3]) / 255.0f;
//         pixels[i] = vec4(r, g, b, a);
//     }

//     for (int i=0; i<H/2; i++)
//     {
//         for (int j=0; j<W; j++)
//         {
//             auto temp = pixels[W*(H-1-i) + j];
//             pixels[W*(H-1-i) + j] = pixels[W*i + j];
//             pixels[W*i + j] = temp;
//         }
//     }
// }

