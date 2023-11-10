#include "1_aes_and_inv_aes_20_round.h"
#include<time.h>
#include<math.h>
#include <stdio.h>
#include <stdlib.h>




u8 const column[4][4] = {{0,1,2,3},{4,5,6,7},{8,9,10,11},{12,13,14,15}};


int v_word(u8* state){   // finding the weight of bytes in a column
	int i,flag = 0;
	for(i = 0; i < 4; i++)
		if(state[i] == 0)
			flag = flag + 1;
	return flag;

}


int v_column(u8* state){   // finding the weight of column in a state
	int i,j;
	int flag = 0;
	int count = 0;
	for(i = 0; i < 4; i++){
		flag = 0;
		for(j = 0; j < 4; j++){
			if(state[4*i+j] == 0)
				flag = flag+1;
			}
		if(flag == 4)
			count = count + 1;
		}
	return count;
}





void simpleSwap_column(u8* state1,u8* state2, u8* out){   // SIMPLESWAP of two states
	
	int i,j;
	u8 temp[16];
	for(i = 0; i < 16; i++)
		out[i] = state2[i];
	
	for(i = 0; i < 16; i++)
		temp[i] = state1[i] ^ state2[i];
	if(v_column(temp) == 0){
		for(j = 0; j < 4; j++)
			out[column[0][j]] = state1[column[0][j]];
		}
	else{	
		for(i = 0; i < 4; i++)
			if(state1[column[i][0]] != state2[column[i][0]] || state1[column[i][1]] != state2[column[i][1]] || state1[column[i][2]] != state2[column[i][2]] || state1[column[i][3]] != state2[column[i][3]]){
				for(j = 0; j < 4; j++)	
					out[column[i][j]] = state1[column[i][j]];
				break;
				}
		}

}
		

/***  random permutation using 20 round AES ***/
void r_perm_aes20(u8 state[16], u8 subkey[21][16]){   
	int i;
	keyAddition(state,subkey[0]);
	for(i = 1; i < 21; i++){
		subBytes(state);
		shiftRows(state);
		mixColumns(state);
		keyAddition(state,subkey[i]);
		}
}


/****    Randomly generate 128-bit state     ****/
void r_func_drand48(u8 state[16]){ 

int i;
for(i = 0; i < 16; i++)
	state[i] = 256.0*drand48();

}



int main(){

srand48(time(NULL));

u8 state1[16];
u8 state2[16];
u8 state[16];

u8 key[16];
u8 subkey[21][16];






long int i,j,k,x;
long int max_j = 10810, max_k ;     //  here j is the x_loop and k is the y_loop in the paper.  we have to chenged the value max_k as required.   here max_j = 2^{13.4} = 10810 
long int success = 0;
int flag = 0;
u8 temp1[16],temp2[16],temp[4];

int exp;
int kk;
float tmp;

for(kk = 0;kk < 93; kk++){
	success = 0;
	tmp = 11.4+0.05*kk;
	max_k = (long int)pow(2,tmp);

	for(exp = 0; exp < 100; exp++){
		r_func_drand48(key);           // generate a 128-bit random key
		genAllRoundKeys(key, subkey);  
		
		for(j = 0;j < max_j ;j++){
			flag = 0;
			r_func_drand48(state1);   // generate a 128-bit state

			// generating the second state
			for(i = 0; i < 4; i++)    //  randomly generating the different first column
				state2[i] = 256.0*drand48();  
			for(i = 4; i < 16; i++)   // copying the other 3 columns from the first state
				state2[i] = state1[i];

			for(k = 0; k < max_k ;k++){
				
				invShiftRows(state1);
				invShiftRows(state2);

				r_perm_aes20(state1,subkey);
				r_perm_aes20(state2,subkey);
			
				invMixColumns(state1);
				invShiftRows(state1);

				invMixColumns(state2);
				invShiftRows(state2);

				simpleSwap_column(state1,state2,temp1);
				simpleSwap_column(state2,state1,temp2);

				
				for(i = 0; i < 16; i++)
					state1[i] = temp1[i];
				for(i = 0; i < 16; i++)
					state2[i] = temp2[i];

				shiftRows(state1);		
    			mixColumns(state1);

				shiftRows(state2);		
    			mixColumns(state2);

				r_perm_aes20(state1,subkey);
				r_perm_aes20(state2,subkey);
				
				shiftRows(state1);
				shiftRows(state2);

				for(i = 0; i < 4; i++){
					for(x = 0; x < 4; x++){
						temp[x] = state1[column[i][x]] ^ state2[column[i][x]];
						}
					if(v_word(temp) > 1 && v_word(temp) < 4){
						flag = 1;
						break;
						}
					}

				if(flag == 1)
					break;


				simpleSwap_column(state1,state2,temp1);
				simpleSwap_column(state2,state1,temp2);

				for(i = 0; i < 16; i++)
					state1[i] = temp1[i];
				for(i = 0; i < 16; i++)
					state2[i] = temp2[i];

				}
			if(k == max_k){
				success++;		
				break;
				}
			}
		
		}
	printf("Total number of success = %ld when y = 2^%f=%ld\n",(exp-success),tmp,max_k);
	}
return 0;
	
}



