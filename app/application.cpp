#include <user_config.h>
#include <SmingCore/SmingCore.h>

///////////////////////////////////////////////////////////////////
// Set your SSID & Pass for initial configuration
#include "../include/configuration.h" // application configuration
///////////////////////////////////////////////////////////////////

#include "special_chars.h"
#include "webserver.h"



// data parsování
String s = "";
String s2 = "{p1=111.1;i1=+2222.2;u1=+3333.3;p2=150.0;i2=1530.0;u2=1530.0;p3=1530.0;i3=1530.0;u3=1530.0;}";
String p1 = "";
String p2 = "";
String p3 = "";
String i1 = "";
String i2 = "";
String i3 = "";
String u1 = "";
String u2 = "";
String u3 = "";
///////////////////////////////////////////////////



IPAddress apIP(192, 168, 2, 50);

Timer procTimer;
Timer vypisovac;
Timer updateDevice;

HttpClient thingSpeak;

void process();
void connectOk();
void connectFail();

void rozparsuj(){
    p1 = s2.substring(s2.indexOf('=')+1,s2.indexOf(';')-1);
    s2 = s2.substring(s2.indexOf(';')+1);
    u1 = s2.substring(s2.indexOf('=')+1,s2.indexOf(';')-1);
    s2 = s2.substring(s2.indexOf(';')+1);
    i1 = s2.substring(s2.indexOf('=')+1,s2.indexOf(';')-1);
    s2 = s2.substring(s2.indexOf(';')+1);

    p2 = s2.substring(s2.indexOf('=')+1,s2.indexOf(';')-1);
    s2 = s2.substring(s2.indexOf(';')+1);
    u2 = s2.substring(s2.indexOf('=')+1,s2.indexOf(';')-1);
    s2 = s2.substring(s2.indexOf(';')+1);
    i2 = s2.substring(s2.indexOf('=')+1,s2.indexOf(';')-1);
    s2 = s2.substring(s2.indexOf(';')+1);

    p3 = s2.substring(s2.indexOf('=')+1,s2.indexOf(';')-1);
    s2 = s2.substring(s2.indexOf(';')+1);
    u3 = s2.substring(s2.indexOf('=')+1,s2.indexOf(';')-1);
    s2 = s2.substring(s2.indexOf(';')+1);
    i3 = s2.substring(s2.indexOf('=')+1,s2.indexOf(';')-1);
}

void onDataSent(HttpClient& client, bool successful)
{
	String response = client.getResponseString();
	Serial.println("Pokus o odeslani byl "+ response);
}

void vypis(){
	thingSpeak.downloadString("http://api.thingspeak.com/update?api_key=7JZ2DTO1WCWE892X&field1="+p1+"&field2="+u1+"&field3="+i1+"&field4="+p2+"&field5="+u2+"&field6="+i2+"&field7="+p3+"&field8="+u3, onDataSent);
}


void updateDev(){
    ActiveConfig = loadConfig();

    

    // vyber fazi
    for (int i = 0; i < 4; i++)
    {
       if (ActiveConfig.z1.priorita > ActiveConfig.z2.priorita)
           if (ActiveConfig.z1.priorita > ActiveConfig.z3.priorita){
            // priorita pro jednicku
           }else{
            // priorita pro trojku
           }
       else
            if (ActiveConfig.z2.priorita > ActiveConfig.z3.priorita){
                // priorita pro dvojku
            }else{
                // priorita pro trojku
            }
          
    }//end for    



    ActiveConfig.z3.priorita
}

void init()
{
	spiffs_mount(); // Mount file system, in order to work with files

	Serial.begin(9600); // 115200 by default
	Serial.systemDebugOutput(false); // Debug output to serial

	ActiveConfig = loadConfig();


	WifiStation.config(ActiveConfig.NetworkSSID, ActiveConfig.NetworkPassword);
	WifiStation.setIP(apIP);
	WifiStation.enable(true);
	WifiAccessPoint.enable(false);

	WifiStation.waitConnection(connectOk, 20, connectFail); // We recommend 20+ seconds for connection timeout at start

	procTimer.initializeMs(100, process).start();
        
	vypisovac.initializeMs(30000, vypis).start();

    updateDevice.initializeMs(3000, updateDev).start();

    rozparsuj();
}


void showValues()
{
    Serial.print("p1: " );
    Serial.print(p1);
    Serial.print("u1: " );
    Serial.print(u1);
    Serial.print("i1: " );
    Serial.println(i1);
}

void process()
{

    char pomoc[100];
    pomoc[0] = '\0';
    int i;
    if(i = Serial.available()){
        Serial.readMemoryBlock(pomoc,100);
        pomoc[i] = '\0';
    }
    if(pomoc[0] == '{'){
        Serial.println("--------------");
        rozparsuj();
        showValues();
        s2 = s;
        s = "";
    }
    s += String(pomoc);

}

void connectOk()
{
	debugf("connected");
	WifiAccessPoint.enable(false);
	Serial.print(WifiStation.getIP().toString());
	// Restart main screen output
	procTimer.restart();

	// At first run we will download web server content
	if (!fileExist("index.html") || !fileExist("config.html"))
		downloadContentFiles();
	else
		startWebServer();
}

void connectFail()
{
	debugf("connection FAILED");
	WifiAccessPoint.config("MeW", "", AUTH_OPEN);
	WifiAccessPoint.enable(true);
	// Stop main screen output
	procTimer.stop();
	Serial.println("WiFi MeW  ----- started");
    Serial.println(WifiAccessPoint.getIP());

	startWebServer();
	WifiStation.waitConnection(connectOk); // Wait connection
}
