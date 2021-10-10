#include <windows.h>
#include <time.h>//시간마다 random하도록
#include<MMsystem.h>//소리나오게
#include "resource.h"//비트맵을 불러온다.
#pragma comment(lib,"winmm.lib")//#pragma매크로는 lib파일을 읽어들이는데에 쓰임
#define ID_EDIT1 200//edit 1
#define ID_EDIT2 201//edit 2
#define ID_EDIT3 202//edit 3
#define ID_EDIT4 203//edit 4

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);						// 메인 윈도우
LRESULT CALLBACK WndProcNew(HWND, UINT, WPARAM, LPARAM);					// 보너스게임 윈도우
LRESULT CALLBACK WndProc_Guide(HWND, UINT, WPARAM, LPARAM);					// 게임설명 윈도우
LRESULT CALLBACK WorldTour(HWND, UINT, WPARAM, LPARAM);						// 세계여행 윈도우
LRESULT CALLBACK Godbless(HWND, UINT, WPARAM, LPARAM);						// 신의축복 윈도우

HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Project");
HWND hEdit1, hEdit2, hEdit3, hEdit4;	// edit 소환
TCHAR str1[128], str2[128];				// edit에서 입력받은 값을 str에 저장
TCHAR nara[128];
HWND hWndNew;							// 보너스 게임 윈도우창
HWND hWnd_Guide;						//가이드 창
HWND hsin, hworld;						//신의축복,세계여행
int cheekfirstmap = 1;
int PlayMode = 0;						// 시작됬는지 감지
int button0 = 0, button1 = 0;			// 버튼눌렸는지 감지
int dicebutton = 0;						//주사위버튼 눌렸는지 감지.
int dicebutton2 = 0;					//주사위버튼 때졌는지 감지.
int dice;								//주사위수 담을 변수
int Player = 0;							//번갈아면서 하기위한 변수
int game = 0;							//게임진행 변수
int RSP0 = 0, RSP1 = 0, RSP2 = 0;		//묵찌빠 구별
int start = 0;
int RSPwin = 0;
int 무인도Player1 = 0, 무인도cnt1 = 0, 무인도Player2 = 0, 무인도cnt2 = 0;

//도시 정보 구조체
struct node
{
	TCHAR* city;
	int money;
	int MaxX;
	int MaxY;
	int MinX;
	int MinY;
	int owner;
	int building;
	int number;
	struct node* next;
};
struct node* head = 0;
struct node* cur;
struct node* cur2;

//캐릭터 정보 구조체
struct character
{
	TCHAR* name;
	int janmoney;
	int ranking;
	int landmark;
	struct character* next;
};
struct character* chead = 0;
struct character* play2; // player1 정보
struct character* play1; // player2 정보

