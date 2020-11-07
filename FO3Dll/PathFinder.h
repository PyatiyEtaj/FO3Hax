#pragma once
#include "FO3Funcs.h"

class PathFinder {
private: // InnerStruct
	struct ActionStruct {
		union {
			uchar firstBytes[4];
			int first4Bytes;
		};
		int params[7];

		void SetZero() {
			for (int i = 0; i < 7; i++)
				params[i] = 0;
		}
	};
public:
	bool NeedMove = false;
	bool IsAttack = false;
	uchar* Target = nullptr;
	bool IsInited = false;
private:
	PBYTE foclient_;
	PBYTE hexManager_;
	uchar** chosen_;
	std::vector<uchar>* steps_;
//---------------------------------------------------------

public:
	void Init() noexcept;
	bool IsFree() const noexcept;
	bool MoveChosenTo(ushort hx, ushort hy);
	bool FindBestHex(ushort& hx, ushort& hy) const noexcept;

	~PathFinder() {
		delete steps_;
	}
private:
	bool MoveChosenNative(ushort hx, ushort hy) noexcept;
	uint DistGame(int x1, int y1, int x2, int y2) const noexcept;
	void SendMove(ushort x, ushort y) const noexcept;
};