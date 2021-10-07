#include <cstdio>
#include <cstdlib>
#include <pthread.h>

#define INT_MAX        100000000


int format;

int read_graph_adjacency
(
	int ** W,
	int ** W_index,
	int * exist,
	int * id,
	FILE * file0,
	int DEG,
	int N
)
{
	char c;
	int number0, number1;
	int previous_node=-1;
	int lines_to_check=0;
	int inter = -1;
	c = getc(file0);
	for(c = getc(file0); c!= EOF; c=getc(file0)){
		if(c =='\n'){
			lines_to_check++;
		}
		if(lines_to_check < 4)
			continue;

		int f0 = fscanf(file0, "%d %d", &number0 , &number1);
		if(f0 != 2 && f0 != EOF)
		{
			printf ("Error: Read %d values, expected 2. Parsing failed.\n",f0);
			printf("Error in line %d !\n",lines_to_check);
			exit (EXIT_FAILURE);
		}
		if (number0 >= N) {
			printf ("Error:  Node %d exceeds maximum graph size of %d.\n",number0,N);
			exit (EXIT_FAILURE);
		}

		exist[number0] = 1; exist[number1] = 1;
		id[number0] = number0;
		if(number0==previous_node) {
			inter++;
		}else {
			inter=0;
		}

		// Make sure we haven't exceeded our maximum degree.
		if (inter >= DEG) {
			printf ("Error:  Node %d, maximum degree of %d exceeded.\n",number0,DEG);
			exit (EXIT_FAILURE);
		}

		// We don't support parallel edges, so check for that and ignore.
		bool exists = false;
		for (int i = 0; i != inter; ++i) {
			if (W_index[number0][i] == number1) {
				exists = true;
				break;
			}
		}
		if (!exists) {
			W[number0][inter] = inter+1;
			W_index[number0][inter] = number1;
			previous_node = number0;
		}
	}
	return 0;
}


int read_graph_weighted
(
	int ** W,
	int ** W_index,
	int * exist,
	int * id,
	FILE * file0,
	int DEG,
	int N
)
{
	char c;
	int number0, number1, number2;
	int lines_to_check=0;
	c = getc(file0);
	for(c = getc(file0); c!= EOF; c=getc(file0)){
		if(c =='\n'){
			lines_to_check++;
		}
		if(lines_to_check < 4)
			continue;

		int f0 = fscanf(file0, "%d %d %d", &number0 , &number1, &number2);
		//printf(" %d; %d; %d\n", number0,number1,number2);
		if(f0 == EOF)
			break;
		if(f0 != 3 && f0 != EOF)
		{
			printf ("Error: Read %d values, expected 3. Parsing failed.\n",f0);
			printf("Error in line %d !\n",lines_to_check);
			exit (EXIT_FAILURE);
		}
		if (number0 >= N) {
			printf ("Error:  Node %d exceeds maximum graph size of %d.\n",number0,N);
			exit (EXIT_FAILURE);
		}

		exist[number0] = 1; exist[number1] = 1;
		id[number0] = number0;


		// We don't support parallel edges, so check for that and ignore.
		bool exists = false;
		int inter = 0;
		for (int i = 0; i < DEG; i++) {
			if((W_index[number0][i] == INT_MAX)){
				inter = i;
				break;
			}
			if(W_index[number0][i] == number1){
				exists = true;
				break;
			}
		}
		// Make sure we haven't exceeded our maximum degree.
		if (inter >= DEG) {
			printf ("Error:  Node %d, maximum degree of %d exceeded.\n",number0,DEG);
			exit (EXIT_FAILURE);
		}
		if (!exists) {
			W[number0][inter] = number2;
			W_index[number0][inter] = number1;
		}
	}
	return 0;
}

int read_graph
(
	int ** W,
	int ** W_index,
	int * exist,
	int * id,
	FILE * file0,
	int DEG,
	int N
)
{
	if(format == 0){
		read_graph_adjacency(W,W_index,exist,id,file0,DEG,N);
	}
	else if(format == 1){
		read_graph_weighted(W,W_index,exist,id,file0,DEG,N);
	}
	return 0;
}


int read_N(FILE * file0)
{
	char c;
	c = getc(file0);
	if(c == '#'){
		format = 0;
	}
	else if(c == '@'){
		format = 1;
	}
	else{
		printf("Not recognized file format starting with %c !\n",c);
		exit (EXIT_FAILURE);
	}
	int n;
	if(fscanf(file0,"%d",&n) != 1){
		printf("Second string in file has to be integer N number of nodes !\n");
		exit(EXIT_FAILURE);
	}
	return n + 1;
}



