#pragma once

class Initializator {
private:
	bool* isInited_ = nullptr;

private:
	bool IsInit() const noexcept { return isInited_ ? *isInited_ : false; }

public:
	void SetInitializator() noexcept;
	void Await() const noexcept;
};