#ifndef THEME_H
#define THEME_H

constexpr auto pawn_symbol[] = " ● ";
constexpr auto queen_symbol[] = " ♕ ";
constexpr auto empty_symbol[] = "   ";

constexpr auto white_color_code[] = "\033[38;5;15m";
constexpr auto black_color_code[] = "\033[38;5;160m";

constexpr auto red_color_code[] = "\033[31m";

constexpr auto reset_color[] = "\033[0m";
constexpr auto clear_screen[] = "\033[2J\033[H";

constexpr auto white_square[] = "\033[48;5;235m";
constexpr auto black_square[] = "\033[48;5;250m";

#endif // THEME_H
