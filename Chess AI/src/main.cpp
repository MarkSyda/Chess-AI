#include <iostream>
#include <array>
#include <vector>
#include <bitset>
//Move Input
#include <map>
#include <string>
//typedef's
typedef unsigned long long  U64;
typedef unsigned char U8; 

// Debug mode defininitions
#ifndef DEBUG_MODE
#define DEBUG_MODE 0 // Set to 1 to enable debug mode, 0 to disable
#endif

#if DEBUG_MODE
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

#if DEBUG_MODE
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

struct BitBoards {
	const U64 StartBitBoard = 0b1111111111111111000000000000000000000000000000001111111111111111;
	const U64 WPawnStartBitBoard = 0b0000000000000000000000000000000000000000000000001111111100000000;
	const U64 BPawnStartBitBoard = 0b0000000011111111000000000000000000000000000000000000000000000000;
	U64 PieceBitBoard = 0b1111111111111111000000000000000000000000000000001111111111111111;
	U64 OccBitBoard = 0b1111111111111111111111110000000000000000111111111111111111111111; //Koik blockitud essad sqrid 0b1111111111111111111111110000000000000000111111111111111111111111;
	U64 BlackPiecesBitBoard = 0b1111111111111111000000000000000000000000000000000000000000000000;
	U64 WhitePiecesBitBoard = 0b0000000000000000000000000000000000000000000000001111111111111111;
	U64 WhitePawnBitBoard = 0b0000000000000000000000000000000000000000000000001111111100000000;
	U64 BlackPawnBitBoard = 0b0000000011111111000000000000000000000000000000000000000000000000;
	
};

