#include "tlb.h"



// File to test TLB accesses
int main(){
	uint64_t a = 1;
	TLB tlb = TLB(16,2048);
	while(1){
		std::cout << "Enter addr: ";
		std::cin >> std::hex >>a;
		a = (a << log_2_int(2048)); 
		if(a == 0){
			break;
		}
		int r = tlb.tlb_access(a);
		std::cout << "Hit? " << r << std::endl;
	}
	std::cout << "TLB sim: hits: " << tlb.hits << std::endl;
       	std::cout << " Misses: " << tlb.misses << std::endl;
	return 0;
};

