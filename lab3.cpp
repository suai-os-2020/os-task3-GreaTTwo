#include "lab3.h"
#include <windows.h> 

//initializing Handlemutex
HANDLE mutex;

// Semaphores for async threads
HANDLE semG1, semD1, semC1;
HANDLE semG2, semD2;
HANDLE semG3, semI3;
HANDLE semM4;
// semaphores for seq threads
HANDLE semGtoB1, semBtoC1, semCtoD1, semDtoG1;
HANDLE semGtoC2, semCtoD2, semDtoG2;
HANDLE semGtoD3, semDtoG3;

HANDLE t[11];
DWORD id;


unsigned int lab3_thread_graph_id() {
    return 7;
}

const char* lab3_unsynchronized_threads() {
    return "dfgi";
}

const char* lab3_sequential_threads() {
    return "bcdg";
}


DWORD WINAPI thread_a(LPVOID) {
    for (int i=0; i<3; i++) {
        WaitForSingleObject(mutex, INFINITE);
        std::cout << 'a' << std::flush;
        ReleaseMutex(mutex);
        computation();
    }

    return TRUE;
}

DWORD WINAPI thread_b(LPVOID) {
    WaitForSingleObject(t[0], INFINITE);
    for (int i=0; i<3; i++) {
        WaitForSingleObject(semGtoB1, INFINITE);
        WaitForSingleObject(mutex, INFINITE);
        std::cout << 'b' << std::flush;
        ReleaseMutex(mutex);
        computation();
        ReleaseSemaphore(semBtoC1, 1, NULL);
    }

    return TRUE;
}

DWORD WINAPI thread_c(LPVOID) {
    WaitForSingleObject(t[0], INFINITE);
    for (int i=0; i<3; i++) {
        WaitForSingleObject(semBtoC1, INFINITE);
        WaitForSingleObject(mutex, INFINITE);
        std::cout << 'c' << std::flush;
        ReleaseMutex(mutex);
        computation();
        ReleaseSemaphore(semCtoD1, 1, NULL);
    }

    ReleaseSemaphore(semC1, 1, NULL);
    ReleaseSemaphore(semC1, 1, NULL);
    ReleaseSemaphore(semC1, 1, NULL);

    WaitForSingleObject(t[1], INFINITE); // b
    WaitForSingleObject(semG1, INFINITE);
    WaitForSingleObject(semD1, INFINITE);
    for (int i=0; i<3; i++) {
        WaitForSingleObject(semGtoC2, INFINITE);
        WaitForSingleObject(mutex, INFINITE);
        std::cout << 'c' << std::flush;
        ReleaseMutex(mutex);
        computation();
        ReleaseSemaphore(semCtoD2, 1, NULL);
    }

    return TRUE;
}

DWORD WINAPI thread_d(LPVOID) {
    WaitForSingleObject(t[0], INFINITE);
    for (int i=0; i<3; i++) {
        WaitForSingleObject(semCtoD1, INFINITE);
        WaitForSingleObject(mutex, INFINITE);
        std::cout << 'd' << std::flush;
        ReleaseMutex(mutex);
        computation();
        ReleaseSemaphore(semDtoG1, 1, NULL);
    }

    ReleaseSemaphore(semD1, 1, NULL);
    ReleaseSemaphore(semD1, 1, NULL);
    ReleaseSemaphore(semD1, 1, NULL);

    WaitForSingleObject(t[1], INFINITE); // b
    WaitForSingleObject(semG1, INFINITE);
    WaitForSingleObject(semC1, INFINITE);
    for (int i=0; i<3; i++) {
        WaitForSingleObject(semCtoD2, INFINITE);
        WaitForSingleObject(mutex, INFINITE);
        std::cout << 'd' << std::flush;
        ReleaseMutex(mutex);
        computation();
        ReleaseSemaphore(semDtoG2, 1, NULL);
    }

    ReleaseSemaphore(semD2, 1, NULL);
    ReleaseSemaphore(semD2, 1, NULL);
    ReleaseSemaphore(semD2, 1, NULL);

    WaitForSingleObject(t[2], INFINITE); // c
    WaitForSingleObject(t[4], INFINITE); // e
    WaitForSingleObject(semG2, INFINITE);
    for (int i=0; i<3; i++) {
        WaitForSingleObject(semGtoD3, INFINITE);
        WaitForSingleObject(mutex, INFINITE);
        std::cout << 'd' << std::flush;
        ReleaseMutex(mutex);
        computation();
        ReleaseSemaphore(semDtoG3, 1, NULL);
    }

    return TRUE;
}

