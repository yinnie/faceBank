#pragma once

#include "ofMain.h"
#include "ofxCv.h"

#include "ofxOpenCv.h"
#define _USE_LIVE_VIDEO	 // uncomment this to use a live camera

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
	bool substitute;
	
	ofImage img1, img2;
	
	//handDetection
#ifdef _USE_LIVE_VIDEO
	ofVideoGrabber 		vidGrabber;
#else
	ofVideoPlayer 		vidPlayer;
#endif
	
	ofxCvColorImage			colorImg;
	
	ofxCvGrayscaleImage 	grayImage;
	ofxCvGrayscaleImage 	grayBg;
	ofxCvGrayscaleImage 	grayDiff;
	
	ofxCvContourFinder 	contourFinder;
	
	int 				threshold;
	bool				bLearnBakground;
	
	bool leftBowl;
	bool rightBowl;
	int circleColor1;
	int circleColor2, border;
	
	
};
