#include <iostream>
#include <string>
#include <thread>
#include <conio.h>
#include <windows.h>

#define FRAME_DELAY 30
#define X_END 100
#define Y_END 21
#define C 262
#define D 294
#define E 330
#define F 369
#define G 392
#define A 440
#define B 494
#define C_high 523

using namespace std;
const short NOTES_COUNT = 32;
short note_indx = 0;
const short NOTES[NOTES_COUNT] = {D, E, G, E, B, B, A, D, E, G, E, A, A, G, D, E, G, E, G, A, F, D, D, A, G, D, E, G, E, B, B, A};
const short BOT_LINE_COUNT = 9;
const string CLEAR_LINE(20, ' ');
const pair<short, string> BOT_Lines[BOT_LINE_COUNT] = {
	{3, "`^~----~~\"``"}, {2, "\\   \\~~~/    /"}, {1, "\\    /===\\     ./~"},
	{0, "|      ``'       |&)"}, {0, "(  @ ) / (  @ )--\\ |"}, {1, "/---.    ,---. \\ \\"},
	{2, "/            \\ |"}, {2, "<= == = = > \\ \\"}, {3, ",_._._._._._"}};

struct Player
{
    short x;
    short y;
    short velocityY;
    bool isJumping;
    bool sound;
    const short JUMP_POWER = -4;
    const short GRAVITY = 1;
};

void updatePlayer(Player& player);
void Goto(short x, short y);
void drawPlayer(Player& player, short prev_x, short prev_y);
bool MoveKey(Player& player);
void HideCursor(void);
void playJumpSound(void);
bool soundPlayThread(void);

int main(void)
{
	SetConsoleTitle("Jaewoong-bot");
	HideCursor();
	system("cls");

    Player player = {X_END / 2, Y_END, 0, false, false};
	bool moved;
	char key = 0;
	short prev_x = player.x, prev_y = player.y;
	drawPlayer(player, prev_x, prev_y);
	do
	{
		if(_kbhit()) key = _getch();
		prev_x = player.x;
		prev_y = player.y;	
		moved = MoveKey(player);
		if (player.sound) player.sound = soundPlayThread();
		updatePlayer(player);		
		if (moved || player.isJumping|| player.y != prev_y)
			drawPlayer(player, prev_x, prev_y);
			Goto(0, 0);
			cout << CLEAR_LINE;
			Goto(0, 0);
			cout << player.x << ", " << player.y << "   ";
		Sleep(FRAME_DELAY);
	} while(key != 27);

	return 0;
}


void updatePlayer(Player& player)
{
    if (player.isJumping)
	{
        player.velocityY += player.GRAVITY;
        player.y += player.velocityY;

        if (player.y > Y_END)
		{
            player.y = Y_END;
            player.velocityY = 0;
            player.isJumping = false;
        }
    }
}

void Goto(short x, short y)
{
	COORD Pos = {x,y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Pos);
}

void drawPlayer(Player& player, short prev_x, short prev_y)
{
	for (short i = 0; i < BOT_LINE_COUNT; ++i)
    {
        Goto(prev_x + BOT_Lines[BOT_LINE_COUNT - i - 1].first, prev_y + i);
        cout << CLEAR_LINE;
    }    
	for (short i = 0; i < BOT_LINE_COUNT; ++i)
	{
    	Goto(player.x + BOT_Lines[BOT_LINE_COUNT - i - 1].first, player.y + i);
		cout << BOT_Lines[BOT_LINE_COUNT - i - 1].second;
		cout.flush();
    }
}

bool MoveKey(Player& player)
{
	short prev_x = player.x;
    if (GetAsyncKeyState(VK_LEFT) & 0x8000) if (player.x > 0) player.x--;
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) if (player.x < X_END) player.x++;
    if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
        if (!player.isJumping)
		{
            player.isJumping = true;
            player.velocityY = player.JUMP_POWER;
            player.sound =true;
        }
    }
    return (player.x != prev_x);
}

void playJumpSound(void)
{
	Beep(NOTES[note_indx], 300);
	note_indx = ++note_indx % NOTES_COUNT;
}

bool soundPlayThread(void) {
	thread soundThread(playJumpSound);
	soundThread.detach();
	return false;
}

void HideCursor(void)
{
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}
