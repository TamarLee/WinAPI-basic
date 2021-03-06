#include <windows.h>

#define ID_BTN 1
#define ID_BUTTON 2
#define ID_TEXTBOX 3
#define ID_FILE_NEW 4
#define ID_FILE_OPEN 5
#define ID_FILE_EXIT 6
#define ID_TREEVIEW 7

static HWND hwndTextbox;

HMENU hMenu;

void AddMenus(HWND);
HWND CreateATreeView(HWND);
void InitCommonControls();

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		
		
		case WM_CREATE: {
			
			// create menu {
			CreateATreeView(hwnd);
			AddMenus(hwnd);
		
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
			
			
			switch(wParam) {
				case 1: {
					MessageBeep(MB_OK);
					break;
				}
				
				case ID_FILE_EXIT: {
					DestroyWindow(hwnd);
					break;
				}
			}
			
			
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
		"My App",
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


void AddMenus(HWND hwnd) {
	
	hMenu = CreateMenu();
	HMENU hFile = CreateMenu();
	HMENU hSub = CreateMenu();
	
	AppendMenu(hSub, MF_STRING, NULL, "One");
	
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFile, "File");
	AppendMenu(hMenu, MF_STRING, NULL, "Edit");
	
	AppendMenu(hFile, MF_STRING, ID_FILE_NEW, "New");
	AppendMenu(hFile, MF_POPUP, (UINT_PTR)hSub, "Open");
	AppendMenu(hFile, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFile, MF_STRING, ID_FILE_EXIT, "Exit");
		
	SetMenu(hwnd, hMenu);
	
}

// Create a tree-view control. 
// Returns the handle to the new control if successful,
// or NULL otherwise. 
// hwndParent - handle to the control's parent window. 
// lpszFileName - name of the file to parse for tree-view items.
// g_hInst - the global instance handle.
// ID_TREEVIEW - the resource ID of the control.

HWND CreateATreeView(HWND hwndParent)
{ 
    RECT rcClient;  // dimensions of client area 
    HWND hwndTV;    // handle to tree-view control 

    // Ensure that the common control DLL is loaded. 
    InitCommonControls(); 

    // Get the dimensions of the parent window's client area, and create 
    // the tree-view control. 
    GetClientRect(hwndParent, &amp, rcClient); 
    hwndTV = CreateWindowEx(0,
                            WC_TREEVIEW,
                            TEXT("Tree View"),
                            WS_VISIBLE | WS_CHILD | WS_BORDER, 
                            0, 
                            0, 
                            rcClient.right, 
                            rcClient.bottom,
                            hwndParent, 
                            (HMENU)ID_TREEVIEW, 
                            g_hInst, 
                            NULL); 

    // Initialize the image list, and add items to the control. 
    // InitTreeViewImageLists and InitTreeViewItems are application- 
    // defined functions, shown later. 
    if (!InitTreeViewImageLists(hwndTV) || 
                !InitTreeViewItems(hwndTV))
    { 
        DestroyWindow(hwndTV); 
        return FALSE; 
    } 
    return hwndTV;
}
void InitCommonControls(

);

