#define _CRT_SECURE_NO_WARNINGS

#include "TXLib.h"
#include "Q_Vector.h"
#include "TXWave.h"
#include "Q_Buttons.h"
//#include "PowerPoint.cpp"


const int RINGSK = 8;


struct Rings
{
	int startOfLesson [RINGSK];
	int endOfLesson [RINGSK];
};


/*
	int &a = b;
	int &x = * (int*) 0
	printf ("%d", a);
	x = a;

	int *a = &b;
	int *x = (int*) 0;
	printf ("%d", *a);
	*x = *a;

* */


struct Rect
{
    Vector pos;
    Vector size;
    Vector finishPos;

    double left  () const { return this->pos.x; }
    double top   () const { return this->pos.y; }
    double right () const { return this->finishPos.x; }
    double bottom() const { return this->finishPos.y; }
};


struct Button
{
	Rect rect = {};
	void (*funcClicked) ();
	COLORREF color;
};



/*
struct colorButton : Button
{
	COLORREF color;
};


Button button1 = {};
Button button2 = {};

colorButton button3 = {};
colorButton button4 = {};

Button Buttons[20] = {};
 */

Rings RING = {};


const int APPEALK = 10;
char Appeals[APPEALK][_MAX_PATH] = {};
int nextAppeal = 0; 
int AppealTime[APPEALK] = {};
char AppealsStr[APPEALK][50] = {};


const Vector UserScreen = {.x = 1445, .y = 1080};


void printTime ();
void readTimeRings ();
void printRings ();
void loadImage (const char* path, HDC &finalImage, Vector originalSize = {}, Vector finalSize = {});
void printTimeBeforeRing ();
void checkTimeForRing ();
int getClockTime ();
char* getClockTimeChar ();
void progressBar (double num, double bugNum);
bool onButtonClicked (Rect Button);
void textOut (int x, int y, const char* text);
void selectFont (const char * font, int fontNum);
void rectangle (int x1, int y1, int x2, int y2);
void findFont ();
int strToTime (const char* time);
void makeAppeal (Rect button);
char* timeToStr (int time);
void scrollBar (double bigNum, double currPosInBigNum, double partOfBigNum);
bool stopRecordingFunc (HWAVEIN, txWaveData_t& data, void*);
bool checkAppeal ();
void changeButton ();
void changeColorButton ();


#define txSetAllColors(color)  { txSetFillColor (color); txSetColor (color); }
#define MAINCOLOR RGB (255, 255, 102)






int main2()
{

	//txWaveData_t ring = txWaveIn ("Purr.wav");
	//txWaveOut (ring);
	//txWaveOut ();
	//return 0;
	//txWaveData_t buf =  txWaveLoadWav ("Sound_17211.wav");  
	//HWAVEOUT waveOut = txWaveOut (buf); assert (waveOut);  // Запускаем звук

	//_txWindowStyle &= ~WS_CAPTION;
	//HWND MainScreen = txCreateWindow (1007, 752);
	HWND MainScreen = txCreateWindow (1445, 1080);

    SetWindowText (MainScreen,  "Rings");
	txSetAllColors (MAINCOLOR);
	txSetTextAlign (TA_CENTER);

	HDC Menu = {};
	loadImage ("Files/Menu4.bmp", Menu);


	//Button[0].pos = {.pos = {190, 920}, .finishPos = {270, 1000}};
	//Button[0].funcClicked = changeButton;
	//Button[1].pos = {.pos = {1355, 6}, .finishPos = {1445, 1008}};
	//Button[2].pos = {.pos {1240, 930}, .finishPos = {1264, 940}};
	//Button[3].pos = {.pos {1240, 990}, .finishPos = {1264, 1005}};
	
	

	//findFont ();
	
	//_getch ();
	readTimeRings ();

	txBegin ();
	//txWaveData_t ring = txWaveLoadWav ("Files/Sound1.wav");
	//txWaveOut (ring);
	//txWaveOut ();
	for (;;)
	{

		txSetTextAlign (TA_CENTER);
		printTimeBeforeRing ();
		printTime ();


		txSetTextAlign (TA_LEFT);
		printRings ();


		//makeAppeal (BUTTONS.VoiceButton);
		checkTimeForRing ();

		if (txGetAsyncKeyState (VK_ESCAPE) || onButtonClicked (BUTTONS.escape_Button))
		{
			break;
		}


		txSleep (30);

		txBitBlt (0, 0, Menu);
	}

	txEnd ();
	txDisableAutoPause ();

	txDeleteDC (Menu);

	return 0;
}

//Engine checking
void checkButtons ()
{
	for (int i = 0; i < 20; i++)
	{
		if (onButtonClicked (Buttons[i]))
		{

		}
	}
}

