//
// Created by ognian on 6.10.21 г..
//

#include "generation_functions.h"
#include <random>



uint64_t rook_masks[64] = {
        282578800148862,
        565157600297596,
        1130315200595066,
        2260630401190006,
        4521260802379886,
        9042521604759646,
        18085043209519166,
        36170086419038334,
        282578800180736,
        565157600328704,
        1130315200625152,
        2260630401218048,
        4521260802403840,
        9042521604775424,
        18085043209518592,
        36170086419037696,
        282578808340736,
        565157608292864,
        1130315208328192,
        2260630408398848,
        4521260808540160,
        9042521608822784,
        18085043209388032,
        36170086418907136,
        282580897300736,
        565159647117824,
        1130317180306432,
        2260632246683648,
        4521262379438080,
        9042522644946944,
        18085043175964672,
        36170086385483776,
        283115671060736,
        565681586307584,
        1130822006735872,
        2261102847592448,
        4521664529305600,
        9042787892731904,
        18085034619584512,
        36170077829103616,
        420017753620736,
        699298018886144,
        1260057572672512,
        2381576680245248,
        4624614895390720,
        9110691325681664,
        18082844186263552,
        36167887395782656,
        35466950888980736,
        34905104758997504,
        34344362452452352,
        33222877839362048,
        30979908613181440,
        26493970160820224,
        17522093256097792,
        35607136465616896,
        9079539427579068672,
        8935706818303361536,
        8792156787827803136,
        8505056726876686336,
        7930856604974452736,
        6782456361169985536,
        4485655873561051136,
        9115426935197958144,
};

uint64_t bishop_masks[64] =
{
        18049651735527936,
        70506452091904,
        275415828992,
        1075975168,
        38021120,
        8657588224,
        2216338399232,
        567382630219776,
        9024825867763712,
        18049651735527424,
        70506452221952,
        275449643008,
        9733406720,
        2216342585344,
        567382630203392,
        1134765260406784,
        4512412933816832,
        9024825867633664,
        18049651768822272,
        70515108615168,
        2491752130560,
        567383701868544,
        1134765256220672,
        2269530512441344,
        2256206450263040,
        4512412900526080,
        9024834391117824,
        18051867805491712,
        637888545440768,
        1135039602493440,
        2269529440784384,
        4539058881568768,
        1128098963916800,
        2256197927833600,
        4514594912477184,
        9592139778506752,
        19184279556981248,
        2339762086609920,
        4538784537380864,
        9077569074761728,
        562958610993152,
        1125917221986304,
        2814792987328512,
        5629586008178688,
        11259172008099840,
        22518341868716544,
        9007336962655232,
        18014673925310464,
        2216338399232,
        4432676798464,
        11064376819712,
        22137335185408,
        44272556441600,
        87995357200384,
        35253226045952,
        70506452091904,
        567382630219776,
        1134765260406784,
        2832480465846272,
        5667157807464448,
        11333774449049600,
        22526811443298304,
        9024825867763712,
        18049651735527936,
};


vector<uint64_t> power_set(uint64_t mask)
{
    vector<uint64_t> result = {0}; // zero is in every power set
    vector<uint64_t> ones;


    uint64_t n = 1;
    for (int i = 0; i < 64; i++)
    {
        if (mask & n) ones.push_back(n);
        n <<= 1;
    }

    uint64_t limit = pow(2, ones.size());

    uint64_t bits;

    for (uint64_t i = 1; i < limit; i++)
    {
        n = 1;
        bits = 0;
        for (uint64_t z = 0; z < ones.size(); z++)
        {
            if (i & n)
                bits |= ones[z];
            n <<= 1;
        }
        result.push_back(bits);
    }

    return result;
}


void generate_bishop_masks()
{
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            bitmask mask = 0;
            for (int yoff = y-1, xoff = x-1; yoff > 0 && xoff > 0; yoff--, xoff--)
            {
                set_bit(mask,xoff,yoff);
            }
            for (int yoff = y-1, xoff = x+1; yoff > 0 && xoff < 7; yoff--, xoff++)
            {
                set_bit(mask,xoff,yoff);

            }
            for (int yoff = y+1, xoff = x-1; yoff < 7 && xoff > 0; yoff++, xoff--)
            {
                set_bit(mask,xoff,yoff);

            }
            for (int yoff = y+1, xoff = x+1; yoff < 7 && xoff < 7; yoff++, xoff++)
            {
                set_bit(mask,xoff,yoff);
            }
            bishop_masks[get_index(x,y)] = mask;
        }
    }
}



void generate_square_ids()
{
    uint64_t n = 1;
    for (int i = 1; i < 9; i++)
    {
        for (char c = 'a'; c <= 'h'; c++)
        {
            cout << c << i << " = " << n << ",\n";
            n <<= 1;
        }
    }
}



void generate_rook_masks()
{
    bitmask mask = 0;

    auto do_ = [&](int x, int y, int x_, int y_)
    {
        for (int xoff = x+x_, yoff = y + y_; ((xoff < 7 && xoff > 0) || !x_) && ((yoff < 7 && yoff > 0) || !y_); xoff += x_, yoff += y_)
        {
            set_bit(mask, xoff,yoff);
        }
    };
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            mask = 0;
            do_(x,y,1,0);
            do_(x,y,-1,0);
            do_(x,y,0,1);
            do_(x,y,0,-1);

            rook_masks[get_index(x,y)] = mask;
        }
    }
}


