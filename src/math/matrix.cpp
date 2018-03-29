#include "matrix.hpp"

Quaternion Matrix::getRotation() const
{
	float result[4];
	float m[4][4];
	Matrix temp(*this);
	temp.removeScale();
	for(uint32 i = 0; i < 4; i++) {
		temp.m[i].store4f(m[i]);
	}
	float trace = m[0][0] + m[1][1] + m[2][2];
	if(trace > 0) {
		float s = 0.5f*Math::rsqrt(trace+1.0f);
		result[3] = 0.25f/s;
		result[0] = (m[1][2] - m[2][1]) * s;
		result[1] = (m[2][0] - m[0][2]) * s;
		result[2] = (m[0][1] - m[1][0]) * s;
	} else {
		if(m[0][0] > m[1][1] && m[0][0] > m[2][2])
		{
			float s = 2.0f * Math::sqrt(1.0f + m[0][0] - m[1][1] - m[2][2]);
			result[3] = (m[1][2] - m[2][1]) / s;
			result[0] = 0.25f * s;
			result[1] = (m[1][0] + m[0][1]) / s;
			result[2] = (m[2][0] + m[0][2]) / s;
		}
		else if(m[1][1] > m[2][2])
		{
			float s = 2.0f * Math::sqrt(1.0f + m[1][1] - m[0][0] - m[2][2]);
			result[3] = (m[2][0] - m[0][2]) / s;
			result[0] = (m[1][0] + m[0][1]) / s;
			result[1] = 0.25f * s;
			result[2] = (m[1][2] + m[2][1]) / s;
		}
		else
		{
			float s = 2.0f * Math::sqrt(1.0f + m[2][2] - m[0][0] - m[1][1]);
			result[3] = (m[0][1] - m[1][0]) / s;
			result[0] = (m[2][0] + m[0][2]) / s;
			result[1] = (m[1][2] + m[2][1]) / s;
			result[2] = 0.25f * s;
		}
	}
	return Quaternion(result[0], result[1], result[2], -result[3]).normalized();
}

void Matrix::extractFrustumPlanes(Plane* planes) const
{
	planes[0] = Plane(m[3]+m[2]).normalized();
	planes[1] = Plane(m[3]-m[2]).normalized();
	planes[2] = Plane(m[3]+m[1]).normalized();
	planes[3] = Plane(m[3]-m[1]).normalized();
	planes[4] = Plane(m[3]+m[0]).normalized();
	planes[5] = Plane(m[3]-m[0]).normalized();
}

Matrix Matrix::toNormalMatrix() const
{
	// TODO: There *should* be a faster and easier way to do this!
	return inverse().transpose();
}
