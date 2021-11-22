#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sim_bp.h"
#include "math.h"
#include <iostream>
#include <cassert>
#include<bits/stdc++.h>
using namespace std; 

/*  argc holds the number of command line arguments
    argv[] holds the commands themselves

    Example:-
    sim bimodal 6 gcc_trace.txt
    argc = 4
    argv[0] = "sim"
    argv[1] = "bimodal"
    argv[2] = "6"
    ... and so on
*/

int main(int argc, char* argv[])
{
    FILE* FP;               // File handler
    char* trace_file;       // Variable that holds trace file name;
    bp_params params;       // look at sim_bp.h header file for the the definition of struct bp_params
    char outcome;           // Variable holds branch outcome
    unsigned long int addr; // Variable holds the address read from input file

    if (!(argc == 4 || argc == 5 || argc == 7))
    {
        printf("Error: Wrong number of inputs:%d\n", argc - 1);
        exit(EXIT_FAILURE);
    }

    params.bp_name = argv[1];

    // strtoul() converts char* to unsigned long. It is included in <stdlib.h>
    if (strcmp(params.bp_name, "bimodal") == 0)              // Bimodal
    {
        if (argc != 4)
        {
            printf("Error: %s wrong number of inputs:%d\n", params.bp_name, argc - 1);
            exit(EXIT_FAILURE);
        }
        params.M2 = strtoul(argv[2], NULL, 10);
        trace_file = argv[3];
        printf("COMMAND\n%s %s %lu %s\n", argv[0], params.bp_name, params.M2, trace_file);
        
        
    }
    else if (strcmp(params.bp_name, "gshare") == 0)          // Gshare
    {
        if (argc != 5)
        {
            printf("Error: %s wrong number of inputs:%d\n", params.bp_name, argc - 1);
            exit(EXIT_FAILURE);
        }
        params.M1 = strtoul(argv[2], NULL, 10);
        params.N = strtoul(argv[3], NULL, 10);
        trace_file = argv[4];
        printf("COMMAND\n%s %s %lu %lu %s\n", argv[0], params.bp_name, params.M1, params.N, trace_file);

    }
    else if (strcmp(params.bp_name, "hybrid") == 0)          // Hybrid
    {
        if (argc != 7)
        {
            printf("Error: %s wrong number of inputs:%d\n", params.bp_name, argc - 1);
            exit(EXIT_FAILURE);
        }
        params.K = strtoul(argv[2], NULL, 10);
        params.M1 = strtoul(argv[3], NULL, 10);
        params.N = strtoul(argv[4], NULL, 10);
        params.M2 = strtoul(argv[5], NULL, 10);
        trace_file = argv[6];
        printf("COMMAND\n%s %s %lu %lu %lu %lu %s\n", argv[0], params.bp_name, params.K, params.M1, params.N, params.M2, trace_file);

    }
    else
    {
        printf("Error: Wrong branch predictor name:%s\n", params.bp_name);
        exit(EXIT_FAILURE);
    }

    // Open trace_file in read mode
    FP = fopen(trace_file, "r");
    if (FP == NULL)
    {
        // Throw error and exit if fopen() failed
        printf("Error: Unable to open file %s\n", trace_file);
        exit(EXIT_FAILURE);
    }

    
    //array size declaration for bimodal
    int* arr ;
    if (strcmp(params.bp_name, "bimodal") == 0 ||(strcmp(params.bp_name, "hybrid") == 0)){
    	arr = new int[(int)pow(2, params.M2)];         //make it dynamic array
    	for (int i = 0; i < pow(2, params.M2); i++) {
        	arr[i] = 2;                                     //initializing array to weakly taken
    	}
	}

  

    //array size for gshare PC
    int* arr2;
    if (strcmp(params.bp_name, "gshare") == 0 || (strcmp(params.bp_name, "hybrid") == 0)){
		
	    arr2 = new int[(int)pow(2, params.M1)];
	    //initializing to weakly taken,2
	    for (int i = 0; i < pow(2, params.M1); i++) {
	        arr2[i] = 2;
	    }
	}

    //initializing BHR to 0
    unsigned long int BHR;
    BHR = 0;

    //chooser table
    long int* arr3; 
    if (strcmp(params.bp_name, "hybrid") == 0)
	{
		arr3 = new long int[(int)pow(2, params.K)];
	    for (int i = 0; i < pow(2, params.K); i++) {
	        arr3[i] = 1;
	    }
	}

	
	int a = 0;
	unsigned int misprediction = 0;
	
    char str[2];
    while (fscanf(FP, "%lx %s", &addr, str) != EOF)
    {
	
        outcome = str[0];
        if (outcome == 't');
           // printf("%lx %s\n", addr, "t");           // Print and test if file is read correctly
        else if (outcome == 'n');
          //  printf("%lx %s\n", addr, "n");          // Print and test if file is read correctly
        /*************************************
            Add branch predictor code here
        **************************************/
		bool prediction;
		if(outcome == 'n'){
			prediction = false;
		}
		else prediction = true;


        addr = addr>>2; //right shift by two bits to ignore the lower two bits of PC
        //printf("check4: \n");
        int pred;
		if ((strcmp(params.bp_name, "bimodal") == 0)){  //Bimodal
		    	
		    unsigned mask;
        	mask = ~(~0 << params.M2);
        	unsigned int lastMbits = addr & mask;

        	pred = arr[lastMbits];
        
        	//printf("pred1: %d\n",pred);
        	if(outcome == 'n'){
        		if(pred > 1) misprediction++;
			}
			else{
				if(pred<2) misprediction++;
			}
			
        	if(outcome == 'n'){
        		if(pred !=0)
					pred--;
        	
			}
		
			else{
				if (pred != 3) 
					pred++;
			}
			//printf("lastmbits : %d\n",lastMbits);
			arr[lastMbits] = pred;
			//printf("pred2: %d\n",pred);
		
		}
		
		int pred1;	
		if ((strcmp(params.bp_name, "gshare") == 0)){ //gshare
			//printf("check3: ");
			//unsigned mask1;
        	unsigned mask_1;
        	mask_1 = ~(~0 << (params.M1 - params.N));
        	unsigned int n_bit_mask = ~(~0 << (params.N));
        	// 0xffffeeee
        	// 0x00000007
        	unsigned int NbitsPC = (addr >> (params.M1 - params.N)) & n_bit_mask;		// N bits of addr
        	unsigned int M1bits = addr % ((int)pow(2,params.M1));
			//unsigned int lastMNbits = addr % ((int)pow(2,params.M1-params.N));        //getting the last m-n bits of addr
			unsigned int lastMNbits = (addr & mask_1);
        	
       		unsigned int index = BHR ^ NbitsPC; 							//XOR operation between  N bits of BHR and  N bits of addr
        	int index1 = (index << (params.M1 - params.N)) | lastMNbits;    //joining the index and m - n bits of addr
        	
        	
        	pred1 = arr2[index1];
        	//printf("check:1 %d\n",pred1);
        	
        	if(outcome == 'n'){
        		if(pred1 > 1) misprediction++;
			}
			else{
				if(pred1 < 2) misprediction++;
			}
			
        	if(outcome == 'n'){
        		//printf("output_n,pred1:  %d\n",pred1);
        	BHR = BHR >> 1;
        	if(pred1 !=0)
			pred1--;
			//printf("pred1: %d\n",pred1);
        	
			}
		
			else{
				
				BHR = BHR >> 1;
            	BHR = (1 << (params.N - 1))|BHR;
				if (pred1 != 3) {
					pred1++;
				
            	}
			}
			
			arr2[index1] = pred1;
			
		    }
		
		int pred2;
		if (strcmp(params.bp_name, "hybrid") == 0)					//hybrid
		{
			unsigned int lastK1bits = addr % ((int)pow(2,params.K));
        	pred2 = arr3[lastK1bits];
        	//printf("checka: %d\n",pred2);
			unsigned mask;														//bimodal
	        mask = ~(~0 << params.M2);
	        unsigned int lastMbits = addr & mask;

        	pred = arr[lastMbits];	
        	bool bimodal_pred;
        	if(pred > 1) bimodal_pred = true;
        	else bimodal_pred = false;
			
        	unsigned mask_1;
        	mask_1 = ~(~0 << (params.M1 - params.N));
        	unsigned int n_bit_mask = ~(~0 << (params.N));
        	// 0xffffeeee
        	// 0x00000007
        	unsigned int NbitsPC = (addr >> (params.M1 - params.N)) & n_bit_mask;		// N bits of addr
        	unsigned int M1bits = addr % ((int)pow(2,params.M1));
			//unsigned int lastMNbits = addr % ((int)pow(2,params.M1-params.N));        //getting the last m-n bits of addr
			unsigned int lastMNbits = (addr & mask_1);
        	
       		unsigned int index = BHR ^ NbitsPC; 							//XOR operation between  N bits of BHR and  N bits of addr
        	int index1 = (index << (params.M1 - params.N)) | lastMNbits;    //joining the index and m - n bits of addr
        	
        	bool gshare_pred;
        	pred1 = arr2[index1];
			if(pred1 > 1) gshare_pred = true;
			else gshare_pred = false;				      
			  	
        	int overall_pred;
        	if(pred2 < 2) overall_pred = pred; 				
								
        	else overall_pred = pred1;										//chooses gshare
        	
        	// Update selected branch predictor, but always update BHR
        	if(pred2 > 1) // gshare was selected
        	{
        		// Update gshare
        		if(outcome == 'n'){
        			//printf("output_n,pred1:  %d\n",pred1);
        			BHR = BHR >> 1;
        			if(pred1 !=0)
						pred1--;
					//printf("pred1: %d\n",pred1);
				}
		
				else{
				
					BHR = BHR >> 1;
            		BHR = (1 << (params.N - 1))|BHR;
					if (pred1 != 3) {
						pred1++;
				
            		}
				}
			
				arr2[index1] = pred1;
		    }
			else // update bimodal, but always update BHR
			{
				if(outcome == 'n'){
					BHR = BHR >> 1;
        			if(pred !=0)
						pred--;
        	
				}
		
				else{
					BHR = BHR >> 1;
            		BHR = (1 << (params.N - 1))|BHR;
					if (pred != 3) 
						pred++;
				}
				//printf("lastmbits : %d\n",lastMbits);
				arr[lastMbits] = pred;	
			}
			
			// Update chooser table counter
			// Both incorrect or correct								
        	if(((prediction == bimodal_pred) && (prediction == gshare_pred)) || ((prediction != bimodal_pred) && (prediction != gshare_pred)))
			{
				//pred2 = pred2; // no change
				arr3[lastK1bits] = pred2;	
			} 
        	else if((prediction == gshare_pred) && (prediction != bimodal_pred)){								//if gshare is correct increment
        		if(pred2 < 3) pred2++;
        		arr3[lastK1bits] = pred2;
        	}
        	else if ((prediction == bimodal_pred) && (prediction != gshare_pred)) {											//if bimodal is correct decrement
        		if(pred2 > 0) pred2--;
        		arr3[lastK1bits] = pred2;
        	}
        	else
			{
				assert(0);
			}
			
        	
			
			// Update misprediction stat
			if(outcome == 'n'){
        		if(overall_pred > 1) misprediction++;
			}
			else{
				if(overall_pred < 2) misprediction++;
			}
		}
		
    
    
   // for(int i = 0; i< pow(2,params.M1 - params.N);i++){
   // 	printf(" Btb: %d\n",arr2[i]);
	//}
	
	
//	for(int i = 0; i< pow(2,params.M1);i++){
   // 	printf(" Btb: %d\n",arr2[i]);
//	}
	
    
    a++;
	}
	printf("OUTPUT \n");
	printf("number of predictions: %d \n",a);
	printf("number of mispredictions: %u \n",misprediction);
	float temp=misprediction;
	float rate_of_misprediction = (temp/a)*100;
	cout<<fixed<<setprecision(2)<<"misprediction rate:"<<rate_of_misprediction<<"%"<<endl;
   
    if(strcmp(params.bp_name, "hybrid") == 0)
		{
		printf("FINAL CHOOSER CONTENTS\n");
			for (int i = 0; i < pow(2, params.K); i++) 
	        	cout<<i<<"	"<<arr3[i]<<endl;                                     
	    }
	if((strcmp(params.bp_name, "gshare") == 0)  || (strcmp(params.bp_name, "hybrid") == 0) )
		{
		printf("FINAL GSHARE CONTENTS\n");
			for (int i = 0; i < pow(2, params.M1); i++) 
	        	cout<<i<<"	"<<arr2[i]<<endl;                                   
	    }    
	if((strcmp(params.bp_name, "bimodal") == 0)  || (strcmp(params.bp_name, "hybrid") == 0) )	
		{
		printf("FINAL BIMODAL CONTENTS\n");
			for (int i = 0; i < pow(2, params.M2); i++) 
	        	cout<<i<<"	"<<arr[i]<<endl;                                     
	    }

		    
	return 0;
}