void GamingBackGround(HDC hdc, HBITMAP MyBitmap);
void Player1(HDC hdc, HBITMAP MyBitmap, int x, int y);
void Player2(HDC hdc, HBITMAP MyBitmap, int x, int y);
void addToCity(TCHAR* city, int money, int maxx, int maxy, int minx, int miny, int city_number);
void addToChar(TCHAR* name, int janmoney, int ranking);
void DrawPicture(HDC hdc, HBITMAP MyBitmap, TCHAR file, int w_x, int w_y, int x, int y);
void CharacterTextOut(HDC hdc, int x, int y, struct character *cur);
void diceNum(HDC hdc, int dicenum);
void text(HDC hdc, struct node* cur, struct node* cur2, int Player);
void DrawRSP(HDC hdc, HBITMAP MyBitmap, TCHAR file, int w_x, int w_y, int x, int y);
void player1_building(HDC hdc, HBITMAP MyBitmap, int x, int y);
void player2_building(HDC hdc, HBITMAP MyBitmap, int x, int y);
void landmark_boom(HDC hdc, HBITMAP MyBitmap);
void landmark(HDC hdc, struct node* cur, int player);
void congratulation(HDC hdc, HBITMAP MyBitmap);
void ranking(struct character* cur);
void information();
int random();

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	//보너스게임 새 윈도우
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpfnWndProc = (WNDPROC)WndProcNew;
	WndClass.lpszClassName = TEXT("new");
	RegisterClass(&WndClass);

	// Guide창 class
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpfnWndProc = (WNDPROC)WndProc_Guide;
	WndClass.lpszClassName = TEXT("Guide");
	RegisterClass(&WndClass);

	// 신의 축복
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpfnWndProc = (WNDPROC)Godbless;
	WndClass.lpszClassName = TEXT("sin");
	RegisterClass(&WndClass);

	// 세계 여행
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpfnWndProc = (WNDPROC)WorldTour;
	WndClass.lpszClassName = TEXT("world");
	RegisterClass(&WndClass);

	//도시의 정보함수
	information();

	//랜덤 변환
	srand(time(NULL));

	hWnd = CreateWindow(lpszClass, TEXT("Project"), WS_OVERLAPPEDWINDOW,
		0, 0, 1200, 695, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	PlaySound(MAKEINTRESOURCE(IDR_WAVE1), g_hInst, SND_RESOURCE | SND_LOOP | SND_ASYNC);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

//신의 축복 윈도우
LRESULT CALLBACK Godbless(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static HBITMAP Bitmap;
	struct node* temp;
	switch (iMessage) {

	case WM_CREATE:
		hEdit3 = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE |
			ES_AUTOHSCROLL, 100, 350, 150, 30, hWnd, (HMENU)ID_EDIT3, g_hInst, NULL);
		SetWindowText(hEdit3, TEXT("나라를 선택하세요")); // 나라이름 입력받기
		CreateWindow(TEXT("button"), TEXT("확인"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			250, 340, 70, 30, hWnd, (HMENU)1, g_hInst, NULL);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 1:
			GetWindowText(hEdit3, nara, 128); // 나라이름을 nara에 저장
			temp = head;

			while (1)
			{
				temp = head;
				while (lstrcmp(temp->city, nara) != 0)
				{
					temp = temp->next;
					if (temp == 0)
					{
						break;
					}
				}
				if (temp == 0) // 입력한 이름의 땅을 찾지 못했을 때
				{
					MessageBox(hWnd, TEXT("일치하는 땅이 없습니다."), TEXT("모두의 마블"), MB_OK);
					break;
				}
				else if (temp->building == 0) // 건물이 없을 때
				{
					// 다시입력받음
					MessageBox(hWnd, TEXT("설치된 건물이 없습니다."), TEXT("모두의 마블"), MB_OK);
					break;
				}
				else // 건물이 있을 때
				{
					// 다시 입력받음
					PlaySound(MAKEINTRESOURCE(IDR_WAVE22), g_hInst, SND_RESOURCE | SND_ASYNC);
					MessageBox(hWnd, TEXT("통행료 증가!!"), TEXT("모두의 마블"), MB_OK);
					temp->money = temp->money * 2; // 통행료 두배
					Sleep(500);
					DestroyWindow(hsin); // 팝업 창 끄기
					break;
				}
			}
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		DrawPicture(hdc, Bitmap, (TCHAR)IDB_BITMAP14, 0, 0, 410, 458);
		return 0;
	case WM_DESTROY:
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

//세계 여행 윈도우
LRESULT CALLBACK WorldTour(HWND hworld, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static HBITMAP Bitmap;
	struct node* temp;
	switch (iMessage) {
	case WM_CREATE:
		hEdit4 = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE |
			ES_AUTOHSCROLL, 100, 350, 150, 30, hworld, (HMENU)ID_EDIT4, g_hInst, NULL);
		SetWindowText(hEdit4, TEXT("나라를 선택하세요.")); // 나라이름 입력받기
		CreateWindow(TEXT("button"), TEXT("확인"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			250, 340, 70, 30, hworld, (HMENU)1, g_hInst, NULL);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 1:
			GetWindowText(hEdit4, nara, 128); // 나라이름을 nara에 저장
			while (1)
			{
				temp = head;
				while (lstrcmp(temp->city, nara) != 0)
				{
					temp = temp->next;
					if (temp == 0)
					{
						MessageBox(hworld, TEXT("일치하는 땅이 없습니다."), TEXT("모두의 마블"), MB_OK);
						break;
					}
				}
				if (temp == 0) // 입력한 이름의 땅을 찾지 못했을 때
				{
					break;
				}
				else // 건물이 있을 때
				{
					if (Player % 2 == 0)
					{
						cur2 = temp;
					}
					else
					{
						cur = temp;
					}
					DestroyWindow(hworld); // 팝업 창 끄기
					PlaySound(MAKEINTRESOURCE(IDR_WAVE24), g_hInst, SND_RESOURCE | SND_ASYNC); Sleep(500);
					break;
				}
			}
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hworld, &ps);
		DrawPicture(hdc, Bitmap, (TCHAR)IDB_BITMAP27, 0, 0, 410, 458);
		return 0;
	case WM_DESTROY:
		return 0;
	}
	return(DefWindowProc(hworld, iMessage, wParam, lParam));
}

// 게임설명 창을 띄웠을 때 실행 코드
LRESULT CALLBACK WndProc_Guide(HWND hWnd_Guide, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc_Guide;
	PAINTSTRUCT ps;
	static HBITMAP Bitmap;
	static int i = 0;

	switch (iMessage) {
		// 버튼 창 띄우기
	case WM_CREATE:
		hdc_Guide = BeginPaint(hWnd_Guide, &ps);
		DrawPicture(hdc_Guide, Bitmap, (TCHAR)IDB_BITMAP14, 0, 0, 1200, 700);
		CreateWindow(TEXT("button"), TEXT("PREV"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			200, 50, 100, 25, hWnd_Guide, (HMENU)1, g_hInst, NULL);
		CreateWindow(TEXT("button"), TEXT("NEXT"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			900, 50, 100, 25, hWnd_Guide, (HMENU)2, g_hInst, NULL);

	case WM_COMMAND:
		switch LOWORD(wParam)
		{
		case 1:
			if (i != 0)
				i--;
			InvalidateRect(hWnd_Guide, NULL, TRUE);
			break;
		case 2:
			if (i != 4)
				i++;
			InvalidateRect(hWnd_Guide, NULL, TRUE);
			break;
		}
	case WM_PAINT:
		PlaySound(MAKEINTRESOURCE(IDR_WAVE3), g_hInst, SND_RESOURCE | SND_ASYNC);
		hdc_Guide = BeginPaint(hWnd_Guide, &ps);
		DrawPicture(hdc_Guide, Bitmap, (TCHAR)IDB_BITMAP28 + i, 0, 0, 1200, 700);

	case WM_DESTROY:
		return 0;
	}
	return(DefWindowProc(hWnd_Guide, iMessage, wParam, lParam));
}

//보너스 게임 실 행 창
LRESULT CALLBACK WndProcNew(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	int x, y;
	PAINTSTRUCT ps;
	static HBITMAP Bitmap;  //비트맵 출력을 위한 변수
	int 가위바위보변수;
	switch (iMessage)
	{
	case WM_DESTROY:
		//PostQuitMessage(0);
		return 0;
	case WM_LBUTTONDOWN:
		hdc = GetDC(hWnd);
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		if (x > 50 && y > 270 && x < 140 && y < 345)
		{
			PlaySound(MAKEINTRESOURCE(IDR_WAVE2), g_hInst, SND_RESOURCE | SND_ASYNC); Sleep(100);
			RSP0++;
		}
		if (x > 153 && y > 270 && x < 240 && y < 345)
		{
			PlaySound(MAKEINTRESOURCE(IDR_WAVE2), g_hInst, SND_RESOURCE | SND_ASYNC); Sleep(100);
			RSP1++;
		}
		if (x > 250 && y > 270 && x < 340 && y < 345)
		{
			PlaySound(MAKEINTRESOURCE(IDR_WAVE2), g_hInst, SND_RESOURCE | SND_ASYNC); Sleep(100);
			RSP2++;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	case WM_LBUTTONUP:
		hdc = GetDC(hWnd);
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		//묵
		if (x > 50 && y > 270 && x < 140 && y < 345)
		{
			RSP0++;
			start++;
			PlaySound(MAKEINTRESOURCE(IDR_WAVE3), g_hInst, SND_RESOURCE | SND_ASYNC);
		}
		else
		{
			if (RSP0 != 0)
				RSP0--;
			PlaySound(MAKEINTRESOURCE(IDR_WAVE3), g_hInst, SND_RESOURCE | SND_ASYNC);
		}
		//찌
		if (x > 153 && y > 270 && x < 240 && y < 345)
		{
			RSP1++;
			start++;
			PlaySound(MAKEINTRESOURCE(IDR_WAVE3), g_hInst, SND_RESOURCE | SND_ASYNC);
		}
		else
		{
			if (RSP1 != 0)
				RSP1--;
			PlaySound(MAKEINTRESOURCE(IDR_WAVE3), g_hInst, SND_RESOURCE | SND_ASYNC);
		}
		//빠
		if (x > 250 && y > 270 && x < 340 && y < 345)
		{
			RSP2++;
			start++;
			PlaySound(MAKEINTRESOURCE(IDR_WAVE3), g_hInst, SND_RESOURCE | SND_ASYNC);
		}
		else
		{
			if (RSP2 != 0)
				RSP2--;
			PlaySound(MAKEINTRESOURCE(IDR_WAVE3), g_hInst, SND_RESOURCE | SND_ASYNC);
		}
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		DrawPicture(hdc, Bitmap, IDB_BITMAP21, 0, 0, 400, 400);
		if (RSP0 % 2 == 1)
			DrawRSP(hdc, Bitmap, (TCHAR)IDB_BITMAP18, 50, 270, 368, 300);
		else
			DrawRSP(hdc, Bitmap, (TCHAR)IDB_BITMAP15, 50, 270, 368, 300);

		if (RSP1 % 2 == 1)
			DrawRSP(hdc, Bitmap, (TCHAR)IDB_BITMAP19, 153, 270, 340, 300);
		else
			DrawRSP(hdc, Bitmap, (TCHAR)IDB_BITMAP16, 153, 270, 340, 300);

		if (RSP2 % 2 == 1)
			DrawRSP(hdc, Bitmap, (TCHAR)IDB_BITMAP20, 250, 270, 358, 300);
		else
			DrawRSP(hdc, Bitmap, (TCHAR)IDB_BITMAP17, 250, 270, 358, 300);
		if (start == 1)
		{
			start--;
			for (int i = 0; i < 14; i++)
			{
				Sleep(50 + i * 30);
				DrawPicture(hdc, Bitmap, IDB_BITMAP21, 0, 0, 400, 400);
				DrawRSP(hdc, Bitmap, (TCHAR)IDB_BITMAP15 + i % 3, 153, 120, 368, 300);

				DrawRSP(hdc, Bitmap, (TCHAR)IDB_BITMAP15, 50, 270, 368, 300);//묵
				DrawRSP(hdc, Bitmap, (TCHAR)IDB_BITMAP16, 153, 270, 340, 300);//찌
				DrawRSP(hdc, Bitmap, (TCHAR)IDB_BITMAP17, 250, 270, 358, 300);//빠
			}
			가위바위보변수 = rand() % 3;
			DrawRSP(hdc, Bitmap, (TCHAR)IDB_BITMAP15 + 가위바위보변수, 153, 120, 368, 300);
			//승리조건
			if (RSP0 == 2)
			{
				RSP0 = 0;
				if (가위바위보변수 == 0)
				{
					PlaySound(MAKEINTRESOURCE(IDR_WAVE14), g_hInst, SND_RESOURCE | SND_ASYNC); Sleep(40);
					MessageBox(hWnd, TEXT("비김"), TEXT("가위! 바위! 보!"), MB_OK);
				}
				else if (가위바위보변수 == 1)
				{
					PlaySound(MAKEINTRESOURCE(IDR_WAVE13), g_hInst, SND_RESOURCE | SND_ASYNC); Sleep(40);
					MessageBox(hWnd, TEXT("이김"), TEXT("가위! 바위! 보!"), MB_OK);
					if (Player % 2 == 0)
						play2->janmoney += 300000;
					else
						play1->janmoney += 300000;
				}
				else
				{
					PlaySound(MAKEINTRESOURCE(IDR_WAVE14), g_hInst, SND_RESOURCE | SND_ASYNC); Sleep(40);
					MessageBox(hWnd, TEXT("짐"), TEXT("가위! 바위! 보!"), MB_OK);
				}
				Sleep(500);
				DestroyWindow(hWndNew);
			}
			else if (RSP1 == 2)
			{
				RSP1 = 0;
				if (가위바위보변수 == 0)
				{
					PlaySound(MAKEINTRESOURCE(IDR_WAVE14), g_hInst, SND_RESOURCE | SND_ASYNC); Sleep(40);
					MessageBox(hWnd, TEXT("짐"), TEXT("가위! 바위! 보!"), MB_OK);
				}
				else if (가위바위보변수 == 1)
				{
					PlaySound(MAKEINTRESOURCE(IDR_WAVE14), g_hInst, SND_RESOURCE | SND_ASYNC); Sleep(40);
					MessageBox(hWnd, TEXT("비김"), TEXT("가위! 바위! 보!"), MB_OK);
				}
				else
				{
					PlaySound(MAKEINTRESOURCE(IDR_WAVE13), g_hInst, SND_RESOURCE | SND_ASYNC); Sleep(40);
					MessageBox(hWnd, TEXT("이김"), TEXT("가위! 바위! 보!"), MB_OK);
					if (Player % 2 == 0)
						play2->janmoney += 300000;
					else
						play1->janmoney += 300000;
				}
				Sleep(500);
				DestroyWindow(hWndNew);
			}
			else
			{
				RSP2 = 0;
				if (가위바위보변수 == 0)
				{
					PlaySound(MAKEINTRESOURCE(IDR_WAVE13), g_hInst, SND_RESOURCE | SND_ASYNC); Sleep(40);
					MessageBox(hWnd, TEXT("이김"), TEXT("가위! 바위! 보!"), MB_OK);
					if (Player % 2 == 0)
						play2->janmoney += 300000;
					else
						play1->janmoney += 300000;
				}
				else if (가위바위보변수 == 1)
				{
					PlaySound(MAKEINTRESOURCE(IDR_WAVE14), g_hInst, SND_RESOURCE | SND_ASYNC); Sleep(40);
					MessageBox(hWnd, TEXT("짐"), TEXT("가위! 바위! 보!"), MB_OK);
				}
				else
				{
					PlaySound(MAKEINTRESOURCE(IDR_WAVE14), g_hInst, SND_RESOURCE | SND_ASYNC); Sleep(40);
					MessageBox(hWnd, TEXT("비김"), TEXT("가위! 바위! 보!"), MB_OK);
				}
				Sleep(500);
				DestroyWindow(hWndNew);
			}

		}
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

// 주 실 행 창
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	int x, y;
	static struct node* 무인도place;//무인도관련
	static struct node* 무인도place2;//무인도관련
	static HBITMAP Bitmap;  //비트맵 출력을 위한 변수
	PAINTSTRUCT ps;  //그림을 출력할 때 필요
	BOOL bShow = TRUE;  // edit창을 안보이게하기위한 변수

	switch (iMessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CREATE:
		// Edit창 띄우기
		hEdit1 = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE |
			ES_AUTOHSCROLL, 245, 240, 200, 20, hWnd, (HMENU)ID_EDIT1, g_hInst, NULL);
		SetWindowText(hEdit1, TEXT("닉네임을 입력하시오")); // player1 닉네임 입력받기
		hEdit2 = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE |
			ES_AUTOHSCROLL, 770, 240, 200, 20, hWnd, (HMENU)ID_EDIT2, g_hInst, NULL);
		SetWindowText(hEdit2, TEXT("닉네임을 입력하시오")); // player2 닉네임 입력받기
		return 0;
		//왼쪽버튼 누른 때
	case WM_LBUTTONDOWN:
		hdc = GetDC(hWnd);
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		if (PlayMode == 0)
		{
			if (x >= 475 && x <= 715 && y >= 300 && y <= 345) //'게임 시작' 버튼의 마우스 좌표
			{
				PlaySound(MAKEINTRESOURCE(IDR_WAVE2), g_hInst, SND_RESOURCE | SND_ASYNC);
				button0++;
				cur = head;
				cur2 = head;
			}
			else if (x >= 475 && x <= 715 && y >= 370 && y <= 417) //'게임 설명' 버튼의 마우스 좌표
			{
				PlaySound(MAKEINTRESOURCE(IDR_WAVE2), g_hInst, SND_RESOURCE | SND_ASYNC);
				button1++;
			}
		}
		else if (PlayMode == 1)
		{
			if (x >= 880 && x <= 965 && y >= 475 && y <= 530)
			{
				dicebutton++;
			}
		}
		InvalidateRect(hWnd, NULL, FALSE);
		ReleaseDC(hWnd, hdc);
		return 0;
		//왼쪽버튼 땔 때
	case WM_LBUTTONUP:
		hdc = GetDC(hWnd);
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		// Main화면일때
		if (PlayMode == 0)
		{
			if (x >= 475 && x <= 715 && y >= 300 && y <= 345)//게임 시작
			{
				Sleep(20);
				PlayMode++;
				// Edit창 숨기기
				if (bShow)
				{
					bShow = FALSE;
					ShowWindow(hEdit1, SW_HIDE);
					ShowWindow(hEdit2, SW_HIDE);
				}
				GetWindowText(hEdit2, str1, 128);		// 닉네임을 str1에 저장.
				addToChar(str1, 1000000, 0);			// player1 캐릭터 node를 만든다.
				GetWindowText(hEdit1, str2, 128);		// 닉네임을 str2에 저장.
				addToChar(str2, 1000000, 0);			// player2 캐릭터 node를 만든다
			}
			else if (x >= 475 && x <= 715 && y >= 370 && y <= 417)//게임 설명
			{
				PlaySound(MAKEINTRESOURCE(IDR_WAVE3), g_hInst, SND_RESOURCE | SND_ASYNC);
				Sleep(20);
				button1--;
				hWnd_Guide = CreateWindow(TEXT("Guide"), TEXT("Guide"), WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU,
					0, 0, 1200, 740, hWnd_Guide, (HMENU)0, g_hInst, NULL);
				ShowWindow(hWnd_Guide, SW_SHOW);
			}
			else
			{
				if (button0 != 0)
				{
					PlaySound(MAKEINTRESOURCE(IDR_WAVE1), g_hInst, SND_RESOURCE | SND_LOOP | SND_ASYNC);
					button0--;
				}
				if (button1 != 0)
				{
					PlaySound(MAKEINTRESOURCE(IDR_WAVE1), g_hInst, SND_RESOURCE | SND_LOOP | SND_ASYNC);
					button1--;
				}
			}
		}
		//PlayMode1일댜
		else
		{

			if (x >= 880 && x <= 965 && y >= 475 && y <= 530)
			{
				dicebutton2++;
				if (dicebutton != 0)
					dicebutton--;
				game++;
				if (무인도Player1 == 1)
				{
					무인도place = cur; //무인도의 위치를 저장
				}
				if (무인도Player2 == 1)
				{
					무인도place2 = cur2; //무인도의 위치를 저장
				}
				dice = random();
				for (int j = 0; j < dice; j++)
				{

					if (Player % 2 == 0)
					{
						if (cur->next == 0)
							cur = head;
						else
							cur = cur->next;
					}
					else
					{
						if (cur2->next == 0)
							cur2 = head;
						else
							cur2 = cur2->next;
					}
				}
				if (무인도Player1 == 1)
				{
					if (Player % 2 == 0)
					{
						if (무인도cnt1 != 2)
						{
							cur = 무인도place;
							무인도cnt1++;
						}
						else
						{
							무인도place = head;
							무인도cnt1 = 0;
							무인도Player1 = 0;
						}
					}
				}
				if (무인도Player2 == 1)
				{
					if (Player % 2 == 1)
					{
						if (무인도cnt2 != 2)
						{
							cur2 = 무인도place2;
							무인도cnt2++;
						}
						else
						{
							무인도place2 = head;
							무인도cnt2 = 0;
							무인도Player2 = 0;
						}
					}
				}
				Player++;
			}
			else
			{
				if (dicebutton != 0)
					dicebutton--;
			}
		}
		InvalidateRect(hWnd, NULL, FALSE);
		ReleaseDC(hWnd, hdc);
		return 0;
		//그릴 때
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (PlayMode == 0)
		{
			//main그림
			DrawPicture(hdc, Bitmap, (TCHAR)IDB_BITMAP1, 0, 0, 1200, 700);
			if (button0 % 2 == 1)
				DrawPicture(hdc, Bitmap, (TCHAR)IDB_BITMAP3, 473, 295, 250, 53); // 게임시작 들어간 버튼 모습

			else if (button1 % 2 == 1)
				DrawPicture(hdc, Bitmap, (TCHAR)IDB_BITMAP4, 475, 370, 250, 52); // 게임방법 들어간 버튼 모습
		}
		else
		{
			if (cheekfirstmap)
			{
				PlaySound(MAKEINTRESOURCE(IDR_WAVE4), g_hInst, SND_RESOURCE | SND_ASYNC);
				Sleep(800);
				cheekfirstmap = 0;
			}
			//game그림
			GamingBackGround(hdc, Bitmap);
			DrawPicture(hdc, Bitmap, (TCHAR)IDB_BITMAP6, 900, 90, 170, 170);
			DrawPicture(hdc, Bitmap, (TCHAR)IDB_BITMAP8, 70, 450, 170, 170);
			Player1(hdc, Bitmap, cur->MinX + 10, cur->MinY - 15);
			Player2(hdc, Bitmap, cur2->MinX + 60, cur2->MinY - 15);
			play1 = chead;
			play2 = chead->next;
			ranking(chead);
			CharacterTextOut(hdc, 385, 463, play1); // player1 정보 캐릭터창 띄우기
			CharacterTextOut(hdc, 795, 98, play2); // player2 정보 캐릭터창 띄우기
			text(hdc, cur, cur2, Player);
			if (dicebutton2 % 2 == 1)
			{
				dicebutton2--;
				if (Player % 2 == 0)
				{
					DrawPicture(hdc, Bitmap, (TCHAR)IDB_BITMAP6, 900, 90, 170, 170);
					DrawPicture(hdc, Bitmap, (TCHAR)IDB_BITMAP8, 70, 450, 170, 170);
				}
				if (Player % 2 == 1)
				{
					DrawPicture(hdc, Bitmap, (TCHAR)IDB_BITMAP5, 900, 90, 170, 170);
					DrawPicture(hdc, Bitmap, (TCHAR)IDB_BITMAP9, 70, 450, 170, 170);
				}
				Player1(hdc, Bitmap, cur->MinX + 10, cur->MinY - 15);
				Player2(hdc, Bitmap, cur2->MinX + 60, cur2->MinY - 15);
				diceNum(hdc, dice);
			}
			if (dicebutton % 2 == 1) // 버튼을 누르고 있을 때
			{
				DrawPicture(hdc, Bitmap, TCHAR(IDB_BITMAP13), 870, 470, 120, 80);
				PlaySound(MAKEINTRESOURCE(IDR_WAVE2), g_hInst, SND_RESOURCE | SND_ASYNC);
			}
			else if (dicebutton % 2 == 0) // 버튼을 눌렀다가 뗐을 때
			{
				DrawPicture(hdc, Bitmap, TCHAR(IDB_BITMAP12), 870, 470, 120, 80);
				PlaySound(MAKEINTRESOURCE(IDR_WAVE3), g_hInst, SND_RESOURCE | SND_ASYNC);
			}
			/////////////////////////////////////////////////////// 
			struct node* drawing = head;

			while (drawing->next != 0)
			{
				switch (drawing->owner)
				{
				case 0:
					break;
				case 1:
					switch (drawing->building)
					{
					case 1:
						player1_building(hdc, Bitmap, drawing->MinX, drawing->MinY);
						break;

					case 2:
						landmark(hdc, drawing, drawing->owner);
						break;
					}
					break;
				case 2:
					switch (drawing->building)
					{
					case 1:
						player2_building(hdc, Bitmap, drawing->MinX, drawing->MinY);
						break;
					case 2:
						landmark(hdc, drawing, drawing->owner);
						break;
					}
					break;
				}
				drawing = drawing->next;
			}
			/////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////
			struct character* temp = chead;
			if (game % 2 == 1)
			{
				game--;
				//player 1의 차례일 때
				if (Player % 2 == 1)
				{
					if (cur->money == 0)
					{
						//출발, 포춘카드, 무인도, 신의 축복 등 등
						if (cur->number == 0)
						{
							//출발지점월급
							play1->janmoney = play1->janmoney + 100000;//월급10만원
							return 0;
						}
						if (cur->number == 4 || cur->number == 28)
						{
							//보너스 게임
							hWndNew = CreateWindow(TEXT("new"), TEXT("Bonus Game"), WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU,
								100, 100, 400, 400, hWnd, (HMENU)0, g_hInst, NULL);
							ShowWindow(hWndNew, SW_SHOW);

							return 0;
						}
						if (cur->number == 8 || cur->number == 24)
						{
							//신의 축복
							PlaySound(MAKEINTRESOURCE(IDR_WAVE20), g_hInst, SND_RESOURCE | SND_ASYNC);
							hsin = CreateWindow(TEXT("sin"), TEXT("신의축복"), WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU,
								100, 100, 420, 450, hWnd, (HMENU)0, g_hInst, NULL);
							ShowWindow(hsin, SW_SHOW);
							return 0;
						}
						if (cur->number == 12)
						{
							//출발지점으로
							cur = head;
							play1->janmoney = play1->janmoney + 100000;//월급10만원
							return 0;
						}
						if (cur->number == 16)
						{
							//월드 투어
							PlaySound(MAKEINTRESOURCE(IDR_WAVE20), g_hInst, SND_RESOURCE | SND_ASYNC);
							hworld = CreateWindow(TEXT("world"), TEXT("세계여행"), WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU,
								100, 100, 420, 450, hWnd, (HMENU)0, g_hInst, NULL);
							ShowWindow(hworld, SW_SHOW);
							return 0;
						}
						if (cur->number == 20)
						{
							무인도Player1 = 1;
							if (무인도cnt1 == 0)
							{
								PlaySound(MAKEINTRESOURCE(IDR_WAVE21), g_hInst, SND_RESOURCE | SND_ASYNC);
								MessageBox(hWnd, TEXT("감옥입니다. 3턴 쉬세요."), TEXT("모두의 마블"), MB_OK);
							}
							if (무인도cnt1 == 1)
								MessageBox(hWnd, TEXT("2턴 남았어요."), TEXT("모두의 마블"), MB_OK);
							if (무인도cnt1 == 2)
								MessageBox(hWnd, TEXT("1턴 남았어요."), TEXT("모두의 마블"), MB_OK);
							//감옥
							//3턴쉬시오 메시지
							return 0;
						}
						return 0;
					}

					if (cur->owner == 0 || cur->owner == 1) //빈 땅이거나 내 땅일 경우
					{
						switch (cur->building)
						{
						case 0: //빈 땅
							if (MessageBox(hWnd, TEXT("건물을 건설하시겠습니까?"), TEXT("모두의 마블"), MB_OKCANCEL) == IDOK)
							{
								if (play1->janmoney - cur->money < 0)
								{
									if (MessageBox(hWnd, TEXT("잔액이 부족합니다."), TEXT("모두의 마블"), MB_OK) == IDOK)
									{
										return 0;
									}
								}
								PlaySound(MAKEINTRESOURCE(IDR_WAVE15), g_hInst, SND_RESOURCE | SND_ASYNC);
								cur->owner = 1;
								cur->building = 1;
								player1_building(hdc, Bitmap, cur->MinX, cur->MinY);
								play1->janmoney = play1->janmoney - cur->money;
								cur->money = cur->money * 2;
							}
							return 0;
						case 1: //건물만 세워져 있는 경우
							if (MessageBox(hWnd, TEXT("랜드마크를 건설하시겠습니까?"), TEXT("모두의 마블"), MB_OKCANCEL) == IDOK)
							{
								if (play1->janmoney - cur->money < 0)
								{
									if (MessageBox(hWnd, TEXT("잔액이 부족합니다."), TEXT("모두의 마블"), MB_OK) == IDOK)
									{
										return 0;
									}
								}
								PlaySound(MAKEINTRESOURCE(IDR_WAVE17), g_hInst, SND_RESOURCE | SND_ASYNC);
								cur->building = 2;
								landmark(hdc, cur, cur->owner);
								landmark_boom(hdc, Bitmap);
								play1->janmoney = play1->janmoney - cur->money;
								cur->money = cur->money * 3;
								play1->landmark += 1;
								if (play1->landmark == 10)                //승리 조건 맞는지 확인
								{
									congratulation(hdc, Bitmap);
									PlaySound(MAKEINTRESOURCE(IDR_WAVE23), g_hInst, SND_RESOURCE | SND_ASYNC);
									MessageBox(hWnd, TEXT("Player2 승"), TEXT("모두의 마블"), MB_OK);
									Sleep(3000);
									PostQuitMessage(0);
								}
							}
							return 0;
						case 2: //내 땅에 이미 랜드마크가 세워져 있는 경우
							return 0;
						}
						return 0;
					}

					else if (cur->owner == 2) //간 땅이 남의 땅이었을 경우
					{
						if (play1->janmoney - cur->money < 0) //통행료 낼 돈도 없어서 파산
						{
							if (MessageBox(hWnd, TEXT("잔액이 부족합니다."), TEXT("모두의 마블"), MB_OK) == IDOK)
							{
								congratulation(hdc, Bitmap);
								PlaySound(MAKEINTRESOURCE(IDR_WAVE23), g_hInst, SND_RESOURCE | SND_ASYNC);
								MessageBox(hWnd, TEXT("Player1 승"), TEXT("모두의 마블"), MB_OK);
								Sleep(3000);
								PostQuitMessage(0);
							}
						}
						//통행료 내고
						play1->janmoney = play1->janmoney - cur->money;
						play2->janmoney = play2->janmoney + cur->money;
						switch (cur->building)
						{
						case 1: //건물만 세워져 있음 == 인수하고 랜드마크를 세울 수 있음
							if (MessageBox(hWnd, TEXT("건물을 인수하시겠습니까?"), TEXT("모두의 마블"), MB_OKCANCEL) == IDOK)
							{
								if (play1->janmoney - cur->money < 0)
								{
									if (MessageBox(hWnd, TEXT("잔액이 부족합니다."), TEXT("모두의 마블"), MB_OK) == IDOK)
									{
										return 0;
									}
								}
								PlaySound(MAKEINTRESOURCE(IDR_WAVE16), g_hInst, SND_RESOURCE | SND_ASYNC);
								cur->owner = 1;
								player1_building(hdc, Bitmap, cur->MinX, cur->MinY);
								play1->janmoney = play1->janmoney - cur->money; //인수 비용 부과
								play2->janmoney = play2->janmoney + cur->money;
							}
							return 0;
						case 2: //이미 랜드마크가 세워져 있으므로 통행료 부과
							return 0;
						}
						return 0;
					}
				}

				//2player
				else
				{
					if (cur2->money == 0)
					{
						//포춘카드, 무인도, 신의 축복 등 등
						if (cur2->number == 0)
						{
							//출발지점월급
							play2->janmoney = play2->janmoney + 100000;//월급10만원
							return 0;
						}
						if (cur2->number == 4 || cur2->number == 28)
						{
							//보너스 게임
							hWndNew = CreateWindow(TEXT("new"), TEXT("Bonus Game"), WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU,
								100, 100, 400, 400, hWnd, (HMENU)0, g_hInst, NULL);
							ShowWindow(hWndNew, SW_SHOW);

							return 0;
						}
						if (cur2->number == 8 || cur2->number == 24)
						{
							//신의 축복
							PlaySound(MAKEINTRESOURCE(IDR_WAVE20), g_hInst, SND_RESOURCE | SND_ASYNC);
							hsin = CreateWindow(TEXT("sin"), TEXT("신의축복"), WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU,
								100, 100, 420, 450, hWnd, (HMENU)0, g_hInst, NULL);
							ShowWindow(hsin, SW_SHOW);
							return 0;
						}
						if (cur2->number == 12)
						{
							//출발지점으로
							cur2 = head;
							play2->janmoney = play2->janmoney + 100000;//월급10만원
							return 0;
						}
						if (cur2->number == 16)
						{
							//월드 투어
							PlaySound(MAKEINTRESOURCE(IDR_WAVE20), g_hInst, SND_RESOURCE | SND_ASYNC);
							hworld = CreateWindow(TEXT("world"), TEXT("세계여행"), WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU,
								100, 100, 420, 450, hWnd, (HMENU)0, g_hInst, NULL);
							ShowWindow(hworld, SW_SHOW);
							return 0;
						}
						if (cur2->number == 20)
						{
							무인도Player2 = 1;
							if (무인도cnt2 == 0)
							{
								MessageBox(hWnd, TEXT("감옥입니다. 3턴 쉬세요."), TEXT("모두의 마블"), MB_OK);
								PlaySound(MAKEINTRESOURCE(IDR_WAVE21), g_hInst, SND_RESOURCE | SND_ASYNC);
							}
							if (무인도cnt2 == 1)
								MessageBox(hWnd, TEXT("2턴 남았어요"), TEXT("모두의 마블"), MB_OK);
							if (무인도cnt2 == 2)
								MessageBox(hWnd, TEXT("1턴 남았어요"), TEXT("모두의 마블"), MB_OK);
							//감옥
							return 0;
						}
						return 0;
					}

					if (cur2->owner == 0 || cur2->owner == 2) //빈 땅이거나 내 땅일 경우
					{
						switch (cur2->building)
						{
						case 0: //빈 땅
							if (MessageBox(hWnd, TEXT("건물을 건설하시겠습니까?"), TEXT("모두의 마블"), MB_OKCANCEL) == IDOK)
							{
								//캐릭터 잔액 바꿔주기
								if (play2->janmoney - cur2->money < 0)
								{
									if (MessageBox(hWnd, TEXT("잔액이 부족합니다."), TEXT("모두의 마블"), MB_OK) == IDOK)
									{
										return 0;
									}
								}
								PlaySound(MAKEINTRESOURCE(IDR_WAVE15), g_hInst, SND_RESOURCE | SND_ASYNC);
								cur2->owner = 2;
								cur2->building = 1;
								player2_building(hdc, Bitmap, cur2->MinX, cur2->MinY);
								play2->janmoney = play2->janmoney - cur2->money;
								cur2->money = cur2->money * 2;
							}
							return 0;
						case 1: //내 땅에 건물만 세워져 있는 경우
							if (MessageBox(hWnd, TEXT("랜드마크를 건설하시겠습니까?"), TEXT("모두의 마블"), MB_OKCANCEL) == IDOK)
							{
								//캐릭터 잔액 바꿔주기
								if (play2->janmoney - cur2->money < 0)
								{
									if (MessageBox(hWnd, TEXT("잔액이 부족합니다."), TEXT("모두의 마블"), MB_OK) == IDOK)
									{
										return 0;
									}
								}
								PlaySound(MAKEINTRESOURCE(IDR_WAVE17), g_hInst, SND_RESOURCE | SND_ASYNC);
								cur2->building = 2;
								landmark(hdc, cur2, cur2->owner);
								landmark_boom(hdc, Bitmap);
								play2->janmoney = play2->janmoney - cur2->money;
								cur2->money = cur2->money * 3;
								play2->landmark += 1;
								if (play2->landmark == 10)                        //승리 조건 맞는지 확인
								{
									congratulation(hdc, Bitmap);
									PlaySound(MAKEINTRESOURCE(IDR_WAVE23), g_hInst, SND_RESOURCE | SND_ASYNC);
									MessageBox(hWnd, TEXT("Player1 승"), TEXT("모두의 마블"), MB_OK);
									Sleep(3000);
									PostQuitMessage(0);
								}
							}
							return 0;
						case 2: //내 땅에 이미 랜드마크가 세워져 있는 경우
							return 0;
						}
						return 0;
					}

					else if (cur2->owner == 1) //남의 땅에 간 경우
					{
						if (play2->janmoney - cur2->money < 0) //통행료 낼 돈도 없어서 파산
						{
							if (MessageBox(hWnd, TEXT("잔액이 부족합니다."), TEXT("모두의 마블"), MB_OK) == IDOK)
							{
								congratulation(hdc, Bitmap);
								PlaySound(MAKEINTRESOURCE(IDR_WAVE23), g_hInst, SND_RESOURCE | SND_ASYNC);
								MessageBox(hWnd, TEXT("Player2 승"), TEXT("모두의 마블"), MB_OK);
								Sleep(3000);
								PostQuitMessage(0);
							}
						}
						//통행료 부과
						play2->janmoney = play2->janmoney - cur2->money;
						play1->janmoney = play1->janmoney + cur2->money;
						switch (cur2->building)
						{
						case 1: //건물만 세워져 있어서 인수가능
							if (MessageBox(hWnd, TEXT("건물을 인수하시겠습니까?"), TEXT("모두의 마블"), MB_OKCANCEL) == IDOK)
							{
								if (play2->janmoney - cur2->money < 0)
								{
									if (MessageBox(hWnd, TEXT("잔액이 부족합니다."), TEXT("모두의 마블"), MB_OK) == IDOK)
									{
										return 0;
									}
								}
								PlaySound(MAKEINTRESOURCE(IDR_WAVE16), g_hInst, SND_RESOURCE | SND_ASYNC);
								cur2->owner = 2;
								player2_building(hdc, Bitmap, cur2->MinX, cur2->MinY);
								play2->janmoney = play2->janmoney - cur2->money; //인수 비용 부과
								play1->janmoney = play1->janmoney + cur2->money;
							}
							return 0;
						case 2: //이미 랜드마크가 세워져 있으므로 통행료만 부과
							return 0;
						}
						return 0;
					}
				}

			}
			///////////////////////////////////////////////////////////
		}
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

//그리기기기
void DrawPicture(HDC hdc, HBITMAP MyBitmap, TCHAR file, int w_x, int w_y, int x, int y)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	MemDC = CreateCompatibleDC(hdc);
	MyBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(file));
	OldBitmap = (HBITMAP)SelectObject(MemDC, MyBitmap);
	StretchBlt(hdc, w_x, w_y, x, y, MemDC, 0, 0, x, y, SRCCOPY);
	SelectObject(MemDC, OldBitmap);
	DeleteObject(MyBitmap);
	DeleteDC(MemDC);
}


// 묵 찌 빠
void DrawRSP(HDC hdc, HBITMAP MyBitmap, TCHAR file, int w_x, int w_y, int x, int y)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	MemDC = CreateCompatibleDC(hdc);
	MyBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(file));
	OldBitmap = (HBITMAP)SelectObject(MemDC, MyBitmap);
	StretchBlt(hdc, w_x, w_y, x / 4, y / 4, MemDC, 0, 0, x, y, SRCCOPY);
	SelectObject(MemDC, OldBitmap);
	DeleteObject(MyBitmap);
	DeleteDC(MemDC);
}

//gaming화면
void GamingBackGround(HDC hdc, HBITMAP MyBitmap)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	MemDC = CreateCompatibleDC(hdc);
	MyBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
	OldBitmap = (HBITMAP)SelectObject(MemDC, MyBitmap);
	StretchBlt(hdc, 0, 0, 1200, 700, MemDC, 0, 0, 1200, 740, SRCCOPY);
	SelectObject(MemDC, OldBitmap);
	DeleteObject(MyBitmap);
	DeleteDC(MemDC);
}

//player2
void Player1(HDC hdc, HBITMAP MyBitmap, int x, int y)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	MemDC = CreateCompatibleDC(hdc);
	MyBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP10));
	OldBitmap = (HBITMAP)SelectObject(MemDC, MyBitmap);
	StretchBlt(hdc, x - 10, y + 17, 50, 75, MemDC, 0, 0, 100, 150, SRCAND);
	SelectObject(MemDC, OldBitmap);
	DeleteObject(MyBitmap);
	DeleteDC(MemDC);
}

