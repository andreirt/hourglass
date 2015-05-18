#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    this->gui = new ofxUICanvas(0, 0, ofGetWidth(), ofGetHeight());
    this->gui->setWidgetSpacing(15);
    gui->setFontSize(OFX_UI_FONT_SMALL, 8);
    gui->setFontSize(OFX_UI_FONT_LARGE, 24);

    ofxUIColor backgroundColor = ofxUIColor::lightCoral;
    ofxUIColor fillColor = ofxUIColor::black;
    ofxUIColor fillHightlightColor = ofxUIColor::black;
    ofxUIColor outline = ofxUIColor::black;
    ofxUIColor outlineHighlight = ofxUIColor::red;
    ofxUIColor paddedColor = ofxUIColor::blue;
    ofxUIColor paddedOutlineColor = ofxUIColor::orange;

    this->gui->setUIColors( backgroundColor, outline, outlineHighlight, fillColor, fillHightlightColor, paddedColor, paddedOutlineColor );

    this->gui->addLabel("title", "Ampulheta", OFX_UI_FONT_LARGE);
    this->gui->addSpacer();

    this->cameraPanel = new ofxUICanvas(0, 0, 400, 200);
    this->cameraPanel->setFontSize(OFX_UI_FONT_SMALL, 8);
    this->cameraPanel->setWidgetSpacing(15);
    this->gui->addWidgetDown(this->cameraPanel);

    // lista as câmeras conectadas a este computador
    vector< ofVideoDevice > devices = this->grabber.listDevices();

    vector<string> *cameras = new vector<string>();
    vector<ofVideoDevice>::iterator it;

    for (it = devices.begin(); it != devices.end(); ++it) {
        ofVideoDevice device = *it;
        cameras->push_back(device.deviceName);
    }

    this->cameraList = this->cameraPanel->addDropDownList("Escolha a câmera", *cameras, 300, 10);
    this->cameraList->setAllowMultiple(false);
    this->cameraList->setDrawOutline(true);

    this->cameraPanel->addWidgetDown( new ofxUILabel(160, ofApp::CAMERA_WIDTH_LABEL, OFX_UI_FONT_SMALL) );
    this->cameraWidthTextInput = new ofxUITextInput("CameraWidth", "1920", 80, 18) ;
    this->cameraWidthTextInput->setOnlyNumericInput(true);
    this->cameraWidthTextInput->setDrawOutline(true);
    this->cameraWidthTextInput->setDrawOutlineHighLight(true);
    this->cameraPanel->addWidgetRight( cameraWidthTextInput );

    ofxUILabel* cameraHeightLabel = new ofxUILabel(160, ofApp::CAMERA_HEIGHT_LABEL, OFX_UI_FONT_SMALL);
    this->cameraPanel->addWidgetDown( cameraHeightLabel );
    this->cameraHeightTextInput = new ofxUITextInput("CameraHeight", "1080", 80, 18);
    this->cameraHeightTextInput->setOnlyNumericInput(true);
    this->cameraHeightTextInput->setDrawOutline(true);
    this->cameraHeightTextInput->setDrawOutlineHighLight(true);
    this->cameraPanel->addWidgetRight( this->cameraHeightTextInput );

    this->imagePanel = new ofxUICanvas(0, 0, 400, 200);
    this->imagePanel->setFontSize(OFX_UI_FONT_SMALL, 8);
    this->imagePanel->setWidgetSpacing(15);
    this->gui->addWidgetRight(this->imagePanel);

    this->pixelsPerFrameSlider = new ofxUIIntSlider( ofApp::PIXELS_PER_FRAME_LABEL, 1, 256, 10, 400, 18);
    this->pixelsPerFrameSlider->setDrawOutline(true);
    this->imagePanel->addWidgetDown(pixelsPerFrameSlider);

    ofxUIToggle* saveToggle = this->imagePanel->addToggle("Salvar imagem", true, 16, 16);
    saveToggle->setDrawOutline(true);

    this->intervalToSaveSlider = new ofxUIIntSlider("Intervalo de salvamento (em minutos)", 1, 24 * 60, 30, 270, 18);
    this->intervalToSaveSlider->setDrawOutline(true);
    this->imagePanel->addWidgetDown( this->intervalToSaveSlider );

    ofxUILabelButton* clearButton = this->gui->addLabelButton("Limpar imagem", false, 200, 20);
    clearButton->setDrawFill(true);
    clearButton->setDrawOutline(true);
    this->imagePanel->addWidgetDown( clearButton );

    this->gui->addSpacer();
    ofxUIToggle* showAtStartToggle = this->gui->addToggle("Exibir esta tela ao iniciar", true, 16, 16);
    showAtStartToggle->setDrawOutline(true);
    this->gui->addSpacer();

    ofxUILabelButton* saveButton = this->gui->addLabelButton("Salvar", false, 100, 20);
    saveButton->setDrawFill(true);
    saveButton->setDrawOutline(true);

    ofxUILabelButton* hideButton = new ofxUILabelButton("Ocultar", false, 100, 20);
    hideButton->setDrawFill(true);
    hideButton->setDrawOutline(true);
    this->gui->addWidgetRight(hideButton);
    this->gui->addSpacer();

    this->gui->addLabel("Ampulheta - Andrei Thomaz, 2015");
    this->gui->addLabel("Integrante do projeto Máquinas do Tempo, desenvolvido com apoio da Bolsa de Artes Visual da Funarte 2014");
    this->gui->addLabel("Desenvolvido em C++ / OpenFrameworks e distribuído sob a licença MPL");
    this->gui->addLabel("Programação por Andrei Thomaz e Vitor Andrioli");
    this->gui->addSpacer();

    this->gui->addLabel("Realização");

    this->logos = new ofImage();
    this->logos->loadImage("funarte.png");
    this->gui->addImage("support", this->logos, this->logos->getWidth(), this->logos->getHeight(), false);

    ofAddListener(this->gui->newGUIEvent, this, &ofApp::guiEvent);
    ofAddListener(this->cameraPanel->newGUIEvent, this, &ofApp::cameraPanelEvent);
    ofAddListener(this->imagePanel->newGUIEvent, this, &ofApp::imagePanelEvent);

    this->gui->loadSettings("settings.xml");
    this->cameraPanel->loadSettings("camera.xml");
    this->imagePanel->loadSettings("image.xml");

    if (showAtStartToggle->getValue()) {
        this->gui->setVisible(true);
        this->cameraPanel->setVisible(true);
        this->imagePanel->setVisible(true);
    }
    else {
        this->gui->setVisible(false);
        this->cameraPanel->setVisible(false);
        this->imagePanel->setVisible(false);
    }

    // set camera
    vector<int> selectedCamera = this->cameraList->getSelectedIndeces();
    if (selectedCamera.size() > 0) {
        this->grabber.setDeviceID( selectedCamera[0] - 1 );
    }
    else {
        this->grabber.setDeviceID( 0 );
    }

    this->cameraWidth = this->cameraWidthTextInput->getIntValue();
    this->cameraHeight = this->cameraHeightTextInput->getIntValue();

    this->grabber.setDesiredFrameRate(30);
    this->grabber.initGrabber(this->cameraWidth, this->cameraHeight);

    // image to be drawn
    this->screenImage.allocate(this->cameraWidth, this->cameraHeight, OF_IMAGE_COLOR_ALPHA);
    this->fillImageWithWhite( &this->screenImage );

    // check if we have a image to load
    ofImage previousImage;
    if (previousImage.loadImage("hourglass.png")) {

        if (previousImage.getWidth() == this->screenImage.getWidth() && previousImage.getHeight() == this->screenImage.getHeight()) {

            this->screenImage = previousImage;

        }

    }

    // where we are going to start copying pixels
    this->x = (int) ofRandom(0, this->cameraWidth);
    this->y = (int) ofRandom(0, this->cameraHeight);
}

