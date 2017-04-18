#include <windows.h>
#include <vector>
#include "Arena.h"

#define DELTA_MOVE 10
#define WIDTH 422
#define HEIGHT 422
using namespace std;

const static char title[] = "SecondScreen";
const static char szWindowClass[] = "win32app";
UINT timerId;
const static UINT establishConnection = RegisterWindowMessage("establishConnection");
const static UINT establishedConnection = RegisterWindowMessage("establishedConnection");
const UINT bulletSend = RegisterWindowMessage("BulletSend");
UINT scoreGained = RegisterWindowMessage("ScoreGained");
HWND otherWindow = 0;
int myScore = 0,
enemyScore = 0;
Arena arena = Arena(Direction::RIGHT);

LRESULT CALLBACK WndProc(HWND hwndMain, UINT uMsg, WPARAM wParam, LPARAM lParam);
void render(HDC hdc);
void update();
void manageScore(char*);

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{


	WNDCLASS wcex;


	wcex.style = CS_OWNDC;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;


	RegisterClass(&wcex);

	POINT dimension;
	dimension.x = WIDTH;
	dimension.y = HEIGHT;
	HWND hWnd = CreateWindow(
		szWindowClass,
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		WIDTH, HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
		);
	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain

	SetTimer(hWnd, timerId, 1000 / 40, NULL);
	ShowWindow(hWnd,
		nCmdShow);
	UpdateWindow(hWnd);



	MSG msg;
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hwndMain, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;                       // handle to device context 
	RECT rect;
	PAINTSTRUCT ps;
	int mouseY;
	POINT pt;
	if (uMsg == establishConnection && otherWindow == 0)
	{
		otherWindow = (HWND)wParam;
		ReplyMessage(0);
		SendMessage(otherWindow, establishedConnection, (WPARAM)hwndMain, 0);
		return 0;
	}
	else if (uMsg == bulletSend)
	{
		arena.createBullet((int)wParam);
		return 0;
	}
	else if (uMsg == scoreGained)
	{
		myScore = (int)wParam;
		return 0;
	}
	switch (uMsg) {
	case WM_LBUTTONDOWN:
		arena.createBullet();
		return 0;
	case WM_MOUSEMOVE:

		hdc = GetWindowDC(hwndMain);
		SetMapMode(hdc, MM_LOMETRIC);
		GetCursorPos(&pt);
		DPtoLP(hdc, &pt, 1);
		ReleaseDC(hwndMain, hdc);
		mouseY = pt.y;

		if (mouseY > arena.shipToDraw().getY() - Ship::SIZEY)
			arena.movePlayer(DELTA_MOVE);
		else
			arena.movePlayer(-DELTA_MOVE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwndMain, &ps);
		GetClientRect(hwndMain, &rect);
		char score[25];
		SetMapMode(hdc, MM_LOMETRIC);
		manageScore(score);
		TextOut(hdc, 250, -10, TEXT(score), strlen(score));
		render(hdc);
		EndPaint(hwndMain, &ps);
		return 0;
	case WM_TIMER:
		InvalidateRect(hwndMain, NULL, true);

		update();
		return 0;
	case WM_DESTROY:
		KillTimer(hwndMain, timerId);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwndMain, uMsg, wParam, lParam);
}

void render(HDC hdc)
{
	list<Bullet> bullVec = arena.playerBulletsToDraw();
	int x, y;
	for (auto i = bullVec.begin(); i != bullVec.end(); ++i)
	{
		x = i->getX();
		y = i->getY();
		Ellipse(hdc, x, y, x + Bullet::SIZE, y + Bullet::SIZE);
	}
	bullVec = arena.enemiesBulletsToDraw();
	for (auto i = bullVec.begin(); i != bullVec.end(); ++i)
	{
		x = i->getX();
		y = i->getY();
		Ellipse(hdc, x, y, x + Bullet::SIZE, y + Bullet::SIZE);
	}
	Ship player = arena.shipToDraw();
	x = player.getX();
	y = player.getY();
	Rectangle(hdc, x, y, x + Ship::SIZEX, y + Ship::SIZEY);

}

void update()
{
	arena.update();
	const vector<int> bulletsToSent = arena.getBulletToSendVector();
	if (bulletsToSent.size() > 0 && otherWindow != 0)
		for_each(bulletsToSent.begin(), bulletsToSent.end(), [](int _n)
	{
		PostMessage(otherWindow, bulletSend, (WPARAM)_n, 0);
	});
	if (enemyScore != arena.getScore())
	{
		enemyScore = arena.getScore();
		PostMessage(otherWindow, scoreGained, (WPARAM)enemyScore, 0);
	}
}

void manageScore(char* str)
{
	char myScre[2], enScore[2];
	strcpy(str, "myScore: ");
	_itoa(myScore, myScre, 10);
	_itoa(enemyScore, enScore, 10);
	strcat(str, myScre);
	strcat(str, " enemyScore: ");
	strcat(str, enScore);
}