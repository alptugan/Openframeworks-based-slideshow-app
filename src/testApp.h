#pragma once

#include "ofMain.h"
#include "imgLoader.h"
#include "ParamsLoader.h"

class testApp : public ofBaseApp{

	public:
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased();
    ofImage * loadImage(string filePath);
    void fadeInfadeOut(ofImage* currImg,ofImage* nextImg);
    
    
    //ofVideoPlayer  textureMovie;
    ParamsLoader loaderConfig;
    ofImage * currImage;
    ofImage * nextImg;
    ofTexture temp_cur_texture;
    ofTexture temp_next_texture;
    
    imgLoader   loader;
    bool        loadNextImg;
    bool transitionEnded;
    bool transitionStarted;
    bool firstImage;
    float		step;
    float		progress;
    
private:
	int getCenteredCoordinate(ofImage* image);
    int getCenteredCoordinateY(ofImage* image);
		
};
