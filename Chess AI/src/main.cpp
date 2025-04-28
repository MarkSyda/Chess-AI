#include <iostream>
#include <array>
#include <bitset>
#include <cstdint>
#include <string>
#include <map>
#include <stdexcept> 
#include <vector>

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
	void WhiteMove() {
		const uint64_t StartBitBoard = 0b1111111111111111000000000000000000000000000000001111111111111111;
		const uint64_t WPawnStartBitBoard = 0b0000000000000000000000000000000000000000000000001111111100000000;
		const uint64_t BPawnStartBitBoard = 0b0000000011111111000000000000000000000000000000000000000000000000;
		uint64_t PieceBitBoard = 0b1111111111111111000000000000000000000000000000001111111111111111;
		uint64_t OccBitBoard = 0b1111111111111111000000000000000000000000000000011111111111111111;
		uint64_t WhitePawnBitBoard = 0b0000000000000000000000000000000000000000000000001111111100000000;
		uint64_t BlackPawnBitBoard = 0b0000000011111111000000000000000000000000000000000000000000000000;
		//Printing Bitboards
		PrintBitboard(OccBitBoard, std::string("OccBitBoard"));
		PrintBitboard(BlackPawnBitBoard, std::string("BlackPawnBitBoard"));
		PrintBitboard(WhitePawnBitBoard, std::string("WhitePawnBitBoard"));

		std::vector<int64_t> LegalMoves;

		uint64_t IsPawnOnStartSq = WhitePawnBitBoard & StartBitBoard;
		PrintBitboard(IsPawnOnStartSq, std::string("IsPawnOnStartSq"));

		uint8_t count = countSetBits(IsPawnOnStartSq); //Loeme mitte liigutatud Pawnid üle

		//init vector NotMovedPawns
		std::vector<int>NotMovedPawns;
		TurnedBitLoc(IsPawnOnStartSq, NotMovedPawns); //Updateme oma vectorit ja paneme sinna NotMovedPawnide id-d

		for (unsigned int i = 0; i < count; i++) {
			LegalMoves.push_back(BitMap_Change_Vert(PieceBitBoard, NotMovedPawns[i], 16));
		}

		std::vector<int> AlivePawns;
		TurnedBitLoc(WhitePawnBitBoard, AlivePawns);

		for (unsigned int i = 0; i < 8; i++) {
			uint64_t future_move = BitMap_Change_Vert(PieceBitBoard, AlivePawns[i], 8);
			LegalMoves.push_back(future_move);
		}

		//Count legal moves
		int vec_size = LegalMoves.size();
		std::cout << "Amount of legal moves: " << vec_size << "\n";

		std::string MoveNotation = "";
		std::cin >> MoveNotation;
		//int BitNumber = RecognizeFile(MoveNotation);
		if (islower(MoveNotation[0])) {
			std::cout << MoveNotation[0] << "\n";
			int BitNumber = RecognizeFile(MoveNotation);
			uint64_t IsPawnOnStartSq = WhitePawnBitBoard & StartBitBoard;
			PrintBitboard(IsPawnOnStartSq, std::string("IsPawnOnStartSq"));
		}
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

	uint64_t FlipBit(uint64_t N, int BitPos) {
		return N ^= (1ULL << (BitPos));
	}

	uint8_t countSetBits(uint64_t n){
		uint8_t count = 0;
		while (n) {
			count += n & 1;
			n >>= 1;
		}
		return count;
	}

	void TurnedBitLoc(uint64_t number, std::vector<int>& turnedOnBits) {
		// Loop through all 64 bits
		
		for (int bitIndex = 0; bitIndex < 64; bitIndex++) {
			if (number & 1) {  // Check if the least significant bit (LSB) is 1
				turnedOnBits.push_back(bitIndex);
			}
			number >>= 1;  // Shift number to the right by 1 (discard LSB)
		}
	}
	uint64_t Piece_Vision() {

	}
	uint64_t BitMap_Change_Vert(uint64_t BitMap, int BitNumber, int BitShiftAmount) {
		uint64_t TempBitMap_1 = 0x0000000000000000;
		uint64_t TempBitMap_2 = 0x0000000000000000;
		TempBitMap_1 = FlipBit(TempBitMap_1, BitNumber);
		TempBitMap_2 = FlipBit(TempBitMap_2, BitNumber);
		TempBitMap_2 = TempBitMap_1 << BitShiftAmount;
		TempBitMap_2 = TempBitMap_1 | TempBitMap_2;
		uint64_t result = BitMap^ TempBitMap_2;
		//PrintBitboard(result, "Result");
		return result;
	}
};	

int main() {
	makeMove game;
	game.WhiteMove();
	return 0;
}



