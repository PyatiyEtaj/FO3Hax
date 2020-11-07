#include "PathFinder.h"

uint PathFinder::DistGame(int x1, int y1, int x2, int y2) const noexcept
{
	int dx = (x1 > x2 ? x1 - x2 : x2 - x1);
	if (!(x1 & 1))
	{
		if (y2 <= y1)
		{
			int rx = y1 - y2 - dx / 2;
			return dx + (rx > 0 ? rx : 0);
		}
		else
		{
			int rx = y2 - y1 - (dx + 1) / 2;
			return dx + (rx > 0 ? rx : 0);
		}
	}
	else
	{
		if (y2 >= y1)
		{
			int rx = y2 - y1 - dx / 2;
			return dx + (rx > 0 ? rx : 0);
		}
		else
		{
			int rx = y1 - y2 - (dx + 1) / 2;
			return dx + (rx > 0 ? rx : 0);
		}
	}
}

void PathFinder::SendMove(ushort x, ushort y) const noexcept
{
	NetBufferPushUint(foclient_, 0xBD5A2CAA);
	NetBufferPushUint(foclient_, 0x80000000);
	NetBufferPushUshort(foclient_, x);
	NetBufferPushUshort(foclient_, y);
}

void PathFinder::Init() noexcept
{
	foclient_ = (PBYTE)(*(PDWORD)GET_ADR(FO_CLIENT_ADR));
	hexManager_ = (PBYTE)((PWORD)foclient_ + 16);
	chosen_ = (uchar**)((PDWORD)foclient_ + 12540);
	steps_ = new std::vector<uchar>();
}


bool PathFinder::IsFree() const noexcept
{
	uint gametick = GetGameTime();
	uint startTick = *(DWORD*)(*chosen_ + 5728);
	uint tickCount = *(DWORD*)(*chosen_ + 5724);
	return gametick - startTick >= tickCount;
}

/*
bool PathFinder::MoveChosenTo(ushort hx, ushort hy)
{
	if (!*chosen_) return false;

	int curX = (*chosen_)[6];
	int curY = (*chosen_)[8];

	while (curX != hx || curY != hy) {
		Sleep(100);
		if (IsFree()) {
			FindPath(hexManager_, 0, nullptr, curX, curY, &hx, &hy, steps_, -1);
			if (steps_->size() == 0)
				break;
			MoveHexByDirUnsafe(&curX, &curY, (*steps_)[0]);
			SendMove(curX, curY);
			TransitCritter(hexManager_, 0, *chosen_, curX, curY, true, true);
			steps_->clear();
		}
	}
	return true;
}*/

bool PathFinder::MoveChosenTo(ushort hx, ushort hy)
{
	return *chosen_ ? MoveChosenNative(hx, hy) : false;
}

bool PathFinder::MoveChosenNative(ushort hx, ushort hy) noexcept
{
	int curX = (*chosen_)[6];
	int curY = (*chosen_)[8];
	bool res = FindPath(hexManager_, 0, *chosen_, curX, curY, &hx, &hy, steps_, -1);
	if (res && steps_->size() > 0) {
		DWORD v482 = *(DWORD*)(foclient_ + 482 * 4);
		auto s = (ActionStruct*)(*((DWORD*)foclient_ + 581));
		*((DWORD*)foclient_ + 582) = *((DWORD*)foclient_ + 581) + 28;
		*((DWORD*)foclient_ + 483) += steps_->size();
		s->SetZero();
		s->firstBytes[0] = 1;
		s->params[0] = hx;
		s->params[1] = hy;
		s->params[2] = 1;
		for (int i = 0; i < steps_->size(); i++) {
			*((char*)v482 + i) = steps_->at(i);
		}
	}
	return res;
}

bool PathFinder::FindBestHex(ushort& hx, ushort& hy) const noexcept
{
	if (*chosen_) {
		int curX = (*chosen_)[6];
		int curY = (*chosen_)[8];
		if (DistGame(curX, curY, hx, hy) == 1) {
			for (int i = -1; i <= 1; i++)
			{
				for (int j = -1; j <= 1; j++) {
					int posX = curX + i;
					int posY = curY + j;
					FindPath(hexManager_, 0, nullptr, curX, curY, (ushort*)&posX, (ushort*)&posY, steps_, -1);
					bool pathHasBeenFound = steps_->size() > 0;
					steps_->clear();
					if (DistGame(posX, posY, hx, hy) == 2 &&
						DistGame(curX, curY, posX, posY) == 1 && 
						pathHasBeenFound
						) 
					{
						hx = posX;
						hy = posY;
						return true;
					}					
				}
			}
		}
	}

	return false;
}