//player1
void Player2(HDC hdc, HBITMAP MyBitmap, int x, int y)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	MemDC = CreateCompatibleDC(hdc);
	MyBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP7));
	OldBitmap = (HBITMAP)SelectObject(MemDC, MyBitmap);
	StretchBlt(hdc, x + 5, y + 17, 45, 75, MemDC, 0, 0, 100, 150, SRCAND);
	SelectObject(MemDC, OldBitmap);
	DeleteObject(MyBitmap);
	DeleteDC(MemDC);
}

//도시 정보 구조체 추가기능
void addToCity(TCHAR* city, int money, int maxx, int maxy, int minx, int miny, int city_number)
{
	struct node* _city;
	_city = (struct node*)malloc(sizeof(struct node));
	_city->city = (TCHAR *)malloc(lstrlen(city) + 1);
	_city->city = city;
	_city->money = money;
	_city->MaxX = maxx;
	_city->MaxY = maxy;
	_city->MinX = minx;
	_city->MinY = miny;
	_city->number = city_number;
	_city->building = 0;
	_city->owner = 0;
	_city->next = 0;
	if (head == 0)
	{
		head = _city;
		return;
	}
	{
		struct node* find = head;
		while (find->next != 0)
		{
			find = find->next;
		}
		find->next = _city;
		return;
	}
}

