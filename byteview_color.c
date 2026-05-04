#include "byteview_color.h"
#include "rl_utils.h"


static unsigned char Read8Bits(const unsigned char* fileData, unsigned int fileSize, unsigned int byteIndex);
static unsigned short Read16Bits(const unsigned char* fileData, unsigned int fileSize, unsigned int byteIndex);
static unsigned int Read24Bits(const unsigned char* fileData, unsigned int fileSize, unsigned int byteIndex);
static unsigned int Read32Bits(const unsigned char* fileData, unsigned int fileSize, unsigned int byteIndex);
static unsigned char Expand3To8(unsigned char value);
static unsigned char Expand4To8(unsigned char value);
static unsigned char Expand5To8(unsigned char value);
static unsigned char Expand6To8(unsigned char value);
static unsigned char Scale16To8(unsigned short value);
static unsigned char Read16BitColorComponent(const unsigned char* fileData, unsigned int fileSize, unsigned int byteIndex);
static Color GetRgb332Color(unsigned char pixel);
static Color GetRgba2222Color(unsigned char pixel);
static Color GetRgb565Color(unsigned short pixel);
static Color GetBgr565Color(unsigned short pixel);
static Color GetXrgb1555Color(unsigned short pixel);
static Color GetArgb1555Color(unsigned short pixel);
static Color GetRgbx5551Color(unsigned short pixel);
static Color GetRgba5551Color(unsigned short pixel);
static Color GetRgbx4444Color(unsigned short pixel);
static Color GetXrgb4444Color(unsigned short pixel);
static Color GetRgba4444Color(unsigned short pixel);
static Color GetArgb4444Color(unsigned short pixel);
static Color GetVUY888Color(unsigned int pixel);
static Color GetXRGB2101010Color(unsigned int pixel);
static Color GetXBGR2101010Color(unsigned int pixel);
static Color GetRGBX1010102Color(unsigned int pixel);
static Color GetBGRX1010102Color(unsigned int pixel);
static Color GetARGB2101010Color(unsigned int pixel);
static Color GetABGR2101010Color(unsigned int pixel);
static Color GetRGBA1010102Color(unsigned int pixel);
static Color GetBGRA1010102Color(unsigned int pixel);
static Color GetClassColor(unsigned char pixel);

