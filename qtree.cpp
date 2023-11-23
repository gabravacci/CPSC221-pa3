/**
 * @file qtree.cpp
 * @description student implementation of QTree class used for storing image data
 *              CPSC 221 PA3
 *
 *              SUBMIT THIS FILE
 */

#include "qtree.h"

/**
 * Constructor that builds a QTree out of the given PNG.
 * Every leaf in the tree corresponds to a pixel in the PNG.
 * Every non-leaf node corresponds to a rectangle of pixels
 * in the original PNG, represented by an (x,y) pair for the
 * upper left corner of the rectangle and an (x,y) pair for
 * lower right corner of the rectangle. In addition, the Node
 * stores a pixel representing the average color over the
 * rectangle.
 *
 * The average color for each node in your implementation MUST
 * be determined in constant time. HINT: this will lead to nodes
 * at shallower levels of the tree to accumulate some error in their
 * average color value, but we will accept this consequence in
 * exchange for faster tree construction.
 * Note that we will be looking for specific color values in our
 * autograder, so if you instead perform a slow but accurate
 * average color computation, you will likely fail the test cases!
 *
 * Every node's children correspond to a partition of the
 * node's rectangle into (up to) four smaller rectangles. The node's
 * rectangle is split evenly (or as close to evenly as possible)
 * along both horizontal and vertical axes. If an even split along
 * the vertical axis is not possible, the extra line will be included
 * in the left side; If an even split along the horizontal axis is not
 * possible, the extra line will be included in the upper side.
 * If a single-pixel-wide rectangle needs to be split, the NE and SE children
 * will be null; likewise if a single-pixel-tall rectangle needs to be split,
 * the SW and SE children will be null.
 *
 * In this way, each of the children's rectangles together will have coordinates
 * that when combined, completely cover the original rectangle's image
 * region and do not overlap.
 */
QTree::QTree(const PNG& imIn) {
	// ADD YOUR IMPLEMENTATION BELOW
	this->height = imIn.height();
	this->width = imIn.width();
	pair<unsigned int, unsigned int> ul(0,0);
	pair<unsigned int, unsigned int> lr(width - 1, height - 1);
	this->root = BuildNode(imIn, ul, lr);
}

/**
 * Overloaded assignment operator for QTrees.
 * Part of the Big Three that we must define because the class
 * allocates dynamic memory. This depends on your implementation
 * of the copy and clear funtions.
 *
 * @param rhs The right hand side of the assignment statement.
 */
QTree& QTree::operator=(const QTree& rhs) {
	// ADD YOUR IMPLEMENTATION BELOW
	
}

/**
 * Render returns a PNG image consisting of the pixels
 * stored in the tree. may be used on pruned trees. Draws
 * every leaf node's rectangle onto a PNG canvas using the
 * average color stored in the node.
 *
 * For up-scaled images, no color interpolation will be done;
 * each rectangle is fully rendered into a larger rectangular region.
 *
 * @param scale multiplier for each horizontal/vertical dimension
 * @pre scale > 0
 */
PNG QTree::Render(unsigned int scale) const {
	// Replace the line below with your implementation
	/**
	 * @todo FIX SCALING FACTOR
	*/
	PNG img(width*scale, height*scale);
	
	vector<Node*> stack;
	vector<Node*> leaves;
	stack.push_back(root);
	Node* nd;

	while(!stack.empty()) {
		nd = stack.back();
		stack.pop_back();
		if (!nd->NW) {
			leaves.push_back(nd);
		}
		else {
			stack.push_back(nd->NW);
			if (nd->NE) stack.push_back(nd->NE);
			if (nd->SW) stack.push_back(nd->SW);
			if (nd->SE) stack.push_back(nd->SE);
		}
	}

	
	for (auto & it : leaves) {
		// this seems hella slow, can improve?
		if (scale > 1) {
			unsigned int w = it->upLeft.first;
			unsigned int h = it->upLeft.second;
			if (h == 0) h = 1;
			if (w == 0) w = 1;
			for (unsigned int y = it->upLeft.second; y <= h*scale; y++) {
				for (unsigned int x = it->upLeft.first; x <= w*scale; x++) {
					RGBAPixel* px = img.getPixel(x, y);
					px->r = it->avg.r;
					px->g = it->avg.g;
					px->b = it->avg.b;	
				}
			}
		} else {
			RGBAPixel* px = img.getPixel(it->upLeft.first, it->upLeft.second);
			px->r = it->avg.r;
			px->g = it->avg.g;
			px->b = it->avg.b;
		}
	}
	return img;
}

/**
 *  Prune function trims subtrees as high as possible in the tree.
 *  A subtree is pruned (cleared) if all of the subtree's leaves are within
 *  tolerance of the average color stored in the root of the subtree.
 *  NOTE - you may use the distanceTo function found in RGBAPixel.h
 *  Pruning criteria should be evaluated on the original tree, not
 *  on any pruned subtree. (we only expect that trees would be pruned once.)
 *
 * You may want a recursive helper function for this one.
 *
 * @param tolerance maximum RGBA distance to qualify for pruning
 * @pre this tree has not previously been pruned, nor is copied from a previously pruned tree.
 */
