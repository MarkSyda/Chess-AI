#include <iostream>
#include <array>
#include <bitset>
#include <cstdint>
#include <string>
#include <map>
#include <stdexcept> 


int main() {

	uint64_t WhitePawnBitBoard = 0b000000001111111000000000000000000000000000000000000000000000000;
	int BitPos = 8
	WhitePawnBitBoard |= (1ULL << BitPos);
	std::cout << "After setting 9th bit: " << std::bitset<64>(WhitePawnBitBoard) << endl;

	WhitePawnBitBoard &= ~(1ULL << BitPos); 
	std::cout << "After clearing 9th bit: " << std::bitset<64>(WhitePawnBitBoard) << endl;
	return 0;
}