DWORD WINAPI thread_e(LPVOID) {
    WaitForSingleObject(t[1], INFINITE); // b
    WaitForSingleObject(semG1, INFINITE);
    WaitForSingleObject(semD1, INFINITE);
    WaitForSingleObject(semC1, INFINITE);
    for (int i=0; i<3; i++) {
        WaitForSingleObject(mutex, INFINITE);
        std::cout << 'e' << std::flush;
        ReleaseMutex(mutex);
        computation();
    }

    return TRUE;
}

DWORD WINAPI thread_f(LPVOID) {
    WaitForSingleObject(t[2], INFINITE); // c
    WaitForSingleObject(t[4], INFINITE); // e
    WaitForSingleObject(semD2, INFINITE);
    WaitForSingleObject(semG2, INFINITE);
    for (int i=0; i<3; i++) {
        WaitForSingleObject(mutex, INFINITE);
        std::cout << 'f' << std::flush;
        ReleaseMutex(mutex);
        computation();
    }


    return TRUE;
}

DWORD WINAPI thread_g(LPVOID) {
    WaitForSingleObject(t[0], INFINITE);
    for (int i=0; i<3; i++) {
        WaitForSingleObject(semDtoG1, INFINITE);
        WaitForSingleObject(mutex, INFINITE);
        std::cout << 'g' << std::flush;
        ReleaseMutex(mutex);
        computation();
        ReleaseSemaphore(semGtoB1, 1, NULL);
    }

    ReleaseSemaphore(semG1, 1, NULL);
    ReleaseSemaphore(semG1, 1, NULL);
    ReleaseSemaphore(semG1, 1, NULL);

    WaitForSingleObject(t[1], INFINITE); // b
    WaitForSingleObject(semC1, INFINITE);
    WaitForSingleObject(semD1, INFINITE);
    for (int i=0; i<3; i++) {
        WaitForSingleObject(semDtoG2, INFINITE);
        WaitForSingleObject(mutex, INFINITE);
        std::cout << 'g' << std::flush;
        ReleaseMutex(mutex);
        computation();
        ReleaseSemaphore(semGtoC2, 1, NULL);
    }

    ReleaseSemaphore(semG2, 1, NULL);
    ReleaseSemaphore(semG2, 1, NULL);
    ReleaseSemaphore(semG2, 1, NULL);

    WaitForSingleObject(t[2], INFINITE); // c
    WaitForSingleObject(t[4], INFINITE); // e
    WaitForSingleObject(semD2, INFINITE);
    for (int i=0; i<3; i++) {
        WaitForSingleObject(semDtoG3, INFINITE);
        WaitForSingleObject(mutex, INFINITE);
        std::cout << 'g' << std::flush;
        ReleaseMutex(mutex);
        computation();
        ReleaseSemaphore(semGtoD3, 1, NULL);
    }

    ReleaseSemaphore(semG3, 1, NULL);
    ReleaseSemaphore(semG3, 1, NULL);
    ReleaseSemaphore(semG3, 1, NULL);

    WaitForSingleObject(t[3], INFINITE); // d
    WaitForSingleObject(t[5], INFINITE); // f
    WaitForSingleObject(semI3, INFINITE);
    for (int i=0; i<3; i++) {
        WaitForSingleObject(mutex, INFINITE);
        std::cout << 'g' << std::flush;
        ReleaseMutex(mutex);
        computation();
    }

    return TRUE;
}

DWORD WINAPI thread_i(LPVOID) {
    WaitForSingleObject(t[2], INFINITE); // c
    WaitForSingleObject(t[4], INFINITE); // e
    WaitForSingleObject(semD2, INFINITE);
    WaitForSingleObject(semG2, INFINITE);
    for (int i=0; i<3; i++) {
        WaitForSingleObject(mutex, INFINITE);
        std::cout << 'i' << std::flush;
        ReleaseMutex(mutex);
        computation();
    }

    ReleaseSemaphore(semI3, 1, NULL);
    ReleaseSemaphore(semI3, 1, NULL);
    ReleaseSemaphore(semI3, 1, NULL);

    WaitForSingleObject(t[3], INFINITE); // d
    WaitForSingleObject(t[5], INFINITE); // f
    WaitForSingleObject(semG3, INFINITE);
    for (int i=0; i<3; i++) {
        WaitForSingleObject(mutex, INFINITE);
        std::cout << 'i' << std::flush;
        ReleaseMutex(mutex);
        computation();
    }

    return TRUE;
}

DWORD WINAPI thread_k(LPVOID) {
    WaitForSingleObject(t[3], INFINITE); // d
    WaitForSingleObject(t[5], INFINITE); // f
    WaitForSingleObject(semG3, INFINITE);
    WaitForSingleObject(semI3, INFINITE);
    for (int i=0; i<3; i++) {
        WaitForSingleObject(mutex, INFINITE);
        std::cout << 'k' << std::flush;
        ReleaseMutex(mutex);
        computation();
    }

    return TRUE;
}

