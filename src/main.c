// Released under the MIT license
// Copyright (c) 2023 ColdMacaroni
#include <curses.h>
#include <ncurses.h>
#include <stdlib.h>

#define PT_COLOR 1
#define LINE_COLOR 2

typedef struct Point {
  int x;
  int y;
} Point;

void point_from_mouse(Point *dest) {
  int inp;
  MEVENT event;
  do {
    inp = getch();
  } while (inp != KEY_MOUSE || getmouse(&event) != OK);

  dest->x = event.x;
  dest->y = event.y;
}

void print_values(int x, int y, int startx, int starty, int endx, int endy, int dx, int dy, int error) {
  mvprintw( 1, 2, "Start:\t(%2d, %2d) ", startx, starty);
  mvprintw( 2, 2, "End:\t\t(%2d, %2d) ", endx, endy);
  mvprintw( 3, 2, "Currrent:\t(%2d, %2d) ", x, y);
  mvprintw( 4, 2, "Derivative:\t%2d/%2d", dx, dy);
  mvprintw( 5, 2, "Error:\t%2d", error);

}

void bresenham(const Point *const pt1, const Point *const pt2) {
  /* Thanks to https://www.baeldung.com/cs/bresemhans-line-algorithm */
  // Get the deltas
  const int dy = -abs(pt2->y - pt1->y);
  const int dx = abs(pt2->x - pt1->x);

  // Calculate the steps
  const int stepx = (pt1->x < pt2->x) ? 1 : -1;
  const int stepy = (pt1->y < pt2->y) ? 1 : -1;

  int error = dx + dy;

  // Points we're drawing
  int x = pt1->x;
  int y = pt1->y;
  while (TRUE) {
    // Draw
    print_values(x, y, pt1->x, pt2->y, pt2->x, pt2->y, dx, dy, error);
    mvaddch(y, x, '#');

    mvchgat(y, x, 1, A_NORMAL, LINE_COLOR, NULL);
    mvchgat(pt1->y, pt1->x, 1, A_NORMAL, PT_COLOR, NULL);
    mvchgat(pt2->y, pt2->x, 1, A_NORMAL, PT_COLOR, NULL);

    refresh();
    getch();
    // Stop when we've reached the end point
    if (x == pt2->x && y == pt2->y)
      break;

    if (error * 2 >= dy) {
      // Stop if we are close enough.
      if (x == pt2->x)
        break;

      error += dy;
      x += stepx;
    }

    if (error * 2 <= dx) {
      // Stop if we are close enough.
      if (y == pt2->y)
        break;

      error += dx;
      y += stepy;
    }
  }
}

int main() {
  // Initialise stuff
  initscr();
  curs_set(0);
  start_color();
  use_default_colors();
  noecho();
  cbreak();

  keypad(stdscr, TRUE);
  mousemask(BUTTON1_RELEASED, NULL);

  Point start, end;

  // Colors :)
  init_pair(PT_COLOR, COLOR_CYAN, -1);
  init_pair(LINE_COLOR, COLOR_RED, -1);

  mvprintw(0, 0, "Click to place first point");
  point_from_mouse(&start);
  mvaddch(start.y, start.x, 'X');

  mvprintw(0, 0, "Click to place second point");
  point_from_mouse(&end);

  clear();
  mvaddch(start.y, start.x, 'X');
  mvaddch(end.y, end.x, 'X');
  mvchgat(start.y, start.x, 1, A_NORMAL, PT_COLOR, NULL);
  mvchgat(end.y, end.x, 1, A_NORMAL, PT_COLOR, NULL);
  getch();
  refresh();

  bresenham(&start, &end);

  getch();
  endwin();
  return 0;
}