void progressBar (double num, double bugNum)
{
	$s
	txSetAllColors (RGB (50, 200, 50));
	rectangle (45, 260, 945, 275);

	txSetAllColors (RGB (0, 105, 0));
	rectangle (45, 260, 45 + 945 * (num / bugNum), 275);

}

void checkTimeForRing ()
{
	//char PathName[_MAX_PATH];
	//printf ("%s\n", _getcwd (PathName, _MAX_PATH));
	
	time_t t = time (NULL);
	int clockTime = t - (((int) (t / (3600 * 24))) * 3600 * 24) + 3 * 3600;
	static int lastTime;

	for (int i = 0; i < RINGSK; i++)
	{
		if (RING.startOfLesson[i] == clockTime || RING.endOfLesson[i] == clockTime)
		{
			if (lastTime != clockTime)
			{
				txPlaySound ("Files/Sound1.wav");
			}
			
		}
	}
	//txPlaySound ("Files/Sound1.wav");

	lastTime = clockTime;
}


void loadImage (const char* path, HDC &finalImage, Vector originalSize /* = {}*/, Vector finalSize /* = {}*/)
{
	if (finalSize.x == 0)
	{
		finalImage = txLoadImage (path);
	}
	else
	{
		finalImage = txCreateCompatibleDC (finalSize.x, finalSize.y);

		HDC Copyimage = txLoadImage (path, 0, 0, IMAGE_BITMAP, 0);
		//if (CopeImage == 0) традиционная загрузка
		assert (Copyimage);

		StretchBlt (finalImage, 0, 0, finalSize.x, finalSize.y,  Copyimage, 0, 0, originalSize.x, originalSize.y, SRCCOPY);
		txDeleteDC (Copyimage);
	}
}


void readTimeRings ()
{
	FILE *file = fopen ("Files/Rings.txt", "r");
	for (int i = 0; i < RINGSK; i++)
	{
		int minutes = 0;
		int hours = 0;
		int code = 0;

		code = fscanf (file, "%d:%d ", &hours, &minutes);
		if (code == 0 || code == -1) break;
		RING.startOfLesson[i] = minutes * 60 + hours * 3600;

		fscanf (file, "%d:%d ", &hours, &minutes);
		RING.endOfLesson[i] = minutes * 60 + hours * 3600;
	}
}

