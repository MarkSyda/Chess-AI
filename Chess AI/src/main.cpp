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

struct BitBoards {
	const U64 StartBitBoard = 0b1111111111111111000000000000000000000000000000001111111111111111;
	const U64 WPawnStartBitBoard = 0b0000000000000000000000000000000000000000000000001111111100000000;
	const U64 BPawnStartBitBoard = 0b0000000011111111000000000000000000000000000000000000000000000000;

	U64 PieceBitBoard = 0b1111111111111111000000000000000000000000000000011111111111111111;

	U64 BlackPiecesBitBoard = 0b1111111111111111000000000000000000000000000000000000000000000000;
	U64 WhitePiecesBitBoard = 0b0000000000000000000000000000000000000000000000011111111111111111;

	U64 WhitePawnBitBoard = 0b0000000000000000000000000000000000000000000000011111111100000000;
	U64 BlackPawnBitBoard = 0b0000000011111111000000000000000000000000000000000000000000000000;

	U64 WhiteKnightBitBoard = 0b0000000000000000000000000000000000000000000000000000000001000010;
	U64 BlackKnightBitBoard = 0b0100001000000000000000000000000000000000000000000000000000000000;
	
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
		U64 OccBitBoard = BB.BlackPiecesBitBoard | BB.WhitePiecesBitBoard;
		U64 PBitBoard = BB.PieceBitBoard;
		U64 StartSqPawns = 0;
		std::vector<U64> LegalMoves;
		std::vector<int> AlivePawns;
		std::vector<int> NotMovedPawns;
		uint8_t count = 0;

		//Determine color
		if (Color == 0) {
			TurnedBitLoc(BB.WhitePawnBitBoard, AlivePawns);
			StartSqPawns = BB.WhitePawnBitBoard & BB.StartBitBoard;
			TurnedBitLoc(StartSqPawns, NotMovedPawns);
			count = countSetBits(StartSqPawns);
		}

		else if (Color == 1) {
			TurnedBitLoc(BB.BlackPawnBitBoard, AlivePawns);
			StartSqPawns = BB.BlackPawnBitBoard & BB.StartBitBoard;
			TurnedBitLoc(StartSqPawns, NotMovedPawns);
			count = countSetBits(StartSqPawns);
		}

		//Printing Bitboards
		PrintBitboard(OccBitBoard, std::string("OccBitBoard"));
		PrintBitboard(BB.PieceBitBoard, std::string("PieceBitBoard"));

		//Pawns

		//Single move (1 sq)
		for (unsigned int i = 0; i < AlivePawns.size(); i++) {
			U64 future_move = BitMap_Change_Vert(PBitBoard, AlivePawns[i], 8, Color);
			if (!(IsColliding(OccBitBoard, AlivePawns[i], 8, Color))) {
				LegalMoves.push_back(future_move);
			}
		}

		//Double move (2 sq)
		for (unsigned int i = 0; i < count; i++) {
			U64 future_move_1 = BitMap_Change_Vert(PBitBoard, NotMovedPawns[i], 16, Color);
			if (!IsColliding(OccBitBoard, NotMovedPawns[i], 16, Color) && !IsBlocked(OccBitBoard, 1, NotMovedPawns[i], 16, Color)) {
				LegalMoves.push_back(future_move_1);
			}
		}

		//Pawn taking
		for (unsigned int i = 0; i < AlivePawns.size(); i++) { // Alivepawns[i] naitab pawni bitnummerit
			U64 future_move[2] = { 0 };
			U64 Capture_able_board = 0;
			U64 file = AlivePawns[i] % 8; // File {0-7}
			//Color selection
			if (Color == 0) {
				Capture_able_board = CanCapture(BB.BlackPiecesBitBoard, 1, AlivePawns[i], Color);
			}
			else if (Color == 1) {
				Capture_able_board = CanCapture(BB.WhitePiecesBitBoard, 1, AlivePawns[i], Color);
			}

			if (OuterFileCheck(AlivePawns[i]) != 0) {// Outer pawnidele eraldi check

				if (file == 0) {
					future_move[1] = BitMap_Change_Vert(PBitBoard, AlivePawns[i], 9, Color);
					future_move[1] = Capture_able_board & future_move[1];
				}
				if (file == 7) {
					future_move[0] = BitMap_Change_Vert(PBitBoard, AlivePawns[i], 7, Color);
					future_move[0] = Capture_able_board & future_move[0];
				}
			}
			if (OuterFileCheck(AlivePawns[i]) == 0) {
				future_move[0] = BitMap_Change_Vert(PBitBoard, AlivePawns[i], 7, Color);
				future_move[1] = BitMap_Change_Vert(PBitBoard, AlivePawns[i], 9, Color);
				future_move[0] = Capture_able_board & future_move[0];
				future_move[1] = Capture_able_board & future_move[1];
			}
			if (future_move[0] != 0) {
				LegalMoves.push_back(future_move[0]);
			}
			else if (future_move[1] != 0) {
				LegalMoves.push_back(future_move[1]);
			}
		}
		legal_moves_test(LegalMoves);

