#include <ESP8266.h>
#include <ESP8266Client.h>

#include <DFPlayer_Mini_Mp3.h>

#include <SoftwareSerial.h>

#define SSID "lovinghouse" //와이파이 아이디
#define PASS "20200812" //와이파이 비밀번호

#define SERVER_URL "13.124.23.169" // 접속할 URL주소

unsigned int BUS_STATION_LIST_F[] = {11200,11200,11200,11200,11200,11200,11200,11200,11200,11200,11300,11300,11300,11300,11300,11300,11300,11200,10100,10000,10000,10000,10000,10000,10000,10000,10000,10000,10000,10500,10500,10500,10500,10500,10300,10300,10300,10400,10400,10400,10400,10400,10400,10400,10400,10400,10400,10400,10400,10400,10400,10400,10400,10400,10500,10500,10500,10500,10500,10500,10500,10000,10000,10000,10000,10000,10000,10000,10000,10000,10000,10100,11200,11300,11300,11300,11300,11300,11300,11200,11200,11200,11200,11200,11200,11200,11200,11200,11200,11200};
unsigned int BUS_STATION_LIST_B[] = {10202,10092,10100,10102,10106,10097,10099,10005,10007,10027,10135,10421,10420,10419,10418,10476,10477,10430,10304,10380,10384,10386,10388,10390,10392,10394,10396,10398,10165,10077,10076,10075,10017,10004,10001,10002,10003,10002,10160,10159,10158,10134,10135,10136,10125,10121,10110,10111,10304,10139,10141,10047,10048,10001,10003,10002,10001,10005,10006,10007,10008,10367,10365,10397,10395,10393,10391,10389,10387,10385,10379,10305,10429,10478,10475,10411,10417,10416,10415,10064,10028,10026,10024,10098,10432,10105,10103,10101,10181,10091};
SoftwareSerial esp8266Serial = SoftwareSerial(2, 3);
ESP8266 wifi = ESP8266(esp8266Serial);

String isCalled;
String location;
String stnNm;

void setup()
{

  Serial.begin(9600);
  digitalWrite(7,HIGH);
  delay(1000);
  pinMode(7,OUTPUT);

  isCalled = "false";



}
void loop()
{
  if(isCalled == "false"){
    // ESP8266
  esp8266Serial.begin(9600);

    wifi.begin();
    wifi.setTimeout(1000);
   // restart
    Serial.print("restart: ");
    Serial.println(getStatus(wifi.restart()));
  
    // joinAP
    Serial.print("joinAP: ");
    Serial.println(getStatus(wifi.joinAP(SSID, PASS)));
  
    // connect
    Serial.print("connect: ");
    Serial.println(getStatus(wifi.connect(ESP8266_PROTOCOL_TCP, SERVER_URL, 80)));
    delay(6000);
    // send
    Serial.print("send: ");   
    String url = "GET /check/?vehId=112029296 HTTP/1.1\r\nHost: ";  
    /*
    String u = "GET /check/?vehId=";
    String r = "124031169";
    String l = " HTTP/1.1\r\nHost: ";
    String url = u + r + l;  
    */
    /*String url = "GET /check/?vehId=";
    url = url.concat("124031169");
    url = url.concat(" HTTP/1.1\r\nHost: ");*/
    
    url.concat(SERVER_URL);
    url.concat("\r\nConnection: close\r\n\r\n");
    Serial.println(getStatus(wifi.send(url)));
  
    delay(100);
   
    if (wifi.available()) {
      String line = wifi.readStringUntil('}');
      int i;
      int index[12];
      index[0]=line.indexOf("&#x27");
  
  
  
        for(i=1;i<12;i++){
          index[i] = line.indexOf("&#x27;", index[i-1]+1);
          } 
  
        isCalled = line.substring(index[2]+6,index[3]);
        
        Serial.println(isCalled);
        
        location = line.substring(index[6]+6,index[7]);
        Serial.println(location);
          
        stnNm = line.substring(index[10]+6,index[11]);
        Serial.println(stnNm);
  
  
        /*
        여기에 코드 작성 */
  
   
        }
        delay(5000);
        if(isCalled == "false"){
        digitalWrite(7,LOW);
        }
  }
  else if(isCalled == "true"){
    mp3_set_serial (Serial);      // DFPlayer-mini mp3 module 시리얼 세팅
    delay(10);                    
    mp3_set_volume (30);         // 볼륨값 설정

    delay(10);
    //mp3_play (1); //0001 파일 플레이

    int index=-1;
    for(int i=0;i<200;i++){
      
      if(BUS_STATION_LIST_F[i] == location.substring(0,5).toInt() && BUS_STATION_LIST_B[i] == ("1"+location.substring(5,9)).toInt()){
        
          
          index = i;
          delay(100);
          break;
        }
      }
    Serial.println(index);
    
    mp3_play (index + 1);
    delay (10500);

    mp3_stop();

   
    

    digitalWrite(7,LOW);
    
    isCalled = "false";
    }
        
      


      
}


String getStatus(bool status)
{
  if (status)
    return "OK";

  return "NO";
}

String getStatus(ESP8266CommandStatus status)
{
  switch (status) {
    case ESP8266_COMMAND_INVALID:
      return "INVALID";
      break;

    case ESP8266_COMMAND_TIMEOUT:
      return "TIMEOUT";
      break;

    case ESP8266_COMMAND_OK:
      return "OK";
      break;

    case ESP8266_COMMAND_NO_CHANGE:
      return "NO CHANGE";
      break;

    case ESP8266_COMMAND_ERROR:
      return "ERROR";
      break;

    case ESP8266_COMMAND_NO_LINK:
      return "NO LINK";
      break;

    case ESP8266_COMMAND_TOO_LONG:
      return "TOO LONG";
      break;

    case ESP8266_COMMAND_FAIL:
      return "FAIL";
      break;

    default:
      return "UNKNOWN COMMAND STATUS";
      break;
  }
}
