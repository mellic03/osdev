#pragma once

#include <stddef.h>
#include <stdint.h>
#include <cringe/vec.hpp>


namespace knl
{
    class VideoInterface;
}


namespace kvideo2
{
    union ColorRGBA8
    {
        u8vec4   rgba;
        uint32_t dword;
    
        ColorRGBA8( const u8vec4 &v ): rgba(v) {  };
        ColorRGBA8( uint32_t d ): dword(d) {  };
        ColorRGBA8(): dword(0) {  };
    };


    extern knl::VideoInterface *iface;
    extern uint8_t *fbmem;
    extern uint32_t W;
    extern uint32_t H;
    extern uint32_t BPP;
    extern uint32_t BYPP;
    extern uint32_t Pitch;

    void init();
    void flush();
    void setMode( uint32_t w, uint32_t h, uint32_t bpp );
    void fill( ColorRGBA8 color );
}


namespace knl
{
    class VideoInterface
    {
    public:
        struct Mode
        {
            uint32_t w, h, bpp, pitch;
        };
    
        virtual bool available() = 0;
        virtual void init() = 0;
        virtual void flush( uint32_t x, uint32_t y, uint32_t w, uint32_t h ) = 0;
        virtual void setMode( uint32_t w, uint32_t h, uint32_t bpp ) = 0;
        virtual uint8_t *getFb() = 0;
        virtual void drawRect( int x, int y, int w, int h, kvideo2::ColorRGBA8 ) = 0;

        const Mode &getMode()  { return m_mode; };
        constexpr uint32_t W()     const { return m_mode.w; };
        constexpr uint32_t H()     const { return m_mode.h; };
        constexpr uint32_t BPP()   const { return m_mode.bpp; };
        constexpr uint32_t BYPP()  const { return BPP() / 8; };
        constexpr uint32_t Pitch() const { return m_mode.pitch; };

    protected:
        Mode m_mode;
    };

}



// Bit(s)	Description
// 0	Separate Sync
// 1	Composite Sync
// 2	Sync on green
// 3-4	Unused?
// 5-6	Voltage Level
// 7	Digital Signal

struct VideoInputType
{
    uint8_t separateSync    : 1;
    uint8_t compositeSync   : 1;
    uint8_t syncOnGreen     : 1;
    uint8_t unused          : 2;
    uint8_t voltageLevel    : 2;
    uint8_t digitalSignal   : 1;
};


struct VideoDisplayType
{
    uint8_t unused        : 1; // Unused?
    uint8_t syncLocation  : 1; // Sync Location (OR Horizontal Sync Polarity IF 'Sync Type' == 11)
    uint8_t serrate       : 1; // Serrate (OR Vertical Sync Polarity IF 'Sync Type' == 11)
    uint8_t syncType      : 2; // Sync Type (00 - Analog Composite, 01 - Bipolar Analog Composite, 10 - Digital Composite, 11 - Digital Separate)
    uint8_t stereoMode    : 2; // Stereo Mode (00 - No Stereo, 01 - Right Stereo Sync High, 10 - Left Stereo Sync High)
    uint8_t interlaced    : 1; // Interlaced
};


struct VideoTimingDesc
{
    uint8_t hFreq;              // Horizontal Frequency (kHz)
    uint8_t vFreq;              // Vertical Frequency (Hz)
    uint8_t hActiveTime;        // Horizontal Active Time
    uint8_t hBlankTime;         // Horizontal Blanking Time
    uint8_t hActiveBlankTime;   // Horizontal Active Time / Horizontal Blanking Time
    uint8_t vActiveTime;        // Vertical Active time
    uint8_t vBlankTime;         // Vertical Blanking Time
    uint8_t vActiveBlankTime;   // Vertical Active Time / Vertical Blanking Time
    uint8_t hSyncOffset;        // Horizontal Sync Offset
    uint8_t hSyncPulseW;        // Horizontal Sync Pulse-width
    uint8_t vSyncOffsetPulseW;  // Vertical Sync Offset / Vertical Sync Pulse-width
    uint8_t vhSyncOffsetPulseW; // Vertical/Horizontal Sync Offset / Pulse-width
    uint8_t hImageSize;         // Horizontal Image Size (mm)
    uint8_t vImageSize;         // Vertical Image Size (mm)
    uint8_t hvRatio;            // Horizontal Image Size / Vertical Image Size
    uint8_t hBorder;            // Horizontal Border
    uint8_t vBorder;            // Vertical Border
    VideoDisplayType displayType;   // Type Of Display

};



struct VideoEDID
{
    // 00h	8 Bytes	Padding
    // 08h	1 Word	Manufacture ID (Big-Endian)
    // 0Ah	1 Word	EDID ID code
    // 0Ch	1 DWord	Serial Number
    // 10h	1 Byte	Manufacture Week
    // 11h	1 Byte	Manufacture Year
    // 12h	1 Byte	EDID Version
    // 13h	1 Byte	EDID Revision
    // 14h	1 Byte	Video Input Type
    // 15h	1 Byte	Max Horizontal Size (cm)
    // 16h	1 Byte	Max Vertical Size (cm)
    // 17h	1 Byte	Gama Factor
    // 18h	1 Byte	DPMS Flags
    // 19h	10 Bytes	Chroma Information
    // 23h	1 Byte	Established Timings 1
    // 24h	1 Byte	Established Timings 2
    // 25h	1 Byte	Manufacture's Reserved Timings
    // 26h	8 Words	Standard Timing Identification
    // 36h	18 Bytes	Detailed Timing Description 1
    // 48h	18 Bytes	Detailed Timing Description 2
    // 5Ah	18 Bytes	Detailed Timing Description 3
    // 6Ch	18 Bytes	Detailed Timing Description 4
    // 7Eh	1 Byte	Unused
    // 7Fh	1 Byte	Checksum (Low Byte of 16-bit sum of 00-7Eh)
    uint8_t         padding[8];
    uint16_t        manuacturerID;
    uint16_t        edidID;
    uint32_t        serialNo;
    uint8_t         manufactureWeek;
    uint8_t         manufactureYear;
    uint8_t         edidVersion;
    uint8_t         edidRevision;
    VideoInputType  videoInputType;
    uint8_t         hSizeMax;
    uint8_t         vSizeMax;
    uint8_t         gammaFactor;
    uint8_t         dpmsFlags;
    uint8_t         chromaInfo[10];
    uint8_t         establishedTimings1;
    uint8_t         establishedTimings2;
    uint8_t         manufacturerReservedTimings;
    uint16_t        stdTimingID[8];
    VideoTimingDesc detailedTimingDesc[4];
    uint8_t         unused;
    uint8_t         checksum;
};


// void resers()
// {
//     0x14;
//     constexpr uint64_t A = offsetof(VideoEDID, videoInputType);
//     0x36;
//     constexpr uint64_t B = offsetof(VideoEDID, timingDesc1);
// }

