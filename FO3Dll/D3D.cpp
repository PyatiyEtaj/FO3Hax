#include "D3D.h"


#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"
#include <d3d9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
//#pragma comment(lib, "d3d9")
//#pragma comment(lib, "d3dx9")


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

char buf [256] = "raz dva tri";
float f = 0.5f;
HaxSettings* _haxSettings;

void ImGuiLoop(LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	if (!_haxSettings->IsImGuiInit)
	{
		_haxSettings->IsImGuiInit = true;
		D3DDEVICE_CREATION_PARAMETERS CP;
		auto window = FindWindowA(NULL, FO3_NAME);
		g_pd3dDevice->GetCreationParameters(&CP);
		window = CP.hFocusWindow;
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.MouseDrawCursor = true;
		ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX9_Init(g_pd3dDevice);
		ImGui::SetNextWindowSize(ImVec2(200, 200));
	}


	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Ussless hax");
	ImGui::Text("--------------------------------------------");
	ImGui::Text("# Attack only from 1 hex (Hold Shift)\n# Healing Rate CD\n# Fast reload (Shift + F5)");
	ImGui::Checkbox("Use safe 1 hex", &_haxSettings->UseSafe1Hex);
	ImGui::Checkbox("Show heal rate cd", &_haxSettings->ShowHealRateCD);
	ImGui::ColorEdit4("CH colors", _haxSettings->Colors);


	ImGui::EndFrame();
	ImGui::Render();

	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

#define FLOATS_TO_COLOR(colors) D3DCOLOR_ARGB(int(colors[3]*255), int(colors[0] * 255), int(colors[1] * 255), int(colors[2] * 255))

HRESULT __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDev)
{
	typedef HRESULT(__stdcall* es)(LPDIRECT3DDEVICE9);
	es f = es(hsES->OriginalOps);
	if (_haxSettings) {
		if (_haxSettings->IsMenuOpen)
			ImGuiLoop(pDev);
		SetCursor(_haxSettings->IsMenuOpen ? LoadCursor(NULL, IDC_ARROW) : NULL);
		if (!xPos && !yPos )
			Init();

		D3DVIEWPORT9 Viewport;
		pDev->GetViewport(&Viewport);

		if (_haxSettings->UseSafe1Hex)
		{
			D3DRECT rec1 = { *xPos, *yPos-7, *xPos + 1,  *yPos +6 };
			D3DRECT rec2 = { *xPos-6, *yPos-1, *xPos+7, *yPos };
			DWORD color = FLOATS_TO_COLOR(_haxSettings->Colors);
			pDev->Clear(1, &rec1, D3DCLEAR_TARGET, color, 0, 0);
			pDev->Clear(1, &rec2, D3DCLEAR_TARGET, color, 0, 0);
		}

		if (!IsTdCreated)
		{
			td = CrtD3DTextDrawer(pDev, 50, 300, D3DCOLOR_ARGB(255, 255, 255, 0), "Consolas", 18);
			IsTdCreated = true;
		}
		xDrawText(td, OutputText.c_str());
	}
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


void D3D::Constructor(LPCSTR wndName, HaxSettings* haxSettings)
{
	typedef LPDIRECT3D9(__stdcall* D3DCreate)(UINT);
	_haxSettings = haxSettings;
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
	
	//ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
