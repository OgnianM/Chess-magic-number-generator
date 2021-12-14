#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "bit_manip.h"

#define DOUBLE_FOR for (int x = 0; x < 8; x++) { for (int y = 0; y < 8; y++) {



typedef std::unordered_map<uint64_t, uint64_t> basic_attack_map;
typedef std::vector<basic_attack_map> basic_attack_map_list;

typedef std::unordered_map<uint64_t, std::vector<uint64_t>> attacks_to_positions_map;
typedef std::vector<attacks_to_positions_map> attacks_to_positions_map_list;


extern uint64_t bishop_masks[64];
extern uint64_t rook_masks[64];


using namespace std;
void print_bitboard(bitmask bb, int mark_x=-1, int mark_y=-1, char mark_char=' ');

void print_bitboard_list(std::vector<uint64_t> list, std::vector<std::string> labels);

/*
 * Returns the power set of a set of bits
 */
vector<uint64_t> power_set(uint64_t mask);

void generate_bishop_masks();

void generate_square_ids();

void generate_rook_masks();

basic_attack_map_list generate_rook_attacks();

basic_attack_map_list generate_bishop_attacks();

attacks_to_positions_map map_attacks_to_positions(basic_attack_map const& attacks);

void generate_knight_attacks(uint64_t* knight_attack_masks);

attacks_to_positions_map_list generate_a2p_list(basic_attack_map_list const& attacks);