bool onButtonClicked (Rect Button)
{
    
    if (txMouseButtons () == 1)
    {
        if (txMouseX () > Button.left () && txMouseX () <  Button.right ())
        {
            if (txMouseY () > Button.top () && txMouseY () <  Button.bottom ())
            {
                while (txMouseButtons () == 1)
                {
                };
                if (txMouseX () > Button.left () && txMouseX () <  Button.right ())
                {
                    if (txMouseY () > Button.top () && txMouseY () <  Button.bottom ())
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;

    
}

void printRings ()
{
	selectFont ("Myriad Pro Regular", 35);


	int clockTime = getClockTime ();


	for (int i = 0; i < RINGSK; i++)
	{
		int printX = 100;
		if (4 <= i)
		{
			printX = 590;
		}


		char lesson[50] = {};
		int hours1 =  RING.startOfLesson[i] / 3600;
		int minutes1 = RING.startOfLesson[i] / 60 - hours1 * 60;

		int hours2 =  RING.endOfLesson[i] / 3600;
		int minutes2 = RING.endOfLesson[i] / 60 - hours2 * 60;

		if (RING.startOfLesson[i] != 0 && RING.endOfLesson[i] != 0)
		{
			sprintf (lesson, "%d:%02d - %d:%02d  %d урок",hours1, minutes1, hours2, minutes2, i+1);
		}

		if (RING.startOfLesson[i] < clockTime && clockTime <  RING.endOfLesson[i])
		{
			txSetAllColors (TX_GREEN);
			rectangle (printX, 340 + (i % 4) * 60, printX + 300, 375 + (i % 4) * 60);
			txSetAllColors (MAINCOLOR);
		}

		char rest[50] = {};

		if (i < RINGSK - 1)
		{
			int hours3 =  RING.endOfLesson[i + 1] / 3600;
			int minutes3 = RING.endOfLesson[i + 1] / 60 - hours3 * 60; 
			
			if (RING.endOfLesson[i] != 0 || RING.startOfLesson[i + 1] != 0)
			{
				sprintf (rest, "%d:%02d - %d:%02d  %d перемена", hours2, minutes2, hours3, minutes3, i + 1);
			}

			if (RING.endOfLesson[i] < clockTime && clockTime <  RING.startOfLesson[i + 1])
			{
				txSetAllColors (TX_GREEN);
				rectangle (printX, 370 + (i % 4) * 60, printX + 350, 345 + ((i + 1) % 4) * 60);
				txSetAllColors (MAINCOLOR);
			}
		}
		
		txSetAllColors (RGB (153, 255, 51));
		textOut (printX, 342 + (i % 4) * 60, lesson);
		textOut (printX, 372 + (i % 4) * 60, rest);
		txSetAllColors (MAINCOLOR);
	}	
}

void rectangle (int x1, int y1, int x2, int y2)
{
	txRectangle (x1 * (UserScreen.x / 1007), y1 * (UserScreen.y / 752), x2 * (UserScreen.x / 1007), y2 * (UserScreen.y / 752));
}

void makeAppeal (Rect button)
{
	if (onButtonClicked (button))
	{
		const char *timestr = txInputBox ("Введите время объявления:", "Вопрос жизни или смерти", "9:00");
		AppealTime[nextAppeal] = strToTime (timestr);

		const char* cause = txInputBox ("Введите о чем объявления:", "Вопрос жизни или смерти", "Просто объявление");
		strcpy (AppealsStr[nextAppeal], cause);

		txWaveData_t recording =  txWaveIn (100000, stopRecordingFunc);
		char path[_MAX_PATH] = {};
		sprintf (path, "Files/%s%d.wav", cause, nextAppeal);

		txWaveSaveWav (recording, path);
	
		nextAppeal++;
	}

	static int startPos = 0;

	//if (onButtonClicked (BUTTONS.appealButtonUp) && startPos > 0)
	{
		startPos--;
	}

	if (onButtonClicked (BUTTONS.appealButtonDown) && 0 <= nextAppeal - (startPos + 3))
	{
		startPos++;
	}

	if (nextAppeal > 1)
	{
		scrollBar (nextAppeal, startPos, 2);	
	}
	else
	{
		scrollBar (nextAppeal, startPos, 1);
	}


	for (int i = startPos; i < nextAppeal; i++)
	{
		char text[50] = {};
		sprintf (text, "%s   Трансляция: %s", timeToStr (AppealTime[i]), AppealsStr[i]);
		selectFont ("Arial", 33);
		if ((i - startPos) < 2)
		{
			txSetAllColors (RGB (153, 255, 51));
			txTextOut (393, 925 + (i - startPos) * 40, text);
		}

		if (getClockTime () == AppealTime[i])
		{
			//txWaveOut (Appeals[nextAppeal]);
			char path[_MAX_PATH] = {};
			sprintf (path, "Files/%s%d.wav", AppealsStr[i], i);
			txPlaySound (path);

			sprintf (AppealsStr[i], "[Проиграно]");
			//startPos++;

		}
	}
	if (checkAppeal ())
	{
		//startPos++;
	}

}

bool checkAppeal ()
{
	int clockTime = getClockTime ();

	for (int i = 0; i < nextAppeal; i++)
	{
		if (clockTime > AppealTime[i])
		{
			char cmpText[50] = {};
			char trashStr[50] = {};
			sscanf (AppealsStr[i], "%s", cmpText);
			if (strcmp (cmpText, "[Проиграно]"))
			{
				char path[_MAX_PATH] = {};
				sprintf (path, "Files/%s%d.wav", AppealsStr[i], i);
				txPlaySound (path);


				char copyStr[50] = {};
				strcpy (copyStr, AppealsStr[i]);

				sprintf (AppealsStr[i], "[Проиграно]");
				return true;
			}
		}
	}
	return false;
}

bool stopRecordingFunc (HWAVEIN, txWaveData_t& data, void*)
{
	//if (onButtonClicked (BUTTONS.VoiceButton))
	{
		return false;
	}
	return true;
}

void scrollBar (double bigNum, double currPosInBigNum, double partOfBigNum)
{
	if (bigNum == 0)
	{
		txSetAllColors (RGB (42, 105,0));
		txRectangle (1246, 942, 1265, 990);
		return;
	}
	$s
	txSetAllColors (RGB (151, 182, 130));
	txRectangle (1246, 942, 1265, 990);

	txSetAllColors (RGB (42, 105,0));
	txRectangle (1246, 942 + (990 - 942) * (currPosInBigNum / bigNum), 1265, 942 + (990 - 942) * ((currPosInBigNum + partOfBigNum) / bigNum));
}

void printTimeBeforeRing ()
{
	time_t t = time (NULL);
	int clockTime = t - (((int) (t / (3600 * 24))) * 3600 * 24) + 3 * 3600;
	//$ (clockTime);

	int minDelta = 100000000000000000;
	bool nextLesson = false;
	int lastNumOfLesson = 0;


	for (int i = 0; i < RINGSK; i++)
	{
		if (0 < (RING.startOfLesson[i] - clockTime) && (RING.startOfLesson[i] - clockTime) < minDelta)
		{
			minDelta = RING.startOfLesson[i] - clockTime;
			nextLesson = true;
			lastNumOfLesson = i;
		}

		if (0 < (RING.endOfLesson[i] - clockTime) && (RING.endOfLesson[i] - clockTime) < minDelta)
		{
			minDelta = RING.endOfLesson[i] - clockTime;
			nextLesson = false;
			lastNumOfLesson = i;
		}
	}

	char currThing[60] = {};

	if (! (minDelta > 24 * 3600))
	{
		if (nextLesson)
		{
			sprintf (currThing, "Перемена %d", lastNumOfLesson);
		}
		else
		{
			sprintf (currThing, "Урок %d", lastNumOfLesson + 1);
		}
	}

	txSelectFont ("Arial", 110, 110 * 0.435, 1000);
	txSetAllColors (RGB (255, 255, 102));
	txTextOut (UserScreen.x * 0.5, UserScreen.y * 0.17, "1 час второй пары");


	int timeJetLasting = 0;

	if (nextLesson)
	{
		timeJetLasting = clockTime - RING.endOfLesson[lastNumOfLesson - 1];		
	}
	else
	{
		timeJetLasting = clockTime - RING.startOfLesson[lastNumOfLesson];
	}

	char timeAfter[60] = {};
	double minutes = 0;

	minutes = ceil (timeJetLasting / 60);

	sprintf (timeAfter, "Идет %d минут", (int) minutes);

	if (! (minDelta > 24 * 3600))
	{
		selectFont ("Arial", 45);
		textOut (295, 200, timeAfter);
	}


	char timeBefore[60] = {};
	minutes = 0;

	minutes = ceil (minDelta / 60);

	sprintf (timeBefore, "Осталось %d минут", (int) minutes);
	/*
		}
		else
		{
			sprintf (timeBefore, "Перемена начнется через %d час(-ов/-a) %d минут", hours, minutes);
		} */

	if (! (minDelta > 24 * 3600))
	{
		textOut (665, 200, timeBefore);
		selectFont ("Myriad Pro Regular", 48);
	}

	if (minDelta > 24 * 3600)
	{
		textOut (505, 210, "Уроков сегодня больше нет");
	}


	progressBar (timeJetLasting, timeJetLasting + minDelta);
}


void selectFont (const char * font, int fontNum)
{
	txSelectFont (font, (fontNum * UserScreen.x / 1007) * 0.85, (fontNum * UserScreen.x / 1007) * 0.35);	
}

void textOut (int x, int y, const char* text)
{
	txTextOut ((UserScreen.x / 1007) * x, (UserScreen.y / 752) * y, text); 		
}

int getClockTime ()
{
	time_t t = time (NULL);
	int clockTime = t - (((int) (t / (3600 * 24))) * 3600 * 24) + 3 * 3600;
	return clockTime;
}

char* getClockTimeChar ()
{
	time_t t = time (NULL);
	int clockTime = t - (((int) (t / (3600 * 24))) * 3600 * 24) + 3 * 3600;
	static char text[10] = {};

	int hours1 =  clockTime / 3600;
	int minutes1 = clockTime / 60 - hours1 * 60;

	sprintf (text, "%d:%02d", hours1, minutes1);

	return text;
}

void printTime ()
{

	txSetAllColors (RGB (255, 255, 102));
	selectFont ("Arial", 190);

	selectFont ("Arial", 190);
	//txTextOut (UserScreen.x * 0.5, 0.0005 * UserScreen.y, getClockTimeChar ());
	txTextOut (UserScreen.x * 0.5, -10, getClockTimeChar ());
}

int strToTime (const char* time)
{
	int minutes = 0;
	int hours = 0;
	int code = 0;
	

	code = sscanf (time, "%d:%d ", &hours, &minutes);

	if (code != 2) return -1;

	int finalTime = minutes * 60 + hours * 3600;
	return	finalTime;
}

char* timeToStr (int time)
{
	static char newStr[50] = {};
	int hours =	time / 3600;
	int minutes = time / 60 - hours * 60;


	sprintf (newStr, "%d:%02d", hours, minutes);
	return newStr;
}


void findFont ()
{
	for (int i = 0; i < 20; i++)
	{
		selectFont ("Arial", 150 + i * 10);
		txTextOut (100 + i * 100, 100, "1");
	}
}

void changeButton ()
{
	printf ("1\n");
}


void changeColorButton ()
{
	printf ("Color\n");
}
