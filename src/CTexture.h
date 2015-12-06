#pragma once

#include "GL_common.h"

class CTexture
{
public:
	enum EPixelFormat : uint32_t
	{
		PF_RGB			= 0x00000000,
		PF_BGR			= 0x00000001,
		PF_RGBA			= 0x00000002,
		PF_ARGB			= 0x00000003,
		PF_BGRA			= 0x00000004,
		PF_ABGR			= 0x00000005,
		PF_GRAYSCALE8	= 0x00000007,
		PF_GRAYSCALE16	= 0x00000008,
		PF_GRAYSCALE32	= 0x00000009
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

	struct SPixelGRAYSCALE8
	{
		uint8_t G;
	};

	struct SPixelGRAYSCALE16
	{
		uint16_t G;
	};

	struct SPixelGRAYSCALE32
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
