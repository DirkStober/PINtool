#include "pagetrack.h"

using namespace NDP;





PT::PT(int p_size, int data_distribution){
	data_distribution = data_distribution;
	page_size = p_size;
	uint64_t a = p_size;
	page_off = 0;
	while(a >>= 1){
		page_off++;
	}

	
}

PT::~PT(){}

// Searches vector for matching element
int search_vec_page(
	uint64_t p_in, 
	std::vector<uint64_t> p_addrs
	)
{
	for(uint64_t i = 0; i < p_addrs.size();i++){
		if(p_addrs[i] == p_in){
			return i;
		}
	}
	return -1;
}

int PT::add_memblock(uint64_t mem_start, uint64_t mem_size){
	// Shift addresses by page size
	uint64_t pn_start = mem_start >> page_off;
	uint64_t mem_stop = mem_start + mem_size -1;
	uint64_t pn_stop = (mem_stop) >> page_off;
	//check whether start or stop page already exist
	int * tp, * int_p , * bp;
	// If bot page exists then point pointer to it
	int k = search_vec_page(pn_start,stop_page_addr);
	if(k > -1)
	{
		bp = mem_entries[k].top_page;
	}
	// else allocate and initialize 
	// TODO: Different initialization modes
	else{
		bp = new int;
		*bp = -1;
	}
	// If top page exists then point pointer to it
	 k = search_vec_page(pn_stop,start_page_addr);
	if(k > -1){
		tp = mem_entries[k].bot_page;;
	}
	// If allocated memory is bigger than one page 
	// allocate another page for top page
	else if( (mem_size >> page_off) > 0){
		tp = new int;
		*tp = -1;
	}
	else{
		// Else point top page to bot page
		tp = bp;
	}
	// allocate memory for internal pages
	if(pn_stop - pn_start > 1){	
		int_p = new int[pn_stop - pn_start - 1]{};
	}
	else{
		int_p = NULL;
	};
	
	struct mem_entry tmp_me;
	tmp_me.top_page = tp;
	tmp_me.bot_page = bp;
	tmp_me.int_pages = int_p;
	uint64_t i = 0;
	while(1){
		if(i == mem_entries.size()){
			mem_entries.push_back(tmp_me);
			start_page_addr.push_back(pn_start);
			stop_page_addr.push_back(pn_stop);
			start_addr.push_back(mem_start);	
			stop_addr.push_back(mem_stop);	
			high_addr = mem_stop;
			break;
		}
		if(mem_start < start_addr[i]){
			mem_entries.insert(
					mem_entries.begin()
					+ i,tmp_me);
			start_page_addr.insert(
					start_page_addr.begin()
					+ i,pn_start);
			stop_page_addr.insert(
					stop_page_addr.begin()
					+ i,pn_stop);
			start_addr.insert(
					start_addr.begin()
					+ i,mem_start);
			stop_addr.insert(stop_addr.begin() 
					+ i,mem_stop);
			break;
		}
		i++;
	}
	return 0;
}
	


/*
 * No Hit 
 * Above High addr
 * Page Local 
 * Page not local
 */

int PT::acc_page(uint64_t addr, int mem_id){
	// Check if address is inside heap i.e check 
	// if address is higher than high addr
	if(addr > high_addr){
		//TODO: Define value properly
		return ACC_PAGE_ABOVE;
	}
	int i = -1;
	for(uint64_t j = 0; j < stop_page_addr.size(); j++){
		if(addr <= stop_addr[j]){
			if(addr >= start_addr[j]){
				i = j;
				break;
			}
		}
	}
	if(i == -1){
		return ACC_PAGE_NOT_FOUND;
	}
	uint64_t int_off;
	int_off = addr >> page_off;
	int_off = int_off - start_page_addr[i];
	int * mem_block;
	if(int_off == 0){
		mem_block = mem_entries[i].bot_page;
	}
	else if((addr >> page_off) == stop_page_addr[i]){
		mem_block = mem_entries[i].top_page;
	}
	else{
		int_off = int_off -1;
		mem_block = &(mem_entries[i].int_pages[int_off]);
	}
	// Page is local 
	if(*mem_block == mem_id){
		return ACC_PAGE_SUCC_LOCAL;
	}
	// Page is not local
	if(*mem_block > -1){
		return ACC_PAGE_SUCC_NOT_LOCAL;
	}
	// Page is touched by thread first
	*mem_block = mem_id;
	return ACC_PAGE_SUCC_LOCAL;
}	

int PT::rem_memblock(uint64_t mem_start){
	// Find the memory block
	int i  = search_vec_page(mem_start,start_addr);
	struct mem_entry tmp_mem = mem_entries[i];
	uint64_t p_start,p_stop;
	p_start = start_page_addr[i];
	p_stop  = stop_page_addr[i]; 

	if(i == ((int) mem_entries.size() - 1)){
		mem_entries.pop_back();
		start_page_addr.pop_back();
		stop_page_addr.pop_back();
		start_addr.pop_back();	
		stop_addr.pop_back();	
		high_addr = stop_addr[i-1];
	}
	else{
		mem_entries.erase(
				mem_entries.begin()
				+ i);
		start_page_addr.erase(
				start_page_addr.begin()
				+ i);
		stop_page_addr.erase(
				stop_page_addr.begin()
				+ i);
		start_addr.erase(
				start_addr.begin()
				+ i);
		stop_addr.erase(stop_addr.begin() 
				+ i);
	}

	if(search_vec_page(p_start,stop_page_addr) == -1){
		// Required because block can be first on the page
		// with a trailing block on the same page
		if(search_vec_page(p_start,start_page_addr) == -1){
			delete tmp_mem.bot_page;
		}
	}
	if(p_start != p_stop){
		if(search_vec_page(p_stop,start_page_addr) == -1){
			delete tmp_mem.top_page;
		}
	}
	if(tmp_mem.int_pages != NULL){
		delete tmp_mem.int_pages;
	}
	return 0;
}

 

	
	
	



