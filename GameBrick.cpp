

#include "stdafx.h"
#include "GameBrick.h"
#include <windowsx.h>
#include <CommCtrl.h>
#include <windows.h>
#include <fstream>
#include <string.h>

#pragma comment(lib, "comctl32")
#pragma comment(linker,"\"/manifestdependency:type='win32' \ name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \ processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

HINSTANCE hInst;

HWND hField;//дескриптор игрового поля
TCHAR *szTrueCode1 = new TCHAR[1024];
TCHAR *szTrueCode2 = new TCHAR[1024];
TCHAR *szTrueCode3 = new TCHAR[1024];
TCHAR *szTrueCode4 = new TCHAR[1024];

TCHAR szClientCode1[1024] = { 0 };//буфер инициализированный нулями
TCHAR szClientCodeMorze[1024] = { 0 };//буфер инициализированный нулями
TCHAR szClientCode3[1024] = { 0 };//буфер инициализированный нулями
TCHAR szClientCode4[1024] = { 0 };//буфер инициализированный нулями

//длина массива точек и тире для каждой буквы
int const iLengthLetter=6;
//максимально допустимая длина кодового слова
int const iMaxLengthMorzeCode=10;
//размер массива алфавитно-цифрового сопоставления
int const iLengthAlphabet=42;
//размерность массива точек и тире для заданного слова
int szMorzeWord[iLengthLetter*iMaxLengthMorzeCode]={3};
//интервальные переменные для правильного мигания лампочки
int iDot=500;
int iDash=2000;
int iPauseSymbol=500;
int iPauseLetter=1500;
int iPauseWord=7000;
int iPauseBeforeOpenDoor=40000;
bool bStartThread=false;

//длина принимаемого кода. пришлось вынести в глобальную секцию
int iLengthClientOpenDoorCode=0;

bool bDisableField1=false;
bool bDisableField2=false;
bool bDisableField3=false;
bool bDisableField4=false;
TCHAR szPath [1024]=TEXT("\"C:\\Users\\Public\\Downloads\\MP709\\startvideo.bat\"");//C:\Users\Public\Downloads\MP709
//TCHAR szPath [1024]=TEXT("\"e:\\MP709\\startvideo.bat\"");
HWND pBlinkWindow;
HANDLE hSave;

INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK DlgEnterCode(HWND, UINT, WPARAM, LPARAM);
void OnCommand(HWND, int, HWND, UINT);
DWORD WINAPI MorzeOn(LPVOID);
int GetDataFromFile();

struct LetterMorze
{
	int Letter;
	int szMorzeCode[iLengthLetter];
};