Color GetByteViewColor(const unsigned char* fileData, unsigned int fileSize, unsigned int byteIndex, ByteViewColorType colorType) {
	unsigned char pixel8;
	unsigned short pixel16;
	unsigned int pixel24;
	unsigned int pixel32;
	Color color;

	pixel8 = 0;
	pixel16 = 0;
	pixel32 = 0;
	color = BLACK;

	switch (colorType) {
		case BYTEVIEW_COLOR_TYPE_R8:
			color.r = Read8Bits(fileData, fileSize, byteIndex);
			break;
		case BYTEVIEW_COLOR_TYPE_G8:
			color.g = Read8Bits(fileData, fileSize, byteIndex);
			break;
		case BYTEVIEW_COLOR_TYPE_B8:
			color.b = Read8Bits(fileData, fileSize, byteIndex);
			break;
		case BYTEVIEW_COLOR_TYPE_A8:
			color.a = Read8Bits(fileData, fileSize, byteIndex);
			break;
		case BYTEVIEW_COLOR_TYPE_Y8:
			pixel8 = Read8Bits(fileData, fileSize, byteIndex);
			color.r = pixel8;
			color.g = pixel8;
			color.b = pixel8;
			color.a = 255U;
			break;
		case BYTEVIEW_COLOR_TYPE_RG88:
			color.r = Read8Bits(fileData, fileSize, byteIndex);
			color.g = Read8Bits(fileData, fileSize, byteIndex + 1U);
			break;
		case BYTEVIEW_COLOR_TYPE_GR88:
			color.g = Read8Bits(fileData, fileSize, byteIndex);
			color.r = Read8Bits(fileData, fileSize, byteIndex + 1U);
			break;
		case BYTEVIEW_COLOR_TYPE_Y10:
			pixel16 = Read16Bits(fileData, fileSize, byteIndex);
			pixel16 = pixel16 & 0x03FF;
			pixel16 = (pixel16 * 255) / 1023;
			color.r = (unsigned char)(pixel16);
			color.g = (unsigned char)(pixel16);
			color.b = (unsigned char)(pixel16);
			color.a = 255U;
			break;
		case BYTEVIEW_COLOR_TYPE_Y12:
			pixel16 = Read16Bits(fileData, fileSize, byteIndex);
			pixel16 = pixel16 & 0x0FFF;
			pixel16 = (pixel16 * 255) / 4095;
			color.r = (unsigned char)(pixel16);
			color.g = (unsigned char)(pixel16);
			color.b = (unsigned char)(pixel16);
			color.a = 255U;
			break;
		case BYTEVIEW_COLOR_TYPE_RGB332:
			pixel8 = Read8Bits(fileData, fileSize, byteIndex);
			color = GetRgb332Color(pixel8);
			break;
		case BYTEVIEW_COLOR_TYPE_RGBA2222:
			pixel8 = Read8Bits(fileData, fileSize, byteIndex);
			color = GetRgba2222Color(pixel8);
			break;
		case BYTEVIEW_COLOR_TYPE_RGB565:
			pixel16 = Read16Bits(fileData, fileSize, byteIndex);
			color = GetRgb565Color(pixel16);
			break;
		case BYTEVIEW_COLOR_TYPE_BGR565:
			pixel16 = Read16Bits(fileData, fileSize, byteIndex);
			color = GetBgr565Color(pixel16);
			break;
		case BYTEVIEW_COLOR_TYPE_XRGB1555:
			pixel16 = Read16Bits(fileData, fileSize, byteIndex);
			color = GetXrgb1555Color(pixel16);
			break;
		case BYTEVIEW_COLOR_TYPE_ARGB1555:
			pixel16 = Read16Bits(fileData, fileSize, byteIndex);
			color = GetArgb1555Color(pixel16);
			break;
		case BYTEVIEW_COLOR_TYPE_RGBX5551:
			pixel16 = Read16Bits(fileData, fileSize, byteIndex);
			color = GetRgbx5551Color(pixel16);
			break;
		case BYTEVIEW_COLOR_TYPE_RGBA5551:
			pixel16 = Read16Bits(fileData, fileSize, byteIndex);
			color = GetRgba5551Color(pixel16);
			break;
		case BYTEVIEW_COLOR_TYPE_RGBX4444:
			pixel16 = Read16Bits(fileData, fileSize, byteIndex);
			color = GetRgbx4444Color(pixel16);
			break;
		case BYTEVIEW_COLOR_TYPE_XRGB4444:
			pixel16 = Read16Bits(fileData, fileSize, byteIndex);
			color = GetXrgb4444Color(pixel16);
			break;
		case BYTEVIEW_COLOR_TYPE_RGBA4444:
			pixel16 = Read16Bits(fileData, fileSize, byteIndex);
			color = GetRgba4444Color(pixel16);
			break;
		case BYTEVIEW_COLOR_TYPE_ARGB4444:
			pixel16 = Read16Bits(fileData, fileSize, byteIndex);
			color = GetArgb4444Color(pixel16);
			break;
		case BYTEVIEW_COLOR_TYPE_RGB888:
			color.r = Read8Bits(fileData, fileSize, byteIndex);
			color.g = Read8Bits(fileData, fileSize, byteIndex + 1U);
			color.b = Read8Bits(fileData, fileSize, byteIndex + 2U);
			break;
		case BYTEVIEW_COLOR_TYPE_BGR888:
			color.b = Read8Bits(fileData, fileSize, byteIndex);
			color.g = Read8Bits(fileData, fileSize, byteIndex + 1U);
			color.r = Read8Bits(fileData, fileSize, byteIndex + 2U);
			break;
		case BYTEVIEW_COLOR_TYPE_VUY888:
			pixel24 = Read24Bits(fileData, fileSize, byteIndex);
			color = GetVUY888Color(pixel24);
			break;
		case BYTEVIEW_COLOR_TYPE_RG1616:
			color.r = Read16BitColorComponent(fileData, fileSize, byteIndex);
			color.g = Read16BitColorComponent(fileData, fileSize, byteIndex + 2U);
			break;
		case BYTEVIEW_COLOR_TYPE_XRGB8888:
			color.r = Read8Bits(fileData, fileSize, byteIndex + 1U);
			color.g = Read8Bits(fileData, fileSize, byteIndex + 2U);
			color.b = Read8Bits(fileData, fileSize, byteIndex + 3U);
			break;
		case BYTEVIEW_COLOR_TYPE_XBGR8888:
			color.b = Read8Bits(fileData, fileSize, byteIndex + 1U);
			color.g = Read8Bits(fileData, fileSize, byteIndex + 2U);
			color.r = Read8Bits(fileData, fileSize, byteIndex + 3U);
			break;
		case BYTEVIEW_COLOR_TYPE_RGBX8888:
			color.r = Read8Bits(fileData, fileSize, byteIndex);
			color.g = Read8Bits(fileData, fileSize, byteIndex + 1U);
			color.b = Read8Bits(fileData, fileSize, byteIndex + 2U);
			break;
		case BYTEVIEW_COLOR_TYPE_BGRX8888:
			color.b = Read8Bits(fileData, fileSize, byteIndex);
			color.g = Read8Bits(fileData, fileSize, byteIndex + 1U);
			color.r = Read8Bits(fileData, fileSize, byteIndex + 2U);
			break;
		case BYTEVIEW_COLOR_TYPE_ARGB8888:
			color.a = Read8Bits(fileData, fileSize, byteIndex);
			color.r = Read8Bits(fileData, fileSize, byteIndex + 1U);
			color.g = Read8Bits(fileData, fileSize, byteIndex + 2U);
			color.b = Read8Bits(fileData, fileSize, byteIndex + 3U);
			break;
		case BYTEVIEW_COLOR_TYPE_ABGR8888:
			color.a = Read8Bits(fileData, fileSize, byteIndex);
			color.b = Read8Bits(fileData, fileSize, byteIndex + 1U);
			color.g = Read8Bits(fileData, fileSize, byteIndex + 2U);
			color.r = Read8Bits(fileData, fileSize, byteIndex + 3U);
			break;
		case BYTEVIEW_COLOR_TYPE_RGBA8888:
			color.r = Read8Bits(fileData, fileSize, byteIndex);
			color.g = Read8Bits(fileData, fileSize, byteIndex + 1U);
			color.b = Read8Bits(fileData, fileSize, byteIndex + 2U);
			color.a = Read8Bits(fileData, fileSize, byteIndex + 3U);
			break;
		case BYTEVIEW_COLOR_TYPE_BGRA8888:
			color.b = Read8Bits(fileData, fileSize, byteIndex);
			color.g = Read8Bits(fileData, fileSize, byteIndex + 1U);
			color.r = Read8Bits(fileData, fileSize, byteIndex + 2U);
			color.a = Read8Bits(fileData, fileSize, byteIndex + 3U);
			break;
		case BYTEVIEW_COLOR_TYPE_XVUY8888:
			pixel32 = Read32Bits(fileData, fileSize, byteIndex);
			color = GetVUY888Color(pixel32);
			break;
		case BYTEVIEW_COLOR_TYPE_XRGB2101010:
			pixel32 = Read32Bits(fileData, fileSize, byteIndex);
			color = GetXRGB2101010Color(pixel32);
			break;
		case BYTEVIEW_COLOR_TYPE_XBGR2101010:
			pixel32 = Read32Bits(fileData, fileSize, byteIndex);
			color = GetXBGR2101010Color(pixel32);
			break;
		case BYTEVIEW_COLOR_TYPE_RGBX1010102:
			pixel32 = Read32Bits(fileData, fileSize, byteIndex);
			color = GetRGBX1010102Color(pixel32);
			break;
		case BYTEVIEW_COLOR_TYPE_BGRX1010102:
			pixel32 = Read32Bits(fileData, fileSize, byteIndex);
			color = GetBGRX1010102Color(pixel32);
			break;
		case BYTEVIEW_COLOR_TYPE_ARGB2101010:
			pixel32 = Read32Bits(fileData, fileSize, byteIndex);
			color = GetARGB2101010Color(pixel32);
			break;
		case BYTEVIEW_COLOR_TYPE_ABGR2101010:
			pixel32 = Read32Bits(fileData, fileSize, byteIndex);
			color = GetABGR2101010Color(pixel32);
			break;
		case BYTEVIEW_COLOR_TYPE_RGBA1010102:
			pixel32 = Read32Bits(fileData, fileSize, byteIndex);
			color = GetRGBA1010102Color(pixel32);
			break;
		case BYTEVIEW_COLOR_TYPE_BGRA1010102:
			pixel32 = Read32Bits(fileData, fileSize, byteIndex);
			color = GetBGRA1010102Color(pixel32);
			break;
		case BYTEVIEW_COLOR_TYPE_RGB161616:
			color.r = Read16BitColorComponent(fileData, fileSize, byteIndex);
			color.g = Read16BitColorComponent(fileData, fileSize, byteIndex + 2U);
			color.b = Read16BitColorComponent(fileData, fileSize, byteIndex + 4U);
			break;
		case BYTEVIEW_COLOR_TYPE_BGR161616:
			color.b = Read16BitColorComponent(fileData, fileSize, byteIndex);
			color.g = Read16BitColorComponent(fileData, fileSize, byteIndex + 2U);
			color.r = Read16BitColorComponent(fileData, fileSize, byteIndex + 4U);
			break;
		case BYTEVIEW_COLOR_TYPE_RGBA16161616:
			color.r = Read16BitColorComponent(fileData, fileSize, byteIndex);
			color.g = Read16BitColorComponent(fileData, fileSize, byteIndex + 2U);
			color.b = Read16BitColorComponent(fileData, fileSize, byteIndex + 4U);
			color.a = Read16BitColorComponent(fileData, fileSize, byteIndex + 6U);
			break;
		case BYTEVIEW_COLOR_TYPE_BGRA16161616:
			color.b = Read16BitColorComponent(fileData, fileSize, byteIndex);
			color.g = Read16BitColorComponent(fileData, fileSize, byteIndex + 2U);
			color.r = Read16BitColorComponent(fileData, fileSize, byteIndex + 4U);
			color.a = Read16BitColorComponent(fileData, fileSize, byteIndex + 6U);
			break;
		case BYTEVIEW_COLOR_TYPE_ARGB16161616:
			color.a = Read16BitColorComponent(fileData, fileSize, byteIndex);
			color.r = Read16BitColorComponent(fileData, fileSize, byteIndex + 2U);
			color.g = Read16BitColorComponent(fileData, fileSize, byteIndex + 4U);
			color.b = Read16BitColorComponent(fileData, fileSize, byteIndex + 6U);
			break;
		case BYTEVIEW_COLOR_TYPE_ABGR16161616:
			color.a = Read16BitColorComponent(fileData, fileSize, byteIndex);
			color.b = Read16BitColorComponent(fileData, fileSize, byteIndex + 2U);
			color.g = Read16BitColorComponent(fileData, fileSize, byteIndex + 4U);
			color.r = Read16BitColorComponent(fileData, fileSize, byteIndex + 6U);
			break;
		case BYTEVIEW_COLOR_TYPE_GRADIENT:
			pixel8 = Read8Bits(fileData, fileSize, byteIndex);
			color.r = pixel8;
			color.g = pixel8;
			color.b = pixel8;
			color.a = pixel8;
			break;
		case BYTEVIEW_COLOR_TYPE_CLASS:
			pixel8 = Read8Bits(fileData, fileSize, byteIndex);
			color = GetClassColor(pixel8);
			break;
		case BYTEVIEW_COLOR_TYPE_NONE:
		case BYTEVIEW_COLOR_TYPE_COUNT:
		default: break;
	}

	return color;
}

