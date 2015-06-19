#include "ofApp.h"

const string ofApp::CAMERA_WIDTH_LABEL = "Largura da câmera";
const string ofApp::CAMERA_HEIGHT_LABEL = "Altura da câmera";
const string ofApp::PIXELS_PER_FRAME_LABEL = "Pixels por frame (velocidade)";
const string ofApp::SAVE_IMAGE_LABEL = "Salvar imagem a cada";
const string ofApp::RESET_IMAGE_LABEL = "Limpar imagem";
const string ofApp::SUPPORT_BUTTON_NAME = "support";
const string ofApp::SAVE_LABEL = "Salvar";
const string ofApp::CANCEL_LABEL = "Cancelar";

const string ofApp::ZERO_DEGREES_LABEL = "0 graus";
const string ofApp::NINETY_DEGREES_LABEL = "90 graus";
const string ofApp::ONE_HUNDRED_EIGHTY_DEGREES_LABEL = "180 graus";
const string ofApp::TWO_HUNDRED_SEVENTY_DEGREES_LABEL = "270 graus";

//--------------------------------------------------------------
void ofApp::setup(){

    this->gui = new ofxUICanvas(0, 0, ofGetWidth(), ofGetHeight());
    this->gui->setWidgetSpacing(10);
    gui->setFontSize(OFX_UI_FONT_SMALL, 8);
    gui->setFontSize(OFX_UI_FONT_LARGE, 18);

    ofxUIColor backgroundColor = ofxUIColor::white;
    ofxUIColor fillColor = ofxUIColor::black;
    ofxUIColor fillHightlightColor = ofxUIColor::black;
    ofxUIColor outline = ofxUIColor::black;
    ofxUIColor outlineHighlight = ofxUIColor::red;
    ofxUIColor paddedColor = ofxUIColor::blue;
    ofxUIColor paddedOutlineColor = ofxUIColor::orange;

    this->gui->setUIColors( backgroundColor, outline, outlineHighlight, fillColor, fillHightlightColor, paddedColor, paddedOutlineColor );

    this->gui->addLabel("title", "Ampulheta", OFX_UI_FONT_LARGE);
    this->gui->addSpacer();

    this->cameraPanel = new ofxUICanvas(0, 0, 400, 235);
    this->cameraPanel->setUIColors( backgroundColor, outline, outlineHighlight, fillColor, fillHightlightColor, paddedColor, paddedOutlineColor );
    this->cameraPanel->setFontSize(OFX_UI_FONT_SMALL, 8);
    this->cameraPanel->setWidgetSpacing(10);
    this->gui->addWidgetDown(this->cameraPanel);

    // lista as câmeras conectadas a este computador
    this->videoGrabber = new ofVideoGrabber();
    vector< ofVideoDevice > devices = this->videoGrabber->listDevices();

    vector<string> *cameras = new vector<string>();
    vector<ofVideoDevice>::iterator it;

    for (it = devices.begin(); it != devices.end(); ++it) {
        ofVideoDevice device = *it;
        cameras->push_back(device.deviceName);
    }

    this->cameraList = this->cameraPanel->addDropDownList("Escolha a câmera", *cameras, 300, 10);
    this->cameraList->setAllowMultiple(false);
    this->cameraList->setDrawOutline(true);
    this->cameraList->setAutoClose(true);

    vector<ofxUILabelToggle *> toggles = this->cameraList->getToggles();
    vector<ofxUILabelToggle *>::iterator togglesIterator;

    for (togglesIterator = toggles.begin(); togglesIterator != toggles.end(); ++togglesIterator) {
        ofxUILabelToggle* labelToggle = *togglesIterator;
        labelToggle->setColorFillHighlight(ofxUIColor::black);
        labelToggle->setColorFill(ofxUIColor::white);
        labelToggle->setColorOutlineHighlight(ofxUIColor::black);
        labelToggle->setColorBack(ofxUIColor::white);
    }

    this->cameraPanel->addWidgetDown( new ofxUILabel(170, ofApp::CAMERA_WIDTH_LABEL, OFX_UI_FONT_SMALL) );
    this->cameraWidthTextInput = new ofxUITextInput("CameraWidth", "1920", 80, 18) ;
    this->cameraWidthTextInput->setOnlyNumericInput(true);
    this->cameraWidthTextInput->setDrawOutline(true);
    this->cameraWidthTextInput->setDrawOutlineHighLight(true);
    this->cameraPanel->addWidgetRight( cameraWidthTextInput );
    this->textInputs.push_back(this->cameraWidthTextInput);

    ofxUILabel* cameraHeightLabel = new ofxUILabel(170, ofApp::CAMERA_HEIGHT_LABEL, OFX_UI_FONT_SMALL);
    this->cameraPanel->addWidgetDown( cameraHeightLabel );
    this->cameraHeightTextInput = new ofxUITextInput("CameraHeight", "1080", 80, 18);
    this->cameraHeightTextInput->setOnlyNumericInput(true);
    this->cameraHeightTextInput->setDrawOutline(true);
    this->cameraHeightTextInput->setDrawOutlineHighLight(true);
    this->cameraPanel->addWidgetRight( this->cameraHeightTextInput );
    this->textInputs.push_back(this->cameraHeightTextInput);

    this->rotations = 0;

    this->cameraPanel->addLabel("Rotação da imagem", OFX_UI_FONT_SMALL);

    this->zeroRotationToggle = new ofxUIToggle(ofApp::ZERO_DEGREES_LABEL, true, 16, 16);
    this->zeroRotationToggle->setDrawOutline(true);
    this->cameraPanel->addWidgetDown(this->zeroRotationToggle);

    this->ninetyRotationToggle = new ofxUIToggle(ofApp::NINETY_DEGREES_LABEL, true, 16, 16);
    this->ninetyRotationToggle->setDrawOutline(true);
    this->cameraPanel->addWidgetDown(this->ninetyRotationToggle);

    this->oneHundredEightyRotationToggle = new ofxUIToggle(ofApp::ONE_HUNDRED_EIGHTY_DEGREES_LABEL, true, 16, 16);
    this->oneHundredEightyRotationToggle->setDrawOutline(true);
    this->cameraPanel->addWidgetDown(this->oneHundredEightyRotationToggle);

    this->twoHundredSeventyRotationToggle = new ofxUIToggle(ofApp::TWO_HUNDRED_SEVENTY_DEGREES_LABEL, true, 16, 16);
    this->twoHundredSeventyRotationToggle->setDrawOutline(true);
    this->cameraPanel->addWidgetDown(this->twoHundredSeventyRotationToggle);

    this->imagePanel = new ofxUICanvas(190, 0, 400, 235);
    this->imagePanel->setUIColors( backgroundColor, outline, outlineHighlight, fillColor, fillHightlightColor, paddedColor, paddedOutlineColor );
    this->imagePanel->setFontSize(OFX_UI_FONT_SMALL, 8);
    this->imagePanel->setWidgetSpacing(10);

    ofxUILabel* pixelsPerFrameLabel = new ofxUILabel(230, ofApp::PIXELS_PER_FRAME_LABEL, OFX_UI_FONT_SMALL);
    this->imagePanel->addWidgetDown( pixelsPerFrameLabel );

    this->pixelsPerFrameTextInput =new ofxUITextInput("pixelsPerFrame", "30", 80, 18);
    this->pixelsPerFrameTextInput->setOnlyNumericInput(true);
    this->pixelsPerFrameTextInput->setDrawOutline(true);
    this->imagePanel->addWidgetRight( pixelsPerFrameTextInput );

    this->imagePanel->addSpacer();

    this->saveImageToggle = new ofxUIToggle(ofApp::SAVE_IMAGE_LABEL, true, 16, 16);
    this->saveImageToggle->setDrawOutline(true);
    this->imagePanel->addWidgetDown(this->saveImageToggle);

    this->intervalToSaveImage = 15;

    this->intervalToSaveTextInput = new ofxUITextInput("A cada", "15", 60, 18);
    this->intervalToSaveTextInput->setOnlyNumericInput(true);
    this->intervalToSaveTextInput->setDrawOutline(true);
    this->intervalToSaveTextInput->setDrawOutlineHighLight(true);
    this->imagePanel->addWidgetRight( this->intervalToSaveTextInput );
    this->textInputs.push_back(this->intervalToSaveTextInput);

    ofxUILabel* minutesLabel = new ofxUILabel(90, "minutos", OFX_UI_FONT_SMALL);
    this->imagePanel->addWidgetRight( minutesLabel );

    ofxUILabelButton* clearButton = this->gui->addLabelButton(ofApp::RESET_IMAGE_LABEL, false, 150, 20);
    clearButton->setDrawFill(true);
    clearButton->setDrawOutline(true);
    this->imagePanel->addWidgetDown( clearButton );

    this->imagePanel->addSpacer();

    this->showAtStartupToggle = this->imagePanel->addToggle("Exibir esta tela ao iniciar", true, 16, 16);
    this->showAtStartupToggle->setDrawOutline(true);
    this->imagePanel->addSpacer();

    this->fullScreenToggle = this->imagePanel->addToggle("Tela cheia", true, 16, 16);
    this->fullScreenToggle->setDrawOutline(true);

    this->gui->addWidgetRight(this->imagePanel);

    this->gui->addSpacer();

    ofxUILabelButton* saveButton = this->gui->addLabelButton(ofApp::SAVE_LABEL, false, 100, 20);
    saveButton->setDrawFill(true);
    saveButton->setDrawOutline(true);

    ofxUILabelButton* cancelButton = new ofxUILabelButton(ofApp::CANCEL_LABEL, false, 100, 20);
    cancelButton->setDrawFill(true);
    cancelButton->setDrawOutline(true);
    this->gui->addWidgetRight(cancelButton);
    this->gui->addSpacer();

    this->gui->addLabel("Ampulheta - Andrei Thomaz, 2015", OFX_UI_FONT_SMALL);
    this->gui->addLabel("Integrante do projeto Máquinas do Tempo, desenvolvido com apoio da", OFX_UI_FONT_SMALL);
    this->gui->addLabel("Bolsa Funarte de Estímulo à Produção em Artes Visuais 2014", OFX_UI_FONT_SMALL);
    this->gui->addLabel("Desenvolvido em C++ / OpenFrameworks e distribuído sob a licença MPL", OFX_UI_FONT_SMALL);
    this->gui->addLabel("Programação por Andrei Thomaz", OFX_UI_FONT_SMALL);
    this->gui->addSpacer();

    this->gui->addLabel("Realização");
    this->gui->addImageButton(ofApp::SUPPORT_BUTTON_NAME, "funarte.png", false, 509, 60);

    ofAddListener(this->gui->newGUIEvent, this, &ofApp::guiEvent);
    ofAddListener(this->cameraPanel->newGUIEvent, this, &ofApp::cameraPanelEvent);
    ofAddListener(this->imagePanel->newGUIEvent, this, &ofApp::imagePanelEvent);

    this->cameraPanel->loadSettings("camera.xml");
    this->imagePanel->loadSettings("image.xml");
    this->gui->loadSettings("settings.xml");

    // reads values from controls and stores them into properties
    this->applyConfigurationChanges();

    if (this->showAtStartup) {
        this->showConfigurationPanel();
    }
    else {
        this->hideConfigurationPanel();
    }

    this->reset();
}

