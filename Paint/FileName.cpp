#include "stdafx.h"

FileName::FileName()
{
	currentFileName = (char*)calloc(_MAX_FNAME, sizeof(char));
	currentWindowCaption = (char*)calloc(_MAX_FNAME+strlen(programName), sizeof(char));
	memcpy(currentFileName, baseFileName, strlen(baseFileName)*sizeof(char));
}


FileName::~FileName()
{
	free(currentFileName);
	free(currentWindowCaption);
}

void FileName::ResetFileName()
{
	strcpy_s(currentFileName, _MAX_FNAME, baseFileName);
}

void FileName::SetFileName(char* path)
{
	_splitpath_s(path, NULL, 0, NULL, 0, currentFileName, _MAX_FNAME, NULL, 0);
}

char* FileName::GetCurrentFileName()
{
	return currentFileName;
}

char* FileName::GetSaveQuestionString()
{
	_strset_s(currentWindowCaption, _MAX_FNAME + strlen(programName), '\0');
	sprintf_s(currentWindowCaption, _MAX_FNAME + strlen(programName), "Вы хотите сохранить изменения в файле %s", currentFileName);
	return currentWindowCaption;
}

char* FileName::CreateTempFileName(char* fullpath)
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	char newPath[_MAX_PATH];

	_splitpath_s(fullpath,drive,sizeof(drive),dir,sizeof(dir),fname,sizeof(fname),ext,sizeof(ext));
	strcat_s(fname, sizeof(fname), "_new");
	sprintf_s(newPath, sizeof(newPath), "%s%s%s%s", drive, dir, fname, ext);
	return newPath;
}

void FileName::SetWindowCaption(HWND hWnd)
{
	_strset_s(currentWindowCaption, _MAX_FNAME + strlen(programName), '\0');
	sprintf_s(currentWindowCaption, _MAX_FNAME + strlen(programName), "%s - %s", currentFileName,programName);
	SetWindowText(hWnd,currentWindowCaption);
}