		U64 test_1 = shift_n(BB.WhitePiecesBitBoard, 12, 9);
		PrintBitboard(test_1, "test1");

		U64 test_2 = shift_s(BB.BlackPiecesBitBoard, 52, 2);
		PrintBitboard(test_2, "test2");

		U64 test_3 = shift_e(BB.WhiteKnightBitBoard, 1, 2);
		PrintBitboard(test_3, "test3");

		U64 test_4 = shift_w(BB.WhiteKnightBitBoard, 1, 1);
		PrintBitboard(test_4, "test4");

		U64 test_5 = shift_ne(BB.WhitePiecesBitBoard, 12, 3);
		PrintBitboard(test_5, "test5");

		U64 test_6 = shift_nw(BB.WhitePiecesBitBoard, 12, 3);
		PrintBitboard(test_6, "test6");

		U64 test_7 = shift_se(BB.BlackPiecesBitBoard, 52, 3);
		PrintBitboard(test_7, "test7");

		U64 test_8 = shift_sw(BB.BlackPiecesBitBoard, 52, 3);
		PrintBitboard(test_8, "test8");
	}

	void PieceLogic(int Color) {
		BitBoards bitboard;
		U64 WhiteKnightBitBoard = bitboard.WhiteKnightBitBoard;
		U64 BlackKnightBitBoard = bitboard.BlackKnightBitBoard;
		//Knights
		std::vector<int>AliveKnights;
		if (Color == 0) {
			TurnedBitLoc(WhiteKnightBitBoard, AliveKnights); //push back vector. gives aliveknights bitnumbers
		}
		else if (Color == 1) {
			TurnedBitLoc(BlackKnightBitBoard, AliveKnights);
		}

		for (unsigned int i = 0; i < AliveKnights.size(); i++) {
			U64 future_move[8] = { 0 };
			//future_move[0] <<= 

		}
	}
