#include "tmp.h"





int tas_lock(atomic_int test){
	return atomic_exchange( &test, 2);
};
