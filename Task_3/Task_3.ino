#include <ArduinoSort.h>
#include <SoftwareSerial.h>
//////////////////////////////////////////////////
/*
  SoftwareSerial mySerial(8, 9);
  char mobile_char = ' ';
  int mobile_int = 0;
*/
const byte interruptPin = 2;
volatile byte interrupt_state = LOW;

const byte button1 = 3;
const byte button2 = 4;
const byte button3 = 5;
const byte button4 = 6;
const byte button5 = 7;
const byte button6 = 10;
const byte button7 = 11;
const byte button8 = 12;

byte LED1 = A2;
byte LED2 = A3;
byte LED3 = A4;

int last_floor[3] = {1, 1, 1};

int ele_num = 0;

int Destination_floor[3] = {1, 1, 1};

int ele_array_1[5] = {0};
int ele_array_2[5] = {0};
int ele_array_3[5] = {0};

String state[3] = {"OFF", "OFF", "OFF"};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // mySerial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  pinMode(button1, INPUT); pinMode(button2, INPUT); pinMode(button3, INPUT); pinMode(button4, INPUT); pinMode(button5, INPUT); pinMode(button6, INPUT); pinMode(button7, INPUT); pinMode(button8, INPUT);

  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), Button_Fun, RISING);
  /*
    cli();//stop interrupts
    //set timer0 interrupt at 2kHz
    TCCR0A = 0;// set entire TCCR0A register to 0
    TCCR0B = 0;// same for TCCR0B
    TCNT0  = 0;//initialize counter value to 0
    // set compare match register for 2khz increments
    OCR0A = 255;// = (16*10^6) / (2000*64) - 1 (must be <256)
    // turn on CTC mode
    TCCR0A |= (1 << WGM01);
    // Set CS01 and CS00 bits for 64 prescaler
    TCCR0B |= (1 << CS01) | (1 << CS00);
    // enable timer compare interrupt
    TIMSK0 |= (1 << OCIE0A);
    sei();//allow interrupts
  */
}

