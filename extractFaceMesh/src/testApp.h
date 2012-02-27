#pragma once

#include "ofMain.h"
#include "ofxCv.h"

#include "ofxFaceTracker.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	
	ofVideoGrabber cam;
	ofxFaceTracker srcTracker;
	ofxFaceTracker camTracker;
	
	ofPolyline mouth;
	ofPolyline leftEye;
	
	ofImage srcImg;
	int imageCounter;
	vector<ofVec2f> srcPoints;
	vector<ofVec2f> camPoints;
	ofVec2f camPosition;
	
	int posX, posY;
	
	float scale;
	ofVec3f orientation;
	ofMatrix4x4 rotationMatrix;
	
	
};
