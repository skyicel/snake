#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define HEIGHT 5
#define WIDTH 5
#define MAX_SNAKE_LENGTH 400

typedef struct
{
    int x;
    int y;
} point;

typedef enum {
    LEFT = 1,
    DOWN = 2,
    RIGHT = 3,
    UP = 4
} KEY;

typedef struct {
    point* points;
    int size;
    int move_direction;
} snake;

typedef struct {
    point position;
} apple;

void print_current_game_state(snake* snake, apple apple);

void curse_prepare();

void game();

void change_snake_direction(snake* snake, int ch);

int check_head_snake_colision_with_apple(snake* snake, apple apple);

int check_full_body_snake_colision_with_apple(snake* snake, apple apple);

int check_snake_colision_with_itself(snake* snake);

void update_snake(snake* snake);

int main(void){	
	initscr();

    curse_prepare();

    game();
    
	endwin();
	return 0;
}

void print_current_game_state(snake* snake, apple apple) {
    clear();

    move(0, 0);

    for (int i = 0; i < WIDTH + 2; i++) {
        printw("#");
    }

    for (int i = 0; i < HEIGHT; i++) {
        mvprintw(i + 1, 0, "#");
        mvprintw(i + 1, WIDTH + 1, "#");
    }

    mvprintw(apple.position.y + 1, apple.position.x + 1, "*");

    for (int i = 0; i < snake->size; i++) {
        mvprintw(snake->points[i].y + 1, snake->points[i].x + 1, "@");
    }
    
    move(HEIGHT + 1, 0);

    for (int i = 0; i < WIDTH + 2; i++) {
        printw("#");
    }

    refresh();
}

void curse_prepare() {
    curs_set(0);
    noecho();
    nodelay(stdscr, TRUE);
}

void game() {
    snake* snake = malloc(sizeof(snake));

    snake->points = malloc(MAX_SNAKE_LENGTH * sizeof(point));

    snake->size = 1;

    srand(time(NULL));

    snake->points[0].x = rand() % WIDTH;
    snake->points[0].y = rand() % HEIGHT;
    snake->move_direction = rand() % 4 + 1;

    apple apple;

    apple.position.x = rand() % WIDTH;
    apple.position.y = rand() % HEIGHT;

    point snake_tail_position;

    int ch;

    int game_over = 0;

    while (1) {
        ch = getch();
        
        if (ch == ' ') {
            break;
        } else if (ch != ERR) {
            change_snake_direction(snake, ch);
        }

        snake_tail_position = snake->points[snake->size - 1];

        update_snake(snake);

        if (check_head_snake_colision_with_apple(snake, apple)) {

            while (check_full_body_snake_colision_with_apple(snake, apple)) {
                apple.position.x = rand() % WIDTH;
                apple.position.y = rand() % HEIGHT;
            }

            snake->points[snake->size] = snake_tail_position;
            snake->size++;
        }

        if (check_snake_colision_with_itself(snake)) {
            game_over = 1;
            break;
        }

        print_current_game_state(snake, apple);

        napms(100);

    }

    if (game_over) {
        clear();
        printf("gameover");
        refresh();
    }

    nodelay(stdscr, FALSE);
    getch();

    free(snake->points);
    free(snake);
}

void change_snake_direction(snake* snake, int ch) {
    if (ch == 'a') {
        snake->move_direction = LEFT;
    } else if (ch == 'd') {
        snake->move_direction = RIGHT;
    } else if (ch == 'w') {
        snake->move_direction = UP;
    } else if (ch == 's') {
        snake->move_direction = DOWN;
    }
}

void update_snake(snake* snake) {
    for (int i = snake->size - 1; i >= 1; i--) {
        snake->points[i] = snake->points[i - 1];
    }

    if (snake->move_direction == LEFT) {
        snake->points[0].x = (snake->points[0].x - 1 + WIDTH) % WIDTH;
    } else if (snake->move_direction == RIGHT) {
        snake->points[0].x = (snake->points[0].x + 1 + WIDTH) % WIDTH;
    } else if (snake->move_direction == UP) {
        snake->points[0].y = (snake->points[0].y - 1 + HEIGHT) % HEIGHT ;
    } else if (snake->move_direction == DOWN) {
        snake->points[0].y = (snake->points[0].y + 1 + HEIGHT) % HEIGHT ;
    }
}

int check_head_snake_colision_with_apple(snake* snake, apple apple) {


    if ((snake->points[0].x == apple.position.x) && (snake->points[0].y == apple.position.y)) {
        return 1;
    }

    return 0;
}

int check_full_body_snake_colision_with_apple(snake* snake, apple apple) {
    for (int i = 0; i < snake->size; i++) {
        if ((snake->points[i].x == apple.position.x) && (snake->points[i].y == apple.position.y)) {
            return 1;
        }
    }

    return 0;
}

int check_snake_colision_with_itself(snake* snake) {
    for (int i = 0; i < snake->size; i++) {
        for (int j = 0; j < snake->size; j++) {
            if (i != j && snake->points[j].x == snake->points[i].x && snake->points[j].y == snake->points[i].y) {
                return 1;
            }
        }
    }

    return 0;
}