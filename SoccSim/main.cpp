#include "VisionGen.hpp"
#include "Viewer.hpp"
#include "Physics/Env.hpp"
#include "Config.hpp"
#include "Radio.hpp"

#include <QApplication>

#include <NxPhysics.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	
	Env* env = new Env();
	env->start();
	
	char* configFile = 0;

	//loop arguments and look for config file
	for (int i=1 ; i<argc ; ++i)
	{
		if (strcmp(argv[i], "-c") == 0)
		{
			if (++i < argc)
			{
				configFile = argv[i];
			}
			else
			{
				printf ("Expected config file after -c parameter\n");
				return 0;
			}
		}
		else
		{
			printf("%s is not recognized as a valid flag\n", argv[i]);
			return 0;
		}
	}
	
	Config* config = 0;
	
	if (configFile)
	{
		config = new Config(configFile, env);
	}
	
	VisionGen vision(env);
	vision.start();
	
	Radio radioBlue(Blue, *env);
	Radio radioYellow(Yellow, *env);
	radioBlue.start();
	radioYellow.start();

	Viewer win(env);
	win.setVisible(true);

	int ret = app.exec();
	
	//cleanup
	delete env;
	vision.terminate();
	vision.wait();
	
	radioBlue.terminate();
	radioBlue.wait();
	radioYellow.terminate();
	radioYellow.wait();
	
	delete config;
	
	return ret;
}
