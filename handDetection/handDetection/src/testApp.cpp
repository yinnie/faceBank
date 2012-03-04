#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	#ifdef _USE_LIVE_VIDEO
        vidGrabber.setVerbose(true);
        vidGrabber.initGrabber(320,240);
	#else
        vidPlayer.loadMovie("fingers.mov");
        vidPlayer.play();
	#endif

    colorImg.allocate(320,240);
	grayImage.allocate(320,240);
	grayBg.allocate(320,240);
	grayDiff.allocate(320,240);

	bLearnBakground = true;
	threshold = 80;
	leftBowl = false;
	rightBowl = false;
	circleColor1 = 0;
	circleColor2=0;
	border = 190;
	
}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(100,100,100);
	circleColor1 = 0;
	circleColor2=0;
	leftBowl = false;

    bool bNewFrame = false;

	#ifdef _USE_LIVE_VIDEO
       vidGrabber.grabFrame();
	   bNewFrame = vidGrabber.isFrameNew();
    #else
        vidPlayer.idleMovie();
        bNewFrame = vidPlayer.isFrameNew();
	#endif

	if (bNewFrame){

		#ifdef _USE_LIVE_VIDEO
            colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);
	    #else
            colorImg.setFromPixels(vidPlayer.getPixels(), 320,240);
        #endif

        grayImage = colorImg;
		if (bLearnBakground == true){
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}

		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);

		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 50, (340*240)/3, 1, true);	// find holes
		
		for (int i = 0; i < contourFinder.nBlobs; i++){
			cout << contourFinder.blobs[i].centroid.x << endl;
			
			if(contourFinder.nBlobs>0) {
				if (contourFinder.blobs[i].centroid.x< border) {
					leftBowl = true;
					circleColor1 = 255;
					cout << leftBowl << endl;
				}
			    else if (contourFinder.blobs[i].centroid.x > border) {
					leftBowl= false;
					circleColor2 = 255;
					cout << leftBowl<<endl;
				}
			}
		}
	
	}

}

//--------------------------------------------------------------
void testApp::draw(){


	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetHexColor(0xffffff);
	colorImg.draw(20,20);
	grayImage.draw(360,20);
	grayBg.draw(20,280);
	grayDiff.draw(360,280);
	
	ofSetColor(circleColor1,0,0);
	ofCircle(440, 350, 40);
	ofSetColor(circleColor2, 0,0);
	ofCircle(580, 350, 40);
	

	// then draw the contours:
    /*
	ofFill();
	ofSetHexColor(0x333333);
	ofRect(360,540,320,240);
	 
	ofSetHexColor(0xffffff);
	*/
	// we could draw the whole contour finder
	//contourFinder.draw(240,540);  

	// or, instead we can draw each blob individually,
	// this is how to get access to them:
	ofSetColor(255,255,255);
    for (int i = 0; i < contourFinder.nBlobs; i++){
        contourFinder.blobs[i].draw(360,540);
    }

	// finally, a report:

	ofSetHexColor(0xffffff);
	char reportStr[1024];
	sprintf(reportStr, "bg subtraction and blob detection\npress ' ' to capture bg\nthreshold %i (press: +/-)\nnum blobs found %i, fps: %f", threshold, contourFinder.nBlobs, ofGetFrameRate());
	ofDrawBitmapString(reportStr, 20, 600);
	

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	switch (key){
		case ' ':
			bLearnBakground = true;
			break;
		case '+':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
