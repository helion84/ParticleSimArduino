/*
 Name:		ParticleSIm.ino
 Created:	11/1/2018 7:58:00 PM
 Author:	PC
*/
#define ROW_1 2
#define ROW_2 3
#define ROW_3 4
#define ROW_4 5
#define ROW_5 6
#define ROW_6 7
#define ROW_7 8
#define ROW_8 9

#define COL_1 10
#define COL_2 11
#define COL_3 12
#define COL_4 13
#define COL_5 A0
#define COL_6 A1
#define COL_7 A2
#define COL_8 A3

#define particleNumber 3

const byte rows[] = {
	ROW_1, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6, ROW_7, ROW_8
};

byte output[] = { B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000 };

int particleSpeed = 2;
int particleRadius = 1;
int width = 8;
int height = 8;

int randomAngle, deltaX, deltaY;
int molStore[particleNumber][4];

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);

	for (byte i = 2; i <= 13; i++) {
		pinMode(i, OUTPUT);
	}
	pinMode(A0, OUTPUT);
	pinMode(A1, OUTPUT);
	pinMode(A2, OUTPUT);
	pinMode(A3, OUTPUT);

	for (int i = 0; i < particleNumber; i++) {
		delay(2);
		molStore[i][0] = int(random((0.25*width), (0.75*width)));
		delay(2);
		molStore[i][1] = int(random((0.25*height), (0.75*height)));
		delay(2);
		randomAngle = int(random(360));
		molStore[i][2] = int(sin(randomAngle) * particleSpeed);
		delay(2);
		randomAngle = int(random(360));
		molStore[i][3] = int(cos(randomAngle) * particleSpeed);
	}
}

// the loop function runs over and over again until power down or reset
void loop() {
	for (int i = 0; i < particleNumber; i++) {
		deltaX = molStore[i][2];
		deltaY = molStore[i][3];

		if (molStore[i][0] > width - particleRadius || molStore[i][0] < particleRadius) {
			deltaX = -deltaX;
		}
		if (molStore[i][1] > height - particleRadius || molStore[i][1] < particleRadius) {
			deltaY = -deltaY;
		}

		molStore[i][0] += deltaX;
		molStore[i][1] += deltaY;
		molStore[i][2] = deltaX;
		molStore[i][3] = deltaY;
	}
	drawMatrix();
}

void drawMatrix() {
	for (byte y = 0; y < width; y++) {
		byte temp = B00000000;
		for (byte x = 0; x < height; x++) {
			if (foundInStore(molStore, x, y)) {
				temp |= (1 << x);
			}
			else {
				temp |= (0 << x);
			}
		}
		output[y] = temp;
	}
	drawScreen(output);
}

bool foundInStore(int store[particleNumber][4], int x, int y) {
	for (int i = 0; i < particleNumber; i++) {
		if (store[i][0] == x && store[i][1] == y) {
			return true;
		}
	}
	return false;
}

void  drawScreen(byte buffer2[]) {
	// Turn on each row in series
	for (byte i = 0; i < 8; i++) {
		setColumns(buffer2[i]); // Set columns for this specific row       
		digitalWrite(rows[i], HIGH);
		delay(50); // Set this to 50 or 100 if you want to see the multiplexing effect!
		digitalWrite(rows[i], LOW);
	}
}

void setColumns(byte b) {
	digitalWrite(COL_1, (~b >> 0) & 0x01); // Get the 1st bit: 10000000
	digitalWrite(COL_2, (~b >> 1) & 0x01); // Get the 2nd bit: 01000000
	digitalWrite(COL_3, (~b >> 2) & 0x01); // Get the 3rd bit: 00100000
	digitalWrite(COL_4, (~b >> 3) & 0x01); // Get the 4th bit: 00010000
	digitalWrite(COL_5, (~b >> 4) & 0x01); // Get the 5th bit: 00001000
	digitalWrite(COL_6, (~b >> 5) & 0x01); // Get the 6th bit: 00000100
	digitalWrite(COL_7, (~b >> 6) & 0x01); // Get the 7th bit: 00000010
	digitalWrite(COL_8, (~b >> 7) & 0x01); // Get the 8th bit: 00000001
}