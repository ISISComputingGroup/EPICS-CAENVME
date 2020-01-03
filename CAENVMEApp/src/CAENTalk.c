#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "getopt.h"

typedef unsigned short ushort;

static const char* program_name = NULL;

#include "CAENVMElib.h"

static int data_size_bytes(CVDataWidth dtsize)
{
    switch(dtsize)
	{
		case cvD32:
			return 4;
			break;

		case cvD16:
			return 2;
			break;

		case cvD8:
			return 1;
			break;

		default:
			break;
	}
	return 0;
}


static CVErrorCodes CaenVmeWrite(int32_t BHandle, CVDataWidth dtsize, uint32_t addr, uint32_t data, 
						  ushort am, int count, int data_step)
{
	int i;
  CVErrorCodes	ret;

  if(dtsize == cvD64)
  {
    printf(" Can't execute a D64 Write Cycle\n");
	return cvCommError;
  }

  for (i=0; i < count; i++)
  {
	ret = CAENVME_WriteCycle(BHandle,addr,&data,am,dtsize);
	if (ret != cvSuccess)
		break;
	data += data_step;
	addr += data_size_bytes(dtsize);
  }

  switch (ret)
		{
			case cvSuccess: 
				break ;
			case cvBusError: 
				printf(" Bus Error !!!\n");
				break ;				   
			case cvCommError: 
				printf(" Communication Error !!!\n");
				break ;
			default: 
				printf(" Unknown Error !!!\n");
				break ;
		}
  return ret;

}

static CVErrorCodes CaenVmeRead(int32_t BHandle, CVDataWidth dtsize, uint32_t addr, uint32_t* data, 
						 ushort am, int count, int addr_step)
{
	int i;
	CVErrorCodes	ret;

   if(dtsize == cvD64)
	{
    printf(" Can't execute a D64 Read Cycle\n");
	return cvCommError;
	}
   
   printf("Reading data\n");
   for (i=0; i < count; i++)
   {
		ret = CAENVME_ReadCycle(BHandle,addr,data,am,dtsize);
		switch (ret)
			{
			case cvSuccess   :
                     switch(dtsize)
					 {
					   case cvD32:
							printf(" %x: %08X\n", addr, *data);
							break;
					   case cvD16:
 							printf(" %x: %04X\n", addr, *data & 0xffff);
							break;
					   case cvD8:
							printf(" %x: %02X\n", addr, *data & 0xff);
							break;
					 }
					 break;

			case cvBusError: 
				printf(" Bus Error !!!\n");
				break ;				   

			case cvCommError : 
				printf(" Communication Error !!!\n");
				break;

			default: 
				printf(" Unknown Error !!!");
				break ;
		}
	if (ret != cvSuccess)
		break;
	addr += data_size_bytes(dtsize);
  }

	return ret;
}

static void print_usage()
{
	printf("\n");
	printf("# read and print data value to screen - assumes board 0 unless -bN specified\n\n");
	printf("    %s [-bN] card address [count(=1)] [address_step(=1)]\n\n", program_name);       
    printf("# writes data value to address\n\n");
	printf("    %s [-bN] -w card address value [count(=1)] [value_step(=0)]\n\n", program_name);
	printf("# card, address and value are all in hex\n");
	printf("# count and value_step are in decimal\n");
	printf("# address_step is in decimal and in units of the data width, so for 16bit wide\n");
	printf("#              data address_step=1 will increment the address by 2 bytes\n");
	printf("\n");
}

#define OP_READ	 1
#define OP_WRITE 2

typedef struct
{
    unsigned my_board;
	unsigned caen_board;
	unsigned base_address;
	unsigned card_increment;
} caen_board_t;

int main(int argc, void *argv[]) 

