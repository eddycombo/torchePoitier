#include "testApp.h"
#include "stdio.h"

//--------------------------------------------------------------
void testApp::setup(){
	//we run at 60 fps!
	warper.setup(1024, 768); //initializates ofxGLWarper
	warper.activate();// this allows ofxGLWarper to automatically listen to the mouse and keyboard input and updates automatically it's matrixes.
	ofSetVerticalSync(true);

	//lets load a test image too
	img.loadImage("image.jpg");
	cam.setDeviceID(1);
	cam.initGrabber(640, 480);

    thresh = 43;
    camColor.allocate(640, 480);
    camGray.allocate(640, 480);
    maskResult.allocate(640, 480);
    cleeseImg.allocate(640, 480);
    cleeseImg.setFromPixels(img.getPixels(), 640, 480);
    bgImg.allocate(640, 480);
    diffImg.allocate(640, 480);
    blobImg.allocate(640, 480);

    scaleAmnt = 1.0;

    lightFbo.allocate(640, 480, GL_RGB);
    lightImg.allocate(640, 480);
    pixels.allocate(640, 480, 3);
    lightGray.allocate(640, 480);

    }

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(20, 20, 20);


    cam.grabFrame();

    if(cam.isFrameNew()){


        camColor.setFromPixels(cam.getPixels(), 640, 480);
        camGray = camColor;

        if(doGrab){

            bgImg = camGray;
            doGrab = false;

        }

        diffImg.absDiff(bgImg, camGray);

        diffImg.threshold(thresh);

        camGray.threshold(thresh);
        //camGray.blur();

        blobFinder.findContours(camGray, 100, (640*480)*0.25, 1, true);

        if(blobFinder.blobs.size()>0){
            blobSimple.simplify(blobFinder.blobs[0].pts, simplePoints, 0.0);
            blobSimple.smooth(simplePoints, simplePoints, 0.9, 0.0);
        }


        lightFbo.begin();

        ofFill();
        ofSetColor(0, 0, 0);
        ofRect(0, 0, 640, 480);

        glPushMatrix();
            if(blobFinder.blobs.size()>0){
                ofTranslate(blobFinder.blobs[0].centroid.x, blobFinder.blobs[0].centroid.y, 0);
                ofScale(scaleAmnt, scaleAmnt, 0);
                ofSetColor(255, 255, 255);
                ofBeginShape();
                    for(int i=0; i<simplePoints.size(); i++) {

                        ofFill();
                        ofVertex(simplePoints[i].x - blobFinder.blobs[0].centroid.x, simplePoints[i].y - blobFinder.blobs[0].centroid.y);

                }
                ofEndShape(true);
                ofSetColor(255, 255, 255);


		}
		glPopMatrix();

		lightFbo.end();

            ofTexture tex = lightFbo.getTextureReference();

            tex.readToPixels(pixels);

            lightImg.clear();
            lightImg.allocate(640, 480);
            lightImg.setFromPixels(pixels.getPixels(), 640, 480);
            lightImg.flagImageChanged();

            lightGray = lightImg;



        maskResult.clear();
        maskResult.allocate(640, 480);
        cvCopy(cleeseImg.getCvImage(), maskResult.getCvImage(),  lightGray.getCvImage());
        maskResult.flagImageChanged();

    }



}
//--------------------------------------------------------------
void testApp::draw(){

	//warper.begin();	///all the things that are drawn AFTER ofxGLWarper's draw method are afected by it.
					///el metodo draw de ofxGLWarper afecta a todos los elementos dibujados despues si.
	//warper.draw(); //when active draws a rectangle around the warper area.
	// -- NOW LETS DRAW!!!!!!  -----



	//camGray.draw(0, 0);
	//blobFinder.draw(0, 0);




		//warper.end();
    lightFbo.draw(0, 0);
    maskResult.draw(640, 0,  640, 480);

    ofDrawBitmapString(ofToString(thresh), 0, 500);

    if(blobFinder.blobs.size()>0){
        ofDrawBitmapString(ofToString(blobFinder.blobs[0].pts.size()), 0, 540);
    }
    ofDrawBitmapString(ofToString(simplePoints.size()), 30, 540);
     ofDrawBitmapString(ofToString(scaleAmnt), 60, 540);
}
//--------------------------------------------------------------
void testApp::keyPressed(int key){
switch (key) {
	case ' ':
		if (warper.isActive()) {
			warper.deactivate(); //once you are done with the Wrapping you should call this method, so it realeses the keyboard and mouse and stop processing the transformation matrixes.
								 // this will reduce the amount of precessing needed.
		}else {
			warper.activate();
		}

		break;
		case 'b':
            doGrab = true;
        break;
        case 'a':
            thresh++;
        break;
        case 'q':
            if(thresh == 0){

                thresh = 0;
            }else{thresh = thresh - 1;}
            break;

        case 'z':
            scaleAmnt = scaleAmnt+0.01;
        break;
        case 's':
            if(scaleAmnt==0.1){
                scaleAmnt = 0.1;

            }else{
                scaleAmnt = scaleAmnt-0.01;
            }
            break;
	default:
		break;

}
}
//--------------------------------------------------------------
void testApp::keyReleased(int key){}
//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){}
//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){}
//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){}
//--------------------------------------------------------------
void testApp::mouseReleased(){}