void ofApp::reset()
{
    if (this->videoGrabber != NULL) {
        if (this->videoGrabber->isInitialized()) {
            this->videoGrabber->close();
            delete this->videoGrabber;
            this->videoGrabber = new ofVideoGrabber();
        }
    }

    // set camera
    vector<int> selectedCamera = this->cameraList->getSelectedIndeces();
    if (selectedCamera.size() > 0) {
        this->videoGrabber->setDeviceID( selectedCamera[0] );
    }
    else {
        this->videoGrabber->setDeviceID( 0 );
    }

    this->videoGrabber->setDesiredFrameRate(30);
    this->videoGrabber->initGrabber(this->cameraWidth, this->cameraHeight);

    // image to be drawn
    this->screenImage.allocate(this->imageWidth, this->imageHeight, OF_IMAGE_COLOR_ALPHA);
    this->fillImageWithWhite( &this->screenImage );

    // check if we have a image to load
    ofImage previousImage;
    if (previousImage.loadImage("hourglass.png")) {

        if (previousImage.getWidth() == this->screenImage.getWidth() && previousImage.getHeight() == this->screenImage.getHeight()) {

            this->screenImage = previousImage;

        }

    }

    this->lastTimeImageWasSaved = 0;

    // where we are going to start copying pixels
    this->x = (int) ofRandom(0, this->imageWidth);
    this->y = (int) ofRandom(0, this->imageHeight);
}

