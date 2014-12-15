#include "stdafx.h"
#include "OpenFileDialog.h"


COpenFileDialog::COpenFileDialog(void)
{
    this->mDefaultExtension = 0;
    this->mFileName = new TCHAR[MAX_PATH];
    this->mFilter = 0;
    this->mFilterIndex = 0;
    this->mFlags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    this->mInitialDir = 0;
    this->mOwner = NULL;
    this->mTitle = 0;
}


BOOL COpenFileDialog::ShowDialog(HWND _hwndParent)
{
    mOwner = _hwndParent;
    OPENFILENAME ofn ;

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = this->mOwner;
    ofn.lpstrDefExt = this->mDefaultExtension;
    ofn.lpstrFile = this->mFileName;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = this->mFilter;
    ofn.nFilterIndex = this->mFilterIndex;
    ofn.lpstrInitialDir = this->mInitialDir;
    ofn.lpstrTitle = this->mTitle;
    ofn.Flags = this->mFlags;

    GetOpenFileName(&ofn);

    if (_tcslen(this->mFileName) == 0) 
        return FALSE;

    return TRUE;
}

TCHAR * COpenFileDialog::GetFileName()
{
    return mFileName;
}


COpenFileDialog::~COpenFileDialog(void)
{
    SAFE_DELETE_OBJECT (mOwner);

    SAFE_DELETE_ALL (mDefaultExtension);
    SAFE_DELETE_ALL (mFileName);
    SAFE_DELETE_ALL (mFilter);
    SAFE_DELETE_ALL (mInitialDir);
    SAFE_DELETE_ALL (mTitle);  
}
