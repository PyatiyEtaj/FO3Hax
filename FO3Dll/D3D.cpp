#include "D3D.h"

bool IsCreated = false;
bool IsTdCreated = false;
HookSetter* hsDIP;
HookSetter* hsES;
int origES = 0;
D3DTextDrawer* td;
const DWORD _endScene = 42;
const DWORD _drawIndexedPrimitive = 82;
const DWORD _reset = 16;
std::string OutputText = "";

int* xPos = 0;
int* yPos = 0;
bool* _needMove1Hex;

void Init() {
	xPos = (int*)GET_ADR(GLOBAL_MOUSE_X);
	yPos = (int*)GET_ADR(GLOBAL_MOUSE_Y);
}

HRESULT __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDev)
{
	typedef HRESULT(__stdcall* es)(LPDIRECT3DDEVICE9);
	es f = es(hsES->OriginalOps);

	if (!xPos && !yPos )
		Init();

	D3DVIEWPORT9 Viewport;
	pDev->GetViewport(&Viewport);

	if (*_needMove1Hex)
	{
		D3DRECT rec1 = { *xPos, *yPos-7, *xPos + 1,  *yPos +6 };
		D3DRECT rec2 = { *xPos-6, *yPos-1, *xPos+7, *yPos };
		pDev->Clear(1, &rec1, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 255, 255, 0), 0, 0);
		pDev->Clear(1, &rec2, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 255, 255, 0), 0, 0);
	}

	if (!IsTdCreated)
	{
		td = CrtD3DTextDrawer(pDev, 50, 300, D3DCOLOR_ARGB(255, 255, 255, 0), "Consolas", 18);
		IsTdCreated = true;
	}
//#ifndef RELEASE
	xDrawText(td, OutputText.c_str());
//#endif
	return f(pDev);
}

void D3D::SetOutputText(const std::string &str)
{
	OutputText = str;
}

void D3D::SetOutputText(const std::string&& str)
{
	OutputText = str;
}
void D3D::SetOutputText(const char* str)
{
	OutputText = str;
}
void D3D::Constructor(LPCSTR wndName, bool* NeedMove1Hex)
{
	_needMove1Hex = NeedMove1Hex;
	typedef LPDIRECT3D9(__stdcall* D3DCreate)(UINT);

	HMODULE hDLL = GetModuleHandleA(xorstr("d3d9"));
	if (hDLL == nullptr) { printf_s(xorstr("Can't find module d3d9.dll\nAbort...\n")); return; }
	D3DCreate pDirect3DCreate9 = (D3DCreate)GetProcAddress(hDLL, xorstr("Direct3DCreate9"));

	LPDIRECT3D9 pD3D = pDirect3DCreate9(D3D_SDK_VERSION);
	D3DDISPLAYMODE d3ddm;
	HRESULT hRes = pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = d3ddm.Format;
	IDirect3DDevice9* ppReturnedDeviceInterface = nullptr;
	HWND hWnd = FindWindowA(NULL, wndName);
	if (hWnd) {
		hRes = pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &ppReturnedDeviceInterface
		);
	}

	if (ppReturnedDeviceInterface != nullptr) {
		PDWORD vtable = (PDWORD)(*((PDWORD)ppReturnedDeviceInterface));
		PBYTE endScene = (PBYTE)vtable[_endScene];
		hsES = CrtHookSetter(endScene, (DWORD)&hkEndScene, 7);
		SetHookSetter(hsES);
		ppReturnedDeviceInterface->Release();
		IsCreated = true;
	}

	pD3D->Release();
}


void D3D::Destructor()
{
	UnsetHook(hsES);
}
