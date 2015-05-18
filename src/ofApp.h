#pragma once

#include "ofMain.h"
#include "ofxUI.h"

class ofApp : public ofBaseApp{

private:
    ofxUICanvas *gui;
    ofxUICanvas *cameraPanel;
    ofxUICanvas *imagePanel;

    bool hideButtonReleased = false;

    ofVideoGrabber grabber;
    int selectedCameraIndex = 0;

    ofImage screenImage;
    int x, y;

    int cameraWidth = 640;
    int cameraHeight = 480;

    float lastTimeImageWasSaved = 0;

    const float MAX_STRENGTH_AROUND_PIXEL = .15;
    const static string CAMERA_WIDTH_LABEL;
    const static string CAMERA_HEIGHT_LABEL;
    const static string PIXELS_PER_FRAME_LABEL;
    const static string SAVE_IMAGE_LABEL;
    const static string SAVE_LABEL;
    const static string HIDE_LABEL;
    const static string RESET_IMAGE_LABEL;
    const static string SUPPORT_BUTTON_NAME;

    ofxUISpacer* titleSpacer;
    ofxUIDropDownList* cameraList;
    ofxUITextInput* cameraWidthTextInput;
    ofxUITextInput* cameraHeightTextInput;
    ofxUIIntSlider* pixelsPerFrameSlider;
    ofxUIIntSlider* intervalToSaveSlider;

    void paintPixel( int pixelX, int pixelY, ofPixels pixels, float strength );
    void fillImageWithWhite( ofImage* image );
    void saveCurrentImageWithTimestap();

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
    void cameraPanelEvent(ofxUIEventArgs &e);
    void imagePanelEvent(ofxUIEventArgs &e);

};