LetterMorze szAlphaBet[iLengthAlphabet] = 
{
	//а
	szAlphaBet[0].Letter=1072,
	szAlphaBet[0].szMorzeCode[0]=0,
	szAlphaBet[0].szMorzeCode[1]=1,
	szAlphaBet[0].szMorzeCode[2]=2,
	szAlphaBet[0].szMorzeCode[3]=2,
	szAlphaBet[0].szMorzeCode[4]=2,
	szAlphaBet[0].szMorzeCode[5]=2,
	//б
	szAlphaBet[0].Letter=1073,
	szAlphaBet[0].szMorzeCode[0]=1,
	szAlphaBet[0].szMorzeCode[1]=0,
	szAlphaBet[0].szMorzeCode[2]=0,
	szAlphaBet[0].szMorzeCode[3]=0,
	szAlphaBet[0].szMorzeCode[4]=2,
	szAlphaBet[0].szMorzeCode[5]=2,
	//в
	szAlphaBet[0].Letter=1074,
	szAlphaBet[0].szMorzeCode[0]=0,
	szAlphaBet[0].szMorzeCode[1]=1,
	szAlphaBet[0].szMorzeCode[2]=1,
	szAlphaBet[0].szMorzeCode[3]=2,
	szAlphaBet[0].szMorzeCode[4]=2,
	szAlphaBet[0].szMorzeCode[5]=2,
	//г
	szAlphaBet[0].Letter=1075,
	szAlphaBet[0].szMorzeCode[0]=1,
	szAlphaBet[0].szMorzeCode[1]=1,
	szAlphaBet[0].szMorzeCode[2]=0,
	szAlphaBet[0].szMorzeCode[3]=2,
	szAlphaBet[0].szMorzeCode[4]=2,
	szAlphaBet[0].szMorzeCode[5]=2,
	//д
	szAlphaBet[0].Letter=1076,
	szAlphaBet[0].szMorzeCode[0]=1,
	szAlphaBet[0].szMorzeCode[1]=0,
	szAlphaBet[0].szMorzeCode[2]=0,
	szAlphaBet[0].szMorzeCode[3]=2,
	szAlphaBet[0].szMorzeCode[4]=2,
	szAlphaBet[0].szMorzeCode[5]=2,
	//е
	szAlphaBet[0].Letter=1077,
	szAlphaBet[0].szMorzeCode[0]=0,
	szAlphaBet[0].szMorzeCode[1]=2,
	szAlphaBet[0].szMorzeCode[2]=2,
	szAlphaBet[0].szMorzeCode[3]=2,
	szAlphaBet[0].szMorzeCode[4]=2,
	szAlphaBet[0].szMorzeCode[5]=2,
	//ж
	szAlphaBet[0].Letter=1078,
	szAlphaBet[0].szMorzeCode[0]=0,
	szAlphaBet[0].szMorzeCode[1]=0,
	szAlphaBet[0].szMorzeCode[2]=0,
	szAlphaBet[0].szMorzeCode[3]=1,
	szAlphaBet[0].szMorzeCode[4]=2,
	szAlphaBet[0].szMorzeCode[5]=2,
	//з
	szAlphaBet[0].Letter=1079,
	szAlphaBet[0].szMorzeCode[0]=1,
	szAlphaBet[0].szMorzeCode[1]=1,
	szAlphaBet[0].szMorzeCode[2]=0,
	szAlphaBet[0].szMorzeCode[3]=0,
	szAlphaBet[0].szMorzeCode[4]=2,
	szAlphaBet[0].szMorzeCode[5]=2,
	//и
	szAlphaBet[0].Letter=1080,
	szAlphaBet[0].szMorzeCode[0]=0,
	szAlphaBet[0].szMorzeCode[1]=0,
	szAlphaBet[0].szMorzeCode[2]=2,
	szAlphaBet[0].szMorzeCode[3]=2,
	szAlphaBet[0].szMorzeCode[4]=2,
	szAlphaBet[0].szMorzeCode[5]=2,
	//й
	szAlphaBet[0].Letter=1081,
	szAlphaBet[0].szMorzeCode[0]=0,
	szAlphaBet[0].szMorzeCode[1]=1,
	szAlphaBet[0].szMorzeCode[2]=1,
	szAlphaBet[0].szMorzeCode[3]=1,
	szAlphaBet[0].szMorzeCode[4]=2,
	szAlphaBet[0].szMorzeCode[5]=2,
	//к
	szAlphaBet[0].Letter=1082,
	szAlphaBet[0].szMorzeCode[0]=1,
	szAlphaBet[0].szMorzeCode[1]=0,
	szAlphaBet[0].szMorzeCode[2]=1,
	szAlphaBet[0].szMorzeCode[3]=2,
	szAlphaBet[0].szMorzeCode[4]=2,
	szAlphaBet[0].szMorzeCode[5]=2,
	//л
	szAlphaBet[0].Letter=1083,
	szAlphaBet[0].szMorzeCode[0]=0,
	szAlphaBet[0].szMorzeCode[1]=1,
	szAlphaBet[0].szMorzeCode[2]=0,
	szAlphaBet[0].szMorzeCode[3]=0,
	szAlphaBet[0].szMorzeCode[4]=2,
	szAlphaBet[0].szMorzeCode[5]=2,
	//м
	szAlphaBet[0].Letter=1084,
	szAlphaBet[0].szMorzeCode[0]=1,
	szAlphaBet[0].szMorzeCode[1]=1,
	szAlphaBet[0].szMorzeCode[2]=2,
	szAlphaBet[0].szMorzeCode[3]=2,
	szAlphaBet[0].szMorzeCode[4]=2,
	szAlphaBet[0].szMorzeCode[5]=2,
	//н
	szAlphaBet[0].Letter=1085,
	szAlphaBet[0].szMorzeCode[0]=1,
	szAlphaBet[0].szMorzeCode[1]=0,
	szAlphaBet[0].szMorzeCode[2]=2,
	szAlphaBet[0].szMorzeCode[3]=2,
	szAlphaBet[0].szMorzeCode[4]=2,
	szAlphaBet[0].szMorzeCode[5]=2,
	//о
	szAlphaBet[0].Letter=1086,
	szAlphaBet[0].szMorzeCode[0]=1,
	szAlphaBet[0].szMorzeCode[1]=1,
	szAlphaBet[0].szMorzeCode[2]=1,
	szAlphaBet[0].szMorzeCode[3]=2,
	szAlphaBet[0].szMorzeCode[4]=2,
	szAlphaBet[0].szMorzeCode[5]=2,
	//п
	szAlphaBet[0].Letter=1087,
	szAlphaBet[0].szMorzeCode[0]=0,
	szAlphaBet[0].szMorzeCode[1]=1,
	szAlphaBet[0].szMorzeCode[2]=1,
	szAlphaBet[0].szMorzeCode[3]=0,
	szAlphaBet[0].szMorzeCode[4]=2,
	szAlphaBet[0].szMorzeCode[5]=2,
	//р
	szAlphaBet[0].Letter=1088,
	szAlphaBet[0].szMorzeCode[0]=0,
	szAlphaBet[0].szMorzeCode[1]=1,
	szAlphaBet[0].szMorzeCode[2]=0,
	szAlphaBet[0].szMorzeCode[3]=2,
	szAlphaBet[0].szMorzeCode[4]=2,
	szAlphaBet[0].szMorzeCode[5]=2,
	//с
	szAlphaBet[0].Letter=1089,
	szAlphaBet[0].szMorzeCode[0]=0,
	szAlphaBet[0].szMorzeCode[1]=0,
	szAlphaBet[0].szMorzeCode[2]=0,
	szAlphaBet[0].szMorzeCode[3]=2,
	szAlphaBet[0].szMorzeCode[4]=2,
	szAlphaBet[0].szMorzeCode[5]=2,
	//т
	szAlphaBet[0].Letter=1090,
	szAlphaBet[0].szMorzeCode[0]=1,
	szAlphaBet[0].szMorzeCode[1]=2,
	szAlphaBet[0].szMorzeCode[2]=2,
	szAlphaBet[0].szMorzeCode[3]=2,
	szAlphaBet[0].szMorzeCode[4]=2,
	szAlphaBet[0].szMorzeCode[5]=2,
	//у
	szAlphaBet[0].Letter=1091,
	szAlphaBet[0].szMorzeCode[0]=0,
	szAlphaBet[0].szMorzeCode[1]=0,
	szAlphaBet[0].szMorzeCode[2]=1,
	szAlphaBet[0].szMorzeCode[3]=2,
	szAlphaBet[0].szMorzeCode[4]=2,
	szAlphaBet[0].szMorzeCode[5]=2,
	//ф
	szAlphaBet[0].Letter=1092,
	szAlphaBet[0].szMorzeCode[0]=0,
	szAlphaBet[0].szMorzeCode[1]=0,
	szAlphaBet[0].szMorzeCode[2]=1,
	szAlphaBet[0].szMorzeCode[3]=0,
	szAlphaBet[0].szMorzeCode[4]=2,
	szAlphaBet[0].szMorzeCode[5]=2,
	//х
	szAlphaBet[0].Letter=1093,
	szAlphaBet[0].szMorzeCode[0]=0,
	szAlphaBet[0].szMorzeCode[1]=0,
	szAlphaBet[0].szMorzeCode[2]=0,
	szAlphaBet[0].szMorzeCode[3]=0,
	szAlphaBet[0].szMorzeCode[4]=2,
	szAlphaBet[0].szMorzeCode[5]=2,
	//ц
	szAlphaBet[0].Letter=1094,
	szAlphaBet[0].szMorzeCode[0]=1,
	szAlphaBet[0].szMorzeCode[1]=0,
	szAlphaBet[0].szMorzeCode[2]=1,
	szAlphaBet[0].szMorzeCode[3]=0,
	szAlphaBet[0].szMorzeCode[4]=2,
	szAlphaBet[0].szMorzeCode[5]=2,
	//ч
	szAlphaBet[0].Letter=1095,
	szAlphaBet[0].szMorzeCode[0]=1,
	szAlphaBet[0].szMorzeCode[1]=1,
	szAlphaBet[0].szMorzeCode[2]=1,
	szAlphaBet[0].szMorzeCode[3]=0,
	szAlphaBet[0].szMorzeCode[4]=2,
	szAlphaBet[0].szMorzeCode[5]=2,
	//ш
	szAlphaBet[0].Letter=1096,
	szAlphaBet[0].szMorzeCode[0]=1,
	szAlphaBet[0].szMorzeCode[1]=1,
	szAlphaBet[0].szMorzeCode[2]=1,
	szAlphaBet[0].szMorzeCode[3]=1,
	szAlphaBet[0].szMorzeCode[4]=2,
	szAlphaBet[0].szMorzeCode[5]=2,
	//щ
	szAlphaBet[0].Letter=1097,
	szAlphaBet[0].szMorzeCode[0]=1,
	szAlphaBet[0].szMorzeCode[1]=1,
	szAlphaBet[0].szMorzeCode[2]=0,
	szAlphaBet[0].szMorzeCode[3]=1,
	szAlphaBet[0].szMorzeCode[4]=2,
	szAlphaBet[0].szMorzeCode[5]=2,
	//ъ
	szAlphaBet[0].Letter=1098,
	szAlphaBet[0].szMorzeCode[0]=1,
	szAlphaBet[0].szMorzeCode[1]=1,
	szAlphaBet[0].szMorzeCode[2]=0,
	szAlphaBet[0].szMorzeCode[3]=1,
	szAlphaBet[0].szMorzeCode[4]=1,
	szAlphaBet[0].szMorzeCode[5]=2,
	//ы
	szAlphaBet[0].Letter=1099,
	szAlphaBet[0].szMorzeCode[0]=1,
	szAlphaBet[0].szMorzeCode[1]=0,
	szAlphaBet[0].szMorzeCode[2]=1,
	szAlphaBet[0].szMorzeCode[3]=1,
	szAlphaBet[0].szMorzeCode[4]=2,
	szAlphaBet[0].szMorzeCode[5]=2,
	//ь
	szAlphaBet[0].Letter=1100,
	szAlphaBet[0].szMorzeCode[0]=1,
	szAlphaBet[0].szMorzeCode[1]=0,
	szAlphaBet[0].szMorzeCode[2]=0, 
	szAlphaBet[0].szMorzeCode[3]=1, 
	szAlphaBet[0].szMorzeCode[4]=2,
	szAlphaBet[0].szMorzeCode[5]=2,
	//э
	szAlphaBet[0].Letter=1101,
	szAlphaBet[0].szMorzeCode[0]=0,
	szAlphaBet[0].szMorzeCode[1]=0,
	szAlphaBet[0].szMorzeCode[2]=1,
	szAlphaBet[0].szMorzeCode[3]=0, 
	szAlphaBet[0].szMorzeCode[4]=0,
	szAlphaBet[0].szMorzeCode[5]=2,
	//ю
	szAlphaBet[0].Letter=1102,
	szAlphaBet[0].szMorzeCode[0]=0,
	szAlphaBet[0].szMorzeCode[1]=0,
	szAlphaBet[0].szMorzeCode[2]=1,
	szAlphaBet[0].szMorzeCode[3]=1,
	szAlphaBet[0].szMorzeCode[4]=2,
	szAlphaBet[0].szMorzeCode[5]=2,
	//я
	szAlphaBet[0].Letter=1103,
	szAlphaBet[0].szMorzeCode[0]=0,
	szAlphaBet[0].szMorzeCode[1]=1,
	szAlphaBet[0].szMorzeCode[2]=0,
	szAlphaBet[0].szMorzeCode[3]=1,
	szAlphaBet[0].szMorzeCode[4]=2,
	szAlphaBet[0].szMorzeCode[5]=2,

	szAlphaBet[0].Letter='1', szAlphaBet[0].szMorzeCode[0]=0, szAlphaBet[0].szMorzeCode[1]=1, szAlphaBet[0].szMorzeCode[2]=1, szAlphaBet[0].szMorzeCode[3]=1, szAlphaBet[0].szMorzeCode[4]=1, szAlphaBet[0].szMorzeCode[5]=2,
	szAlphaBet[0].Letter='2', szAlphaBet[0].szMorzeCode[0]=0, szAlphaBet[0].szMorzeCode[1]=0, szAlphaBet[0].szMorzeCode[2]=1, szAlphaBet[0].szMorzeCode[3]=1, szAlphaBet[0].szMorzeCode[4]=1, szAlphaBet[0].szMorzeCode[5]=2,
	szAlphaBet[0].Letter='3', szAlphaBet[0].szMorzeCode[0]=0, szAlphaBet[0].szMorzeCode[1]=0, szAlphaBet[0].szMorzeCode[2]=0, szAlphaBet[0].szMorzeCode[3]=1, szAlphaBet[0].szMorzeCode[4]=1, szAlphaBet[0].szMorzeCode[5]=2,
	szAlphaBet[0].Letter='4', szAlphaBet[0].szMorzeCode[0]=0, szAlphaBet[0].szMorzeCode[1]=0, szAlphaBet[0].szMorzeCode[2]=0, szAlphaBet[0].szMorzeCode[3]=0, szAlphaBet[0].szMorzeCode[4]=1, szAlphaBet[0].szMorzeCode[5]=2,
	szAlphaBet[0].Letter='5', szAlphaBet[0].szMorzeCode[0]=0, szAlphaBet[0].szMorzeCode[1]=0, szAlphaBet[0].szMorzeCode[2]=0, szAlphaBet[0].szMorzeCode[3]=0, szAlphaBet[0].szMorzeCode[4]=0, szAlphaBet[0].szMorzeCode[5]=2,
	szAlphaBet[0].Letter='6', szAlphaBet[0].szMorzeCode[0]=1, szAlphaBet[0].szMorzeCode[1]=0, szAlphaBet[0].szMorzeCode[2]=0, szAlphaBet[0].szMorzeCode[3]=0, szAlphaBet[0].szMorzeCode[4]=0, szAlphaBet[0].szMorzeCode[5]=2,
	szAlphaBet[0].Letter='7', szAlphaBet[0].szMorzeCode[0]=1, szAlphaBet[0].szMorzeCode[1]=1, szAlphaBet[0].szMorzeCode[2]=0, szAlphaBet[0].szMorzeCode[3]=0, szAlphaBet[0].szMorzeCode[4]=0, szAlphaBet[0].szMorzeCode[5]=2,
	szAlphaBet[0].Letter='8', szAlphaBet[0].szMorzeCode[0]=1, szAlphaBet[0].szMorzeCode[1]=1, szAlphaBet[0].szMorzeCode[2]=1, szAlphaBet[0].szMorzeCode[3]=0, szAlphaBet[0].szMorzeCode[4]=0, szAlphaBet[0].szMorzeCode[5]=2,
	szAlphaBet[0].Letter='9', szAlphaBet[0].szMorzeCode[0]=1, szAlphaBet[0].szMorzeCode[1]=1, szAlphaBet[0].szMorzeCode[2]=1, szAlphaBet[0].szMorzeCode[3]=1, szAlphaBet[0].szMorzeCode[4]=0, szAlphaBet[0].szMorzeCode[5]=2,
	szAlphaBet[0].Letter='0', szAlphaBet[0].szMorzeCode[0]=1, szAlphaBet[0].szMorzeCode[1]=1, szAlphaBet[0].szMorzeCode[2]=1, szAlphaBet[0].szMorzeCode[3]=1, szAlphaBet[0].szMorzeCode[4]=1, szAlphaBet[0].szMorzeCode[5]=2,
};

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	hInst = hInstance;
	INT_PTR nResult;
	if(GetDataFromFile())
	{
		nResult = DialogBox(hInst, MAKEINTRESOURCE(IDD_ENTER), NULL, DlgEnterCode);
		pBlinkWindow = CreateWindow(_T("STATIC"), _T(""), WS_CHILD | WS_BORDER | WS_VISIBLE, 10, 10, 10, 10, HWND_MESSAGE, (HMENU)777, hInst, NULL);
	}
	else
	{
		nResult = DialogBox(hInstance, MAKEINTRESOURCE(IDD_NEW_CODE), NULL, DlgProc);
	}
	//INT_PTR nResult = DialogBox(hInstance, MAKEINTRESOURCE(IDD_NEW_CODE), NULL, DlgProc);
	return nResult;
}

