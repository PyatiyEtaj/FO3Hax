#pragma once
#include <windows.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9")
#pragma comment(lib, "d3dx9")

typedef struct
{
	LPD3DXFONT D3D_Font;
	RECT Rect;
	D3DCOLOR Color;
	int Size;
}D3DTextDrawer;

D3DTextDrawer* CrtD3DTextDrawer(LPDIRECT3DDEVICE9 pDev, int x, int y, D3DCOLOR color, LPCSTR fontName, int Size = 8);

void xDrawText(D3DTextDrawer* td, LPCSTR xText);
