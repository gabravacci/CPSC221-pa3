/**
 * @file qtree.cpp
 * @description student implementation of QTree class used for storing image data
 *              CPSC 221 PA3
 *
 *              SUBMIT THIS FILE
 */

#include "qtree.h"
#include <math.h> 

void flip_horizontal(Node* nd);

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

	// build the tree
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
	if (this != &rhs) {
		Clear(); // clear old tree 
		Copy(rhs); 
	}
    return *this;
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
	cout << endl;
	vector<Node*> stack;
	vector<Node*> leaves;
	stack.push_back(root);
	Node* nd;

	while (!stack.empty()) {
		nd = stack.back();
		stack.pop_back(); // remove last element (stack)

		int w = scale * (nd->lowRight.first - nd->upLeft.first + 1);
		int h = scale * (nd->lowRight.second - nd->upLeft.second + 1);

		for (uint x_iter = 0; x_iter < w; x_iter++) {
			for (uint y_iter = 0; y_iter < h; y_iter++) {
				int x = scale*nd->upLeft.first + x_iter; 
				int y = scale*nd->upLeft.second + y_iter;
				RGBAPixel* px = img.getPixel(x, y);

	 			px->r = nd->avg.r;
	 			px->g = nd->avg.g;
	 			px->b = nd->avg.b;	
			}
		}

		if (nd->NE) stack.push_back(nd->NE);
		if (nd->SW) stack.push_back(nd->SW);
		if (nd->SE) stack.push_back(nd->SE);
		if (nd->NW) stack.push_back(nd->NW);

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
	flip_horizontal(this->root);
}

void QTree::flip_horizontal(Node* nd) {
	if (nd) {

		Node* temp = nullptr;
		if (nd->NW && nd->NE) {
			swap_positions(nd->NW, nd->NE);
			update_child_positions(nd->NW);
			update_child_positions(nd->NE);
			temp = nd->NW;
			nd->NW = nd->NE;
			nd->NE = temp;
		}
		if (nd->SW && nd->SE) {
			swap_positions(nd->SW, nd->SE);
			update_child_positions(nd->SW);
			update_child_positions(nd->SE);
			temp = nd->SW;
			nd->SW = nd->SE;
			nd->SE = temp;
		}
		flip_horizontal(nd->NW);
		flip_horizontal(nd->NE);
		flip_horizontal(nd->SW);
		flip_horizontal(nd->SE);
	}
}

// nodes keep the same members but switch positions
void QTree::swap_positions(Node* nd_west, Node* nd_east) {
	if (nd_west && nd_east) {

		// cout 	<< "west" << "(" << nd_west->upLeft.first << ", " << nd_west->upLeft.second <<")" << " "
		// 		<< "(" << nd_west->lowRight.first << ", " << nd_west->lowRight.second << ")" << " east" << "(" << nd_east->upLeft.first << ", " << nd_east->upLeft.second <<")" << " "
		// 		<< "(" << nd_east->lowRight.first << ", " << nd_east->lowRight.second << ") --> ";
		
		int west_width = nd_west->lowRight.first - nd_west->upLeft.first + 1;
		int east_width = nd_east->lowRight.first - nd_east->upLeft.first + 1;
		
		int s = nd_east->upLeft.first -	west_width;
		int q = nd_east->lowRight.first - west_width;

		nd_east->upLeft.first = (uint) s;
		nd_east->lowRight.first = (uint) q;

		nd_west->upLeft.first += east_width;
		nd_west->lowRight.first += east_width;

		// cout 	<< "west" << "(" << nd_west->upLeft.first << ", " << nd_west->upLeft.second <<")" << " "
		// 		<< "(" << nd_west->lowRight.first << ", " << nd_west->lowRight.second << ")" << " east" << "(" << nd_east->upLeft.first << ", " << nd_east->upLeft.second <<")" << " "
		// 		<< "(" << nd_east->lowRight.first << ", " << nd_east->lowRight.second << ")" << endl;
		
	}
}