const char* GetByteViewColorTypeName(ByteViewColorType byteViewColorType) {
	switch (byteViewColorType) {
		case BYTEVIEW_COLOR_TYPE_NONE:
			return "BYTEVIEW_COLOR_TYPE_NONE";
		case BYTEVIEW_COLOR_TYPE_R8:
			return "BYTEVIEW_COLOR_TYPE_R8";
		case BYTEVIEW_COLOR_TYPE_G8:
			return "BYTEVIEW_COLOR_TYPE_G8";
		case BYTEVIEW_COLOR_TYPE_B8:
			return "BYTEVIEW_COLOR_TYPE_B8";
		case BYTEVIEW_COLOR_TYPE_A8:
			return "BYTEVIEW_COLOR_TYPE_A8";
		case BYTEVIEW_COLOR_TYPE_Y8:
			return "BYTEVIEW_COLOR_TYPE_Y8";
		case BYTEVIEW_COLOR_TYPE_RG88:
			return "BYTEVIEW_COLOR_TYPE_RG88";
		case BYTEVIEW_COLOR_TYPE_GR88:
			return "BYTEVIEW_COLOR_TYPE_GR88";
		case BYTEVIEW_COLOR_TYPE_Y10:
			return "BYTEVIEW_COLOR_TYPE_Y10";
		case BYTEVIEW_COLOR_TYPE_Y12:
			return "BYTEVIEW_COLOR_TYPE_Y12";
		case BYTEVIEW_COLOR_TYPE_RGB332:
			return "BYTEVIEW_COLOR_TYPE_RGB332";
		case BYTEVIEW_COLOR_TYPE_RGBA2222:
			return "BYTEVIEW_COLOR_TYPE_RGBA2222";
		case BYTEVIEW_COLOR_TYPE_RGB565:
			return "BYTEVIEW_COLOR_TYPE_RGB565";
		case BYTEVIEW_COLOR_TYPE_BGR565:
			return "BYTEVIEW_COLOR_TYPE_BGR565";
		case BYTEVIEW_COLOR_TYPE_XRGB1555:
			return "BYTEVIEW_COLOR_TYPE_XRGB1555";
		case BYTEVIEW_COLOR_TYPE_ARGB1555:
			return "BYTEVIEW_COLOR_TYPE_ARGB1555";
		case BYTEVIEW_COLOR_TYPE_RGBX5551:
			return "BYTEVIEW_COLOR_TYPE_RGBX5551";
		case BYTEVIEW_COLOR_TYPE_RGBA5551:
			return "BYTEVIEW_COLOR_TYPE_RGBA5551";
		case BYTEVIEW_COLOR_TYPE_RGBX4444:
			return "BYTEVIEW_COLOR_TYPE_RGBX4444";
		case BYTEVIEW_COLOR_TYPE_XRGB4444:
			return "BYTEVIEW_COLOR_TYPE_XRGB4444";
		case BYTEVIEW_COLOR_TYPE_RGBA4444:
			return "BYTEVIEW_COLOR_TYPE_RGBA4444";
		case BYTEVIEW_COLOR_TYPE_ARGB4444:
			return "BYTEVIEW_COLOR_TYPE_ARGB4444";
		case BYTEVIEW_COLOR_TYPE_RGB888:
			return "BYTEVIEW_COLOR_TYPE_RGB888";
		case BYTEVIEW_COLOR_TYPE_BGR888:
			return "BYTEVIEW_COLOR_TYPE_BGR888";
		case BYTEVIEW_COLOR_TYPE_VUY888:
			return "BYTEVIEW_COLOR_TYPE_VUY888";
		case BYTEVIEW_COLOR_TYPE_RG1616:
			return "BYTEVIEW_COLOR_TYPE_RG1616";
		case BYTEVIEW_COLOR_TYPE_XRGB8888:
			return "BYTEVIEW_COLOR_TYPE_XRGB8888";
		case BYTEVIEW_COLOR_TYPE_XBGR8888:
			return "BYTEVIEW_COLOR_TYPE_XBGR8888";
		case BYTEVIEW_COLOR_TYPE_RGBX8888:
			return "BYTEVIEW_COLOR_TYPE_RGBX8888";
		case BYTEVIEW_COLOR_TYPE_BGRX8888:
			return "BYTEVIEW_COLOR_TYPE_BGRX8888";
		case BYTEVIEW_COLOR_TYPE_ARGB8888:
			return "BYTEVIEW_COLOR_TYPE_ARGB8888";
		case BYTEVIEW_COLOR_TYPE_ABGR8888:
			return "BYTEVIEW_COLOR_TYPE_ABGR8888";
		case BYTEVIEW_COLOR_TYPE_RGBA8888:
			return "BYTEVIEW_COLOR_TYPE_RGBA8888";
		case BYTEVIEW_COLOR_TYPE_BGRA8888:
			return "BYTEVIEW_COLOR_TYPE_BGRA8888";
		case BYTEVIEW_COLOR_TYPE_XVUY8888:
			return "BYTEVIEW_COLOR_TYPE_XVUY8888";
		case BYTEVIEW_COLOR_TYPE_XRGB2101010:
			return "BYTEVIEW_COLOR_TYPE_XRGB2101010";
		case BYTEVIEW_COLOR_TYPE_XBGR2101010:
			return "BYTEVIEW_COLOR_TYPE_XBGR2101010";
		case BYTEVIEW_COLOR_TYPE_RGBX1010102:
			return "BYTEVIEW_COLOR_TYPE_RGBX1010102";
		case BYTEVIEW_COLOR_TYPE_BGRX1010102:
			return "BYTEVIEW_COLOR_TYPE_BGRX1010102";
		case BYTEVIEW_COLOR_TYPE_ARGB2101010:
			return "BYTEVIEW_COLOR_TYPE_ARGB2101010";
		case BYTEVIEW_COLOR_TYPE_ABGR2101010:
			return "BYTEVIEW_COLOR_TYPE_ABGR2101010";
		case BYTEVIEW_COLOR_TYPE_RGBA1010102:
			return "BYTEVIEW_COLOR_TYPE_RGBA1010102";
		case BYTEVIEW_COLOR_TYPE_BGRA1010102:
			return "BYTEVIEW_COLOR_TYPE_BGRA1010102";
		case BYTEVIEW_COLOR_TYPE_RGB161616:
			return "BYTEVIEW_COLOR_TYPE_RGB161616";
		case BYTEVIEW_COLOR_TYPE_BGR161616:
			return "BYTEVIEW_COLOR_TYPE_BGR161616";
		case BYTEVIEW_COLOR_TYPE_RGBA16161616:
			return "BYTEVIEW_COLOR_TYPE_RGBA16161616";
		case BYTEVIEW_COLOR_TYPE_BGRA16161616:
			return "BYTEVIEW_COLOR_TYPE_BGRA16161616";
		case BYTEVIEW_COLOR_TYPE_ARGB16161616:
			return "BYTEVIEW_COLOR_TYPE_ARGB16161616";
		case BYTEVIEW_COLOR_TYPE_ABGR16161616:
			return "BYTEVIEW_COLOR_TYPE_ABGR16161616";
		case BYTEVIEW_COLOR_TYPE_GRADIENT:
			return "BYTEVIEW_COLOR_TYPE_GRADIENT";
		case BYTEVIEW_COLOR_TYPE_CLASS:
			return "BYTEVIEW_COLOR_TYPE_CLASS";
		case BYTEVIEW_COLOR_TYPE_COUNT:
			return "BYTEVIEW_COLOR_TYPE_COUNT";
		default:
			return "None";
	}
}

