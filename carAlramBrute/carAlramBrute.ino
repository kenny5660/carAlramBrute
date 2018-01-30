/*
 Name:		carAlramBrute.ino
 Created:	28.01.2018 19:01:59
 Author:	misha
*/
const int BUTTON = 3;
const int RED = 4;
const int GREEN = 5;
bool enterPas(short a);
void setup() {
	Serial.begin(115200);
	pinMode(13, OUTPUT);
	pinMode(BUTTON, OUTPUT);
	pinMode(RED, INPUT);
	pinMode(GREEN, INPUT);
	digitalWrite(BUTTON, 0);


	/*digitalWrite(BUTTON, 1);
	delay(200);
	Serial.print(digitalRead(RED));
	Serial.println(digitalRead(GREEN));
	while (!(digitalRead(GREEN) || digitalRead(RED)));
	digitalWrite(BUTTON, 0);
	delay(100);
	Serial.print(digitalRead(RED));
	Serial.println(digitalRead(GREEN));
	while (!digitalRead(RED));
	while (digitalRead(GREEN) || digitalRead(RED));*/
	enterPas(1234);
	delay(5100);
	enterPas(1111);
}

// the loop function runs over and over again until power down or reset
void loop() {
		
	digitalWrite(13, 1);
	delay(500);
	digitalWrite(13, 0);
	delay(500);
	
}
bool enterPas(short a) {
	char m[4],i = 3;
	while (a>0)
	{
		m[i] = a % 10;
		a = a / 10;
		i--;
	}
	for (i=0; i < 4; i++) {
		for (char j = 0; j < m[i]; j++) {
			digitalWrite(BUTTON, 1);
			while (!(digitalRead(GREEN) || digitalRead(RED)));
			delay(200);
			digitalWrite(BUTTON, 0);
			while ((digitalRead(GREEN) || digitalRead(RED)));
			delay(200);
		}
		
		
		while (!digitalRead(RED));
		while (digitalRead(GREEN) || digitalRead(RED));

		Serial.println(((int)m[i]));
	}
	return true;
}