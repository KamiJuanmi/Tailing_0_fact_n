// Numbers of trailing 0 in the factorial of a number n with d digits, tested up to 1800 digits
// Based on recursive divisions of the number by 5 and its quotients

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DIVIDER 5

void printVector(int * vector, int vec_size, int mode) //TRUE LEFT-RIGHT, FALSE RIGHT-LEFT
{
        if(mode) {
                for(int i=0; i<vec_size; i++)
                {
                        printf("%d", vector[i]);
                }
        }else
        {       
                for(int i=vec_size-1; i>=0; i--)
                {
                        printf("%d", vector[i]);
                }
        }
        printf("\n");
}

void main (void) {
        int i, dividend;
        int num[1800];  // Every digit of the number is stored in one position, in order to use less memory we can store
                        // blocks of more than 1 digit but this way is easier

        FILE *F1;

        F1 = fopen ("Data.DAT", "r");

        for (i = 1; i <10; i++) {                // Should change it to be dynamic in order to do it as many times as numbers has the file without a 'magic number'  
        
                int quotient[90000];            // Every digit of every quotient will be stored like the principal number (still too large for a 'normal' type)
                                                
                long int positions[4000];       // We store the position of the beginning of the next quotient
                                                // FIRST QUOTIENT --> [0,position[0])
                                                // SECOND QUOTIENT -> [position[0], position[1])
                int sum[100000];                // Used to sum the quotients
                int m = 0;
                char aux;
                //INPUT OF THE DATA (was given in this format)
                do {
                        aux=fgetc(F1);
                        if(aux <58 && aux>47) //We check if it's a number
                        {
                                num[m]=(int)aux -48;
                                m++;
                        }
                } while(aux !='\n'); //Until the end of the line


                //OUTPUT OF THE NUMBER
                printf("Num = ");
                printVector(num, m, 1);
                
                //DIVIDING
                int num_divisions = 0;
                int t=0;
                int numb_digits_next, must_cont=1;
                do {
                        
                        dividend=num[0];

                        //Dividing the number by 5 as we learn as kids
                        // 60 / 5 ----- 6/5 => q1=1 r1=1 ------ we take down 0 and we add it to r1*10 ---- 10/5 => q2=2 r2=0 ----- we sum the quotients q1*10+q2
                        for(int j=0; j<m; j++) {
                                if(dividend<DIVIDER && dividend != 0 )
                                {
                                        if(j!=(m-1))
                                                dividend = dividend*10+num[j+1];
                                }else
                                {
                                        quotient[t] = dividend / DIVIDER;
                                        dividend =( dividend % DIVIDER )* 10 + num [j+1];
                                        t++;
                                }
                        }
                        
                        // We store where it's the next quotient
                        positions[num_divisions] = t;
                        
                        // We move the quotient to the vector 'num' in order to divide it again
                        m=0;
                        for(int j = num_divisions==0 ? 0 : positions[num_divisions-1]; j<positions[num_divisions]; j++)
                        {
                                num[m] = quotient[j];
                                m++;
                        }
                        
                        // We check if we have finished dividing
                        numb_digits_next = num_divisions == 0 ? positions[num_divisions] : positions[num_divisions] - positions[num_divisions-1];

                        if(numb_digits_next==1)
                        {
                                must_cont= quotient[positions[num_divisions]-1] >= 5 ? 1 : 0;
                        }

                        num_divisions++;
                        
                } while(must_cont);
                

                // SUMMATION
                
                // We obtain the first quotient and 'write' it from right to left -- num 200 == 002
                // This way is easier to compute the summation, S[i] = S[i] + B[i] being i from 0,...,p ->
                // If  S[p] > 9, S[p+1] = 1, but because we don't preset the vector of the summation all in 0 we cannot do
                // S[p+1]++, S[p+1] is uncertain. We have more space allocated than needed for the vector so we can do p+1 without 
                // going out of scope but we can implement this using dynamic memory so we don't 'lose' that memory
                m=positions[0]-1;
                int sum_digits= positions[0];
                for(int j=0; j<sum_digits; j++)
                {
                        sum[m]=quotient[j];
                        m--;
                }

                // We sum every quotient
                for(int k=0; k<num_divisions-1; k++) {
                        
                        int next_number[435];
                        numb_digits_next= positions[k+1] - positions[k];

                        int aux=positions[k+1]-positions[k]-1;
                        
                        // Obtain the next one R-L
                        for(int j= positions[k]; j<positions[k+1]; j++)
                        {
                                next_number[aux]= quotient[j];
                                aux--;
                        }
                        
                        // Sum as previously explained
                        for(int j=0; j<numb_digits_next; j++)
                        {
                                int sum_temp = sum[j] + next_number[j];
                                sum[j] = sum_temp % 10;
                                if(sum_temp > 10)
                                {
                                        if(j+1 != sum_digits)
                                        {
                                                sum[j+1] +=1;
                                        }else{
                                                sum[sum_digits]=sum_temp /10;
                                                sum_digits++;
                                        }
                                }
                        }
                }


                printf("Number of trailing zeros = ");
                printVector(sum,sum_digits,0);  // To show the result we just show the numbers from right to left               
                
                int summation_sum_digits =0;
                
                for(int j=0;j<sum_digits;j++)
                {
                        summation_sum_digits+=sum[j];
                }
                printf("sum of the digits of the number represeting the number of zeros = %d\n", summation_sum_digits);
                
        }
        fclose (F1);

}
