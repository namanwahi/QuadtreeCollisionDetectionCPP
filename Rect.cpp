#include "Rect.h"


bool Rect::intersect(const Rect& r1) {
	return !(r1.x > x + w
        || r1.x + r1.w < x
        || r1.y > y + h
        || r1.y + r1.h < y);
}