class makeMove {
public:
	unsigned int Player(unsigned int turn_num) {
		// 0 -> White, 1 -> Black
		if (turn_num % 2 == 0) {
			return 0;
		}
		else {
			return 1;
		}
	}
	//TODO: asenda koik vectorid arraydega U64 arraydega
	// int Color = 0 - White, 1->Black
	void PawnLogic(int Color) {
		BitBoards BB;
		BB.StartBitBoard;
		U64 OccBitBoard = BB.OccBitBoard;
		U64 PBitBoard = BB.PieceBitBoard;
		U64 StartSqPawns = 0;
		U64 future_move[4];
		std::vector<U64> LegalMoves;
		std::vector<int> AlivePawns;
		std::vector<int> NotMovedPawns;
		uint8_t count = 0; // idk mai maleta miks see siin peab olema aga ilma ei toota

		int pawn_move_count = 0;// For later use on array navigation

		//Determine color
		if (Color == 0) {
			TurnedBitLoc(BB.WhitePawnBitBoard, AlivePawns);
			StartSqPawns = BB.WhitePawnBitBoard & BB.StartBitBoard;
			TurnedBitLoc(StartSqPawns, NotMovedPawns);
			count = countSetBits(StartSqPawns);
			std::cout << "Color: White"<< "\n";
		}
		else if (Color == 1) {
			TurnedBitLoc(BB.BlackPawnBitBoard, AlivePawns);
			StartSqPawns = BB.BlackPawnBitBoard & BB.StartBitBoard;
			TurnedBitLoc(StartSqPawns, NotMovedPawns);
			count = countSetBits(StartSqPawns);
			std::cout << "Color: Black" << "\n";
		}
		
		//Printing Bitboards
		PrintBitboard(BB.OccBitBoard, std::string("OccBitBoard"));
		PrintBitboard(BB.PieceBitBoard, std::string("PieceBitBoard"));

		//Pawns
		//Single move (1 sq)
		for (unsigned int i = 0; i < AlivePawns.size(); i++) {
			future_move[0] = BitMapMoveBit(PBitBoard, AlivePawns[i], 8, Color);
			if (!IsColliding(OccBitBoard, AlivePawns[i], 8, Color)) {
				LegalMoves.push_back(future_move[0]);
				size_t vec_size = LegalMoves.size();
				DebugBitboard(future_move[0], "Future move");
				debug("Amount of legal moves for loop 2:", vec_size);
				pawn_move_count++;
			}
		}

		//Double move (2 sq)
		for (unsigned int i = 0; i < count; i++) {
			future_move[1] = BitMapMoveBit(PBitBoard, NotMovedPawns[i], 16, Color);
			if (!IsColliding(OccBitBoard, NotMovedPawns[i], 16, Color) && !IsBlocked(OccBitBoard, 1, NotMovedPawns[i], 16, Color)) {
				LegalMoves.push_back(future_move[1]);
				size_t vec_size = LegalMoves.size();
				DebugBitboard(future_move[1], "Future move 1");
				debug("Amount of legal moves for loop 1:", vec_size);
				pawn_move_count++;
			}
		}

		//Pawn taking tootab ainult valgetega praegu
		for (unsigned int i = 0; i < AlivePawns.size(); i++) {
			U64 Capture_able_board = CanCapture(OccBitBoard, 1, AlivePawns[i], Color);
			U64 file = AlivePawns[i] % 8;
			file += 1;

			if (OuterFileCheck(AlivePawns[i]) == 0 && Capture_able_board != 0) {
				
				U64 capture_able_test_right = BitMapMoveBit(PBitBoard, AlivePawns[i], 9, Color);
				U64 capture_able_test_left = BitMapMoveBit(PBitBoard, AlivePawns[i], 7, Color);
				U64 result1 = capture_able_test_right & Capture_able_board;
				U64 result2 = capture_able_test_left & Capture_able_board;
				//PrintBitboard(Capture_able_board, "Capture able board");
				//PrintBitboard(capture_able_test_right, "capture_able_test_right");
				//PrintBitboard(capture_able_test_left, "capture_able_test_left");

				//Check wich side we can capture if only 1 is capture able
				if ((capture_able_test_left & Capture_able_board) != 0) {
					future_move[2] = BitMapMoveBit(PBitBoard, AlivePawns[i], 9, Color);
					DebugBitboard(future_move[2], "future move[3]");
					LegalMoves.push_back(future_move[2]);
					pawn_move_count++;
				}
				if ((capture_able_test_right & Capture_able_board) != 0) {
					future_move[3] = BitMapMoveBit(PBitBoard, AlivePawns[i], 7, Color);
					DebugBitboard(future_move[3], "future move[4]");
					LegalMoves.push_back(future_move[3]);
					pawn_move_count++;
				}
			}

			//Kui ettur on laua ääres, siis teeb ainult ühe suunalise rünnaku
			else{
				if (file == 1 && Capture_able_board != 0) {
					if (Color == 0) {
						future_move[2] = BitMapMoveBit(PBitBoard, AlivePawns[i], 9, Color); // Kui ettur on esimese faili peal siis votame ainult paremale
					}
					else {
						future_move[2] = BitMapMoveBit(PBitBoard, AlivePawns[i], 7, Color); // Kui ettur on esimese faili peal siis votame ainult paremale
					}
					
					PrintBitboard(future_move[2], "future move[1]_special_file 0");
					LegalMoves.push_back(future_move[2]);
					pawn_move_count++;
				}
				if (file == 8 && Capture_able_board != 0) {
					if (Color == 0) {
						future_move[3] = BitMapMoveBit(PBitBoard, AlivePawns[i], 7, Color); // Kui ettur on kaheksanda faili peal siis votame ainult vasakule
					}
					else {
						future_move[3] = BitMapMoveBit(PBitBoard, AlivePawns[i], 9, Color); // Kui ettur on kaheksanda faili peal siis votame ainult vasakule
					}
					
					PrintBitboard(future_move[3], "future move[0]_special_file 8");
					LegalMoves.push_back(future_move[3]);
					pawn_move_count++;
				}
			}

		}
		std::cout << "\n" << "Amount of legal moves available:" << pawn_move_count << "\n";

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
	void TurnedBitLoc(U64 number, std::vector<int>& turnedOnBits) {
		// Loop through all 64 bits
		for (int bitIndex = 0; bitIndex < 64; bitIndex++) {
			if (number & 1) {  // Check if the least significant bit (LSB) is 1
				turnedOnBits.push_back(bitIndex);
			}
			number >>= 1;  // Shift number to the right by 1 (discard LSB)
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
		
		return !TempBitMap_1 == 0;
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

class pawn {
public:
	U64 playbitboard = 0;

	void move(int startsq, int destsq) {
	}
private:
};
int main() {
	BitBoards BB;
	makeMove game;
	game.PawnLogic(1);
	return 0;
}
