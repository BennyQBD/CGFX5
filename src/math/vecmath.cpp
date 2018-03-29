#include "vecmath.hpp"
#include <limits>

const Vector VectorConstants::ZERO(Vector::make(0.0f, 0.0f, 0.0f, 0.0f));
const Vector VectorConstants::ONE(Vector::make(1.0f, 1.0f, 1.0f, 1.0f));
const Vector VectorConstants::TWO(Vector::make(2.0f, 2.0f, 2.0f, 2.0f));
const Vector VectorConstants::HALF(Vector::make(0.5f, 0.5f, 0.5f, 0.5f));
const Vector VectorConstants::INF(Vector::make(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()));
const Vector VectorConstants::MASK_X(Vector::make((uint32)0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF));
const Vector VectorConstants::MASK_Y(Vector::make((uint32)0xFFFFFFFF, 0, 0xFFFFFFFF, 0xFFFFFFFF));
const Vector VectorConstants::MASK_Z(Vector::make((uint32)0xFFFFFFFF, 0xFFFFFFFF, 0, 0xFFFFFFFF));
const Vector VectorConstants::MASK_W(Vector::make((uint32)0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0));
const Vector VectorConstants::SIGN_MASK(Vector::make((uint32)0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF));

