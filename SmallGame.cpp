#include <iostream>
#include "player.h"
#include <ncurses.h>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

//这个文件到时候用来装小游戏用，通关获得钥匙的那个小游戏

void JiaZhuangTaShiYiGeYouXi (){
    cout << "这真的是一个游戏，你获得了一个钥匙" << endl;
    cout << "输入一个zimu出去" << endl;
    char a;
    cin >> a;
    number_of_key ++;
    return; 
}


// Game structs
struct Plane {
    int x, y;
    int hp;
};

struct Bullet {
    int x, y;
};

// Game state
int max_y, max_x;
Plane player;
std::vector<Plane> enemies;
std::vector<Bullet> bullets;
int player_lives;
int defeated_count;
int total_to_defeat;
int enemy_hp;
float enemy_speed; // rows per second
int bullets_per_shot;
float spawn_timer;
float shoot_timer;
bool game_running;

// Difficulty settings
struct Difficulty {
    int lives;
    int enemies_to_defeat;
    int enemy_hp;
    float enemy_speed;
    int bullets_per_shot;
};

void setup_difficulty(int choice) {
    Difficulty diff;
    switch (choice) {
        case 1: // Easy
            diff = {5, 10, 1, 1.0, 7};
            break;
        case 2: // Normal
            diff = {3, 20, 2, 2.0, 7};
            break;
        case 3: // Hard
            diff = {1, 30, 3, 3.0, 7};
            break;
        default:
            diff = {5, 10, 1, 1.0, 7};
    }
    player_lives = diff.lives;
    total_to_defeat = diff.enemies_to_defeat;
    enemy_hp = diff.enemy_hp;
    enemy_speed = diff.enemy_speed;
    bullets_per_shot = diff.bullets_per_shot;
}

void display_title() {
    const char* title = R"(
  _____ _           _               _     _
 / ____| |         | |             | |   | |
| (___ | |__   __ _| |__   ___  ___| |__ | |
 \___ \| '_ \ / _` | '_ \ / _ \/ __| '_ \| |
 ____) | | | | (_| | |_) |  __/ (__| | | | |
|_____/|_| |_|__,_|_.__/ \___|\___|_| |_|_|
    )";
    mvprintw(0, 0, "%s", title);
    mvprintw(10, 0, "Press any key to continue...");
    refresh();
    nodelay(stdscr, FALSE); // Make getch() blocking
    getch();
    nodelay(stdscr, TRUE); // Restore non-blocking
}

void display_rules() {
    clear();
    mvprintw(0, 0, "Game Rules:");
    mvprintw(1, 0, "1. Enemy planes spawn at the top and move down.");
    mvprintw(2, 0, "2. Move your plane left/right with arrow keys.");
    mvprintw(3, 0, "3. Press space to shoot bullets (up to 5 per second).");
    mvprintw(4, 0, "4. Bullets hitting enemies reduce their HP.");
    mvprintw(5, 0, "5. Enemy collision with your plane reduces your HP.");
    mvprintw(6, 0, "6. Win by defeating all enemies; lose if all planes are destroyed.");
    mvprintw(8, 0, "Choose difficulty: 1. Easy 2. Normal 3. Hard");
    refresh();
    nodelay(stdscr, FALSE); // Make getch() blocking
    int choice = getch() - '0';
    nodelay(stdscr, TRUE); // Restore non-blocking
    setup_difficulty(choice);
}

