#include <iostream>
#include <cstdint>
#include "generation_functions.h"
#include "magics.h"
#include <condition_variable>

using namespace std;

condition_variable term_signal;

bool terminated = false;
void catch_ctrl_c(int sig)
{
    terminated = true;
    term_signal.notify_all();
}

void wait_for_stop()
{
    signal(SIGINT, catch_ctrl_c);

    mutex m;
    unique_lock lock(m);
    term_signal.wait(lock, [&](){return terminated;});
    cout << "Terminating..." << endl;
}

void make_bishop_tables(int threads, int maximum_shift)
{
    cout << "Looking for bishop magic numbers." << endl;

    auto attacks = generate_bishop_attacks();
    magic_number_generator generator(attacks, "bishop", "best_bishop_magics.txt", bishop_masks, maximum_shift);

    generator.start_search(threads);

    wait_for_stop();

    generator.stop_search();

    cout << "Generating tables and saving..." << endl;
    generator.generate_magic_tables();
    generator.export_to_cpp();
    generator.save_magics();
}


void make_rook_tables(int threads, int maximum_shift)
{
    cout << "Looking for rook magic numbers." << endl;
    auto attacks = generate_rook_attacks();
    magic_number_generator generator(attacks, "rook", "best_rook_magics.txt", rook_masks, maximum_shift);

    generator.start_search(threads);

    wait_for_stop();

    generator.stop_search();

    cout << "Generating tables and saving..." << endl;
    generator.generate_magic_tables();
    generator.export_to_cpp();
    generator.save_magics();
}


int main(int argc, char* argv[])
{
    cout << std::log2((uint64_t)a8) << endl;

    if (argc < 3)
    {
        print_usage:
        cout << "Usage: magic_generator [bishop | rook] [threads] [maximum shift (optional)]" << endl;
        cout << "Example: ./magic_generator bishop 16 60" << endl;
        return 1;
    }

    int maximum_shift = -1;
    if (argc > 3)
    {
        maximum_shift = stoi(argv[3]);
    }

    if (argv[1] == string("rook"))
    {
        make_rook_tables(stoi(argv[2]), maximum_shift != -1 ? maximum_shift : 56);
    }
    else if (argv[1] == string("bishop"))
    {
        make_bishop_tables(stoi(argv[2]), maximum_shift != -1 ? maximum_shift : 60);
    }
    else goto print_usage;

    return 0;
}