DWORD WINAPI thread_m(LPVOID) {
    WaitForSingleObject(t[3], INFINITE); // d
    WaitForSingleObject(t[5], INFINITE); // f
    WaitForSingleObject(semG3, INFINITE);
    WaitForSingleObject(semI3, INFINITE);
    for (int i=0; i<3; i++) {
        WaitForSingleObject(mutex, INFINITE);
        std::cout << 'm' << std::flush;
        ReleaseMutex(mutex);
        computation();
    }

    ReleaseSemaphore(semM4, 1, NULL);

    WaitForSingleObject(t[6], INFINITE); // g
    WaitForSingleObject(t[7], INFINITE); // i
    WaitForSingleObject(t[8], INFINITE); // k
    for (int i=0; i<3; i++) {
        WaitForSingleObject(mutex, INFINITE);
        std::cout << 'm' << std::flush;
        ReleaseMutex(mutex);
        computation();
    }

    return TRUE;
}

DWORD WINAPI thread_n(LPVOID) {
    WaitForSingleObject(t[6], INFINITE); // g
    WaitForSingleObject(t[7], INFINITE); // i
    WaitForSingleObject(t[8], INFINITE); // k
    WaitForSingleObject(semM4, INFINITE);
    for (int i=0; i<3; i++) {
        WaitForSingleObject(mutex, INFINITE);
        std::cout << 'n' << std::flush;
        ReleaseMutex(mutex);
        computation();
    }

    return TRUE;
}