static unsigned char Read8Bits(const unsigned char* fileData, unsigned int fileSize, unsigned int byteIndex) {
	if (byteIndex < fileSize) return fileData[byteIndex];

	return 0;
}

static unsigned short Read16Bits(const unsigned char* fileData, unsigned int fileSize, unsigned int byteIndex) {
	unsigned int low;
	unsigned int high;

	low = (unsigned int)Read8Bits(fileData, fileSize, byteIndex);
	high = (unsigned int)Read8Bits(fileData, fileSize, byteIndex + 1U);

	return (unsigned short)(low | (high << 8));
}

static unsigned int Read24Bits(const unsigned char* fileData, unsigned int fileSize, unsigned int byteIndex) {
	unsigned int b1;
	unsigned int b2;
	unsigned int b3;

	b1 = (unsigned int)Read8Bits(fileData, fileSize, byteIndex);
	b2 = (unsigned int)Read8Bits(fileData, fileSize, byteIndex + 1U);
	b3 = (unsigned int)Read8Bits(fileData, fileSize, byteIndex + 2U);

	return b1 | (b2 << 8) | (b3 << 16);
}

static unsigned int Read32Bits(const unsigned char* fileData, unsigned int fileSize, unsigned int byteIndex) {
	unsigned int b1;
	unsigned int b2;
	unsigned int b3;
	unsigned int b4;

	b1 = (unsigned int)Read8Bits(fileData, fileSize, byteIndex);
	b2 = (unsigned int)Read8Bits(fileData, fileSize, byteIndex + 1U);
	b3 = (unsigned int)Read8Bits(fileData, fileSize, byteIndex + 2U);
	b4 = (unsigned int)Read8Bits(fileData, fileSize, byteIndex + 3U);

	return b1 | (b2 << 8) | (b3 << 16) | (b4 << 24);
}

