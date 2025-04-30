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
	U64 PieceBitBoard = 0b1111111111111111000000000000000000000000000000001111111111111111;
	U64 OccBitBoard = 0b1111111111111111000000000000000000000000000000011111111111111111;
	U64 BlackPiecesBitBoard = 0b1111111111111111000000000000000000000000000000000000000000000000;
	U64 WhitePiecesBitBoard = 0b0000000000000000000000000000000000000000000000001111111111111111;
	U64 WhitePawnBitBoard = 0b0000000000000000000000000000000000000000000000001111111100000000;
	U64 BlackPawnBitBoard = 0b0000000011111111000000000000000000000000000000000000000000000000;
	U64 IsPawnOnStartSq = WhitePawnBitBoard & StartBitBoard;
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
	void PawnLogic() {
		BitBoards BB;
		BB.StartBitBoard;
		U64 StartSqPawns = BB.IsPawnOnStartSq;
		U64 OccBitBoard = BB.OccBitBoard;
		U64 PBitBoard = BB.PieceBitBoard;
		std::vector<int64_t> LegalMoves;

		//init vector NotMovedPawns
		std::vector<int>NotMovedPawns;
		TurnedBitLoc(StartSqPawns, NotMovedPawns); //Updateme oma vectorit ja paneme sinna NotMovedPawnide id-d

		//Init vec Alivepawns
		std::vector<int> AlivePawns;
		TurnedBitLoc(BB.WhitePawnBitBoard, AlivePawns);

		//Printing Bitboards
		PrintBitboard(BB.OccBitBoard, std::string("OccBitBoard"));
		PrintBitboard(BB.PieceBitBoard, std::string("PieceBitBoard"));

		uint8_t count = countSetBits(StartSqPawns); //Loeme mitte liigutatud Pawnid üle 

		for (unsigned int i = 0; i < count; i++) {
			U64 future_move_1 = BitMap_Change_Vert(PBitBoard, NotMovedPawns[i], 16);
			if (!IsColliding(OccBitBoard, NotMovedPawns[i], 16) && !IsBlocked(OccBitBoard, 1, NotMovedPawns[i], 16)) {
				LegalMoves.push_back(future_move_1);
				size_t vec_size = LegalMoves.size();
				std::cout << "Amount of legal moves (for loop 2): " << vec_size << "\n";
			}
		} std::cout << "\n";

		for (unsigned int i = 0; i < AlivePawns.size(); i++) {
			U64 future_move = BitMap_Change_Vert(PBitBoard, AlivePawns[i], 8);
			if (!IsColliding(OccBitBoard, AlivePawns[i], 8)) {
				LegalMoves.push_back(future_move);
				size_t vec_size = LegalMoves.size();
				std::cout << "Amount of legal moves (for loop 1): " << vec_size << "\n";
			}
		}

		for (unsigned int i = 0; i < AlivePawns.size(); i++) {
			U64 future_move[2];
			U64 Capture_able_board = CanCapture(OccBitBoard, 1, AlivePawns[i]);
			future_move[0] = BitMap_Change_Vert(PBitBoard, AlivePawns[i], 7);
			future_move[1] = BitMap_Change_Vert(PBitBoard, AlivePawns[i], 9);

			if ((Capture_able_board & future_move[0]) != 0) {
				LegalMoves.push_back(future_move[0]);
				size_t vec_size = LegalMoves.size();
				std::cout << "Amount of legal moves (for loop 3): " << vec_size << "\n";
			}
			else if ((Capture_able_board & future_move[1]) != 0) {
				LegalMoves.push_back(future_move[1]);
				int64_t vec_size = LegalMoves.size();
				std::cout << "Amount of legal moves (for loop 3): " << vec_size << "\n";
			}std::cout << "\n";
		}
	}
	void WhiteMove() {

	}
