#include "raylib.h"

typedef enum { 
    STATE_SPLASH_CAPCOM, 
    STATE_SPLASH_ENGINE, 
    STATE_MAIN_MENU, 
    STATE_SETTINGS, 
    STATE_GAMEPLAY 
} GameState;

// إعدادات البلايستيشن 4 العادي (طبق الأصل)
typedef struct {
    int brightness;      // سطوع
    bool motionBlur;     // ضبابية الحركة
    bool filmGrain;      // حبيبات الفيلم
    bool lensFlare;      // انعكاس الضوء
    bool depthOfField;   // عمق الميدان
    bool ambientOcclusion; // الإطباق المحيط
} PS4Settings;

int main(void) {
    // دقة 720p (مقربة من 900p الخاصة بـ PS4 لتلائم PS3)
    InitWindow(1280, 720, "RE4 Remake - Base PS4 Settings Replica");
    
    GameState currentState = STATE_SPLASH_CAPCOM;
    
    // الإعدادات الافتراضية كما هي في PS4
    PS4Settings settings = { 50, true, true, true, true, true };
    
    float splashTimer = 2.0f; 
    int menuSelection = 0;
    
    // نظام التبويبات (مثل PS4: Display, Graphics, Camera, Controls)
    int currentTab = 1; // نبدأ في تبويب Graphics مثل الصورة
    const char* tabs[] = { "DISPLAY", "GRAPHICS", "CAMERA", "CONTROLS" };
    int numTabs = 4;
    
    int settingsSelection = 0;
    int maxSelection = 6; // عدد الخيارات في تبويب Graphics

    // تحميل ليون
    Model leon = LoadModel("leon.glb");
    Vector3 leonPos = { 0.0f, 0.0f, 0.0f };
    
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 1.5f, 2.0f, -4.0f }; 
    camera.target = leonPos;
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f; 
    camera.projection = CAMERA_PERSPECTIVE;

    RenderTexture2D canvas = LoadRenderTexture(1280, 720);

    while (!WindowShouldClose()) {
        
        // --- منطق الشعارات ---
        if (currentState >= STATE_SPLASH_CAPCOM && currentState <= STATE_SPLASH_ENGINE) {
            splashTimer -= GetFrameTime();
            if (splashTimer <= 0 || IsKeyPressed(KEY_ENTER)) {
                currentState++;
                splashTimer = 2.0f;
            }
        }
        // --- القائمة الرئيسية ---
        else if (currentState == STATE_MAIN_MENU) {
            if (IsKeyPressed(KEY_DOWN)) menuSelection = (menuSelection + 1) % 3;
            if (IsKeyPressed(KEY_UP)) menuSelection = (menuSelection - 1 + 3) % 3;
            
            if (IsKeyPressed(KEY_ENTER)) {
                if (menuSelection == 0) currentState = STATE_GAMEPLAY;
                if (menuSelection == 1) currentState = STATE_SETTINGS;
                if (menuSelection == 2) break;
            }
        }
        // --- قائمة الإعدادات (طبق الأصل PS4) ---
        else if (currentState == STATE_SETTINGS) {
            if (IsKeyPressed(KEY_ESCAPE)) currentState = STATE_MAIN_MENU;
            
            // التنقل بين التبويبات
            if (IsKeyPressed(KEY_RIGHT)) currentTab = (currentTab + 1) % numTabs;
            if (IsKeyPressed(KEY_LEFT)) currentTab = (currentTab - 1 + numTabs) % numTabs;
            
            // التنقل بين الخيارات
            if (IsKeyPressed(KEY_DOWN)) settingsSelection = (settingsSelection + 1) % maxSelection;
            if (IsKeyPressed(KEY_UP)) settingsSelection = (settingsSelection - 1 + maxSelection) % maxSelection;

            // تغيير القيم (باستخدام الأسهم A و D)
            if (IsKeyPressed(KEY_D)) {
                if (currentTab == 0) { // Display
                    if (settingsSelection == 0) settings.brightness += 5; if (settings.brightness > 100) settings.brightness = 100;
                } else if (currentTab == 1) { // Graphics
                    if (settingsSelection == 1) settings.motionBlur = !settings.motionBlur;
                    if (settingsSelection == 2) settings.filmGrain = !settings.filmGrain;
                    if (settingsSelection == 3) settings.lensFlare = !settings.lensFlare;
                    if (settingsSelection == 4) settings.depthOfField = !settings.depthOfField;
                    if (settingsSelection == 5) settings.ambientOcclusion = !settings.ambientOcclusion;
                }
            }
            if (IsKeyPressed(KEY_A)) {
                if (currentTab == 0) { // Display
                    if (settingsSelection == 0) settings.brightness -= 5; if (settings.brightness < 0) settings.brightness = 0;
                } else if (currentTab == 1) { // Graphics
                    if (settingsSelection == 1) settings.motionBlur = !settings.motionBlur;
                    if (settingsSelection == 2) settings.filmGrain = !settings.filmGrain;
                    if (settingsSelection == 3) settings.lensFlare = !settings.lensFlare;
                    if (settingsSelection == 4) settings.depthOfField = !settings.depthOfField;
                    if (settingsSelection == 5) settings.ambientOcclusion = !settings.ambientOcclusion;
                }
            }
        }
        // --- اللعب ---
        else if (currentState == STATE_GAMEPLAY) {
            if (IsKeyPressed(KEY_ESCAPE)) currentState = STATE_MAIN_MENU;
            if (IsKeyDown(KEY_W)) leonPos.z += 0.1f;
            if (IsKeyDown(KEY_S)) leonPos.z -= 0.1f;
        }

        camera.target = leonPos;
        camera.position.x = leonPos.x + 1.5f;
        camera.position.z = leonPos.z - 4.0f;

        // --- الرسم ---
        BeginDrawing();
        ClearBackground(BLACK);

        if (currentState == STATE_SPLASH_CAPCOM) {
            DrawText("CAPCOM", GetScreenWidth()/2 - MeasureText("CAPCOM", 80)/2, GetScreenHeight()/2 - 40, 80, RED);
        }
        else if (currentState == STATE_SPLASH_ENGINE) {
            DrawText("RE ENGINE", GetScreenWidth()/2 - MeasureText("RE ENGINE", 60)/2, GetScreenHeight()/2 - 30, 60, WHITE);
        }
        else if (currentState == STATE_MAIN_MENU) {
            DrawText("RESIDENT EVIL 4", 100, 100, 60, RED);
            const char* options[] = { "NEW GAME", "SETTINGS", "EXIT" };
            for (int i = 0; i < 3; i++) {
                Color color = (menuSelection == i) ? WHITE : DARKGRAY;
                DrawText(options[i], 100, 300 + (i * 50), 30, color);
            }
        }
        else if (currentState == STATE_SETTINGS) {
            // خلفية رمادية داكنة
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){ 15, 15, 15, 255 });
            
            // --- شريط التبويبات (أعلى الشاشة) ---
            int tabX = 100;
            for (int i = 0; i < numTabs; i++) {
                Color tabColor = (currentTab == i) ? WHITE : GRAY;
                DrawText(tabs[i], tabX, 50, 18, tabColor);
                if (currentTab == i) {
                    // خط أبيض تحت التبويب النشط
                    DrawRectangle(tabX, 75, MeasureText(tabs[i], 18), 2, WHITE);
                }
                tabX += MeasureText(tabs[i], 18) + 40;
            }

            // --- محتوى التبويبات ---
            int yStart = 160;
            int xText = 150;
            int xValue = 600;

            if (currentTab == 0) { // Display
                DrawText("Screen Resolution", xText, yStart, 20, GRAY);
                DrawText("1920 x 1080", xValue, yStart, 20, WHITE);
                
                DrawText("Brightness", xText, yStart + 50, 20, (settingsSelection == 0) ? WHITE : GRAY);
                DrawText(TextFormat("< %i >", settings.brightness), xValue, yStart + 50, 20, WHITE);
            }
            else if (currentTab == 1) { // Graphics
                DrawText("Anti-Aliasing", xText, yStart, 20, GRAY);
                DrawText("FXAA", xValue, yStart, 20, WHITE); // مقفل في PS4

                DrawText("Motion Blur", xText, yStart + 40, 20, (settingsSelection == 1) ? WHITE : GRAY);
                DrawText(TextFormat("< %s >", settings.motionBlur ? "On" : "Off"), xValue, yStart + 40, 20, WHITE);

                DrawText("Film Grain", xText, yStart + 80, 20, (settingsSelection == 2) ? WHITE : GRAY);
                DrawText(TextFormat("< %s >", settings.filmGrain ? "On" : "Off"), xValue, yStart + 80, 20, WHITE);

                DrawText("Lens Flare", xText, yStart + 120, 20, (settingsSelection == 3) ? WHITE : GRAY);
                DrawText(TextFormat("< %s >", settings.lensFlare ? "On" : "Off"), xValue, yStart + 120, 20, WHITE);

                DrawText("Depth of Field", xText, yStart + 160, 20, (settingsSelection == 4) ? WHITE : GRAY);
                DrawText(TextFormat("< %s >", settings.depthOfField ? "On" : "Off"), xValue, yStart + 160, 20, WHITE);

                DrawText("Ambient Occlusion", xText, yStart + 200, 20, (settingsSelection == 5) ? WHITE : GRAY);
                DrawText(TextFormat("< %s >", settings.ambientOcclusion ? "On" : "Off"), xValue, yStart + 200, 20, WHITE);
            }

            // --- الصندوق التوضيحي (Preview) على اليمين ---
            DrawRectangle(850, 150, 350, 200, (Color){ 40, 30, 20, 255 }); // خلفية الغرفة
            DrawText("Preview", 980, 240, 20, DARKGRAY);
            
            // مربع الإضاءة (محاكاة سطوع)
            if (currentTab == 0) {
                DrawRectangle(850, 150, 350, 200, Fade((Color){ 255, 200, 100, 255 }, settings.brightness / 100.0f));
            }

            // تعليمات أسفل الشاشة
            DrawText("Use ARROWS to navigate, ENTER to select, ESC to go back", 100, GetScreenHeight() - 50, 15, DARKGRAY);
        }
        else if (currentState == STATE_GAMEPLAY) {
            BeginTextureMode(canvas);
                ClearBackground((Color){ 20, 20, 20, 255 }); 
                // إضاءة البلايستيشن 4 (بدون Ray Tracing)
                SetTextureFilter(canvas.texture, TEXTURE_FILTER_BILINEAR); 
                
                BeginMode3D(camera);
                    DrawModel(leon, leonPos, 1.0f, WHITE);
                    DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 100.0f, 100.0f }, (Color){ 30, 30, 30, 255 });
                    if (settings.ambientOcclusion) {
                        DrawCircle3D((Vector3){ leonPos.x, 0.01f, leonPos.z }, 1.0f, (Vector3){ 1, 0, 0 }, 90.0f, Fade(BLACK, 0.8f));
                    }
                EndMode3D();
            EndTextureMode();

            Rectangle src = { 0.0f, 0.0f, (float)canvas.texture.width, (float)-canvas.texture.height };
            Rectangle dst = { 0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight() };
            DrawTexturePro(canvas.texture, src, dst, (Vector2){ 0, 0 }, 0.0f, WHITE);

            DrawText(TextFormat("FPS: %i", GetFPS()), 10, 10, 20, GREEN);
            DrawText("Press ESC to return to menu", 10, 40, 20, GRAY);
        }

        EndDrawing();
    }

    UnloadRenderTexture(canvas);
    UnloadModel(leon);
    CloseWindow();
    return 0;
}
