#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

#define WIDTH 400
#define HEIGHT 400
#define TITLE "Balls and their admirers"
#define BALL_COUNT 100
#define FPS 60
#define VEL_MAX 5
#define RADIUS_MAX 20
#define RADIUS_MIN 5
#define COUNT_OF(array) (sizeof(array)/sizeof(array[0]))

Color COLORS[] = {
    LIGHTGRAY, GRAY,   DARKGRAY, YELLOW,     GOLD,      ORANGE,  PINK,
    RED,       MAROON, GREEN,    LIME,       DARKGREEN, SKYBLUE, BLUE,
    DARKBLUE,  PURPLE, VIOLET,   DARKPURPLE, BEIGE,     BROWN,   DARKBROWN,
};

// Definition of Ball
// Ball stores state and other properties
// YOUR CODE HERE
typedef struct Ball {
  int posx, posy, radius, velx, vely;
  struct Ball *follows;
  struct Color color;
} Ball;

Ball balls[BALL_COUNT];

// Initializes a ball with random values
// init_ball_random is a function that takes a pointer to a ball as it argument
// and returns a pointer to a Ball.
// YOUR CODE HERE
Ball* init_ball_random(Ball *p) {
  p->radius =GetRandomValue(RADIUS_MIN, RADIUS_MAX); // switched to raylib random value instead of srand time + rand()
  p->posx = GetRandomValue(p->radius, WIDTH - p->radius); //So they don't spawn outside the screen
  p->posy = GetRandomValue(p->radius, HEIGHT - p->radius);
  int randomColor = GetRandomValue(0, 20);
  p->color = COLORS[randomColor];
  p->velx = GetRandomValue(-VEL_MAX,VEL_MAX);
  p->vely = GetRandomValue(-VEL_MAX,VEL_MAX);

  // Find a leading ball other than the initialized ball itself.
  int currentIndex = p - balls;
  //^ index of p in the balls array (because balls = &balls[0] so p - balls is &balls[i] - &balls[0] and in pointer arithmetic that returns the distance in elements (index)s, not bytes (for some reason?)
  int randomLeaderIndex;
  do{
    randomLeaderIndex = GetRandomValue(0, BALL_COUNT - 1);}
    while (currentIndex == randomLeaderIndex); //runs until it finds a ball that is not itself
    
  Ball *leader = &balls[randomLeaderIndex];
  p->follows = leader;

      return p;
}

// Initialize all `balls`
void init_balls_random() {
  for (size_t i = 0; i < BALL_COUNT; i++) {
    init_ball_random(&balls[i]);
 }
}

Ball *draw_ball(Ball *p) {
  DrawCircle(p->posx, p->posy, p->radius, p->color);
  return p;
}

// Updates the positions of balls according to their velocities
Ball *update_pos(Ball *p) {
  p->posx = (WIDTH + p->posx + p->velx) %
            WIDTH; // `WIDTH +` because C uses truncated division
  p->posy = (HEIGHT + p->posy + p->vely) % HEIGHT;
  return p; //remainder might be a bit of a weird solution for this since it can result in negative numbers I think? maybe causing weird wrapping. But task was specifically NOT to touch others parts of the code so leaving.
}

// Updates the velocity of a ball so that it follows the leading ball
Ball *update_vel_for_following(Ball *p) {
  int errx = p->follows->posx - p->posx;
  int erry = p->follows->posy - p->posy;
  p->velx = errx > 0 ? 1 : -1;
  p->vely = erry > 0 ? 1 : -1;
  return p;
}

// Update all elements
void update_elements() {
  for (size_t i = 0; i < COUNT_OF(balls); ++i) { //defined my own _Countof macro as it did not work for me before
    draw_ball(update_pos(update_vel_for_following(&balls[i])));
  }
}

int main() {
  InitWindow(WIDTH, HEIGHT, TITLE);
  SetTargetFPS(FPS);

  init_balls_random();

  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    BeginDrawing();
    update_elements();
    ClearBackground(RAYWHITE);
    EndDrawing();
  }
}