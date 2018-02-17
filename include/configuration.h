#ifndef INCLUDE_CONFIGURATION_H_
#define INCLUDE_CONFIGURATION_H_

#include <user_config.h>
#include <SmingCore/SmingCore.h>

// If you want, you can define WiFi settings globally in Eclipse Environment Variables
/**#ifndef WIFI_SSID
	#define WIFI_SSID "Net_Dolezel" // Put you SSID and Password here
	#define WIFI_PWD "24797399"
#endif
*/
#ifndef WIFI_SSID
    #define WIFI_SSID "AP Hlousek" // Put you SSID and Password here
    #define WIFI_PWD "tojetajne"
#endif


#define MEW_CONFIG_FILE "mew.conf" // leading point for security reasons :)

struct Zarizeni
{
	Zarizeni()
	{
		priorita = 0;
		hodnota = 0;
		spotreba = 0;
		faze = 1;
        hysterze = 10;
        stav = false;
        stavTime = 0;
        ipAdd = 0;
	}
	int priorita;
	int hodnota;
	int spotreba;
	int faze;
	int hysterze;
    bool stav;
    int stavTime;
    int ipAdd;
};


struct Mew
{
	Mew()
	{

	}

	String NetworkSSID;
	String NetworkPassword;

	Zarizeni z1;
    Zarizeni z2;
    Zarizeni z3;
};

Mew loadConfig();
void saveConfig(Mew& cfg);
extern Mew ActiveConfig;

#endif /* INCLUDE_CONFIGURATION_H_ */
