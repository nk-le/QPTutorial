# Setup Platform IO Projects Using AMDC Libraries

The modules are tracked on the [local Gitlab server](http://gitlab.amdc.local/amdc-drone). 

Ask for the according access: Developer ... Maintainer ...

## IDE Setup
- [ ] Install [Visual Studio Code](https://code.visualstudio.com/)
- [ ] Install PlatformIO Extension. The version used for this instruction is [Core 6.1.6](https://docs.platformio.org/en/latest/core/history.html?utm_source=platformio&utm_medium=piohome))
- [ ] Install QP bundles (if you are developing a QP project) at [link](https://www.state-machine.com/). Current version: 7.x
- [ ] Ask for your access to our [Gitlab server](http://www.gitlab.amdc.local/) 
- [ ] Install Gitbash or equivalent git terminals for Git usage

## Native Arduino Project (No QP) 

Now we create an application project, e.g., a CCU in the folder 'app-cuav'

0. Take an arbitrary workspace folder, e.g., named `amdc-ws`

		amdc-ws/
    
1. Create a directory named `dependencies` and clone the common-modules into this folder.

		amdc-ws/
    		├── dependencies/
          		├── arduino_log.git/	
             	├── amdc_core.git/	
        		├── custom_mcu_drivers.git/       

2. Create a folder for your own project, e.g., for the command-control-unit in the folder `app-cuav` with the following minimal structure

        amdc-ws/
            ├── app-cuav/
            │   ├── lib/
            │   ├── include/
            │   ├── src/
            │   │   └── main.cpp
            │   ├── test/
            │   └── platformio.ini
            └── dependencies/
                └── .../

	The default platforimio.ini file should declare the basic MCU, Framework ... Use the following template for teensy4.1
					
      	[platformio]
		; Note: workspace_dir points (per default) to platformio.ini/.pio/
		
    	[env]
        framework = arduino
        platform = teensy
        board = teensy41
        lib_ldf_mode = chain+
        upload_protocol = teensy-gui

> For more info, please refer to ... Briefly, [env] declares the common configuration for all other environments (paths, compile flags, etc.). They can be overriden or extended in any particular environment.
	{.is-info}


3. Now we will configure your project dependencies in the `plarformio.ini` file. First, we create a working environment named `env_amdc_native`
		
    	[env_amdc_native]
        lib_extra_dirs = 
            ../dependencies/

>  The line that declares the search path for the source files is `lib_extra_dirs = ../dependencies/`. Don't forget it :D
  {.is-warning}

   
4. We should be able to compile a Hello-World project using the common modules already. Copy the following source code into the `main.cpp` of your project and upload it to the MCU

      	#include <Arduino.h>
        #include <MathCore/Types/BasicTypes.h>

        Position _posWgs84;
        Position_ECEF _posECEF;
        Velocity _velNED;
        Velocity_B _velBody;

        void setup() {
          Serial.begin(115200);
          Serial.println("Hello Hello!");
        }

        void loop() {}

	A successful compile means that we are able to include the libaries `MathCore`. Different libraries in the `Dependencies` can be declared and used similarly. 

    


## QP Dependent Projects

1. Clone the QP related libraries (modules, drivers, wrapper, etc.) into the `dependencies` folder.

		amdc-ws/
     		├── dependencies/
        		├── arduino_log.git/	
        		├── amdc_core.git/	
        		├── custom_mcu_drivers.git/   				
        		├── drone_logger.git/
        		├── DroneAPI.git/     /*TODO: Rename the git repo.*/  
        		└── drone_sensors.git/


2. The `plarformio.ini` file must be updated too! Create a new environment named [env_amdc_qp] and add the following line as follows

		[env_amdc_native]
			lib_extra_dirs =  ../dependencies/
			lib_deps = http://gitlab.amdc.local/amdc-drone/qp.git#master

> The `lib_deps` line tells platformio to clone the master (as declared #master) branch from the URL amdc-drone/qp.git. Then this libraries is compiled together afterwards.
{.is-info}

3. We should be able to compile a Hello-QP project using the common modules already. Copy the following source code into the `main.cpp` of your project and upload it to the MCU

        #include <Arduino.h>
        #include <ArduinoLog.h>
        #include <qpcpp.hpp>

        #include <QPT4Config/BoardConfig.hpp>
        #include <Sensors.h>
        #include <DatenLoggerManager.h>

        using namespace QP;
        using namespace BoardConfig;

        /* QP Modules */
        SBUSReceiver _RC;
        LW20 _Lidar;
        VN200 _IMU;
        DatenloggerManager _Logger;

        void setup() {
            // Initialized QP ...
            BoardConfig::init();

          	Serial.begin(115200);
          	Serial.println("Hello Hello!");

            _Logger.begin(&Serial5, 921600);
            _IMU.begin(&Serial4, 921600);
            _Lidar.begin(&Serial3, 115200);
        }


        void loop() {}




















