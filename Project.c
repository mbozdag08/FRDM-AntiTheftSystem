#include "mbed.h"
#include <string> 
#include "GSM.h"
#include "DHT22.h"
#include "GPS.h"
#include "GeoPosition.h"

DHT22 sensor(PTC7);
DigitalIn digital(PTA3);
AnalogIn analog(PTB0);
GPS cipies(PTE22,PTE23);

InterruptIn SMSInt(PTA1); // Interrupt on digital port.
Serial sim800l(PTE0,PTE1);                                   //Serial communication of the sim800 and pc.
Serial pc(USBTX,USBRX);
GSM sim(PTE0, PTE1, 9600, "+905355514040");
char masterSim[] = "+905355514040";

PwmOut rled(LED1);
PwmOut gled(LED2);
void SMSTeq(void);

bool tempHumCom, GPSCom, sysCond, meteksanBool;
char numTeq[13];
char x[300],y[30],z[30],l[30],m[30];
char *messageTeq;
double baseLat = 39.8720733;
double baseLong = 32.7508188;
char resp[20];
char resp1[20];
int maxTemp = 40;
int maxHum = 70;
int maxRain = 30;
int currentDistance = 0;
long t1 = 2;
long t2 = 2;
long h1 = 2;
long h2 = 2;
long r1 = 2;
long r2 = 2;
bool tbool, hbool, rbool = true;

GeoPosition baseCoord(baseLat, baseLong);

double meteksanLat = 39.8733138;
double meteksanLong = 32.7508379;

GeoPosition meteksanCoord( meteksanLat, meteksanLong);
GeoPosition currentCoord( baseLat, baseLong);

int alarmDistance = 150;
  
