#include "testApp.h"


ofImage * testApp::loadImage(string filePath) {
        ofImage* nextTexture = new ofImage();
        //nextTexture->setUseTexture(false);
        nextTexture->loadImage(filePath);
		printf("Loading image: %s\n",  filePath.c_str());
		//Scaling the image 
		float aspect_ratio = (float)nextTexture->width / nextTexture->height;
		//Check if i need to scale
		if (nextTexture->height != 768 || nextTexture->width != 1024){
			int new_width = 1024;
			int new_height = 768;
			if (nextTexture->height > 768) new_width = (int)(aspect_ratio * new_height);
			if (nextTexture->width > 1024) new_height = (int)(new_width / aspect_ratio);
			nextTexture->resize(new_width,new_height);
			printf("Scaled image: %f, %d, %d\n",aspect_ratio, new_width, new_height);
		};		
		return nextTexture;
}

ofBaseImage * testApp::loadVideo(string filePath) {
        ofVideoPlayer* nextTexture = new ofVideoPlayer();
        //nextTexture->setUseTexture(false);
        nextTexture->loadMovie(filePath);
		printf("Loading video: %s\n",  filePath.c_str());
		//Scaling the image 
		//float aspect_ratio = (float)nextTexture->width / nextTexture->height;
		//Check if i need to scale
		return nextTexture;
}


void testApp::setup() {
    ofBackground(0,0,0);
    ofSetVerticalSync(true);
	transitionShader.loadShader("shaders/Shrink");
	ofDisableArbTex();
    currImage=loadImage("data/images/100_7620.JPG");
	nextImg=loadVideo("data/images/finger.mov");
	//textureMovie=loadVideo("data/images/finger.mov");
    loadNextImg=false;
	transitionEnded = false;
	progress = 0;
	step = 0.02;
    temp_cur_texture.allocate(1024,1024,GL_RGB); // CHANGE FOR THE SIZE OF THE IMAGES ...
	temp_next_texture.allocate(1024,1024,GL_RGB);
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetFrameRate(37);
}

void testApp::update(){
    //if (ofGetFrameNum()%500 == 0) loadNextImg=true;
	ofSetWindowTitle(ofToString(ofGetFrameRate(), 2.0));
	//Center the image
	//check the 0,0 position of the image
	//if (currImage->width != 1024) centerX = 512 - (currImage->width / 2);
    if(loadNextImg)
    {
		progress = progress + step;
           if(transitionEnded) {
			    printf("Animation ended, swapping.\n");
				ofBaseImage * temp = nextImg;
				nextImg=currImage;
				currImage=temp;
				loadNextImg=false;
				transitionEnded = false;
				progress = 0;
           }
    }
}


void testApp::fadeInfadeOut(ofBaseImage* currImg,ofBaseImage* nextImg) {
				glEnable(GL_BLEND);
				glColor4f(1.0f, 1.0f, 1.0f,1 - (progress * 2));   
				glBlendFunc(GL_SRC_ALPHA, GL_ZERO);   // takes src brightness (ignore dest color)	
				temp_cur_texture.loadData(currImg->getPixels(),currImg->width,currImg->height,GL_RGB);
				temp_cur_texture.draw(getCenteredCoordinate(currImg),0,currImg->width,currImg->height); //
				//If the second image disappeared
				if (progress >=0.5) {
					//Blend della seconda immagine
					glColor4f(1.0f, 1.0f, 1.0f,(progress - 0.5) * 2);   
					glBlendFunc(GL_SRC_ALPHA, GL_ONE);   // takes src brightness (ignore dest color)	
					temp_cur_texture.loadData(nextImg->getPixels(),nextImg->width,nextImg->height,GL_RGB);
					temp_cur_texture.draw(getCenteredCoordinate(nextImg),0,nextImg->width,nextImg->height); //
					if (progress >= 1.0) transitionEnded = true;
				}
}


int testApp::getCenteredCoordinate(ofBaseImage* image) {
	int x = 0;
	if (image->width != 1024) x = 512 - (image->width / 2);
	return x;
}


void testApp::shaderTransition(ofBaseImage* currImg,ofBaseImage* nextImg) {
				transitionShader.setShaderActive(true);
				ofEnableAlphaBlending();
				temp_cur_texture.loadData(currImg->getPixels(),currImg->width,currImg->height,GL_RGB);
				temp_next_texture.loadData(nextImg->getPixels(),nextImg->width,nextImg->height,GL_RGB);
				glActiveTexture(GL_TEXTURE1);
				temp_cur_texture.bind();
				glActiveTexture(GL_TEXTURE2);
				temp_next_texture.bind();
				temp_cur_texture.draw(getCenteredCoordinate(currImg),0,currImg->width,currImg->height);
				//Start shader code
				transitionShader.setUniformVariable1i("oldTex", 1);
				transitionShader.setUniformVariable1i("newTex", 2);
				transitionShader.setUniformVariable1f("progress", progress);
				//check the 0,0 position of the image
				temp_next_texture.draw(getCenteredCoordinate(nextImg),0,nextImg->width,nextImg->height);
				if (progress > 1.0)  {
					//transitionShader.setShaderActive(false);
					temp_cur_texture.unbind();
					temp_next_texture.unbind();
					glActiveTexture(GL_TEXTURE0);
					transitionEnded = true;
				};
				ofDisableAlphaBlending();
}


void testApp::draw(){
    if(currImage!=NULL)
    {
		if (loadNextImg && nextImg!=NULL){
			transitionEnded = false;
			//crossFadeTransition(currImage,nextImg);
			fadeInfadeOut(currImage,nextImg);
			//shaderTransition(currImage,nextImg);
		 } else {
			ofSetColor(0xffffff);
			temp_cur_texture.loadData(currImage->getPixels(),currImage->width,currImage->height,GL_RGB);
			temp_cur_texture.draw(getCenteredCoordinate(currImage),0,1024,768); //
		}
    }

}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
    /*
    if (key == 'a'){
        TO.start();
    } else if (key == 's'){
        TO.stop();
    }
    */
	if (key == 'a'){
       loadNextImg=true;
    }
}



//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    //currX = x;
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(){

}