//캐릭터 정보 구조체 추가기능
void addToChar(TCHAR* name, int janmoney, int ranking)
{
	struct character* _char;
	_char = (struct character*)malloc(sizeof(struct character));
	_char->name = (TCHAR *)malloc(lstrlen(name) + 1);
	_char->name = name;
	_char->janmoney = janmoney;
	_char->ranking = ranking;
	_char->next = 0;
	if (chead == 0)
	{
		chead = _char;
		return;
	}
	{
		struct character* find = chead;
		while (find->next != 0)
		{
			find = find->next;
		}
		find->next = _char;
		return;
	}
}

//도시 정보 추가
void information()
{
	addToCity(TEXT("START"), 0, 1145, 640, 1020, 570, 0);			//출발
	addToCity(TEXT("그리스"), 10000, 1020, 640, 900, 570, 1);		//1
	addToCity(TEXT("네덜란드"), 15000, 900, 640, 780, 570, 2);		//2
	addToCity(TEXT("덴마크"), 20000, 780, 640, 660, 570, 3);			//3
	addToCity(TEXT("Bonus Game"), 0, 660, 640, 535, 570, 4);		//게임
	addToCity(TEXT("독일"), 25000, 660, 570, 535, 500, 5);			//4
	addToCity(TEXT("루마니아"), 30000, 660, 500, 535, 430, 6);		//5
	addToCity(TEXT("멕시코"), 35000, 660, 436, 535, 364, 7);			//6
	addToCity(TEXT("God Bless"), 0, 660, 370, 535, 294, 8);			//무인도
	addToCity(TEXT("리투아니아"), 40000, 660, 300, 535, 224, 9);		//7
	addToCity(TEXT("몰도바"), 45000, 660, 230, 535, 154, 10);		//8
	addToCity(TEXT("벨기에"), 50000, 660, 160, 535, 84, 11);			//9
	addToCity(TEXT("출발지점으로~!"), 0, 660, 90, 535, 14, 12);		//음?
	addToCity(TEXT("불가리아"), 55000, 535, 90, 415, 14, 13);		//10
	addToCity(TEXT("스웨덴"), 60000, 415, 90, 290, 14, 14);			//11
	addToCity(TEXT("칠레"), 65000, 290, 90, 170, 14, 15);			//12
	addToCity(TEXT("World Tour"), 0, 170, 90, 45, 14, 16);			//뭐하
	addToCity(TEXT("스위스"), 70000, 170, 160, 45, 84, 17);			//13
	addToCity(TEXT("스페인"), 75000, 170, 230, 45, 154, 18);			//14
	addToCity(TEXT("영국"), 80000, 170, 300, 45, 224, 19);			//15
	addToCity(TEXT("Prison"), 0, 170, 370, 45, 300, 20);			//ㅎㅎ
	addToCity(TEXT("오스트리아"), 85000, 290, 370, 170, 294, 21);	//16
	addToCity(TEXT("이탈리아"), 90000, 415, 370, 290, 294, 22);		//17
	addToCity(TEXT("페루"), 100000, 535, 370, 415, 294, 23);			//18
	addToCity(TEXT("God Bless"), 0, 660, 370, 535, 294, 24);		//무인도
	addToCity(TEXT("체코"), 105000, 780, 370, 660, 294, 25);			//19
	addToCity(TEXT("크로아티아"), 110000, 900, 370, 780, 294, 26);	//20
	addToCity(TEXT("폴란드"), 115000, 1020, 370, 900, 294, 27);		//21
	addToCity(TEXT("Bonus Game"), 0, 1145, 370, 1020, 294, 28);		//끝
	addToCity(TEXT("프랑스"), 120000, 1145, 436, 1020, 364, 29);		//22
	addToCity(TEXT("핀란드"), 125000, 1145, 500, 1020, 430, 30);		//23
	addToCity(TEXT("헝가리"), 130000, 1145, 570, 1020, 494, 31);		//24
}

