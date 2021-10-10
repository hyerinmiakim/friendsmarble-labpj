#include <windows.h>
#include <time.h>//�ð����� random�ϵ���
#include<MMsystem.h>//�Ҹ�������
#include "resource.h"//��Ʈ���� �ҷ��´�.
#pragma comment(lib,"winmm.lib")//#pragma��ũ�δ� lib������ �о���̴µ��� ����
#define ID_EDIT1 200//edit 1
#define ID_EDIT2 201//edit 2
#define ID_EDIT3 202//edit 3
#define ID_EDIT4 203//edit 4

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);						// ���� ������
LRESULT CALLBACK WndProcNew(HWND, UINT, WPARAM, LPARAM);					// ���ʽ����� ������
LRESULT CALLBACK WndProc_Guide(HWND, UINT, WPARAM, LPARAM);					// ���Ӽ��� ������
LRESULT CALLBACK WorldTour(HWND, UINT, WPARAM, LPARAM);						// ���迩�� ������
LRESULT CALLBACK Godbless(HWND, UINT, WPARAM, LPARAM);						// �����ູ ������

HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Project");
HWND hEdit1, hEdit2, hEdit3, hEdit4;	// edit ��ȯ
TCHAR str1[128], str2[128];				// edit���� �Է¹��� ���� str�� ����
TCHAR nara[128];
HWND hWndNew;							// ���ʽ� ���� ������â
HWND hWnd_Guide;						//���̵� â
HWND hsin, hworld;						//�����ູ,���迩��
int cheekfirstmap = 1;
int PlayMode = 0;						// ���ۉ���� ����
int button0 = 0, button1 = 0;			// ��ư���ȴ��� ����
int dicebutton = 0;						//�ֻ�����ư ���ȴ��� ����.
int dicebutton2 = 0;					//�ֻ�����ư �������� ����.
int dice;								//�ֻ����� ���� ����
int Player = 0;							//�����Ƹ鼭 �ϱ����� ����
int game = 0;							//�������� ����
int RSP0 = 0, RSP1 = 0, RSP2 = 0;		//����� ����
int start = 0;
int RSPwin = 0;
int ���ε�Player1 = 0, ���ε�cnt1 = 0, ���ε�Player2 = 0, ���ε�cnt2 = 0;

//���� ���� ����ü
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

//ĳ���� ���� ����ü
struct character
{
	TCHAR* name;
	int janmoney;
	int ranking;
	int landmark;
	struct character* next;
};
struct character* chead = 0;
struct character* play2; // player1 ����
struct character* play1; // player2 ����

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

	//���ʽ����� �� ������
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpfnWndProc = (WNDPROC)WndProcNew;
	WndClass.lpszClassName = TEXT("new");
	RegisterClass(&WndClass);

	// Guideâ class
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpfnWndProc = (WNDPROC)WndProc_Guide;
	WndClass.lpszClassName = TEXT("Guide");
	RegisterClass(&WndClass);

	// ���� �ູ
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpfnWndProc = (WNDPROC)Godbless;
	WndClass.lpszClassName = TEXT("sin");
	RegisterClass(&WndClass);

	// ���� ����
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpfnWndProc = (WNDPROC)WorldTour;
	WndClass.lpszClassName = TEXT("world");
	RegisterClass(&WndClass);

	//������ �����Լ�
	information();

	//���� ��ȯ
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

