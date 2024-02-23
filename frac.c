#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <raylib.h>

#define DA_INIT_CAP 2

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400

#define VIRTUAL_WIDTH 1000
#define VIRTUAL_HEIGHT 1000

#define da_append(da, item)                                                          \
    do {                                                                             \
        if ((da)->count >= (da)->capacity) {                                         \
            (da)->capacity = (da)->capacity == 0 ? DA_INIT_CAP : (da)->capacity*2;   \
            (da)->items = realloc((da)->items, (da)->capacity*sizeof(*(da)->items)); \
            assert((da)->items != NULL && "Buy more RAM lol");                       \
        }                                                                            \
                                                                                     \
        (da)->items[(da)->count++] = (item);                                         \
    } while (0)

void draw_triangle_at_rec(const Rectangle rec) {
	DrawTriangleLines(
		(Vector2){rec.x, rec.y + rec.height},
		(Vector2){rec.x + rec.width / 2.0, rec.y},
		(Vector2){rec.x + rec.width, rec.y + rec.height},
		RAYWHITE
	);
}

void divide_rec(const Rectangle rec, Rectangle *out_recs) {
	out_recs[0] = (Rectangle){
		.x = rec.x, .y = rec.y + rec.height / 2,
		.width = rec.width / 2, .height = rec.height / 2,
	};
	out_recs[1] = (Rectangle){
		.x = rec.x + rec.width / 2, .y = rec.y + rec.height / 2,
		.width = rec.width / 2, .height = rec.height / 2,
	};
	out_recs[2] = (Rectangle){
		.x = rec.x + rec.width / 4, .y = rec.y,
		.width = rec.width / 2, .height = rec.height / 2,
	};
}

struct node {
	Rectangle rec;
	size_t level;
};

struct {
	size_t count, capacity;
	Rectangle *items;
} draw_recs = { 0 };

void gen_recs(Rectangle rec) {
	Rectangle out_recs[3] = {0};

	struct {
		size_t count, capacity;
		struct node *items;
	} stack = { 0 };


	struct node n = {rec, 0};
	da_append(&stack, n);

	while (stack.count != 0) {
		stack.count--;
		struct node curr = stack.items[stack.count];

		divide_rec(curr.rec, out_recs);

		for (int i = 0; i < 3; i++) {
			struct node temp;
			temp.rec = out_recs[i];
			temp.level = curr.level + 1;

			if (temp.rec.width <= 7.0) {
				da_append(&draw_recs, temp.rec);
				continue;
			}

			da_append(&stack, temp);
		}
	}

}

int main(void) {
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "frac");
	SetWindowState(FLAG_WINDOW_RESIZABLE);                    // Set window configuration state using flags (only PLATFORM_DESKTOP)

	Rectangle rec = (Rectangle){
		.x = 10, .y = 10,
		.width = VIRTUAL_WIDTH - 20, .height = VIRTUAL_HEIGHT - 20,
	};

	gen_recs(rec);

	float window_w = WINDOW_WIDTH;
	float window_h = WINDOW_HEIGHT;

	while (!WindowShouldClose()) {
		if (IsWindowResized()) {
			window_w = GetRenderWidth();
			window_h = GetRenderHeight();
		}

		ClearBackground(BLACK);
		DrawFPS(0, 0);
		BeginDrawing();
		for (size_t i = 0; i < draw_recs.count; i++) {
			Rectangle rec = draw_recs.items[i];

			float h_ratio = window_w / VIRTUAL_WIDTH;
			float v_ratio = window_h / VIRTUAL_HEIGHT;

			rec.x *= h_ratio;
			rec.width *= h_ratio;
			rec.y *= v_ratio;
			rec.height *= v_ratio;
			draw_triangle_at_rec(rec);
		}
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
