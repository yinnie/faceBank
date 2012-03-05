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
	
	imageCounter =4;
	imgID=1;
	substitute = false;
	resetting = true;
	savedtime =0;
	
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
	
	img1.loadImage(ofToString(imageCounter-2) + ".png");
	img2.loadImage(ofToString(imageCounter-3) + ".png");
	img3.loadImage(ofToString(imageCounter-4) + ".png");
	
	
	/*
	faces.allowExt("jpg");
	faces.allowExt("png");
	faces.listDir("faces");
	currentFace = 0;
	if(faces.size()!=0){
		loadFace(faces.getPath(currentFace));
	}
	 */
}

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		camTracker.update(toCv(cam));
		
		scale = camTracker.getScale();
		orientation = camTracker.getOrientation();
		rotationMatrix = camTracker.getRotationMatrix();
	}
	
	/*
	if (!camTracker.getFound() && substitute && resetting) {
	    savedtime =ofGetElapsedTimeMillis();
		resetting = false;
		cout << savedtime << endl;
	}

		int timePassed = ofGetElapsedTimeMillis()-savedtime;
		//cout << timePassed <<endl;

		if (timePassed >= 5000) {
			substitute = false;
			resetting = true;
		}
	 
	 //ofGetElapsedTimeMicros()...   
*/
	
}

void testApp::draw() {
	ofSetColor(255);
	cam.draw(0, 0);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
	
	
	srcImg.draw(0,0, 100,100); //draw srcimg thumbnail
	//srcImg.draw
		
	img1.draw(0,100,100,100);
	img2.draw(0,200,100,100);
	img3.draw(0,300,100,100);
	

	
	if(camTracker.getFound() && substitute ) {
		
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
     
		
}

void testApp::keyPressed(int key) {
	
	switch (key) {
	   case 'r':
			camTracker.reset();
			posX =0;
			posY =0;
			break;
		case 'l':
			substitute= true;
		    srcImg.loadImage(ofToString(imageCounter-1) + ".png");
						
			//cout << imageCounter;
			srcTracker.update(toCv(srcImg));
			srcPoints = srcTracker.getImagePoints();
			 
			//loadFace(currentFace);
			break;
		case 's':
		    srcImg.grabScreen(200,0,(ofGetWindowWidth()-200),ofGetWindowHeight());
			srcImg.saveImage(ofToString(imageCounter) + ".png");
			img1.loadImage(ofToString(imageCounter-2) + ".png");
			img2.loadImage(ofToString(imageCounter-3) + ".png");
			img3.loadImage(ofToString(imageCounter-4) + ".png");
			
			cout<<imageCounter<<" is the counter "<<endl;
			
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

void testApp :: mouseMoved(int mouseX, int mouseY) {
	if (mouseX < ofGetWindowWidth()/2) {
		imgID = imageCounter-1;
		//cout << imgID;
	}
	else if (mouseX >= ofGetWindowWidth()/2) {
		imgID = imageCounter-2;
		//cout << imgID;
	}
}
/*
void testApp::loadFace(string face){
	src.loadImage(face);
	if(src.getWidth() > 0) {
		srcTracker.update(toCv(src));
		srcPoints = srcTracker.getImagePoints();
	}
}
 */
