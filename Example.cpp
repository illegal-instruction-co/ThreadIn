#include "ThreadIn.h"

#include <iostream>

DWORD WINAPI threadStartFunction(LPVOID lpParam) {
	using namespace std;
	while (true) {
		cout << "Hello world!" << endl;

		this_thread::sleep_for(1s);
	}
}

DWORD WINAPI threadStartFunction2(LPVOID lpParam) {
	using namespace std;
	while (true) {
		cout << "Hello world!" << endl;

		this_thread::sleep_for(1s);
	}
}

void threadStartFunction2Detour(__int64 a1, __int64 a2, __int64 a3, ...) {
	using namespace std;
	cout << "From detour, a1: " << a1 << endl;
	cout << "From detour, a2: " << a2 << endl;
	cout << "From detour, a3: " << a3 << endl;
}

int main() {
	using namespace std;

	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)threadStartFunction, (HMODULE) nullptr, 0, nullptr);
	// CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)threadStartFunction2, (HMODULE) nullptr, 0, nullptr);

	// Freeze threadStartFunction
	ii::ThreadIn(reinterpret_cast<__int64>(&threadStartFunction));

	// Hook threadStartFunction2 to threadStartFunction2Detour
	// ii::ThreadIn(reinterpret_cast<__int64>(&threadStartFunction2), reinterpret_cast<__int64>(&threadStartFunction2Detour));

	return getchar();
}