//주사위
int random()
{
	return rand() % 6 + 1;
}

//주사위 보이게
void diceNum(HDC hdc, int dicenum)
{
	HDC MemDC;
	HBITMAP MyBitmap, OldBitmap;
	MemDC = CreateCompatibleDC(hdc);
	MyBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP11));
	OldBitmap = (HBITMAP)SelectObject(MemDC, MyBitmap);
	switch (dicenum)
	{
	case 1:
		BitBlt(hdc, 640, 340, 230, 270, MemDC, 0, 0, SRCAND);
		PlaySound(MAKEINTRESOURCE(IDR_WAVE6), g_hInst, SND_RESOURCE | SND_ASYNC);
		return;
	case 2:
		BitBlt(hdc, 640, 335, 230, 270, MemDC, 230, 0, SRCAND);
		PlaySound(MAKEINTRESOURCE(IDR_WAVE7), g_hInst, SND_RESOURCE | SND_ASYNC);
		return;
	case 3:
		BitBlt(hdc, 640, 330, 230, 270, MemDC, 470, 0, SRCAND);
		PlaySound(MAKEINTRESOURCE(IDR_WAVE8), g_hInst, SND_RESOURCE | SND_ASYNC);
		return;
	case 4:
		BitBlt(hdc, 640, 325, 230, 270, MemDC, 730, 0, SRCAND);
		PlaySound(MAKEINTRESOURCE(IDR_WAVE9), g_hInst, SND_RESOURCE | SND_ASYNC);
		return;
	case 5:
		BitBlt(hdc, 640, 330, 230, 270, MemDC, 980, 0, SRCAND);
		PlaySound(MAKEINTRESOURCE(IDR_WAVE10), g_hInst, SND_RESOURCE | SND_ASYNC);
		return;
	case 6:
		BitBlt(hdc, 640, 340, 230, 270, MemDC, 1230, 0, SRCAND);
		PlaySound(MAKEINTRESOURCE(IDR_WAVE11), g_hInst, SND_RESOURCE | SND_ASYNC);
		return;
	}
	SelectObject(MemDC, OldBitmap);
	DeleteObject(MyBitmap);
	DeleteDC(MemDC);
}

