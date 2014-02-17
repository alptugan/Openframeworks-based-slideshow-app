#include "ofMain.h"

class slide : public ofBaseImage {

	public:
		slide(){};
		~slide();
	  
		slide * load(string filePath);
		bool isVideo();
		int getWidth();
		int getHeight();

};


