#include <stdio.h>

class ParamsLoader {

	public:
		ParamsLoader();
		~ParamsLoader();
	  
		bool ReadConfigFile(char *configfile, char **files, int *params, int numfiles, int numparams);

};