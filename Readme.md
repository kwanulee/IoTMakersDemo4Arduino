# IoTMakers와 아두이노 연동
본 자료는 http://iotmakers.kt.com/openp/index.html#/blog/blogDetail/10/KOR 의 내용을 바탕으로 작성되었습니다.

## 목표
- [1. 아두이노 MKR WIFI1010에서 LED 제어하기](#1)
- [2. 아두이노 MKR WIFI1010에서 DHT-11 센서를 활용한 온도 센서 값 수집하기](#2)
- [3. 아두이노 MKR WIFI1010에서 WIFI 테스트](#3)
- [4. IoTMakers와 아두이노 연동](#4)
- [5. IoTMakers를 통해 규칙에 의한 아두이노 LED on/off 제어하기](#5)

## 준비사항

- 아두이노 [MKR WiFi 1010](https://www.arduino.cc/en/Guide/MKRWiFi1010) 보드
- 브레드보드 1개
- LED 1개
- 저항 (330~1K 옴)  1개
- DHT11 (온/습도 센서) 1개
- 점퍼 케이블
- IoTMakers 계정 ( http://iotmakers.olleh.com/ )

<a name="1"></a>
## 1. 아두이노 MKR WIFI 1010 에서 LED 제어하기
### 1.1 하드웨어 구성
![](figures/arduino-led.png)

- **아두이노 MKR1010**의 **GND**를 **LED 음극**(캐소드, 짧은 단자)에 연결
- **LED 양극**(애노드, 긴 단자)을 **저항 (330~1K 옴)**에 연결
- 저항의 나머지 한 끝을 **아두이노 MKR1010**의 디지털입출력핀 **D5** 연결

### 1.2 MKR WIFI 1010 설정
- 	**아두이노 MKR1010** 보드를 사용하기 위해서는 **Arduino SAMD Boards**를 보드 매니저를 통해 설치해야 합니다.
	- **툴-보드-보드매니저**를 선택하여 보드 매니저를 실행한 후에, 검색 창에  *Arduino SAMD Boards*를 입력한 후, 검색된 보드를 설치한다.

	![](figures/mkr1010-board.png)

### 1.3 스케치 코드 실행 및 확인
- 디지털입출력 5번핀에 연결된 LED를 1초마다 깜박이게 하는 코드

	```c
	#define PIN_LED    5

	void setup() {
	  pinMode(PIN_LED, OUTPUT);
	}

	void loop() {
	  digitalWrite(PIN_LED,HIGH);
	  delay(1000);
	  digitalWrite(PIN_LED,LOW);
	  delay(1000);
	}
	```
- LED가 1초마다 깜박이는 지를 확인해 본다.

<a name="2"></a>
## 2. 아두이노 MKR WIFI1010에서 DHT-11 센서를 활용한 온도 센서 값 수집하기

### 2.1 DHT-11 센서
- 아두이노와 연결하기도 쉬워 아두이노의 기초를 배우거나 프로젝트를 진행 때 자주 사용되는 온도, 습도 센서입니다.
- 센서 자체의 측정 범위는 매우 높지는 않지만 일반적인 상황 (집안의 온도를 측정하는 등)에서 사용하기는 적합한 제품입니다.

### 2.2 하드웨어 구성
![](figures/arduino-dht11.png)

- **아두이노 MKR1010**의 **5V**를 **DHT-11**의 **VCC** 핀과 연결
- **아두이노 MKR1010**의 **D2** (디지털입출력핀 2)를 **DHT-11**의 **Data** 핀과 연결
- **아두이노 MKR1010**의 **GND**를 D**DHT-11**의 **GND** 핀과 연결

**[주의]**  **DHT-11**의 **VCC**, **Data**, **GND** 핀의 위치가 다소 차이가 있을 수 있음

### 2.3 관련 라이브러리 설치
- **DHT-11**을 사용하기 위해서는 다음 두가지 라이브러리가 설치되어야 합니다.
	- Adafruit Unified Sensor
	- DHT sensor library
- 라이브러리 설치 방법
	- **Arduino IDE**의 **스케치-라이브러리 포함하기-라이브러리 관리** 메뉴 클릭
	- 검색 창에서 위의 두가지 라이브러리를 검색하여 설치

		![](figures/install-library.png)

### 2.4 DHTTester 예제
- 앞서 언급한 DHT sensor library를 설치한 후, [**파일**]-[**예제**]-[**DHT sensor library**]-[**DHTTest.ino**]을 열어 **DHT-11** 센서를 테스트 할 수 있다.
- [**주의**] **DHTTest.ino** 예제에서 **#define DHTTYPE DHT11**의 주석을 풀고, **#define DHTTYPE DHT22** 부분을 주석처리 해야 함.

	```
	#define DHTTYPE DHT11   // DHT 11
	//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
	```
- (수정된) 예제 코드

```c
#include "DHT.h"

#define DHTPIN 2     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
}
```

### 2.4 실행결과 확인
- 시리얼 모니터 창을 열어 결과를 확인해 본다.
	![](figures/dht-11_result.png)

<a name="3"></a>
## 3. 아두이노 MKRWIFI1010에서 WIFI 테스트
### 3.1 **WIFININA** 라이브러리 추가
- **MKRWIFI1010**에서 WiFi를 사용하기 위해서는 **WIFININA** 라이브러리가 추가되어야 한다.
	- **Arduino IDE**의 [**스케치**]-[**라이브러리 포함하기**]-[**라이브러리 관리**] 메뉴를 선택
	- 라이브러리 매니저의 검색창에 **WIFININA**를 입력하여 검색된 **WIFININA** 라이브러리를 설치


	![](figures/WiFiNINA_LibMan.png)

### 3.2 WIFI 테스트 예제 실행
- **[파일]-[예제]-[WIFININA]**에 있는 다양한 WIFI 테스트 예제를 실행하여 와이파이 통신이 잘 되는 지를 확인해 본다.
	1. **[파일]-[예제]-[WIFININA]-[WiFiPing]**을 선택
	2.  ardunio_secrets.h 탭을 클릭하여 SECRET\_SSID와  SECRET\_PASS의 값을 자신의 무선환경에 맞게 설정
	3.  프로그램을 보드에 업로드 후에, 시리얼 모니터 창을 열어 결과를 확인

	![](figures/wifiping_test.png)

<a name="4"></a>
## 4. IoTMakers와 아두이노 연동

### 4.1 하드웨어 구성
![](figures/arduino-dht11-led.png)

1. LED 제어 배선
	- **아두이노 MKRWIFI1010**의 **GND**를 **LED 음극**(캐소드, 짧은 단자)에 연결
	- **LED 양극**(애노드, 긴 단자)을 **저항 (330~1K 옴)**에 연결
	- 저항의 나머지 한 끝을 **아두이노 MKR1010**의 디지털입출력핀 **D5** 연결

2. DHT-11 온습도 센서
	- **아두이노 MKRWIFI1010**의 **5V**를 **DHT-11**의 **VCC** 핀과 연결
	- **아두이노 MKRWIFI1010**의 **D2** (디지털입출력핀 2)를 **DHT-11**의 **Data** 핀과 연결
	- **아두이노 MKRWIFI1010**의 **GND**를 **DHT-11**의 **GND** 핀과 연결

### 4.2 아두이노/IoTMakers 연결: 디바이스 등록
1. 실습을 위한 준비를 마쳤다면 IoTMakers에 접속해서 디바이스를 등록해보자.
처음 가입하면 다음과 같이 디바이스 등록 화면을 볼 수 있다.

	![](http://iotmakers.kt.com/share/file/blog/201606/02/16060213160500.png)

2. 디바이스 명을 입력하고 프로토콜 유형을 선택한다.

	![](http://iotmakers.kt.com/share/file/blog/201606/02/16060213163101.png)

3. 디바이스 연결 상태: OFF 확인

	![](http://iotmakers.kt.com/share/file/blog/201606/02/16060213165905.png)

4. 디바이스 상세 정보를 미리 확인해 둔다.
	- 디바이스 아이디
	- 디바이스 패스워드
	- Gateway 연결 ID

	![](http://iotmakers.kt.com/share/file/blog/201606/02/16060213173206.png)

### 4.3 아두이노/IoTMakers 연결: SDK 설치
- **IoTMakers SDK(Software Development Kit)**는 IoT디바이스가 IoTMakers플랫폼에 연동할 때에 필요한 **API(Application Programming Interface)**를 제공한다.
- IoT디바이스는 제공되는 API를 통하여 **접속 및 장비인증, 수집데이터 전송하고, 플랫폼으로부터 제어데이터를 수신**한다.
- IoTMakers(iotmakers.olleh.com) → **IoT개발** → **나의 디바이스** →  등록된 디바이스 선택 → SDK - SDK : SDK_Arduino.zip (현재 버전 2.0.1) 다운로드 한다.	
- 다운로드 받은 zip 파일을 풀면, IoTMakers 폴더가 있다. 이를 사용자의 아두이노 라이브러리 폴더 하위로 옮긴다.
	- MS Windows

		My Documents\Arduino\libraries\IoTMakers

	- Mac OS X

		~/Documents/Arduino/libraries/IoTMakers
	- Ununtu

		~/Documents/Arduino/libraries/IoTMakers
- [**주의**] https://iotmakers.kt.com/openp/assets/files/SDK_Arduino_2.0.1.zip 에 있는 Arduino SDK를 최신버전의 Arduino IDE 2.0 및 MKRWiFi1010 보드에서 컴파일하면 컴파일 오류가 발생합니다. 이를 해결하기 위해서는 src/packet 하위의 body.c와 head.c 파일을 열고 상단에 #include \<avr/pgmspace.h\> 를 추가하면 된다.

다음 첨부된 Arduino SDK는 이를 수정한 버전이니,  zip 파일을 풀어 생성된 IoTMakers 폴더를 기존에 설치된 IoTMakers 폴더와 대체하면 됩니다. 

### 4.4 아두이노/IoTMakers 연결: IoTMakersDemo 실행
1. [IoTMakersDemo4Arduino](https://github.com/kwanulee/IoTMakersDemo4Arduino/archive/refs/heads/main.zip)을 다운로드하여 Arduino IDE에서 실행한다.

2.  소스코드에서 다음을 찾아 사용 환경에 맞도록 수정후, 빌드/업로드 한다.

	- WIFI\_SSID = 무선랜아이디
	- WIFI\_PASS = 무선랜패스워드
	- deviceID = 디바이스아이디
	- authnRqtNo = 디바이스패스워드
	- extrSysID = gateway 연결ID

3. 아두이노 IDE의 시리얼 모니터를 열고, 접속이 제대로 이루어지는 지 확인해 본다.
	![](figures/iotmakers_arduino_run_serail.png)


### 4.5 아두이노/IoTMakers 연결: 상태확인 (ON)

![](http://iotmakers.kt.com/share/file/blog/201606/02/16060213203502.png)

### 4.6 IoTMakers에서 LED on/off 제어

#### 4.6.1 LED 제어태그 등록
- IoTMakers에서 앞서 등록한 디바이스에 LED on/off를 위한 제어태그를 등록한다.
	- **Tag Stream ID**: *LED*
	- **Tag Stream Type**: *제어*
	- **Value Type**: *문자형식*

	![](http://iotmakers.kt.com/share/file/blog/201606/02/16060213222600.png)

- led=on 제어 전송 해보기

	![](http://iotmakers.kt.com/share/file/blog/201606/02/16060213223300.png)

#### 4.6.2 LED제어 소스코드

```c
// ... 생략
#define PIN_LED		5

// ... 생략

void mycb_strdata_handler(char *tagid, char *strval)
{
	// !!! USER CODE HERE
	//Serial.print(tagid);Serial.print(F("="));Serial.println(strval);

	if ( strcmp(tagid, "LED")==0 && strcmp(strval, "ON")==0 )  	
		digitalWrite(PIN_LED, HIGH);
	else if ( strcmp(tagid, "LED")==0 && strcmp(strval, "OFF")==0 )  	
		digitalWrite(PIN_LED, LOW);
}
```

### 4.7 IoTMakers를 통해 온도값 수집

#### 4.7.1 온도 수집태그 등록
- IoTMakers에서 앞서 등록한 디바이스에 온도수집을 위한 수집태그를 등록한다.
	- **Tag Stream ID**: *temperature*
	- **Tag Stream Type**: *수집*
	- **Value Type**: *숫자형식*

![](http://iotmakers.kt.com/share/file/blog/201606/02/16060213240001.png)

#### 4.7.2 온도 수집 소스코드
```c
\\… 생략 …

void loop()
{
	static unsigned long tick = millis();

	// 3초 주기로 센서 정보 송신
	if ( ( millis() - tick) > 3000 )
	{
		digitalWrite(PIN_LED, HIGH);
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

\\… 생략 …
```

#### 4.7.3 IoTMakers를 통해 온도값 수집: 태그스트림 확인

![](http://iotmakers.kt.com/share/file/blog/201606/02/16060213251005.png)

<a name="5"></a>
## 5. IoTMakers를 통해 규칙에 의한 아두이노 LED on/off 제어하기
- **IoTMakers**를 통해 등록한 디바이스의 태그스트림을 활용하여 이벤트를 생성하고, 이벤트 발생 시 액션을 취하게 끔 나만의 시나리오를 작성할 수 있다.
- 본 예제에서는 온도 센서로부터 수집된 값이 특정 온도 (예, 31도) 보다 높으면 LED를 켜는 시나리오를 구현해 볼 것이다.

### 5.1 디바이스의 **Event** 관리
- **Event** 등록 절차
	1. **[이벤트 관리](http://iotmakers.kt.com/openp/index.html#/event)** 화면의 **이벤트 등록** 버튼 클릭
		![](http://iotmakers.kt.com/share/file/blog/201604/15/16041514133004.png)
	2. **디바이스데이터**를 캔버스에 drag&drop 한 후, **수정**버튼을 누른 후에 이벤트를 발생시킬 디바이스를 선택한 다음에 **저장**버튼을 클릭
		![](http://iotmakers.kt.com/share/file/blog/201604/15/16041514160200.png)
	3. **이벤트 정의**를 캔버스에 drag&drop 한 후, **수정**버튼을 누른 후에 다음과 같이 이벤트 정의를 입력 후에 **저장**버튼을 클릭
		![](http://iotmakers.kt.com/share/file/blog/201604/15/16041514162605.png)
		- 이벤트 구분: *경고*
		- 이벤트 명: *온도가 31도보다 높으면 경고 표시*
	4. 디바이스로부터 데이터를 수집하여 이벤트를 발생시키기 위하여 디바이스 박스와 이벤트 정의박스를 연결시킨다.
		![](http://iotmakers.kt.com/share/file/blog/201604/15/16041514171502.png)
	5. 	**이벤트 발생조건** 추가/생성
		- 이벤트 발생 조건은 이벤트 정의 박스 내에 위치시켜야 한다.
   		- ‘온도가 31도보다 높으면’이라는 이벤트 조건을 만들기 위해 논리식 입력기를 사용하여 다음과 같은 논리식을 만들어보자.  

			```
			디바이스명.temperature > 31
			```
			![](figures/event_register.png)
	6. 이벤트가 발생하고 난 후에 디바이스를 제어하기 위해서, **제어**를 캔버스에 drag&drop 한 후, 이벤트 정의와 연결합니다.
		![](figures/device_control_start.png)
	7. **제어**박스의 **수정**버튼을 누른 후에 다음과 같이 제어처리 대상을 추가 후에 **확인**버튼을 클릭
		![](figures/device_control.png)


### 5.3 결과 확인
- **이벤트 관리** 화면의 목록에서 해당 이벤트를 클릭한 후 로그 보기 탭을 선택하면 이벤트 발생 로그를 확인할 수 있다.
- 온도가 31도 이상일 때 이벤트가 발생되는 것을 확인 할 수 있다.
	![](http://iotmakers.kt.com/share/file/blog/201604/15/16041514224306.png)
- 온도가 31도 이상일 때, LED가 ON 되었는지를 확인한다.



## 추가 자료
- [Arduino SDK 메뉴얼](https://iotmakers.kt.com/portalapi/v1/dvlguide/582/imageDown?access_token=eyJhbGciOiJSUzI1NiJ9.eyJzdmNfdGd0X3NlcSI6bnVsbCwicHViX3RpbWUiOjE1NjcyODk3MjA1MjAsImF1dGhvcml0aWVzIjpbIlJPTEVfT1BFTkFQSSIsIlJPTEVfVVNFUiJdLCJwbGF0Zm9ybSI6IjNNUCIsInRoZW1lX2NkIjoiUFRMIiwiY2xpZW50X2lkIjoiTWpaaVpXTXhPR05pTmpnME5HVTJaV0psWXpZeFltVmhOVFpsT1dJeFpERXhORE15TWpBMk9UZzRPVFUwIiwiYXVkIjpbIklPVC1BUEkiXSwidW5pdF9zdmNfY2QiOiIwMDIiLCJzY29wZSI6WyJ0cnVzdCJdLCJkc3RyX2NkIjoiMDAxIiwiY29tcGFueSI6Ikt0IiwiZXhwIjoxNTY3ODg5NzIwLCJqdGkiOiJiYThlMTQ1OC02MmI1LTRkMGEtYTEyYy1iNTA5YzkwNWM4MjgifQ.eTSArN-2c9MjsVasiQ5i5kgWR5qVN3q1A0eh6U16uLs3fUUcrosVF9yXxD9OG70OWV5XvhHOG_yVEImSrQqnhOdkmiokrWtDAbzpPB6WHLS1AjG5GnR20kTiT-npSIfjwE1Qp3X5bsGzcUnEV7-6zpm7icN9vHxKzeW5ibOi5jhBNkuIBQOjhrpH4sY6lKiCLmOEOTJiVRiPKFfuMJyM3zEuHL1t1c7FmXjxYhU0W9RtFDMY_W8cqmKya3irBMLd2tSqtozibIvGjrTp5-0XYLrktppbkw3UVwjyKfhK8Jh2ax9VxMoJHfqC9swHekxjw9mghBVIDk9Fp-Re5wLkFA)
