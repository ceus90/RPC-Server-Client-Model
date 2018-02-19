struct msg 
{
            char a[999];
};

struct node 
{
    int list_data[999];
    int list_size;
};

struct check_node
{
	char fname[999];
	int res;
};


program ECHO_PROG{
	version ECHO_VERS{ 
		msg get_PATH(msg) = 1;
		node sort_list(node) = 2;
		msg get_ECHO(msg) = 3;
		check_node check_file(check_node) = 4;
		node multiplier(node) = 5;
	} = 1;
} = 0x31111111;