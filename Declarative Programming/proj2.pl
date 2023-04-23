% The SWI Prolog library provides two different, incompatible
% transpose/2 predicates, and unfortunately autoloads the wrong
% one by default. Put below line to use the correct one
:- ensure_loaded(library(clpfd)).



main(PuzzleFile, WordlistFile, SolutionFile) :-
	read_file(PuzzleFile, Puzzle),
	read_file(WordlistFile, Wordlist),
	valid_puzzle(Puzzle),
	solve_puzzle(Puzzle, Wordlist, Solved),
	print_puzzle(SolutionFile, Solved).

% read_file(Filename, Content)
% Open the Filename and save content to Stream.
%
% Filename : the name of a file
% Content  : a list of lines containing a list of characters
read_file(Filename, Content) :-
	open(Filename, read, Stream),
	read_lines(Stream, Content),
	close(Stream).

% read_lines(Stream, Content)
% Recursive reading the Stream one line by one line, save each line to
% the Content.
%
% Stream   : the content of a file
% Content  : a list of lines, a 2d list of characters
% Line     : one line in the file, a list of characters
% Last     : check if it's the last line of content 
read_lines(Stream, Content) :-
	read_line(Stream, Line, Last),
	(   Last = true
	->  (   Line = []
	    ->  Content = []
	    ;   Content = [Line]
	    )
	;  Content = [Line|Content1],
	    read_lines(Stream, Content1)
	).

% read_line(Stream, Line, Last)
% Recursive reading one line character by character and save them into
% variable Line which contains a list of characters, if it
% is the end of file, set variable Last true otherwise false.
%
% Stream : content of file, a list of lines
% Line   : one line of content, a list of characters
% Last   : boolean variable, check if it's end of file
read_line(Stream, Line, Last) :-
	get_char(Stream, Char),
	(   Char = end_of_file
	->  Line = [],
	    Last = true
	; Char = '\n'
	->  Line = [],
	    Last = false
	;   Line = [Char|Line1],
	    read_line(Stream, Line1, Last)
	).

% print_puzzle(SolutionFile, Puzzle)
% Create a new file and write output puzzle (puzzle with words filled)
% 
% SolutionFile : the file name which contains output puzzle
% Puzzle       : output puzzle with words filled
print_puzzle(SolutionFile, Puzzle) :-
	open(SolutionFile, write, Stream),
	maplist(print_row(Stream), Puzzle),
	close(Stream).

% print_row(Stream, Row)
% Read the output puzzle and write it into output file row by row.
%
% Row : one row of output puzzle
print_row(Stream, Row) :-
	maplist(put_puzzle_char(Stream), Row),
	nl(Stream).

% put_puzzle_char(Stream, Char)
% Read one row of output puzzle and write it into the file 
% character by character.
%
% Char  : one character, could be '#', '_' or any single character
put_puzzle_char(Stream, Char) :-
	(   var(Char)
	->  put_char(Stream, '_')
	;   put_char(Stream, Char)
	).

% valid_puzzle([Row|Rows])
% Check if current puzzle is valid - each row has a same length
%
% Row : one row of puzzle
valid_puzzle([]).
valid_puzzle([Row|Rows]) :-
	maplist(same_length(Row), Rows).


% solve_puzzle(Puzzle0, WordList, Puzzle)
% should hold when Puzzle is a solved version of Puzzle0, with the
% empty slots filled in with words from WordList.  Puzzle0 and Puzzle
% should be lists of lists of characters (single-character atoms), one
% list per puzzle row.  WordList is also a list of lists of
% characters, one list per word.

solve_puzzle(Puzzle, Wordlist, Solved) :-
	% Create a puzzle within logical variables and unify them
	copy_puzzle(Puzzle, Solved),
	% Collet all slots in puzzle
	pick_slots_from_puzzle(Solved, Slots),
	% Fill words into slots
	fill_words_in_slots(Slots, Wordlist).



