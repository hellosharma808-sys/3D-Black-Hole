
#include <raylib.h>
#include <raymath.h>
#include <vector>

struct black_hole {
    double mass;
    double event_horizon;
    Vector3 position;
};

struct ray_pixel {
    Vector3 position;
    Vector3 velocity;
    Color pixel_color;
    bool is_done;
};

int main() {
    int screenWidth = 150;
    int screenHeight = 150;
    InitWindow(500, 500, "3D Black Hole Ray Tracer");
    SetTargetFPS(60);

    Camera3D camera = { 0 };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    black_hole bh;
    bh.position = (Vector3){ 0.0f, 0.0f, 0.0f };
    bh.event_horizon = 2.0f;
    bh.mass = 3.5f;

    Image screenImage = GenImageColor(screenWidth, screenHeight, BLACK);
    Texture2D screenTexture = LoadTextureFromImage(screenImage);

    float cameraRadius = 25.0f;
    float camera_y = 0.0f;
    float camera_pinch = 0.2f;

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))  camera_y -= 0.04f;
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) camera_y += 0.04f;
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))    camera_pinch += 0.04f;
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))  camera_pinch -= 0.04f;

        if (camera_pinch > 1.5f) camera_pinch = 1.5f;
        if (camera_pinch < -1.5f) camera_pinch = -1.5f;

        cameraRadius -= GetMouseWheelMove() * 2.0f;
        if (cameraRadius < 5.0f) cameraRadius = 5.0f;

        camera.position.x = bh.position.x + cameraRadius * cosf(camera_pinch) * sinf(camera_y);
        camera.position.y = bh.position.y + cameraRadius * sinf(camera_pinch);
        camera.position.z = bh.position.z + cameraRadius * cosf(camera_pinch) * cosf(camera_y);

        Color *pixels = (Color *)screenImage.data;

        Matrix viewMatrix = MatrixLookAt(camera.position, camera.target, camera.up);
        Matrix invViewMatrix = MatrixInvert(viewMatrix);

        float fovRad = camera.fovy * DEG2RAD;
        float halfHeight = tanf(fovRad / 2.0f);
        float halfWidth = halfHeight * ((float)screenWidth / (float)screenHeight);

        for (int y = 0; y < screenHeight; y++) {
            for (int x = 0; x < screenWidth; x++) {
                float px = (2.0f * ((float)x + 0.5f) / (float)screenWidth - 1.0f) * halfWidth;
                float py = (1.0f - 2.0f * ((float)y + 0.5f) / (float)screenHeight) * halfHeight;

                Vector3 rayDir = (Vector3){ px, py, -1.0f };
                rayDir = Vector3Transform(rayDir, invViewMatrix);
                rayDir = Vector3Subtract(rayDir, camera.position);
                rayDir = Vector3Normalize(rayDir);

                ray_pixel rp;
                rp.position = camera.position;
                rp.velocity = Vector3Scale(rayDir, 0.8f);
                rp.pixel_color = BLACK;
                rp.is_done = false;

                for (int step = 0; step < 50; step++) {
                    double dx = bh.position.x - rp.position.x;
                    double dy = bh.position.y - rp.position.y;
                    double dz = bh.position.z - rp.position.z;
                    double distance = sqrt(dx * dx + dy * dy + dz * dz);

                    if (distance < bh.event_horizon) {
                        rp.pixel_color = BLACK;
                        rp.is_done = true;
                        break;
                    }

                    if (distance > 40.0f) {
                        rp.pixel_color = (Color){ 15, 15, 25, 255 };
                        rp.is_done = true;
                        break;
                    }

                    double force = bh.mass / (distance * distance);
                    rp.velocity.x += (dx / distance) * force * 0.8f;
                    rp.velocity.y += (dy / distance) * force * 0.8f;
                    rp.velocity.z += (dz / distance) * force * 0.8f;

                    Vector3 prev_pos = rp.position;
                    rp.position.x += rp.velocity.x;
                    rp.position.y += rp.velocity.y;
                    rp.position.z += rp.velocity.z;

                    if ((prev_pos.y > 0.0f && rp.position.y <= 0.0f) || (prev_pos.y < 0.0f && rp.position.y >= 0.0f)) {
                        float t = -prev_pos.y / (rp.position.y - prev_pos.y);
                        float ix = prev_pos.x + t * (rp.position.x - prev_pos.x);
                        float iz = prev_pos.z + t * (rp.position.z - prev_pos.z);
                        double flat_dist = sqrt(ix * ix + iz * iz);

                        if (flat_dist > 4.0f && flat_dist < 11.0f) {
                            rp.pixel_color = ORANGE;
                            rp.is_done = true;
                            break;
                        }
                    }
                }

                if (!rp.is_done) {
                    rp.pixel_color = (Color){ 15, 15, 25, 255 };
                }

                pixels[y * screenWidth + x] = rp.pixel_color;
            }
        }

        UpdateTexture(screenTexture, screenImage.data);

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(screenTexture, (Rectangle){ 0, 0, (float)screenWidth, (float)screenHeight }, (Rectangle){ 0, 0, 500, 500 }, (Vector2){ 0, 0 }, 0.0f, WHITE);
        DrawFPS(10, 10);
        EndDrawing();
    }

    UnloadImage(screenImage);
    UnloadTexture(screenTexture);
    CloseWindow();
    return 0;
}
