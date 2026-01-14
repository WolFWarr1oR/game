#include <raylib.h>
#include <limits.h>
#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

#define PARTCILE_RADIUS 35
#define PARTCILE_COLOR BLUE
#define MAX_PARTICLES 25
#define PARTCILE_BORDER LIGHTGRAY
#define LINE_COLOR PURPLE
#define PARTICLE_DURATION 5

typedef struct {
    Vector2 position;
    double start_time;
    double finish_time;
} Particle;

typedef struct {
    int prev;
    int peak_fps;
    Particle particles[MAX_PARTICLES];
    bool used_particles[MAX_PARTICLES];
} Gamedata;

Particle create_particle(Vector2 position, double duration)
{
    double current_time = GetTime();
    Particle particle;
    particle.position = position;
    particle.start_time = current_time;
    particle.finish_time = current_time + duration;
    return particle;
}

void draw_particles(Gamedata *gamedata)
{
    double remained_time;
    double duration;
    double transparency;
    Vector2 current_pos;
    Vector2 previous_pos;

    for (int i = 0; i < MAX_PARTICLES; ++i) {
	if (gamedata->used_particles[i]) {
	    Particle particle = gamedata->particles[i];

	    remained_time = particle.finish_time - GetTime();
	    duration = particle.finish_time - particle.start_time;
	    transparency = remained_time / duration;
	    current_pos = particle.position;

	    Color faded_color = Fade(PARTCILE_COLOR, transparency);
	    Color faded_border = Fade(PARTCILE_BORDER, transparency);
	    DrawCircleV(current_pos, PARTCILE_RADIUS, faded_color);
	    DrawRing(current_pos, PARTCILE_RADIUS, PARTCILE_RADIUS+3, 0, 360, 360, faded_border);

	    if (gamedata->prev-1 < 0 && gamedata->prev-1 == i) continue;
	    previous_pos = gamedata->particles[gamedata->prev-1].position;
	    DrawLineEx(current_pos, previous_pos, 2.0f, LINE_COLOR);
	    gamedata->prev = i+1;
	}
    }
}

int get_next_free_particle(Gamedata *gamedata)
{
    int retval = -1;
    for (int i = 0; i<MAX_PARTICLES;++i) {
	if(!gamedata->used_particles[i]) {
	    retval = i;
	    break;
	}
    }
    if (retval == -1) TraceLog(LOG_INFO, "There is no free particle");
    return retval;
}

void clear_dead_particles(Gamedata *gamedata)
{
    for (int i = 0; i < MAX_PARTICLES; ++i) {
	if(gamedata->used_particles[i]) {
	    Particle p = gamedata->particles[i];
	    if (p.finish_time < GetTime()) {
		gamedata->used_particles[i]=false;
	    }
	}
    }
}

void gameloop(Gamedata *gamedata)
{
    int fps = GetFPS();
    if (fps > gamedata->peak_fps) {
	gamedata->peak_fps = fps;
    }

    const char *FPSText = TextFormat("FPS: %d", fps);
    const char *PeakFPSText = TextFormat("PEAK = %d", gamedata->peak_fps);
    DrawText(FPSText, 2, 0, 30, BLACK);
    DrawText(PeakFPSText, 2, 30, 30, BLACK);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
	Vector2 mouse_position = GetMousePosition();
	int index = get_next_free_particle(gamedata);
	Particle p = create_particle(mouse_position, PARTICLE_DURATION);
	if (index >= 0) {
	    gamedata->used_particles[index] = true;
	    gamedata->particles[index] = p;
	}
    }
    clear_dead_particles(gamedata);
    draw_particles(gamedata);
}


void UpdateGameFrame(void *arg)
{
    Gamedata *gamedata = (Gamedata*)arg;
    BeginDrawing();
    ClearBackground(WHITE);
    gameloop(gamedata);
    EndDrawing();
}

int main(void)
{
    Gamedata gamedata = {0};
    gamedata.peak_fps = 0;

    InitWindow(1280, 700, "OwOlfwarrior");

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop_arg(UpdateGameFrame, &gamedata, INT_MAX, 1);
#else
    while (!WindowShouldClose()) {
	UpdateGameFrame(&gamedata);
    }
#endif
    CloseWindow();
    TraceLog(LOG_INFO, "Goodbye! uwu");
    return 0;
}
