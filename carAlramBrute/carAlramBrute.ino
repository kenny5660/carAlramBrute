/*
 Name:		carAlramBrute.ino
 Created:	28.01.2018 19:01:59
 Author:	misha
*/

// the setup function runs once when you press reset or power the board
void setup() {
	pinMode(13, OUTPUT);

}

// the loop function runs over and over again until power down or reset
void loop() {
	digitalWrite(13, 1);
	delay(500);
	digitalWrite(13, 0);
	delay(500);
}
