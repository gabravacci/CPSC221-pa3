/**
 * @file main.cpp
 * @description basic test cases for QTree
 *              CPSC 221 PA3
 * 
 *              You may add your own tests to this file
 *
 *              THIS FILE WILL NOT BE SUBMITTED
 */

#include <iostream>
#include <string>

#include "qtree.h"

using namespace std;

/**********************************/
/*** TEST FUNCTION DECLARATIONS ***/
/**********************************/
void ColorTest(int scale);
void TestBuildRender(unsigned int scale);
void TestFlipHorizontal();
void TestRGBFlip(string file);
void TestRotateCCW();
void TestPrune(double tol);
void CopyTest();

/***********************************/
/*** MAIN FUNCTION PROGRAM ENTRY ***/
/***********************************/

int main(int argc, char* argv[]) {

	// TestBuildRender(1);
	// TestBuildRender(6);
	
	TestFlipHorizontal();
	
	// TestRotateCCW();
	// CopyTest();

	TestRGBFlip("t1");
	TestRGBFlip("r2");
	TestRGBFlip("t2");
	TestRGBFlip("t3");

	// TestPrune(0.01);
	// TestPrune(0.05);

	return 0;
}

/*************************************/
/*** TEST FUNCTION IMPLEMENTATIONS ***/
/*************************************/

void ColorTest(int scale) {
	PNG input;
	input.readFromFile("images-original/frame.png");

	QTree t(input);
	PNG output = t.Render(scale);
	string outfilename = "images-output/frame-png_x" + to_string(scale) + ".png";
	output.writeToFile(outfilename);
}

void CopyTest() {
	PNG input;
	input.readFromFile("images-original/malachi-60x87.png");
	QTree t1(input);

	QTree t2(t1);
	PNG output = t2.Render(1);
	output.writeToFile("images-output/out-copy.png");
}

void TestBuildRender(unsigned int scale) {
	cout << "Entered TestBuildRender, scale: " << scale << endl;

	// read input PNG
	PNG input;
	input.readFromFile("images-original/malachi-60x87.png");

	cout << "Constructing QTree from image... ";
	QTree t(input);
	cout << "done." << endl;

	cout << "Rendering tree to PNG at x" << scale << " scale... ";
	PNG output = t.Render(scale);
	cout << "done." << endl;

	// write output PNG
	string outfilename = "images-output/malachi-render_x" + to_string(scale) + ".png";
	cout << "Writing rendered PNG to file... ";
	output.writeToFile(outfilename);
	cout << "done." << endl;

	PNG soln;
	string solnname = "images-soln/soln-malachi-render_x" + to_string(scale) + ".png";
    soln.readFromFile(solnname);
    cout << "RESULT: " << (output == soln) << endl;

	cout << "Exiting TestBuildRender.\n" << endl;
}


void TestRGBFlip(string file) {
	cout << "flipping " << file << endl;

	// read input PNG
	PNG input;
	input.readFromFile("test/"+file+".png");

	QTree t(input);


	t.FlipHorizontal();

	PNG output = t.Render(1);

	// write output PNG
	string outfilename = "test/"+file+"-flip.png";
	output.writeToFile(outfilename);
}

void TestFlipHorizontal() {
	cout << "Entered TestFlipHorizontal" << endl;

	// read input PNG
	PNG input;
	input.readFromFile("images-original/malachi-60x87.png");

	cout << "Constructing QTree from image... ";
	QTree t(input);
	cout << "done." << endl;

	cout << "Calling FlipHorizontal... ";
	t.FlipHorizontal();
	cout << "done." << endl;

	cout << "Rendering tree to PNG at x1 scale... ";
	PNG output = t.Render(1);
	cout << "done." << endl;

	// write output PNG
	string outfilename = "images-output/malachi-fliphorizontal_x1-render_x1.png";
	cout << "Writing rendered PNG to file... ";
	output.writeToFile(outfilename);
	cout << "done." << endl;

	PNG soln;
	string solnname = "images-soln/soln-malachi-fliphorizontal_x1-render_x1.png";
    soln.readFromFile(solnname);
    cout << "RESULT: " << (output == soln) << endl;

	cout << "Calling FlipHorizontal a second time... ";
	t.FlipHorizontal();
	cout << "done." << endl;

	cout << "Rendering tree to PNG at x1 scale... ";
	output = t.Render(1);
	cout << "done." << endl;

	// write output PNG
	outfilename = "images-output/malachi-fliphorizontal_x2-render_x1.png";
	cout << "Writing rendered PNG to file... ";
	output.writeToFile(outfilename);
	cout << "done." << endl;

	solnname = "images-soln/soln-malachi-fliphorizontal_x2-render_x1.png";
    soln.readFromFile(solnname);
    cout << "RESULT: " << (output == soln) << endl;

	cout << "Exiting TestFlipHorizontal.\n" << endl;
}