void QTree::update_child_positions(Node* nd) {

	pair<uint, uint> lr = nd->lowRight;
	pair<uint, uint> ul = nd->upLeft;

	uint width = lr.first - ul.first + 1;
	uint height = lr.second - ul.second + 1;

	uint west_width = ceil((float)width/2.0);
	uint east_width = floor((float)width/2.0);
	uint north_height = ceil((float)height/2.0);
	uint south_height = floor((float)height/2.0);

	pair<uint, uint> nw_ul = ul;
	pair<uint, uint> nw_lr(west_width+nw_ul.first-1, north_height+nw_ul.second-1);

	pair<uint, uint> ne_ul(nw_lr.first+1, nw_ul.second);
	pair<uint, uint> ne_lr(east_width+ne_ul.first-1, north_height+ne_ul.second-1);

	pair<uint, uint> sw_ul(nw_ul.first, nw_lr.second+1);
	pair<uint, uint> sw_lr(west_width+sw_ul.first-1, south_height+sw_ul.second-1);

	pair<uint, uint> se_ul(sw_lr.first+1, sw_ul.second);
	pair<uint, uint> se_lr(east_width+se_ul.first-1, south_height+se_ul.second-1);

	if (nd->NW) {
		nd->NW->upLeft = nw_ul;
		nd->NW->lowRight = nw_lr;
	}
	if (nd->NE) {
		nd->NE->upLeft = ne_ul;
		nd->NE->lowRight = ne_lr;
	}
	if (nd->SW) {
		nd->SW->upLeft = sw_ul;
		nd->SW->lowRight = sw_lr;
	}
	if (nd->SE) {
		nd->SE->upLeft = se_ul;
		nd->SE->lowRight = se_ul;
	}
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
	PNG current = Render(1); // save
	Clear(); // clear current tree
	PNG rotated = rotate(current); // flip
	pair<uint, uint> ul(0, 0);
	pair<uint, uint> lr(rotated.width()-1,rotated.height()-1);
	this->root = BuildNode(rotated, ul, lr); // build new tree

	this->width = rotated.width();
	this->height = rotated.height();
}

/**
 * Destroys all dynamically allocated memory associated with the
 * current QTree object. Complete for PA3.
 * You may want a recursive helper function for this one.
 */
void QTree:: Clear() {
	// ADD YOUR IMPLEMENTATION BELOW
	clear(this->root);
}

/**
 * Copies the parameter other QTree into the current QTree.
 * Does not free any memory. Called by copy constructor and operator=.
 * You may want a recursive helper function for this one.
 * @param other The QTree to be copied.
 */
void QTree::Copy(const QTree& other) {

	this->width = other.width;
	this->height = other.height;
	this->root = r_copy(other.root);

}

Node*  QTree::r_copy(Node* other_nd) {
	if (other_nd) {
		Node* nd_copy = new Node(other_nd->upLeft, other_nd->lowRight, other_nd->avg);

		nd_copy->NW = r_copy(other_nd->NW);
		nd_copy->NE = r_copy(other_nd->NE);
		nd_copy->SW = r_copy(other_nd->SW);
		nd_copy->SE = r_copy(other_nd->SE);

		return nd_copy;
	}
	return nullptr;
}


/**
 * Private helper function for the constructor. Recursively builds
 * the tree according to the specification of the constructor.
 * @param img reference to the original input image.
 * @param ul upper left point of current node's rectangle.
 * @param lr lower right point of current node's rectangle.
 */