static unsigned char Expand3To8(unsigned char value) {
	return (unsigned char)((value << 5) | (value << 2) | (value >> 1));
}

static unsigned char Expand4To8(unsigned char value) {
	return (unsigned char)((value << 4) | value);
}

static unsigned char Expand5To8(unsigned char value) {
	return (unsigned char)((value << 3) | (value >> 2));
}

static unsigned char Expand6To8(unsigned char value) {
	return (unsigned char)((value << 2) | (value >> 4));
}

static unsigned char Scale16To8(unsigned short value) {
	return (unsigned char)(((unsigned int)value * 255U) / 65535U);
}

static unsigned char Read16BitColorComponent(const unsigned char* fileData, unsigned int fileSize, unsigned int byteIndex) {
	unsigned short value;

	value = Read16Bits(fileData, fileSize, byteIndex);

	return Scale16To8(value);
}

static Color GetRgb332Color(unsigned char pixel) {
	unsigned char r3;
	unsigned char g3;
	unsigned char b2;
	Color color;

	r3 = (unsigned char)((pixel >> 5) & 0x07U);
	g3 = (unsigned char)((pixel >> 2) & 0x07U);
	b2 = (unsigned char)(pixel & 0x03U);

	color.r = Expand3To8(r3);
	color.g = Expand3To8(g3);
	color.b = (unsigned char)(b2 * 85U);
	color.a = 255U;

	return color;
}

