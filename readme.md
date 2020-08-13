This is a purely C Chess Engine, made for simulating chess games.

The code is written almost as a pseudocode, using only the basic programming operations (ifses, elses and fors).

Most of the focus is directed toward providing a very fast simulator.

At this stage of development, the code is able to play a random game, and return a winning team. It’d be the backbone of any chess engine.

The most important function is the one named ‘Check’, which tells if a given king is currently under check. It has to be called each time a new move is being evaluated as
valid or not. The verification starts at the king’s position, and looks for the squares it could have been being checked by some enemy piece (looks for knights on knight squares around
the king, looks for pawns, bishops or queens on diagonal direction squares, etc)