//--------------------------------------------------------------
void ofApp::update(){
    this->grabber.update();
    if (this->grabber.isFrameNew()) {
        ofPixels pixels = this->grabber.getPixelsRef();

        int pixelsPerFrame = this->pixelsPerFrameSlider->getScaledValue();
        for (int i = 0; i < pixelsPerFrame; i++) {

            // moves current position
            this->x += (int) ofRandom(-4, 4);
            this->y += (int) ofRandom(-4, 4);

            // keeps x and y inside camera area
            if (this->x < 0) {
                this->x = 0;
            }
            if (this->x >= this->cameraWidth) {
                this->x = this->cameraWidth - 1;
            }
            if (this->y < 0) {
                this->y = 0;
            }
            if (this->y >= this->cameraHeight) {
                this->y = this->cameraHeight - 1;
            }

            float strength = .2 + ofRandom(.8);
            this->paintPixel( this->x, this->y, pixels, strength );

            this->paintPixel( this->x - 1,  this->y - 1, pixels, ofRandom(0, ofApp::MAX_STRENGTH_AROUND_PIXEL));
            this->paintPixel( this->x,      this->y - 1, pixels, ofRandom(0, ofApp::MAX_STRENGTH_AROUND_PIXEL));
            this->paintPixel( this->x + 1,  this->y - 1, pixels, ofRandom(0, ofApp::MAX_STRENGTH_AROUND_PIXEL));

            this->paintPixel( this->x - 1, this->y, pixels, ofRandom(0, ofApp::MAX_STRENGTH_AROUND_PIXEL));
            this->paintPixel( this->x + 1, this->y, pixels, ofRandom(0, ofApp::MAX_STRENGTH_AROUND_PIXEL));

            this->paintPixel( this->x - 1,  this->y + 1, pixels, ofRandom(0, ofApp::MAX_STRENGTH_AROUND_PIXEL));
            this->paintPixel( this->x,      this->y + 1, pixels, ofRandom(0, ofApp::MAX_STRENGTH_AROUND_PIXEL));
            this->paintPixel( this->x + 1,  this->y + 1, pixels, ofRandom(0, ofApp::MAX_STRENGTH_AROUND_PIXEL));
        }

        this->screenImage.update();

        int intervalToSaveImage = this->intervalToSaveSlider->getValue();
        if (ofGetElapsedTimef() - this->lastTimeImageWasSaved > intervalToSaveImage * 60) {
            // image we will try to load on start up
            this->screenImage.saveImage("hourglass.png");

            // image we will save to have a recording of work
            char filename[64];
            sprintf( filename, "hourglass_%04d_%02d_%02d_%02d_%02d_%02d.png", ofGetYear(), ofGetMonth(), ofGetDay(), ofGetHours(), ofGetMinutes(), ofGetSeconds() );

            this->screenImage.saveImage(filename);

            this->lastTimeImageWasSaved = ofGetElapsedTimef();
        }

    }

}