basic_attack_map_list generate_rook_attacks()
{
    auto get_attacks_given_occupancies = [](int const& origin_x, int const& origin_y, bitmask const& occupancies)
    {
        bitmask attacks = 0;
        for (int x = origin_x + 1; x < 8; x++)
        {
            set_bit(attacks, x,origin_y);
            if (occupancies & get_bit(x, origin_y))
                break;
        }

        for (int x = origin_x - 1; x >= 0; x--)
        {
            set_bit(attacks, x,origin_y);
            if (occupancies & get_bit(x, origin_y))
                break;
        }

        for (int y = origin_y + 1; y < 8; y++)
        {
            set_bit(attacks, origin_x, y);
            if (occupancies & get_bit(origin_x, y))
                break;
        }

        for (int y = origin_y - 1; y >= 0; y--)
        {
            set_bit(attacks, origin_x, y);
            if (occupancies & get_bit(origin_x, y))
                break;
        }

        return attacks;
    };


    basic_attack_map_list result(64);

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            auto mask = rook_masks[get_index(x,y)];

            for (auto& pos : power_set(mask))
                result[get_index(x,y)][pos] = get_attacks_given_occupancies(x,y, pos);
        }
    }
    return result;
}





attacks_to_positions_map map_attacks_to_positions(basic_attack_map const& attacks)
{
    unordered_map<uint64_t, vector<uint64_t>> result;

    for (auto& i : attacks)
        result[i.second].push_back(i.first);

    return result;
}


basic_attack_map_list generate_bishop_attacks()
{
    auto get_attacks_given_occupancies = [](int const& origin_x, int const& origin_y, bitmask const& occupancies)
    {
        bitmask attacks = 0;

        for (int xoff = origin_x - 1, yoff = origin_y - 1; xoff >= 0 && yoff >= 0; xoff--, yoff--)
        {
            set_bit(attacks, xoff, yoff);
            if (occupancies & get_bit(xoff, yoff)) break;
        }


        for (int xoff = origin_x - 1, yoff = origin_y + 1; xoff >= 0 && yoff < 8; xoff--, yoff++)
        {
            set_bit(attacks, xoff, yoff);
            if (occupancies & get_bit(xoff, yoff)) break;
        }


        for (int xoff = origin_x + 1, yoff = origin_y - 1; xoff < 8 && yoff >= 0; xoff++, yoff--)
        {
            set_bit(attacks, xoff, yoff);
            if (occupancies & get_bit(xoff, yoff)) break;
        }

        for (int xoff = origin_x + 1, yoff = origin_y + 1; xoff <8 && yoff < 8; xoff++, yoff++)
        {
            set_bit(attacks, xoff, yoff);
            if (occupancies & get_bit(xoff, yoff)) break;
        }

        return attacks;
    };

    basic_attack_map_list result(64);

    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            auto mask = bishop_masks[get_index(x,y)];

            for (auto& pos : power_set(mask))
            {
                result[get_index(x,y)][pos] = get_attacks_given_occupancies(x,y, pos);
            }
        }
    }
    return result;
}



void generate_knight_attacks(uint64_t* knight_attack_masks)
{
    bitmask b;

    auto mk_attack = [&](int x, int y)
    {
        if (x >= 0 && x < 8 && y >= 0 && y < 8)
        {
            b |= (uint64_t(1) << (y*8+x));
        }
    };

    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            int knight_idx = y*8 + x;
            b = 0;

            mk_attack(x+1, y+2);
            mk_attack(x+1, y-2);
            mk_attack(x-1, y+2);
            mk_attack(x-1, y-2);

            mk_attack(x+2, y+1);
            mk_attack(x+2, y-1);
            mk_attack(x-2, y+1);
            mk_attack(x-2, y-1);

            knight_attack_masks[knight_idx] = b;
        }
    }
}

void print_bitboard(bitmask bb, int mark_x, int mark_y, char mark_char)
{
    uint64_t bit = 1;
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (mark_x == x && mark_y == y) cout << mark_char;
            else
            if (bb & bit)
                cout << 1;
            else cout << 0;

            bit <<= 1;
            cout << "  ";
        }
        cout << endl;
    }
    cout << endl;
}


attacks_to_positions_map_list generate_a2p_list(basic_attack_map_list const& attacks)
{
    attacks_to_positions_map_list a2p_list;

    for(auto& i : attacks)
        a2p_list.push_back(map_attacks_to_positions(i));
    return a2p_list;
}



void print_bitboard_list(std::vector<uint64_t> list, std::vector<std::string> labels)
{
    for (int i = 0; i < labels.size(); i++)
    {
        cout << labels[i];
        for (int y = labels[i].size(); y < 9*3; y++)
            cout << ' ';
    }
    cout << endl;
    for (int y = 0; y < 8; y++)
    {
        for (int i = 0; i < list.size(); i++)
        {
            for (int x = 0; x < 8; x++)
            {
                if (list[i] & get_bit(x,y))
                    cout << "1  ";
                else cout << "0  ";
            }
            cout << "   ";
        }
        cout << endl;
    }
    cout << endl;
}