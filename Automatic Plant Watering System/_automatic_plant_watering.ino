const int ANALOG_INPUT = A2; 
const int sensorpin = A0;
const int sensorpower = 8;
const int LED1 = 2;//ROSU
const int LED2 = 3;
const int LED3 = 4;//VERDE
const int pumppin = 9;
int sensor = 0;
int state = 0;
const int delayTime = 100; 

int wet = 480;
int dry = 751;
                   
int    analog_output ;                                                      
int    revised_output;                                                      
float  temp_C ;  
double Temp ;

void setup(){ 
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(sensorpower,OUTPUT);
  pinMode(pumppin,OUTPUT);
  pinMode ( ANALOG_INPUT, INPUT )  ;  
  Serial.begin(9600);
}

void loop(){ 
  if(Serial.available()>0){
    state=Serial.read();
  }

analog_output = analogRead ( ANALOG_INPUT ) ;         
 revised_output= map ( analog_output, 0, 1023, 1023, 0 ) ;
 temp_C    = Thermistor ( revised_output ) ;
//
delay(10);

  if(state =='0' || (state=='1' && sensor<wet)){
    digitalWrite(pumppin,LOW);
    Serial.print(float(temp_C/3.08));
    Serial.print("|");
    Serial.print("Pompa:OFF");
    Serial.print("|");
 if (temp_C>55.4 && temp_C<64.4){
   Serial.print("Temperatura ideala pentru crizantema");
   Serial.print("|");
}else{
  Serial.print("Temperatura prea ridicata pentru crizantema");
  Serial.print("|");
  }
    delay(700);
    state=0;
  }else if(state == '1' && sensor>wet){
     digitalWrite(pumppin,HIGH);
    Serial.print(float(temp_C/3.08));
    Serial.print("|");
    Serial.print("Pompa:ON");
    Serial.print("|");
    if (temp_C>55.4 && temp_C<64.4){
   Serial.print("Temperatura ideala pentru crizantema");
   Serial.print("|");
}else{
  Serial.print("Temperatura prea ridicata pentru crizantema");
  Serial.print("|");
  }
    delay(700);
    state=0;
  }
  delay(500);

  digitalWrite(sensorpower,HIGH);
  delay(10);
  sensor = analogRead(sensorpin);
  digitalWrite(sensorpower,LOW);
  
  //Serial.println(sensor);
  if(sensor<wet){
     digitalWrite(pumppin,LOW);
   digitalWrite(LED1,LOW);
    digitalWrite(LED2,LOW);
    digitalWrite(LED3,HIGH);//verde
    
  }
  else if(sensor<dry && sensor>wet){
    if(state=='1'){
         digitalWrite(pumppin,HIGH);
    }
    digitalWrite(LED1,LOW);
    digitalWrite(LED2,HIGH);
    digitalWrite(LED3,LOW);
  }
  else if(sensor>dry){
     if(state=='1'){
         digitalWrite(pumppin,HIGH);
    }
    digitalWrite(LED1,HIGH);
    digitalWrite(LED2,LOW);
    digitalWrite(LED3,LOW);
  }                                                                                                                        
}

double Thermistor ( int RawADC )
{
  Temp = log (2*((10240000/RawADC)-10000));
  Temp =( 1 / ( 0.001129148 + ( 0.000234125 * Temp ) + ( 0.0000000876741 * Temp * Temp * Temp ) )) ;
  Temp = Temp - 273.15 ;           
  return Temp;
}
