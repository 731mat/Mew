#include "../include/configuration.h"

#include <SmingCore/SmingCore.h>

Mew ActiveConfig;

Mew loadConfig()
{
	DynamicJsonBuffer jsonBuffer;
	Mew cfg;
	if (fileExist(MEW_CONFIG_FILE))
	{
		int size = fileGetSize(MEW_CONFIG_FILE);
		char* jsonString = new char[size + 1];
		fileGetContent(MEW_CONFIG_FILE, jsonString, size + 1);
		JsonObject& root = jsonBuffer.parseObject(jsonString);

		JsonObject& network = root["network"];
		cfg.NetworkSSID = String((const char*)network["ssid"]);
		cfg.NetworkPassword = String((const char*)network["password"]);

		JsonObject& za1 = root["zarizeni1"];
		cfg.z1.priorita = za1["priorita"];
		cfg.z1.hodnota = za1["hodnota"];
		cfg.z1.spotreba = za1["spotreba"];
        cfg.z1.faze = za1["faze"];
        cfg.z1.hysterze = za1["hysterze"];
        cfg.z1.stav = za1["stav"];
        cfg.z1.stavTime = za1["stavTime"];
        cfg.z1.ipAdd = za1["ipADD"];

        JsonObject& za2 = root["zarizeni2"];
        cfg.z2.priorita = za2["priorita"];
        cfg.z2.hodnota = za2["hodnota"];
        cfg.z2.spotreba = za2["spotreba"];
        cfg.z2.faze = za2["faze"];
        cfg.z2.hysterze = za2["hysterze"];
        cfg.z2.stav = za2["stav"];
        cfg.z2.stavTime = za2["stavTime"];
        cfg.z2.ipAdd = za2["ipADD"];
        
        JsonObject& za3 = root["zarizeni3"];
        cfg.z3.priorita = za3["priorita"];
        cfg.z3.hodnota = za3["hodnota"];
        cfg.z3.spotreba = za3["spotreba"];
        cfg.z3.faze = za3["faze"];
        cfg.z3.hysterze = za3["hysterze"];
        cfg.z3.stav = za3["stav"];
        cfg.z3.stavTime = za3["stavTime"];
        cfg.z3.ipAdd = za3["ipADD"];
		
        delete[] jsonString;
	}
	else
	{
		cfg.NetworkSSID = WIFI_SSID;
		cfg.NetworkPassword = WIFI_PWD;
	}
	return cfg;
}

void saveConfig(Mew& cfg)
{
	ActiveConfig = cfg;

	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();

	JsonObject& network = jsonBuffer.createObject();
	root["network"] = network;
	network["ssid"] = cfg.NetworkSSID.c_str();
	network["password"] = cfg.NetworkPassword.c_str();

	JsonObject& za1 = jsonBuffer.createObject();
	root["zarizeni1"] = za1;
    za1["priorita"] = cfg.z1.priorita;
    za1["hodnota"] = cfg.z1.hodnota;
    za1["spotreba"] = cfg.z1.spotreba;
    za1["faze"] = cfg.z1.faze;
    za1["hysterze"] = cfg.z1.hysterze;
    za1["stav"] = cfg.z1.stav;
    za1["stavTime"] = cfg.z1.stavTime;
    za1["ipAdd"] = cfg.z1.ipAdd;


    JsonObject& za2 = jsonBuffer.createObject();
    root["zarizeni2"] = za2;
    za2["priorita"] = cfg.z2.priorita;
    za2["hodnota"] = cfg.z2.hodnota;
    za2["spotreba"] = cfg.z2.spotreba;
    za2["faze"] = cfg.z2.faze;
    za2["hysterze"] = cfg.z2.hysterze;
    za2["stav"] = cfg.z2.stav;
    za2["stavTime"] = cfg.z2.stavTime;
    za2["ipAdd"] = cfg.z2.ipAdd;

    JsonObject& za3 = jsonBuffer.createObject();
    root["zarizeni3"] = za3;
    za3["priorita"] = cfg.z3.priorita;
    za3["hodnota"] = cfg.z3.hodnota;
    za3["spotreba"] = cfg.z3.spotreba;
    za3["faze"] = cfg.z3.faze;
    za3["hysterze"] = cfg.z3.hysterze;
    za3["stav"] = cfg.z3.stav;
    za3["stavTime"] = cfg.z3.stavTime;
    za3["ipAdd"] = cfg.z3.ipAdd;

	char buf[3048];
	root.prettyPrintTo(buf, sizeof(buf));
	fileSetContent(MEW_CONFIG_FILE, buf);
}


