#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

extern "C" __declspec(dllexport) void call_Read_QOI_W(wchar_t* param_filepath);

//Function definition: Read_QOI_W(wchat_t* param_filepath, wchat_t* param_inipath, wchat_t* param_buffer, wchar_t* param_none, int* param_opt);
typedef HGLOBAL* (WINAPIV* Read_QOI_W_t)(wchar_t*, wchar_t*, wchar_t*, wchar_t*, int*);

HMODULE formats_dll_handle;
Read_QOI_W_t Read_QOI_W_handle;
wchar_t param_inipath[2048] = L"C:\Users\user\AppData\Roaming\IrfanView\i_view32.ini";
wchar_t param_buffer[2048] = { 0 };
wchar_t param_none[2048] = L"None";

int param_opt[17] = { 0 };

__declspec(noinline) void call_Read_QOI_W(wchar_t* param_filepath) {
	// Interestingly, this gets called twice - but only for some files
	HGLOBAL* pGM  = Read_QOI_W_handle(param_filepath, param_inipath, param_buffer, param_none, param_opt);
	if (pGM) {
		GlobalFree(pGM);
	}
}

int wmain(int argc, wchar_t** argv)
{
	if (argc != 2) {
		printf("USAGE: %ws input-file\n", argv[0]);
		return 1;
	}

	formats_dll_handle = LoadLibraryA("FORMATS.DLL");
	if (formats_dll_handle == NULL) {
		printf("Failed to load FORMATS.DLL\n");
		return 1;
	}

	Read_QOI_W_handle = (Read_QOI_W_t)(GetProcAddress(formats_dll_handle, "Read_QOI_W"));
	if (Read_QOI_W_handle == NULL) {
		printf("Failed to get address for Read_QOI_W\n");
		return 1;
	}

	memset(param_opt, 0, sizeof(param_opt));
	param_opt[0] = 0x44;
	param_opt[3] = 1; // a3
	param_opt[4] = 1; // a12
	param_opt[5] = 1;

	call_Read_QOI_W(argv[1]);
}