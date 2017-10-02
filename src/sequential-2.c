#include <stdio.h>
#include <stdlib.h>

// The input argument is "<number of Item> <weight of the bag> <weight of item1 (w1)> <value of item1 (v1)> <w2> <v2> ..."
int main(int argc, char *argv[]){

	const int itemNum;
	const int maxWeight;

	if((argc-1)%2!=0){
		printf("The number of input arguments is wrong. Maybe you missed an argument for an item. Please try again.");
		return 0;
	}

	sscanf(argv[1],"%i",&itemNum);
	sscanf(argv[2],"%i",&maxWeight);

	struct item{
		int weight;
		int value;
	}items[itemNum+1]; //item1=item[1], item[0] is not used.

	int m[itemNum+1][maxWeight+1];

	for(int i=1;i<itemNum+1;i++){
		sscanf(argv[i*2+1],"%i",&items[i].weight);
		sscanf(argv[i*2+2],"%i",&items[i].value);
	}

	for(int i=0;i<itemNum+1;i++){
		for(int j=0;j<maxWeight+1;j++){
			if(i==0 || j==0){
				m[i][j]=0;
			}
		}
	}

	for(int i=1;i<itemNum+1;i++){
		for(int w=1;w<maxWeight+1;w++){
			if(items[i].weight<=w && (m[i-1][w-items[i].weight]+items[i].value > m[i-1][w])){
				m[i][w]=m[i-1][w-items[i].weight]+items[i].value;				
			}else{
				m[i][w]=m[i-1][w];
			}
		}
	}

	printf("The max value is %d \n", m[itemNum][maxWeight]);
	return 0;
}
