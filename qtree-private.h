/**
 * @file qtree-private.h
 * @description student declaration of private QTree functions
 *              CPSC 221 PA3
 *
 *              SUBMIT THIS FILE.
 * 
 *				Simply declare your function prototypes here.
 *              No other scaffolding is necessary.
 */

// begin your declarations below
void flip_horizontal(Node* nd);
void clear(Node* nd);
PNG hflip(PNG & img);
PNG rotate(PNG & img);
Node* r_copy(Node* nd);
void swap_positions(Node* nd_west, Node* nd_east);
void update_child_positions(Node* nd);