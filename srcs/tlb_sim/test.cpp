#include "tlb.h"


int log_2_int(int a){
	int result = 0;
	while(a >>= 1){
		result++;
	}
	return result;
}


// File to test TLB accesses
int main(){
	uint64_t a = 1;
	int hits = 0;
	int misses = 0;
	TLB tlb = TLB(32,2);
	while(1){
		std::cout << "Enter addr: ";
		std::cin >> a;
		if(a == 0){
			break;
		}
		int r = tlb.tlb_access(a);
		if(r == TLB_HIT){
			hits++;
			std::cout << " Hit!\n";
		}
		else{
			misses++;
			std::cout << "Miss !\n";
		}
	}
	std::cout << "Hits: " << hits << " ;Misses: " << misses << std::endl;
	return 0;
};

