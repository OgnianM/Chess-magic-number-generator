#pragma once
#include "generation_functions.h"
#include <unordered_map>
#include <vector>

#include <cstring>
#include <sstream>
#include <random>
#include <csignal>
#include <thread>
#include <iomanip>
#include <mutex>

template< typename T >
std::string int_to_hex( T i )
{
    std::stringstream stream;
    stream << "0x"
           << std::setfill ('0') << std::setw(sizeof(T)*2)
           << std::hex << i;
    auto str = stream.str();
    if (sizeof(T) == 8) str += "ULL";
    return str;
}



class magic_number_generator
{
    vector<thread> search_threads;
    attacks_to_positions_map_list a2p_list;
    bool searching = false;
    uint64_t initial_shift;

    bool test_magic_number(uint64_t magic, uint64_t shift, int square_index, uint64_t* table);
    bool generate_magic_table(int square_index, uint64_t* table);
    void search_instance(int thread_id);

public:
    basic_attack_map_list& attacks;
    string piece_name, magic_file;
    uint8_t magic_number_count;

    vector<uint64_t> shifts;
    vector<uint64_t> magics;
    vector<uint64_t> table_sizes;
    vector<uint64_t*> tables;

    mutex magic_update_lock;

    uint64_t* masks;

    uint64_t get_attacks_for_piece(uint64_t position, int idx);

    // Debugging function
    void stress_test_magic_number(int index, int iterations=1000000);

    magic_number_generator(basic_attack_map_list& attacks, const char* piece_name,
                           const char* magic_file, uint64_t* masks, uint64_t initial_shift=55);


    // Check if we have a magic number for all 64 piece positions
    bool have_full_set();


    void start_search(int threads=8);
    void stop_search();


    // Create the tables used for move generation
    bool generate_magic_tables();

    // Create a cpp file with all the necessary data for move generation.
    void export_to_cpp();

    // Save magic numbers and shifts to a text file, to resume search later.
    void save_magics();

    // Read magic numbers from file created by save_magics
    void read_magics();
};
