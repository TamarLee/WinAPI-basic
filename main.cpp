#include <windows.h>

#define ID_BTN 1
#define ID_BUTTON 2
#define ID_TEXTBOX 3

static HWND hwndTextbox;

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		
		
		
		
		
		
		case WM_CREATE: {
			
			// create menu {
			
			HMENU hMenubar = CreateMenu();
			HMENU hFile = CreateMenu();
			HMENU hOptions = CreateMenu();
			
			
			AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hFile, "File");
			AppendMenu(hMenubar, MF_POPUP, NULL, "Edit");
			AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hOptions, "Options");
			
			AppendMenu(hFile, MF_STRING, NULL, "Exit");
			AppendMenu(hOptions, MF_STRING, NULL, "one");
			AppendMenu(hOptions, MF_STRING, NULL, "two");

			SetMenu(hwnd, hMenubar);
		
			// }
			
			
			hwndTextbox = CreateWindow(
				TEXT("edit"), 
				TEXT(""), 
				WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
				10, 10, 200, 20,
				hwnd, (HMENU) ID_TEXTBOX, NULL, NULL
			);
				
			CreateWindow(
				TEXT("button"),
				TEXT("Click Me"),
				WS_VISIBLE | WS_CHILD,
				10, 40, 80, 20,
				hwnd, (HMENU) ID_BTN, NULL, NULL
			);
			
			CreateWindow(
				TEXT("button"),
				TEXT("Or Click Me"),
				WS_VISIBLE | WS_CHILD,
				10, 70, 80, 20,
				hwnd, (HMENU) ID_BUTTON, NULL, NULL
			);
			
				
				
			break;
		}
		
		case WM_COMMAND: {
			
			// action for default button
			
			if(LOWORD(wParam) == ID_BTN){
				
				//create some default vars
				
				int len = GetWindowTextLength(hwndTextbox)+1;
				static char title[500];
				
				GetWindowText(hwndTextbox, title, len);
				
				
				SetWindowText(hwnd, title); // fanjaras ucvlis saxels
				
			}
			
			if(LOWORD(wParam) == ID_BUTTON){
				
				//create some default vars
				
				int len = GetWindowTextLength(hwndTextbox)+1;
				static char title[500];
				
				GetWindowText(hwndTextbox, title, len);
				
				MessageBox(hwnd, title, "Note", MB_OK); // mesij boxi gamoaqvs / MB_ICONINFORMATION - gamoitans dzaxilis nishans
				
				
			}
			
			
			break;
		}
		
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,
		"WindowClass",
		"Tamara's App",
		WS_VISIBLE|
		WS_SYSMENU,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		700, /* width */
		550, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	while(GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}
	return msg.wParam;
}