void TestRotateCCW() {
	cout << "Entered TestRotateCCW" << endl;

	// read input PNG
	PNG input;
	input.readFromFile("images-original/malachi-60x87.png");

	cout << "Constructing QTree from image... ";
	QTree t(input);
	cout << "done." << endl;

	cout << "Calling RotateCCW... ";
	t.RotateCCW();
	cout << "done." << endl;

	cout << "Rendering tree to PNG at x1 scale... ";
	PNG output = t.Render(1);
	cout << "done." << endl;

	// write output PNG
	string outfilename = "images-output/malachi-rotateccw_x1-render_x1.png";
	cout << "Writing rendered PNG to file... ";
	output.writeToFile(outfilename);
	cout << "done." << endl;

	cout << "Calling RotateCCW a second time... ";
	t.RotateCCW();
	cout << "done." << endl;

	cout << "Rendering tree to PNG at x1 scale... ";
	output = t.Render(1);
	cout << "done." << endl;

	// write output PNG
	outfilename = "images-output/malachi-rotateccw_x2-render_x1.png";
	cout << "Writing rendered PNG to file... ";
	output.writeToFile(outfilename);
	cout << "done." << endl;

	cout << "Calling RotateCCW a third time... ";
	t.RotateCCW();
	cout << "done." << endl;

	cout << "Rendering tree to PNG at x1 scale... ";
	output = t.Render(1);
	cout << "done." << endl;

	// write output PNG
	outfilename = "images-output/malachi-rotateccw_x3-render_x1.png";
	cout << "Writing rendered PNG to file... ";
	output.writeToFile(outfilename);
	cout << "done." << endl;

	cout << "Calling RotateCCW a fourth time... ";
	t.RotateCCW();
	cout << "done." << endl;

	cout << "Rendering tree to PNG at x1 scale... ";
	output = t.Render(1);
	cout << "done." << endl;

	// write output PNG
	outfilename = "images-output/malachi-rotateccw_x4-render_x1.png";
	cout << "Writing rendered PNG to file... ";
	output.writeToFile(outfilename);
	cout << "done." << endl;

	cout << "Exiting TestRotateCCW.\n" << endl;
}

void TestPrune(double tol) {
	cout << "Entered TestPrune, tolerance: " << tol << endl;

	// read input PNG
	PNG input;
	input.readFromFile("images-original/kkkk_nnkm-256x224.png");

	cout << "Constructing QTree from image... ";
	QTree t(input);
	cout << "done." << endl;

	cout << "Tree contains " << t.CountNodes() << " nodes and " << t.CountLeaves() << " leaves." << endl;

	cout << "Calling Prune... ";
	t.Prune(tol);
	cout << "done." << endl;

	cout << "Pruned tree contains " << t.CountNodes() << " nodes and " << t.CountLeaves() << " leaves." << endl;

	cout << "Rendering tree to PNG at x1 scale... ";
	PNG output = t.Render(1);
	cout << "done." << endl;

	// write output PNG
	string outfilename = "images-output/kkkk_nnkm-256x224-prune_" + to_string(tol) + "-render_x1.png";
	cout << "Writing rendered PNG to file... ";
	output.writeToFile(outfilename);
	cout << "done." << endl;

	cout << "Exiting TestPrune.\n" << endl;
}