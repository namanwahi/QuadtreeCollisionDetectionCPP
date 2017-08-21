#ifndef RECT_H
#define RECT_H

struct Rect {
	//x and y position of top left corner
	int x;
	int y;

	//width and height of the rectangle
	int w;
	int h;

	bool intersect(const Rect&) const;
};

#endif
