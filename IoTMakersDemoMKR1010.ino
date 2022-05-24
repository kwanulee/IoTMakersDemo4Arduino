#include <Arduino.h>
#include <SPI.h>

#include "IoTMakers.h"
#include "Shield_Wrapper.h"
#include "DHT.h"    // 온습도 센서(DHT11)를 위한 라이브러리

/*
Arduino Shield
*/
Shield_Wrapper	g_shield;

#define SDCARD_CS	4
#define DHTPIN 2     // Digital pin connected to the DHT sensor

// Initialize DHT sensor.
DHT dht(DHTPIN, DHT11);

void sdcard_deselect()
{
	pinMode(SDCARD_CS, OUTPUT);
	digitalWrite(SDCARD_CS, HIGH); //Deselect the SD card
}
void init_shield()
{
	sdcard_deselect();
	
	const char* WIFI_SSID = "AndroidHotspot9328";
	const char* WIFI_PASS = "027605864";
	g_shield.begin(WIFI_SSID, WIFI_PASS);

	g_shield.print();
}


/*
IoTMakers
*/
IoTMakers g_im;

const char deviceID[]   = "kwanu7D1630827948677";
const char authnRqtNo[] = "hldfwk8g0";
const char extrSysID[]  = "OPEN_TCP_001PTL001_1000003350";

void init_iotmakers()
{
	Client* client = g_shield.getClient();
	if ( client == NULL )	{
		Serial.println(F("No client from shield."));
		while(1);
	}

	g_im.init(deviceID, authnRqtNo, extrSysID, *client);
	g_im.set_numdata_handler(mycb_numdata_handler);
	g_im.set_strdata_handler(mycb_strdata_handler);
	g_im.set_dataresp_handler(mycb_resp_handler);

	// IoTMakers 서버 연결
	Serial.println(F("connect()..."));
	while ( g_im.connect() < 0 ){
		Serial.println(F("retrying."));
		delay(3000);
	}

	// Auth

	Serial.println(F("auth."));
	while ( g_im.auth_device() < 0 ) {
		Serial.println(F("fail"));
		while(1);
	}

	//Serial.print(F("FreeRAM="));Serial.println(g_im.getFreeRAM());
}

#define LED_PIN		5

void setup() 
{
	Serial.begin(115200);
  	while ( !Serial )  {
	  ;
	}

	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, HIGH);

	init_shield();
	
	init_iotmakers();

	digitalWrite(PIN_LED, LOW);

  dht.begin();

}

void loop()
{
	static unsigned long tick = millis();

	// 10초 주기로 센서 정보 송신
	if ( ( millis() - tick) > 10000 )
	{
		send_temperature();

		tick = millis();
 	}
  
	// IoTMakers 서버 수신처리 및 keepalive 송신
	g_im.loop();
}


int send_temperature()
{ 
  float temperature = dht.readTemperature();
  
	Serial.print(F("Temperature (c): ")); Serial.println(temperature);
	if ( g_im.send_numdata("temperature", (double)temperature) < 0 ) {
  		Serial.println(F("fail"));  
		return -1;
	}
	return 0;   
}


void mycb_numdata_handler(char *tagid, double numval)
{
	// !!! USER CODE HERE
	//Serial.print(tagid);Serial.print(F("="));Serial.println(numval);
}
void mycb_strdata_handler(char *tagid, char *strval)
{
	// !!! USER CODE HERE
	//Serial.print(tagid);Serial.print(F("="));Serial.println(strval);
  
	if ( strcmp(tagid, "LED")==0 && strcmp(strval, "ON")==0 )  	
		digitalWrite(PIN_LED, HIGH);
	else if ( strcmp(tagid, "LED")==0 && strcmp(strval, "OFF")==0 )  	
		digitalWrite(PIN_LED, LOW);
}
void mycb_resp_handler(long long trxid, char *respCode)
{
	if ( strcmp(respCode, "100")==0 )
		Serial.println("resp:OK");
	else
		Serial.println("resp:Not OK");
}
