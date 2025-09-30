#include <iostream>
#include <array>
#include <vector>
#include <bitset>
#include <map>
#include <unordered_map>
#include <string>
#include <sstream>
#include <limits>
#include <functional>

//typedef's
typedef unsigned long long U64;
typedef const U64 C64;
typedef unsigned char U8; 



// Debug mode defininitions
#define DEBUG_MODE 1 // 0 = off, 1 = basic debug, 2 = verbose debug, 3 = both

#if DEBUG_MODE == 0
	#define DEBUG_BASIC 0
	#define DEBUG_VERBOSE 0
#elif DEBUG_MODE == 1
	#define DEBUG_BASIC 1
	#define DEBUG_VERBOSE 0
#elif DEBUG_MODE == 2
	#define DEBUG_BASIC 0
	#define DEBUG_VERBOSE 1
#elif DEBUG_MODE == 3
	#define DEBUG_BASIC 1
	#define DEBUG_VERBOSE 1
#endif

#if DEBUG_BASIC
inline void DebugBitboard(U64 BitBoard, const std::string& Name) {
	std::bitset<64> bits(BitBoard);

	std::cout << "BITBOARD_NAME::" << Name << "\n";

	for (int row = 0; row < 8; ++row) {
		for (int column = 0; column < 8; ++column) {
			std::cout << bits[(7 - row) * 8 + column] << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

#else
// Empty inline — compiler erases calls completely in release
inline void DebugBitboard(U64, const std::string&) {}
#endif

#if DEBUG_VERBOSE
template<typename... Args>
inline void debug(Args&&... args) {
	((std::cout << std::forward<Args>(args) << " "), ...);
	std::cout << "\n";
}
#else
// Compiles away completely in release
template<typename... Args>
inline void debug(Args&&...) {}
#endif

struct MagicBitBoards {
	//Magic bitboards for sliding pieces (rook, bishop, queen)
	//TODO
};

// Stored bitboards
struct BitBoards {
	static constexpr U64 StartBitBoard = 0b1111111111111111000000000000000000000000000000001111111111111111;
	static constexpr U64 WPawnStartBitBoard = 0b0000000000000000000000000000000000000000000000001111111100000000;
	static constexpr U64 BPawnStartBitBoard = 0b0000000011111111000000000000000000000000000000000000000000000000;

	//Pieces
	U64 PieceBitBoard = 0b1111111111111111000000000000000000000000000000001111111111111111;
	U64 OccBitBoard = 0b1111111111111111000000000000000000000000000000001111111111111111;
	
	U64 WhitePawnBitBoard = 0b0000000000000000000000000000000000000000000000001111111100000000;
	U64 BlackPawnBitBoard = 0b0000000011111111000000000000000000000000000000000000000000000000;

	U64 WhiteKnightBitBoard = 0b0000000000000000000000000000000000000000000000000000000001000010;
	U64 BlackKnightBitBoard = 0b0100001000000000000000000000000000000000000000000000000000000000;

	U64 WhiteKingBitBoard = 0b0000000000000000000000000000000000000000000000000000000000001000;
	U64 BlackKingBitBoard = 0b0000100000000000000000000000000000000000000000000000000000000000;

	
	// Files
	static constexpr U64 FileBBitBoard = 0b0000001000000010000000100000001000000010000000100000001000000010;
	static constexpr U64 FileABitBoard = 0b0000000100000001000000010000000100000001000000010000000100000001;
	static constexpr U64 FileGBitBoard = 0b0100000001000000010000000100000001000000010000000100000001000000;
	static constexpr U64 FileHBitBoard = 0b1000000010000000100000001000000010000000100000001000000010000000;

	// Ranks
	static constexpr U64 Rank1BitBoard = 0b0000000000000000000000000000000000000000000000000000000011111111;
	static constexpr U64 Rank2BitBoard = 0b0000000000000000000000000000000000000000000000001111111100000000;
	static constexpr U64 Rank7BitBoard = 0b0000000011111111000000000000000000000000000000000000000000000000;
	static constexpr U64 Rank8BitBoard = 0b1111111100000000000000000000000000000000000000000000000000000000;


	static constexpr U64 VariableBitBoard[5] = {
	0b0,// Tühja bitboardi jaoks
	0b1111111111111111111111110000000000000000111111111111111111111111,// Koik blockitud essad sqrid
	0b1111111111111111000000000000000000000000000000001111111111111111,// Default
	0b1111111100000000000000000000000000000000000000000000000011111111,// 0 pawns
	0b0100001000000000000000000000000000000000000000000000000001000010,// knights
	};
};


class Logic {
public:
	std::vector<U64> LegalMoves;


	void DebugMode() {
		BitBoards BB;
		Logic logic;

		std::cout << ".help for commands\n";
		int color = 0;
		int defaultcommand = 2;


		// Command loop
		std::unordered_map<std::string, std::function<void(std::stringstream&)>> commands;

		// 2️ Define the helper right after the map declaration
		auto addCommand = [&](std::initializer_list<std::string> keys,
			std::function<void(std::stringstream&)> fn) {
				for (auto& k : keys) commands[k] = fn;
			};


		addCommand({ ".help" }, [&](std::stringstream&) {
			std::cout << "\n.exit - Exit the program\n"
				<< ".color [0/1] - Set color (0=White, 1=Black)\n"
				<< ".bitboard [0-3] - Use a specific OccBitBoard\n"
				<< ".runpawns - Run pawn logic\n"
				<< ".runknights - Run knight logic\n"
				<< ".runkings - Run king logic\n";

		});

		addCommand({ ".color", ".c"}, [&](std::stringstream& ss) {
			int newColor; ss >> newColor;
			if (newColor == 0 || newColor == 1) {
				color = newColor;
				std::cout << "Color set to " << (color == 0 ? "White" : "Black") << "\n";
			}
			else {
				std::cout << "Error: Color must be 0 or 1.\n";
			}
		});
				
		addCommand({ ".bitboard", ".bb" }, [&](std::stringstream& ss) {
			int idx; ss >> idx;	
			if (idx >= 0 && idx < 5) {
				PrintBitboard(BB.VariableBitBoard[idx], "OccBitBoard");
				defaultcommand = idx;
			}
			else {
				std::cout << "Error: Bitboard index must be 0–4.\n";
			}
		});

		addCommand({ ".runpawns", "rp"}, [&](std::stringstream&) {
			PrintBitboard(BB.VariableBitBoard[defaultcommand], "OccBitBoard");
			logic.PawnLogic(color, defaultcommand);
			logic.Flush();
		});

		addCommand({ ".runknights",".rn"}, [&](std::stringstream&) {
			PrintBitboard(BB.VariableBitBoard[defaultcommand], "OccBitBoard");
			logic.KnightLogic(color, defaultcommand);
			logic.Flush();
		});

		addCommand({ ".runkings", ".rk"}, [&](std::stringstream&) {
			PrintBitboard(BB.VariableBitBoard[defaultcommand], "OccBitBoard");
			logic.KingLogic(color, defaultcommand);
			logic.Flush();
		});

		addCommand({ "", ".runall"}, [&](std::stringstream&) { // enter key = run all
			PrintBitboard(BB.VariableBitBoard[defaultcommand], "OccBitBoard");
			logic.PawnLogic(color, defaultcommand);
			logic.KnightLogic(color, defaultcommand);
			logic.KingLogic(color, defaultcommand);
			logic.Flush();
		});

		while (true) {
			std::cout << "> ";
			std::string line; std::getline(std::cin, line);
			std::stringstream ss(line);
			std::string command; ss >> command;

			// Command aliases
			if (command == ".bb") command = ".bitboard";
			if (command == ".exit") break;
			 
			auto it = commands.find(command);
			if (it != commands.end()) it->second(ss); 
			else if (!command.empty()) std::cout << "Error: Command not found\n"; 
			else commands[""](ss); // empty input
		}
	}

	unsigned int Player(unsigned int turn_num) {
		// 0 -> White, 1 -> Black
		if (turn_num % 2 == 0) {
			return 0;
		}
		else {
			return 1;
		}
	}
	//TODO: asenda koik vectorid U64 arraydega
	// int Color = 0 - White, 1->Black
	void PawnLogic(int Color
#if DEBUG_MODE
		, int debugInput  // default value for release mode compatibility
#endif
	) {
		BitBoards BB;       // always the same BB
		U64 OccBitBoard;


#if DEBUG_MODE
		OccBitBoard = BB.VariableBitBoard[debugInput];
#else
		U64& OccBitBoard = BB.OccBitBoard;
#endif

		BB.StartBitBoard;
		U64 PBitBoard = BB.PieceBitBoard;
		U64 StartSqPawns = 0;
		U64 future_move[4];
		std::vector<U64> AlivePawns;
		std::vector<U64> NotMovedPawns;
		uint8_t count = 0; // idk mai maleta miks see siin peab olema aga ilma ei toota
		int pawn_move_count = 0;// For later use on array navigation

		//Determine color
		if (Color == 0) {
			TurnedBitLoc(BB.WhitePawnBitBoard, AlivePawns);
			StartSqPawns = BB.WhitePawnBitBoard & BB.StartBitBoard;
			TurnedBitLoc(StartSqPawns, NotMovedPawns);
			count = countSetBits(StartSqPawns);
			std::cout << "Color: White" << "\n";
		}
		else if (Color == 1) {
			TurnedBitLoc(BB.BlackPawnBitBoard, AlivePawns);
			StartSqPawns = BB.BlackPawnBitBoard & BB.StartBitBoard;
			TurnedBitLoc(StartSqPawns, NotMovedPawns);
			count = countSetBits(StartSqPawns);
			std::cout << "Color: Black" << "\n";
		}

		//Printing Bitboards
		PrintBitboard(BB.PieceBitBoard, "PieceBitBoard");

		//Pawns
		//Single move (1 sq)
		for (auto pawn : AlivePawns) {
			const int pawnloc = SingleTurnedBitLoc(pawn);
			future_move[0] = BitMapMoveBit(PBitBoard, pawnloc, 8, Color);
			if (!IsColliding(OccBitBoard, pawnloc, 8, Color)) {
				LegalMoves.push_back(future_move[0]);
				size_t vec_size = LegalMoves.size();

				DebugBitboard(future_move[0], "Future move [0]");
				debug("Amount of legal moves for loop [0]:", vec_size);

				pawn_move_count++;
			}
		}

		//Double move (2 sq)
		for (auto pawn : NotMovedPawns) {
			const int pawnloc = SingleTurnedBitLoc(pawn);
			future_move[1] = BitMapMoveBit(PBitBoard, pawnloc, 16, Color);
			if (!IsColliding(OccBitBoard, pawnloc, 16, Color) && !IsBlocked(OccBitBoard, 1, pawnloc, 16, Color)) {
				LegalMoves.push_back(future_move[1]);
				size_t vec_size = LegalMoves.size();

				DebugBitboard(future_move[1], "Future move [1]");
				debug("Amount of legal moves for loop [1]:", vec_size);

				pawn_move_count++;
			}
		}

		//Capture moves (diagonal 1 sq)
		for (auto pawn : AlivePawns) {
			const int pawnloc = SingleTurnedBitLoc(pawn);
			U64 Capture_able_board = CanCapture(OccBitBoard, 1, pawnloc, Color);
			int file = (pawnloc % 8);
			file += 1;

			if (OuterFileCheck(pawnloc) == 0 && Capture_able_board != 0) {

				U64 capture_able_test_right = BitMapMoveBit(PBitBoard, pawnloc, 9, Color);
				U64 capture_able_test_left = BitMapMoveBit(PBitBoard, pawnloc, 7, Color);

				//Check wich side we can capture if only 1 is capture able
				if ((capture_able_test_left & Capture_able_board) != 0) {
					future_move[2] = BitMapMoveBit(PBitBoard, pawnloc, 9, Color);
					LegalMoves.push_back(future_move[2]);

					size_t vec_size = LegalMoves.size();
					DebugBitboard(future_move[2], "future move[3]");
					debug("Amount of legal moves for loop [3]:", vec_size);

					pawn_move_count++;
				}
				if ((capture_able_test_right & Capture_able_board) != 0) {
					future_move[3] = BitMapMoveBit(PBitBoard, pawnloc, 7, Color);
					LegalMoves.push_back(future_move[3]);

					size_t vec_size = LegalMoves.size();
					DebugBitboard(future_move[3], "future move[4]");
					debug("Amount of legal moves for loop [4]:", vec_size);
					pawn_move_count++;
				}
			}

			//Kui ettur on laua ääres, siis teeb ainult ühe suunalise rünnaku
			else {
				if (file == 1 && Capture_able_board != 0) {
					if (Color == 0) {
						future_move[2] = BitMapMoveBit(PBitBoard, pawnloc, 9, Color); // Kui ettur on esimese faili peal siis votame ainult paremale
					}
					else {
						future_move[2] = BitMapMoveBit(PBitBoard, pawnloc, 7, Color); // Kui ettur on esimese faili peal siis votame ainult paremale
					}

					DebugBitboard(future_move[2], "future move[1]_special_file 0");
					LegalMoves.push_back(future_move[2]);
					size_t vec_size = LegalMoves.size();
					debug("Amount of legal moves for loop [3]:", vec_size);

					pawn_move_count++;
				}
				if (file == 8 && Capture_able_board != 0) {
					if (Color == 0) {
						future_move[3] = BitMapMoveBit(PBitBoard, pawnloc, 7, Color); // Kui ettur on kaheksanda faili peal siis votame ainult vasakule
					}
					else {
						future_move[3] = BitMapMoveBit(PBitBoard, pawnloc, 9, Color); // Kui ettur on kaheksanda faili peal siis votame ainult vasakule
					}

					DebugBitboard(future_move[3], "future move[0]_special_file 8");
					LegalMoves.push_back(future_move[3]);
					size_t vec_size = LegalMoves.size();
					debug("Amount of legal moves for loop [4]:", vec_size);

					pawn_move_count++;
				}
			}
		}
	}

	void KnightLogic(int Color
#if DEBUG_MODE
		, int debugInput  // default value for release mode compatibility
#endif
	) {
		BitBoards BB;// always the same BB
		U64 OccBitBoard;
		std::vector<U64> AliveKnights;
		U64 PbitBoard = 0;
		if (Color == 0) {
			TurnedBitLoc(BB.WhiteKnightBitBoard, AliveKnights);
			PbitBoard = BB.PieceBitBoard;
			std::cout << "Color: White" << "\n";
		}
		else if (Color == 1) {
			TurnedBitLoc(BB.BlackKnightBitBoard, AliveKnights);
			PbitBoard = BB.PieceBitBoard;
			std::cout << "Color: Black" << "\n";
		}

#if DEBUG_MODE
		OccBitBoard = BB.VariableBitBoard[debugInput];
#else
		OccBitBoard = BB.OccBitBoard;
#endif


		//uint8_t count = 0; // idk mai maleta miks see siin peab olema aga ilma ei toota
		int knight_move_count = 0;// For later use on array navigation

		U64 future_move = 0;

		for (auto Knight : AliveKnights) {

			// NoNoWe +15 
			if (((Knight & (BB.FileHBitBoard | BB.Rank7BitBoard | BB.Rank8BitBoard)) == 0) && (!IsColliding(OccBitBoard, SingleTurnedBitLoc(Knight), 15, 0))) {
				future_move = BitMapMoveBit(PbitBoard, SingleTurnedBitLoc(Knight), 15, 0);
				DebugBitboard(future_move, "Future move +15");
				LegalMoves.push_back(future_move);
			}
			// NoNoEa +17
			if ((Knight & (BB.FileABitBoard | BB.Rank7BitBoard | BB.Rank8BitBoard)) == 0 && (!IsColliding(OccBitBoard, SingleTurnedBitLoc(Knight), 17, 0))) {
				future_move = BitMapMoveBit(PbitBoard, SingleTurnedBitLoc(Knight), 17, 0);
				DebugBitboard(future_move, "Future move +17");
				LegalMoves.push_back(future_move);
			}
			// NoWeWe +6
			if ((Knight & (BB.FileBBitBoard | BB.FileABitBoard | BB.Rank8BitBoard)) == 0 && (!IsColliding(OccBitBoard, SingleTurnedBitLoc(Knight), 6, 0))) {
				future_move = BitMapMoveBit(PbitBoard, SingleTurnedBitLoc(Knight), 6, 0);
				DebugBitboard(future_move, "Future move +6");
				LegalMoves.push_back(future_move);
			}
			//NoEaEa +10
			if ((Knight & (BB.FileHBitBoard | BB.FileGBitBoard | BB.Rank8BitBoard)) == 0 && (!IsColliding(OccBitBoard, SingleTurnedBitLoc(Knight), 10, 0))) {
				future_move = BitMapMoveBit(PbitBoard, SingleTurnedBitLoc(Knight), 10, 0);
				DebugBitboard(future_move, "Future move +10");
				LegalMoves.push_back(future_move);
			}

			// miinustega moovid

			// SoSoEa -15
			if ((Knight & (BB.FileABitBoard | BB.Rank1BitBoard | BB.Rank2BitBoard)) == 0 && (!IsColliding(OccBitBoard, SingleTurnedBitLoc(Knight), 15, 1))) {
				future_move = BitMapMoveBit(PbitBoard, SingleTurnedBitLoc(Knight), 15, 1);
				DebugBitboard(future_move, "Future move -15");
				LegalMoves.push_back(future_move);
			}
			// SoSoEa -17
			if ((Knight & (BB.FileHBitBoard | BB.Rank1BitBoard | BB.Rank2BitBoard)) == 0 && (!IsColliding(OccBitBoard, SingleTurnedBitLoc(Knight), 17, 1))) {
				future_move = BitMapMoveBit(PbitBoard, SingleTurnedBitLoc(Knight), 17, 1);
				DebugBitboard(future_move, "Future move -17");
				LegalMoves.push_back(future_move);
			}
			// SoEaEa -6
			if ((Knight & (BB.FileHBitBoard | BB.FileGBitBoard | BB.Rank1BitBoard)) == 0 && (!IsColliding(OccBitBoard, SingleTurnedBitLoc(Knight), 6, 1))) {
				future_move = BitMapMoveBit(PbitBoard, SingleTurnedBitLoc(Knight), 6, 1);
				DebugBitboard(future_move, "Future move -6");
				LegalMoves.push_back(future_move);
			}
			// SoWeWe -10
			if ((Knight & (BB.FileBBitBoard | BB.FileABitBoard | BB.Rank1BitBoard)) == 0 && (!IsColliding(OccBitBoard, SingleTurnedBitLoc(Knight), 10, 1))) {
				future_move = BitMapMoveBit(PbitBoard, SingleTurnedBitLoc(Knight), 10, 1);
				DebugBitboard(future_move, "Future move -10");
				LegalMoves.push_back(future_move);

			}
		}
	}


	void KingLogic(int Color
#if DEBUG_MODE
		, int debugInput
#endif
	) 
	{
		BitBoards BB;
		U64 PbitBoard = 0;
		U64 OccBitBoard;

#if DEBUG_MODE
		OccBitBoard = BB.VariableBitBoard[debugInput];
#else
		OccBitBoard = BB.OccBitBoard;
#endif

		//DebugBitboard(BB.WhiteKingBitBoard | BB.BlackKingBitBoard, "Kings");
		if (Color == 0) {
			//PbitBoard = BB.WhitePiecesBitBoard;
			std::cout << "Color: White" << "\n";
		}
		else if (Color == 1) {
			//PbitBoard = BB.BlackPiecesBitBoard;
			std::cout << "Color: Black" << "\n";
		}

		// All 8 possible king moves
		const int king_moves[8]{
		 7, 8, 9, 1,
		-1,-7,-8,-9
		};

		const int king_moves_len = sizeof(king_moves) / sizeof(king_moves[0]);
		//bit map with only king
		U64 king = { Color == 0 ? BB.WhiteKingBitBoard : BB.BlackKingBitBoard }; // Kui varv on 0, siis valge kuningas, kui 1, siis must kuningas

		int kingloc = SingleTurnedBitLoc(king);

		for (int i = 0; i < king_moves_len; i++) {
			int move = king_moves[i];

			// Continue skipib siin selle iterationi ja pmst i++
			if ((move == -1 || move == 7 || move == -9) && (king & BB.FileABitBoard)) continue; // left edge
			if ((move == 1 || move == 9 || move == -7) && (king & BB.FileHBitBoard)) continue; // right edge 

			if ((move == 7 || move == 8 || move == 9) && (king & BB.Rank8BitBoard)) continue; // Top
			if ((move == -7 || move == -8 || move == -9) && (king & BB.Rank1BitBoard)) continue; // Bottom

			if (!IsColliding(OccBitBoard, kingloc, abs(king_moves[i]), (king_moves[i] > 0 ? 0 : 1))) { // (king_moves[i] > 0 ? 0 : 1) simple way to convert to 0 or 1 depending on the sign of king_moves[i]
				U64 future_move = BitMapMoveBit(BB.WhiteKingBitBoard, kingloc, abs(king_moves[i]), (king_moves[i] > 0 ? 0 : 1));

				DebugBitboard(future_move, "Future move king");
				
				LegalMoves.push_back(future_move);
			}
		}
	}
	// Rook, Bishop, Queen logic
	void SlidePieceLogic(int color, 
	#if DEBUG_MODE
			int debugInput
	#endif
	)
	{
		BitBoards BB;
		U64 PbitBoard = 0;
		U64 OccBitBoard;

#if DEBUG_MODE
		OccBitBoard = BB.VariableBitBoard[debugInput];
#else
		OccBitBoard = BB.OccBitBoard;
#endif

		if (color == 0) {
			PbitBoard = BB.PieceBitBoard;
			std::cout << "Color: White" << "\n";
		}
		else if (color == 1) {
			PbitBoard = BB.PieceBitBoard;
			std::cout << "Color: Black" << "\n";
		}
		
		//Magic bitboards: TODO

	}

	// Clears the data in LegalMoves vector for next use
	void Flush() {
		std::cout << "Amount of legal moves available:" << LegalMoves.size() << "\n";
		LegalMoves.clear();
	}

private:
	void PrintBitboard(U64 BitBoard, const std::string& Name) {
		std::bitset<64> bits(BitBoard);

		std::cout << "BITBOARD_NAME::" << Name << "\n";

		for (int row = 0; row < 8; ++row) {
			for (int column = 0; column < 8; ++column) {
				std::cout << bits[(7 - row) * 8 + column] << " ";
			}
			std::cout << "\n";
		}
		std::cout << "\n";
	}
	// Finds all the positions of turned on bits (1s) in a 64-bit integer
	void TurnedBitLoc(U64 number, std::vector<U64>& turnedOnBits) {
		turnedOnBits.clear();   // make sure the vector is empty

		for (int bitIndex = 0; bitIndex < 64; bitIndex++) {
			if (number & 1) {  // LSB is set
				turnedOnBits.push_back(1ULL << bitIndex);  // store as a single-bit U64
			}
			number >>= 1;  // shift to check next bit
		}
	}

	// Finds the position of a single turned on bit (1) in a 64-bit integer
	int SingleTurnedBitLoc(U64 N) {
		U64 result = log2(N);
		return result;
	}

	// Removes/adds a piece to at the specified bit position
	void RemovePiece(U64 BitMap, int BitNumber) {
		FlipBit(BitMap, BitNumber);
	}

	// Checks if the bit at the specified position is on (1) or off (0)
	U64 OuterFileCheck(int Bitnumber) {
		U64 OuterFile = 0b1111111110000001100000011000000110000001100000011000000111111111;
		U64 tempmap = CreateTempBit(Bitnumber);
		tempmap = tempmap & OuterFile;
		return tempmap;
	}

	// Converts chess notation (e.g., "e4") to a bit position (0-63)
	int RecognizeFile(std::string SquareAdress) {
		char file = SquareAdress[0];
		char rank = (SquareAdress[1]);

		std::map<char, int> AdressToNumber = {
			{'a', 1}, {'b', 2}, {'c', 3}, {'d', 4},
			{'e', 5}, {'f', 6}, {'g', 7}, {'h', 8}
		};

		if (AdressToNumber.find(file) == AdressToNumber.end()) {
			std::cout << ("Error: Key '" + std::string(1, file) + "' not found in the map.\n");
			return -1;
		}

		int int_file = AdressToNumber[file];
		int int_rank = rank - '0'; //ASCII maagia
		int BitNumber = (int_rank-1)*8 + (int_file-1); //Arvutame Biti numbri bitboardil. numbrid on 0-63
		std::cout << BitNumber << "\n";
		return BitNumber; 
	}
	
	//Brian Kernighan's way BigCounting
	U8 countSetBits(U64 n) {
		U8 count = 0;
		while (n) {
			count++;
			n &= n - 1; // reset LS1B
		}
		return count;
	}

	// Creates a bitboard with only one bit set at the specified position
	U64 CreateTempBit(int bitnumber) {
		U64 TempBitNumber = 0x0000000000000000;
		TempBitNumber = FlipBit(TempBitNumber, bitnumber);
		return TempBitNumber;
	}

	// Flips the bit at the specified position
	U64 FlipBit(U64 N, int BitPos) {
		return N ^= (1ULL << (BitPos));
	}
	// Moves a bit from one position to another in the bitboard
	U64 BitMapMoveBit(U64 BitMap, int BitNumber, int BitShiftAmount, int Dir) {
		U64 TempBitMap_1 = CreateTempBit(BitNumber);
		U64 TempBitMap_2 = CreateTempBit(BitNumber);
		if (Dir == 0) {
			TempBitMap_2 = TempBitMap_1 << BitShiftAmount;
		}
		else if (Dir == 1) {
			TempBitMap_2 = TempBitMap_1 >> BitShiftAmount;
		}
		TempBitMap_2 = TempBitMap_1 | TempBitMap_2;
		U64 result = BitMap^ TempBitMap_2;
		//PrintBitboard(result, "Result");
		return result;
	}

	// int Piece 1 = pawn, 2 = knigt, 3 = bishop, 5 = rook, 9 = queen
	U64 CanCapture(U64 BitMap, int Piece, int BitNumber, int Dir) {
		U64 TempBitMap_1 = CreateTempBit(BitNumber);
		U64 TempBitMap_2 = CreateTempBit(NULL);

		if (Piece == 1) {
			if (OuterFileCheck(BitNumber) && BitNumber % 2 == 0) {
				if (Dir == 0){
					TempBitMap_1 = TempBitMap_1 << 9; 
				}
				else if (Dir == 1) {
					TempBitMap_1 = TempBitMap_1 >> 7;
				}
				//PrintBitboard(TempBitMap_1, "Debug test"); 
				TempBitMap_1 = TempBitMap_1 & BitMap;
				return TempBitMap_1;
			}
			else if (OuterFileCheck(BitNumber) && BitNumber % 2 == 1) {
				if (Dir == 0) {
					TempBitMap_1 = TempBitMap_1 << 7; 
				}
				else if (Dir == 1) {
					TempBitMap_1 = TempBitMap_1 >> 9; 
				}
				//PrintBitboard(TempBitMap_1, "Debug test");
				TempBitMap_1 = TempBitMap_1 & BitMap;
				return TempBitMap_1;
			}
			else {
				if (Dir == 0) {
					TempBitMap_1 = TempBitMap_1 << 7;
					TempBitMap_2 = TempBitMap_1 << 2;
					//PrintBitboard(TempBitMap_2, "Debug test");
					//PrintBitboard(TempBitMap_1, "Debug test2");
				}
				else if (Dir == 1) {
					TempBitMap_1 = TempBitMap_1 >> 7;
					TempBitMap_2 = TempBitMap_1 >> 2;
					//PrintBitboard(TempBitMap_2, "Debug test");
					//PrintBitboard(TempBitMap_1, "Debug test2");
				}
				

				TempBitMap_2 = TempBitMap_1 | TempBitMap_2;
				TempBitMap_1 = TempBitMap_2 & BitMap;
				//PrintBitboard(TempBitMap_2, "Debug test");
				return TempBitMap_1;
			}
		}

		if (Piece == 2) {

		}
		else {
			std::cout << "\n\n" << "SOMETHING::HAS::GONE::WRON::LEGAL::MOVE::CAPTURE::ERROR" << "\n\n";
			return 0;
		}
	}

	// Check if the move collides with any pieces on the board
	bool IsColliding(U64 BitMapCollision, int BitNumber, int BitShiftAmount, int Dir) {
		U64 TempBitMap_1 = CreateTempBit(BitNumber);
		if (Dir == 0) {
			TempBitMap_1 = TempBitMap_1 << BitShiftAmount; // after bitshift = movedpos
		}
		else if (Dir == 1) {
			TempBitMap_1 = TempBitMap_1 >> BitShiftAmount; // after bitshift = movedpos
		}
		TempBitMap_1 = TempBitMap_1 & BitMapCollision;
		
		return (!TempBitMap_1) == 0; // Returns true if colliding
	}

	// int Piece 1 = pawn, 3 = bishop, 5 = rook, 9 = queen
	bool IsBlocked(U64 BitMap, int Piece, int BitNumber, int BitShiftAmount, int Color) {
		U64 TempBitMap_1 = CreateTempBit(BitNumber);
		U64 TempBitMap_2 = CreateTempBit(NULL); //NULL teeb tühja bitmapi

		if (Color == 0) {
			TempBitMap_2 = TempBitMap_1 << BitShiftAmount; 
		}
		else if (Color == 1) {
			TempBitMap_2 = TempBitMap_1 >> BitShiftAmount; 
		}
		
		if (Piece == 1) { // Expected for 2sq pawn moves
			if (Color == 0) { 
				TempBitMap_2 = TempBitMap_2 >> 8;
			}
			else if (Color == 1) {
				TempBitMap_2 = TempBitMap_2 << 8;
			}
			TempBitMap_2 = TempBitMap_2 & BitMap;
			return !TempBitMap_2 == 0;
		}
		else return false;
	}
};	



struct MakeMove{
	int startsq;
	int destsq;
};

struct MoveList {
	MakeMove moves[218]; // fixed maximum capacity
	int count = 0;   // how many moves are currently stored

	void add(MakeMove m) {
		moves[count++] = m;
	}
};

class Move {
public:
	U64 playbitboard = 0;

	void move(int startsq, int destsq) {
	}
private:
};

int main() {
	Logic logic;

	#if DEBUG_MODE
		logic.DebugMode();
		return 0;
	#else
	logic.PawnLogic(0);
	logic.KnightLogic(0);
	logic.KingLogic(0);
	return 0;
	#endif
}