void display_game_over(bool won) {
    clear();
    nodelay(stdscr, FALSE); // Make getch() blocking
    if (won) {
        mvprintw(max_y / 2, max_x / 2 - 5, "Victory! All enemies defeated!");
        mvprintw(max_y / 2 + 2, max_x / 2 - 10, "Press any key to exit.");
        refresh();
        getch();
        endwin();
        exit(0);
    } else {
        mvprintw(max_y / 2, max_x / 2 - 5, "BOOM! Game Over!");
        mvprintw(max_y / 2 + 2, max_x / 2 - 10, "1. Play again 2. Save 3. Exit");
        refresh();
        int choice;
        do {
            choice = getch() - '0';
            if (choice == 3) {
                mvprintw(max_y / 2 + 4, max_x / 2 - 15, "Exit will lose all data. Confirm? (y/n)");
                refresh();
                int confirm = getch();
                if (confirm == 'y') {
                    endwin();
                    exit(0);
                }
            }
        } while (choice != 1 && choice != 2);
        if (choice == 2) {
            // Placeholder for save functionality
            mvprintw(max_y / 2 + 4, max_x / 2 - 10, "Game saved (placeholder).");
            refresh();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        // Restart
        player = {(max_x - 5) / 2, max_y - 3, 5};
        enemies.clear();
        bullets.clear();
        defeated_count = 0;
        spawn_timer = 0;
        shoot_timer = 0;
        game_running = true;
        nodelay(stdscr, TRUE); // Restore non-blocking for game loop
        display_rules();
    }
}

void AirplaneBattleGame() {
    // Initialize ncurses
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);
    getmaxyx(stdscr, max_y, max_x);
    srand(static_cast<unsigned int>(time(0)));

    // Show title and rules
    display_title();
    display_rules();

    // Initialize game state
    player = {(max_x - 5) / 2, max_y - 3, 5};
    defeated_count = 0;
    spawn_timer = 0;
    shoot_timer = 0;
    game_running = true;

    // Game loop
    auto last_time = std::chrono::steady_clock::now();
    while (game_running) {
        auto current_time = std::chrono::steady_clock::now();
        float delta_time = std::chrono::duration<float>(current_time - last_time).count();
        last_time = current_time;

        // Handle input
        int ch = getch();
        if (ch != ERR) {
            switch (ch) {
                case KEY_LEFT:
                    if (player.x > 0) player.x--;
                    break;
                case KEY_RIGHT:
                    if (player.x < max_x - 5) player.x++;
                    break;
                case ' ':
                    if (shoot_timer <= 0) {
                        for (int i = -bullets_per_shot / 2; i <= bullets_per_shot / 2; i++) {
                            Bullet b = {player.x + i, player.y - 1};
                            if (b.x >= 0 && b.x < max_x) bullets.push_back(b);
                        }
                        shoot_timer = 0.2; // 5 shots per second
                    }
                    break;
            }
        }

        // Update timers
        spawn_timer -= delta_time;
        shoot_timer -= delta_time;

        // Spawn enemies
        if (spawn_timer <= 0 && enemies.size() < 10 && defeated_count < total_to_defeat) {
            int x = rand() % max_x;
            Plane enemy = {x, 0, enemy_hp};
            enemies.push_back(enemy);
            spawn_timer = 1.0;
        }

        // Move bullets
        for (auto& b : bullets) {
            b.y--;
        }
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b) { return b.y < 0; }), bullets.end());

        // Move enemies
        static float enemy_move_timer = 0;
        enemy_move_timer += delta_time;
        if (enemy_move_timer >= 1.0 / enemy_speed) {
            for (auto& e : enemies) {
                e.y++;
            }
            enemy_move_timer = 0;
        }
        enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
            [](const Plane& e) { return e.y >= max_y; }), enemies.end());

        // Check collisions
        for (auto& b : bullets) {
            for (auto& e : enemies) {
                if (b.x == e.x && b.y == e.y) {
                    e.hp--;
                    if (e.hp <= 0) {
                        defeated_count++;
                        e.y = -1;
                    }
                    b.y = -1;
                }
            }
        }
        enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
            [](const Plane& e) { return e.y < 0; }), enemies.end());
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b) { return b.y < 0; }), bullets.end());

        // Check player-enemy collisions
        for (auto& e : enemies) {
            if (e.x >= player.x && e.x <= player.x + 4 && e.y >= player.y && e.y <= player.y + 2) {
                player.hp--;
                e.y = -1;
                if (player.hp <= 0) {
                    player_lives--;
                    if (player_lives > 0) {
                        player = {(max_x - 5) / 2, max_y - 3, 5};
                    } else {
                        game_running = false;
                        display_game_over(false);
                    }
                }
            }
        }

        // Draw screen
        clear();
        // Draw player as a 5x3 block
        for (int dy = 0; dy < 3; dy++) {
            for (int dx = 0; dx < 5; dx++) {
                mvaddch(player.y + dy, player.x + dx, '@');
            }
        }
        for (const auto& e : enemies) {
            mvaddch(e.y, e.x, 'V');
        }
        for (const auto& b : bullets) {
            mvaddch(b.y, b.x, '*');
        }
        mvprintw(0, max_x - 20, "Lives: %d HP: %d", player_lives, player.hp);
        mvprintw(1, max_x - 20, "Defeated: %d/%d", defeated_count, total_to_defeat);
        refresh();

        // Check win condition
        if (defeated_count >= total_to_defeat) {
            game_running = false;
            display_game_over(true);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    endwin();
    return;
}

