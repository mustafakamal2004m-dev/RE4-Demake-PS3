#include <stdio.h>
#include <unistd.h>
#include <io/pad.h>
#include <sysutil/sysutil.h>

int main() {
    ioPadInit(0);
    sysUtilInit();
    printf("RE4 Demake Started!\n");
    
    while(1) {
        padInfo padinfo;
        ioPadGetInfo(&padinfo);
        if(padinfo.status[0]) {
            break;
        }
        usleep(100000);
    }
    
    sysUtilExit();
    ioPadEnd();
    return 0;
}
