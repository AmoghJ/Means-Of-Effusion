#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(26.0f);
	ofSetVerticalSync(true);
	ofPoint iniPos(0,0,0); //the position is refering to the center of the grid
	ofPoint gridSize(2500, 2000, 1500);
	int gridResX = 90;
	int gridResY = 90;
	int gridResZ = 90;
	marchingCubes.init(iniPos, gridSize, gridResX, gridResY, gridResZ);
	glEnable(GL_DEPTH_TEST);
	metaBalls.resize(NUM_META_BALLS);
	for(int i=0; i<metaBalls.size(); i++){
		metaBalls[i].init(iniPos);
	}
    
    ofEnableLighting();
    light.enable();    
    light.setup();
    
	light.setSpecularColor(ofColor(255.0f));

	ofSetLogLevel(OF_LOG_VERBOSE);
	bSaveModel = false;

	fbo.allocate(2816, 1720, GL_RGB, 4);
	fbo.begin();
	ofClear(ofColor(26.0f));
	fbo.end();
}

//--------------------------------------------------------------
void testApp::update(){


	float counter = ((float)ofGetFrameNum()) * 0.01f;
	float threshold = (sin(counter)*0.5+0.5)*0.25+0.25;
	marchingCubes.resetIsoValues();
	ofPoint force;
	ofPoint gridCenter = marchingCubes.getGridPos();
	float randomStep = 150;


	for (int i = 0; i < metaBalls.size(); i++) {
		/*force.x = ofSignedNoise(i + counter + 10) * randomStep;
		force.y = ofSignedNoise(i+counter+20) * randomStep;
		force.z = ofSignedNoise(i+counter+30) * randomStep;
		force += (gridCenter - metaBalls[i]) * 0.006f;
		metaBalls[i].update(force, 0.8);*/
		metaBalls[i].set(ofVec3f(ofSignedNoise(i, counter*0.2f + 10)*650.0f, ofSignedNoise(i, counter*0.2f + 20)*650.0f, ofSignedNoise(i, counter*0.2f + 30)*650.0f));

		marchingCubes.addMetaBall(metaBalls[i], metaBalls[i].size);
	}
	
	marchingCubes.update(threshold, true);
	
	if(bSaveModel){
		bSaveModel = false;
		marchingCubes.saveModel("testBin.stl");
		marchingCubes.saveModel("testASCII.stl", true);
	}
}

//--------------------------------------------------------------
void testApp::draw(){
  
	fbo.begin();
	ofBackground(26.0f);

	ofPoint center(ofGetWidth(), ofGetHeight(), -1000);
	ofPoint gridSize = marchingCubes.getGridSize();
	float counter = (float)ofGetFrameNum();
   

	ofPushMatrix();
	ofTranslate(center.x, center.y, center.z);
	ofRotate(counter*0.05f, 1.0f, 0.0f, 0.0f);
	ofRotate(counter*0.1f, 0.0f, 1.0f, 0.0f);
    
	ofSetColor(255.0f, 103.0f, 0.0f);    
	//marchingCubes.debugDraw();
	//marchingCubes.draw();
	//marchingCubes.drawFilled();
	marchingCubes.drawWireFrame();
	ofPopMatrix();

	fbo.end();

	ofBackground(0.0f);

	fbo.draw(0.0f, 0.0f, ofGetWidth(), ofGetHeight());

	if (startRec) {
		ofPixels pixels;
		fbo.readToPixels(pixels);
		img.setFromPixels(pixels);

		img.save("output/render" + to_string(ofGetFrameNum()) + ".jpg");
	}

    /*ofSetColor(255.0f);
	string info = "fps:" + ofToString(ofGetLastFrameTime()) + 
	+ "\nnum triangles:" + ofToString(marchingCubes.getNumTriangles(), 0)
	+ "\nthreshold:" + ofToString(marchingCubes.getThreshold());
	ofDrawBitmapString(info, 20, 20);*/
}


//--------------------------------------------------------------
void testApp::keyReleased(int key){
	if(key == ' ')bSaveModel = true;
}

void testApp::keyPressed(int key)
{
	if (key == 'h') {
		startRec = true;
	}
}

void testApp::mouseMoved(int x, int y){}
void testApp::mouseDragged(int x, int y, int button){}
void testApp::mousePressed(int x, int y, int button){}
void testApp::mouseReleased(int x, int y, int button){}
void testApp::windowResized(int w, int h){}
void testApp::gotMessage(ofMessage msg){}
void testApp::dragEvent(ofDragInfo dragInfo){ }
