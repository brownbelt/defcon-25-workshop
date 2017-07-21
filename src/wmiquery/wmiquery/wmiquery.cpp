#include <Windows.h>
#include <atlbase.h>

#include <wbemidl.h> // WMI
#pragma comment(lib, "wbemuuid.lib")

#include <string>
#include <iostream>

int main()
{
	CComPtr<IWbemLocator> pWbemLocator;
	CComPtr<IWbemServices> pWbemServices;
	CComPtr<IEnumWbemClassObject> pEnum;
	CComPtr<IWbemClassObject> pWmiObject;
	CComVariant cvtName;
	ULONG uObjectCount = 0;

	/* Step 0. Initialize COM, set the security context to some defaults */
	CoInitialize(NULL);
	CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_PKT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, 0);

	/* Step 1. Connect to WMI root\securitycenter2 namespace */
	pWbemLocator.CoCreateInstance(CLSID_WbemLocator);
	pWbemLocator->ConnectServer(CComBSTR(L"root\\securitycenter2"), NULL, NULL, 0, NULL, 0, NULL, &pWbemServices);

	/* Step 2. Execute WQL query */
	pWbemServices->ExecQuery(CComBSTR("WQL"), CComBSTR(L"SELECT displayName FROM AntiVirusProduct"), WBEM_FLAG_FORWARD_ONLY, NULL, &pEnum);

	/* Step 3. Get first result's displayName */
	pEnum->Next(WBEM_INFINITE, 1, &pWmiObject, &uObjectCount);
	pWmiObject->Get(L"displayName", 0, &cvtName, 0, 0);

	std::cout << CW2A(cvtName.bstrVal) << std::endl;

	system("PAUSE");
	return 0;
}