void loop() {
  // put your main code here, to run repeatedly:
  SearchEle1();

  SearchEle2();
  SearchEle3();

  // Serial.println(" void loop ");
  delay(2000);
}
/*
  ISR(TIMER0_COMPA_vect) {
  if (mySerial.available() > 0)
  {
    mobile_char = mySerial.read();
    mobile_int = int(mobile_char) - 48;
    if (mobile_int >= 1 && mobile_int <= 5) {
      Serial.write(mobile_int);
      push_array(0, mobile_int);
    }
    if (mobile_int >= 6 && mobile_int <= 10) {
      push_array(1, mobile_int);
    }
    if (mobile_int >= 11 && mobile_int <= 15) {
      push_array(2, mobile_int);
    }
  }
  }
*/
void Button_Fun(void) {
  String ele_state;
  if (digitalRead(button1) == HIGH) {
    Serial.println("1");
    ele_state = "UP";
    outButton(1, ele_state);
    while (digitalRead(button1) == HIGH) {}
  }
  if (digitalRead(button2) == HIGH) {
    Serial.println("2");
    ele_state = "UP";
    outButton(2, ele_state);
    while (digitalRead(button2) == HIGH) {}
  }
  if (digitalRead(button3) == HIGH) {
    ele_state = "Down";
    outButton(2, ele_state);
    Serial.println("22");
    while (digitalRead(button3) == HIGH) {}
  }
  if (digitalRead(button4) == HIGH) {
    ele_state = "UP";
    outButton(3, ele_state);
    Serial.println("3");
    while (digitalRead(button4) == HIGH) {}
  }
  if (digitalRead(button5) == HIGH) {
    ele_state = "Down";
    outButton(3, ele_state);
    Serial.println("33");   while (digitalRead(button5) == HIGH) {}
  }
  if (digitalRead(button6) == HIGH) {
    ele_state = "UP";
    outButton(4, ele_state);
    Serial.println("4");   while (digitalRead(button6) == HIGH) {}
  }
  if (digitalRead(button7) == HIGH) {
    ele_state = "Down";
    outButton(4, ele_state);
    Serial.println("44");   while (digitalRead(button7) == HIGH) {}
  }
  if (digitalRead(button8) == HIGH) {
    ele_state = "Down";
    outButton(5, ele_state);
    Serial.println("5");   while (digitalRead(button8) == HIGH) {}
  }
  //detachInterrupt(digitalPinToInterrupt(2));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void SearchEle1(void) {
  sortArray(ele_array_1, 5);
  Serial.println("SORTED 1 :");
  Serial.print(ele_array_1[0]);  Serial.print("  ");
  Serial.print(ele_array_1[1]);  Serial.print("  ");
  Serial.print(ele_array_1[2]);  Serial.print("  ");
  Serial.print(ele_array_1[3]);  Serial.print("  ");
  Serial.print(ele_array_1[4]);  Serial.print("  ");
  Serial.println(state[0]);
  ele_num = 0;
  if (state[ele_num] == "UP") {
    for (int i = 0; i < 5; i++) {
      if (ele_array_1[i] != 0) {
        if (last_floor[ele_num] <= ele_array_1[i] ) {
          Destination_floor[ele_num] = ele_array_1[i];
          ele_array_1[i] = 0;
          elevator_moving(Destination_floor[ele_num], ele_num);
          Serial.print ("Destination_floor  ");          Serial.println (Destination_floor[ele_num]);
          Serial.print ("ele_num  ");          Serial.println (ele_num);
        }
        if (ele_array_1[i] == 5) {
          state[ele_num] = "OFF";
        }
      }
    }
  }
  else if (state[ele_num] == "Down") {
    for (int i = 4; i >= 0; i--) {
      if (ele_array_1[i] != 0) {
        if (last_floor[ele_num] >= ele_array_1[i]) {
          Destination_floor[ele_num] = ele_array_1[i];
          ele_array_1[i] = 0;
          elevator_moving(Destination_floor[ele_num], ele_num);
          Serial.print ("Destination_floor  ");          Serial.println (Destination_floor[ele_num]);
          Serial.print ("ele_num  ");          Serial.println (ele_num);
        }
        if (ele_array_1[i] == 1) {
          state[ele_num] = "OFF";
        }
      }
    }
  }
  else if (state[ele_num] == "OFF") {
    int dis[5] = {0};
    int nearest_floor = 0;
    for (int i = 0; i < 5; i++) {
      dis[i] = abs (last_floor[ele_num] - ele_array_1[i]);
    }
    Serial.print("dis_arr11111111111   ");
    Serial.print(dis[0]); Serial.print(" "); Serial.print(dis[1]); Serial.print(" "); Serial.print(dis[2]); Serial.print(" "); Serial.print(dis[3]); Serial.print(" "); Serial.println(dis[4]);

    nearest_floor = getMin(dis, 5, ele_array_1);
    Serial.print("floornum:  ");    Serial.println(nearest_floor);
    if (nearest_floor > last_floor[ele_num] && nearest_floor > 0) {
      state[ele_num] = "UP";
    }
    else if (nearest_floor < last_floor[ele_num] && nearest_floor > 0) {
      state[ele_num] = "Down";
    }
    else if (nearest_floor == last_floor[ele_num] && nearest_floor > 0) {
      Destination_floor[ele_num] = last_floor[ele_num];
      for (int i = 0; i < 5; i++) {
        if (Destination_floor[ele_num] == ele_array_1[i]) {
          ele_array_1[i] = 0;
          break;
        }
      }
      elevator_moving(Destination_floor[ele_num], ele_num);
      Serial.print ("Destination_floor  ");          Serial.println (Destination_floor[ele_num]);
      Serial.print ("ele_num  ");          Serial.println (ele_num);
    }
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void SearchEle2(void) {
  sortArray(ele_array_2, 5);
  Serial.println("SORTED 2 :");
  Serial.print(ele_array_2[0]);  Serial.print("  ");
  Serial.print(ele_array_2[1]);  Serial.print("  ");
  Serial.print(ele_array_2[2]);  Serial.print("  ");
  Serial.print(ele_array_2[3]);  Serial.print("  ");
  Serial.print(ele_array_2[4]);  Serial.print("  ");
  Serial.println(state[1]);
  ele_num = 1;
  if (state[ele_num] == "UP") {
    for (int i = 0; i < 5; i++) {
      if (ele_array_2[i] != 0) {
        if (last_floor[ele_num] <= ele_array_2[i] ) {
          Destination_floor[ele_num] = ele_array_2[i];
          ele_array_2[i] = 0;
          elevator_moving(Destination_floor[ele_num], ele_num);
          Serial.print ("Destination_floor  ");          Serial.println (Destination_floor[ele_num]);
          Serial.print ("ele_num  ");          Serial.println (ele_num);
        }
        if (ele_array_2[i] == 5) {
          state[ele_num] = "OFF";
        }
      }
    }
  }
  else if (state[ele_num] == "Down") {
    for (int i = 4; i >= 0; i--) {
      if (ele_array_2[i] != 0) {
        if (last_floor[ele_num] >= ele_array_2[i]) {
          Destination_floor[ele_num] = ele_array_2[i];
          ele_array_2[i] = 0;
          elevator_moving(Destination_floor[ele_num], ele_num);
          Serial.print ("Destination_floor  ");          Serial.println (Destination_floor[ele_num]);
          Serial.print ("ele_num  ");          Serial.println (ele_num);
        }
        if (ele_array_2[i] == 1) {
          state[ele_num] = "OFF";
        }
      }
    }
  }

  else if (state[ele_num] == "OFF") {
    int dis[5] = {0};
    int nearest_floor = 0;
    for (int i = 0; i < 5; i++) {
      dis[i] = abs (last_floor[ele_num] - ele_array_2[i]);
    }
    Serial.print("dis_arr22222222222   ");
    Serial.print(dis[0]); Serial.print(" "); Serial.print(dis[1]); Serial.print(" "); Serial.print(dis[2]); Serial.print(" "); Serial.print(dis[3]); Serial.print(" "); Serial.println(dis[4]);
    nearest_floor = getMin(dis, 5, ele_array_2);

    if (nearest_floor > last_floor[ele_num] && nearest_floor > 0) {
      state[ele_num] = "UP";
    }

    else if (nearest_floor < last_floor[ele_num] && nearest_floor > 0) {
      state[ele_num] = "Down";
    }

    else if (nearest_floor == last_floor[ele_num] && nearest_floor > 0) {
      Destination_floor[ele_num] = last_floor[ele_num];
      for (int i = 0; i < 5; i++) {
        if (Destination_floor[ele_num] == ele_array_2[i]) {
          ele_array_2[i] = 0;
        }
      }
      elevator_moving(Destination_floor[ele_num], ele_num);
      Serial.print ("Destination_floor  ");          Serial.println (Destination_floor[ele_num]);
      Serial.print ("ele_num  ");          Serial.println (ele_num);
    }
  }
}

void SearchEle3(void) {
  sortArray(ele_array_3, 5);
  Serial.println("SORTED 3 :");
  Serial.print(ele_array_3[0]);  Serial.print("  ");
  Serial.print(ele_array_3[1]);  Serial.print("  ");
  Serial.print(ele_array_3[2]);  Serial.print("  ");
  Serial.print(ele_array_3[3]);  Serial.print("  ");
  Serial.print(ele_array_3[4]);  Serial.print("  ");
  Serial.println(state[2]);
  ele_num = 2;
  if (state[ele_num] == "UP") {
    for (int i = 0; i < 5; i++) {
      if (ele_array_3[i] != 0) {
        if (last_floor[ele_num] <= ele_array_3[i] ) {
          Destination_floor[ele_num] = ele_array_3[i];
          ele_array_3[i] = 0;
          elevator_moving(Destination_floor[ele_num], ele_num);
          Serial.print ("Destination_floor  ");          Serial.println (Destination_floor[ele_num]);
          Serial.print ("ele_num  ");                    Serial.println (ele_num);
        }
        if (ele_array_3[i] == 5) {
          state[ele_num] = "OFF";
        }
      }
    }
  }
  else if (state[ele_num] == "Down") {
    for (int i = 4; i >= 0; i--) {
      if (ele_array_3[i] != 0) {
        if (last_floor[ele_num] >= ele_array_3[i]) {
          Destination_floor[ele_num] = ele_array_3[i];
          ele_array_3[i] = 0;
          elevator_moving(Destination_floor[ele_num], ele_num);
          Serial.print ("Destination_floor  ");          Serial.println (Destination_floor[ele_num]);
          Serial.print ("ele_num  ");                    Serial.println (ele_num);
        }

        if (ele_array_3[i] == 1) {
          state[ele_num] = "OFF";
        }
      }
    }
  }

  else if (state[ele_num] == "OFF") {
    int dis[5] = {0};
    int nearest_floor = 0;
    for (int i = 0; i < 5; i++) {
      dis[i] = abs (last_floor[ele_num] - ele_array_3[i]);
    }
    Serial.print("dis_arr333333333333   ");
    Serial.print(dis[0]); Serial.print(" "); Serial.print(dis[1]); Serial.print(" "); Serial.print(dis[2]); Serial.print(" "); Serial.print(dis[3]); Serial.print(" "); Serial.println(dis[4]);
    nearest_floor = getMin(dis, 5, ele_array_3);

    if (nearest_floor > last_floor[ele_num] && nearest_floor > 0) {
      state[ele_num] = "UP";
    }
    else if (nearest_floor < last_floor[ele_num] && nearest_floor > 0) {
      state[ele_num] = "Down";
    }
    else if (nearest_floor == last_floor[ele_num] && nearest_floor > 0) {
      Destination_floor[ele_num] = last_floor[ele_num];
      for (int i = 0; i < 5; i++) {
        if (Destination_floor[ele_num] == ele_array_3[i]) {
          ele_array_3[i] = 0;
        }
      }
      elevator_moving(Destination_floor[ele_num], ele_num);
      Serial.print ("Destination_floor  ");          Serial.println (Destination_floor[ele_num]);
      Serial.print ("ele_num  ");          Serial.println (ele_num);
    }
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void outButton(int floorBut, String stateBut) {
  int result[3] = {0};
  int elevatorNum = 4;
  for (int i = 0; i < 3; i++) {
    if (((state[i] == stateBut && last_floor[i] > floorBut ) || (state[i] != stateBut && last_floor[i] < floorBut && Destination_floor[i] < floorBut ) || (state[i] == "OFF" ))) {
      result[i] = abs(last_floor[i] - floorBut);
    }
    if (((state[i] == stateBut && last_floor[i] < floorBut ) || (state[i] != stateBut && last_floor[i] > floorBut ) || (state[i] != stateBut && last_floor[i] < floorBut && Destination_floor[i] > floorBut )) && (stateBut == "Down")) {
      result[i] = abs(Destination_floor[i] - last_floor[i]) + abs(Destination_floor[i] - floorBut);
    }

    if (((state[i] == stateBut && last_floor[i] < floorBut ) || (state[i] != stateBut && last_floor[i] > floorBut && Destination_floor[i] > floorBut )) && (stateBut == "UP")) {
      result[i] = abs(last_floor[i] - floorBut);
    }
    if (((state[i] == stateBut && last_floor[i] > floorBut ) || (state[i] != stateBut && last_floor[i] < floorBut ) || (state[i] != stateBut && last_floor[i] > floorBut && Destination_floor[i] < floorBut )) && (stateBut == "UP")) {
      result[i] = abs(Destination_floor[i] - last_floor[i]) + abs(Destination_floor[i] - floorBut);
    }
  }
  elevatorNum = min(min(result[0], result[1]), result[2]);
  Serial.print("result0     ");  Serial.println(result[0]);        Serial.print("result1     "); Serial.println(result[1]);  Serial.print("result2     ");  Serial.println(result[2]);
  for (int i = 0; i < 3; i++) {
    if (elevatorNum == result[i]) { //call elevator num i
      Serial.print("Elevator_Num  ");
      Serial.println(i + 1);
      Serial.print("level  ");
      Serial.println(floorBut);
      push_array(i, floorBut);
      break;
    }
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void push_array(int Elevator_Num, int level) {
  if (Elevator_Num == 0) {
    for (int i = 0; i < 5; i++) {
      if (level == ele_array_1[i]) {
        return;
      }
    }
    for (int i = 0; i < 5; i++) {
      if (ele_array_1[i] == 0) {
        ele_array_1[i] = level;
        break;
      }
    }
  }
  else if (Elevator_Num == 1) {
    for (int i = 0; i < 5; i++) {
      if (level == ele_array_2[i]) {
        return;
      }
    }
    for (int i = 0; i < 5; i++) {
      if (ele_array_2[i] == 0) {
        ele_array_2[i] = level;
        break;
      }
    }
  }
  else if (Elevator_Num == 2) {
    for (int i = 0; i < 5; i++) {
      if (level == ele_array_3[i]) {

        return;
      }
    }
    for (int i = 0; i < 5; i++) {
      if (ele_array_3[i] == 0) {
        ele_array_3[i] = level;
        break;
      }
    }
  }
  else {
    return;
  }
  Serial.print("arr1:  ");
  Serial.print(ele_array_1[0]); Serial.print(" "); Serial.print(ele_array_1[1]); Serial.print(" "); Serial.print(ele_array_1[2]); Serial.print(" "); Serial.print(ele_array_1[3]); Serial.print(" "); Serial.println(ele_array_1[4]);
  Serial.print("arr2:  ");
  Serial.print(ele_array_2[0]); Serial.print(" "); Serial.print(ele_array_2[1]); Serial.print(" "); Serial.print(ele_array_2[2]); Serial.print(" "); Serial.print(ele_array_2[3]); Serial.print(" "); Serial.println(ele_array_2[4]);
  Serial.print("arr3:  ");
  Serial.print(ele_array_3[0]); Serial.print(" "); Serial.print(ele_array_3[1]); Serial.print(" "); Serial.print(ele_array_3[2]); Serial.print(" "); Serial.print(ele_array_3[3]); Serial.print(" "); Serial.println(ele_array_3[4]);
  return;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void elevator_moving( int D_Level, int ele_num) {
  if (last_floor[ele_num] == D_Level) {
    digitalWrite(LED1, HIGH);    // turn the LED on
    delay(1000);
    digitalWrite(LED1, LOW);   // turn the LED off
    delay(1000);
    state[ele_num] == "OFF";
  }
  if (state[ele_num] == "UP") {
    Serial.println("moving up");
    for (last_floor[ele_num]; last_floor[ele_num] < D_Level; last_floor[ele_num]++) {
      Serial.print(last_floor[ele_num] + 1);  Serial.print(" ----> CURRENT FLOOR of Elevator  ");   Serial.println(ele_num + 1);
      digitalWrite(LED2, HIGH);    // turn the LED on
      delay(1000);
      digitalWrite(LED2, LOW);   // turn the LED off
      delay(1000);
    }
    state[ele_num] = "OFF";
  }
  else if (state[ele_num] == "Down") {
    Serial.println("moving down");
    for (last_floor[ele_num]; last_floor[ele_num] > D_Level; last_floor[ele_num]--) {
      Serial.print(last_floor[ele_num] - 1);  Serial.print(" ----> CURRENT FLOOR of Elevator  "); Serial.println(ele_num + 1);
      digitalWrite(LED3, HIGH);    // turn the LED on
      delay(1000);
      digitalWrite(LED3, LOW);   // turn the LED off
      delay(1000);
    }
    state[ele_num] = "OFF";
  }
  else {
    return;
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int getMin(int* array1, int size, int* array2) {
  int minimum = 6;
  int floornum = 0;
  for (int i = 0; i < size; i++)
  {
    if (array2[i] != 0) {
      if (array1[i] < minimum) {
        minimum = array1[i];
        floornum = array2[i];
      }
    }
  }
  return floornum;
}
