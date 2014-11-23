// AddInDemo.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "AddInDemo.h"
#include <initguid.h>
#include "AddInDemo_i.c"
#include "MyAddIn.h"
#include "AddInRegistrar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CAddInDemoApp


class CAddInDemoModule :
	public ATL::CAtlMfcModule
{
public:
	DECLARE_LIBID(LIBID_AddInDemoLib);
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ADDINDEMO, "{A9ED4AA5-5A45-45DD-AA9E-C13040F4EE8F}");
};

CAddInDemoModule _AtlModule;

BEGIN_MESSAGE_MAP(CAddInDemoApp, CWinApp)
END_MESSAGE_MAP()


// CAddInDemoApp construction

CAddInDemoApp::CAddInDemoApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CAddInDemoApp object

CAddInDemoApp theApp;


// CAddInDemoApp initialization

BOOL CAddInDemoApp::InitInstance()
{
	COleObjectFactory::RegisterAll();
	CWinApp::InitInstance();

	return TRUE;
}

// DllCanUnloadNow - Allows COM to unload DLL
#if !defined(_WIN32_WCE) && !defined(_AMD64_) && !defined(_IA64_)
#pragma comment(linker, "/EXPORT:DllCanUnloadNow=_DllCanUnloadNow@0,PRIVATE")
#pragma comment(linker, "/EXPORT:DllGetClassObject=_DllGetClassObject@12,PRIVATE")
#pragma comment(linker, "/EXPORT:DllRegisterServer=_DllRegisterServer@0,PRIVATE")
#pragma comment(linker, "/EXPORT:DllUnregisterServer=_DllUnregisterServer@0,PRIVATE")
#else
#if defined(_X86_) || defined(_SHX_)
#pragma comment(linker, "/EXPORT:DllCanUnloadNow=_DllCanUnloadNow,PRIVATE")
#pragma comment(linker, "/EXPORT:DllGetClassObject=_DllGetClassObject,PRIVATE")
#pragma comment(linker, "/EXPORT:DllRegisterServer=_DllRegisterServer,PRIVATE")
#pragma comment(linker, "/EXPORT:DllUnregisterServer=_DllUnregisterServer,PRIVATE")
#else
#pragma comment(linker, "/EXPORT:DllCanUnloadNow,PRIVATE")
#pragma comment(linker, "/EXPORT:DllGetClassObject,PRIVATE")
#pragma comment(linker, "/EXPORT:DllRegisterServer,PRIVATE")
#pragma comment(linker, "/EXPORT:DllUnregisterServer,PRIVATE")
#endif // (_X86_)||(_SHX_)
#endif // !_WIN32_WCE && !_AMD64_ && !_IA64_ 

STDAPI DllCanUnloadNow()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (_AtlModule.GetLockCount() > 0)
		return S_FALSE;
	return S_OK;
}

// DllGetClassObject - Returns class factory
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (S_OK == _AtlModule.GetClassObject(rclsid, riid, ppv))
		return S_OK;
	return AfxDllGetClassObject(rclsid, riid, ppv);
}

// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_AtlModule.UpdateRegistryAppId(TRUE);
	HRESULT hRes2 = _AtlModule.RegisterServer(TRUE);
	if (hRes2 != S_OK)
		return hRes2;
	if (!COleObjectFactory::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

#pragma region Solid Edge specific

	// Solid Edge environment(s) that your addin supports. Defined in secatids.h.
	CATID environments[] =
	{
		CATID_SEApplication,
		CATID_SEAllDocumentEnvrionments
	};

	VARIANT_BOOL bAutoConnect = VARIANT_TRUE;

	// Note that Register() is overloaded for additional options.
	CAddInRegistrar::Register(CLSID_MyAddIn, L"AddInDemo", L"My description", _countof(environments), environments, bAutoConnect);

#pragma endregion

	return S_OK;
}

// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_AtlModule.UpdateRegistryAppId(FALSE);
	HRESULT hRes2 = _AtlModule.UnregisterServer(TRUE);
	if (hRes2 != S_OK)
		return hRes2;
	if (!COleObjectFactory::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

#pragma region Solid Edge specific

	CAddInRegistrar::Unregister(CLSID_MyAddIn);

#pragma endregion

	return S_OK;
}
