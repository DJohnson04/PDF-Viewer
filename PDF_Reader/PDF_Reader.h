#pragma once

#include "targetver.h"
#include <vector>
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <commctrl.h>
#include <commdlg.h>
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "Comdlg32.lib")
#define MAX_LOADSTRING 100

//MACROS
//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ generated include file.
// Used by PDF_Reader.rc

#define IDS_APP_TITLE			103
#define IDM_OPEN_PDF 40001
#define IDR_MAINFRAME			128
#define IDD_PDFREADER_DIALOG	102
#define IDD_ABOUTBOX			103
#define IDM_ABOUT				104
#define IDM_EXIT				105
#define IDI_PDFREADER			107
#define IDI_SMALL				108
#define IDC_PDFREADER			109
#define IDC_MYICON				2
#ifndef IDC_STATIC
#define IDC_STATIC				-1
#define IDC_MAIN_TAB 101
#endif

// End preprocessor conditional block
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS

#define _APS_NO_MFC					130
#define _APS_NEXT_RESOURCE_VALUE	129
#define _APS_NEXT_COMMAND_VALUE		32771
#define _APS_NEXT_CONTROL_VALUE		1000
#define _APS_NEXT_SYMED_VALUE		110
#endif
#endif  // APSTUDIO_INVOKED
