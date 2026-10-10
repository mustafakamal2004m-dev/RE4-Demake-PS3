#include <tiny3d.h>
#include <io/pad.h>
#include <sysutil/sysutil.h>
#include <math.h>

// دالة لرسم مكعب بسيط (سنطورها لاحقاً)
void draw_cube(float x, float y, float z, float size, u32 color) {
    tiny3d_DrawPoint(x, y, z, color);
}

int main() {
    tiny3d_init(0);
    tiny3d_SetLight(0, 0, 0, 0, 1.0f, 1.0f, 1.0f);
    
    while(1) {
        tiny3d_Clear(0x00000000);
        draw_cube(0, 0, 0, 1.0f, 0xFF00FF00);
        tiny3d_Flip();
        
        padInfo padinfo;
        ioPadGetInfo(&padinfo);
        if(padinfo.status[0]) {
            break;
        }
    }
    
    tiny3d_Exit();
    return 0;
}