int main()
{
    pc.printf("Initialising...\n");
    SMSInt.fall(&SMSTeq);
    while(true)
    {
        
        pc.baud(9600);
        sim800l.baud(9600);
        if(sim.powerCheck() == 0)
            pc.printf("OKEY KARDESIM\r\n");
        else
            pc.printf("BASH\r\n\n");

        if(sim.settingSMS() == 0)
        pc.printf("SMS is set\n");
        else
        pc.printf("SMS BASH\n");
        cipies.sample();
        cipies.sample();
        cipies.sample();
        cipies.sample();
        cipies.sample();
        cipies.sample();
        cipies.sample();
        cipies.sample();
        cipies.sample();
        cipies.sample();
        cipies.sample();
        if( (26 < cipies.longitude) && (45 > cipies.longitude) && (cipies.latitude > 36) && (cipies.latitude < 42))
        {
            currentCoord.set(cipies.latitude, cipies.longitude);
        }
        pc.printf("\nCurrent Coordinates: %f,%f\n", cipies.latitude, cipies.longitude);
//        if(sim.sendSMS("+905350297766", "AIZIN ADAMINIZ ALMIYOR MU SES KAYDI") == 0)
//           pc.printf("Message sent.");
//       else
//           pc.printf("Message is bash.")
        
        sensor.sample();
        wait(1);
            int tempp = sensor.getTemperature();
            int humm= sensor.getHumidity();
            float aVall = analog*100;
            float rainpp = 100-aVall;
            float temp2 = tempp / 10.0;
            float hum2 = humm / 10.0;
            pc.printf("T:%f H:%f R:%f", temp2, hum2, rainpp);
        if(temp2 > maxTemp)
        {
                t1 = 0;
                t2++;
                // RED LED
                tbool = false;
                if( t2 == 1)
                {
                    char msgSend[64];
                    snprintf(msgSend, sizeof(msgSend), "Temperature exceeded, the system will be shut down.");
                    pc.printf("Temperature exceeded, the system will be shut down.");
                    if(sim.sendSMS(masterSim, msgSend) == 0)
                        pc.printf("Message is sent.\n");
                    else
                        pc.printf("Message is BASH.\n");
                }
            }
            else
            {
                t2 = 0;
                t1++;
                tbool = true;
                if( t1 == 1)
                {
                    char msgSend[64];
                    snprintf(msgSend, sizeof(msgSend), "Acceptable temperature detected, the system will be running.");
                    pc.printf("Acceptable temperature detected, the system will be running.");
                    if(sim.sendSMS(masterSim, msgSend) == 0)
                        pc.printf("Message is sent.\n");
                    else
                        pc.printf("Message is BASH.\n");
                }
            }
///////////////////////////////////////////////////////////////////////////
        if(hum2 > maxHum)
        {
                h1 = 0;
                h2++;
                // RED LED
                hbool = false;
                if( h2 == 1)
                {
                    char msgSend[64];
                    snprintf(msgSend, sizeof(msgSend), "Humidity exceeded, the system will be shut down.");
                    pc.printf("Humidity exceeded, the system will be shut down.");
                    if(sim.sendSMS(masterSim, msgSend) == 0)
                        pc.printf("Message is sent.\n");
                    else
                        pc.printf("Message is BASH.\n");
                }
            }
            else
            {
                h2 = 0;
                h1++;
                hbool = true;
                if( h1 == 1)
                {
                    char msgSend[64];
                    snprintf(msgSend, sizeof(msgSend), "Acceptable humidity detected, the system will be running.");
                    pc.printf("Acceptable humidity detected, the system will be running.");
                    if(sim.sendSMS(masterSim, msgSend) == 0)
                        pc.printf("Message is sent.\n");
                    else
                        pc.printf("Message is BASH.\n");
                }
            }
//////////////////////////////////////////////////////////////////
        if(rainpp > maxRain)
        {
                r1 = 0;
                r2++;
                // RED LED
                rbool = false;
                if( r2 == 1)
                {
                    char msgSend[64];
                    snprintf(msgSend, sizeof(msgSend), "Rain exceeded, the system will be shut down.");
                    pc.printf("Rain exceeded, the system will be shut down.");
                    if(sim.sendSMS(masterSim, msgSend) == 0)
                        pc.printf("Message is sent.\n");
                    else
                        pc.printf("Message is BASH.\n");
                }
            }
            else
            {
                r2 = 0;
                r1++;
                rbool = true;
                if( r1 == 1)
                {
                    char msgSend[64];
                    snprintf(msgSend, sizeof(msgSend), "Acceptable rain detected, the system will be running.");
                    pc.printf("Acceptable rain detected, the system will be running.");
                    if(sim.sendSMS(masterSim, msgSend) == 0)
                        pc.printf("Message is sent.\n");
                    else
                        pc.printf("Message is BASH.\n");
                }
            }
       
        if( (tbool == true) && (hbool == true) && (rbool == true) )
        {
            rled = 1;
            gled = 0;
        }
        else
        {
            rled = 0;
            gled = 1;
        }
        
        if( sysCond == true)
        {
            sysCond = false;
            int temp;
            int hum;
            if(sensor.sample())
            {
                temp = sensor.getTemperature();
                hum = sensor.getHumidity();
                float aVal = analog*100;
                float rainp = 100-aVal;
                pc.printf("\n\nTemperature: %0.1f %cC \r\n Humidity = %0.1f%c \r\n Rain Percentage = %0.2f%c\n\n", temp / 10.0, 176 ,hum / 10.0, 37 ,rainp, 37);
                float temp1 = temp / 10.0;
                float hum1 = hum / 10.0;
                char msgSend[256];
                snprintf(msgSend, sizeof(msgSend), "Temp = %0.1f  Max = %d \nHum = %0.1f  Max = %d \nRain = %0.2f  Max = %d \nDist = %d  Max = %d", temp1, maxTemp, hum1, maxHum, rainp, maxRain, currentDistance, alarmDistance);
                
                if(sim.sendSMS(numTeq, msgSend) == 0)
                    pc.printf("Message is sent.");
                else
                    pc.printf("Message is BASH.");
                
                
//          pc.printf("Rain Percentage = %0.2f%c", rainp, 37);
            }
            else
                pc.printf("Condition Measurement BASH\n");
        }
        
        
        if(GPSCom == true)
        {
            GPSCom = false;
            cipies.sample();
            cipies.sample();
            cipies.sample();
            cipies.sample();
            cipies.sample();
            cipies.sample();
            cipies.sample();
            cipies.sample();
            cipies.sample();
            cipies.sample();
            cipies.sample();
            if((26 < cipies.longitude) && (45 > cipies.longitude) && (cipies.latitude > 36) && (cipies.latitude < 42))
            {
                char msgSend[64];
                snprintf(msgSend, sizeof(msgSend), "%f, %f", cipies.latitude, cipies.longitude);
                pc.printf("Coordinates: %f,%f", cipies.latitude, cipies.longitude);
                if(sim.sendSMS(numTeq, msgSend) == 0)
                    pc.printf("Message is sent.\n");
                else
                    pc.printf("Message is BASH.\n");
            }
            else
            {
                pc.printf("Unable to lock the GPS.");
                char msgSend[64];
                snprintf(msgSend, sizeof(msgSend), "Unable to lock.\n");
                if(sim.sendSMS(numTeq, msgSend) == 0)
                    pc.printf("Message is sent.\n");
                else
                    pc.printf("Message is BASH.\n");
            }
        }
        else
        {
            pc.printf("GPS Command BASH\n");
        }
        
        
        if(tempHumCom == true)
        {
            pc.printf("Temp Hum\n\n");
            tempHumCom = false;
        }
        else
        {
            pc.printf("TempHum Command BASH\n\n");
        }
        
        if(meteksanBool == true)
        {
            meteksanBool = false;
            float distance;
            distance = currentCoord.distance(meteksanCoord);
            char msgSend[64];
            snprintf(msgSend, sizeof(msgSend), "Distance = %f", distance);
            if(sim.sendSMS(numTeq, msgSend) == 0)
                pc.printf("Message is sent.\n");
            else
                pc.printf("Message is BASH.\n");
        }
        currentDistance = baseCoord.distance(currentCoord);
        while( currentDistance > alarmDistance )
        {
            cipies.sample();
            cipies.sample();
            cipies.sample();
            cipies.sample();
            cipies.sample();
            cipies.sample();
            cipies.sample();
            cipies.sample();
            cipies.sample();
            cipies.sample();
            cipies.sample();
            if((26 < cipies.longitude) && (45 > cipies.longitude) && (cipies.latitude > 36) && (cipies.latitude < 42))
            {
                currentCoord.set(cipies.latitude, cipies.longitude);
                currentDistance = baseCoord.distance(currentCoord);
                pc.printf("Current Distance: %d\n", currentDistance);
                pc.printf("%f, %f\n", currentCoord.latitude(), currentCoord.longitude());
                char msgSend[64];
                snprintf(msgSend, sizeof(msgSend), "%f, %f", currentCoord.latitude(), currentCoord.longitude());
                if(sim.sendSMS(masterSim, msgSend) == 0)
                    pc.printf("Message is sent.\n");
                else
                    pc.printf("Message is BASH.\n");
            }
            else
            {
                pc.printf("Unable to lock the GPS.");
                char msgSend[64];
                snprintf(msgSend, sizeof(msgSend), "Unable to lock.\nCoordinates: %f,%f", cipies.latitude, cipies.longitude);
                if(sim.sendSMS(masterSim, msgSend) == 0)
                    pc.printf("Message is sent.\n");
                else
                    pc.printf("Message is BASH.\n");
            }
                wait(15);
        }
    //    sim800l.printf("AT+CREG?\r\n");

   //     if(sim800l.readable())
    //    {
   //         sim800l.gets(resp, 20);
  //          sim800l.gets(resp1, 20);
  //      }
 //       pc.printf("%s\n", resp1);
        
        wait(1);
    }
}
void SMSTeq(void){
        char *numindex;
        char num[13];
        char msg[40];
        char msg2[30];
        char msg3[256];

        pc.printf("Interrupt...\n\n");
        if(sim800l.readable())
        {
            sim800l.gets(msg, 40);
            sim800l.gets(msg2, 30);
            sim800l.gets(msg3, 256);
        }
        if( (numindex = strstr(msg, "+90")) != NULL)
        {
        for(int i = 0; i <= 12; i++)
        {
            num[i] = numindex[i];
            pc.printf("%c", num[i]);
        }
        pc.printf("\n");
        for(int i = 0; i <= 12; i++)
        {
            numTeq[i] = num[i];
            pc.printf("%c", numTeq[i]);
        }
        pc.printf("\n");
    //    if(msg3 == "SA")
    //        if(sim.sendSMS(numTeq, "AS") == 0)
    //           pc.printf("Message sent.");
    //       else
    //           pc.printf("Message is bash.");
            
        
        pc.printf("%s\r\n%s\n%s\n%s\n\n", msg, msg2, msg3, numTeq);
        if(  (strstr(msg3, "TempHum?") != NULL) || (strstr(msg3, "TEMPHUM?") != NULL) )
        {
            tempHumCom = true;
        }
        else
        {
            tempHumCom = false;
        }
        if( (strstr(msg3, "GPS?") != NULL) || (strstr(msg3, "gps?") != NULL) )
        {
            GPSCom = true;
        }
        else
        {
            GPSCom = false;
        }
        if( (strstr(msg3, "SysCond?") != NULL) || (strstr(msg3, "SYSCOND?") != NULL) )
        {
            sysCond = true;
        }
        else
        {
            sysCond = false;
        }
        if( (strstr(msg3, "METEKSAN?") != NULL) || (strstr(msg3, "meteksan?") != NULL) )
        {
            meteksanBool = true;
        }
        else
        {
            meteksanBool = false;
        }
        char *base;
        if( ((base = strstr(msg3, "setbase")) != NULL) || ((base =  strstr(msg3, "SETBASE")) != NULL) )
        {
            base = base + 8;
            char recLat[10];
            char recLong[10];
            for(int i = 0; i < 10; i++)
            {
                recLat[i] = base[i];
            }
            base = base + 12;
            for(int i = 0; i < 10; i++)
            {
                recLong[i] = base[i];
            }
            float newLat = strtof(recLat, NULL);
            float newLong = strtof(recLong, NULL );
            pc.printf("%s, %s\n", recLat, recLong);
            pc.printf("%f, %f\n", newLat, newLong);
            baseCoord.set(newLat, newLong);
            pc.printf("New Base: %f, %f", baseCoord.latitude(), baseCoord.longitude());
        }
        char *alarmDist;
        if( ((alarmDist = strstr(msg3, "setdist")) != NULL) || ((alarmDist =  strstr(msg3, "SETDIST")) != NULL) )
        {
            alarmDist = alarmDist + 8;
            char recDist[3];
            for(int i = 0; i <= 2; i++)
            {
                recDist[i] = alarmDist[i];
            }
            alarmDistance = strtof(recDist, NULL);
            pc.printf("New Distance: %d\n", alarmDistance);
        }
        char *alarmTemp;
        if( ((alarmTemp = strstr(msg3, "settemp")) != NULL) || ((alarmTemp =  strstr(msg3, "SETTEMP")) != NULL) )
        {
            alarmTemp = alarmTemp + 8;
            char recTemp[3];
            for(int i = 0; i <= 2; i++)
            {
                recTemp[i] = alarmTemp[i];
            }
            maxTemp = strtof(recTemp, NULL);
            pc.printf("New Temperature: %d\n", maxTemp);
        }
        char *alarmHum;
        if( ((alarmHum = strstr(msg3, "sethum")) != NULL) || ((alarmHum =  strstr(msg3, "SETHUM")) != NULL) )
        {
            alarmHum = alarmHum + 7;
            char recHum[3];
            for(int i = 0; i <= 2; i++)
            {
                recHum[i] = alarmHum[i];
            }
            maxHum = strtof(recHum, NULL);
            pc.printf("New Humidity: %d\n", maxHum);
        }
        char *alarmRain;
        if( ((alarmRain = strstr(msg3, "setrain")) != NULL) || ((alarmRain =  strstr(msg3, "SETRAIN")) != NULL) )
        {
            alarmRain = alarmRain + 8;
            char recRain[3];
            for(int i = 0; i <= 2; i++)
            {
                recRain[i] = alarmRain[i];
            }
            maxRain = strtof(recRain, NULL);
            pc.printf("New Rain: %d\n", maxRain);
        }
    }
//else  
//{
//    pc.printf("GSM REGISTER BASH\n");
//}

//    if(sim.readSMS(messageTeq, 0) == 0)
//    {
//        pc.printf("Message is read.\n");
//        pc.printf("%s\n", messageTeq);
//    }
//    else
//        pc.printf("Message reading BASH.\n");
}
