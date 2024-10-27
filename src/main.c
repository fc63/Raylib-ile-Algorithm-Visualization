#include "resource_dir.h"
#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>

#define WIDTH 800
#define HEIGHT 600
#define BACKGROUND_COLOR (Color){ 30, 30, 30, 255 }
#define BAR_COLOR (Color){ 70, 130, 180, 255 }
#define HIGHLIGHT_COLOR (Color){ 255, 69, 0, 255 }
#define TEXT_COLOR (Color){ 255, 255, 255, 255 }
#define CODE_HIGHLIGHT_COLOR (Color){ 173, 216, 230, 255 }
#define BAR_WIDTH 80

typedef struct {
    int array[7];
    int codeLine;
    int i, j;
    bool swapped;
}
Step;

Step steps[100];
int numSteps = 0;

const char* codeLines[] = {
  "for (i = 0; i < n-1; i++) {",
  "    swapped = false;",
  "    for (j = 0; j < n-1-i; j++) {",
  "        if (arr[j] > arr[j+1]) {",
  "            swap(&arr[j], &arr[j+1]);",
  "            swapped = true;",
  "        }",
  "    }",
  "    if (swapped == false)",
  "        break;",
  "}"
};

void RecordStep(int* array, int codeLine, int i, int j, bool swapped) {
    for (int k = 0; k < 7; k++) {
        steps[numSteps].array[k] = array[k];
    }
    steps[numSteps].codeLine = codeLine;
    steps[numSteps].i = i;
    steps[numSteps].j = j;
    steps[numSteps].swapped = swapped;
    numSteps++;
}

void BubbleSort(int* array) {
    int n = 7;
    for (int i = 0; i < n - 1; i++) {
        RecordStep(array, 0, i, -1, false);
        bool swapped = false;
        RecordStep(array, 1, i, -1, swapped);

        for (int j = 0; j < n - 1 - i; j++) {
            RecordStep(array, 2, i, j, swapped);

            if (array[j] > array[j + 1]) {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;

                swapped = true;

                RecordStep(array, 4, i, j, swapped);
            }
        }

        if (!swapped) {
            RecordStep(array, 8, i, -1, swapped);
            break;
        }
    }
}

void DrawArray(int* array, int highlightIdx, int activeCodeLine, int i, int j, bool swapped) {
    ClearBackground(BACKGROUND_COLOR);

    for (int idx = 0; idx < 10; idx++) {

        Color color = ((idx == activeCodeLine) || (activeCodeLine == 4 && idx == 5)) ? CODE_HIGHLIGHT_COLOR : TEXT_COLOR;
        DrawText(codeLines[idx], 10, idx * 30 + 10, 20, color);
    }

    for (int index = 0; index < 7; index++) {
        int x = WIDTH - (7 - index) * BAR_WIDTH;
        int y = HEIGHT - array[index] * 5;
        Color color = (index == highlightIdx) ? HIGHLIGHT_COLOR : BAR_COLOR;
        DrawRectangle(x, y, BAR_WIDTH - 2, array[index] * 5, color);
        DrawText(TextFormat("%d", array[index]), x + BAR_WIDTH / 4, y - 20, 20, TEXT_COLOR);
    }

    DrawText(TextFormat("i = %d, j = %d, swapped = %s", i, j, swapped ? "true" : "false"), WIDTH - 300, 20, 20, TEXT_COLOR);
}

int main(void) {

    InitWindow(WIDTH, HEIGHT, "Bubble Sort Visualization with C Raylib");
    SetTargetFPS(60);

    int array[] = {
      64,
      34,
      25,
      12,
      22,
      11,
      90
    };
    BubbleSort(array);

    int currentStep = 0;
    bool paused = true;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) paused = !paused;
        if (paused && IsKeyPressed(KEY_RIGHT) && currentStep < numSteps - 1) currentStep++;
        if (paused && IsKeyPressed(KEY_LEFT) && currentStep > 0) currentStep--;

        if (!paused && currentStep < numSteps - 1) {
            currentStep++;
#ifdef _WIN32
            Sleep(500);
#else
            usleep(500000);
#endif
        }

        Step step = steps[currentStep];
        DrawArray(step.array, step.j, step.codeLine, step.i, step.j, step.swapped);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}