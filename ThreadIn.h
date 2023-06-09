#pragma once

#ifdef _WIN64
#define XIP Rip
#else
#define XIP Eip
#endif

#include <thread>
#include <Windows.h>
#include <TlHelp32.h>

namespace ii {
class ThreadIn final {

public:
	ThreadIn(__int64 threadStartAddress)
		: m_currentProcessId(GetCurrentProcessId()), m_thread([this, threadStartAddress]() { Freeze(threadStartAddress, 0); }) {}

	ThreadIn(__int64 threadStartAddress, __int64 forwardAddress)
		: m_currentProcessId(GetCurrentProcessId()), m_thread([this, threadStartAddress, forwardAddress]() {
			  while (true) {
				  Freeze(threadStartAddress, forwardAddress);
			  }
		  }) {}

	ThreadIn(DWORD pid, __int64 threadStartAddress) : m_currentProcessId(pid), m_thread([this, threadStartAddress]() { Freeze(threadStartAddress, 0); }) {}

	ThreadIn(DWORD pid, __int64 threadStartAddress, __int64 forwardAddress)
		: m_currentProcessId(pid), m_thread([this, threadStartAddress, forwardAddress]() {
			  while (true) {
				  Freeze(threadStartAddress, forwardAddress);
			  }
		  }) {}

private:
	__int64 m_currentProcessId;

	std::jthread m_thread;

	enum THREADINFOCLASS {
		ThreadQuerySetWin32StartAddress = 9,
	};

	typedef DWORD(__stdcall* f_NtQueryInformationThread)(HANDLE, THREADINFOCLASS, void*, ULONG_PTR, ULONG_PTR*);

	ULONG_PTR GetThreadStartAddress(HANDLE hThread) {
		auto NtQueryInformationThread =
			reinterpret_cast<f_NtQueryInformationThread>(GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueryInformationThread"));
		if (!NtQueryInformationThread)
			return 0;

		ULONG_PTR ulStartAddress = 0;
		DWORD ret = NtQueryInformationThread(hThread, ThreadQuerySetWin32StartAddress, &ulStartAddress, sizeof(ULONG_PTR), nullptr);

		if (ret)
			return 0;

		return ulStartAddress;
	}

	void Freeze(__int64 tSA, __int64 forwardAddress) {

		HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
		THREADENTRY32 te32;
		CONTEXT ctx;

		hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
		if (hThreadSnap == INVALID_HANDLE_VALUE)
			return;

		te32.dwSize = sizeof(te32);
		ctx.ContextFlags = CONTEXT_FULL;

		Thread32First(hThreadSnap, &te32);
		while (Thread32Next(hThreadSnap, &te32)) {
			if (te32.th32OwnerProcessID == m_currentProcessId) {
				HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, te32.th32ThreadID);

				if (GetThreadStartAddress(hThread) == tSA || tSA == 0) {
					GetThreadContext(hThread, &ctx);

					if (forwardAddress == 0) {
						ctx.XIP = (__int64)+[]() noexcept {
							if (__rdtsc())
								WaitMessage();
						};
					}
					else
						ctx.XIP = forwardAddress;

					SetThreadContext(hThread, &ctx);
				}
			}
		}

		CloseHandle(hThreadSnap);
	}
};
} // namespace ii
