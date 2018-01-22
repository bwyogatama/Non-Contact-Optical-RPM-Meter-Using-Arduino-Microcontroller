#include<LiquidCrystal.h>

LiquidCrystal lcd(13, 12, 11, 10, 9, 8);    // definisi pin LCD

volatile byte REV;       //  volatile data untuk menyimpan revolusi
 
unsigned long int rpm, maxRPM;  //  deklarasi rpm dan maksimum rpm
 
unsigned long time;         //  lamanya waktu untuk 1 revolusi
 
int ledPin = 7;           //   status led
 
int led = 0,RPMlen , prevRPM;  //  nilai led saat ini, rpm saat ini, dan rpm sebelumnya
 
int flag = 0;             //  menentukan apakah layar LCD perlu diclear atau tidak

long prevtime = 0;       
    
 void setup()
 {
     Serial.begin(9600);   // inisialisasi serial monitor
     
     lcd.begin(16, 2);     // inisialisasi LCD
     
     attachInterrupt(digitalPinToInterrupt(2), RPMCount, RISING);     //  HIGH PRIORITY ACTION ( AN INTERRUPT)  ketika sensor berubah dari low ke high
     
     REV = 0;      //  inisialisasi variabel
     
     rpm = 0;

     maxRPM = 0;
     
     time = 0;
     
     pinMode(ledPin, OUTPUT);
     
     pinMode(3, OUTPUT);           
     
     pinMode(4, OUTPUT);
     
     digitalWrite(3, HIGH);             //  VCC PIN untuk sensor
     
     digitalWrite(4, LOW);              // GROUND PIN untuk sensor
     
     lcd.print("RPM METER");           //   teks yang muncul pertama kali
     lcd.setCursor(0, 1);
     lcd.print("-BWY71,JL11-");          //  identitas perancang
     delay(2000);
     lcd.clear();
 }
 
 void loop()
 {
  long currtime = millis();                 // mendapatkan waktu saat ini
  
  long idletime = currtime - prevtime;        //  menghitung idle time
    
    if(REV >= 4 )                  //  apabila REV lebih dari 4 kali terdeteksi
   {
     
             
     if(flag==0)                     //  lcd clear
     {
       lcd.clear();
       lcd.print("MEASURING ...");
       flag=1;                          
     }
     
     rpm = 60/2*1000/(millis() - time)*(REV);       //  menghitung rpm
     
     if(rpm > maxRPM)
     maxRPM = rpm;                             //  menghitung max rpm
    
     time = millis();                            
     
     REV = 0;
     
     int x= rpm;                                //  menghitung jumlah digit pada rpm
     while(x!=0)
     {
       x = x/10;
       RPMlen++;
     }       
          
     
     
     if(RPMlen!=prevRPM)                             
     {
       lcd.clear();
       prevRPM = RPMlen;
       flag=0;
       lcd.print("MEASURING ...");
     }
     
     lcd.setCursor(0, 1);
     lcd.print(rpm,DEC);                        //  menampilkan rpm dalam format desimal pada LCD
     
     lcd.setCursor(6,1);
     lcd.print("RPM");
     delay(500);
     
     prevtime = currtime;                        // reset perhitungan idle time
    
   }
   
   if(idletime > 5000 )                      //  apabila tidak ada pembacaan selama 5 detik, maka kondisi berikut akan dijalankan
   {
     
     if(flag==1)                            // clear LCD
     {
       lcd.clear();
       flag=0;
     }
     
     lcd.clear();
     lcd.print("MAX RPM");
     lcd.setCursor(0, 1);
     lcd.print(maxRPM,DEC);                     // menampilkan maksimum rpm
     lcd.print("   RPM");
     delay(2000);
     lcd.clear();
     lcd.print("IDLE ...");
     lcd.setCursor(0, 1);
     lcd.print("I AM READY");
     delay(2000);
     prevtime = currtime;
     Serial.println(digitalRead(2));
   }
     
 }
 
 void RPMCount()                                // setiap kali sensor mengalami perubahan logika dari LOW ke HIGH 
 {
   REV++;                                         // meningkatkan revolusi
   
   if (led == LOW)
   {
     
     led = HIGH;                                 //  Toggle status LED
   } 
   
   else
   {
     led = LOW;
   }
   digitalWrite(ledPin, led);
 }

