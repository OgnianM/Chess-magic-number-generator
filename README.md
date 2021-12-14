<h1>A magic number generator for chess rook and bishop move generation.</h1>

See https://www.chessprogramming.org/Magic_Bitboards for more detailed information.

<code> Usage: magic_generator [bishop | rook] [threads] [maximum shift (optional)] </code>

The generator will search for better magic numbers (that lead to smaller lookup tables) indefinitely, 
if you want to terminate at any point, just CTRL-C and all important data will be saved.

If a full set of magic numbers (for all 64 squares) has been generated, a .cpp/.h file pair will be created on exit.

These 2 files contain all necessary information for move generation.

The header should only contain a function declaration like

<code> uint64_t get_rook_attacks(uint64_t position, int piece_idx); </code>

while the .cpp file will contain all the masks, magic numbers, lookup tables and shifts.

<code>position</code> should contain all pieces on the board, regardless of type, <code>piece_idx</code>
should contain the (zero based) index for the piece for which you want to generate moves.

For example, to generate the possible legal moves for a bishop on c1 do <code>get_bishop_attacks(all_pieces, 2);</code>,
generating moves for a rook on h8 would be <code> get_rook_attacks(all_pieces, 63);</code>

The bit indices are row-major, i.e. <code>a1 == 0, a2 == 1, ..., h1 == 7, ..., a8 == 56, ... h7 == 62, h8 == 63 </code>
 
Aside from the .cpp/.h files, the generator also creates a file <code> best_{piece}_magics.txt</code> which contains
the best magic/shift pair generated so far, if such a file is found (in the working directory), the generator will use it as a base for further searching.

I've included a <code>best_bishop_magics.txt</code> and <code>best_rook_magics.txt</code>, along with their respective .cpp/.h files which I generated after a modest amount of searching, the total size of the tables comes around to about 1 MB.