static Color GetRgba2222Color(unsigned char pixel) {
	unsigned char r2;
	unsigned char g2;
	unsigned char b2;
	unsigned char a2;
	Color color;

	r2 = (unsigned char)((pixel >> 6) & 0x03U);
	g2 = (unsigned char)((pixel >> 4) & 0x03U);
	b2 = (unsigned char)((pixel >> 2) & 0x03U);
	a2 = (unsigned char)(pixel & 0x03U);

	color.r = (unsigned char)(r2 * 85U);
	color.g = (unsigned char)(g2 * 85U);
	color.b = (unsigned char)(b2 * 85U);
	color.a = (unsigned char)(a2 * 85U);

	return color;
}

static Color GetRgb565Color(unsigned short pixel) {
	unsigned char r5;
	unsigned char g6;
	unsigned char b5;
	Color color;

	r5 = (unsigned char)((pixel >> 11) & 0x01FU);
	g6 = (unsigned char)((pixel >> 5) & 0x03FU);
	b5 = (unsigned char)(pixel & 0x01FU);

	color.r = Expand5To8(r5);
	color.g = Expand6To8(g6);
	color.b = Expand5To8(b5);
	color.a = 255U;

	return color;
}

static Color GetBgr565Color(unsigned short pixel) {
	unsigned char b5;
	unsigned char g6;
	unsigned char r5;
	Color color;

	b5 = (unsigned char)((pixel >> 11) & 0x01FU);
	g6 = (unsigned char)((pixel >> 5) & 0x03FU);
	r5 = (unsigned char)(pixel & 0x01FU);

	color.r = Expand5To8(r5);
	color.g = Expand6To8(g6);
	color.b = Expand5To8(b5);
	color.a = 255U;

	return color;
}

static Color GetXrgb1555Color(unsigned short pixel) {
	unsigned char r5;
	unsigned char g5;
	unsigned char b5;
	Color color;

	r5 = (unsigned char)((pixel >> 10) & 0x01FU);
	g5 = (unsigned char)((pixel >> 5) & 0x01FU);
	b5 = (unsigned char)(pixel & 0x01FU);

	color.r = Expand5To8(r5);
	color.g = Expand5To8(g5);
	color.b = Expand5To8(b5);
	color.a = 255U;

	return color;
}

static Color GetArgb1555Color(unsigned short pixel) {
	unsigned char a1;
	unsigned char r5;
	unsigned char g5;
	unsigned char b5;
	Color color;

	a1 = (unsigned char)((pixel >> 15) & 0x01U);
	r5 = (unsigned char)((pixel >> 10) & 0x01FU);
	g5 = (unsigned char)((pixel >> 5) & 0x01FU);
	b5 = (unsigned char)(pixel & 0x01FU);

	color.r = Expand5To8(r5);
	color.g = Expand5To8(g5);
	color.b = Expand5To8(b5);
	color.a = (a1) ? 255U : 0U;

	return color;
}

