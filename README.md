# Revisiting-Yoyo-Tricks-on-AES
This repository contains the source codes for the paper, '**{``Revisiting Yoyo Tricks on AES''} **'.

## Language Used for code
1. C-language

## Setup

1. To install gcc, use the following command:
	* `sudo apt update`
	* `sudo apt install build-essential`


## File Structure

1. (a)  `1_aes_and_inv_aes.h`: Script for writing necessary functions for encryption and decryption of AES-128.
   (b)  `1_aes_and_inv_aes_20_round.h`: Script for writing necessary functions for encryption and decryption of AES with 20 round. We use this as a random permutation.


2. (a) `2_yoyo_5_round_random.c`: Script to measure the success of 5-round AES distingusher when black box cipher is random permutation using the drand48() function.
   (b) `2_yoyo_5_round_random_20_round_aes.c' Script to measure the success of 5-round AES distingusher when black box cipher is random permutation using the 20 round AES-128 as a random function.

3. `3_yoyo_5_round_aes.c`: Script to measure the success of 5-round AES distingusher when black box cipher is 5-round AES.

Note : In 2 and 3  the variable max_j and max_k  represent the values of 'x' and 'y' respectively. 

## Usage
1. compile the programme: `gcc file_name.c -lm`
2. run the file:           `./a.out`
