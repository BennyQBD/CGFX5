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

	inline Color(float red, float green, float blue, float alpha=1.0f);
	inline Color(const Vector& colorVector);

	inline float operator[](uint32 index) const;
	inline Color operator-() const;
	inline Color operator+(const Color& other) const;
	inline Color operator+=(const Color& other);
	inline Color operator-(const Color& other) const;
	inline Color operator-=(const Color& other);
	inline Color operator*(const Color& other) const;
	inline Color operator*=(const Color& other);
	inline Color operator/(const Color& other) const;
	inline Color operator/=(const Color& other);
	inline bool operator==(const Color& other) const;
	inline bool operator!=(const Color& other) const;
	inline bool equals(const Color& other, float errorMargin=1.e-4f) const;

	inline float getLuminance() const;
	inline Color quantized() const;

	inline Color contrastAdjust(float contrast) const;
	inline Color saturationAdjust(float saturation) const;
	inline Color varianceAdjust(float variance) const;
private:
	static const Vector luminanceVector;

	Vector data;
};

inline Color::Color(float red, float green, float blue, float alpha) :
	data(Vector::make(red, green, blue, alpha)) {}

inline Color::Color(const Vector& colorVector) :
	data(colorVector) {}

inline float Color::operator[](uint32 index) const
{
	return data[index];
}

inline Color Color::operator-() const
{
	return Color((VectorConstants::ONE - data)
		.select(VectorConstants::MASK_W, data));
}

inline Color Color::operator+(const Color& other) const
{
	return Color(data + other.data);
}

inline Color Color::operator+=(const Color& other)
{
	data = data + other.data;
	return *this;
}

inline Color Color::operator-(const Color& other) const
{
	return Color(data - other.data);
}

inline Color Color::operator-=(const Color& other)
{
	data = data - other.data;
	return *this;
}

inline Color Color::operator*(const Color& other) const
{
	return Color(data * other.data);
}

inline Color Color::operator*=(const Color& other)
{
	data = data * other.data;
	return *this;
}

inline Color Color::operator/(const Color& other) const
{
	return Color(data / other.data);
}

inline Color Color::operator/=(const Color& other)
{
	data = data / other.data;
	return *this;
}

inline bool Color::operator==(const Color& other) const
{
	return (data != other.data).isZero4f();
}

inline bool Color::operator!=(const Color& other) const
{
	return (data == other.data).isZero4f();
}

inline bool Color::equals(const Color& other, float errorMargin) const
{
	return (data.notEquals(other.data, errorMargin)).isZero4f();
}


inline float Color::getLuminance() const
{
	return luminanceVector.dot4(data)[0];
}

inline Color Color::quantized() const
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


inline Color Color::contrastAdjust(float contrast) const
{
	return Color(
			Math::lerp(VectorConstants::HALF, data, contrast)
			.select(VectorConstants::MASK_W, data));
}

inline Color Color::saturationAdjust(float saturation) const
{
	return Color(
			Math::lerp(Vector::load1f(getLuminance()), data, saturation)
			.select(VectorConstants::MASK_W, data));
}

inline Color Color::varianceAdjust(float variance) const
{
	return Color(
			(data * Vector::load1f(variance))
			.select(VectorConstants::MASK_W, data));
}