//도시 정보 출력
void text(HDC hdc, struct node* cur, struct node* cur2, int Player)
{
	HFONT font, oldfont;
	TCHAR str[128];

	font = CreateFont(30, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, TEXT("맑은고딕"));
	oldfont = (HFONT)SelectObject(hdc, font);
	SetTextColor(hdc, RGB(0, 0, 0));
	SetBkMode(hdc, TRANSPARENT);
	if (Player % 2 == 0)
	{
		TextOut(hdc, 200, 110, TEXT(">Player 1"), 10);
		TextOut(hdc, 200, 160, cur2->city, lstrlen(cur2->city));
		wsprintf(str, TEXT("%d"), cur2->money);
		TextOut(hdc, 200, 200, str, lstrlen(str));
	}
	else
	{
		TextOut(hdc, 200, 110, TEXT(">Player 2"), 10);
		TextOut(hdc, 200, 160, cur->city, lstrlen(cur->city));
		wsprintf(str, TEXT("%d"), cur->money);
		TextOut(hdc, 200, 200, str, lstrlen(str));
	}
	SelectObject(hdc, oldfont);
	DeleteObject(font);
}

//캐릭터 정보 출력
void CharacterTextOut(HDC hdc, int x, int y, struct character *cur)
{
	HFONT font, oldfont;
	TCHAR str[128];
	font = CreateFont(15, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, TEXT("돋음"));
	oldfont = (HFONT)SelectObject(hdc, font);
	SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, x, y, cur->name, lstrlen(cur->name)); // 캐릭터 이름 출력
	wsprintf(str, TEXT("%d"), cur->janmoney);
	TextOut(hdc, x - 10, y + 45, str, lstrlen(str));// 캐릭터 money 출력
	wsprintf(str, TEXT("%d"), cur->ranking);
	TextOut(hdc, x + 50, y + 95, str, lstrlen(str)); // 캐릭터 순위 출력
	SelectObject(hdc, oldfont);
	DeleteObject(font);
}