void ofApp::paintPixel( int pixelX, int pixelY, ofPixels pixels, float strength )
{
    if (pixelX < 0 || pixelX >= pixels.getWidth()) {
        return;
    }
    if (pixelY < 0 || pixelY >= pixels.getHeight()) {
        return;
    }

    ofColor newColor = pixels.getColor( pixelX, pixelY );
    ofColor oldColor = this->screenImage.getPixelsRef().getColor( pixelX, pixelY );
    float diffR = newColor.r - oldColor.r;
    float diffG = newColor.g - oldColor.g;
    float diffB = newColor.b - oldColor.b;

    ofColor resultColor;
    resultColor.r = oldColor.r + (int) (diffR * strength);
    resultColor.g = oldColor.g + (int) (diffG * strength);
    resultColor.b = oldColor.b + (int) (diffB * strength);

    this->screenImage.setColor( pixelX, pixelY, resultColor);
}

//--------------------------------------------------------------
void ofApp::draw(){

    // We scale the image so it covers all the screen.
    // The image is croppred if necessary.
    float hScale = (float) ofGetWidth() / (float) this->screenImage.width;
    float vScale = (float) ofGetHeight() / (float) this->screenImage.height;

    float scale = max(hScale, vScale);

    int scaledWidth = (int) roundf((float) this->screenImage.width * scale);
    int scaledHeight = (int) roundf((float) this->screenImage.height * scale);

    int x = (ofGetWidth() - scaledWidth) / 2;
    int y = (ofGetHeight() - scaledHeight) / 2;

    this->screenImage.draw(x, y, scaledWidth, scaledHeight);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if (key == OF_KEY_TAB) {

        if (this->gui->isVisible()) {
            this->gui->setVisible(false);
            this->cameraPanel->setVisible(false);
            this->imagePanel->setVisible(false);
        }
        else {
            this->gui->setVisible(true);
            this->cameraPanel->setVisible(true);
            this->imagePanel->setVisible(true);
        }

    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

    if (this->hideButtonReleased) {
        this->gui->setVisible(false);
        this->hideButtonReleased = false;
    }
    else {
        this->gui->setVisible(true);
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){


}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

//--------------------------------------------------------------
// ofxUI
void ofApp::exit()
{
    //delete gui;
}

//--------------------------------------------------------------
// ofxUI
void ofApp::guiEvent(ofxUIEventArgs &e)
{
    ofLog() << "e.getName(): " << e.getName();
    if (e.getName() == "Salvar") {
        this->gui->saveSettings("settings.xml");
        this->cameraPanel->saveSettings("camera.xml");
        this->imagePanel->saveSettings("image.xml");
    }
    else if (e.getName() == "Ocultar") {
        ofxUIButton* button = e.getButton();
        if (button->getValue()) {
            this->hideButtonReleased = true;
            button->setValue(false);
        }
    }
    else if (e.getName() == "Limpar imagem") {
        this->fillImageWithWhite( &this->screenImage );
        this->screenImage.saveImage("hourglass.png");
    }
}

void ofApp::cameraPanelEvent(ofxUIEventArgs &e)
{
    ofLog() << "e.getName(): " << e.getName();
}

void ofApp::imagePanelEvent(ofxUIEventArgs &e)
{
    ofLog() << "e.getName(): " << e.getName();
    if (e.getName() == "Limpar imagem") {
        this->fillImageWithWhite( &this->screenImage );
        this->screenImage.saveImage("hourglass.png");
    }
}

void ofApp::fillImageWithWhite( ofImage* image )
{
    // paints with white
    unsigned char* pixels = image->getPixels();
    int bytesPerPixel = 3;
    if (image->type == OF_IMAGE_COLOR_ALPHA)
        bytesPerPixel = 4;
    else if (image->type == OF_IMAGE_GRAYSCALE)
        bytesPerPixel = 1;
    for (int i =0; i < image->getWidth() * image->getHeight() * bytesPerPixel; i++) {
        pixels[i] = 255;
    }
}
