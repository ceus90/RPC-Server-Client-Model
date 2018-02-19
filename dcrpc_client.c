#include "dcrpc.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

void
echo_prog_1(char *host)
{
	//objects for struct variables in dcrpc.x files generated by rpcgen command
	CLIENT *clnt;
	msg  *result_1;
	msg  get_path_1_arg;
	node  *result_2;
	node  sort_list_1_arg;
	msg  *result_3;
	msg  get_echo_1_arg;
	check_node  *result_4;
	check_node  check_file_1_arg;
	node  *result_5;
	node  multiplier_1_arg;
	//variables for string/char/int array manipulation
	int ch;
    int cond = 1;
    int temp;
    int i,j,k;
    int d;
	int mat1[999];
	int mat2[999];
    char ctemp[999];
    
#ifndef	DEBUG
	clnt = clnt_create (host, ECHO_PROG, ECHO_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	while(cond){//Client program runs until the user decides to terminate
	//Tasks user would like to do
        printf("\nOptions are as below:\n");
        printf("\nExit --> 0\nPath --> 1\nSort --> 2\nEcho --> 3\nCheck --> 4\nMultiplier --> 5\n");
        printf("\nEnter your choice here: ");
        scanf("%d",&ch);
        
        switch(ch){
                
            case 0://If user decides to terminate the program
                printf("\nExiting the program\n");
                cond = 0;
                break;
                
            case 1://Getting path of current directory from the server
                result_1 = get_path_1(&get_path_1_arg, clnt);
                if (result_1 == (msg *) NULL)
                    clnt_perror (clnt, "call failed");
                else
                    printf("Path sent from server is: %s\n",result_1);
                break;
                
            case 2://Sending unsorted list to server and getting back the sorted list
                printf("\nEnter the size of the list you intend to input: \n");
                scanf("%d",&temp);//Size of the list
                sort_list_1_arg.list_size = temp;
                for(i = 0; i < temp; i++)
                {//Taking input from user for list elements 
                    printf("\nEnter element number %d: \n",i+1);
                    scanf("%d",&d);
                    sort_list_1_arg.list_data[i] = d;
                }//Copying the array elements to the object of struct variable in client, so server can access the same
                printf("\nThe input list is as below:\n");
                for(i = 0; i < temp; i++)
                    printf("%d\n",sort_list_1_arg.list_data[i]);
                result_2 = sort_list_1(&sort_list_1_arg, clnt);
                if (result_2 == (node *) NULL)//sending the unsorted list to server
                    clnt_perror (clnt, "call failed");
                else {
                    printf("\nThe sorted list is as below:\n");
                    for(i = 0; i < temp; i++)//printing the sorted list from server
                        printf("%d\n",result_2->list_data[i]);
                }
                break;
                
            case 3://Taking a string from user and sending it to the server
                printf("\nEnter the information you want server to echo: ");
                scanf("%s",get_echo_1_arg.a);
                scanf("%[^\n]s",&ctemp);
                strcat(get_echo_1_arg.a,ctemp);
                result_3 = get_echo_1(&get_echo_1_arg, clnt);
                if (result_3 == (msg *) NULL)
                    clnt_perror (clnt, "call failed");
                else//Receiving echo from server and printing the same
                    printf("Echo from server is: %s\n",result_3);
                break;
                
            case 4: //Taking the filename from user to check if it's present in the current directory
				check_file_1_arg.res = 0;//Condition if the file is not present
				printf("\nEnter the file you want server to check: ");
                scanf("%s",&check_file_1_arg.fname);
				result_4 = check_file_1(&check_file_1_arg, clnt);
				if (result_4 == (check_node *) NULL)
					clnt_perror (clnt, "call failed");
				if (result_4->res == 0)//If the server returns 0, then file is not present
					printf("\nFile %s not found in the current directory\n",check_file_1_arg.fname);
				else//If the server returns 1, then file is present
					printf("\nFile %s is present in the current directory\n",check_file_1_arg.fname);
                break;

            case 5://First the user inputs the row number or the column number of the matrices.
				printf("\nEnter the dimension of the matrices.\nIf you want to have matrices of size 4 * 4, please enter 4 here: \n");
				scanf("%d",&temp);//Since its the square matrix, I am taking only one input and squaring it later
				multiplier_1_arg.list_size = temp;
				//Taking the input for matrix elements from the user and storing them in local integer array
				printf("\nEnter the elements of first matrix.\n");
				for(i = 0; i < temp*temp; i++)
				{
					printf("Enter the element number %d for Matrix 1- ", i+1);
					scanf("%d",&mat1[i]);
				}
				printf("\nEnter the elements of second matrix.\n");
				for(i = 0; i < temp*temp; i++)
				{
					printf("Enter the element number %d for Matrix 2- ", i+1);
					scanf("%d",&mat2[i]);
				}

				printf("\nThe entered matrices are as below.\n");
				printf("\nMatrix-1:\n");
				for (i = 0; i < temp*temp; i+=temp) { 
					for (j = 0; j < temp; j++)
            			printf("%d   ",mat1[i+j]);
					printf("\n");
				}
				printf("\nMatrix-2:\n");
				for (i = 0; i < temp*temp; i+=temp) { 
					for (j = 0; j < temp; j++) 
            			printf("%d   ",mat2[i+j]);
					printf("\n");
				}
				//Copying the matrix elements into the single integer array, and sending it to server
				for(i = 0; i < temp*temp; i++)
					multiplier_1_arg.list_data[i] = mat1[i];
				for(j = temp*temp; j<2*temp*temp; j++)
					multiplier_1_arg.list_data[j] = mat2[j-temp*temp];
				//Sending the input matrices as single int array and the dimensions to server
				result_5 = multiplier_1(&multiplier_1_arg, clnt);
				if (result_5 == (node *) NULL)
					clnt_perror (clnt, "call failed");
				else {//Printing the result from server
                   printf("\nThe product of the input matrices are as below:\n");
                   for (i = 0; i < temp*temp; i+=temp) { 
    		       for (j = 0; j < temp; j++)
            			printf("%d   ",result_5->list_data[i+j]);
					printf("\n");
					}
				}
                break;
            
            default:
                printf("\nInvalid input. Exiting the program\n");
                exit (0);
                break;
        }
    }

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}

int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}//No changes done here
	host = argv[1];
	echo_prog_1 (host);
	exit (0);
}
