#pragma once

#include "vecmath.hpp"

class Color
{
public:
	static const Color WHITE;
	static const Color BLACK;
	static const Color RED;
	static const Color GREEN;
	static const Color BLUE;
	static const Color CYAN;
	static const Color YELLOW;
	static const Color MAGENTA;
	static const Color TRANSPARENT;

	FORCEINLINE Color(float red, float green, float blue, float alpha=1.0f);
	FORCEINLINE Color(const Vector& colorVector);

	FORCEINLINE float operator[](uint32 index) const;
	FORCEINLINE Color operator-() const;
	FORCEINLINE Color operator+(const Color& other) const;
	FORCEINLINE Color operator+=(const Color& other);
	FORCEINLINE Color operator-(const Color& other) const;
	FORCEINLINE Color operator-=(const Color& other);
	FORCEINLINE Color operator*(const Color& other) const;
	FORCEINLINE Color operator*=(const Color& other);
	FORCEINLINE Color operator/(const Color& other) const;
	FORCEINLINE Color operator/=(const Color& other);
	FORCEINLINE bool operator==(const Color& other) const;
	FORCEINLINE bool operator!=(const Color& other) const;
	FORCEINLINE bool equals(const Color& other, float errorMargin=1.e-4f) const;

	FORCEINLINE float getLuminance() const;
	FORCEINLINE Color quantized() const;
	FORCEINLINE int32 toInt() const;

	FORCEINLINE Color contrastAdjust(float contrast) const;
	FORCEINLINE Color saturationAdjust(float saturation) const;
	FORCEINLINE Color varianceAdjust(float variance) const;
private:
	static const Vector luminanceVector;

	Vector data;
};

FORCEINLINE Color::Color(float red, float green, float blue, float alpha) :
	data(Vector::make(red, green, blue, alpha)) {}

FORCEINLINE Color::Color(const Vector& colorVector) :
	data(colorVector) {}

FORCEINLINE float Color::operator[](uint32 index) const
{
	return data[index];
}

FORCEINLINE Color Color::operator-() const
{
	return Color((VectorConstants::ONE - data)
		.select(VectorConstants::MASK_W, data));
}

FORCEINLINE Color Color::operator+(const Color& other) const
{
	return Color(data + other.data);
}

FORCEINLINE Color Color::operator+=(const Color& other)
{
	data = data + other.data;
	return *this;
}

FORCEINLINE Color Color::operator-(const Color& other) const
{
	return Color(data - other.data);
}

FORCEINLINE Color Color::operator-=(const Color& other)
{
	data = data - other.data;
	return *this;
}

FORCEINLINE Color Color::operator*(const Color& other) const
{
	return Color(data * other.data);
}

FORCEINLINE Color Color::operator*=(const Color& other)
{
	data = data * other.data;
	return *this;
}

FORCEINLINE Color Color::operator/(const Color& other) const
{
	return Color(data / other.data);
}

FORCEINLINE Color Color::operator/=(const Color& other)
{
	data = data / other.data;
	return *this;
}

FORCEINLINE bool Color::operator==(const Color& other) const
{
	return (data != other.data).isZero4f();
}

FORCEINLINE bool Color::operator!=(const Color& other) const
{
	return (data == other.data).isZero4f();
}

FORCEINLINE bool Color::equals(const Color& other, float errorMargin) const
{
	return (data.notEquals(other.data, errorMargin)).isZero4f();
}


FORCEINLINE float Color::getLuminance() const
{
	return luminanceVector.dot4(data)[0];
}

FORCEINLINE Color Color::quantized() const
{
	float vals[4];
	Vector conversionAmt(Vector::load1f(255.0f));
	(data * conversionAmt).store4f(vals);
	for(uint32 i = 0; i < 4; i++) {
		vals[i] = Math::clamp(vals[i], 0.0f, 255.0f);
		vals[i] = Math::roundToFloat(vals[i]);
	}
	return Color(Vector::load4f(vals) / conversionAmt);
}

FORCEINLINE int32 Color::toInt() const
{
	float vals[4];
	Vector conversionAmt(Vector::load1f(255.0f));
	(data * conversionAmt).store4f(vals);
	uint32 result = 0;
	for(uint32 i = 0; i < 4; i++) {
		vals[i] = Math::clamp(vals[i], 0.0f, 255.0f);
		result = (result << 8) | (uint32)Math::roundToInt(vals[i]);
	}
	return (int32)result;
}


FORCEINLINE Color Color::contrastAdjust(float contrast) const
{
	return Color(
			Math::lerp(VectorConstants::HALF, data, contrast)
			.select(VectorConstants::MASK_W, data));
}

FORCEINLINE Color Color::saturationAdjust(float saturation) const
{
	return Color(
			Math::lerp(Vector::load1f(getLuminance()), data, saturation)
			.select(VectorConstants::MASK_W, data));
}

FORCEINLINE Color Color::varianceAdjust(float variance) const
{
	return Color(
			(data * Vector::load1f(variance))
			.select(VectorConstants::MASK_W, data));
}