//���� �ູ ������
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
		SetWindowText(hEdit3, TEXT("���� �����ϼ���")); // �����̸� �Է¹ޱ�
		CreateWindow(TEXT("button"), TEXT("Ȯ��"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			250, 340, 70, 30, hWnd, (HMENU)1, g_hInst, NULL);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 1:
			GetWindowText(hEdit3, nara, 128); // �����̸��� nara�� ����
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
				if (temp == 0) // �Է��� �̸��� ���� ã�� ������ ��
				{
					MessageBox(hWnd, TEXT("��ġ�ϴ� ���� �����ϴ�."), TEXT("����� ����"), MB_OK);
					break;
				}
				else if (temp->building == 0) // �ǹ��� ���� ��
				{
					// �ٽ��Է¹���
					MessageBox(hWnd, TEXT("��ġ�� �ǹ��� �����ϴ�."), TEXT("����� ����"), MB_OK);
					break;
				}
				else // �ǹ��� ���� ��
				{
					// �ٽ� �Է¹���
					PlaySound(MAKEINTRESOURCE(IDR_WAVE22), g_hInst, SND_RESOURCE | SND_ASYNC);
					MessageBox(hWnd, TEXT("����� ����!!"), TEXT("����� ����"), MB_OK);
					temp->money = temp->money * 2; // ����� �ι�
					Sleep(500);
					DestroyWindow(hsin); // �˾� â ����
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

//���� ���� ������
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
		SetWindowText(hEdit4, TEXT("���� �����ϼ���.")); // �����̸� �Է¹ޱ�
		CreateWindow(TEXT("button"), TEXT("Ȯ��"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			250, 340, 70, 30, hworld, (HMENU)1, g_hInst, NULL);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 1:
			GetWindowText(hEdit4, nara, 128); // �����̸��� nara�� ����
			while (1)
			{
				temp = head;
				while (lstrcmp(temp->city, nara) != 0)
				{
					temp = temp->next;
					if (temp == 0)
					{
						MessageBox(hworld, TEXT("��ġ�ϴ� ���� �����ϴ�."), TEXT("����� ����"), MB_OK);
						break;
					}
				}
				if (temp == 0) // �Է��� �̸��� ���� ã�� ������ ��
				{
					break;
				}
				else // �ǹ��� ���� ��
				{
					if (Player % 2 == 0)
					{
						cur2 = temp;
					}
					else
					{
						cur = temp;
					}
					DestroyWindow(hworld); // �˾� â ����
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

// ���Ӽ��� â�� ����� �� ���� �ڵ�
LRESULT CALLBACK WndProc_Guide(HWND hWnd_Guide, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc_Guide;
	PAINTSTRUCT ps;
	static HBITMAP Bitmap;
	static int i = 0;

	switch (iMessage) {
		// ��ư â ����
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

//���ʽ� ���� �� �� â
LRESULT CALLBACK WndProcNew(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	int x, y;
	PAINTSTRUCT ps;
	static HBITMAP Bitmap;  //��Ʈ�� ����� ���� ����
	int ��������������;
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
		//��
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
		//��
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
		//��
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

				DrawRSP(hdc, Bitmap, (TCHAR)IDB_BITMAP15, 50, 270, 368, 300);//��
				DrawRSP(hdc, Bitmap, (TCHAR)IDB_BITMAP16, 153, 270, 340, 300);//��
				DrawRSP(hdc, Bitmap, (TCHAR)IDB_BITMAP17, 250, 270, 358, 300);//��
			}
			�������������� = rand() % 3;
			DrawRSP(hdc, Bitmap, (TCHAR)IDB_BITMAP15 + ��������������, 153, 120, 368, 300);
			//�¸�����
			if (RSP0 == 2)
			{
				RSP0 = 0;
				if (�������������� == 0)
				{
					PlaySound(MAKEINTRESOURCE(IDR_WAVE14), g_hInst, SND_RESOURCE | SND_ASYNC); Sleep(40);
					MessageBox(hWnd, TEXT("���"), TEXT("����! ����! ��!"), MB_OK);
				}
				else if (�������������� == 1)
				{
					PlaySound(MAKEINTRESOURCE(IDR_WAVE13), g_hInst, SND_RESOURCE | SND_ASYNC); Sleep(40);
					MessageBox(hWnd, TEXT("�̱�"), TEXT("����! ����! ��!"), MB_OK);
					if (Player % 2 == 0)
						play2->janmoney += 300000;
					else
						play1->janmoney += 300000;
				}
				else
				{
					PlaySound(MAKEINTRESOURCE(IDR_WAVE14), g_hInst, SND_RESOURCE | SND_ASYNC); Sleep(40);
					MessageBox(hWnd, TEXT("��"), TEXT("����! ����! ��!"), MB_OK);
				}
				Sleep(500);
				DestroyWindow(hWndNew);
			}
			else if (RSP1 == 2)
			{
				RSP1 = 0;
				if (�������������� == 0)
				{
					PlaySound(MAKEINTRESOURCE(IDR_WAVE14), g_hInst, SND_RESOURCE | SND_ASYNC); Sleep(40);
					MessageBox(hWnd, TEXT("��"), TEXT("����! ����! ��!"), MB_OK);
				}
				else if (�������������� == 1)
				{
					PlaySound(MAKEINTRESOURCE(IDR_WAVE14), g_hInst, SND_RESOURCE | SND_ASYNC); Sleep(40);
					MessageBox(hWnd, TEXT("���"), TEXT("����! ����! ��!"), MB_OK);
				}
				else
				{
					PlaySound(MAKEINTRESOURCE(IDR_WAVE13), g_hInst, SND_RESOURCE | SND_ASYNC); Sleep(40);
					MessageBox(hWnd, TEXT("�̱�"), TEXT("����! ����! ��!"), MB_OK);
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
				if (�������������� == 0)
				{
					PlaySound(MAKEINTRESOURCE(IDR_WAVE13), g_hInst, SND_RESOURCE | SND_ASYNC); Sleep(40);
					MessageBox(hWnd, TEXT("�̱�"), TEXT("����! ����! ��!"), MB_OK);
					if (Player % 2 == 0)
						play2->janmoney += 300000;
					else
						play1->janmoney += 300000;
				}
				else if (�������������� == 1)
				{
					PlaySound(MAKEINTRESOURCE(IDR_WAVE14), g_hInst, SND_RESOURCE | SND_ASYNC); Sleep(40);
					MessageBox(hWnd, TEXT("��"), TEXT("����! ����! ��!"), MB_OK);
				}
				else
				{
					PlaySound(MAKEINTRESOURCE(IDR_WAVE14), g_hInst, SND_RESOURCE | SND_ASYNC); Sleep(40);
					MessageBox(hWnd, TEXT("���"), TEXT("����! ����! ��!"), MB_OK);
				}
				Sleep(500);
				DestroyWindow(hWndNew);
			}

		}
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

// �� �� �� â
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	int x, y;
	static struct node* ���ε�place;//���ε�����
	static struct node* ���ε�place2;//���ε�����
	static HBITMAP Bitmap;  //��Ʈ�� ����� ���� ����
	PAINTSTRUCT ps;  //�׸��� ����� �� �ʿ�
	BOOL bShow = TRUE;  // editâ�� �Ⱥ��̰��ϱ����� ����

	switch (iMessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CREATE:
		// Editâ ����
		hEdit1 = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE |
			ES_AUTOHSCROLL, 245, 240, 200, 20, hWnd, (HMENU)ID_EDIT1, g_hInst, NULL);
		SetWindowText(hEdit1, TEXT("�г����� �Է��Ͻÿ�")); // player1 �г��� �Է¹ޱ�
		hEdit2 = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE |
			ES_AUTOHSCROLL, 770, 240, 200, 20, hWnd, (HMENU)ID_EDIT2, g_hInst, NULL);
		SetWindowText(hEdit2, TEXT("�г����� �Է��Ͻÿ�")); // player2 �г��� �Է¹ޱ�
		return 0;
		//���ʹ�ư ���� ��
	case WM_LBUTTONDOWN:
		hdc = GetDC(hWnd);
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		if (PlayMode == 0)
		{
			if (x >= 475 && x <= 715 && y >= 300 && y <= 345) //'���� ����' ��ư�� ���콺 ��ǥ
			{
				PlaySound(MAKEINTRESOURCE(IDR_WAVE2), g_hInst, SND_RESOURCE | SND_ASYNC);
				button0++;
				cur = head;
				cur2 = head;
			}
			else if (x >= 475 && x <= 715 && y >= 370 && y <= 417) //'���� ����' ��ư�� ���콺 ��ǥ
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
		//���ʹ�ư �� ��
	case WM_LBUTTONUP:
		hdc = GetDC(hWnd);
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		// Mainȭ���϶�
		if (PlayMode == 0)
		{
			if (x >= 475 && x <= 715 && y >= 300 && y <= 345)//���� ����
			{
				Sleep(20);
				PlayMode++;
				// Editâ �����
				if (bShow)
				{
					bShow = FALSE;
					ShowWindow(hEdit1, SW_HIDE);
					ShowWindow(hEdit2, SW_HIDE);
				}
				GetWindowText(hEdit2, str1, 128);		// �г����� str1�� ����.
				addToChar(str1, 1000000, 0);			// player1 ĳ���� node�� �����.
				GetWindowText(hEdit1, str2, 128);		// �г����� str2�� ����.
				addToChar(str2, 1000000, 0);			// player2 ĳ���� node�� �����
			}
			else if (x >= 475 && x <= 715 && y >= 370 && y <= 417)//���� ����
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
		//PlayMode1�ϴ�
		else
		{

			if (x >= 880 && x <= 965 && y >= 475 && y <= 530)
			{
				dicebutton2++;
				if (dicebutton != 0)
					dicebutton--;
				game++;
				if (���ε�Player1 == 1)
				{
					���ε�place = cur; //���ε��� ��ġ�� ����
				}
				if (���ε�Player2 == 1)
				{
					���ε�place2 = cur2; //���ε��� ��ġ�� ����
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
				if (���ε�Player1 == 1)
				{
					if (Player % 2 == 0)
					{
						if (���ε�cnt1 != 2)
						{
							cur = ���ε�place;
							���ε�cnt1++;
						}
						else
						{
							���ε�place = head;
							���ε�cnt1 = 0;
							���ε�Player1 = 0;
						}
					}
				}
				if (���ε�Player2 == 1)
				{
					if (Player % 2 == 1)
					{
						if (���ε�cnt2 != 2)
						{
							cur2 = ���ε�place2;
							���ε�cnt2++;
						}
						else
						{
							���ε�place2 = head;
							���ε�cnt2 = 0;
							���ε�Player2 = 0;
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
		//�׸� ��
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (PlayMode == 0)
		{
			//main�׸�
			DrawPicture(hdc, Bitmap, (TCHAR)IDB_BITMAP1, 0, 0, 1200, 700);
			if (button0 % 2 == 1)
				DrawPicture(hdc, Bitmap, (TCHAR)IDB_BITMAP3, 473, 295, 250, 53); // ���ӽ��� �� ��ư ���

			else if (button1 % 2 == 1)
				DrawPicture(hdc, Bitmap, (TCHAR)IDB_BITMAP4, 475, 370, 250, 52); // ���ӹ�� �� ��ư ���
		}
		else
		{
			if (cheekfirstmap)
			{
				PlaySound(MAKEINTRESOURCE(IDR_WAVE4), g_hInst, SND_RESOURCE | SND_ASYNC);
				Sleep(800);
				cheekfirstmap = 0;
			}
			//game�׸�
			GamingBackGround(hdc, Bitmap);
			DrawPicture(hdc, Bitmap, (TCHAR)IDB_BITMAP6, 900, 90, 170, 170);
			DrawPicture(hdc, Bitmap, (TCHAR)IDB_BITMAP8, 70, 450, 170, 170);
			Player1(hdc, Bitmap, cur->MinX + 10, cur->MinY - 15);
			Player2(hdc, Bitmap, cur2->MinX + 60, cur2->MinY - 15);
			play1 = chead;
			play2 = chead->next;
			ranking(chead);
			CharacterTextOut(hdc, 385, 463, play1); // player1 ���� ĳ����â ����
			CharacterTextOut(hdc, 795, 98, play2); // player2 ���� ĳ����â ����
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
			if (dicebutton % 2 == 1) // ��ư�� ������ ���� ��
			{
				DrawPicture(hdc, Bitmap, TCHAR(IDB_BITMAP13), 870, 470, 120, 80);
				PlaySound(MAKEINTRESOURCE(IDR_WAVE2), g_hInst, SND_RESOURCE | SND_ASYNC);
			}
			else if (dicebutton % 2 == 0) // ��ư�� �����ٰ� ���� ��
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
				//player 1�� ������ ��
				if (Player % 2 == 1)
				{
					if (cur->money == 0)
					{
						//���, ����ī��, ���ε�, ���� �ູ �� ��
						if (cur->number == 0)
						{
							//�����������
							play1->janmoney = play1->janmoney + 100000;//����10����
							return 0;
						}
						if (cur->number == 4 || cur->number == 28)
						{
							//���ʽ� ����
							hWndNew = CreateWindow(TEXT("new"), TEXT("Bonus Game"), WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU,
								100, 100, 400, 400, hWnd, (HMENU)0, g_hInst, NULL);
							ShowWindow(hWndNew, SW_SHOW);

							return 0;
						}
						if (cur->number == 8 || cur->number == 24)
						{
							//���� �ູ
							PlaySound(MAKEINTRESOURCE(IDR_WAVE20), g_hInst, SND_RESOURCE | SND_ASYNC);
							hsin = CreateWindow(TEXT("sin"), TEXT("�����ູ"), WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU,
								100, 100, 420, 450, hWnd, (HMENU)0, g_hInst, NULL);
							ShowWindow(hsin, SW_SHOW);
							return 0;
						}
						if (cur->number == 12)
						{
							//�����������
							cur = head;
							play1->janmoney = play1->janmoney + 100000;//����10����
							return 0;
						}
						if (cur->number == 16)
						{
							//���� ����
							PlaySound(MAKEINTRESOURCE(IDR_WAVE20), g_hInst, SND_RESOURCE | SND_ASYNC);
							hworld = CreateWindow(TEXT("world"), TEXT("���迩��"), WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU,
								100, 100, 420, 450, hWnd, (HMENU)0, g_hInst, NULL);
							ShowWindow(hworld, SW_SHOW);
							return 0;
						}
						if (cur->number == 20)
						{
							���ε�Player1 = 1;
							if (���ε�cnt1 == 0)
							{
								PlaySound(MAKEINTRESOURCE(IDR_WAVE21), g_hInst, SND_RESOURCE | SND_ASYNC);
								MessageBox(hWnd, TEXT("�����Դϴ�. 3�� ������."), TEXT("����� ����"), MB_OK);
							}
							if (���ε�cnt1 == 1)
								MessageBox(hWnd, TEXT("2�� ���Ҿ��."), TEXT("����� ����"), MB_OK);
							if (���ε�cnt1 == 2)
								MessageBox(hWnd, TEXT("1�� ���Ҿ��."), TEXT("����� ����"), MB_OK);
							//����
							//3�Ͻ��ÿ� �޽���
							return 0;
						}
						return 0;
					}

					if (cur->owner == 0 || cur->owner == 1) //�� ���̰ų� �� ���� ���
					{
						switch (cur->building)
						{
						case 0: //�� ��
							if (MessageBox(hWnd, TEXT("�ǹ��� �Ǽ��Ͻðڽ��ϱ�?"), TEXT("����� ����"), MB_OKCANCEL) == IDOK)
							{
								if (play1->janmoney - cur->money < 0)
								{
									if (MessageBox(hWnd, TEXT("�ܾ��� �����մϴ�."), TEXT("����� ����"), MB_OK) == IDOK)
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
						case 1: //�ǹ��� ������ �ִ� ���
							if (MessageBox(hWnd, TEXT("���帶ũ�� �Ǽ��Ͻðڽ��ϱ�?"), TEXT("����� ����"), MB_OKCANCEL) == IDOK)
							{
								if (play1->janmoney - cur->money < 0)
								{
									if (MessageBox(hWnd, TEXT("�ܾ��� �����մϴ�."), TEXT("����� ����"), MB_OK) == IDOK)
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
								if (play1->landmark == 10)                //�¸� ���� �´��� Ȯ��
								{
									congratulation(hdc, Bitmap);
									PlaySound(MAKEINTRESOURCE(IDR_WAVE23), g_hInst, SND_RESOURCE | SND_ASYNC);
									MessageBox(hWnd, TEXT("Player2 ��"), TEXT("����� ����"), MB_OK);
									Sleep(3000);
									PostQuitMessage(0);
								}
							}
							return 0;
						case 2: //�� ���� �̹� ���帶ũ�� ������ �ִ� ���
							return 0;
						}
						return 0;
					}

					else if (cur->owner == 2) //�� ���� ���� ���̾��� ���
					{
						if (play1->janmoney - cur->money < 0) //����� �� ���� ��� �Ļ�
						{
							if (MessageBox(hWnd, TEXT("�ܾ��� �����մϴ�."), TEXT("����� ����"), MB_OK) == IDOK)
							{
								congratulation(hdc, Bitmap);
								PlaySound(MAKEINTRESOURCE(IDR_WAVE23), g_hInst, SND_RESOURCE | SND_ASYNC);
								MessageBox(hWnd, TEXT("Player1 ��"), TEXT("����� ����"), MB_OK);
								Sleep(3000);
								PostQuitMessage(0);
							}
						}
						//����� ����
						play1->janmoney = play1->janmoney - cur->money;
						play2->janmoney = play2->janmoney + cur->money;
						switch (cur->building)
						{
						case 1: //�ǹ��� ������ ���� == �μ��ϰ� ���帶ũ�� ���� �� ����
							if (MessageBox(hWnd, TEXT("�ǹ��� �μ��Ͻðڽ��ϱ�?"), TEXT("����� ����"), MB_OKCANCEL) == IDOK)
							{
								if (play1->janmoney - cur->money < 0)
								{
									if (MessageBox(hWnd, TEXT("�ܾ��� �����մϴ�."), TEXT("����� ����"), MB_OK) == IDOK)
									{
										return 0;
									}
								}
								PlaySound(MAKEINTRESOURCE(IDR_WAVE16), g_hInst, SND_RESOURCE | SND_ASYNC);
								cur->owner = 1;
								player1_building(hdc, Bitmap, cur->MinX, cur->MinY);
								play1->janmoney = play1->janmoney - cur->money; //�μ� ��� �ΰ�
								play2->janmoney = play2->janmoney + cur->money;
							}
							return 0;
						case 2: //�̹� ���帶ũ�� ������ �����Ƿ� ����� �ΰ�
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
						//����ī��, ���ε�, ���� �ູ �� ��
						if (cur2->number == 0)
						{
							//�����������
							play2->janmoney = play2->janmoney + 100000;//����10����
							return 0;
						}
						if (cur2->number == 4 || cur2->number == 28)
						{
							//���ʽ� ����
							hWndNew = CreateWindow(TEXT("new"), TEXT("Bonus Game"), WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU,
								100, 100, 400, 400, hWnd, (HMENU)0, g_hInst, NULL);
							ShowWindow(hWndNew, SW_SHOW);

							return 0;
						}
						if (cur2->number == 8 || cur2->number == 24)
						{
							//���� �ູ
							PlaySound(MAKEINTRESOURCE(IDR_WAVE20), g_hInst, SND_RESOURCE | SND_ASYNC);
							hsin = CreateWindow(TEXT("sin"), TEXT("�����ູ"), WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU,
								100, 100, 420, 450, hWnd, (HMENU)0, g_hInst, NULL);
							ShowWindow(hsin, SW_SHOW);
							return 0;
						}
						if (cur2->number == 12)
						{
							//�����������
							cur2 = head;
							play2->janmoney = play2->janmoney + 100000;//����10����
							return 0;
						}
						if (cur2->number == 16)
						{
							//���� ����
							PlaySound(MAKEINTRESOURCE(IDR_WAVE20), g_hInst, SND_RESOURCE | SND_ASYNC);
							hworld = CreateWindow(TEXT("world"), TEXT("���迩��"), WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU,
								100, 100, 420, 450, hWnd, (HMENU)0, g_hInst, NULL);
							ShowWindow(hworld, SW_SHOW);
							return 0;
						}
						if (cur2->number == 20)
						{
							���ε�Player2 = 1;
							if (���ε�cnt2 == 0)
							{
								MessageBox(hWnd, TEXT("�����Դϴ�. 3�� ������."), TEXT("����� ����"), MB_OK);
								PlaySound(MAKEINTRESOURCE(IDR_WAVE21), g_hInst, SND_RESOURCE | SND_ASYNC);
							}
							if (���ε�cnt2 == 1)
								MessageBox(hWnd, TEXT("2�� ���Ҿ��"), TEXT("����� ����"), MB_OK);
							if (���ε�cnt2 == 2)
								MessageBox(hWnd, TEXT("1�� ���Ҿ��"), TEXT("����� ����"), MB_OK);
							//����
							return 0;
						}
						return 0;
					}

					if (cur2->owner == 0 || cur2->owner == 2) //�� ���̰ų� �� ���� ���
					{
						switch (cur2->building)
						{
						case 0: //�� ��
							if (MessageBox(hWnd, TEXT("�ǹ��� �Ǽ��Ͻðڽ��ϱ�?"), TEXT("����� ����"), MB_OKCANCEL) == IDOK)
							{
								//ĳ���� �ܾ� �ٲ��ֱ�
								if (play2->janmoney - cur2->money < 0)
								{
									if (MessageBox(hWnd, TEXT("�ܾ��� �����մϴ�."), TEXT("����� ����"), MB_OK) == IDOK)
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
						case 1: //�� ���� �ǹ��� ������ �ִ� ���
							if (MessageBox(hWnd, TEXT("���帶ũ�� �Ǽ��Ͻðڽ��ϱ�?"), TEXT("����� ����"), MB_OKCANCEL) == IDOK)
							{
								//ĳ���� �ܾ� �ٲ��ֱ�
								if (play2->janmoney - cur2->money < 0)
								{
									if (MessageBox(hWnd, TEXT("�ܾ��� �����մϴ�."), TEXT("����� ����"), MB_OK) == IDOK)
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
								if (play2->landmark == 10)                        //�¸� ���� �´��� Ȯ��
								{
									congratulation(hdc, Bitmap);
									PlaySound(MAKEINTRESOURCE(IDR_WAVE23), g_hInst, SND_RESOURCE | SND_ASYNC);
									MessageBox(hWnd, TEXT("Player1 ��"), TEXT("����� ����"), MB_OK);
									Sleep(3000);
									PostQuitMessage(0);
								}
							}
							return 0;
						case 2: //�� ���� �̹� ���帶ũ�� ������ �ִ� ���
							return 0;
						}
						return 0;
					}

					else if (cur2->owner == 1) //���� ���� �� ���
					{
						if (play2->janmoney - cur2->money < 0) //����� �� ���� ��� �Ļ�
						{
							if (MessageBox(hWnd, TEXT("�ܾ��� �����մϴ�."), TEXT("����� ����"), MB_OK) == IDOK)
							{
								congratulation(hdc, Bitmap);
								PlaySound(MAKEINTRESOURCE(IDR_WAVE23), g_hInst, SND_RESOURCE | SND_ASYNC);
								MessageBox(hWnd, TEXT("Player2 ��"), TEXT("����� ����"), MB_OK);
								Sleep(3000);
								PostQuitMessage(0);
							}
						}
						//����� �ΰ�
						play2->janmoney = play2->janmoney - cur2->money;
						play1->janmoney = play1->janmoney + cur2->money;
						switch (cur2->building)
						{
						case 1: //�ǹ��� ������ �־ �μ�����
							if (MessageBox(hWnd, TEXT("�ǹ��� �μ��Ͻðڽ��ϱ�?"), TEXT("����� ����"), MB_OKCANCEL) == IDOK)
							{
								if (play2->janmoney - cur2->money < 0)
								{
									if (MessageBox(hWnd, TEXT("�ܾ��� �����մϴ�."), TEXT("����� ����"), MB_OK) == IDOK)
									{
										return 0;
									}
								}
								PlaySound(MAKEINTRESOURCE(IDR_WAVE16), g_hInst, SND_RESOURCE | SND_ASYNC);
								cur2->owner = 2;
								player2_building(hdc, Bitmap, cur2->MinX, cur2->MinY);
								play2->janmoney = play2->janmoney - cur2->money; //�μ� ��� �ΰ�
								play1->janmoney = play1->janmoney + cur2->money;
							}
							return 0;
						case 2: //�̹� ���帶ũ�� ������ �����Ƿ� ����Ḹ �ΰ�
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

//�׸�����
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


// �� �� ��
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

//gamingȭ��
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

//���� ���� ����ü �߰����
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

//ĳ���� ���� ����ü �߰����
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

//���� ���� �߰�
void information()
{
	addToCity(TEXT("START"), 0, 1145, 640, 1020, 570, 0);			//���
	addToCity(TEXT("�׸���"), 10000, 1020, 640, 900, 570, 1);		//1
	addToCity(TEXT("�״�����"), 15000, 900, 640, 780, 570, 2);		//2
	addToCity(TEXT("����ũ"), 20000, 780, 640, 660, 570, 3);			//3
	addToCity(TEXT("Bonus Game"), 0, 660, 640, 535, 570, 4);		//����
	addToCity(TEXT("����"), 25000, 660, 570, 535, 500, 5);			//4
	addToCity(TEXT("�縶�Ͼ�"), 30000, 660, 500, 535, 430, 6);		//5
	addToCity(TEXT("�߽���"), 35000, 660, 436, 535, 364, 7);			//6
	addToCity(TEXT("God Bless"), 0, 660, 370, 535, 294, 8);			//���ε�
	addToCity(TEXT("�����ƴϾ�"), 40000, 660, 300, 535, 224, 9);		//7
	addToCity(TEXT("������"), 45000, 660, 230, 535, 154, 10);		//8
	addToCity(TEXT("���⿡"), 50000, 660, 160, 535, 84, 11);			//9
	addToCity(TEXT("�����������~!"), 0, 660, 90, 535, 14, 12);		//��?
	addToCity(TEXT("�Ұ�����"), 55000, 535, 90, 415, 14, 13);		//10
	addToCity(TEXT("������"), 60000, 415, 90, 290, 14, 14);			//11
	addToCity(TEXT("ĥ��"), 65000, 290, 90, 170, 14, 15);			//12
	addToCity(TEXT("World Tour"), 0, 170, 90, 45, 14, 16);			//����
	addToCity(TEXT("������"), 70000, 170, 160, 45, 84, 17);			//13
	addToCity(TEXT("������"), 75000, 170, 230, 45, 154, 18);			//14
	addToCity(TEXT("����"), 80000, 170, 300, 45, 224, 19);			//15
	addToCity(TEXT("Prison"), 0, 170, 370, 45, 300, 20);			//����
	addToCity(TEXT("����Ʈ����"), 85000, 290, 370, 170, 294, 21);	//16
	addToCity(TEXT("��Ż����"), 90000, 415, 370, 290, 294, 22);		//17
	addToCity(TEXT("���"), 100000, 535, 370, 415, 294, 23);			//18
	addToCity(TEXT("God Bless"), 0, 660, 370, 535, 294, 24);		//���ε�
	addToCity(TEXT("ü��"), 105000, 780, 370, 660, 294, 25);			//19
	addToCity(TEXT("ũ�ξ�Ƽ��"), 110000, 900, 370, 780, 294, 26);	//20
	addToCity(TEXT("������"), 115000, 1020, 370, 900, 294, 27);		//21
	addToCity(TEXT("Bonus Game"), 0, 1145, 370, 1020, 294, 28);		//��
	addToCity(TEXT("������"), 120000, 1145, 436, 1020, 364, 29);		//22
	addToCity(TEXT("�ɶ���"), 125000, 1145, 500, 1020, 430, 30);		//23
	addToCity(TEXT("�밡��"), 130000, 1145, 570, 1020, 494, 31);		//24
}

//�ֻ���
int random()
{
	return rand() % 6 + 1;
}

//�ֻ��� ���̰�
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

//���� ���� ���
void text(HDC hdc, struct node* cur, struct node* cur2, int Player)
{
	HFONT font, oldfont;
	TCHAR str[128];

	font = CreateFont(30, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, TEXT("�������"));
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

//ĳ���� ���� ���
void CharacterTextOut(HDC hdc, int x, int y, struct character *cur)
{
	HFONT font, oldfont;
	TCHAR str[128];
	font = CreateFont(15, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, TEXT("����"));
	oldfont = (HFONT)SelectObject(hdc, font);
	SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, x, y, cur->name, lstrlen(cur->name)); // ĳ���� �̸� ���
	wsprintf(str, TEXT("%d"), cur->janmoney);
	TextOut(hdc, x - 10, y + 45, str, lstrlen(str));// ĳ���� money ���
	wsprintf(str, TEXT("%d"), cur->ranking);
	TextOut(hdc, x + 50, y + 95, str, lstrlen(str)); // ĳ���� ���� ���
	SelectObject(hdc, oldfont);
	DeleteObject(font);
}

//���帶ũ ���
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

//player1�� ���� ��� (������)
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

//player2�� ���� ��� (�Ķ���)
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

//���帶ũ ����
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

//��ī��ī
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

//��ŷ ���ϴ� �Լ�
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