//랜드마크 출력
void landmark(HDC hdc, struct node* cur, int player)
{
#define t 39
#define p1 9
#define p2 70
	HDC MemDC;
	HBITMAP MyBitmap, OldBitmap;
	MemDC = CreateCompatibleDC(hdc);
	MyBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP24));
	OldBitmap = (HBITMAP)SelectObject(MemDC, MyBitmap);
	if (player == 1)
	{
		switch (cur->number)
		{
		case 1:
			StretchBlt(hdc, cur->MinX + p1, cur->MinY + t, 37, 23, MemDC, 0, 0, 70, 45, SRCCOPY);
			return;
		case 2:
			StretchBlt(hdc, cur->MinX + p1, cur->MinY + t, 37, 23, MemDC, 70, 0, 70, 45, SRCCOPY);
			return;
		case 3:
			StretchBlt(hdc, cur->MinX + p1, cur->MinY + t, 37, 23, MemDC, 140, 0, 70, 45, SRCCOPY);
			return;
		case 5:
			StretchBlt(hdc, cur->MinX + p1, cur->MinY + t, 37, 23, MemDC, 205, 0, 70, 45, SRCCOPY);
			return;
		case 6:
			StretchBlt(hdc, cur->MinX + p1, cur->MinY + t, 37, 23, MemDC, 278, 0, 70, 45, SRCCOPY);
			return;
		case 7:
			StretchBlt(hdc, cur->MinX + p1, cur->MinY + t, 37, 23, MemDC, 347, 0, 70, 45, SRCCOPY);
			return;
		case 9:
			StretchBlt(hdc, cur->MinX + p1, cur->MinY + t, 37, 23, MemDC, 0, 45, 70, 45, SRCCOPY);
			return;
		case 10:
			StretchBlt(hdc, cur->MinX + p1, cur->MinY + t, 37, 23, MemDC, 70, 45, 70, 45, SRCCOPY);
			return;
		case 11:
			StretchBlt(hdc, cur->MinX + p1, cur->MinY + t, 37, 23, MemDC, 140, 45, 70, 45, SRCCOPY);
			return;
		case 13:
			StretchBlt(hdc, cur->MinX + p1, cur->MinY + t, 37, 23, MemDC, 205, 45, 70, 45, SRCCOPY);
			return;
		case 14:
			StretchBlt(hdc, cur->MinX + p1, cur->MinY + t, 37, 23, MemDC, 278, 45, 70, 45, SRCCOPY);
			return;
		case 15:
			StretchBlt(hdc, cur->MinX + p1, cur->MinY + t, 37, 23, MemDC, 347, 45, 70, 45, SRCCOPY);
			return;
		case 17:
			StretchBlt(hdc, cur->MinX + p1, cur->MinY + t, 37, 23, MemDC, 0, 90, 70, 45, SRCCOPY);
			return;
		case 18:
			StretchBlt(hdc, cur->MinX + p1, cur->MinY + t, 37, 23, MemDC, 70, 90, 70, 45, SRCCOPY);
			return;
		case 19:
			StretchBlt(hdc, cur->MinX + p1, cur->MinY + t, 37, 23, MemDC, 140, 90, 70, 45, SRCCOPY);
			return;
		case 21:
			StretchBlt(hdc, cur->MinX + p1, cur->MinY + t, 37, 23, MemDC, 205, 90, 70, 45, SRCCOPY);
			return;
		case 22:
			StretchBlt(hdc, cur->MinX + p1, cur->MinY + t, 37, 23, MemDC, 278, 90, 70, 45, SRCCOPY);
			return;
		case 23:
			StretchBlt(hdc, cur->MinX + p1, cur->MinY + t, 37, 23, MemDC, 347, 90, 70, 45, SRCCOPY);
			return;
		case 25:
			StretchBlt(hdc, cur->MinX + p1, cur->MinY + t, 37, 23, MemDC, 0, 135, 70, 45, SRCCOPY);
			return;
		case 26:
			StretchBlt(hdc, cur->MinX + p1, cur->MinY + t, 37, 23, MemDC, 70, 135, 70, 45, SRCCOPY);
			return;
		case 27:
			StretchBlt(hdc, cur->MinX + p1, cur->MinY + t, 37, 23, MemDC, 140, 135, 70, 45, SRCCOPY);
			return;
		case 29:
			StretchBlt(hdc, cur->MinX + p1, cur->MinY + t, 37, 23, MemDC, 205, 135, 70, 45, SRCCOPY);
			return;
		case 30:
			StretchBlt(hdc, cur->MinX + p1, cur->MinY + t, 37, 23, MemDC, 278, 135, 70, 45, SRCCOPY);
			return;
		case 31:
			StretchBlt(hdc, cur->MinX + p1, cur->MinY + t, 37, 23, MemDC, 347, 135, 70, 45, SRCCOPY);
			return;
		}
	}

	if (player == 2)
	{
		switch (cur->number)
		{
		case 1:
			StretchBlt(hdc, cur->MinX + p2, cur->MinY + t, 37, 23, MemDC, 0, 0, 70, 45, SRCCOPY);
			return;
		case 2:
			StretchBlt(hdc, cur->MinX + p2, cur->MinY + t, 37, 23, MemDC, 70, 0, 70, 45, SRCCOPY);
			return;
		case 3:
			StretchBlt(hdc, cur->MinX + p2, cur->MinY + t, 37, 23, MemDC, 140, 0, 70, 45, SRCCOPY);
			return;
		case 5:
			StretchBlt(hdc, cur->MinX + p2, cur->MinY + t, 37, 23, MemDC, 205, 0, 70, 45, SRCCOPY);
			return;
		case 6:
			StretchBlt(hdc, cur->MinX + p2, cur->MinY + t, 37, 23, MemDC, 278, 0, 70, 45, SRCCOPY);
			return;
		case 7:
			StretchBlt(hdc, cur->MinX + p2, cur->MinY + t, 37, 23, MemDC, 347, 0, 70, 45, SRCCOPY);
			return;
		case 9:
			StretchBlt(hdc, cur->MinX + p2, cur->MinY + t, 37, 23, MemDC, 0, 45, 70, 45, SRCCOPY);
			return;
		case 10:
			StretchBlt(hdc, cur->MinX + p2, cur->MinY + t, 37, 23, MemDC, 70, 45, 70, 45, SRCCOPY);
			return;
		case 11:
			StretchBlt(hdc, cur->MinX + p2, cur->MinY + t, 37, 23, MemDC, 140, 45, 70, 45, SRCCOPY);
			return;
		case 13:
			StretchBlt(hdc, cur->MinX + p2, cur->MinY + t, 37, 23, MemDC, 205, 45, 70, 45, SRCCOPY);
			return;
		case 14:
			StretchBlt(hdc, cur->MinX + p2, cur->MinY + t, 37, 23, MemDC, 278, 45, 70, 45, SRCCOPY);
			return;
		case 15:
			StretchBlt(hdc, cur->MinX + p2, cur->MinY + t, 37, 23, MemDC, 347, 45, 70, 45, SRCCOPY);
			return;
		case 17:
			StretchBlt(hdc, cur->MinX + p2, cur->MinY + t, 37, 23, MemDC, 0, 90, 70, 45, SRCCOPY);
			return;
		case 18:
			StretchBlt(hdc, cur->MinX + p2, cur->MinY + t, 37, 23, MemDC, 70, 90, 70, 45, SRCCOPY);
			return;
		case 19:
			StretchBlt(hdc, cur->MinX + p2, cur->MinY + t, 37, 23, MemDC, 140, 90, 70, 45, SRCCOPY);
			return;
		case 21:
			StretchBlt(hdc, cur->MinX + p2, cur->MinY + t, 37, 23, MemDC, 205, 90, 70, 45, SRCCOPY);
			return;
		case 22:
			StretchBlt(hdc, cur->MinX + p2, cur->MinY + t, 37, 23, MemDC, 278, 90, 70, 45, SRCCOPY);
			return;
		case 23:
			StretchBlt(hdc, cur->MinX + p2, cur->MinY + t, 37, 23, MemDC, 347, 90, 70, 45, SRCCOPY);
			return;
		case 25:
			StretchBlt(hdc, cur->MinX + p2, cur->MinY + t, 37, 23, MemDC, 0, 135, 70, 45, SRCCOPY);
			return;
		case 26:
			StretchBlt(hdc, cur->MinX + p2, cur->MinY + t, 37, 23, MemDC, 70, 135, 70, 45, SRCCOPY);
			return;
		case 27:
			StretchBlt(hdc, cur->MinX + p2, cur->MinY + t, 37, 23, MemDC, 140, 135, 70, 45, SRCCOPY);
			return;
		case 29:
			StretchBlt(hdc, cur->MinX + p2, cur->MinY + t, 37, 23, MemDC, 205, 135, 70, 45, SRCCOPY);
			return;
		case 30:
			StretchBlt(hdc, cur->MinX + p2, cur->MinY + t, 37, 23, MemDC, 278, 135, 70, 45, SRCCOPY);
			return;
		case 31:
			StretchBlt(hdc, cur->MinX + p2, cur->MinY + t, 37, 23, MemDC, 347, 135, 70, 45, SRCCOPY);
			return;
		}
	}
}

