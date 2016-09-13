#pragma once
class FileName
{
	public:
		FileName();
		void SetFileName(char*);
		char* GetCurrentFileName();
		char* GetSaveQuestionString();
		char* CreateTempFileName(char* filename);
		void SetWindowCaption(HWND);
		void ResetFileName();
		~FileName();
	private:
		char* currentFileName;
		char* currentWindowCaption;
		const char* baseFileName = "Безымянный";
		const char* programName = "Paint";
};

