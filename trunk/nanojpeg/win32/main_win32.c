#include <windows.h>

#define _NJ_INCLUDE_HEADER_ONLY
#include "nanojpeg.c"

static char cmdline[1024];
static char header[20] = "P5\n    0     0\n255\n";

static void putnum(char *c, int num) {
    do {
        *c-- = '0' + (num % 10);
    } while ((num /= 10));
}

#pragma code_seg(".main")  // helps Crinkler a bit

void mainCRTStartup(void) {
    char *argv[3], *buf = GetCommandLine();
    enum { S_WHITESPACE, S_PARAM, S_QUOTE } state = S_WHITESPACE;
    int argc, size, esize;
    HANDLE f;
    for (argc = 0;  (argc < 1022) && buf[argc];  ++argc)
        cmdline[argc] = buf[argc];
    cmdline[argc] = 0;
    for (buf = cmdline, argc = 0;  *buf;  ++buf) {
        switch (state) {
            case S_WHITESPACE:
                if (buf[0] == 32)
                    buf[0] = 0;
                else {
                    if (argc >= 3) ExitProcess(2);
                    if (buf[0] == 34) {
                        argv[argc++] = buf + 1;
                        state = S_QUOTE;
                    } else {
                        argv[argc++] = buf;
                        state = S_PARAM;
                    }
                }
                break;
            case S_PARAM:
                if (buf[0] == 32) {
                    buf[0] = 0;
                    state = S_WHITESPACE;
                } else if (buf[0] == 34) {
                    argv[argc - 1] = buf + 1;
                    state = S_QUOTE;
                }
                break;
            case S_QUOTE:
                if (buf[0] == 34) {
                    buf[0] = 0;
                    state = S_WHITESPACE;
                }
                break;
        }
    }
    if (argc < 2) ExitProcess(2);

    f = CreateFile(argv[1], GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (f == INVALID_HANDLE_VALUE) ExitProcess(1);
    size = (int) SetFilePointer(f, 0, NULL, FILE_END);
    buf = LocalAlloc(LMEM_FIXED, size);
    if (!buf) ExitProcess(1);
    SetFilePointer(f, 0, NULL, FILE_BEGIN);
    ReadFile(f, buf, size, (LPDWORD) &esize, NULL);
    CloseHandle(f);

    njInit();
    if (njDecode(buf, esize)) ExitProcess(1);
    f = CreateFile((argc > 2) ? argv[2] : (njIsColor() ? "nanojpeg_out.ppm" : "nanojpeg_out.pgm"),
        GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
    if (f == INVALID_HANDLE_VALUE) ExitProcess(1);
    if (njIsColor()) header[1] = '6';
    putnum(&header[7], njGetWidth());
    putnum(&header[13], njGetHeight());
    WriteFile(f, header, 19, (LPDWORD) &esize, NULL);
    WriteFile(f, njGetImage(), njGetImageSize(), (LPDWORD) &esize, NULL);
    njDone();

    ExitProcess(0);
}
