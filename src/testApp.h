#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxGLWarper.h"
#include "ofxOpenCv.h"
#include "ofxCvContourSimplify.h"


class testApp : public ofSimpleApp{

	public:

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();

		ofxGLWarper warper;

		ofImage img;


        ofVideoGrabber cam;
        ofxCvColorImage camColor, cleeseImg;
        ofxCvGrayscaleImage camGray, lightGray;
        ofxCvGrayscaleImage  bgImg;
        ofxCvGrayscaleImage  diffImg, blobImg;
        ofxCvColorImage     maskResult, lightImg;

        ofxCvContourFinder blobFinder;
        ofxCvContourSimplify   blobSimple;

        vector<ofPoint>    simplePoints;

        ofFbo               lightFbo;


        bool doGrab;

        int thresh;
        float scaleAmnt;

      ofPixels  pixels;

};

#endif

