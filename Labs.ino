int buttonPin = 12;
int bluePin = 2;
int greenPin = 3;
int redPin = 5;

void setup()
{
	Serial.begin(9600);
	pinMode(bluePin, OUTPUT);
	pinMode(greenPin, OUTPUT);
	pinMode(redPin, OUTPUT);
	pinMode(buttonPin, INPUT);
}

int ledColor = 7;


void turnOnLed()
{
	digitalWrite(redPin, ledColor & 1);
	digitalWrite(greenPin, ledColor & 2);
	digitalWrite(bluePin, ledColor & 4);
}

void switchLed()
{
	ledColor = ledColor % 7 + 1;
	turnOnLed();
}

void turnOffLed()
{
	digitalWrite(redPin, LOW);
	digitalWrite(greenPin, LOW);
	digitalWrite(bluePin, LOW);
}

bool buttonPrevious = false;

bool onSerial = false;

String getColorString()
{
	String str = "";
	if (ledColor & 1)
		str += "R";
	if (ledColor & 2)
		str += "G";
	if (ledColor & 4)
		str += "B";
	return str;
}

int buttonPressedOn = -1;

void sendCurrentColorToSerial()
{
	Serial.println(getColorString() + ": " + String(ledColor));
}

void loop()
{
	if (Serial.available() > 0)
		onSerial = true;


	if (onSerial)
	{
		auto a = Serial.readString();
		a.toLowerCase();

		if (a.indexOf("?") != -1)
		{
			sendCurrentColorToSerial();
			return;
		}
		if (a.indexOf("exit") != -1)
		{
			onSerial = false;
     buttonPressedOn = -1;
			return;
		}

		if (a.indexOf("0") != -1)
			ledColor = 0;
		if (a.indexOf("r") != -1)
			ledColor |= 1;
		if (a.indexOf("g") != -1)
			ledColor |= 2;
		if (a.indexOf("b") != -1)
			ledColor |= 4;
		turnOnLed();
	}
	else
	{
		delay(30);
		auto button = !digitalRead(buttonPin);
		if ((buttonPrevious & button) == HIGH)
		{
			if (buttonPressedOn != -1 && millis() - buttonPressedOn > 1000)
			{
				sendCurrentColorToSerial();
				buttonPressedOn = -1;
			}
			return;
		}
		if (button)
		{
			buttonPrevious = true;
			buttonPressedOn = millis();
			switchLed();
		}
		else
		{
			buttonPrevious = false;
			buttonPressedOn = -1;
			turnOffLed();
		}
	}
}