INT_PTR CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
	}
	return 0;
}

void OnCommand(HWND hDlg, int Id, HWND hCtrl, UINT codeNotify)
{
	static int stat;
	int iLengthCode1=0;
	int iLengthCode2=0;
	int iLengthCode3=0;
	int iLengthCode4=0;
	int iIndexMorzeWordArray=0;
	if(szMorzeWord[0]==3)
	{
		for(int i=1;i<(iLengthLetter*iMaxLengthMorzeCode);i++)
		{
			szMorzeWord[i]=4;
		}
	}
	switch (Id)
	{
		case IDCANCEL:
		{
			EndDialog(hDlg, IDCANCEL);
		}
			break;
		case IDC_EXIT:
		{
			EndDialog(hDlg, IDCANCEL);
		}
			break;
		case IDC_SAVE:		
		{
			iLengthCode1 = SendDlgItemMessage(hDlg, IDC_CODE1, WM_GETTEXTLENGTH, 0, 0);
			TCHAR *pCode1 = new TCHAR[iLengthCode1 + 1];
			SendDlgItemMessage(hDlg, IDC_CODE1, WM_GETTEXT, iLengthCode1+1, (LPARAM)pCode1);
			SendDlgItemMessage(hDlg, IDC_CODE1, WM_SETTEXT, NULL, NULL);
			wcscpy_s(szTrueCode1, 1024, pCode1);

			iLengthCode2 = SendDlgItemMessage(hDlg, IDC_CODE2, WM_GETTEXTLENGTH, 0, 0);
			TCHAR *pCode2 = new TCHAR[iLengthCode2 + 1];
			SendDlgItemMessage(hDlg, IDC_CODE2, WM_GETTEXT, iLengthCode2+1, (LPARAM)pCode2);
			SendDlgItemMessage(hDlg, IDC_CODE2, WM_SETTEXT, NULL, NULL);
			wcscpy_s(szTrueCode2, 1024, pCode2);

			iLengthCode3 = SendDlgItemMessage(hDlg, IDC_CODE3, WM_GETTEXTLENGTH, 0, 0);
			TCHAR *pCode3 = new TCHAR[iLengthCode3 + 1];
			SendDlgItemMessage(hDlg, IDC_CODE3, WM_GETTEXT, iLengthCode3+1, (LPARAM)pCode3);
			SendDlgItemMessage(hDlg, IDC_CODE3, WM_SETTEXT, NULL, NULL);
			wcscpy_s(szTrueCode3, 1024, pCode3);

			iLengthCode4 = SendDlgItemMessage(hDlg, IDC_CODE4, WM_GETTEXTLENGTH, 0, 0);
			TCHAR *pCode4 = new TCHAR[iLengthCode4 + 1];
			SendDlgItemMessage(hDlg, IDC_CODE4, WM_GETTEXT, iLengthCode4+1, (LPARAM)pCode4);
			SendDlgItemMessage(hDlg, IDC_CODE4, WM_SETTEXT, NULL, NULL);
			wcscpy_s(szTrueCode4, 1024, pCode4);



			if(pCode1!=0 && iLengthCode4<iMaxLengthMorzeCode)
			{
				//цикл обхода слова
				for(int j=0;j<iLengthCode4;j++)
				{
					//цикл обхода массива соответствий букв и цифр значениям кода морзе
					for(int i=0;i<iLengthAlphabet;i++)
					{
						//проверка на соответствие букве/цифре
						//если не совпало, переходим к следующему символу в массиве соответствий символов кодам
						if((int)szTrueCode4[j]!=szAlphaBet[i].Letter)
						{
							continue;
						}
						else
						{
							//если совпало, то открываем цикл присваивания кода морзе совпавшего символа спецмассиву
							for(int k=0;k<iLengthLetter;k++)
							{
								//проверка на конец кода морзе в описательной строке массива
								//и выяснение что это не последний символ в кодовом слове
								if(szAlphaBet[i].szMorzeCode[k]!=2)
								{
									szMorzeWord[iIndexMorzeWordArray]=szAlphaBet[i].szMorzeCode[k];
									iIndexMorzeWordArray++;
								}
								//проверка на конец кода морзе в описательной строке массива
								//и выяснение что это последняя точка/тире в букве и что буква в слове не последняя
								//(вставка паузы в коде морзе между буквами)
								else if(szAlphaBet[i].szMorzeCode[k]==2)
								{
									szMorzeWord[iIndexMorzeWordArray]=szAlphaBet[i].szMorzeCode[k];
									iIndexMorzeWordArray++;
									//проверка на конец кода морзе в описательной строке массива
									//и выяснение что это последний символ в кодовом слове
									//(вставка паузы между повторением слова, она будет больше чем пауза между буквами слова
									if((j+1)==iLengthCode4)
									{
										szMorzeWord[iIndexMorzeWordArray]=3;
									}
									break;
								}
							}
							break;
						}
					}
				}
				delete [] pCode1;
				delete [] pCode4;
				delete [] pCode3;
				delete [] pCode2;

				if(szMorzeWord[0]==3)
				{
					EndDialog(hDlg, IDCANCEL);
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ENTER), hDlg, DlgEnterCode);
				}
				else
				{
					EndDialog(hDlg, IDCANCEL);
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ENTER), hDlg, DlgEnterCode);
					pBlinkWindow = CreateWindow(_T("STATIC"), _T(""), WS_CHILD | WS_BORDER | WS_VISIBLE, 10, 10, 10, 10, HWND_MESSAGE, (HMENU)777, hInst, NULL);
					//hSave = CreateThread(NULL, 0, MorzeOn, (LPVOID)pBlinkWindow, 0, NULL);
				}
			}
		}
		break;
	}
}