%%%%%%%%%%%%% Copy puzzle will start from here %%%%%%%%%%%
% copy_puzzle(Puzzle, Solved)
% Copy current received puzzle and paste a new puzzle and
% replace '_' with logical variables. It will call copy_puzzle/3 
% within a initial accmulator - a empty list. 
%
% Puzzle : input puzzle
% Solved : output puzzle with logical variables
copy_puzzle(Puzzle, Solved) :-
	copy_puzzle(Puzzle, [], Solved).

% copy_puzzle(Puzzle, Accumulator, SolvedPuzzle)
% Copy input puzzle row by row, and replace '_'
% with logical variables. Use accmulator to save
% each row and save whole rows to SolvedPuzzle if
% already read last row.
%
% Row          : each row of input puzzle
% Accu         : an accmulator, accumulate each whole row 
% SolvedPuzzle : output puzzle within logical variables
copy_puzzle([], Accu, Accu).
copy_puzzle([Row|Rows], Accu, SolvedPuzzle) :-
	% Operate on one row by calling copy_puzzle_by_one_row/2
	copy_puzzle_by_one_row(Row, SolvedRow),
	% accumulate one row
	append(Accu, [SolvedRow], NewAccu),
	% Operate on remainng rows
	copy_puzzle(Rows, NewAccu, SolvedPuzzle).


% copy_puzzle_by_one_row(Row, SolvedRow)
% Operate copy things on one row. It will call copy_puzzle_by_one_row/3
% with a initial accmulator - an empty list
%
% Row       : one row of input puzzle
% SolvedRow : one row of output puzzle
copy_puzzle_by_one_row(Row, SolvedRow) :-
	copy_puzzle_by_one_row(Row, [], SolvedRow).

% copy_puzzle_by_one_row(Row, Accumulator, SolvedRow)
% Read one row character by character, if it is an underscore('_'),
% replace it with logical variable otherwise append it to accmulator.
% Save them to SolvedRow if already read whole row
% 
% X    : current character of one row
% Accu : an accmulator, accumulate each character
% SolvedRow : one row of output puzzle within logical variables
copy_puzzle_by_one_row([], Accu, Accu).
copy_puzzle_by_one_row([X|Xs], Accu, SolvedRow) :-
	( X = '_' ->
		append(Accu,[_], NewAccu),
		copy_puzzle_by_one_row(Xs, NewAccu, SolvedRow)
	; 	append(Accu,[X], NewAccu),
		copy_puzzle_by_one_row(Xs, NewAccu, SolvedRow)
	).
%%%%%%%%%%%%% Copy puzzle will end at here %%%%%%%%%%%%%%%%


%%%%%%%%%% Start from here to collect all slots from puzzle %%%%%%%%%%%
% pick_slots_from_puzzle(Puzzle, Slots)
% Collet all slots in the puzzle. It will call pick_slots_from_puzzle/3
% with an inital accmulator - an empty list.
%
% Puzzle   : an input puzzle
% Slots    : a sequence of fill-able and pre-filled squares
pick_slots_from_puzzle(Puzzle, Slots) :-
	pick_slots_from_puzzle(Puzzle, [], Slots).

% pick_slots_from_puzzle(Puzzle, Accu, TotalSlots)
% Collect all horizontal slots row by row in the puzzle. Transpose the puzzle
% and collect slots (vertical slots in the original puzzle) again. After 
% reading all rows,add two lists of slots together and save them to totalslots
%
% Puzzle     : an input puzzle
% Accu       : an accmulator, accumulates collected slots
% TotalSlots : all slots (horizontal slots and vertical slots) of the puzzle
pick_slots_from_puzzle([], Accu, Accu).
pick_slots_from_puzzle(Puzzle, Accu, TotalSlots) :-

	pick_slots_from_all_rows(Puzzle, RowSlots),
	transpose(Puzzle, TransposedPuzzle),
	pick_slots_from_all_rows(TransposedPuzzle, ColSlots),

	append(RowSlots, Accu, NewAccu),
	append(ColSlots, NewAccu, TotAccu),
	pick_slots_from_puzzle([], TotAccu, TotalSlots).