int lab3_init() {

    mutex = CreateMutex(NULL, FALSE, NULL);
	if (mutex == NULL) {
		std::cerr << "CreateMutex error: " << GetLastError() << '\n';
		return 1;
	}
    semG1 = CreateSemaphore(NULL, 0, 7, NULL);
	if (semG1 == NULL) {
		std::cerr << "CreateSemaphore error: " << GetLastError() << '\n'; return 1;
	}
    semD1 = CreateSemaphore(NULL, 0, 7, NULL);
	if (semD1 == NULL) {
		std::cerr << "CreateSemaphore error: " << GetLastError() << '\n'; return 1;
	}
    semC1 = CreateSemaphore(NULL, 0, 7, NULL);
	if (semC1 == NULL) {
		std::cerr << "CreateSemaphore error: " << GetLastError() << '\n'; return 1;
	}
    semG2 = CreateSemaphore(NULL, 0, 7, NULL);
	if (semG2 == NULL) {
		std::cerr << "CreateSemaphore error: " << GetLastError() << '\n'; return 1;
	}
    semD2 = CreateSemaphore(NULL, 0, 7, NULL);
	if (semD2 == NULL) {
		std::cerr << "CreateSemaphore error: " << GetLastError() << '\n'; return 1;
	}
    semG3 = CreateSemaphore(NULL, 0, 7, NULL);
	if (semG3 == NULL) {
		std::cerr << "CreateSemaphore error: " << GetLastError() << '\n'; return 1;
	}
    semI3 = CreateSemaphore(NULL, 0, 7, NULL);
	if (semI3 == NULL) {
		std::cerr << "CreateSemaphore error: " << GetLastError() << '\n'; return 1;
	}
    semM4 = CreateSemaphore(NULL, 0, 7, NULL);
	if (semM4 == NULL) {
		std::cerr << "CreateSemaphore error: " << GetLastError() << '\n'; return 1;
	}

    //     HANDLE semGtoB1, semBtoC1, semCtoD1, semDtoG1;

    semGtoB1 = CreateSemaphore(NULL, 0, 7, NULL);
	if (semGtoB1 == NULL) {
		std::cerr << "CreateSemaphore error: " << GetLastError() << '\n'; return 1;
	}
    semBtoC1 = CreateSemaphore(NULL, 0, 7, NULL);
	if (semBtoC1 == NULL) {
		std::cerr << "CreateSemaphore error: " << GetLastError() << '\n'; return 1;
	}
    semCtoD1 = CreateSemaphore(NULL, 0, 7, NULL);
	if (semCtoD1 == NULL) {
		std::cerr << "CreateSemaphore error: " << GetLastError() << '\n'; return 1;
	}
    semDtoG1 = CreateSemaphore(NULL, 1, 7, NULL);
	if (semDtoG1 == NULL) {
		std::cerr << "CreateSemaphore error: " << GetLastError() << '\n'; return 1;
	}

    // HANDLE semGtoC2, semCtoD2, semDtoG2;

    semGtoC2 = CreateSemaphore(NULL, 0, 7, NULL);
	if (semGtoC2 == NULL) {
		std::cerr << "CreateSemaphore error: " << GetLastError() << '\n'; return 1;
	}
    semCtoD2 = CreateSemaphore(NULL, 0, 7, NULL);
	if (semCtoD2 == NULL) {
		std::cerr << "CreateSemaphore error: " << GetLastError() << '\n'; return 1;
	}
    semDtoG2 = CreateSemaphore(NULL, 1, 7, NULL);
	if (semDtoG2 == NULL) {
		std::cerr << "CreateSemaphore error: " << GetLastError() << '\n'; return 1;
	}

    // HANDLE semGtoD3, semDtoG3;

    semGtoD3 = CreateSemaphore(NULL, 0, 7, NULL);
	if (semGtoD3 == NULL) {
		std::cerr << "CreateSemaphore error: " << GetLastError() << '\n'; return 1;
	}
    semDtoG3 = CreateSemaphore(NULL, 1, 7, NULL);
	if (semDtoG3 == NULL) {
		std::cerr << "CreateSemaphore error: " << GetLastError() << '\n'; return 1;
	}

    // threads creating
    
    t[0] = CreateThread(NULL, 0, thread_a, NULL, 0, &id);
	if (t[0] == NULL) {
		std::cerr << "CreateThread error: " << GetLastError() << '\n'; return 1;
	}
    t[1] = CreateThread(NULL, 0, thread_b, NULL, 0, &id);
	if (t[1] == NULL) {
		std::cerr << "CreateThread error: " << GetLastError() << '\n'; return 1;
	}
    t[2] = CreateThread(NULL, 0, thread_c, NULL, 0, &id);
	if (t[2] == NULL) {
		std::cerr << "CreateThread error: " << GetLastError() << '\n'; return 1;
	}
    t[3] = CreateThread(NULL, 0, thread_d, NULL, 0, &id);
	if (t[3] == NULL) {
		std::cerr << "CreateThread error: " << GetLastError() << '\n'; return 1;
	}
    t[4] = CreateThread(NULL, 0, thread_e, NULL, 0, &id);
	if (t[4] == NULL) {
		std::cerr << "CreateThread error: " << GetLastError() << '\n'; return 1;
	}
    t[5] = CreateThread(NULL, 0, thread_f, NULL, 0, &id);
	if (t[5] == NULL) {
		std::cerr << "CreateThread error: " << GetLastError() << '\n'; return 1;
	}
    t[6] = CreateThread(NULL, 0, thread_g, NULL, 0, &id);
	if (t[6] == NULL) {
		std::cerr << "CreateThread error: " << GetLastError() << '\n'; return 1;
	}
    t[7] = CreateThread(NULL, 0, thread_i, NULL, 0, &id);
	if (t[7] == NULL) {
		std::cerr << "CreateThread error: " << GetLastError() << '\n'; return 1;
	}
    t[8] = CreateThread(NULL, 0, thread_k, NULL, 0, &id);
	if (t[8] == NULL) {
		std::cerr << "CreateThread error: " << GetLastError() << '\n'; return 1;
	}

    t[9] = CreateThread(NULL, 0, thread_m, NULL, 0, &id);
	if (t[9] == NULL) {
		std::cerr << "CreateThread error: " << GetLastError() << '\n'; return 1;
	}

    t[10] = CreateThread(NULL, 0, thread_n, NULL, 0, &id);
	if (t[10] == NULL) {
		std::cerr << "CreateThread error: " << GetLastError() << '\n'; return 1;
	}

    WaitForSingleObject(t[9], INFINITE);
    WaitForSingleObject(t[10], INFINITE);

    // Semaphores for async threads
    // HANDLE semG1, semD1, semC1;
    // HANDLE semG2, semD2;
    // HANDLE semG3, semI3;
    // HANDLE semM4;
    // semaphores for seq threads
    // HANDLE semGtoB1, semBtoC1, semCtoD1, semDtoG1;
    // HANDLE semGtoC2, semCtoD2, semDtoG2;
    // HANDLE semGtoD3, semDtoG3;

    for (int i = 0; i < 11; i++) {
        CloseHandle(t[i]);
    }

    CloseHandle(semG1);
    CloseHandle(semD1);
    CloseHandle(semC1);
    CloseHandle(semG2);
    CloseHandle(semD2);
    CloseHandle(semG3);
    CloseHandle(semI3);   
    CloseHandle(semM4);

    CloseHandle(semGtoB1);
    CloseHandle(semBtoC1);
    CloseHandle(semCtoD1);
    CloseHandle(semDtoG1);
    CloseHandle(semGtoC2);
    CloseHandle(semCtoD2);
    CloseHandle(semDtoG2);   
    CloseHandle(semGtoD3);    
    CloseHandle(semDtoG3);    
             
    return 0;
}