Node* QTree::BuildNode(const PNG& img, pair<unsigned int, unsigned int> ul, pair<unsigned int, unsigned int> lr) {
	
	uint width = lr.first - ul.first + 1;
	uint height = lr.second - ul.second + 1;
	uint total_area = width * height;

	RGBAPixel a(100, 20, 20); // average color is initially uninitialized
	Node* nd = new Node(ul, lr, a); // make partition node, initially empty average
	nd->upLeft = ul;	// add upper left
	nd->lowRight = lr; 	// add lower right
	
	uint west_width = ceil((float)width/2.0);
	uint east_width = floor((float)width/2.0);
	uint north_height = ceil((float)height/2.0);
	uint south_height = floor((float)height/2.0);

	pair<uint, uint> nw_ul = ul;
	pair<uint, uint> nw_lr(west_width+nw_ul.first-1, north_height+nw_ul.second-1);

	pair<uint, uint> ne_ul(nw_lr.first+1, nw_ul.second);
	pair<uint, uint> ne_lr(east_width+ne_ul.first-1, north_height+ne_ul.second-1);

	pair<uint, uint> sw_ul(nw_ul.first, nw_lr.second+1);
	pair<uint, uint> sw_lr(west_width+sw_ul.first-1, south_height+sw_ul.second-1);

	pair<uint, uint> se_ul(sw_lr.first+1, sw_ul.second);
	pair<uint, uint> se_lr(east_width+se_ul.first-1, south_height+se_ul.second-1);

	// BASE CASE, no more recursion
	if (width == 1 && height == 1) {
		nd->avg = *img.getPixel(lr.first, lr.second); // avg is just pixel
	}
	else if (width == 1) {
		nd->NW = BuildNode(img, nw_ul, nw_lr);
		nd->NE = nullptr;
		nd->SW = BuildNode(img, sw_ul, sw_lr);
		nd->SE = nullptr;
		
		RGBAPixel nw_pixel = nd->NW->avg;
		uint nw_area = north_height * west_width;
		RGBAPixel sw_pixel = nd->SW->avg;
		uint sw_area = south_height * west_width;

		float r_avg = (float) (1.0/total_area) * (nw_area * nw_pixel.r + sw_area * sw_pixel.r);
		float g_avg = (float) (1.0/total_area) * (nw_area * nw_pixel.g + sw_area * sw_pixel.g);
		float b_avg = (float) (1.0/total_area) * (nw_area * nw_pixel.b + sw_area * sw_pixel.b);

		nd->avg.r = (uint) r_avg;
		nd->avg.g = (uint) g_avg;
		nd->avg.b = (uint) b_avg;
	}
	else if (height == 1) {
		nd->NW = BuildNode(img, nw_ul, nw_lr);
		nd->NE = BuildNode(img, ne_ul, ne_lr);
		nd->SW = nullptr;
		nd->SE = nullptr;

		RGBAPixel nw_pixel = nd->NW->avg;
		uint nw_area = north_height * west_width;
		RGBAPixel ne_pixel = nd->NE->avg;
		uint ne_area = north_height * east_width;

		float r_avg = (float) (1.0/total_area) * (nw_area * nw_pixel.r + ne_area * ne_pixel.r);
		float g_avg = (float) (1.0/total_area) * (nw_area * nw_pixel.g + ne_area * ne_pixel.g);
		float b_avg = (float) (1.0/total_area) * (nw_area * nw_pixel.b + ne_area * ne_pixel.b);
		
		nd->avg.r = (uint) r_avg;
		nd->avg.g = (uint) g_avg;
		nd->avg.b = (uint) b_avg;
	}
	else {
		nd->NW = BuildNode(img, nw_ul, nw_lr);
		nd->NE = BuildNode(img, ne_ul, ne_lr);
		nd->SW = BuildNode(img, sw_ul, sw_lr);
		nd->SE = BuildNode(img, se_ul, se_lr);

		RGBAPixel nw_pixel = nd->NW->avg;
		uint nw_area = north_height * west_width;
		RGBAPixel ne_pixel = nd->NE->avg;
		uint ne_area = north_height * east_width;
		RGBAPixel sw_pixel = nd->SW->avg;
		uint sw_area = south_height * west_width;
		RGBAPixel se_pixel = nd->SE->avg;
		uint se_area = south_height * east_width;

		float r_avg = (float) (1.0/total_area) * (nw_area * nw_pixel.r + ne_area * ne_pixel.r + 
												sw_area * sw_pixel.r + se_area * se_pixel.r);
		float g_avg = (float) (1.0/total_area) * (nw_area * nw_pixel.g + ne_area * ne_pixel.g + 
												sw_area * sw_pixel.g + se_area * se_pixel.g);
		float b_avg = (float) (1.0/total_area) * (nw_area * nw_pixel.b + ne_area * ne_pixel.b + 
												sw_area * sw_pixel.b + se_area * se_pixel.b);

		nd->avg.r = (uint) r_avg;
		nd->avg.g = (uint) g_avg;
		nd->avg.b = (uint) b_avg;
	}

	return nd;
}

/*********************************************************/
/*** IMPLEMENT YOUR OWN PRIVATE MEMBER FUNCTIONS BELOW ***/
/*********************************************************/

void QTree::clear(Node* nd) {
	// if node is not null
	if (nd) {
		// delete available children
		if (nd->NW) clear(nd->NW);
		if (nd->NE) clear(nd->NE);
		if (nd->SW) clear(nd->SW);
		if (nd->SE) clear(nd->SE);

		// then delete node
		delete nd;
	}
	
}


PNG QTree::hflip(PNG & img) {

	uint width = img.width();
	uint height = img.height();

	for (uint y=0; y < height; y++) {
		for (uint x=0; x < width/2; x++) {
			int start = x;
			int end = width - 1 - x;

			RGBAPixel temp(0, 0, 0);
			RGBAPixel* px1 = img.getPixel(start, y);
			RGBAPixel* px2 = img.getPixel(end, y);

			temp.r = px1->r;
			temp.g = px1->g;
			temp.b = px1->b;

			px1->r = px2->r;
			px1->g = px2->g;
			px1->b = px2->b;

			px2->r = temp.r;
			px2->g = temp.g;
			px2->b = temp.b;			

		}
	}

	return img;
}

PNG QTree::rotate(PNG & img) {
	uint width = img.width();
	uint height = img.height();

	PNG rotated(height, width); // swap

	for (uint y=0; y < height; y++) {
		for (uint x=0; x < width; x++) {
			RGBAPixel* src_pixel = img.getPixel(x, y);

			RGBAPixel* dest_pixel = rotated.getPixel(y, width-x-1);

			dest_pixel->r = src_pixel->r;
			dest_pixel->g = src_pixel->g;
			dest_pixel->b = src_pixel->b;
		}
	}

	return rotated;
}