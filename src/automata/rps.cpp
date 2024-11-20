#include "rps.hpp"

RPS::RPS(std::string path_str, GLFWwindow *window, int square_size)
    : Automaton(path_str, window, square_size, red) {

    colors.push_back(green);
    colors.push_back(black);
    colors.push_back(blue);
    set_cell_colors();
};

RPS::~RPS() { glDeleteProgram(shader_program.program_ID); }

enum rps { RCK = 0b100, PPR = 0b010, SZA = 0b001 };

void RPS::update() {
    for (int offset = 0; offset < cell_count; offset++) {
        int row = offset % cols;
        if (cells[offset]
            && offset >= 0
            && offset < cell_count
            && row < cols
            && row >= 0
        ) {
            int bot = offset + cols;
            int top = offset - cols;
            int lft = offset - 1;
            int rgt = offset + 1;

            // test
            const int COUNT = 8;
            std::array<int, COUNT> nghbr_at = {top - 1, top, top + 1, lft, rgt,
                                               bot - 1, bot, bot + 1};
            std::vector<int> free_at;
            free_at.reserve(COUNT);
            std::vector<int> opp_at;
            opp_at.reserve(COUNT);

            int state = cells[offset];
            for (const int at : nghbr_at) {
                if (at < 0 || at > cell_count)
                    continue;

                int state_at = cells[at];
                if (state_at == state)
                    continue;

                int winner = 0;
                switch (state_at | state) {
                    case PPR | RCK: winner = PPR; break;
                    case SZA | PPR: winner = SZA; break;
                    case RCK | SZA: winner = RCK; break;
                    default: break;
                }

                // if (!state_at && !update_cells[at]) {
                if (!update_cells[at]) {
                    free_at.push_back(at);
                }
                if (state == winner) {
                    opp_at.push_back(at);
                }
            }

            if (!opp_at.empty()) {
                int ridx = rand() % opp_at.size();
                int idx = opp_at[ridx];
                update_cells[idx] = state;
                update_cells[offset] = 0;
                continue;
            }

            if (!free_at.empty()) {
                int ridx = rand() % free_at.size();
                int idx = free_at[ridx];
                update_cells[idx] = state;
                update_cells[offset] = 0;
            }
        }
    }
    cells = update_cells;

    update_states();
}

void RPS::update_chunk(int thread_idx, size_t thread_count) {
    int chunk_size = cell_count / thread_count;
    int start = thread_idx * chunk_size;
    int end = start + chunk_size;
    // std::cout << start << "|" << chunk_size << "|" << end << std::endl;

    for (int offset = start; offset < end; offset++) {
        int row = offset % cols;
        if (cells[offset]
            && offset >= 0
            && offset < cell_count
            && row < cols
            && row >= 0
        ) {
            int bot = offset + cols;
            int top = offset - cols;
            int lft = offset - 1;
            int rgt = offset + 1;

            // test
            const int COUNT = 8;
            std::array<int, COUNT> nghbr_at = {top - 1, top, top + 1, lft, rgt,
                                               bot - 1, bot, bot + 1};
            std::vector<int> free_at;
            free_at.reserve(COUNT);
            std::vector<int> opp_at;
            opp_at.reserve(COUNT);

            int state = cells[offset];
            for (const int at : nghbr_at) {
                if (at < 0 || at > cell_count)
                    continue;

                int state_at = cells[at];
                if (state_at == state)
                    continue;

                int winner = 0;
                switch (state_at | state) {
                    case PPR | RCK: winner = PPR; break;
                    case SZA | PPR: winner = SZA; break;
                    case RCK | SZA: winner = RCK; break;
                    default: break;
                }

                // if (!state_at && !update_cells[at]) {
                if (!update_cells[at]) {
                    free_at.push_back(at);
                }
                if (state == winner) {
                    opp_at.push_back(at);
                }
            }

            if (!opp_at.empty()) {
                int ridx = rand() % opp_at.size();
                int idx = opp_at[ridx];
                update_cells[idx] = state;
                update_cells[offset] = 0;
                continue;
            }

            if (!free_at.empty()) {
                int ridx = rand() % free_at.size();
                int idx = free_at[ridx];
                update_cells[idx] = state;
                update_cells[offset] = 0;
            }
        }
    }
}

void RPS::update_cell_states() {
    cells = update_cells;
    update_states();
}

std::string RPS::get_type() {
    std::string type_name = typeid(*this).name();
    std::string clean_name(type_name.begin() + 1, type_name.end());

    return "nan: " + clean_name;
}
