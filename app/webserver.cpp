#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include "../include/configuration.h"

bool serverStarted = false;
HttpServer server;
extern String p1, p2,p3,i1,i2,i3,u1,u2,u3; // Sensors string values


void onIndex(HttpRequest &request, HttpResponse &response)
{
	TemplateFileStream *tmpl = new TemplateFileStream("index.html");
	auto &vars = tmpl->variables();
    vars["IP"] = WifiStation.getIP().toString();
    vars["MAC"] = WifiStation.getMAC();
    vars["P1"] = p1;
    vars["U1"] = u1;
    vars["I1"] = i1;

    vars["P2"] = p2;
    vars["U2"] = u2;
    vars["I2"] = i2;

    vars["P3"] = p3;
    vars["U3"] = u3;
    vars["I3"] = i3;
	response.sendTemplate(tmpl);
}
void onGraf(HttpRequest &request, HttpResponse &response)
{
    TemplateFileStream *tmpl = new TemplateFileStream("graf.html");
    response.sendTemplate(tmpl);
}

void onConfiguration(HttpRequest &request, HttpResponse &response)
{
	Mew cfg = loadConfig();
	if (request.getRequestMethod() == RequestMethod::POST)
	{
		debugf("Update config");
		// Update config
		if (request.getPostParameter("SSID").length() > 0) // Network
		{
			cfg.NetworkSSID = request.getPostParameter("SSID");
			cfg.NetworkPassword = request.getPostParameter("PASSWORD");
		}
		if (request.getPostParameter("PRIORITA1").length() > 0) // Correction
		{
            cfg.z1.priorita = request.getPostParameter("PRIORITA1").toInt();
			cfg.z1.hodnota = request.getPostParameter("HODNOTA1").toInt();
			cfg.z1.spotreba = request.getPostParameter("SPOTREBA1").toInt();
            cfg.z1.faze = request.getPostParameter("FAZE1").toInt();
            cfg.z1.hysterze = request.getPostParameter("HYSTERZE1").toInt();
            cfg.z1.ipAdd = request.getPostParameter("IPADD1").toInt();
		}
        if (request.getPostParameter("PRIORITA2").length() > 0) // Correction
        {
            cfg.z2.priorita = request.getPostParameter("PRIORITA2").toInt();
            cfg.z2.hodnota = request.getPostParameter("HODNOTA2").toInt();
            cfg.z2.spotreba = request.getPostParameter("SPOTREBA2").toInt();
            cfg.z2.faze = request.getPostParameter("FAZE2").toInt();
            cfg.z2.hysterze = request.getPostParameter("HYSTERZE2").toInt();
            cfg.z2.ipAdd = request.getPostParameter("IPADD2").toInt();
        }
        if (request.getPostParameter("PRIORITA3").length() > 0) // Correction
        {
            cfg.z3.priorita = request.getPostParameter("PRIORITA3").toInt();
            cfg.z3.hodnota = request.getPostParameter("HODNOTA3").toInt();
            cfg.z3.spotreba = request.getPostParameter("SPOTREBA3").toInt();
            cfg.z3.faze = request.getPostParameter("FAZE3").toInt();
            cfg.z3.hysterze = request.getPostParameter("HYSTERZE3").toInt();
            cfg.z3.ipAdd = request.getPostParameter("IPADD3").toInt();
        }

		saveConfig(cfg);
		response.redirect();
	}

	debugf("Send template");
	TemplateFileStream *tmpl = new TemplateFileStream("config.html");
	auto &vars = tmpl->variables();

    vars["SSID"] = cfg.NetworkSSID;
    vars["PASSWORD"] = cfg.NetworkPassword;

    vars["PRIORITA1"] = String(cfg.z1.priorita);
    vars["HODNOTA1"] = String(cfg.z1.hodnota);
    vars["SPOTREBA1"] = String(cfg.z1.spotreba);
    vars["FAZE1"] = String(cfg.z1.faze);
    vars["HYSTERZE1"] = String(cfg.z1.hysterze);
    vars["IPADD1"] = String(cfg.z1.ipAdd);

    vars["PRIORITA2"] = String(cfg.z2.priorita);
    vars["HODNOTA2"] = String(cfg.z2.hodnota);
    vars["SPOTREBA2"] = String(cfg.z2.spotreba);
    vars["FAZE2"] = String(cfg.z2.faze);
    vars["HYSTERZE2"] = String(cfg.z2.hysterze);
    vars["IPADD2"] = String(cfg.z2.ipAdd);

    vars["PRIORITA3"] = String(cfg.z3.priorita);
    vars["HODNOTA3"] = String(cfg.z3.hodnota);
    vars["SPOTREBA3"] = String(cfg.z3.spotreba);
    vars["FAZE3"] = String(cfg.z3.faze);
    vars["HYSTERZE3"] = String(cfg.z3.hysterze);
	vars["IPADD3"] = String(cfg.z3.ipAdd);

    //uloženíí
    response.sendTemplate(tmpl);
}

