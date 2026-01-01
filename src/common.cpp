#include "common.hpp"

float Locate::Top(Vector2 point, float offset) { return point.y - offset; }
float Locate::Left(Vector2 point, float offset) { return point.x - offset; }
float Locate::Right(Vector2 point, float offset) { return point.x + offset; }
float Locate::Bottom(Vector2 point, float offset) { return point.y + offset; }
