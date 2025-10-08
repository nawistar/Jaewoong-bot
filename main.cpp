#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>
#define X_END 100
#define Y_END 21
using namespace std;


const int BOT_LINE_COUNT = 9;
const string BOT_lines[BOT_LINE_COUNT] = {
	 "   `^~----~~\"``","  \\   \\~~~/    /"," \\ 0  /===\\  0  ./~",
	 "|      ``'       |&)","(  @ ) / (  @ )--\\ |"," /---.    ,---. \\ \\",
	 "  /            \\ |","  <= == = = > \\ \\","   ,_._._._._._"
};

struct Player
{
    short x;
    short y;
    short velocityY;
    bool isJumping;
    const short JUMP_POWER = -5; 
    const short GRAVITY = 1;
};

void updatePlayer(Player &player);
void Goto(short x, short y);
void Move_Key(Player &player);
void HideCursor(void);


int main(void)
{
	SetConsoleTitle("Jaewoong-bot");
	HideCursor();
	char key;
    Player player = {X_END / 2, Y_END, 0, false};
	do
	{
		system("cls");
		updatePlayer(player); 
		Goto(0, 0);
		cout << player.x << ", " << player.y; 
		for (short i = 0; i < BOT_LINE_COUNT; ++i)
		{
	    	Goto(player.x, player.y + i);
			cout << BOT_lines[BOT_LINE_COUNT - i - 1];
	    }
		Move_Key(player);
		
		Sleep(player.isJumping ? 50 : 30);
	} while(key != 27);
	
	return 0;
}


void updatePlayer(Player &player)
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


void Move_Key(Player &player)
{
    if (GetAsyncKeyState(VK_LEFT) & 0x8000) if (player.x > 0) player.x--;
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) if (player.x < X_END) player.x++;
    if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
        if (!player.isJumping)
		{
            player.isJumping = true;
            player.velocityY = player.JUMP_POWER;
        }
    }
}


void HideCursor(void)
{
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}
