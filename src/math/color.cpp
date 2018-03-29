#include "color.hpp"

const Color Color::WHITE(1.0f, 1.0f, 1.0f);
const Color Color::BLACK(0.0f, 0.0f, 0.0f);
const Color Color::RED(1.0f, 0.0f, 0.0f);
const Color Color::GREEN(0.0f, 1.0f, 0.0f);
const Color Color::BLUE(0.0f, 0.0f, 1.0f);
const Color Color::CYAN(0.0f, 1.0f, 1.0f);
const Color Color::YELLOW(1.0f, 1.0f, 0.0f);
const Color Color::MAGENTA(1.0f, 0.0f, 1.0f);
const Color Color::TRANSPARENT(0.0f, 0.0f, 0.0f, 0.0f);

const Vector Color::luminanceVector(Vector::make(0.2125f, 0.7154f, 0.0721f, 0.0f));
