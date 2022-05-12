// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the DLLTOOLS_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// DLLTOOLS_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DLLTOOLS_EXPORTS
#define DLLTOOLS_API __declspec(dllexport)
#else
#define DLLTOOLS_API __declspec(dllimport)
#endif

// This class is exported from the dll
class DLLTOOLS_API CDLLTools {
public:
	CDLLTools(void);
	// TODO: add your methods here.
};

extern DLLTOOLS_API int nDLLTools;

DLLTOOLS_API int fnDLLTools(void);
