#include "magics.h"
#include <fstream>

uint64_t rand64()
{
    static std::random_device rng;
    thread_local mt19937_64 generator(rng());
    thread_local std::uniform_int_distribution<uint64_t> dist(0);

    return dist(generator);
}


bool magic_number_generator::test_magic_number(uint64_t magic, uint64_t shift, int square_index, uint64_t* table)
{
    auto& a2p = a2p_list[square_index];
    uint64_t maximal_index = 0;


    auto clear_table = [&]()
    {
        memset(table, 0, maximal_index * sizeof(uint64_t) + sizeof(uint64_t));
    };

    for (auto& attacks_and_positions : a2p)
    {
        for (auto& position : attacks_and_positions.second)
        {
            auto index = (position * magic) >> shift;


            if (index > maximal_index) {
                maximal_index = index;
                if ((maximal_index+1) >= table_sizes[square_index])
                {
                    clear_table();
                    return false;
                }
            }

            uint64_t& fm = table[index];

            if (fm != 0 && fm != attacks_and_positions.first) {
                clear_table();
                return false;
            }

            fm = attacks_and_positions.first;
        }
    }

    maximal_index++;

    magic_update_lock.lock();
    if (!magics[square_index]) magic_number_count++;
    magics[square_index] = magic;
    shifts[square_index] = shift;
    table_sizes[square_index] = maximal_index;

    cout << "New best magic for square " << square_index << " magic = " << magic << " shift = " << shift
         << " table_len = " << maximal_index << endl;

    magic_update_lock.unlock();

    clear_table();
    return true;
}


bool magic_number_generator::generate_magic_table(int square_index, uint64_t* table)
{
    auto& a2p = a2p_list[square_index];
    uint64_t maximal_index = 0;
    auto magic = magics[square_index];
    auto shift = shifts[square_index];


    auto clear_table = [&]()
    {
        memset(table, 0, maximal_index * sizeof(uint64_t) + sizeof(uint64_t));
    };

    for (auto& attacks_and_positions : a2p)
    {
        for (auto& position : attacks_and_positions.second)
        {
            auto index = (position * magic) >> shift;

            if (index > maximal_index) {
                maximal_index = index;
            }

            uint64_t& fm = table[index];

            if (fm != 0 && fm != attacks_and_positions.first)
            {
                cout << "[ERROR] Could not generate table, encountered improper collision." << endl;

                clear_table();

                cout << "Test magic number output: " << test_magic_number(magic, shift, square_index, table) << endl;
                return false;
            }

            fm = attacks_and_positions.first;
        }
    }

    maximal_index++;
    tables[square_index] = new uint64_t[maximal_index];
    memcpy(tables[square_index], table, maximal_index * sizeof(uint64_t));

    //stress_test_magic_number(square_index);

    clear_table();
    return true;
}



uint64_t magic_number_generator::get_attacks_for_piece(uint64_t position, int idx)
{
    auto index = ((position & masks[idx]) * magics[idx]) >> shifts[idx];
    cout << index << endl;
    return tables[idx][index];
}


void magic_number_generator::stress_test_magic_number(int index, int iterations)
{
    for (; iterations; iterations--)
    {
        auto r = rand64();

        auto masked = r & masks[index];
        auto true_value = attacks[index][masked];

        //auto table_index = (masked * magics[index]) >> shifts[index];
        //auto table_value = tables[index][table_index];

        auto table_value = get_attacks_for_piece(r, index);


        if (table_value != true_value)
        {
            cout << "Stress test failed for index = " << index << endl;
            print_bitboard_list({r, masked, true_value, table_value}, {"Board", "Masked", "True", "Table"});

            //throw "ERROR";
        }
    }
    cout << "Stress test passed" << endl;
}


magic_number_generator::magic_number_generator(basic_attack_map_list& attacks, const char* piece_name,
                                               const char* magic_file, uint64_t* masks, uint64_t initial_shift) :
    attacks(attacks), piece_name(piece_name), magic_file(magic_file),
    shifts(64), magics(64), table_sizes(64, -1), tables(64, nullptr), initial_shift(initial_shift),
    magic_number_count(0), masks(masks)
{
    a2p_list = generate_a2p_list(attacks);
    read_magics();
}


bool magic_number_generator::have_full_set()
{
    return magic_number_count == 64;
}


void magic_number_generator::read_magics()
{
    ifstream ifs(magic_file);

    if (ifs.good()) {
        char file_data[2048] = {0};
        ifs.getline(file_data, 2048, '\n');

        stringstream ss(file_data);

        string number;
        for (int i = 0; i < 64; i++) {
            getline(ss, number, ',');
            magics[i] = stoull(number);
        }

        memset(file_data,0,2048);

        ifs.getline(file_data, 2048);
        ss = stringstream(file_data);

        for (int i = 0; i < 64; i++) {
            getline(ss, number, ',');
            shifts[i] = stoull(number);
        }


        uint64_t temp_table[32000] = {0};
        for (int i = 0; i < 64; i++)
        {
            if (magics[i])
            {
                if (!test_magic_number(magics[i], shifts[i], i, temp_table))
                {
                    cout << "Failed reading magic for " << i << endl;
                }
                else
                {
                    cout << "Read magic number " << magics[i] << " for index " << i << endl;
                    magic_number_count++;
                }
            }
        }
    }
}



