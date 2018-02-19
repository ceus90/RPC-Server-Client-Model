#include "dcrpc.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

msg *
get_path_1_svc(msg *argp, struct svc_req *rqstp)
{
    static msg  result;
    char *pwd;
    printf("\nClient request received for getting the path of the current directory\n");
    pwd = getenv("PWD");//Getting current directory path and storing it in local variable
    strcpy(result.a,pwd);//Copying the path to the object of struct variable, and sending it to client
    printf("The path of the current directory sent to the client\n");
    return &result;//return path
}

node *
sort_list_1_svc(node *argp, struct svc_req *rqstp)
{
    static node  result;
    int i, j, temp;
    int temp_array[999];
	printf("Client request received for sorting a list\n");

    for(i = 0; i < argp->list_size; i++)//Gettin unsorted list from client and copying it in the local variable
        temp_array[i] = argp->list_data[i];
    
    printf("\nThe unsorted list received from client is as below:\n");
    for(i = 0; i < argp->list_size; i++)
		printf("%d, ",temp_array[i]);
	//performing bubble sort on the unsorted list
    for(i = 0; i < argp->list_size; i++)
        for(j = 0; j < argp->list_size-1; j++)
            if(temp_array[j] > temp_array[j + 1] ){
                temp = temp_array[j];
                temp_array[j] = temp_array[j+1];
                temp_array[j+1] = temp;
            }
    //Copying the sorted list to the object of struct variable in server, and sending it to client
    for(i = 0; i < argp->list_size; i++)
        result.list_data[i]=temp_array[i];

    printf("\n\nSorted list:\n");
    for(i = 0; i < argp->list_size; i++)
		printf("%d, ",temp_array[i]);
    printf("\nSorted list sent to client\n");
	//Sending the sorted list to client
    return &result;
}

msg *
get_echo_1_svc(msg *argp, struct svc_req *rqstp)
{
    static msg  *result;
    printf("\nClient request received to echo: %s\n",argp->a);
    result = argp;//Copying the information sent by client into a local variable in server
    printf("Echo sent back to the client\n");
    return result;//Sending back the information stored in local variable of server, which is same as the information sent by the client
}

check_node *
check_file_1_svc(check_node *argp, struct svc_req *rqstp)
{
	static check_node  result;
    printf("\nClient request received to check for a file in current directory\n");
	result.res = 0;//The condition if the file is not found
	char *fname;
	fname = argp->fname;//Copying the filename sent by client, into a local variable
	if( access( fname, F_OK ) != -1 )//Checking if the file is present or not
    		result.res = 1;//If present, change the value to 1. Or else, keep it as 0.
    printf("The result whether the file is present or not, sent back to the client\n");	
	return &result;//Send the value to client
}

node *
multiplier_1_svc(node *argp, struct svc_req *rqstp)
{
	static node  result;
	int mat1[999];
	int mat2[999];
	int mat3[999];
	int i, j, k;
	int temp, n, d;
    //Copy the int array into local variable
	temp = argp->list_size;
	n= temp*temp;
    //Split the merged list into two, and copy into respective matrices locally
	for(i = 0; i < n; i++)
		mat1[i] = argp->list_data[i];
	for(j = temp*temp; j<2*n; j++)
		mat2[j-n] = argp->list_data[j];
	
	printf("\nMatrix-1:\n");
		for (i = 0; i < n; i+=temp) { 
    		   for (j = 0; j < temp; j++)
            			printf("%d   ",mat1[i+j]);
			printf("\n");
		   }
	printf("\nMatrix-2:\n");
		for (i = 0; i < n; i+=temp) { 
        		for (j = 0; j < temp; j++) 
            			printf("%d   ",mat2[i+j]);
			printf("\n");
		   }
    //perform matrix multiplication using local variables
	for (k = 0; k < temp; ++k)
        	for (i = 0; i < temp; ++i)
            		for(j = 0; j < temp; ++j)
            		{
                		d = mat3[i*temp+j];
                		d += mat1[i*temp+k] * mat2[k*temp+j];
                		mat3[i*temp+j] = d;
            		}
	for(i = 0; i < n; i++)
		result.list_data[i] = mat3[i]; 
	//Copy the product of two matrices into the object of struct variable and pass the same to server
	printf("\nProduct of Matrix-1 and Matrix-2, sent to client:\n");
		for (i = 0; i < n; i+=temp) { 
    		   for (j = 0; j < temp; j++)
            			printf("%d   ",mat3[i+j]);
			printf("\n");
		   }

	return &result;
}
