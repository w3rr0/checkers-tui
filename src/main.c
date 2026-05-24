#include <ctype.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "board.h"
#include "theme.h"

#define SHM_NAME "/checkers_shm_board"
#define SEM_WHITE "/checkers_sem_white"
#define SEM_BLACK "/checkers_sem_black"

Move get_user_move();

int main(int argc, char *argv[]) {
  if (argc != 2 ||
      (strcmp(argv[1], "white") != 0 && strcmp(argv[1], "black") != 0)) {
    printf("Usage: %s <white|black>\n", argv[0]);
    return 1;
  }

  bool is_white = strcmp(argv[1], "white") == 0;

  int shm_fd;
  Board *shared_board;
  sem_t *sem_white;
  sem_t *sem_black;

  if (is_white) {
    // White resets the shared memory and semaphores
    shm_unlink(SHM_NAME);
    sem_unlink(SEM_WHITE);
    sem_unlink(SEM_BLACK);

    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(Board));
    shared_board = mmap(NULL, sizeof(Board), PROT_READ | PROT_WRITE, MAP_SHARED,
                        shm_fd, 0);

    sem_white = sem_open(SEM_WHITE, O_CREAT, 0666, 1); // White starts
    sem_black = sem_open(SEM_BLACK, O_CREAT, 0666, 0); // Black waits

    init_board(shared_board);
    printf("Created new game. Waiting for black player to join...\n");
  } else {
    // Black joins an existing game
    shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
      printf("No game found! Please start the white player first\n");
      return 1;
    }
    shared_board = mmap(NULL, sizeof(Board), PROT_READ | PROT_WRITE, MAP_SHARED,
                        shm_fd, 0);

    sem_white = sem_open(SEM_WHITE, 0);
    sem_black = sem_open(SEM_BLACK, 0);
    printf("Joined the game as black player\n");

    // Show board - black
    printf("\033[2J\033[H");
    print_board(shared_board);
  }

  sem_t *my_sem = is_white ? sem_white : sem_black;
  sem_t *opp_sem = is_white ? sem_black : sem_white;

  while (true) {
    printf("Turn: %s (You are %s)\n",
           shared_board->white_turn ? "WHITE" : "BLACK",
           is_white ? "WHITE" : "BLACK");
    printf("Waiting for opponent...\n");
    sem_wait(my_sem);
    printf("\033[2J\033[H"); // Clear screen

    print_board(shared_board);

    bool winner = true;

    // Check for win
    if (shared_board->white_pieces == 0) {
      printf("BLACK WINS!\n");
    } else if (shared_board->black_pieces == 0) {
      printf("WHITE WINS!\n");
    } else {
      winner = false;
    }

    if (winner) {
      sem_post(opp_sem); // opponent can see they lost
      break;
    }

    printf("Turn: %s (You are %s)\n",
           shared_board->white_turn ? "WHITE" : "BLACK",
           is_white ? "WHITE" : "BLACK");

    while (true) {
      Move m = get_user_move();
      if (execute_move(shared_board, m)) {
        break;
      }
      printf("%sWrong move! Please try again%s\n", red_color_code, reset_color);
    }

    // Show board after own move
    printf("%s", clear_screen);
    print_board(shared_board);

    // Pass turn to opponent
    sem_post(opp_sem);
  }

  munmap(shared_board, sizeof(Board));
  close(shm_fd);
  sem_close(sem_white);
  sem_close(sem_black);

  if (is_white) {
    shm_unlink(SHM_NAME);
    sem_unlink(SEM_WHITE);
    sem_unlink(SEM_BLACK);
  }

  return 0;
} // MAIN

Move get_user_move() {
  char col_from, col_to;
  int row_from, row_to;

  printf("Enter your move: ");

  while (true) {
    int assigned = scanf(" %c%d %c%d", &col_from, &row_from, &col_to, &row_to);
    if (assigned == 4) {
      break;
    }
    printf("\033[31mWrong format! Please enter correct move\033[0m\n");

    while (getchar() != '\n')
      ;
  }

  return (Move){.from = {.x = toupper(col_from) - 'A', .y = row_from - 1},
                .to = {.x = toupper(col_to) - 'A', .y = row_to - 1}};
}
