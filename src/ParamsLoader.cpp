#include "ParamsLoader.h"

ParamsLoader::ParamsLoader(void) 
{
}
//---------------------------------------------------------------------------
ParamsLoader::~ParamsLoader(void)
{
}

bool ParamsLoader::ReadConfigFile(char *configfile, char **files, int *params, int numfiles, int numparams)
{
	FILE *fc=fopen(configfile,"r");
	if (fc)
	{
		for (int i=0;i<numfiles;i++) 
		{
			fscanf(fc,"%s",files[i]);
		}
		for (int i=0;i<numparams;i++) 
		{
			int rc = fscanf(fc,"%d",&params[i]);

			if (rc != 1)
			{
				params[i]=0;	//default value
			}
		}
		fclose(fc);
		return true;
	}
	return false;
}