void QTree::Prune(double tolerance) {
	// ADD YOUR IMPLEMENTATION BELOW
	
}

/**
 *  FlipHorizontal rearranges the contents of the tree, so that
 *  its rendered image will appear mirrored across a vertical axis.
 *  This may be called on a previously pruned/flipped/rotated tree.
 *
 *  After flipping, the NW/NE/SW/SE pointers must map to what will be
 *  physically rendered in the respective NW/NE/SW/SE corners, but it
 *  is no longer necessary to ensure that 1-pixel wide rectangles have
 *  null eastern children
 *  (i.e. after flipping, a node's NW and SW pointers may be null, but
 *  have non-null NE and SE)
 * 
 *  You may want a recursive helper function for this one.
 */
void QTree::FlipHorizontal() {
	// ADD YOUR IMPLEMENTATION BELOW
	
}

/**
 *  RotateCCW rearranges the contents of the tree, so that its
 *  rendered image will appear rotated by 90 degrees counter-clockwise.
 *  This may be called on a previously pruned/flipped/rotated tree.
 *
 *  Note that this may alter the dimensions of the rendered image, relative
 *  to its original dimensions.
 *
 *  After rotation, the NW/NE/SW/SE pointers must map to what will be
 *  physically rendered in the respective NW/NE/SW/SE corners, but it
 *  is no longer necessary to ensure that 1-pixel tall or wide rectangles
 *  have null eastern or southern children
 *  (i.e. after rotation, a node's NW and NE pointers may be null, but have
 *  non-null SW and SE, or it may have null NW/SW but non-null NE/SE)
 *
 *  You may want a recursive helper function for this one.
 */
void QTree::RotateCCW() {
	// ADD YOUR IMPLEMENTATION BELOW
	
}

/**
 * Destroys all dynamically allocated memory associated with the
 * current QTree object. Complete for PA3.
 * You may want a recursive helper function for this one.
 */
void QTree:: Clear() {
	// ADD YOUR IMPLEMENTATION BELOW
	
}

/**
 * Copies the parameter other QTree into the current QTree.
 * Does not free any memory. Called by copy constructor and operator=.
 * You may want a recursive helper function for this one.
 * @param other The QTree to be copied.
 */
void QTree::Copy(const QTree& other) {
	// ADD YOUR IMPLEMENTATION BELOW
	
}

/**
 * Private helper function for the constructor. Recursively builds
 * the tree according to the specification of the constructor.
 * @param img reference to the original input image.
 * @param ul upper left point of current node's rectangle.
 * @param lr lower right point of current node's rectangle.
 */