% pick_slots_from_all_rows(Puzzle, RowSlots)
% Collet all slots from all rows of puzzle. It will call 
% copy_puzzle_by_one_row/3 with an inital accmulator - an empty list.
% 
% Puzzle   : an input puzzle
% RowSlots : all horizontal slots of the puzzle
pick_slots_from_all_rows(Puzzle, RowSlots) :-
	pick_slots_from_all_rows(Puzzle, [], RowSlots).


% pick_slots_from_all_rows([Row|RestRows], Accu, RowSlots)
% Read the puzzle row by row and collect slots. Accumulator will accumulates
% slots of each row and save them to RowSlots if already read all rows.
%
% Row      : one row of puzzle
% Accu     : an accmulator, accumulates slots of each row
% RowSlots : slots from all rows of the puzzle
pick_slots_from_all_rows([], Accu, Accu).
pick_slots_from_all_rows([Row|RestRows], Accu, RowSlots) :-
	pick_slots_from_one_row(Row, OneRowSlots),
	append(OneRowSlots, Accu, NewAccu),
	pick_slots_from_all_rows(RestRows, NewAccu, RowSlots).

% pick_slots_from_one_row(Row, OneRowSlots)
% Collet slot(s) from one row. It will call pick_slots_from_one_row/4 with 2
% inital accmulators - two empty list
%
% Row         : one row of puzzle
% OneRowSlots : all slots of one row
pick_slots_from_one_row(Row, OneRowSlots) :-
	pick_slots_from_one_row(Row, [], [], OneRowSlots).

% pick_slots_from_one_row(Row, OneSlot, Accu, OneRowSlots)
% Two base caeses. 
% First one is when already read whole row and current slot is empty, save
% content of Accu which contains all slots from one line to OneRowSlots.
% Second one is when already read whole row and current slot is not empty,
% check if current is valid.Accu will accumulates this slot if valid otherwise
% drop the slot and call first base case.
%
% OneSlot : an accumulator, accumulates characters
% Accu    : an accmulator containing one or more slots from current row
% OneRowSlots : all slots from current row
pick_slots_from_one_row([], [], Accu, Accu).
pick_slots_from_one_row([], OneSlot, Accu, OneRowSlots) :-
	length(OneSlot, Len),
	( Len > 1 ->
	 	append(Accu, [OneSlot], NewAccu),
	 	pick_slots_from_one_row([], [], NewAccu, OneRowSlots)
	;	pick_slots_from_one_row([], [], Accu, OneRowSlots)
	).

% pick_slots_from_one_row(Row, OneSlot, Accu, OneRowSlots)
% Read current row character by character. Save characters into OneSlot
% before meeting '#'. If current character is '#', check if OneSlot is
% valid. Accu accumulates current slot if it is valid otherwise drop it off.
% 
% OneSlot : an accumulator, accumulates characters of one slot
% Accu    : an accmulator containing one or more slots from current row
% OneRowSlots : all slots from current row
pick_slots_from_one_row([X|Xs], OneSlot, Accu, RowSlots) :-
	% if current character is not #, append it to OneSlot
	( X \== '#' ->
		append(OneSlot, [X], NewOneSlot),
		pick_slots_from_one_row(Xs, NewOneSlot, Accu, RowSlots)

	;	(OneSlot == [] ->
			pick_slots_from_one_row(Xs, [], Accu, RowSlots)
		;	length(OneSlot, Len), 

			( Len > 1 ->
				append(Accu, [OneSlot], NewAccu),
				pick_slots_from_one_row(Xs, [], NewAccu, RowSlots)
			;	pick_slots_from_one_row(Xs, [], Accu, RowSlots)
			)
		)
	).
%%%%%%%%%% Collect slots will end here %%%%%%%%%%%%%%%%%%


