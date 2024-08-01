#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 25
#define FILED 64
#define CELL 1
#define DEAD_CELL 0

void read_file(int play_filed[HEIGHT][WIDTH]);
void update(int play_filed[HEIGHT][WIDTH]);
int count_neighbors(int play_filed[HEIGHT][WIDTH], int i, int j);
void check_neighbors(int (*play_filed)[WIDTH], int (*new_play_filed)[WIDTH], int neighbors, int i, int j);
void draw_world(int play_filed[HEIGHT][WIDTH]);
void speed(const char *key_press, int *speed);

int main() {
    int play_filed[HEIGHT][WIDTH];
    char key_press = '-';
    int drawing_frame = 0;
    read_file(play_filed);
    FILE *f = freopen("/dev/tty", "r", stdin);
    system("stty -icanon min 0 time 1 -echo");

    while (key_press != 'q') {
        system("clear");
        draw_world(play_filed);
        printf("rules: press f to increase the speed and l to decrease the speed and q to quit\n");
        update(play_filed);
        usleep(drawing_frame);
        speed(&key_press, &drawing_frame);
        read(STDIN_FILENO, &key_press, 1);
    }
    system("stty sane");
    fclose(f);
    return 0;
}

void read_file(int play_filed[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH + 1; j++) {
            char c;
            c = getchar();
            if (c == 111) {
                play_filed[i][j] = 1;
            } else {
                play_filed[i][j] = 0;
            }
        }
    }
}

void update(int play_filed[HEIGHT][WIDTH]) {
    int new_play_filed[HEIGHT][WIDTH];

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int neighbors = count_neighbors(play_filed, i, j);
            check_neighbors(play_filed, new_play_filed, neighbors, i, j);
        }
    }

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            play_filed[i][j] = new_play_filed[i][j];
        }
    }
}

int count_neighbors(int play_filed[HEIGHT][WIDTH], int i, int j) {
    int neighbors = 0;

    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            if (x == 0 && y == 0) continue;
            int neighbor_i = (i + x + HEIGHT) % (HEIGHT);
            int neighbor_j = (j + y + WIDTH) % (WIDTH);
            neighbors += play_filed[neighbor_i][neighbor_j];
        }
    }
    return neighbors;
}

void check_neighbors(int (*play_filed)[WIDTH], int (*new_play_filed)[WIDTH], int neighbors, int i, int j) {
    if (play_filed[i][j] == 1) {
        if (neighbors < 2 || neighbors > 3) {
            new_play_filed[i][j] = DEAD_CELL;
        } else {
            new_play_filed[i][j] = CELL;
        }
    } else {
        if (neighbors == 3) {
            new_play_filed[i][j] = CELL;
        } else {
            new_play_filed[i][j] = DEAD_CELL;
        }
    }
}

void draw_world(int play_filed[HEIGHT][WIDTH]) {
    for (int i = -1; i <= HEIGHT; i++) {
        for (int j = -1; j <= WIDTH; j++) {
            if ((i == -1) || (i == HEIGHT) || (j == -1) || (j == WIDTH)) {
                printf("@");
            } else if (play_filed[i][j] == 0) {
                printf(" ");
            } else {
                printf("\033[31mo\033[0m");
            }
        }
        printf("\n");
    }
}

void speed(const char *key_press, int *speed) {
    if (*key_press == 'l' && *speed < 500000) {
        *speed += 10000;
    } else if (*key_press == 'f' && *speed > 0) {
        *speed -= 10000;
    }
}