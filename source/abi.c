#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#include "../include/abi_bf.h"
#include "../include/abi_cmt.h"


//!< command line parameter
#define STR_PARAM  "f:"


int main(int argc, char * argv[]){
        
        int c = 0;  //!< opt character
        /*  \brief mode flag
         *  \enum MODE_SCRIPT interpret a brainfuck script file
         *  \enum MODE_INTERACTIVE run the brainfuck interpreter in interactive mode 
         * */
        enum {MODE_SCRIPT,MODE_INTERACTIVE} mode = MODE_INTERACTIVE;

        char * file = NULL;  //!< script file

        FILE * i_stream = NULL;

        //!< handle command line parameters
        while((c = getopt(argc,argv,STR_PARAM)) != -1){
                switch(c){
                case 'f':
                        //!< interpret brainfuck script
                        if(access(optarg, F_OK) != -1){
                                mode = MODE_SCRIPT;
                                file = optarg;
                        }else{
                                fprintf(stderr,"Err:unexist file %s!\n",optarg);
                                abort();
                        }
                        break;
                case '?':
                        if(optopt == 'f')
                                fprintf(stderr,"Option -%c requires an argument.\n",optopt);
                        else if(isprint(optopt))
                                fprintf(stderr,"Unkown option `-%c`.\n",optopt);
                        else
                                fprintf(stderr,"Unkown option character `\\x%x`.\n",optopt);
                        break;
                default:
                        abort();
                        break;
                }
        }       

        //!< select script input file to `i_stream`
        if(MODE_SCRIPT == mode){   
                i_stream = fopen(file,"r");
                
                if(NULL == i_stream){
                        fprintf(stderr,"Err:null file.\n");
                        abort();
                }

        }else if(MODE_INTERACTIVE == mode){
                i_stream = stdin;
                if(NULL == i_stream){
                        fprintf(stderr,"Err:null stdin.\n");
                        abort();
                }
        }else{
                fprintf(stderr,"Unkown mode!\n");
                abort();
        }

        //!< handle brainfuck 
        abi_f(i_stream);

        //!< close file 
        fclose(i_stream);

        printf("ok!\n");
        return 0;
}

/* ******************** brainfuck interpreter ******************** */


/*  \brief append a byte to buffer
 *  \param buf buffer to append
 *  \param max max size of buffer
 *  \param byte byte appended to buffer
 *  \retval 0 for ok,-1 for fail
 * */
/*
static void append(struct buffer buf, size_t max, uint8_t byte){
        if(buf.len_valid < max){  //!< buf.buf[max_length] == '\0'
                buf.buf[buf.len_valid] = byte;
                buf.len_valid++;
        }else{
                fprintf(stderr,"Out of buffer in `%s`.\n",__FUNCTION__);
                abort();
        }
}
*/

