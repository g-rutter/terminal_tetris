#include <chrono>
#include <curses.h>
#include <optional>

#include "view.hpp"
#include "active_piece.hpp"

using namespace std;

enum class InputMode{
    SplashScreen,
    Play,
    Restart
};

enum class InputResult{
    Continue, // Continue the input loop
    EndCycle, // End the innermost input loop, continue the game mode
    EndMode,  // Move to the next game mode: Splash->Game->EndScreen->Game->EndScreen->...
    EndGame   // Terminate the program
};

template<InputMode mode>
struct InputManager {
    // TODO not ideal since active_piece is only needed for InputMode::Play:
    InputManager(const TetrisView& tetrisview, ActivePiece& active_piece)
        : m_tetrisview{tetrisview}, m_active_piece{active_piece} {}

    InputResult input_loop(int cycle_time_ms) const {
        const chrono::time_point start = chrono::steady_clock::now();
        int elapsed, ch;
        InputResult input_result{};
        do {
            ch = getch();

            if constexpr(mode == InputMode::Play) input_result = play_input(ch);
            if constexpr(mode == InputMode::SplashScreen) input_result = splashscreen_input(ch);
            else if constexpr(mode == InputMode::Restart) input_result = endgame_input(ch);

            if(ch != ERR) m_tetrisview.update_gridview(m_active_piece);
            chrono::time_point end = chrono::steady_clock::now();
            elapsed = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        } while (input_result == InputResult::Continue && elapsed < cycle_time_ms);

        return input_result;
    }

    private:
        InputResult play_input(const int& ch) const {
            switch (ch) {
                case 'a':
                case KEY_LEFT:
                    m_active_piece.left();
                    return InputResult::Continue;
                case 'd':
                case KEY_RIGHT:
                    m_active_piece.right();
                    return InputResult::Continue;
                case 'w':
                case KEY_UP:
                    m_active_piece.rotate();
                    return InputResult::Continue;
                case 's':
                case KEY_DOWN:
                    m_active_piece.down();
                    return m_active_piece.m_landed ? InputResult::EndCycle : InputResult::Continue;
                case ' ':
                    m_active_piece.fall();
                    return InputResult::EndCycle;
                default:
                    return InputResult::Continue;
            }
        }

        InputResult splashscreen_input(const int& ch) const {
            switch (ch) {
                case ' ':
                    return InputResult::EndMode;
                default:
                    return InputResult::Continue;
            }
        }

        InputResult endgame_input(const int& ch) const {
            switch (ch) {
                case 'r':
                    return InputResult::EndMode;
                case 'q':
                    return InputResult::EndGame;
                default:
                    return InputResult::Continue;
            }
        }
        const TetrisView& m_tetrisview;
        ActivePiece& m_active_piece; // TODO Replace with something compile time and safer
};