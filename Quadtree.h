#ifndef QUADTREE_H
#define QUADTREE_H

#include "Rect.h"
#include <memory>
#include <vector>
#include <utility>

template <class T>
class Quadtree {

public:
	typedef Rect Region2D;

	Quadtree(const Region2D& region, int level = 0);

	void insert(const T&);

	//represents a collision stored in the tree
	typedef std::pair<T*, T*> collision;

	std::vector<collision> getCollisions() {
		std::vector<collision> collision;
		getCollisionsHelper(collision);
		return collision;
	}

private:
	static constexpr int NODE_CAPACITY = 4;
	static constexpr int MAX_DEPTH = 5;

	//the region covered by this quad tree
	Region2D region;

	//depth of this current node
	int level;

	//pointers to each subregion
	typedef std::unique_ptr<Quadtree> pQuadtree;
	pQuadtree NW;
	pQuadtree NE;
	pQuadtree SW;
	pQuadtree SE;

	//vector containing copies of the obects entered into the tree
	std::vector<const T> node_data;

	//splits the tree into 4 subtrees
	void split();

	bool hasSplit();

	//helper method to collect the collisions in the tree
	void getCollisionsHelper(std::vector<collision>&);
};

template <class T>
Quadtree<T>::Quadtree(const Region2D& region, int level)
	: region(region), level(level),
		NW(nullptr), NE(nullptr),
		SW(nullptr), SW(nullptr) {}

template <class T>
void Quadtree<T>::split() {
	int half_w = region.w/2;
	int half_h = region.h/2;

	int new_x = region.x + half_w;
	int new_y = region.y + half_h;
	int op_w = region.w - half_w;
	int op_h = region.h - half_h;
		
	Region2D nw_rect = {region.x, region.y, half_w + 1, half_h + 1};
	Region2D ne_rect = {new_x, region.y, op_w, half_h + 1 };
	Region2D sw_rect = {region.x, new_y, half_w + 1, op_h};
	Region2D se_rect = {new_x, new_y, op_w, op_h};

	NW = pQuadtree(new Quadtree(nw_rect, level + 1));
	NE = pQuadtree(new Quadtree(ne_rect, level + 1));
	SW = pQuadtree(new Quadtree(sw_rect, level + 1));
	SE = pQuadtree(new Quadtree(se_rect, level + 1));
}

template <class T>
void Quadtree<T>::insert(const T& obj) {

	//if the region cant contain the object then return
	if (!region.intersect(obj)) {
		return;
	}

	//if the node has been split then try to insert the object into all subtrees
	if (hasSplit()) {
		NW->insert(obj);
		NE->insert(obj);
		SW->insert(obj);
		SE->insert(obj);
		return;
	}

	//if there are too many itmes in this current node and this is the max depth then split into four subtrees
	if (node_data.size() > NODE_CAPACITY && level < MAX_DEPTH) {

		split();

		//insert all the objects in the current node into the subtrees
		while (!node_data.empty()) {
			NW->insert(node_data.back());
	  	NE->insert(node_data.back());
		  SW->insert(node_data.back());
   		SE->insert(node_data.back());
			node_data.pop_back();
		}
	}
}

template <class T>
bool Quadtree<T>::hasSplit() {
	//if NW is not null then the tree has split
	return NW;
}

template <class T>
void Quadtree<T>::getCollisionsHelper(std::vector<Quadtree<T>::collision>& bucket) {

	//if the node has split then collect in each subtree
	if (hasSplit()) {
		NW->getCollisionsHelper(bucket);
		SW->getCollisionsHelper(bucket);
		NE->getCollisionsHelper(bucket);
		SE->getCollisionsHelper(bucket);
	} else {
		typedef typename std::vector<const T>::size_type size_vc;
		for (size_vc i = 0 ; i < node_data.size() ; i++) {
			for (size_vc j = i + 1 ; j < node_data.size() ; j++) {
				if (node_data[i]->collides(*node_data[j])) {
					collision p = {node_data[i], node_data[j]};
					bucket.push_back(p);
				}
			}
		}
		
	}
}
#endif
