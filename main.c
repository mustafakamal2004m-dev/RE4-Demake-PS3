#include <stdio.h>
#include <unistd.h>
#include <io/pad.h>
#include <sysutil/sysutil.h>
#include <tiny3d.h>

int main() {
    // تهيئة Tiny3D
    tiny3d_init(0);
    ioPadInit(0);
    sysUtilInit();
    
    printf("RE4 Demake Started!\n");
    
    // إعداد الإضاءة (فانوس برتقالي)
    tiny3d_SetLight(0, 0, 2.5, 0, 1.0f, 0.6f, 0.2f);
    
    // حلقة اللعبة
    while(1) {
        tiny3d_Clear(0x00000000); // شاشة سوداء
        
        // هنا نرسم الغرفة، ليون، الزومبي (لاحقاً)
        
        tiny3d_Flip();
        
        // التحقق من الإدخال (للخروج)
        padInfo padinfo;
        ioPadGetInfo(&padinfo);
        if(padinfo.status[0]) break;
        
        usleep(16000); // 60 إطار في الثانية
    }
    
    tiny3d_Exit();
    ioPadEnd();
    sysUtilExit();
    return 0;
}