void onFile(HttpRequest &request, HttpResponse &response)
{
	String file = request.getPath();
	if (file[0] == '/')
		file = file.substring(1);

	if (file[0] == '.')
		response.forbidden();
	else
	{
		response.setCache(86400, true); // It's important to use cache for better performance.
		response.sendFile(file);
	}
}

///  API


void getDataJson(HttpRequest &request, HttpResponse &response)
{
    JsonObjectStream* stream = new JsonObjectStream();
    JsonObject& sensors = stream->getRoot();
    sensors["p1"] = p1.toInt();
    sensors["u1"] = u1.toInt();
    sensors["i1"] = i1.toInt();
    sensors["p2"] = p2.toInt();
    sensors["u2"] = u2.toInt();
    sensors["i2"] = i2.toInt();
    sensors["p3"] = p3.toInt();
    sensors["u3"] = u3.toInt();
    sensors["i3"] = i3.toInt();
    response.sendDataStream(stream, "");
}

int getVykonNaFazi(int faze){
    switch(faze){
        case 1:
            return p1.toInt();
        case  2:
            return p2.toInt();
        case 3:
            return p3.toInt();
    }
    Serial.println("------SPATNA FAZE----");
}




void getZarizeni(HttpRequest &request, HttpResponse &response)
{
    Mew cfg = loadConfig();
	response.setContentType(ContentType::HTML);
	if (request.getRequestMethod() == RequestMethod::GET)
	{
		if (request.getQueryParameter("id").length() > 0)
		{
			switch(request.getQueryParameter("id").toInt()){
				case 1:
                    if(getVykonNaFazi(cfg.z1.faze) >  cfg.z1.hodnota && cfg.z1.priorita != 0) {
                        if(cfg.z1.stav == true) {
                            response.sendString("1");
                            cfg.z1.stavTime = RTC.getRtcSeconds();
                        }else
                            if((RTC.getRtcSeconds() - cfg.z1.stavTime) > cfg.z1.hysterze) {
                                Serial.println("-------------TRUE--------------");
                                response.sendString("1");
                                cfg.z1.stav = true;
                                cfg.z1.stavTime = RTC.getRtcSeconds();
                            }else {
                                response.sendString("0");
                                Serial.println(RTC.getRtcSeconds() - cfg.z1.stavTime);
                            }
                    }else {
                        if(cfg.z1.stav == false) {
                            response.sendString("0");
                            cfg.z1.stavTime = RTC.getRtcSeconds();
                        }else
                        if((RTC.getRtcSeconds() - cfg.z1.stavTime) > cfg.z1.hysterze) {
                            Serial.println("-------------FALSE--------------");
                            response.sendString("0");
                            cfg.z1.stav = false;
                            cfg.z1.stavTime = RTC.getRtcSeconds();
                        }else {
                            response.sendString("1");
                            Serial.println(RTC.getRtcSeconds() - cfg.z1.stavTime);
                        }
                    }
					break;
				case 2:

                    break;
				case 3:
					break;
				default:
					response.sendString("0");
			}
            saveConfig(cfg);
		}else
            response.sendString("zadej paramter ?id=<id>");
	}
}


void startWebServer()
{
	if (serverStarted) return;

	server.listen(80);
	server.addPath("/", onIndex);
    server.addPath("/graf", onGraf);
	server.addPath("/config", onConfiguration);
    server.addPath("/getDataJson", getDataJson);
	server.addPath("/getZarizeni", getZarizeni);
	server.setDefaultHandler(onFile);
	serverStarted = true;

	if (WifiStation.isEnabled())
		debugf("STA: %s", WifiStation.getIP().toString().c_str());
	if (WifiAccessPoint.isEnabled())
		debugf("AP: %s", WifiAccessPoint.getIP().toString().c_str());
}

/// FileSystem Initialization ///

Timer downloadTimer;
HttpClient downloadClient;
int dowfid = 0;
void downloadContentFiles()
{
	if (!downloadTimer.isStarted())
	{
		downloadTimer.initializeMs(3000, downloadContentFiles).start();
	}

	if (downloadClient.isProcessing()) return; // Please, wait.
	debugf("DownloadContentFiles");

	if (downloadClient.isSuccessful())
		dowfid++; // Success. Go to next file!
	downloadClient.reset(); // Reset current download status

	if (dowfid == 0)
		downloadClient.downloadFile("http://simple.anakod.ru/templates/MeteoControl/MeteoControl.html", "index.html");
	else if (dowfid == 1)
		downloadClient.downloadFile("http://simple.anakod.ru/templates/MeteoControl/MeteoConfig.html", "config.html");
	else if (dowfid == 2)
		downloadClient.downloadFile("http://simple.anakod.ru/templates/MeteoControl/MeteoAPI.html", "api.html");
	else if (dowfid == 3)
		downloadClient.downloadFile("http://simple.anakod.ru/templates/bootstrap.css.gz");
	else if (dowfid == 4)
		downloadClient.downloadFile("http://simple.anakod.ru/templates/jquery.js.gz");
	else
	{
		// Content download was completed
		downloadTimer.stop();
		startWebServer();
	}
}
