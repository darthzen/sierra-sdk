#include "sqf.h"
#include <stdio.h>
#include <stdlib.h>
#define DEFAULT_OUTPUT_FILE_PATH "./out.sqf"

sqf_t sqf;
void printusage()
{
	fprintf(stderr,"Usage : \r\n");
	fprintf(stderr,"\ttest source.sqf target.sqf \n");
	fprintf(stderr,"\ttest filter.sqf (overwrite)\n");
	fprintf(stderr,"\ttest (no arg \"out.sqf\" should be generated)\n\n");
	exit(1);
}
int main(int arc, char **argv)
{
    sqf_createbuffer();
    if(arc>1)
    {
		if(arc==2)
		{
			if(strncmp(argv[1],"-h",2)==0)
			{
				printusage();
			}			
		}
		fprintf(stderr,"LOAD:%s\n",argv[1]);fflush(stderr);
        if(sqf_load_file(argv[1])==0)
    	{
			printusage();
    	}
    }
    
    sqf_set(LOG_MASK,eSQF_LOG_Access_Channel);
    sqf_clear(LOG_MASK,eSQF_LOG_QMI_Link_1_RX_PDU);

    sqf_set(MSG_MASK,eSQF_MSG_Flute);
    sqf_clear(MSG_MASK,eSQF_MSG_RFS_Access);
    
    sqf_set(EVENT_MASK,eSQF_EVENT_1X_RSCH_REQUEST);
    sqf_clear(EVENT_MASK,eSQF_EVENT_CM_DS_LCS_MOLR_CONF);
    
    if(arc>2)
    {
        fprintf(stderr,"Save: %s\n",argv[2]);fflush(stderr);
        sqf_save_file(argv[2]);
    }
    else if(arc>1)
    {
        fprintf(stderr,"Save(Overwrite): %s\n",argv[1]);fflush(stderr);
        sqf_save_file(argv[1]);
    }
    else
    {
        fprintf(stderr,"Save: %s\n",DEFAULT_OUTPUT_FILE_PATH);fflush(stderr);
        sqf_save_file(DEFAULT_OUTPUT_FILE_PATH);
    }
    sqf_destroybuffer();
    return 0;
}
