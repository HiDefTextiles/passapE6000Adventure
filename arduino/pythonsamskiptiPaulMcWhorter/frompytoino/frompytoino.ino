String myCmd;

// fyrsta gildi + eða -
// annað gidli
// 32 - 0
// 123 - 90

char munztur[179] = {0};

char patt[1];
int kongulo[179] = {1};
int nr = 0;

void setup() {
  // put your setup code here, to run once:
  byte test = 0x0;
  Serial.write(test);
Serial.begin(115200);
pinMode(13,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
while(Serial.available()==0){
  //pinMode(13,OUTPUT);

}
while(Serial.available()!=0) {
  String test2 = Serial.readStringUntil('!');
  int lengd = test2.length();
  if (lengd > 2) {
      int numby = (test2.charAt(1) - '0')*10 + (test2.charAt(2)-'0');
  int possy = (test2.charAt(0) == '+') ? numby : -numby;
    for (int i = 3; i < lengd;i++){
      munztur[i-3] = test2.charAt(i);
      //Serial.println(test2.charAt(i));
    }
    Serial.println(munztur);
    }
  // if (int(test2) < 190 && int(test2) !=10) {
          //Serial.println(test2.length());

    // }
  }
  //myCmd=Serial.readStringUntil('\r');
  //if(myCmd="ON"){
   // digitalWrite(13,1);
    //Serial.println(digitalRead(13));
  //}
  //if(myCmd="OFF"){
      //  digitalWrite(13,0);

  //}
//Serial.println(Serial.read());
}
