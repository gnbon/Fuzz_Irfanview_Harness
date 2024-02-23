#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

extern "C" __declspec(dllexport) void fuzzme(wchar_t* param_filepath);

//Function definition: ReadWebP_W(wchat_t* WebpFilePath, wchat_t* IniFilePath, wchat_t* ErrorLogBuffer, wchar_t* NoneBuffer, int* OptArray);
typedef HGLOBAL(WINAPIV* ReadWebP_W_t)(wchar_t*, wchar_t*, wchar_t*, wchar_t*, int*);

HMODULE coverage_dll_handle;
ReadWebP_W_t ReadWebP_W_handle;
wchar_t IniFilePath[2048] = L"C:\\Users\\user\\AppData\\Roaming\\IrfanView\\i_view32.ini";
wchar_t ErrorLogBuffer[2048] = { 0 };
wchar_t NoneBuffer[520] = L"None";

int OptArray[17] = { 0 };

__declspec(noinline) void fuzzme(wchar_t* WebpFilePath) {

	HGLOBAL pGM = ReadWebP_W_handle(WebpFilePath, IniFilePath, ErrorLogBuffer, NoneBuffer, OptArray);
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

	coverage_dll_handle = LoadLibraryA("WebP.DLL");
	if (coverage_dll_handle == NULL) {
		printf("Failed to load WebP.DLL\n");
		return 1;
	}

	ReadWebP_W_handle = (ReadWebP_W_t)(GetProcAddress(coverage_dll_handle, "ReadWebP_W"));
	if (ReadWebP_W_handle == NULL) {
		printf("Failed to get address for ReadWebP_W\n");
		return 1;
	}

	OptArray[0] = 0x44;
	OptArray[3] = 1;
	OptArray[4] = 1;

	fuzzme(argv[1]);
}