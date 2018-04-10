#pragma once

#include "core/common.hpp"
#include "dataStructures/string.hpp"

class ArrayBitmap
{
public:
	ArrayBitmap(int32 width=1, int32 height=1);
	ArrayBitmap(int32 width, int32 height, int32* pixels);
	ArrayBitmap(int32 width, int32 height, int32* pixels, int32 offsetX,
			int32 offsetY, int32 rowOffset);
	ArrayBitmap(const String& fileName);
	virtual ~ArrayBitmap();

	bool load(const String& fileName);
	bool save(const String& fileName) const;
	void clear(int32 color);

	inline int32 getWidth() const;
	inline int32 getHeight() const;

	inline int32 get(int32 x, int32 y) const;
	inline void set(int32 x, int32 y, int32 pixel);
	inline int32* getPixelArray();
	inline const int32* getPixelArray() const;
private:
	int32 width;
	int32 height;
	int32* pixels;

	inline uintptr getPixelsSize() const;
	NULL_COPY_AND_ASSIGN(ArrayBitmap)
};

inline int32 ArrayBitmap::getWidth() const
{
	return width;
}

inline int32 ArrayBitmap::getHeight() const
{
	return height;
}

inline uintptr ArrayBitmap::getPixelsSize() const
{
	return (uintptr)(width * height) * sizeof(pixels[0]);
}

inline int32 ArrayBitmap::get(int32 x, int32 y) const
{
	assertCheck(x >= 0 && x < width);
	assertCheck(y >= 0 && y < height);
	return pixels[x + y * width];
}

inline void ArrayBitmap::set(int32 x, int32 y, int32 pixel)
{
	assertCheck(x >= 0 && x < width);
	assertCheck(y >= 0 && y < height);
	pixels[x + y * width] = pixel;
}

inline int32* ArrayBitmap::getPixelArray()
{
	return pixels;
}

inline const int32* ArrayBitmap::getPixelArray() const
{
	return pixels;
}

