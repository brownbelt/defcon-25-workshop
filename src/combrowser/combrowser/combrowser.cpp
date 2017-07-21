#include <windows.h>
#include <mshtml.h>
#include <exdisp.h>

#include <stdio.h>

#include <SecurityBaseAPI.h>

int main()
{
	// https://www.experts-exchange.com/questions/20194736/Looking-for-a-simple-IWebBrowser2-sample.html
	CLSID clsid;

	LPUNKNOWN pUnknown = NULL;
	IWebBrowser2 *pBrowser = NULL;
	IDispatch *pDispatch = NULL;
	IHTMLDocument2 *pHTML = NULL;
	//IHTMLElementCollection *pElement = NULL;

	VARIANT var;
	VARIANT_BOOL vBusy;
	
	// 0. get IE class
	OleInitialize(NULL);
	CLSIDFromProgID(OLESTR("InternetExplorer.Application"), &clsid);

	// 1. create instance of IE
	CoCreateInstance(clsid, NULL, CLSCTX_SERVER, IID_IUnknown, (LPVOID *)&pUnknown);

	if (!pUnknown)
		return -1;

	pUnknown->QueryInterface(IID_IWebBrowser2, (LPVOID *)&pBrowser);
	pUnknown->Release();

	// 2. navigate to URL
	BSTR bstrUrl = SysAllocString(L"www.example.com");
	var.vt = VT_I4; // 4 byte signed int
	var.lVal = 0;

	pBrowser->put_Visible(VARIANT_FALSE);
	pBrowser->Navigate(bstrUrl, &var, &var, &var, &var);
	SysFreeString(bstrUrl);

	// 3. wait for page to load
	do
	{
		Sleep(1);
		pBrowser->get_Busy(&vBusy);
	} while (vBusy);

	// 4. get the HTML
	pBrowser->get_Document(&pDispatch);
	pDispatch->QueryInterface(IID_IHTMLDocument2, (LPVOID *)&pHTML);
	pDispatch->Release();

	// 5. print the <title>
	pHTML->get_title(&bstrUrl);
	printf("%ws\n", bstrUrl);

	// 6. cleanup
	system("PAUSE");
	pHTML->Release();
	pBrowser->Quit();
	pBrowser->Release();
	OleUninitialize();

	return 0;
}