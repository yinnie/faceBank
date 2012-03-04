#include "testApp.h"

using namespace ofxCv;

void testApp::setup() {
	ofSetVerticalSync(true);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
	cam.listDevices();
	cam.setDeviceID(5);
	cam.initGrabber(640, 480);
	ofEnableAlphaBlending();
	
	camTracker.setup();

	posX=ofGetWindowWidth()/2;
	posY=ofGetWindowHeight()/2;
	
	imageCounter =1;
	imgID=1;
	
	
	//face tracker on source
	srcTracker.setup();
	srcTracker.setRescale(.5);
	
	srcTracker.setup();
    srcTracker.setIterations(25);
    srcTracker.setAttempts(2);
    srcTracker.setClamp(4);
    srcTracker.setTolerance(.01);
    
	srcImg.loadImage("face.jpeg");
    srcTracker.update(toCv(srcImg));
    srcPoints = srcTracker.getImagePoints();
	//************************************
#ifdef _USE_LIVE_VIDEO
	vidGrabber.setVerbose(true);
	vidGrabber.setDeviceID(4);
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

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		camTracker.update(toCv(cam));
		
		scale = camTracker.getScale();
		orientation = camTracker.getOrientation();
		rotationMatrix = camTracker.getRotationMatrix();
	}
	
	//*****************************************
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
					imgID = imageCounter-1;
					circleColor1 = 255;
					cout << leftBowl << endl;
				}
			    else if (contourFinder.blobs[i].centroid.x > border) {
					leftBowl= false;
					imgID = imageCounter-2;
					circleColor2 = 255;
					cout << leftBowl<<endl;
				}
			}
		}
		
	}
	
}

void testApp::draw() {
	ofSetColor(255);
	cam.draw(0, 0);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
	
	srcImg.draw(0,0, 100,100); //draw srcimg thumbnail
	
	if(camTracker.getFound()) {
		
		ofMesh camMesh = camTracker.getImageMesh();
				
		ofMesh objectMesh = camTracker.getObjectMesh();
		
		//move the object mesh to the corner of the screen
		cam.getTextureReference().bind();
		ofPushMatrix();
		ofSetupScreenOrtho(640, 480, OF_ORIENTATION_UNKNOWN, true, -1000, 1000);
		ofTranslate(posX,posY);
		applyMatrix(rotationMatrix);
        ofScale(3.3,3.3,3.3);
        ofDrawAxis(3.3);		
		objectMesh.draw();
		ofPopMatrix();
		cam.getTextureReference().unbind();
		
		//substitute the imageMesh with srcImg
		camMesh.clearTexCoords();
		camMesh.addTexCoords(srcPoints);
		srcImg.getTextureReference().bind();
		ofSetupScreenOrtho(640, 480, OF_ORIENTATION_UNKNOWN, true, -1000, 1000);
		camMesh.draw();
		srcImg.getTextureReference().unbind();
		
		
		/*
		 leftEye = tracker.getImageFeature(ofxFaceTracker::EYE_LEFT);
		 mouth = tracker.getImageFeature(ofxFaceTracker::OUTER_MOUTH);
		 */
	    mouth.setClosed(true);
        ofPushStyle();
        ofFill();
        ofSetColor(60,0,0);
        ofBeginShape();
        for(int i =0; i<mouth.size(); i++){
            ofVertex(mouth[i]);
        }
        ofEndShape(true);
        ofPopStyle();  
		mouth.draw();
	}
	
   
	if (posY < 400 && posX >0 ) {
		posY ++;
		posX --;}
	else {
		posY = 400; 
		posX = 50;	
	}
		
	/* distort the mouth 
	ofSetLineWidth(1);
	tracker.draw();
	
	ofPolyline leftEye = tracker.getImageFeature(ofxFaceTracker::LEFT_EYE);
	ofPolyline rightEye = tracker.getImageFeature(ofxFaceTracker::RIGHT_EYE);
	ofPolyline faceOutline = tracker.getImageFeature(ofxFaceTracker::FACE_OUTLINE);
	
	ofSetLineWidth(2);
	ofSetColor(ofColor::red);
	leftEye.draw();
	ofSetColor(ofColor::green);
	rightEye.draw();
	ofSetColor(ofColor::blue);
	faceOutline.draw();
	
	 if(tracker.getFound()) {
	 ofSetColor(255);
	 bool inside = faceOutline.inside(mouseX, mouseY);
	 ofDrawBitmapString(inside ? "inside" : "outside", 10, 40);
	 }
	*/
     
	//********************************bowls
	ofSetColor(circleColor1,0,0);
	ofCircle(440, 350, 40);
	ofSetColor(circleColor2, 0,0);
	ofCircle(580, 350, 40);
	
	//draw the blobs
	for (int i = 0; i < contourFinder.nBlobs; i++){
        contourFinder.blobs[i].draw(300,200);
    }
	
}

void testApp::keyPressed(int key) {
	
	switch (key) {
	   case 'r':
			camTracker.reset();
			posX =0;
			posY =0;
			break;
		case 'l':
		    srcImg.loadImage(ofToString(imgID) + ".png");
			//cout << imageCounter;
			srcTracker.update(toCv(srcImg));
			srcPoints = srcTracker.getImagePoints();
			break;
		case 's':
		    srcImg.grabScreen(200,0,(ofGetWindowWidth()-200),ofGetWindowHeight());
			srcImg.saveImage(ofToString(imageCounter) + ".png");
			imageCounter ++;
			break;
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


