#pragma once

#include "ofMain.h"
#include "ofxUI.h"

class ofApp : public ofBaseApp{

private:
    ofxUICanvas *gui;
    bool hideButtonReleased = false;
    
    ofVideoGrabber grabber;
    int selectedCameraIndex = 0;
    
    ofImage screenImage;
    int x, y;
    int pixelsPerFrame = 10;
    
    int cameraWidth = 640;
    int cameraHeight = 480;
    
    float lastTimeImageWasSaved = 0;
    int intervalToSaveImage = 15;
    
    const float MAX_STRENGTH_AROUND_PIXEL = .15;
    const string CAMERA_WIDTH_LABEL = "Largura da câmera"; // "Camera width"
    const string CAMERA_HEIGHT_LABEL = "Altura da câmera"; // "Camera height"
    const string PIXELS_PER_FRAME_LABEL = "Pixels por frame (velocidade)"; // "Pixels per frame (speed)
    
    ofxUIDropDownList* cameraList;
    ofxUITextInput* cameraWidthTextInput;
    ofxUITextInput* cameraHeightTextInput;
    ofxUIIntSlider* pixelsPerFrameSlider;
    
    void paintPixel( int pixelX, int pixelY, ofPixels pixels, float strength );

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    // ofxUI
    void exit();
    void guiEvent(ofxUIEventArgs &e);
		
};
