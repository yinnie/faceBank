#include "testApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 2560, 800, OF_FULLSCREEN);
	ofRunApp(new testApp());
}
