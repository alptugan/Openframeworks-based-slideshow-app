#include "imgLoader.h"

//--------------------------------------------------------------
imgLoader::imgLoader() {
    //start();
}

//--------------------------------------------------------------
imgLoader::~imgLoader() {
    delete [] textures;
    textures = NULL;
}

//--------------------------------------------------------------
void imgLoader::fetchTextureNames(string path) {

    dirList.reset();
    dirList.allowExt("jpg");
    dirList.allowExt("png");
    numTextures = dirList.listDir(path);

    //dirList.reset();
    //    dirList.allowExt("png");
    //    numTextures = dirList.listDir("path");
    ofLog(OF_LOG_VERBOSE,"TOTAL OF IMAGES FOUND %d",numTextures);

    textures = new string[numTextures];

    for (int i=0; i < numTextures; i++) {
        //textures[i] = "textures/"+dirList.getName(i);
        textures[i] = dirList.getPath(i);
    }

    // load the first texture
    textureIndex = -1;
    //loadNextTexture();
}
//--------------------------------------------------------------
void imgLoader::start(string pathToImages) {
	imgLoaded = false;

	ofDisableArbTex();
    fetchTextureNames(pathToImages);
    currTexture=NULL;
    nextTexture=NULL;
	to_delete_img = NULL;
    startThread(true, false);  // blocking, verbose
}

//--------------------------------------------------------------
void imgLoader::stop() {
    stopThread();
}


void imgLoader::loadNextTexture() {
        textureIndex = (textureIndex+1)%numTextures;
        nextTexture = new ofImage();
        nextTexture->setUseTexture(false);
        nextTexture->loadImage(textures[textureIndex]);
        ofLog(OF_LOG_VERBOSE,"Loading image %d: %s", textureIndex,  textures[textureIndex].c_str());
		//Scaling the image 
		float aspect_ratio = (float)nextTexture->width / nextTexture->height;
		//Check if i need to scale

		if (nextTexture->height != 768 || nextTexture->width != 1024){
			int new_width = 1024;
			int new_height = 768;
            
			if (nextTexture->height >= nextTexture->width) new_width = (int)(aspect_ratio * new_height);
			if (nextTexture->width >= nextTexture->height) new_height = (int)(new_width / aspect_ratio);
            
			nextTexture->resize(new_width,new_height);
			ofLog(OF_LOG_VERBOSE,"Scaled image: %f, %d, %d",aspect_ratio, new_width, new_height);
		};		
		
		imgLoaded=true;
}

ofImage * imgLoader::getNextTexture() {
    if(imgLoaded){
		to_delete_img = currTexture;
        currTexture=nextTexture;
        imgLoaded=false;
		ofLog(OF_LOG_VERBOSE,"Showing: %s", textures[textureIndex].c_str());
        return currTexture;
    }else{
        return NULL;
    }
}

void imgLoader::releaseCurrentTexture(){
	   delete to_delete_img;
}

//--------------------------------------------------------------
void imgLoader::threadedFunction() {
    while (isThreadRunning() != 0)
	{
        if (!imgLoaded) {
			if( lock() ){
				//Double calls to load two textures for the transition.
				loadNextTexture();
				unlock();
			}
		}
		ofSleepMillis(1 * 1000);
    }
}