INT_PTR CALLBACK DlgEnterCode(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	TCHAR szRecieveCode[1024] = { 0 };
	TCHAR szRecieveCode2[1024] = { 0 };
	TCHAR szRecieveCode3[1024] = { 0 };
	TCHAR szRecieveCode4[1024] = { 0 };
	TCHAR szErr[]=L"ОШИБКА!!!";
	TCHAR szEnterCorrectCode[]=L"ПРАВИЛЬНО!!!";
	
	TCHAR szExitCode[10]={'1','1','1','1','0'};
	UNREFERENCED_PARAMETER(lParam);
	if(!bStartThread && szMorzeWord[0]!=3)
	{
		Sleep(1000);
		if(hSave!=NULL)
		{
			TerminateThread(hSave, 0);
		}
		hSave = CreateThread(NULL, 0, MorzeOn, (LPVOID)pBlinkWindow, 0, NULL);
		bStartThread=true;
	}
	switch (message)
	{
	case WM_COMMAND:
		//прием кода из первого поля
		if (LOWORD(wParam) == IDC_RECIEVE && !bDisableField1)
		{
			iLengthClientOpenDoorCode = SendDlgItemMessage(hDlg, IDC_CODE, WM_GETTEXTLENGTH, 0, 0);
			TCHAR *pCodeRecieve = new TCHAR[iLengthClientOpenDoorCode + 1];
			SendDlgItemMessage(hDlg, IDC_CODE, WM_GETTEXT, iLengthClientOpenDoorCode+1, (LPARAM)pCodeRecieve);
			wcscpy_s(szRecieveCode, iLengthClientOpenDoorCode+1, pCodeRecieve);
			delete [] pCodeRecieve;

			if(!(wcscmp(szRecieveCode, szExitCode)))
			{
				bool bExit=false;
				std::ofstream outf("MP709.local.set", std::ios::out);
				do
				{
					if(!outf)
					{
						Sleep(50);
						bExit=true;
					}
					else
					{
						bExit=false;
						outf<<"SVET=OFF";
						outf.close();
					}
				}
				while(bExit);
				if(hSave!=NULL)
				{
					TerminateThread(hSave, 0);
				}
				CloseHandle(hSave);
				EndDialog(pBlinkWindow, IDCANCEL);
				EndDialog(hDlg, IDCANCEL);
			}				
			else if (!(wcscmp(szRecieveCode, szTrueCode1)))
			{
				bDisableField1=true;
				EnableWindow(GetDlgItem(hDlg, IDC_RECIEVE), false);
				EnableWindow(GetDlgItem(hDlg, IDC_CODE), false);
				SetDlgItemText(hDlg, IDC_STATIC, NULL);

				for(int i=0;i<3;i++)
				{
					SetDlgItemText(hDlg, IDC_STATIC, NULL);
					Sleep(400);
					SetDlgItemText(hDlg, IDC_STATIC, szEnterCorrectCode);
					Sleep(400);
				}
				SetDlgItemText(hDlg, IDC_STATIC, NULL);

				if(bDisableField1 && bDisableField2 && bDisableField3 && (bDisableField4 || szMorzeWord[0]==3))
				{
					EnableWindow(GetDlgItem(hDlg, IDC_RECIEVE), true);
					EnableWindow(GetDlgItem(hDlg, IDC_CODE), true);
					SendDlgItemMessage(hDlg, IDC_CODE, WM_SETTEXT, NULL, NULL);

					EnableWindow(GetDlgItem(hDlg, IDC_RECIEVE2), true);
					EnableWindow(GetDlgItem(hDlg, IDC_CODE2), true);
					SendDlgItemMessage(hDlg, IDC_CODE2, WM_SETTEXT, NULL, NULL);

					EnableWindow(GetDlgItem(hDlg, IDC_RECIEVE3), true);
					EnableWindow(GetDlgItem(hDlg, IDC_CODE3), true);
					SendDlgItemMessage(hDlg, IDC_CODE3, WM_SETTEXT, NULL, NULL);

					EnableWindow(GetDlgItem(hDlg, IDC_RECIEVE4), true);
					EnableWindow(GetDlgItem(hDlg, IDC_CODE4), true);
					SendDlgItemMessage(hDlg, IDC_CODE4, WM_SETTEXT, NULL, NULL);
					SetDlgItemText(hDlg, IDC_STATIC, NULL);

					bDisableField1=false;
					bDisableField2=false;
					bDisableField3=false;
					bDisableField4=false;

					for(int i=0;i<1024;i++)
					{
						szRecieveCode[i] = 0;
						szRecieveCode2[i] = 0;
						szRecieveCode3[i] = 0;
						szRecieveCode4[i] = 0;
					}
					if(hSave!=NULL)
					{
						TerminateThread(hSave, 0);
					}
					ShowWindow(hDlg, SW_HIDE);
									
					STARTUPINFO si={0};
					PROCESS_INFORMATION pi={0};
					memset(&si, 0, sizeof(STARTUPINFO));
					BOOL bStartVideo = CreateProcess(NULL, szPath, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
					Sleep(200);

					if(bStartVideo)
					{
						WaitForSingleObject(pi.hProcess, INFINITE);
					}
					Sleep(1000);
					CloseHandle(pi.hProcess);
					CloseHandle(pi.hThread);

					bool bExit=false;
					for(int j=0;j<4;j++)
					{
						if(!(j%2))
						{
							std::ofstream outf("MP709.local.set", std::ios::out);
							do
							{
								if(!outf)
								{
									Sleep(50);
									bExit=true;
								}
								else
								{
									bExit=false;
									outf<<"DOOR=ON";
									outf.close();
								}
							}
							while(bExit);
							Sleep(1000);
						}
						else
						{
							std::ofstream outf("MP709.local.set", std::ios::out);
							do
							{
								if(!outf)
								{
									Sleep(50);
									bExit=true;
								}
								else
								{
									bExit=false;
									outf<<"DOOR=OFF";
									outf.close();
								}
							}
							while(bExit);
						}
					}
					Sleep(500);
					ShowWindow(hDlg, SW_SHOW);
					if(szMorzeWord[0]!=3)
					{
						if(hSave!=NULL)
						{
							TerminateThread(hSave, 0);
						}
						hSave = CreateThread(NULL, 0, MorzeOn, (LPVOID)pBlinkWindow, 0, NULL);
					}
				}
			}
			else
			{
				for(int i=0;i<3;i++)
				{
					SetDlgItemText(hDlg, IDC_STATIC, NULL);
					Sleep(400);
					SetDlgItemText(hDlg, IDC_STATIC, szErr);
					Sleep(400);
				}
				SetDlgItemText(hDlg, IDC_STATIC, NULL);
				SetDlgItemText(hDlg, IDC_STATIC, NULL);
				SetDlgItemText(hDlg, IDC_CODE, NULL);
			}			
		}
		//прием кода из второго поля
		else if (LOWORD(wParam) == IDC_RECIEVE2 && !bDisableField2)
		{
			iLengthClientOpenDoorCode = SendDlgItemMessage(hDlg, IDC_CODE2, WM_GETTEXTLENGTH, 0, 0);
			TCHAR *pCodeRecieve = new TCHAR[iLengthClientOpenDoorCode + 1];
			SendDlgItemMessage(hDlg, IDC_CODE2, WM_GETTEXT, iLengthClientOpenDoorCode+1, (LPARAM)pCodeRecieve);
			wcscpy_s(szRecieveCode2, iLengthClientOpenDoorCode+1, pCodeRecieve);

			if(!(wcscmp(szRecieveCode2, szExitCode)))
			{
				bool bExit=false;
				std::ofstream outf("MP709.local.set", std::ios::out);
				do
				{
					if(!outf)
					{
						Sleep(50);
						bExit=true;
					}
					else
					{
						bExit=false;
						outf<<"SVET=OFF";
						outf.close();
					}
				}
				while(bExit);
				if(hSave!=NULL)
				{
					TerminateThread(hSave, 0);
				}
				CloseHandle(hSave);
				EndDialog(pBlinkWindow, IDCANCEL);
				EndDialog(hDlg, IDCANCEL);
			}
			else if(!(wcscmp(szRecieveCode2, szTrueCode2)))
			{
				bDisableField2=true;
				SetDlgItemText(hDlg, IDC_STATIC, NULL);
				EnableWindow(GetDlgItem(hDlg, IDC_RECIEVE2), false);
				EnableWindow(GetDlgItem(hDlg, IDC_CODE2), false);

				for(int i=0;i<3;i++)
				{
					SetDlgItemText(hDlg, IDC_STATIC, NULL);
					Sleep(400);
					SetDlgItemText(hDlg, IDC_STATIC, szEnterCorrectCode);
					Sleep(400);
				}

				if(bDisableField1 && bDisableField2 && bDisableField3 && (bDisableField4 || szMorzeWord[0]==3))
				{
					EnableWindow(GetDlgItem(hDlg, IDC_RECIEVE), true);
					EnableWindow(GetDlgItem(hDlg, IDC_CODE), true);
					SendDlgItemMessage(hDlg, IDC_CODE, WM_SETTEXT, NULL, NULL);

					EnableWindow(GetDlgItem(hDlg, IDC_RECIEVE2), true);
					EnableWindow(GetDlgItem(hDlg, IDC_CODE2), true);
					SendDlgItemMessage(hDlg, IDC_CODE2, WM_SETTEXT, NULL, NULL);

					EnableWindow(GetDlgItem(hDlg, IDC_RECIEVE3), true);
					EnableWindow(GetDlgItem(hDlg, IDC_CODE3), true);
					SendDlgItemMessage(hDlg, IDC_CODE3, WM_SETTEXT, NULL, NULL);

					EnableWindow(GetDlgItem(hDlg, IDC_RECIEVE4), true);
					EnableWindow(GetDlgItem(hDlg, IDC_CODE4), true);
					SendDlgItemMessage(hDlg, IDC_CODE4, WM_SETTEXT, NULL, NULL);
					SetDlgItemText(hDlg, IDC_STATIC, NULL);

					bDisableField1=false;
					bDisableField2=false;
					bDisableField3=false;
					bDisableField4=false;

					for(int i=0;i<1024;i++)
					{
						szRecieveCode[i] = 0;
						szRecieveCode2[i] = 0;
						szRecieveCode3[i] = 0;
						szRecieveCode4[i] = 0;
					}
					if(hSave!=NULL)
					{
						TerminateThread(hSave, 0);
					}
					ShowWindow(hDlg, SW_HIDE);

					STARTUPINFO si={0};
					PROCESS_INFORMATION pi={0};
					memset(&si, 0, sizeof(STARTUPINFO));
					BOOL bStartVideo = CreateProcess(NULL, szPath, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
					Sleep(200);

					if(bStartVideo)
					{
						WaitForSingleObject(pi.hProcess, INFINITE);
					}
					Sleep(1000);	
					CloseHandle(pi.hProcess);
					CloseHandle(pi.hThread);
					bool bExit=false;
					for(int j=0;j<4;j++)
					{
						if(!(j%2))
						{
							std::ofstream outf("MP709.local.set", std::ios::out);
							do
							{
								if(!outf)
								{
									Sleep(50);
									bExit=true;
								}
								else
								{
									bExit=false;
									outf<<"DOOR=ON";
									outf.close();
								}
							}
							while(bExit);
							Sleep(1000);
						}
						else
						{
							std::ofstream outf("MP709.local.set", std::ios::out);
							do
							{
								if(!outf)
								{
									Sleep(50);
									bExit=true;
								}
								else
								{
									bExit=false;
									outf<<"DOOR=OFF";
									outf.close();
								}
							}
							while(bExit);
						}
					}
					Sleep(500);
					ShowWindow(hDlg, SW_SHOW);
					if(szMorzeWord[0]!=3)
					{
						if(hSave!=NULL)
						{
							TerminateThread(hSave, 0);
						}
						hSave = CreateThread(NULL, 0, MorzeOn, (LPVOID)pBlinkWindow, 0, NULL);
					}
				}
				else
				{

				}
			}			
			else
			{
				for(int i=0;i<3;i++)
				{
					SetDlgItemText(hDlg, IDC_STATIC, NULL);
					Sleep(400);
					SetDlgItemText(hDlg, IDC_STATIC, szErr);
					Sleep(400);
				}
				SetDlgItemText(hDlg, IDC_STATIC, NULL);
				SetDlgItemText(hDlg, IDC_STATIC, NULL);
				SetDlgItemText(hDlg, IDC_CODE2, NULL);
			}
			delete [] pCodeRecieve;
		}
		//прием кода из третьего поля
		else if (LOWORD(wParam) == IDC_RECIEVE3 && !bDisableField3)
		{
			iLengthClientOpenDoorCode = SendDlgItemMessage(hDlg, IDC_CODE3, WM_GETTEXTLENGTH, 0, 0);
			TCHAR *pCodeRecieve = new TCHAR[iLengthClientOpenDoorCode + 1];
			SendDlgItemMessage(hDlg, IDC_CODE3, WM_GETTEXT, iLengthClientOpenDoorCode+1, (LPARAM)pCodeRecieve);
			wcscpy_s(szRecieveCode3, iLengthClientOpenDoorCode+1, pCodeRecieve);

			if(!(wcscmp(szRecieveCode3, szExitCode)))
			{
				bool bExit=false;
				std::ofstream outf("MP709.local.set", std::ios::out);
				do
				{
					if(!outf)
					{
						Sleep(50);
						bExit=true;
					}
					else
					{
						bExit=false;
						outf<<"SVET=OFF";
						outf.close();
					}
				}
				while(bExit);
				if(hSave!=NULL)
				{
					TerminateThread(hSave, 0);
				}
				CloseHandle(hSave);
				EndDialog(pBlinkWindow, IDCANCEL);
				EndDialog(hDlg, IDCANCEL);
			}
			else if(!(wcscmp(szRecieveCode3, szTrueCode3)))
			{
				bDisableField3=true;
				SetDlgItemText(hDlg, IDC_STATIC, NULL);
				EnableWindow(GetDlgItem(hDlg, IDC_RECIEVE3), false);
				EnableWindow(GetDlgItem(hDlg, IDC_CODE3), false);

				for(int i=0;i<3;i++)
				{
					SetDlgItemText(hDlg, IDC_STATIC, NULL);
					Sleep(400);
					SetDlgItemText(hDlg, IDC_STATIC, szEnterCorrectCode);
					Sleep(400);
				}

				if(bDisableField1 && bDisableField2 && bDisableField3 && (bDisableField4 || szMorzeWord[0]==3))
				{
					EnableWindow(GetDlgItem(hDlg, IDC_RECIEVE), true);
					EnableWindow(GetDlgItem(hDlg, IDC_CODE), true);
					SendDlgItemMessage(hDlg, IDC_CODE, WM_SETTEXT, NULL, NULL);

					EnableWindow(GetDlgItem(hDlg, IDC_RECIEVE2), true);
					EnableWindow(GetDlgItem(hDlg, IDC_CODE2), true);
					SendDlgItemMessage(hDlg, IDC_CODE2, WM_SETTEXT, NULL, NULL);

					EnableWindow(GetDlgItem(hDlg, IDC_RECIEVE3), true);
					EnableWindow(GetDlgItem(hDlg, IDC_CODE3), true);
					SendDlgItemMessage(hDlg, IDC_CODE3, WM_SETTEXT, NULL, NULL);

					EnableWindow(GetDlgItem(hDlg, IDC_RECIEVE4), true);
					EnableWindow(GetDlgItem(hDlg, IDC_CODE4), true);
					SendDlgItemMessage(hDlg, IDC_CODE4, WM_SETTEXT, NULL, NULL);
					SetDlgItemText(hDlg, IDC_STATIC, NULL);

					bDisableField1=false;
					bDisableField2=false;
					bDisableField3=false;
					bDisableField4=false;

					for(int i=0;i<1024;i++)
					{
						szRecieveCode[i] = 0;
						szRecieveCode2[i] = 0;
						szRecieveCode3[i] = 0;
						szRecieveCode4[i] = 0;
					}
					if(hSave!=NULL)
					{
						TerminateThread(hSave, 0);
					}
					ShowWindow(hDlg, SW_HIDE);

					STARTUPINFO si={0};
					PROCESS_INFORMATION pi={0};
					memset(&si, 0, sizeof(STARTUPINFO));
					BOOL bStartVideo = CreateProcess(NULL, szPath, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
					Sleep(200);

					if(bStartVideo)
					{
						WaitForSingleObject(pi.hProcess, INFINITE);
					}
					Sleep(1000);
					CloseHandle(pi.hProcess);
					CloseHandle(pi.hThread);
					bool bExit=false;
					for(int j=0;j<4;j++)
					{
						if(!(j%2))
						{
							std::ofstream outf("MP709.local.set", std::ios::out);
							do
							{
								if(!outf)
								{
									Sleep(50);
									bExit=true;
								}
								else
								{
									bExit=false;
									outf<<"DOOR=ON";
									outf.close();
								}
							}
							while(bExit);
							Sleep(1000);
						}
						else
						{
							std::ofstream outf("MP709.local.set", std::ios::out);
							do
							{
								if(!outf)
								{
									Sleep(50);
									bExit=true;
								}
								else
								{
									bExit=false;
									outf<<"DOOR=OFF";
									outf.close();
								}
							}
							while(bExit);
						}
					}
					Sleep(500);
					ShowWindow(hDlg, SW_SHOW);
					if(szMorzeWord[0]!=3)
					{
						if(hSave!=NULL)
						{
							TerminateThread(hSave, 0);
						}
						hSave = CreateThread(NULL, 0, MorzeOn, (LPVOID)pBlinkWindow, 0, NULL);
					}
				}
			}			
			else
			{
				for(int i=0;i<3;i++)
				{
					SetDlgItemText(hDlg, IDC_STATIC, NULL);
					Sleep(400);
					SetDlgItemText(hDlg, IDC_STATIC, szErr);
					Sleep(400);
				}
				SetDlgItemText(hDlg, IDC_STATIC, NULL);
				SetDlgItemText(hDlg, IDC_STATIC, NULL);
				SetDlgItemText(hDlg, IDC_CODE3, NULL);
			}
			delete [] pCodeRecieve;
		}
		//прием кода из четвертого поля
		else if (LOWORD(wParam) == IDC_RECIEVE4 && !bDisableField4)
		{
			iLengthClientOpenDoorCode = SendDlgItemMessage(hDlg, IDC_CODE4, WM_GETTEXTLENGTH, 0, 0);
			TCHAR *pCodeRecieve = new TCHAR[iLengthClientOpenDoorCode + 1];
			SendDlgItemMessage(hDlg, IDC_CODE4, WM_GETTEXT, iLengthClientOpenDoorCode+1, (LPARAM)pCodeRecieve);
			wcscpy_s(szRecieveCode4, iLengthClientOpenDoorCode+1, pCodeRecieve);

			if(!(wcscmp(szRecieveCode4, szExitCode)))
			{
				bool bExit=false;
				std::ofstream outf("MP709.local.set", std::ios::out);
				do
				{
					if(!outf)
					{
						Sleep(50);
						bExit=true;
					}
					else
					{
						bExit=false;
						outf<<"SVET=OFF";
						outf.close();
					}
				}
				while(bExit);
				if(hSave!=NULL)
				{
					TerminateThread(hSave, 0);
				}
				CloseHandle(hSave);
				EndDialog(pBlinkWindow, IDCANCEL);
				EndDialog(hDlg, IDCANCEL);
			}
			else if(!(wcscmp(szRecieveCode4, szTrueCode4)))
			{
				bDisableField4=true;
				SetDlgItemText(hDlg, IDC_STATIC, NULL);
				EnableWindow(GetDlgItem(hDlg, IDC_RECIEVE4), false);
				EnableWindow(GetDlgItem(hDlg, IDC_CODE4), false);

				for(int i=0;i<3;i++)
				{
					SetDlgItemText(hDlg, IDC_STATIC, NULL);
					Sleep(400);
					SetDlgItemText(hDlg, IDC_STATIC, szEnterCorrectCode);
					Sleep(400);
				}
				if(bDisableField1 && bDisableField2 && bDisableField3 && (bDisableField4 || szMorzeWord[0]==3))
				{
					EnableWindow(GetDlgItem(hDlg, IDC_RECIEVE), true);
					EnableWindow(GetDlgItem(hDlg, IDC_CODE), true);
					SendDlgItemMessage(hDlg, IDC_CODE, WM_SETTEXT, NULL, NULL);

					EnableWindow(GetDlgItem(hDlg, IDC_RECIEVE2), true);
					EnableWindow(GetDlgItem(hDlg, IDC_CODE2), true);
					SendDlgItemMessage(hDlg, IDC_CODE2, WM_SETTEXT, NULL, NULL);

					EnableWindow(GetDlgItem(hDlg, IDC_RECIEVE3), true);
					EnableWindow(GetDlgItem(hDlg, IDC_CODE3), true);
					SendDlgItemMessage(hDlg, IDC_CODE3, WM_SETTEXT, NULL, NULL);

					EnableWindow(GetDlgItem(hDlg, IDC_RECIEVE4), true);
					EnableWindow(GetDlgItem(hDlg, IDC_CODE4), true);
					SendDlgItemMessage(hDlg, IDC_CODE4, WM_SETTEXT, NULL, NULL);
					SetDlgItemText(hDlg, IDC_STATIC, NULL);

					bDisableField1=false;
					bDisableField2=false;
					bDisableField3=false;
					bDisableField4=false;

					for(int i=0;i<1024;i++)
					{
						szRecieveCode[i] = 0;
						szRecieveCode2[i] = 0;
						szRecieveCode3[i] = 0;
						szRecieveCode4[i] = 0;
					}
					if(hSave!=NULL)
					{
						TerminateThread(hSave, 0);
					}
					ShowWindow(hDlg, SW_HIDE);

					STARTUPINFO si={0};
					PROCESS_INFORMATION pi={0};
					memset(&si, 0, sizeof(STARTUPINFO));
					BOOL bStartVideo = CreateProcess(NULL, szPath, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);					
					Sleep(200);

					if(bStartVideo)
					{
						WaitForSingleObject(pi.hProcess, INFINITE);
					}
					Sleep(1000);
					CloseHandle(pi.hProcess);
					CloseHandle(pi.hThread);

					bool bExit=false;
					for(int j=0;j<4;j++)
					{
						if(!(j%2))
						{
							std::ofstream outf("MP709.local.set", std::ios::out);
							do
							{
								if(!outf)
								{
									Sleep(50);
									bExit=true;
								}
								else
								{
									bExit=false;
									outf<<"DOOR=ON";
									outf.close();
								}
							}
							while(bExit);
							Sleep(1000);
						}
						else
						{
							std::ofstream outf("MP709.local.set", std::ios::out);
							do
							{
								if(!outf)
								{
									Sleep(50);
									bExit=true;
								}
								else
								{
									bExit=false;
									outf<<"DOOR=OFF";
									outf.close();
								}
							}
							while(bExit);
						}
					}
					Sleep(500);
					ShowWindow(hDlg, SW_SHOW);
					if(szMorzeWord[0]!=3)
					{
						if(hSave!=NULL)
						{
							TerminateThread(hSave, 0);
						}
						hSave = CreateThread(NULL, 0, MorzeOn, (LPVOID)pBlinkWindow, 0, NULL);
					}
				}
			}		
			else
			{
				for(int i=0;i<3;i++)
				{
					SetDlgItemText(hDlg, IDC_STATIC, NULL);
					Sleep(400);
					SetDlgItemText(hDlg, IDC_STATIC, szErr);
					Sleep(400);
				}
				SetDlgItemText(hDlg, IDC_STATIC, NULL);
				SetDlgItemText(hDlg, IDC_STATIC, NULL);
				SetDlgItemText(hDlg, IDC_CODE4, NULL);
			}
			delete [] pCodeRecieve;
		}
	}
	return (INT_PTR)FALSE;
}

DWORD WINAPI MorzeOn(LPVOID hBlink)
{
	HDC hdc = GetDC((HWND)hBlink);
    HBRUSH hWhite = CreateSolidBrush(RGB(255,255,255));
	HBRUSH hBlack = CreateSolidBrush(RGB(0,0,0));
	SelectObject(hdc,hWhite);
	int iHorRes = GetDeviceCaps(hdc, HORZRES);
	int iVertRes = GetDeviceCaps(hdc, VERTRES);
	Rectangle(hdc,0,0,iHorRes,iVertRes);
	//Rectangle(hdc,0,0,200,150);

	do
	{
		for(int i=0;i<(iLengthLetter*iMaxLengthMorzeCode);i++)
		{
			if(szMorzeWord[i]==0)
			{
				SelectObject(hdc,hWhite);
				Rectangle(hdc,0,0,iHorRes,iVertRes);
				Sleep(iDot);
				SelectObject(hdc,hBlack);
				Rectangle(hdc,0,0,iHorRes,iVertRes);
				Sleep(iPauseSymbol);
			}
			else if(szMorzeWord[i]==1)
			{
				SelectObject(hdc,hWhite);
				Rectangle(hdc,0,0,iHorRes,iVertRes);
				Sleep(iDash);
				SelectObject(hdc,hBlack);
				Rectangle(hdc,0,0,iHorRes,iVertRes);
				Sleep(iPauseSymbol);
			}
			else if(szMorzeWord[i]==2)
			{
				//держим выключенным свет на интервал между буквами слова
				SelectObject(hdc,hBlack);
				Rectangle(hdc,0,0,iHorRes,iVertRes);
				Sleep(iPauseLetter);
			}
			else if(szMorzeWord[i]==3)
			{
				//держим выключенным свет на интервал между повторами слова
				SelectObject(hdc,hBlack);
				Rectangle(hdc,0,0,iHorRes,iVertRes);
				Sleep(iPauseWord);
				break;
			}
		}
	}while(true);
	ReleaseDC((HWND)hBlink, hdc);
}

int GetDataFromFile()
{
	int iExit=1;
	char szBufTemp[1024]={'\0'};
	char ch;
	int iCounterArray=0;
	int iSeparatorCounter=0;
	int iCounterCodeArray=0;
	int iLengthCodeMorze=0;
	int iIndexMorzeWordArray=0;

	CHAR szTempTrueCode1[1024]={'\0'};
	CHAR szTempTrueCode2[1024]={'\0'};
	CHAR szTempTrueCode3[1024]={'\0'};
	CHAR szTempTrueCode4[1024]={'\0'};

	std::ifstream inf("c:\\Users\\Public\\Downloads\\MP709\\data.txt", std::ios::in | std::ios::binary);
	while(inf.get(ch))
	{
		int j=(int)ch;
		if(ch!='0')
		{
			if(ch=='|')
			{
					iSeparatorCounter++;
					iCounterCodeArray=0;
					continue;
			}
			switch(iSeparatorCounter)
			{
			case 0:
				{
					for(iCounterArray=0;iCounterArray<1024;iCounterArray++)
					{
						szTempTrueCode1[iCounterCodeArray++]=ch;
						break;
					}
					MultiByteToWideChar(CP_ACP, NULL, szTempTrueCode1, iCounterCodeArray, szTrueCode1, iCounterCodeArray);
					szTrueCode1[iCounterCodeArray]='\0';
				}
				break;
			case 1:
				{
					for(iCounterArray=0;iCounterArray<1024;iCounterArray++)
					{
						szTempTrueCode2[iCounterCodeArray++]=ch;
						break;
					}
					MultiByteToWideChar(CP_ACP, NULL, szTempTrueCode2, iCounterCodeArray, szTrueCode2, iCounterCodeArray);
					szTrueCode2[iCounterCodeArray]='\0';
				}
				break;
			case 2:
				{
					for(iCounterArray=0;iCounterArray<1024;iCounterArray++)
					{
						szTempTrueCode3[iCounterCodeArray++]=ch;
						break;
					}
					MultiByteToWideChar(CP_ACP, NULL, szTempTrueCode3, iCounterCodeArray, szTrueCode3, iCounterCodeArray);
					szTrueCode3[iCounterCodeArray]='\0';
				}
				break;
			case 3:
				{
					for(iCounterArray=0;iCounterArray<1024;iCounterArray++)
					{
						szTempTrueCode4[iCounterCodeArray++]=ch;
						break;
					}
					MultiByteToWideChar(CP_ACP, NULL, szTempTrueCode4, iCounterCodeArray, szTrueCode4, iCounterCodeArray);
					szTrueCode4[iCounterCodeArray]='\0';
					iLengthCodeMorze=iCounterCodeArray;
				}
				break;
			}
		}
		else
		{
			iExit=0;
			break;
		}
	}
	inf.close();


	if(szMorzeWord[0]==3)
	{
		for(int i=1;i<(iLengthLetter*iMaxLengthMorzeCode);i++)
		{
			szMorzeWord[i]=4;
		}
	}

	//цикл обхода слова
	for(int j=0;j<iLengthCodeMorze;j++)
	{
		//цикл обхода массива соответствий букв и цифр значениям кода морзе
		for(int i=0;i<iLengthAlphabet;i++)
		{
			//проверка на соответствие букве/цифре
			//если не совпало, переходим к следующему символу в массиве соответствий символов кодам
			if((int)szTrueCode4[j]!=szAlphaBet[i].Letter)
			{
				continue;
			}
			else
			{
				//если совпало, то открываем цикл присваивания кода морзе совпавшего символа спецмассиву
				for(int k=0;k<iLengthLetter;k++)
				{
					//проверка на конец кода морзе в описательной строке массива
					//и выяснение что это не последний символ в кодовом слове
					if(szAlphaBet[i].szMorzeCode[k]!=2)
					{
						szMorzeWord[iIndexMorzeWordArray]=szAlphaBet[i].szMorzeCode[k];
						iIndexMorzeWordArray++;
					}
					//проверка на конец кода морзе в описательной строке массива
					//и выяснение что это последняя точка/тире в букве и что буква в слове не последняя
					//(вставка паузы в коде морзе между буквами)
					else if(szAlphaBet[i].szMorzeCode[k]==2)
					{
						szMorzeWord[iIndexMorzeWordArray]=szAlphaBet[i].szMorzeCode[k];
						iIndexMorzeWordArray++;
						//проверка на конец кода морзе в описательной строке массива
						//и выяснение что это последний символ в кодовом слове
						//(вставка паузы между повторением слова, она будет больше чем пауза между буквами слова
						if((j+1)==iLengthCodeMorze)
						{
							szMorzeWord[iIndexMorzeWordArray]=3;
						}
						break;
					}
				}
				break;
			}
		}
}

	return iExit;
}