//player1의 빌딩 출력 (빨간색)
void player1_building(HDC hdc, HBITMAP MyBitmap, int x, int y)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	MemDC = CreateCompatibleDC(hdc);
	MyBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP23));
	OldBitmap = (HBITMAP)SelectObject(MemDC, MyBitmap);
	StretchBlt(hdc, x + 40, y + 15, 40, 55, MemDC, 0, 0, 90, 125, SRCAND);
	SelectObject(MemDC, OldBitmap);
	DeleteObject(MyBitmap);
	DeleteDC(MemDC);
}

//player2의 빌딩 출력 (파란색)
void player2_building(HDC hdc, HBITMAP MyBitmap, int x, int y)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	MemDC = CreateCompatibleDC(hdc);
	MyBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP22));
	OldBitmap = (HBITMAP)SelectObject(MemDC, MyBitmap);
	StretchBlt(hdc, x + 40, y + 15, 40, 55, MemDC, 0, 0, 90, 125, SRCAND);
	SelectObject(MemDC, OldBitmap);
	DeleteObject(MyBitmap);
	DeleteDC(MemDC);
}

//랜드마크 글자
void landmark_boom(HDC hdc, HBITMAP MyBitmap)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	MemDC = CreateCompatibleDC(hdc);
	MyBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP25));
	OldBitmap = (HBITMAP)SelectObject(MemDC, MyBitmap);
	StretchBlt(hdc, 100, 100, 1000, 200, MemDC, 0, 0, 1000, 200, SRCAND);
	SelectObject(MemDC, OldBitmap);
	DeleteObject(MyBitmap);
	DeleteDC(MemDC);
}

//추카추카
void congratulation(HDC hdc, HBITMAP MyBitmap)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	MemDC = CreateCompatibleDC(hdc);
	MyBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP26));
	OldBitmap = (HBITMAP)SelectObject(MemDC, MyBitmap);
	StretchBlt(hdc, 0, 0, 1200, 700, MemDC, 0, 0, 1200, 700, SRCCOPY);
	SelectObject(MemDC, OldBitmap);
	DeleteObject(MyBitmap);
	DeleteDC(MemDC);
}

//랭킹 정하는 함수
void ranking(struct character* cur)
{
	int sum1 = 0;
	int sum2 = 0;
	struct node* temp = head;
	while (temp != 0)
	{
		if (temp->owner == 1)
		{
			sum1 = sum1 + temp->building;
		}
		if (temp->owner == 2)
		{
			sum2 = sum2 + temp->building;
		}
		temp = temp->next;
	}
	if (sum1 > sum2)
	{
		cur->ranking = 1;
		cur->next->ranking = 2;
		return;
	}
	if (sum1 < sum2)
	{
		cur->ranking = 2;
		cur->next->ranking = 1;
		return;
	}
	else
	{
		if (cur->janmoney > cur->next->janmoney)
		{
			cur->ranking = 1;
			cur->next->ranking = 2;
			return;
		}
		if (cur->janmoney < cur->next->janmoney)
		{
			cur->ranking = 2;
			cur->next->ranking = 1;
			return;
		}
	}
	return;
}