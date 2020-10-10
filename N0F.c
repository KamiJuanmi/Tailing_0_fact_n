// Numbers of trailing 0 in the factorial of a number n with d digits, tested up to 1800 digits
// Based on recursive divisions of the number by 5 and its quotients

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FIRST_SIZE 10
#define DIVIDER 5

void printVector(int *vector, int vec_size, int mode) //TRUE LEFT-RIGHT, FALSE RIGHT-LEFT
{
        if (mode)
        {
                for (int i = 0; i < vec_size; i++)
                {
                        printf("%d", vector[i]);
                }
        }
        else
        {
                for (int i = vec_size - 1; i >= 0; i--)
                {
                        printf("%d", vector[i]);
                }
        }
        printf("\n");
}

void main(void)
{
        FILE *F1;

        F1 = fopen("Data.DAT", "r");

        if (F1 == NULL)
        {
                perror("ERROR OPENING THE INPUT FILE\n");
                exit(EXIT_FAILURE);
        }

        for (int i = 1; i < 9; i++) // Should change it to be dynamic in order to do it as many times as numbers has the file without a 'magic number'
        {
                int dividend;

                int *num; // Every digit of the number is stored in one position, in order to use less memory we can store
                          // blocks of more than 1 digit but this way is easier

                int vec_size_num = FIRST_SIZE;
                num = malloc(vec_size_num * sizeof(int));

                if (num == NULL)
                {
                        perror("ERROR ALLOCATING MEMORY FOR NUM\n");
                        exit(EXIT_FAILURE);
                }

                int *quotient; // Every digit of every quotient will be stored like the principal number (still too large for a 'normal' type)
                int vec_size_coc = FIRST_SIZE;

                quotient = malloc(vec_size_coc * sizeof(int));

                if (quotient == NULL)
                {
                        perror("ERROR ALLOCATING MEMORY FOR QUOTIENT\n");

                        exit(EXIT_FAILURE);
                }

                long int *positions; // We store the position of the beginning of the next quotient
                                     // FIRST QUOTIENT --> [0,position[0])
                                     // SECOND QUOTIENT -> [position[0], position[1])

                int vec_size_pos = FIRST_SIZE;

                positions = malloc(vec_size_pos * sizeof(long int));

                if (positions == NULL)
                {
                        perror("ERROR ALLOCATING MEMORY FOR POSITIONS\n");
                        exit(EXIT_FAILURE);
                }

                int *summation; // Used to sum the quotients
                int vec_size_summation = FIRST_SIZE;

                summation = malloc(vec_size_summation * sizeof(int));

                if (summation == NULL)
                {
                        perror("ERROR ALLOCATING MEMORY FOR SUMMATION\n");
                        exit(EXIT_FAILURE);
                }

                int m = 0;

                char aux;
                //INPUT OF THE DATA (was given in this format)
                do
                {
                        aux = fgetc(F1);
                        if (aux < 58 && aux > 47) //We check if it's a number
                        {
                                if (m == vec_size_num)
                                {
                                        num = realloc(num, 2 * vec_size_num * sizeof(int));

                                        if (num == NULL)
                                        {
                                                perror("ERROR RESIZING NUM\n");
                                                exit(EXIT_FAILURE);
                                        }

                                        vec_size_num *= 2;
                                }
                                num[m] = (int)aux - 48;
                                m++;
                        }

                } while (aux != '\n'); //Until the end of the line

                //OUTPUT OF THE NUMBER
                printf("Num = ");
                printVector(num, m, 1);

                //DIVIDING
                int number_divisions = 0;
                int t = 0;
                int number_digts_next, must_cont = 1;

                do
                {
                        dividend = num[0];

                        //Dividing the number by 5 as we learn as kids
                        // 60 / 5 ----- 6/5 => q1=1 r1=1 ------ we take down 0 and we add it to r1*10 ---- 10/5 => q2=2 r2=0 ----- we sum the quotients q1*10+q2
                        for (int j = 0; j < m; j++)
                        {
                                if (dividend < DIVIDER && dividend != 0)
                                {
                                        if (j != (m - 1))
                                                dividend = dividend * 10 + num[j + 1];
                                }
                                else
                                {
                                        if (t == vec_size_coc)
                                        {
                                                quotient = realloc(quotient, 2 * vec_size_coc * sizeof(int));

                                                if (quotient == NULL)
                                                {
                                                        perror("ERROR AL HACER RESIZE DE quotient\n");
                                                        exit(EXIT_FAILURE);
                                                }

                                                vec_size_coc *= 2;
                                        }
                                        quotient[t] = dividend / DIVIDER;
                                        dividend = (dividend % DIVIDER) * 10 + num[j + 1];
                                        t++;
                                }
                        }

                        if (number_divisions == vec_size_pos)
                        {
                                positions = realloc(positions, (vec_size_pos + 5) * sizeof(long int));

                                if (positions == NULL)
                                {
                                        perror("ERROR AL HACER RESIZE DE positions\n");
                                        exit(EXIT_FAILURE);
                                }

                                vec_size_pos += 5;
                        }
                        // We store where it's the next quotient
                        positions[number_divisions] = t;

                        // We move the quotient to the vector 'num' in order to divide it again
                        m = 0;

                        // Reset of the vector 'num'
                        free(num);
                        vec_size_num = FIRST_SIZE;
                        num = malloc(FIRST_SIZE * sizeof(vec_size_num));
                        if (num == NULL)
                        {
                                perror("ERROR AL RESERVAR MEMORIA DE NUM DESPUES DEL RESET\n");
                                exit(EXIT_FAILURE);
                        }

                        for (int j = number_divisions == 0 ? 0 : positions[number_divisions - 1]; j < positions[number_divisions]; j++)
                        {
                                if (m == vec_size_num)
                                {
                                        num = realloc(num, 2 * vec_size_num * sizeof(int));

                                        if (num == NULL)
                                        {
                                                perror("ERROR AL HACER RESIZE DE NUM\n");
                                                exit(EXIT_FAILURE);
                                        }

                                        vec_size_num *= 2;
                                }
                                num[m] = quotient[j];
                                m++;
                        }

                        // We check if we have finished dividing
                        number_digts_next = number_divisions == 0 ? positions[number_divisions] : positions[number_divisions] - positions[number_divisions - 1];

                        if (number_digts_next == 1)
                        {
                                must_cont = quotient[positions[number_divisions] - 1] >= 5 ? 1 : 0;
                        }

                        number_divisions++;

                } while (must_cont);

                // SUMMATION

                // We obtain the first quotient and 'write' it from right to left -- num 200 == 002
                // This way is easier to compute the summation, S[i] = S[i] + B[i] being i from 0,...,p ->
                // If  S[p] > 9, S[p+1] = 1, but because we don't preset the vector of the summation all in 0 we cannot do
                // S[p+1]++, S[p+1] is uncertain. 
                
                m = positions[0] - 1;
                if (m >= vec_size_summation)
                {
                        summation = realloc(summation, (m + 10) * sizeof(int));

                        if (summation == NULL)
                        {
                                perror("ERROR RESIZING SUM\n");
                                exit(EXIT_FAILURE);
                        }

                        vec_size_summation = m + 10;
                }

                int digits_summation = positions[0];
                for (int j = 0; j < digits_summation; j++)
                {

                        summation[m] = quotient[j];
                        m--;
                }

                // We sum every quotient
                for (int k = 0; k < number_divisions - 1; k++)
                {
                        int *siguiente_numero;
                        number_digts_next = positions[k + 1] - positions[k];

                        siguiente_numero = malloc(number_digts_next * sizeof(int));

                        if (num == NULL)
                        {
                                perror("ERROR ALLOCATING MEMORY FOR THE NEXT NUMBER\n");
                                exit(EXIT_FAILURE);
                        }

                        int aux = positions[k + 1] - positions[k] - 1;

                        // Obtain the next one R-L
                        for (int j = positions[k]; j < positions[k + 1]; j++)
                        {
                                siguiente_numero[aux] = quotient[j];
                                aux--;
                        }

                        // Sum as previously explained
                        for (int j = 0; j < number_digts_next; j++)
                        {
                                int sum_temp = summation[j] + siguiente_numero[j];
                                summation[j] = sum_temp % 10;
                                if (sum_temp > 9) 
                                {
                                        if (j + 1 != digits_summation)
                                        {
                                                summation[j + 1]++;
                                        }
                                        else
                                        {
                                                summation = realloc(summation, (vec_size_summation + 2) * sizeof(int));

                                                if (num == NULL)
                                                {
                                                        perror("ERROR RESIZING SUM2\n");
                                                        exit(EXIT_FAILURE);
                                                }

                                                vec_size_summation += 2;
                                                summation[digits_summation] = sum_temp / 10;
                                                digits_summation++;
                                        }
                                }
                        }
                }

                printf("Number of trailing zeros = ");
                printVector(summation, digits_summation, 0); // To show the result we just show the numbers from right to left

                int summation_digits_summation = 0;

                for (int j = 0; j < digits_summation; j++)
                {
                        summation_digits_summation += summation[j];
                }
                printf("sum of the digits of the number represeting the number of zeros = %d\n", summation_digits_summation);

                free(summation);
                free(quotient);
                free(num);
                free(positions);
        }

        fclose(F1);
}