void magic_number_generator::search_instance(int thread_id)
{
    int failures = 0;
    uint64_t shift, magic, table_len;

    uint64_t temp_table[32000] = {0};
    while (searching)
    {
        for (int square = 0; square < 64 && searching; square++)
        {
            shift = initial_shift;
            failures = 0;
            magic = rand64();

            while(!test_magic_number(magic, shift, square, temp_table))
            {
                magic = rand64();

                if (failures++ > 100000)
                {
                    failures = 0;
                    if (--shift < (shifts[square]))
                    {
                        table_len = -1;
                        break;
                    }
                }
            }
        }
    }
}

void magic_number_generator::start_search(int threads)
{
    searching = true;
    for (int i = 0; i < threads; i++)
    {
        this_thread::sleep_for(50ms);
        search_threads.push_back(thread(&magic_number_generator::search_instance, this, i));
    }
}

void magic_number_generator::stop_search()
{
    searching = false;
    for (int i = 0; i < search_threads.size(); i++)
        search_threads[i].join();
    search_threads.clear();
}

bool magic_number_generator::generate_magic_tables()
{
    uint64_t temp_table[32000] = {0};
    for (int i = 0; i < 64; i++)
    {
        if (!generate_magic_table(i, temp_table))
        {
            cout << "Failed generating table for square " << i << endl;
            return false;
        }
    }
    return true;
}


void magic_number_generator::export_to_cpp()
{

    cout << "Generating .cpp/.h pair..." << endl;

    if (!have_full_set())
    {
        cout << "You have not generated a full set of magic numbers, aborting export." << endl;
        cout << "Missing numbers for squares: ";

        for (int i = 0; i < 64; i++)
        {
            if (magics[i] == 0)
            {
                cout << i << ", ";
            }
        }
        cout << endl;

        return;
    }

    //vector<uint64_t> best_magics;
    //vector<uint64_t> best_magic_shifts;

    for (auto& i : tables)
    {
        if (!i)
        {
            if (!generate_magic_tables())
            {
                cout << "Could not generate magic tables, aborting export." << endl;
                return;
            }
            break;
        }
    }




    ofstream ofs(piece_name + "_attacks.cpp", ios::trunc);


    auto print_numlist = [&](auto numbers, uint64_t length)
    {
        for (uint64_t i = 0; i < length; i++)
        {
            if (i % 5 == 0 && i != 0) ofs << '\n';

            ofs << int_to_hex(numbers[i]) << ", ";
        }
    };

    if (!ofs.good())
    {
        cout << "Error creating cpp file." << endl;
        return;
    }

    ofs << "\n\n";
    ofs << "#include <cstdint>\n\n";

    ofs << "const uint64_t _" << piece_name << "_masks[64] = {\n";
    print_numlist(masks, 64);

    ofs << "};\n\nconst uint64_t " << piece_name << "_magics[64] = {\n";
    print_numlist(magics, 64);

    ofs << "};\n\n" "const uint64_t " << piece_name << "_shifts[64] = {\n";
    print_numlist(shifts, 64);

    ofs << "};\n\n";

    uint64_t total_size = 0;
    for (int i = 0; i < 64; i++)
    {
        ofs << "const uint64_t " << piece_name << "_table_" << i << '[' << table_sizes[i] << "] = {\n";
        print_numlist(tables[i], table_sizes[i]);

        total_size += table_sizes[i];

        ofs << "};\n\n";
    }


    ofs << " const uint64_t* " << piece_name << "_tables[64] = {\n";

    for (int i = 0; i < 64; i++)
    {
        if (i % 5 == 0 && i != 0) ofs << '\n';

        ofs << piece_name << "_table_" << i << ", ";
    }

    ofs << "};\n\n";

    ofs << "const uint64_t " << piece_name << "_tables_total_size = " << total_size << " * 8;\n\n";


    ofs << "uint64_t get_" << piece_name << "_attacks(uint64_t position, int piece_idx) {\n";

    ofs << "    return " << piece_name << "_tables[piece_idx][((position&_" << piece_name << "_masks[piece_idx]) * " << piece_name <<
        "_magics[piece_idx]) >> " << piece_name << "_shifts[piece_idx]];\n}\n";


    ofs.close();



    ofs.open(piece_name + "_attacks.h", ios::trunc);


    ofs << "#ifndef " << piece_name << "_ATTACKS_HEADER\n#define " << piece_name << "_ATTACKS_HEADER\n\n";
    ofs << "uint64_t get_" << piece_name << "_attacks(uint64_t position, int piece_idx);" << endl;
    ofs << "#endif\n";

    ofs.close();
}




void magic_number_generator::save_magics()
{
    ofstream ofs(magic_file, ios::trunc);

    cout << "Saving magics..." << endl;
    for (auto& i : magics)
    {
        ofs << i << ',';
    }
    ofs << endl;

    for (auto& i : shifts)
    {
        ofs << i << ',';
    }
    ofs.close();
}