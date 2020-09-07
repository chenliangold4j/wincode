#pragma once
#include <Windows.h>
#include <functional>


#ifdef HOT_KEY_IMP_
class __declspec(dllexport) HotKey {
#else
class __declspec(dllimport) HotKey {
#endif // HOT_KEY_IMP_
private:
	DWORD tid;
	HANDLE t;

public:
	HotKey();
	~HotKey();

	void Register(UINT mod, UINT key, std::function<void(void)> func);
	void Unregister(UINT mod, UINT key);
};