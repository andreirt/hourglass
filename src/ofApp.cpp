#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    this->gui = new ofxUICanvas(0, 0, this->cameraWidth * 2, this->cameraHeight * 2);
    this->gui->setWidgetSpacing(10);
    gui->setFontSize(OFX_UI_FONT_SMALL, 8);
    
    // lista as câmeras conectadas a este computador
    vector< ofVideoDevice > devices = this->grabber.listDevices();
    
    vector<string> *cameras = new vector<string>();
    vector<ofVideoDevice>::iterator it;
    
    for (it = devices.begin(); it != devices.end(); ++it) {
        ofVideoDevice device = *it;
        cameras->push_back(device.deviceName);
    }
    
    this->cameraList = this->gui->addDropDownList("Escolha a câmera", *cameras, 400, 10);
    this->cameraList->setAllowMultiple(false);
    
    this->gui->addWidgetRight( new ofxUILabel(180, ofApp::CAMERA_WIDTH_LABEL, OFX_UI_FONT_SMALL) );
    this->cameraWidthTextInput = new ofxUITextInput("CameraWidth", "1920", 80, 18) ;
    this->cameraWidthTextInput->setOnlyNumericInput(true);
    this->gui->addWidgetRight( cameraWidthTextInput );
    
    this->gui->addWidgetRight( new ofxUILabel(160, ofApp::CAMERA_HEIGHT_LABEL, OFX_UI_FONT_SMALL) );
    this->cameraHeightTextInput = new ofxUITextInput("CameraHeight", "1080", 80, 18);
    this->cameraHeightTextInput->setOnlyNumericInput(true);
    this->gui->addWidgetRight( this->cameraHeightTextInput );
    
    this->gui->addSpacer();
    
    this->pixelsPerFrameSlider = this->gui->addIntSlider( ofApp::PIXELS_PER_FRAME_LABEL, 1, 256, 10, 400, 18);
    this->gui->addSpacer();
    
    this->gui->addToggle("Salvar imagem", true, 16, 16);
    this->gui->addWidgetRight(new ofxUIIntSlider("Intervalo de salvamento (em minutos)", 1, 24 * 60, 30, 270, 18));
    this->gui->addSpacer();
    
    this->gui->addLabelButton("Aplicar alterações", false, 250, 20);
    this->gui->addWidgetRight(new ofxUILabelButton("Cancelar", false, 250, 20));
    this->gui->addSpacer();
    
    this->gui->addLabel("Ampulheta - Andrei Thomaz, 2015");
    this->gui->addLabel("Integrante do projeto Máquinas do Tempo, desenvolvido com apoio da Bolsa de Artes Visual da Funarte 2015");
    this->gui->addLabel("Desenvolvido em C++ / OpenFrameworks e distribuído sob a licença MPL");
    this->gui->addLabel("Programação por Andrei Thomaz e Vitor Andrioli");
    this->gui->addLabel("Ícones por Gabriela Araújo");
    
    this->gui->addSpacer();
    this->gui->addLabelButton("Ocultar", false, 250, 20);
    
    ofAddListener(this->gui->newGUIEvent, this, &ofApp::guiEvent);
    this->gui->loadSettings("settings.xml");
    
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
    
    // paints with white
    unsigned char* pixels = this->screenImage.getPixels();
    for (int i =0; i<this->screenImage.getWidth() * this->screenImage.getHeight() * 4; i++) {
        pixels[i] = 255;
    }
    
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
    
    // speed
    this->pixelsPerFrame = this->pixelsPerFrameSlider->getScaledValue();

    
    //ofxUITextInput* windowWidth = (ofxUITextInput*) this->gui->getWidget("Largura da janela");
    //this->windowWidth = windowWidth->getIntValue();
    
    //ofSetWindowShape( this->gui->getuitext, <#int height#>);
    //ofSetWindowPosition(<#int x#>, <#int y#>);

    
}

//--------------------------------------------------------------
void ofApp::update(){
    this->grabber.update();
    if (this->grabber.isFrameNew()) {
        ofPixels pixels = this->grabber.getPixelsRef();
        
        for (int i = 0; i < this->pixelsPerFrame; i++) {
            
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
        
        
        if (ofGetElapsedTimef() - this->lastTimeImageWasSaved > this->intervalToSaveImage * 60) {
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
    this->screenImage.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
    delete gui;
}

//--------------------------------------------------------------
// ofxUI
void ofApp::guiEvent(ofxUIEventArgs &e)
{
    if (e.getName() == "Aplicar alterações") {
        this->gui->saveSettings("settings.xml");
        
        // speed
        this->pixelsPerFrame = this->pixelsPerFrameSlider->getScaledValue();
    }
    else if (e.getName() == "Ocultar") {
        ofxUIButton* button = e.getButton();
        if (button->getValue()) {
            this->hideButtonReleased = true;
            button->setValue(false);
        }
    }
}