//--------------------------------------------------------------
void ofApp::update(){
    this->videoGrabber->update();

    if (this->gui->isVisible()) {
        return;
    }

    if (this->videoGrabber->isFrameNew()) {
        ofPixels pixels = this->videoGrabber->getPixelsRef();
        pixels.rotate90( this->rotations );

        for (int i = 0; i < this->pixelsPerFrame; i++) {

            // moves current position
            this->x += (int) ofRandom(-4, 4);
            this->y += (int) ofRandom(-4, 4);

            // keeps x and y inside camera area
            if (this->x < 0) {
                this->x = 0;
            }
            if (this->x >= this->imageWidth) {
                this->x = this->imageWidth - 1;
            }
            if (this->y < 0) {
                this->y = 0;
            }
            if (this->y >= this->imageHeight) {
                this->y = this->imageHeight - 1;
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

        if (this->saveImage)
        {
            if (ofGetElapsedTimef() - this->lastTimeImageWasSaved > this->intervalToSaveImage * 60) {
                // image we will try to load on start up
                this->screenImage.saveImage("hourglass.png");

                // image we will save to have a recording of work
                this->saveCurrentImage();
                this->lastTimeImageWasSaved = ofGetElapsedTimef();
            }
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
            this->hideConfigurationPanel();
        }
        else {
            this->showConfigurationPanel();
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
    if (e.getName() == ofApp::SAVE_LABEL) {
        // catches the click when mouse is released, not pressed
        if (!e.getButton()->getValue()) {

            this->cameraPanel->saveSettings("camera.xml");
            this->imagePanel->saveSettings("image.xml");
            this->gui->saveSettings("settings.xml");
            this->applyConfigurationChanges();
            this->reset();
            this->hideConfigurationPanel();

        }
    }
    else if (e.getName() == ofApp::CANCEL_LABEL) {
        // catches the click when mouse is released, not pressed
        if (!e.getButton()->getValue()) {

            this->cancelConfigurationChanges();
            this->hideConfigurationPanel();

        }
    }
    else if (e.getName() == ofApp::SUPPORT_BUTTON_NAME) {
        // catches the click when mouse is released, not pressed
        if (!e.getButton()->getValue()) {
            ofLaunchBrowser("http://www.funarte.gov.br/");
        }
    }
}

void ofApp::cameraPanelEvent(ofxUIEventArgs &e)
{
    if (e.getName() == ofApp::ZERO_DEGREES_LABEL && e.getToggle()->getValue()) {
        this->ninetyRotationToggle->setValue(false);
        this->oneHundredEightyRotationToggle->setValue(false);
        this->twoHundredSeventyRotationToggle->setValue(false);
    }
    else if (e.getName() == ofApp::NINETY_DEGREES_LABEL && e.getToggle()->getValue()) {
        this->zeroRotationToggle->setValue(false);
        this->oneHundredEightyRotationToggle->setValue(false);
        this->twoHundredSeventyRotationToggle->setValue(false);
    }
    else if (e.getName() == ofApp::ONE_HUNDRED_EIGHTY_DEGREES_LABEL && e.getToggle()->getValue()) {
        this->zeroRotationToggle->setValue(false);
        this->ninetyRotationToggle->setValue(false);
        this->twoHundredSeventyRotationToggle->setValue(false);
    }
    else if (e.getName() == ofApp::TWO_HUNDRED_SEVENTY_DEGREES_LABEL && e.getToggle()->getValue()) {
        this->zeroRotationToggle->setValue(false);
        this->ninetyRotationToggle->setValue(false);
        this->oneHundredEightyRotationToggle->setValue(false);
    }
    else {
        this->checkTextInputFocus(e);
    }
}

void ofApp::imagePanelEvent(ofxUIEventArgs &e)
{
    if (e.getName() == ofApp::RESET_IMAGE_LABEL) {
        // catches the click when mouse is released, not pressed
        if (!e.getButton()->getValue()) {
            this->fillImageWithWhite( &this->screenImage );
            this->screenImage.saveImage("hourglass.png");
        }
    }
    else {
        this->checkTextInputFocus(e);
    }
}

void ofApp::checkTextInputFocus(ofxUIEventArgs &e)
{
    if (e.getKind() == OFX_UI_WIDGET_TEXTINPUT){
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        if (ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_FOCUS){
            this->unfocusAllTextInputs(ti);
        }
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

void ofApp::saveCurrentImage()
{
    char filename[64];
    sprintf( filename, "hourglass_%04d_%02d_%02d_%02d_%02d_%02d.png", ofGetYear(), ofGetMonth(), ofGetDay(), ofGetHours(), ofGetMinutes(), ofGetSeconds() );

    this->screenImage.saveImage(filename);
}

void ofApp::hideConfigurationPanel()
{
    this->gui->setVisible(false);
    this->cameraPanel->setVisible(false);
    this->imagePanel->setVisible(false);
    ofHideCursor();
}

void ofApp::showConfigurationPanel()
{
    this->gui->setVisible(true);
    this->gui->disableKeyEventCallbacks();

    this->cameraPanel->setVisible(true);
    this->imagePanel->setVisible(true);
    ofShowCursor();
}

void ofApp::cancelConfigurationChanges()
{
    this->zeroRotationToggle->setValue(this->rotations == 0);
    this->ninetyRotationToggle->setValue(this->rotations == 1);
    this->oneHundredEightyRotationToggle->setValue(this->rotations == 2);
    this->twoHundredSeventyRotationToggle->setValue(this->rotations == 3);

    this->cameraWidthTextInput->setTextString( ofToString(this->cameraWidth) );
    this->cameraHeightTextInput->setTextString( ofToString(this->cameraHeight) );

    this->pixelsPerFrameTextInput->setTextString( ofToString(this->pixelsPerFrame) );
    this->intervalToSaveTextInput->setTextString( ofToString(this->intervalToSaveImage) );
    this->showAtStartupToggle->setValue( this->showAtStartup );
    this->fullScreenToggle->setValue( this->fullScreen );
    this->saveImageToggle->setValue( this->saveImage );
}

void ofApp::applyConfigurationChanges()
{
    if (this->zeroRotationToggle->getValue()) {
        this->rotations = 0;
    }
    if (this->ninetyRotationToggle->getValue()) {
        this->rotations = 1;
    }
    else if (this->oneHundredEightyRotationToggle->getValue()) {
        this->rotations = 2;
    }
    else if (this->twoHundredSeventyRotationToggle->getValue()) {
        this->rotations = 3;
    }

    this->zeroRotationToggle->setValue(this->rotations == 0);
    this->ninetyRotationToggle->setValue(this->rotations == 1);
    this->oneHundredEightyRotationToggle->setValue(this->rotations == 2);
    this->twoHundredSeventyRotationToggle->setValue(this->rotations == 3);

    this->cameraWidth = this->cameraWidthTextInput->getIntValue();
    this->cameraHeight = this->cameraHeightTextInput->getIntValue();

    if (this->rotations % 2 == 0) {
        this->imageWidth = this->cameraWidth;
        this->imageHeight = this->cameraHeight;
    }
    else {
        this->imageWidth = this->cameraHeight;
        this->imageHeight = this->cameraWidth;
    }

    this->pixelsPerFrame = this->pixelsPerFrameTextInput->getIntValue();
    this->intervalToSaveImage = this->intervalToSaveTextInput->getIntValue();

    this->showAtStartup = this->showAtStartupToggle->getValue();
    this->fullScreen = this->fullScreenToggle->getValue();
    this->saveImage = this->saveImageToggle->getValue();

    if (this->fullScreen) {
        ofSetFullscreen(true);
    }
    else {
        ofSetFullscreen(false);
        ofSetWindowShape(this->imageWidth, this->imageHeight);
    }
}

void ofApp::unfocusAllTextInputs(ofxUITextInput* except){
    for (int i = 0; i < this->textInputs.size(); i ++){
        if (except != this->textInputs[i]){
            this->textInputs[i]->setFocus(false);
        }
    }
}
