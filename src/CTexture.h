#pragma once

#include "CommonHeaders.h"

class CTexture
{
public:
	enum EPixelFormat : uint32_t
	{
		PF_RGB				= 0x00000000,
		PF_BGR				= 0x00000001,
		PF_RGBA				= 0x00000002,
		PF_ARGB				= 0x00000003,
		PF_BGRA				= 0x00000004,
		PF_ABGR				= 0x00000005,
		PF_GRAYSCALE_U8		= 0x00000007,
		PF_GRAYSCALE_U16	= 0x00000008,
		PF_GRAYSCALE_U32	= 0x00000009,
		PF_GRAYSCALE_F16	= 0x0000000a,
		PF_GRAYSCALE_F24	= 0x0000000b,
		PF_GRAYSCALE_F32	= 0x0000000b,
		PF_RGB_32F			= 0x0000000c
	};

	struct SPixelRGB888
	{
		uint8_t R;
		uint8_t G;
		uint8_t B;
	};

	struct SPixelRGBA8888
	{
		uint8_t R;
		uint8_t G;
		uint8_t B;
		uint8_t A;
	};

	struct SPixelGrayscaleU8
	{
		uint8_t G;
	};

	struct SPixelGrayscaleU16
	{
		uint16_t G;
	};

	struct SPixelGrayscaleU32
	{
		uint32_t G;
	};

	CTexture();
	~CTexture();

	bool Load(const std::string& filename);

	uint32_t GetWidth() const;
	uint32_t GetHeight() const;
	EPixelFormat GetPixelFormat() const;

	int GetPixelSize() const;

	const void* ConstLock() const;
	void* Lock();
	bool Unlock();

	GLuint GetTextureID() const;

private:
	bool LoadFromFileSDL(const std::string& filename);
	bool LoadFromFileMTF(const std::string& filename);
	bool LoadToOpenGL();

	GLuint TextureID;

	void* PixelData;

	EPixelFormat PixelFormat;
	uint32_t Width;
	uint32_t Height;
};
