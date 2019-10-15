#include <windows.h>
#include <Windowsx.h>
#include <strsafe.h>

using namespace std;
void SystemShutdown(UINT nSDType);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//--------------------------------------------------------------------------------------------------------------------------------
HINSTANCE hInstance;
INT_PTR CALLBACK windDialProc(HWND, UINT, WPARAM, LPARAM);
HWND hwnd;

HWND btnCancel;

HWND btnLogoff;
HWND btnShutdown;
HWND btnReboot;
//--------------------------------------------------------------------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)

{
	HINSTANCE hInstance = _hInstance;
	MSG msg;
	WNDCLASSEX wndclass;
	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = L"manager";
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wndclass))
	{
		MessageBox(hwnd, L"Не удалось зарегестрировать класс окна", L"Ошибка регистрации класса окна", MB_OK);
		return 0;
	}

	hwnd = CreateWindow(
		wndclass.lpszClassName,
		L"Выключалка-Ковырялка",
		WS_POPUP,
		(GetSystemMetrics(SM_CXSCREEN) / 2 - 320),
		(GetSystemMetrics(SM_CYSCREEN) / 2 - 240),
		640,
		480,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hwnd)
	{
		MessageBox(hwnd, L"Не удалось создать экземпляр окна", L"Ошибка создания экземпляра окна", MB_OK);
		return 0;
	}
	ShowWindow(hwnd, iCmdShow); 

	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_CREATE: {
		/*background form start*/
		HBITMAP hImage = (HBITMAP)LoadImage(NULL, L"turnOff.bmp", IMAGE_BITMAP, 640, 480, LR_LOADFROMFILE);
		HWND hImageView = CreateWindowEx(NULL, L"STATIC", NULL, SS_BITMAP | WS_VISIBLE | WS_CHILD, 0, 0, 640, 480, hwnd, NULL, GetModuleHandle(NULL), NULL);
		SendMessage(hImageView, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage);
		/*background form end*/


		btnCancel = CreateWindow(L"button", L"Отмена", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 500, 420, 120, 40, hwnd, NULL, hInstance, NULL);

		/*buttons start*/
		HBITMAP hImgLogoff = (HBITMAP)LoadImage(NULL, L"logoff.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
		btnLogoff = CreateWindow(L"button", L"Отмена", BS_BITMAP |WS_VISIBLE | WS_CHILD, 70, 200, 100, 100, hwnd, NULL, hInstance, NULL);
		SendMessage(btnLogoff, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgLogoff);

		HBITMAP hImgShutdown = (HBITMAP)LoadImage(NULL, L"Shutdown.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
		btnShutdown = CreateWindow(L"button", L"Отмена", BS_BITMAP | WS_VISIBLE | WS_CHILD, 270, 200, 100, 100, hwnd, NULL, hInstance, NULL);
		SendMessage(btnShutdown, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgShutdown);


		HBITMAP hImgReboot = (HBITMAP)LoadImage(NULL, L"reboot.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
		btnReboot = CreateWindow(L"button", L"Отмена", BS_BITMAP | WS_VISIBLE | WS_CHILD, 470, 200, 100, 100, hwnd, NULL, hInstance, NULL);
		SendMessage(btnReboot, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgReboot);

		/*buttons end*/


		break;
	}
	case WM_COMMAND:
	{
		if (HIWORD(wParam) == BN_CLICKED) {
			if (btnCancel == (HWND)lParam) {
				exit(EXIT_SUCCESS);
			}
			if (btnLogoff == (HWND)lParam) {
				SystemShutdown(0);
			}
			if (btnShutdown == (HWND)lParam) {
				SystemShutdown(1);
			}
			if (btnReboot == (HWND)lParam) {
				SystemShutdown(2);
			}
		}
		break;
	}
	case WM_KEYDOWN: {
		if (wParam == VK_ESCAPE) {
			exit(EXIT_SUCCESS);
		}
		break;
	}
	case WM_DESTROY: {
		PostQuitMessage(0);
		break;
	}

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
void SystemShutdown(UINT nSDType)
{
	//0 - завершить сеанс
	//1 - выключить комп
	//2 - перезагрузка
	HANDLE           hToken;
	TOKEN_PRIVILEGES tkp;

	::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	::LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);

	tkp.PrivilegeCount = 1; // set 1 privilege
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// get the shutdown privilege for this process
	::AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

	switch (nSDType)
	{
		case 0: ::ExitWindowsEx(EWX_LOGOFF | EWX_FORCE, 0); break;
		case 1: ::ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, 0); break;
		case 2: ::ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0); break;
	}
}