{
    CVBoardTypes  VMEBoard = cvV1718;
    short         Link = 0; /* not used for v1718 */
    short         Device = 0; /* board number in link */
    int32_t       BHandle;
	int i, opt, count, step, bn = 0, verbose = 0;
	unsigned card, base_address, card_increment;
	ushort am = cvA32_U_DATA;            
	CVDataWidth dtsize = cvD16;
	int op = OP_READ;
	uint32_t addr, data;
	CVErrorCodes ret;
	caen_board_t caen_board[20];
	FILE* inif;
	char ini_path[256];
	char buffer[256];
	unsigned nboard = 0;
	program_name = "CaenTalk.exe";
	while( (opt = getopt(argc, argv, "a:b:d:whv")) != -1 )
	{
		switch(opt)
		{
			case 'w':
				op = OP_WRITE;
				break;

			case 'b': /* board */
				bn = atoi(optarg);
				break;

			case 'h':
				print_usage();
				exit(EXIT_SUCCESS);
				break;

			case 'a':
				i = atoi(optarg);
				switch(i)
				{

					case 16:
						am = 0;
						break;

					case 24:
						am = cvA24_U_DATA;
						break;

					case 32:
						am = cvA32_U_DATA;
						break;

					default:
						printf("Illegal value %d passed to -a; A32 will be assumed\n", i);
						am = cvA32_U_DATA;
						break;
				}
				break;

			case 'd':
				i = atoi(optarg);
				switch(i)
				{
					case 8:
						dtsize = cvD8;
						break;

					case 16:
						dtsize = cvD16;
						break;

					case 32:
						dtsize = cvD32;
						break;

					default:
						printf("Illegal value %d passed to -d; D16 will be assumed\n", i);
						dtsize = cvD16;
						break;
				}
				break;

			case 'v':
				verbose = 1;
				break;

			default:
				print_usage();
				exit(EXIT_FAILURE);
				break;
		}
	}
	if ( (argc - optind) == 0 )
	{
		print_usage();
		exit(EXIT_SUCCESS);
	}

	ini_path[0] = '\0';
	GetModuleFileName(NULL, ini_path, sizeof(ini_path));
	strcpy(strrchr(ini_path, '\\') + 1, "CaenTalk.ini");
	inif = fopen(ini_path, "r");
	if (inif != NULL)
	{
		if (verbose)
		{
			printf("Loading mappings from \"%s\"\n", ini_path);
		}
		while(fgets(buffer, sizeof(buffer), inif) != NULL)
		{
			if (buffer[0] == '#')
			{
				continue;
			}
			if (sscanf(buffer, "%x %x %x %x", &(caen_board[nboard].my_board), &(caen_board[nboard].caen_board), &(caen_board[nboard].base_address), &(caen_board[nboard].card_increment)) != 4)
			{
				printf("Cannot parse line \"%s\" from \"%s\"\n", buffer, ini_path);
				exit(EXIT_FAILURE);
			}
			++nboard;
		}
		fclose(inif);
		if (verbose)
		{
			printf("Loaded %d board mappings\n", nboard);
		}
	}

// lookup board
	Device = bn;
	base_address = 0x0;
	card_increment = 0x10000;
	if (verbose)
	{
		printf("before mapping: %x %x %x\n", Device, base_address, card_increment);
	}
	for(i=0; i<nboard; ++i)
	{
		if (caen_board[i].my_board == bn)
		{
			Device = caen_board[i].caen_board;
			base_address = caen_board[i].base_address;
			card_increment = caen_board[i].card_increment;
		}
	}
	if (verbose)
	{
		printf("after mapping: %x %x %x\n", Device, base_address, card_increment);
	}
// Initialize the Board
    if( CAENVME_Init(VMEBoard, Link, Device, &BHandle) != cvSuccess ) 
    {
        printf("\n\n Error opening CAEN device %d link %d\n", Device, Link);
        exit(EXIT_FAILURE);
    }

	if (op == OP_READ)
	{
		if ( (argc - optind) < 2 )
		{
			printf("error: must give card and address to read from\n");
			print_usage();
			CAENVME_End(BHandle);
			exit(EXIT_FAILURE);
		}
		sscanf(argv[optind], "%x", &card);
		sscanf(argv[optind+1], "%x", &addr);
		addr += (base_address + card * card_increment);
		count = ((argc - optind) > 2) ? atol(argv[optind+2]) : 1;
		step = ((argc - optind) > 3) ? atol(argv[optind+3]) : 1;
		ret = CaenVmeRead(BHandle, dtsize, addr, &data, am, count, step);
	}

	if (op == OP_WRITE)
	{
		if ( (argc - optind) < 3 )
		{
			printf("error: must give card, address and value to write to\n");
			print_usage();
			CAENVME_End(BHandle);
			exit(EXIT_FAILURE);
		}
		sscanf(argv[optind], "%x", &card);
		sscanf(argv[optind+1], "%x", &addr);
		addr += (base_address + card * card_increment);
		sscanf(argv[optind+2], "%x", &data);
		count = ((argc - optind) > 3) ? atol(argv[optind+3]) : 1;
		step = ((argc - optind) > 4) ? atol(argv[optind+4]) : 0;
		ret = CaenVmeWrite(BHandle, dtsize, addr, data, am, count, step);
	}
    CAENVME_End(BHandle);
	return ret;
}
