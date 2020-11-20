#include "D3D.h"


#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"
#include <d3d9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
#include "Packets.h"
#include "Logger.h"
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


void Init() {
	xPos = (int*)GET_ADR(GLOBAL_MOUSE_X);
	yPos = (int*)GET_ADR(GLOBAL_MOUSE_Y);
}

extern HaxSettings g_HaxSettings;

void ImGuiLoop(LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	if (!g_HaxSettings.IsImGuiInit)
	{
		g_HaxSettings.IsImGuiInit = true;
		D3DDEVICE_CREATION_PARAMETERS CP;
		auto window = FindWindowA(NULL, FO3_NAME);
		g_pd3dDevice->GetCreationParameters(&CP);
		window = CP.hFocusWindow;
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.MouseDrawCursor = true;
		ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
		ImGui::StyleColorsClassic();
		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX9_Init(g_pd3dDevice);
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Ussless hax");

	ImGui::Separator();
	ImGui::Text("# Attack only from 1 hex (Hold Shift)");
	ImGui::Checkbox("Use safe 1 hex", &g_HaxSettings.UseSafe1Hex);
	ImGui::Checkbox("Show heal rate cd", &g_HaxSettings.ShowHealRateCD);
#ifndef RELEASE
	ImGui::Checkbox("Disable orig WndProc", &g_HaxSettings.DisableWndProc);
#endif // !RELEASE
	if (ImGui::Button("Refresh"))
		g_HaxSettings.NeedRefresh = true;
#ifndef RELEASE
	ImGui::SameLine();
	if (ImGui::Button("Open menu Packets"))
		g_HaxSettings.OpenPackets = true;
	ImGui::SameLine();
	if (ImGui::Button("Log"))
		g_HaxSettings.OpenLog = true;
#endif // !RELEASE
	ImGui::Dummy(ImVec2(0.0f, 4.0f));
	ImGui::ColorEdit3("CH colors", g_HaxSettings.Colors);
	ImGui::SliderInt("CH mult", &g_HaxSettings.CrossHairMul, 1, 5);
	ImGui::SliderInt("Thread latency", &g_HaxSettings.ThreadLatency, 30, 120);
	ImGui::Dummy(ImVec2(0.0f, 4.0f));
	ImGui::Separator();
	if (ImGui::Button("Uninject"))
		g_HaxSettings.Uninject = true;
	ImGui::SameLine();
	ImGui::Text(" You will hear `BEEEEP`");
#ifndef RELEASE
	if (g_HaxSettings.OpenPackets)
		Packets::Draw("Packets");
	if (g_HaxSettings.OpenLog)
		Logger::Draw("Log");
#endif
	//ImGui::ShowDemoWindow();
	ImGui::End();
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

#define FLOATS_TO_COLOR(colors) D3DCOLOR_ARGB(255, int(colors[0] * 255), int(colors[1] * 255), int(colors[2] * 255))

HRESULT __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDev)
{
	typedef HRESULT(__stdcall* es)(LPDIRECT3DDEVICE9);
	es f = es(hsES->OriginalOps);

	if (g_HaxSettings.IsMenuOpen)
		ImGuiLoop(pDev);
	SetCursor(g_HaxSettings.IsMenuOpen ? LoadCursor(NULL, IDC_ARROW) : NULL);
	if (!xPos && !yPos)
		Init();

	D3DVIEWPORT9 Viewport;
	pDev->GetViewport(&Viewport);

	if (g_HaxSettings.UseSafe1Hex)
	{
		D3DRECT rec1 = { *xPos, *yPos - 7 * g_HaxSettings.CrossHairMul, *xPos + 1 * g_HaxSettings.CrossHairMul,  *yPos + 6 * g_HaxSettings.CrossHairMul };
		D3DRECT rec2 = { *xPos - 6 * g_HaxSettings.CrossHairMul, *yPos - 1 * g_HaxSettings.CrossHairMul, *xPos + 7 * g_HaxSettings.CrossHairMul, *yPos };
		DWORD color = FLOATS_TO_COLOR(g_HaxSettings.Colors);
		pDev->Clear(1, &rec1, D3DCLEAR_TARGET, color, 0, 0);
		pDev->Clear(1, &rec2, D3DCLEAR_TARGET, color, 0, 0);
	}

	if (!IsTdCreated)
	{
		td = CrtD3DTextDrawer(pDev, 50, 300, D3DCOLOR_ARGB(255, 255, 255, 0), "Consolas", 18);
		IsTdCreated = true;
	}
	xDrawText(td, OutputText.c_str());

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


void D3D::Constructor(LPCSTR wndName)
{
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
	
	//ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