Node* QTree::BuildNode(const PNG& img, pair<unsigned int, unsigned int> ul, pair<unsigned int, unsigned int> lr) {
	// first of all: SORRY LMAO
	unsigned int width = (lr.first - ul.first + 1);
	unsigned int height = (lr.second - ul.second + 1);
	unsigned int totalArea = height*width; // useful later
	// default blank node so I don't have to reconstruct it everytime #memoryefficient #POGUS
	RGBAPixel a;
	Node* nd = new Node(ul, lr, a);
	// base-case: single pixel (ul == lr) ==> height = width = 1
	if (height == 1 && width == 1) {
		nd->NE = nullptr;
		nd->NW = nullptr;
		nd->SE = nullptr;
		nd->SW = nullptr;
		// they're the same so doesn't matter LOLOLOLOL
		RGBAPixel* px = img.getPixel(ul.first, lr.second);
		// avg color of a pixel = pixel GOATED
		a.r = px->r;
		a.g = px->g;
		a.b = px->b;
		nd->avg = a;
		return nd;
	}

	// subcase 1: unit width rectangle 
	if (width == 1) {
		unsigned int midY = (ul.second + lr.second)/2;

		// NW COORDINATES AND AREA ****************************************************
		pair<unsigned int, unsigned int> ulNW(ul.first, ul.second);
		pair<unsigned int, unsigned int> lrNW(ul.first, midY);
		unsigned int weightNW = (midY - ul.second + 1); // area of rectangle, basically
		// SW COORDINATES AND ARE *****************************************************
		pair<unsigned int, unsigned int> ulSW(ul.first, midY+1);
		pair<unsigned int, unsigned int> lrSW(ul.first, lr.second);
		unsigned int weightSW = (lr.second - midY);

		if ((weightNW + weightSW) != totalArea) {
			std::cout << "SKILL ISSUE" << std::endl;
			return nullptr;
		}

		// recursion OMEGALOL
		nd->NW = BuildNode(img, ulNW, lrNW);
		nd->NE = nullptr;
		nd->SW = BuildNode(img, ulSW, lrSW);
		nd->SE = nullptr;

		// time to calculate the average in O(1) POGUS
		a.r = (nd->NW->avg.r)*weightNW + (nd->SW->avg.r)*weightSW;
		a.r = a.r/(weightNW + weightSW);
		a.g = (nd->NW->avg.g)*weightNW + (nd->SW->avg.g)*weightSW;
		a.g = a.g/(weightNW + weightSW);
		a.b = (nd->NW->avg.b)*weightNW + (nd->SW->avg.b)*weightSW;
		a.b = a.b/(weightNW + weightSW);
	}
	// subcase 2: unit height rectangle
	else if (height == 1) {
		unsigned int midX = (ul.first + lr.first)/2;

		// NW COORDINATES AND AREA ****************************************************
		pair<unsigned int, unsigned int> ulNW(ul.first, ul.second);
		pair<unsigned int, unsigned int> lrNW(midX, ul.second);
		unsigned int weightNW = (midX - ul.first + 1); // area of rectangle, basically
		// NE COORDINATES AND AREA ****************************************************
		pair<unsigned int, unsigned int> ulNE(midX+1, ul.second);
		pair<unsigned int, unsigned int> lrNE(lr.first, ul.second);
		unsigned int weightNE = (lr.first - midX);

		if ((weightNW + weightNE) != totalArea) {
			std::cout << "SKILL ISSUE" << std::endl;
			return nullptr;
		}

		// recursion OMEGALOL
		nd->NW = BuildNode(img, ulNW, lrNW);
		nd->NE = BuildNode(img, ulNE, lrNE);
		nd->SW = nullptr;
		nd->SE = nullptr;

		// time to calculate the average in O(1) POGUS
		a.r = (nd->NW->avg.r)*weightNW + (nd->NE->avg.r)*weightNE;
		a.r = a.r/(weightNW + weightNE);
		a.g = (nd->NW->avg.g)*weightNW + (nd->NE->avg.g)*weightNE;
		a.g = a.g/(weightNW + weightNE);
		a.b = (nd->NW->avg.b)*weightNW + (nd->NE->avg.b)*weightNE;
		a.b = a.b/(weightNW + weightNE);
	}
	else {
		// find midpoint in X
		unsigned int midX = (ul.first + lr.first)/2;
		unsigned int midY = (ul.second + lr.second)/2;

		// start making BABIES
		// NW COORDINATES AND AREA ****************************************************
		pair<unsigned int, unsigned int> ulNW(ul.first, ul.second);
		pair<unsigned int, unsigned int> lrNW(midX, midY);
		unsigned int weightNW = (midX - ul.first + 1)*(midY - ul.second + 1); // area of rectangle, basically
		// NE COORDINATES AND AREA ****************************************************
		pair<unsigned int, unsigned int> ulNE(midX+1, ul.second);
		pair<unsigned int, unsigned int> lrNE(lr.first, midY);
		unsigned int weightNE = (lr.first - midX)*(midY - ul.second + 1);
		// SW COORDINATES AND ARE *****************************************************
		pair<unsigned int, unsigned int> ulSW(ul.first, midY+1);
		pair<unsigned int, unsigned int> lrSW(midX, lr.second);
		unsigned int weightSW = (midX - ul.first + 1)*(lr.second - midY);
		// SE COORDINATES AND AREA ****************************************************
		pair<unsigned int, unsigned int> ulSE(midX+1, midY+1);
		pair<unsigned int, unsigned int> lrSE(lr.first, lr.second);
		unsigned int weightSE = (lr.first - midX)*(lr.second - midY);

		if ((weightNW + weightNE + weightSW + weightSE) != totalArea) {
			std::cout << "SKILL ISSUE" << std::endl;
			return nullptr;
		}

		// recursion OMEGALOL
		nd->NW = BuildNode(img, ulNW, lrNW);
		nd->NE = BuildNode(img, ulNE, lrNE);
		nd->SW = BuildNode(img, ulSW, lrSW);
		nd->SE = BuildNode(img, ulSE, lrSE);

		// time to calculate the average in O(1) POGUS
		a.r = (nd->NW->avg.r)*weightNW + (nd->NE->avg.r)*weightNE + (nd->SW->avg.r)*weightSW + (nd->SE->avg.r)*weightSE;
		// a.r = a.r/(weightNW + weightNE + weightSW + weightSE);
		a.g = (nd->NW->avg.g)*weightNW + (nd->NE->avg.g)*weightNE + (nd->SW->avg.g)*weightSW + (nd->SE->avg.g)*weightSE;
		// a.g = a.g/(weightNW + weightNE + weightSW + weightSE);
		a.b = (nd->NW->avg.b)*weightNW + (nd->NE->avg.b)*weightNE + (nd->SW->avg.b)*weightSW + (nd->SE->avg.b)*weightSE;
		// a.b = a.b/(weightNW + weightNE + weightSW + weightSE);
	}
	return nd;
}

/*********************************************************/
/*** IMPLEMENT YOUR OWN PRIVATE MEMBER FUNCTIONS BELOW ***/
/*********************************************************/