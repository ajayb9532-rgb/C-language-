// Diwali.cpp - Console animation: firecrackers, glowing building, changing wishes
// Compile: g++ -std=c++17 diwali.cpp -o diwali
// Run:     ./diwali   (Ctrl+C to stop). Works in any ANSI-color terminal.

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <thread>
#include <chrono>
#include <cmath>
using namespace std;

const int W = 72, H = 28;

struct Cell { char c = ' '; int col = 0; };
struct Particle { float x, y, vx, vy; int life; int col; char ch; };
struct Rocket { float x, y, vy; int targetY; int col; };

Cell buf[H][W];
mt19937 rng(random_device{}());

int rnd(int a, int b) { return uniform_int_distribution<int>(a, b)(rng); }
float rndf(float a, float b) { return uniform_real_distribution<float>(a, b)(rng); }

void put(int x, int y, char c, int col) {
    if (x >= 0 && x < W && y >= 0 && y < H) { buf[y][x].c = c; buf[y][x].col = col; }
}
void text(int x, int y, const string& s, int col) {
    for (size_t i = 0; i < s.size(); i++) put(x + (int)i, y, s[i], col);
}
void clearBuf() {
    for (auto& row : buf) for (auto& c : row) c = Cell();
}
void render() {
    string out = "\033[H";
    int cur = -1;
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            if (buf[y][x].col != cur) {
                cur = buf[y][x].col;
                out += cur ? "\033[" + to_string(cur) + "m" : "\033[0m";
            }
            out += buf[y][x].c;
        }
        out += "\033[0m\n"; cur = -1;
    }
    cout << out << flush;
}

const int PALETTE[] = {91, 92, 93, 94, 95, 96, 97};

void drawBuilding(int frame, vector<vector<bool>>& lit) {
    const int x0 = 24, x1 = 48, top = H - 13, ground = H - 1;

    // ground
    for (int x = 0; x < W; x++) put(x, ground, '=', 33);

    // walls
    for (int y = top; y < ground; y++) {
        put(x0, y, '|', 37);
        put(x1, y, '|', 37);
    }
    text(x0, top - 1, string(x1 - x0 + 1, '_'), 37);

    // string lights along the roof (twinkle)
    for (int x = x0; x <= x1; x++) {
        int col = PALETTE[(x + frame / 3) % 7];
        put(x, top - 2, (x + frame) % 2 ? 'o' : '*', col);
    }
    // diyas on the roof corners
    text(x0 - 1, top - 2, "(\")", 93);
    text(x1 - 1, top - 2, "(\")", 93);

    // windows
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 5; c++) {
            int wx = x0 + 3 + c * 4, wy = top + 1 + r * 2;
            if (lit[r][c]) {
                int col = (frame / 4 + r + c) % 2 ? 93 : 33;   // warm flicker
                text(wx, wy, "[#]", col);
            } else {
                text(wx, wy, "[ ]", 90);
            }
        }
    }
    // door with a rangoli-ish welcome
    int mid = (x0 + x1) / 2;
    text(mid - 1, ground - 2, "|_|", 97);
    text(mid - 1, ground - 1, "|_|", 97);
    text(mid - 4, ground - 1, "~*~", 95);
    text(mid + 2, ground - 1, "~*~", 95);
}

int main() {
    const vector<string> wishes = {
        "*** HAPPY DIWALI! ***",
        "May your life be full of light!",
        "Wishing you joy, health & wealth",
        "Shubh Deepavali!",
        "Deepavali ki Hardik Shubhkamnayein!",
        "Light a diya, spread the happiness",
        "May Lakshmi bless your home",
    };

    vector<Particle> sparks;
    vector<Rocket> rockets;
    vector<vector<bool>> lit(4, vector<bool>(5, false));
    vector<pair<int,int>> stars;
    for (int i = 0; i < 40; i++) stars.push_back({rnd(0, W - 1), rnd(0, H - 8)});

    cout << "\033[2J\033[?25l";   // clear screen, hide cursor

    for (int frame = 0; frame < 1200; frame++) {
        clearBuf();

        // stars
        for (auto& s : stars)
            if ((frame + s.first) % 10 < 7) put(s.first, s.second, '.', 90);

        // gradually light up the building
        if (frame % 6 == 0) lit[rnd(0, 3)][rnd(0, 4)] = true;
        if (frame > 150 && frame % 9 == 0) lit[rnd(0, 3)][rnd(0, 4)] = rnd(0, 4) != 0;
        drawBuilding(frame, lit);

        // launch rockets
        if (rnd(0, 100) < 14)
            rockets.push_back({(float)rnd(6, W - 7), (float)(H - 2), -rndf(0.5f, 0.9f),
                               rnd(3, 12), PALETTE[rnd(0, 6)]});

        // update rockets
        for (size_t i = 0; i < rockets.size();) {
            Rocket& r = rockets[i];
            r.y += r.vy;
            put((int)r.x, (int)r.y, '|', 97);
            put((int)r.x, (int)r.y + 1, ':', 33);
            if (r.y <= r.targetY) {             // BOOM
                int n = rnd(28, 44);
                int style = rnd(0, 2);
                for (int k = 0; k < n; k++) {
                    float a = 2 * M_PI * k / n;
                    float sp = style == 0 ? rndf(0.4f, 1.3f) : 1.1f;
                    int col = style == 2 ? PALETTE[rnd(0, 6)] : r.col;
                    sparks.push_back({r.x, r.y, cos(a) * sp * 1.8f, sin(a) * sp * 0.9f,
                                      rnd(10, 20), col, "*+."[rnd(0, 2)]});
                }
                rockets.erase(rockets.begin() + i);
            } else i++;
        }

        // update sparks
        for (size_t i = 0; i < sparks.size();) {
            Particle& p = sparks[i];
            p.x += p.vx; p.y += p.vy; p.vy += 0.05f; p.vx *= 0.97f; p.life--;
            char ch = p.life < 5 ? '.' : p.ch;
            put((int)round(p.x), (int)round(p.y), ch, p.life < 5 ? 90 : p.col);
            if (p.life <= 0) sparks.erase(sparks.begin() + i); else i++;
        }

        // changing Diwali wish (drawn last so it stays on top)
        const string& w = wishes[(frame / 35) % wishes.size()];
        int wcol = PALETTE[(frame / 5) % 7];
        text((W - (int)w.size()) / 2, 1, w, wcol);
        text(W / 2 - 9, 2, "~ Deepavali 2026 ~", 93);

        render();
        this_thread::sleep_for(chrono::milliseconds(80));
    }

    cout << "\033[0m\033[?25h\033[" << H + 1 << ";1H" << "Happy Diwali! Stay safe.\n";
    return 0;
}
