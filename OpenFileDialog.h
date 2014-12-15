#pragma once
#include <windows.h>
#include "GlobalDefines.h"


class COpenFileDialog
{
public:
    COpenFileDialog(void);

    BOOL ShowDialog(HWND _hwndParent);
    TCHAR *GetFileName();

    ~COpenFileDialog(void);

private:
    TCHAR *mDefaultExtension;
    TCHAR *mFileName;
    TCHAR *mFilter;
    INT mFilterIndex;
    INT mFlags;
    TCHAR *mInitialDir;
    HWND mOwner;
    TCHAR *mTitle;  
};