private:
	void PrintBitboard(int64_t BitBoard, std::string Name) {
		std::bitset<64> bits(BitBoard);  // Create bitset from BitBoard

		std::cout << "BITBOARD_NAME::" << Name << "\n\n";

		// Print the bitboard row by row
		for (int row = 0; row < 8; ++row) {
			// Print each bit in the row from left to right
			for (int column = 0; column < 8; ++column) {
				// Access the bits from left to right as expected on a chessboard
				std::cout << bits[(7 - row) * 8 + column] << " ";
			}
			std::cout << "\n";
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

	void RemovePiece(U64 BitMap, int BitNumber) {
		FlipBit(BitMap, BitNumber);
	}

	bool OuterFileCheck(int Bitnumber) {
		U64 OuterFile = 0b1111111110000001100000011000000110000001100000011000000111111111;
		U64 tempmap = CreateTempBit(Bitnumber);
		tempmap = tempmap & OuterFile;
		if (tempmap != 0) {
			return true;
		}
		else {
			return false;
		}
	}

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

	U64 CreateTempBit(int bitnumber) {
		U64 TempBitNumber = 0x0000000000000000;
		TempBitNumber = FlipBit(TempBitNumber, bitnumber);
		return TempBitNumber;
	}

	U64 FlipBit(U64 N, int BitPos) {
		return N ^= (1ULL << (BitPos));
	}

	U64 BitMap_Change_Vert(U64 BitMap, int BitNumber, int BitShiftAmount) {
		U64 TempBitMap_1 = CreateTempBit(BitNumber);
		U64 TempBitMap_2 = CreateTempBit(BitNumber);
		TempBitMap_2 = TempBitMap_1 << BitShiftAmount;
		TempBitMap_2 = TempBitMap_1 | TempBitMap_2;
		U64 result = BitMap^ TempBitMap_2;
		//PrintBitboard(result, "Result");
		return result;
	}

	// int Piece 1 = pawn, 2 = knigt, 3 = bishop, 5 = rook, 9 = queen
	U64 CanCapture(U64 BitMap, int Piece, int BitNumber) {
		U64 TempBitMap_1 = CreateTempBit(BitNumber);
		U64 TempBitMap_2 = CreateTempBit(NULL);

		if (Piece == 1) {
			if (OuterFileCheck(BitNumber) && BitNumber % 2 == 0) {
				TempBitMap_1 = TempBitMap_1 << 9;
				TempBitMap_1 = TempBitMap_1 & BitMap;
				return TempBitMap_1;
			}
			else if (OuterFileCheck(BitNumber) && BitNumber % 2 == 1) {
				TempBitMap_1 = TempBitMap_1 << 7;
				TempBitMap_1 = TempBitMap_1 & BitMap;
				return TempBitMap_1;
			}
			else {
				TempBitMap_1 = TempBitMap_1 << 7;
				TempBitMap_2 = TempBitMap_1 << 2;
				TempBitMap_2 = TempBitMap_1 | TempBitMap_2;
				TempBitMap_1 = TempBitMap_1 & BitMap;
				return TempBitMap_1;
			}
		}
		else {
			std::cout << "\n\n" << "SOMETHING::HAS::GONE::WRON::LEGAL::MOVE::CAPTURE::ERROR" << "\n\n";
			return 0;
		}
	}
	bool IsColliding(U64 BitMapCollision, int BitNumber, int BitShiftAmount) {
		U64 TempBitMap_1 = CreateTempBit(BitNumber);
		TempBitMap_1 = TempBitMap_1 << BitShiftAmount; // after bitshift = movedpos
		TempBitMap_1 = TempBitMap_1 & BitMapCollision;
		
		return !TempBitMap_1 == 0;
	}
	// int Piece 1 = pawn, 3 = bishop, 5 = rook, 9 = queen
	bool IsBlocked(U64 BitMap, int Piece, int BitNumber, int BitShiftAmount) {
		U64 TempBitMap_1 = CreateTempBit(BitNumber);
		U64 TempBitMap_2 = CreateTempBit(NULL); //NULL teeb tühja bitmapi
		TempBitMap_2 = TempBitMap_1 << BitShiftAmount; // TempBitMap 2 = new pos
		
		if (Piece == 1) { // Expected for 2sq pawn moves
			TempBitMap_2 = TempBitMap_2 >> 8;
			TempBitMap_2 = TempBitMap_2 & BitMap;
			return !TempBitMap_2 == 0;
		}
		else return false;
	}
};	

int main() {
	BitBoards BB;
	makeMove game;
	game.PawnLogic();
	return 0;
}



