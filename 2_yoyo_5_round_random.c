#include "1_aes_and_inv_aes.h"
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
		

/***  random permutation using drand48   ***/
void r_perm_drand48(u8 state[16], u8 key[16]){  
	int i;
	for(i = 0; i < 16; i++) 
		state[i] = 256.0*drand48();
}


/****    Randomly generate 128 bits state     ****/
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
u8 subkey[11][16];






long int i,j,k,x;
long int max_j = 10810, max_k;     //  here j is the x_loop and k is the y_loop in the paper.  we have to chenged the value max_k as required.   here max_j = 2^{13.4} = 10810.
long int success = 0;
int flag = 0;
u8 temp1[16],temp2[16],temp[4];

int exp;
int kk;
float tmp;


for(kk = 0;kk < 93; kk++){    // here we run the value of y from 2^{11.4} to 2^{16}. 
	success = 0;              // here we count success when algorithm return AES.
	tmp = 11.4+0.05*kk;
	max_k = (long int)pow(2,tmp);

	for(exp = 0; exp < 100; exp++){
		r_func_drand48(key);
//		genAllRoundKeys(key, subkey);      // no need to generate round keys as here enc and dec functions are random and generated through drand48() function.  
		
		for(j = 0;j < max_j ;j++){
			flag = 0;
			r_func_drand48(state1);
			for(i = 0; i < 4; i++)
				state2[i] = 256.0*drand48();

			for(i = 4; i < 16; i++)
				state2[i] = state1[i];

			for(k = 0; k < max_k ;k++){
				
				invShiftRows(state1);
				invShiftRows(state2);

				r_perm_drand48(state1,key);
				r_perm_drand48(state2,key);
			
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

				r_perm_drand48(state1,key);
				r_perm_drand48(state2,key);
				
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