%%%%%%%%%% Start from here to fill words into slots %%%%%%%%%%%%%%%%%
% fill_words_in_slots(Slots, Wordlist)
% Fill words into slots. Find out one slot with least matching words and
% fill it then remove this filled slot from slots list and remove used words
% from words list.
%
% Slots : all slots from puzzle
% Wordlist : a list of words which use to fill slots
fill_words_in_slots([], []).
fill_words_in_slots(Slots, Wordlist) :-
	% Find out one slot with least matching words
	slot_with_least_matching_words(Slots, Wordlist, FinalSlot),
	% prune the word list by leaving the same length between slot and word
	include(same_length(FinalSlot), Wordlist, PrunedWordlist),
	% Choose one word from pruned word list
    member(Word, PrunedWordlist),
    % fill word into the slot if applicable
    FinalSlot = Word,
    % remove uesd word and filed slot from WordList and slots seprately
    exclude(==(Word), Wordlist, RemainingWords),
    exclude(==(FinalSlot), Slots, RemainingSlots),
    fill_words_in_slots(RemainingSlots, RemainingWords).

% slot_matching_words(Slot, Wordlist, Count)
% Count the number of matching words for the slot. It will call
% slot_matching_words/4 with an initial accmulator - initial value is 0
%
% Slot : one slot
% Wordlist : a list of words
% Count    : the amount of matching words
slot_matching_words(Slot, Wordlist, Count) :-
	slot_matching_words(Slot, Wordlist, 0, Count).

% slot_matching_words(Slot, Words, Accu, Count)
% Count the amount of words that can be filled into slot.
% Save it to Count after reading all words in wordlist
%
% Slot : one slot
% Word : one word from wordlist
% Accu : an accumulator, accumulates the amount of matching words
% Count : the final value of Accu
slot_matching_words(_, [], Accu, Accu).
slot_matching_words(Slot, [Word|Words], Accu, Count) :-
	length(Slot, LenOfSlot),
	length(Word, LenOfWord),
	(LenOfSlot == LenOfWord ->
		slot_matching_words(Slot, Words, Accu+1, Count)
	; 	slot_matching_words(Slot, Words, Accu, Count)
	).


% slot_with_least_matching_words(AllSlots, Wordlist, FinalSlot)
% Choose first slot from slot list and count the matching words and use it
% to compare with other slots. It will call slot_with_least_matching_words/5 
% with the first slot and its count of matching words to be compared.
%
% AllSlots  : a list of slots
% Wordlist : a list of words
% FinalSlot : the slot with least matching words
slot_with_least_matching_words([Slot|Slots], Wordlist, FinalSlot) :-
	slot_matching_words(Slot, Wordlist, Count),
	slot_with_least_matching_words(Slots, Wordlist, FinalSlot, Count, Slot).

% slot_with_least_matching_words(Slots, Wordlist, FinalSlot, LM, LMSlot)
% Compute the count of matching words of slot from slots one by one and
% choose the slot with the smallest count as final slot we used to be filled 
% 
% Slot : current slot from slots list
% Wordlist : a list of words
% FinalSlot : the slot with the least matching words from all slots
% LM        : the count of least matching words
% LMSlot    : current slot with the least matching words
slot_with_least_matching_words([], _, FinalSlot, _, FinalSlot).
slot_with_least_matching_words([Slot|Slots],Wordlist, FinalSlot, LM, LMSlot):-
	slot_matching_words(Slot, Wordlist, Count),
	% If the slot has only one word to match, then cut down and pass  
	% the slot to be filled at first.
	(Count == 1 ->
		slot_with_least_matching_words([], Wordlist, FinalSlot, Count, Slot)
	;	(Count < LM ->
		   slot_with_least_matching_words(Slots,Wordlist,FinalSlot,Count,Slot)
		;  slot_with_least_matching_words(Slots,Wordlist,FinalSlot,LM,LMSlot)
		)
	).
%%%%%%%%%%%%%% Fill words will end at here %%%%%%%%%%%%%%%%%%%%%%%%	