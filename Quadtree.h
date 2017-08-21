#ifndef QUADTREE_H
#define QUADTREE_H

#include "Rect.h"
#include <memory>

template <class T>
class Quadtree {

public:
	typedef Rect Region2D;

	Quadtree(Region2D region, int level = 0);

	void insert(std::shared_ptr<const T>);

private:
	static constexpr int NODE_CAPACITY = 4;
	static constexpr int MAX_DEPTH = 5;

	//the region covered by this quad tree
	Region2D region;

	//pointers to each subregion
	typedef std::unique_ptr<Quadtree> pQuadtree;
	pQuadtree NW;
	pQuadtree NE;
	pQuadtree SW;
	pQuadtree SE;
	
};

#endif