static Color GetRgbx5551Color(unsigned short pixel) {
	unsigned char r5;
	unsigned char g5;
	unsigned char b5;
	Color color;

	r5 = (unsigned char)((pixel >> 11) & 0x01FU);
	g5 = (unsigned char)((pixel >> 6) & 0x01FU);
	b5 = (unsigned char)((pixel >> 1) & 0x01FU);

	color.r = Expand5To8(r5);
	color.g = Expand5To8(g5);
	color.b = Expand5To8(b5);
	color.a = 255U;

	return color;
}

static Color GetRgba5551Color(unsigned short pixel) {
	unsigned char r5;
	unsigned char g5;
	unsigned char b5;
	unsigned char a1;
	Color color;

	r5 = (unsigned char)((pixel >> 11) & 0x1FU);
	g5 = (unsigned char)((pixel >> 6) & 0x1FU);
	b5 = (unsigned char)((pixel >> 1) & 0x1FU);
	a1 = (unsigned char)(pixel & 0x01U);

	color.r = Expand5To8(r5);
	color.g = Expand5To8(g5);
	color.b = Expand5To8(b5);
	color.a = (a1) ? 255U : 0U;

	return color;
}

static Color GetRgbx4444Color(unsigned short pixel) {
	unsigned char r4;
	unsigned char g4;
	unsigned char b4;
	Color color;

	r4 = (unsigned char)((pixel >> 12) & 0x0FU);
	g4 = (unsigned char)((pixel >> 8) & 0x0FU);
	b4 = (unsigned char)((pixel >> 4) & 0x0FU);

	color.r = (unsigned char)(r4 * 17U);
	color.g = (unsigned char)(g4 * 17U);
	color.b = (unsigned char)(b4 * 17U);
	color.a = 255U;

	return color;
}

static Color GetXrgb4444Color(unsigned short pixel) {
	unsigned char r4;
	unsigned char g4;
	unsigned char b4;
	Color color;

	r4 = (unsigned char)((pixel >> 8) & 0x0FU);
	g4 = (unsigned char)((pixel >> 4) & 0x0FU);
	b4 = (unsigned char)(pixel & 0x0FU);

	color.r = (unsigned char)(r4 * 17U);
	color.g = (unsigned char)(g4 * 17U);
	color.b = (unsigned char)(b4 * 17U);
	color.a = 255U;

	return color;
}

static Color GetRgba4444Color(unsigned short pixel) {
	unsigned char r4;
	unsigned char g4;
	unsigned char b4;
	unsigned char a4;
	Color color;

	r4 = (unsigned char)((pixel >> 12) & 0x0FU);
	g4 = (unsigned char)((pixel >> 8) & 0x0FU);
	b4 = (unsigned char)((pixel >> 4) & 0x0FU);
	a4 = (unsigned char)(pixel & 0x0FU);

	color.r = (unsigned char)(r4 * 17U);
	color.g = (unsigned char)(g4 * 17U);
	color.b = (unsigned char)(b4 * 17U);
	color.a = (unsigned char)(a4 * 17U);

	return color;
}

static Color GetArgb4444Color(unsigned short pixel) {
	unsigned char r4;
	unsigned char g4;
	unsigned char b4;
	unsigned char a4;
	Color color;

	a4 = (unsigned char)((pixel >> 12) & 0x0FU);
	r4 = (unsigned char)((pixel >> 8) & 0x0FU);
	g4 = (unsigned char)((pixel >> 4) & 0x0FU);
	b4 = (unsigned char)(pixel & 0x0FU);

	color.r = (unsigned char)(r4 * 17U);
	color.g = (unsigned char)(g4 * 17U);
	color.b = (unsigned char)(b4 * 17U);
	color.a = (unsigned char)(a4 * 17U);

	return color;
}

static Color GetVUY888Color(unsigned int pixel) {
	unsigned char v;
	unsigned char u;
	unsigned char y;
	float c;
	float d;
	float e;
	float r;
	float g;
	float b;
	Color color;

	v = (pixel >> 16) & 0x0FFU;
	u = (pixel >> 8) & 0x0FFU;
	y = pixel & 0x0FFU;

	c = (float)(y)-16.0f;
	d = (float)(u)-128.0f;
	e = (float)(v)-128.0f;

	r = 1.164f * c + 1.596f * e;
	g = 1.164f * c - 0.392f * d - 0.813f * e;
	b = 1.164f * c + 2.017f * d;

	color.r = (unsigned char)Clamp(r, 0.0f, 255.0f);
	color.g = (unsigned char)Clamp(g, 0.0f, 255.0f);
	color.b = (unsigned char)Clamp(b, 0.0f, 255.0f);
	color.a = 255U;

	return color;
}

static Color GetXRGB2101010Color(unsigned int pixel) {
	unsigned int r10;
	unsigned int g10;
	unsigned int b10;
	Color color;

	r10 = (pixel >> 20) & 0x3FFU;
	g10 = (pixel >> 10) & 0x3FFU;
	b10 = pixel & 0x3FFU;

	color.r = (unsigned char)((r10 * 255U) / 1023U);
	color.g = (unsigned char)((g10 * 255U) / 1023U);
	color.b = (unsigned char)((b10 * 255U) / 1023U);
	color.a = (255U);

	return color;
}