private:
	void PrintBetterBitboard(U64 BitBoard, U64 wBitboard, U64 bBitboard, std::string name) {
		std::bitset<64> bits(BitBoard);

		std::cout << "BITBOARD_NAME::" << name << "\n";

		for (int row = 0; row < 8; ++row) {
			for (int column = 0; column < 8; ++column) {
				int index = (7 - row) * 8 + column;
				U64 mask = 1ULL << index;

				if (BitBoard & mask) {
					if (wBitboard & mask) {
						std::cout << "W ";
					}
					else if (bBitboard & mask) {
						std::cout << "B ";
					}
					else {
						std::cout << "M ";
					}
				}
				else {
					std::cout << "O ";
				}
			}
			std::cout << "\n";
		}
		std::cout << "\n";
	}

	void PrintBitboard(U64 BitBoard, std::string Name) {
		std::bitset<64> bits(BitBoard);  // Create bitset from BitBoard

		std::cout << "BITBOARD_NAME::" << Name << "\n";

		// Print the bitboard row by row
		for (int row = 0; row < 8; ++row) {
			// Print each bit in the row from left to right
			for (int column = 0; column < 8; ++column) {
				// Access the bits from left to right as expected on a chessboard
				std::cout << bits[(7 - row) * 8 + column] << " ";
			}
			std::cout << "\n";
		}
		std::cout << "\n";
	}

	void legal_moves_test(std::vector<U64>LegalMoves) {
		BitBoards bitboard;
		std::cout << "Amount of legal moves available:" << LegalMoves.size() << "\n";
		int n = LegalMoves.size();
		for (int i = 0; i < n; i++) {
			PrintBetterBitboard(LegalMoves[i], bitboard.WhitePiecesBitBoard, bitboard.BlackPiecesBitBoard, "Legal move nr" + std::to_string(i + 1));
		}
	}

	void TurnedBitLoc(U64 number, std::vector<int>& turnedOnBits) {
		// Loop through all 64 bits
		for (int bitIndex = 0; bitIndex < 64; bitIndex++) {
			if (number & 1) {  // Check if the least significant bit (LSB) is 1
				turnedOnBits.push_back(bitIndex);
			}
			number >>= 1;  // Shift number to the right by 1 (discard LSB)
		}
	}

	int SingleTurnedBitLoc(U64 N) {
		U64 result = log2(N);
		return result;
	}

	void RemovePiece(U64 BitMap, int BitNumber) {
		FlipBit(BitMap, BitNumber);
	}

	U64 OuterFileCheck(int Bitnumber) {
		U64 OuterFile = 0b1111111110000001100000011000000110000001100000011000000111111111;
		U64 tempmap = CreateTempBit(Bitnumber);
		//PrintBitboard(OuterFile, "outerfile");
		tempmap = tempmap & OuterFile;
		return tempmap;
	}

	int RecognizeFile(std::string SquareAdress) {
		char file = SquareAdress[0];
		char rank = SquareAdress[1];

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
		int BitNumber = (int_rank - 1) * 8 + (int_file - 1); //Arvutame Biti numbri bitboardil. numbrid on 0-63
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

	U64 CreateTempBit(int bitnumber) {
		U64 TempBitNumber = 0x0000000000000000;
		TempBitNumber = FlipBit(TempBitNumber, bitnumber);
		return TempBitNumber;
	}

	U64 FlipBit(U64 N, int BitPos) {
		return N ^= (1ULL << (BitPos));
	}

	U64 BitMap_Change_Vert(U64 BitMap, int BitNumber, int BitShiftAmount, int Dir) {
		U64 TempBitMap_1 = CreateTempBit(BitNumber);
		U64 TempBitMap_2 = CreateTempBit(BitNumber);
		if (Dir == 0) {
			TempBitMap_2 = TempBitMap_1 << BitShiftAmount;
		}
		else if (Dir == 1) {
			TempBitMap_2 = TempBitMap_1 >> BitShiftAmount;
		}
		TempBitMap_2 = TempBitMap_1 | TempBitMap_2;
		U64 result = BitMap ^ TempBitMap_2;
		//PrintBitboard(result, "Result");
		return result;
	}

	// int Piece 1 = pawn, 2 = knigt, 3 = bishop, 5 = rook, 9 = queen, // Dir -> Color
	U64 CanCapture(U64 BitMap, int Piece, int BitNumber, int Dir) {
		U64 TempBitMap_1 = CreateTempBit(BitNumber);
		U64 TempBitMap_2 = CreateTempBit(NULL);

		if (Piece == 1) {
			//Special logic for outer files
			if (OuterFileCheck(BitNumber) && BitNumber % 2 == 0) {
				if (Dir == 0) {
					TempBitMap_1 = TempBitMap_1 << 9;
				}
				else if (Dir == 1) {
					TempBitMap_1 = TempBitMap_1 >> 7;
				}
				TempBitMap_1 = TempBitMap_1 & BitMap;
				return TempBitMap_1;
			}
			//Special logic for outer files
			else if (OuterFileCheck(BitNumber) && BitNumber % 2 == 1) {
				if (Dir == 0) {
					TempBitMap_1 = TempBitMap_1 << 7;
				}
				else if (Dir == 1) {
					TempBitMap_1 = TempBitMap_1 >> 9;
				}
				TempBitMap_1 = TempBitMap_1 & BitMap;
				return TempBitMap_1;
			}
			else {
				if (Dir == 0) {
					TempBitMap_1 = TempBitMap_1 << 7;
					TempBitMap_2 = TempBitMap_1 << 2;
				}
				else if (Dir == 1) {
					TempBitMap_1 = TempBitMap_1 >> 7;
					TempBitMap_2 = TempBitMap_1 >> 2;
				}


				TempBitMap_2 = TempBitMap_1 | TempBitMap_2;
				TempBitMap_1 = TempBitMap_1 & BitMap;
				return TempBitMap_1;
			}
		}

		if (Piece == 2) {

		}
		else {
			std::cout << "\n\n" << "SOMETHING::HAS::GONE::WRONG::LEGAL::MOVE::CAPTURE::ERROR" << "\n\n";
			return 0;
		}
	}

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


	void give_filerank(int BitNumber, unsigned int& file, unsigned int& rank) {
		// integer division nii et 7/8 for example annab endiselt 0
		file = (BitNumber % 8) + 1;
		rank = (BitNumber / 8) + 1;
	}

	//Bitshift directions
	U64 shift_n(U64 bitmap, int bitnumber, int num_squares) {
		U64 temp_map = CreateTempBit(bitnumber);
		int bitshift_amount = num_squares * 8;
		unsigned int file;
		unsigned int rank;
		
		//Check for out of bounds movement
		give_filerank(bitnumber, file, rank);
		int max_num_squares_amount = 8 - rank;
		if (max_num_squares_amount < num_squares) {
			bitmap = 0;
			return bitmap;
		}
		else {
			bitmap ^= temp_map;
			temp_map <<= bitshift_amount;
			bitmap |= temp_map;
			return bitmap;
		}
	}

	U64 shift_s(U64 bitmap, int bitnumber, int num_squares) {
		U64 temp_map = CreateTempBit(bitnumber);
		int bitshift_amount = num_squares * 8;
		unsigned int file;
		unsigned int rank;
		PrintBitboard(bitmap, "shift_s bitmap");
		//Check for out of bounds movement
		give_filerank(bitnumber, file, rank);
		int max_num_squares_amount = rank - 1;
		if (max_num_squares_amount < num_squares) {
			bitmap = 0;
			return bitmap;
		}
		else {
			bitmap ^= temp_map;
			temp_map >>= bitshift_amount;
			bitmap |= temp_map;
			return bitmap;
		}
	}

	U64 shift_e(U64 bitmap, int bitnumber, int num_squares) {
		U64 temp_map = CreateTempBit(bitnumber);
		int bitshift_amount = num_squares;
		unsigned int file;
		unsigned int rank;

		//Check for out of bounds movement
		give_filerank(bitnumber, file, rank);
		int max_num_squares_amount = 8-file;
		if (max_num_squares_amount < num_squares) {
			bitmap = 0;
			return bitmap;
		}
		else {
			bitmap ^= temp_map;
			temp_map <<= bitshift_amount;
			bitmap |= temp_map;
			return bitmap;
		}
	}

	U64 shift_w(U64 bitmap, int bitnumber, int num_squares) {
		U64 temp_map = CreateTempBit(bitnumber);
		int bitshift_amount = num_squares;
		unsigned int file;
		unsigned int rank;

		//Check for out of bounds movement
		give_filerank(bitnumber, file, rank);
		int max_num_squares_amount = file - 1;
		if (max_num_squares_amount < num_squares) {
			bitmap = 0;
			return bitmap;
		}
		else {
			bitmap ^= temp_map;
			temp_map >>= bitshift_amount;
			bitmap |= temp_map;
			return bitmap;
		}
	}

	U64 shift_ne(U64 bitmap, int bitnumber, int num_squares) {
		U64 temp_map = CreateTempBit(bitnumber);
		int bitshift_amount = num_squares * 9;
		unsigned int file;
		unsigned int rank;

		// Out-of-bounds check
		give_filerank(bitnumber, file, rank);
		int max_num_squares_amount = std::min(8 - rank, 8 - file);
		if (max_num_squares_amount < num_squares) {
			bitmap = 0;
			return bitmap;
		}

		else {
			bitmap ^= temp_map;
			temp_map <<= bitshift_amount;
			bitmap |= temp_map;
			return bitmap;
		}
	}
	U64 shift_nw(U64 bitmap, int bitnumber, int num_squares) {
		U64 temp_map = CreateTempBit(bitnumber);
		int bitshift_amount = num_squares * 7;
		unsigned int file;
		unsigned int rank;

		// Out-of-bounds check
		give_filerank(bitnumber, file, rank);
		int max_num_squares_amount = std::min(8 - rank, file -1);
		if (max_num_squares_amount < num_squares) {
			bitmap = 0;
			return bitmap;
		}

		else {
			bitmap ^= temp_map;
			temp_map <<= bitshift_amount;
			bitmap |= temp_map;
			return bitmap;
		}
	}

	U64 shift_se(U64 bitmap, int bitnumber, int num_squares) {
		U64 temp_map = CreateTempBit(bitnumber);
		int bitshift_amount = num_squares * 7;
		unsigned int file;
		unsigned int rank;

		// Out-of-bounds check
		give_filerank(bitnumber, file, rank);
		int max_num_squares_amount = std::min(rank - 1, 8 - file);
		if (max_num_squares_amount < num_squares) {
			bitmap = 0;
			return bitmap;
		}

		else {
			bitmap ^= temp_map;
			temp_map >>= bitshift_amount;
			bitmap |= temp_map;
			return bitmap;
		}
	}

	U64 shift_sw(U64 bitmap, int bitnumber, int num_squares) {
		U64 temp_map = CreateTempBit(bitnumber);
		int bitshift_amount = num_squares * 9;
		unsigned int file;
		unsigned int rank;

		// Out-of-bounds check
		give_filerank(bitnumber, file, rank);
		int max_num_squares_amount = std::min(rank - 1, file - 1);
		if (max_num_squares_amount < num_squares) {
			bitmap = 0;
			return bitmap;
		}

		else {
			bitmap ^= temp_map;
			temp_map >>= bitshift_amount;
			bitmap |= temp_map;
			return bitmap;
		}
	}
};

int main() {
	BitBoards BB;
	makeMove game;
	game.PawnLogic(0);

	return 0;
}