static Color GetXBGR2101010Color(unsigned int pixel) {
	unsigned int r10;
	unsigned int g10;
	unsigned int b10;
	Color color;

	b10 = (pixel >> 20) & 0x3FFU;
	g10 = (pixel >> 10) & 0x3FFU;
	r10 = pixel & 0x3FFU;

	color.r = (unsigned char)((r10 * 255U) / 1023U);
	color.g = (unsigned char)((g10 * 255U) / 1023U);
	color.b = (unsigned char)((b10 * 255U) / 1023U);
	color.a = 255U;

	return color;
}

static Color GetRGBX1010102Color(unsigned int pixel) {
	unsigned int r10;
	unsigned int g10;
	unsigned int b10;
	Color color;

	r10 = (pixel >> 22) & 0x3FFU;
	g10 = (pixel >> 12) & 0x3FFU;
	b10 = (pixel >> 2) & 0x3FFU;

	color.r = (unsigned char)((r10 * 255U) / 1023U);
	color.g = (unsigned char)((g10 * 255U) / 1023U);
	color.b = (unsigned char)((b10 * 255U) / 1023U);
	color.a = 255U;

	return color;
}

static Color GetBGRX1010102Color(unsigned int pixel) {
	unsigned int r10;
	unsigned int g10;
	unsigned int b10;
	Color color;

	b10 = (pixel >> 22) & 0x3FFU;
	g10 = (pixel >> 12) & 0x3FFU;
	r10 = (pixel >> 2) & 0x3FFU;

	color.r = (unsigned char)((r10 * 255U) / 1023U);
	color.g = (unsigned char)((g10 * 255U) / 1023U);
	color.b = (unsigned char)((b10 * 255U) / 1023U);
	color.a = 255U;

	return color;
}

static Color GetARGB2101010Color(unsigned int pixel) {
	unsigned int a2;
	unsigned int r10;
	unsigned int g10;
	unsigned int b10;
	Color color;

	a2 = (pixel >> 30) & 0x03U;
	r10 = (pixel >> 20) & 0x3FFU;
	g10 = (pixel >> 10) & 0x3FFU;
	b10 = pixel & 0x3FFU;

	color.r = (unsigned char)((r10 * 255U) / 1023U);
	color.g = (unsigned char)((g10 * 255U) / 1023U);
	color.b = (unsigned char)((b10 * 255U) / 1023U);
	color.a = (unsigned char)(a2 * 85U);

	return color;
}

static Color GetABGR2101010Color(unsigned int pixel) {
	unsigned int a2;
	unsigned int r10;
	unsigned int g10;
	unsigned int b10;
	Color color;

	a2 = (pixel >> 30) & 0x03U;
	b10 = (pixel >> 20) & 0x3FFU;
	g10 = (pixel >> 10) & 0x3FFU;
	r10 = pixel & 0x3FFU;

	color.r = (unsigned char)((r10 * 255U) / 1023U);
	color.g = (unsigned char)((g10 * 255U) / 1023U);
	color.b = (unsigned char)((b10 * 255U) / 1023U);
	color.a = (unsigned char)(a2 * 85U);

	return color;
}

static Color GetRGBA1010102Color(unsigned int pixel) {
	unsigned int r10;
	unsigned int g10;
	unsigned int b10;
	unsigned int a2;
	Color color;

	r10 = (pixel >> 22) & 0x3FF;
	g10 = (pixel >> 12) & 0x3FF;
	b10 = (pixel >> 2) & 0x3FF;
	a2 = pixel & 0x03;

	color.r = (unsigned char)((r10 * 255U) / 1023U);
	color.g = (unsigned char)((g10 * 255U) / 1023U);
	color.b = (unsigned char)((b10 * 255U) / 1023U);
	color.a = (unsigned char)(a2 * 85U);

	return color;
}

static Color GetBGRA1010102Color(unsigned int pixel) {
	unsigned int r10;
	unsigned int g10;
	unsigned int b10;
	unsigned int a2;
	Color color;

	b10 = (pixel >> 22) & 0x3FF;
	g10 = (pixel >> 12) & 0x3FF;
	r10 = (pixel >> 2) & 0x3FF;
	a2 = pixel & 0x003;

	color.r = (unsigned char)((r10 * 255U) / 1023U);
	color.g = (unsigned char)((g10 * 255U) / 1023U);
	color.b = (unsigned char)((b10 * 255U) / 1023U);
	color.a = (unsigned char)(a2 * 85U);

	return color;
}


static Color GetClassColor(unsigned char pixel) {
	Color color;

	color = BLANK;

	if (pixel == 0U) {
		color = BLACK;
	} else if (pixel == 255U) {
		color = WHITE;
	} else if ((pixel > 31U) && (pixel < 127U)) {
		color = BLUE;
	} else {
		color = RED;
	}

	return color;
}
