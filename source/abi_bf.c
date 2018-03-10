#include "stdio.h"
#include "assert.h"
#include "stdlib.h"

/******************** inner-memory[data,instruction] of brainfuck interpreter ********************/
#define MAX_MEM_BUF_LEN  30000
#define MAX_INS_BUF_LEN  30000

struct buffer {
        char * buf;
        size_t len_valid;  //!< length of data cached
};

//!< brainfuck interpreter memory
static char mem[MAX_MEM_BUF_LEN] = {0};
static struct buffer mem_buffer = {mem+0,0};
//!< brainfuck memory pointer
static char * mem_pointer = mem+0;


//!< buffer for brainfuck script cache
static char instruction[MAX_INS_BUF_LEN] = {0};
static struct buffer ins_buffer = {instruction+0,0};
//!< brainfuck script stream pointer
static char * ins_pointer = instruction+0;

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

/*  \brief find the matching '[' 
 *  \param buf buffer to find
 *  \param max max size of buf
 *  \retval -1 for not find
 *          0 for ok
 * */
static int jump_match_left(struct buffer buf){
        int right = 1;  //!< count of ']'
        do{
                if(']' == *ins_pointer){
                        right++;
                }else if('[' == *ins_pointer){
                        right--;
                }
                if(0 == right){
                        //!< next instruction
                        return 0;
                }
        }
        while(ins_pointer-- != buf.buf+0);

        //!< not found
        return -1;
}

/*  \brief find the matching ']' 
 *  \param buf buffer to find
 *  \param max max size of buf
 *  \retval -1 for not find
 *          0 for ok
 * */
static int jump_match_right(struct buffer buf){
        int left = 1;  //!< count of '['
        while(++ins_pointer != buf.buf+buf.len_valid){
                if('[' == *ins_pointer){
                        left++;
                }else if(']' == *ins_pointer){
                        left--;
                }
                if(0 == left){
                        //!< next instruction
                        return 0;
                }
        }

        //!< not found
        return -1;
}

//!< brainfuck all operator
/*
static char bf_instruction[] =  {
        '>',  //!< increment the byte pointer
        '<',  //!< decrement the byte pointer
        '+',  //!< increment the pointed byte
        '-',  //!< increment the pointed byte
        '.',  //!< output the pointed byte
        ',',  //!< accept one byte input to pointed byte
        '[',  //!< if the pointed byte equal to zero,then jump forward to ']'
        ']',  //!< if the pointed byte not equal to zero,then jump back to '['
};
*/

/*  \brief handle a operiton of brainfuck
 * */
static void abi_op(void){

        switch(*ins_pointer){
                case '>':
                        if(mem+sizeof(mem) == mem_pointer){
                                //!< outof memory
                                fprintf(stderr,"Err:Increment operation out of memory!\n");
                                abort();
                        }else{
                                //printf("Inc instruction pointer.\n");
                                mem_pointer++;
                        }
                        break;

                case '<':
                        if(mem+0 == mem_pointer){
                                //!< outof memory
                                fprintf(stderr,"Err:Decrement operation out of memory!\n");
                                abort();
                        }else{
                                mem_pointer--;
                        }
                        break;

                case '+':
                        (*mem_pointer)++;
                        break;

                case '_':
                        (*mem_pointer)--;
                        break;

                case '.':
                        putchar(*mem_pointer);
                        break;

                case ',':
                        if(ins_buffer.buf+ins_buffer.len_valid-1 != ins_pointer){
                                ins_pointer++;
                                *mem_pointer = *ins_pointer;
                        }
                        break;

                case '[':
                        if(0 == *mem_pointer){
                                //!< jump forward to matching ']' in ins_buffer
                                if(-1 == jump_match_right(ins_buffer)){
                                        fprintf(stderr,"Err:find matching `[` fail!\n");
                                }
                                printf("[info]:forward-`%c`.\n",*ins_pointer);
                        }else{
                                //printf("[info]:`[` not jump.\n");
                        }
                        break;

                case ']':
                        if(0 == *mem_pointer){
                                //!< next instruction
                                printf("[info]:`]` not jump.\n");
                        }else{
                                printf("[info]:instruction offset==%d.\n",ins_pointer-ins_buffer.buf);
                                //!< jump back to matching '[' in ins_buffer
                                if(-1 == jump_match_left(ins_buffer)){
                                        fprintf(stderr,"Err:find matching `[` fail!\n");
                                }
                                //printf("[info]:back-`%c`,offset==`%d`.\n",*ins_pointer,ins_pointer-ins_buffer.buf);
                                //printf("[info]:instruction offset==%d.\n",ins_pointer-ins_buffer.buf);
                                //printf("[info]:mem_pointer==%d.\n",*mem_pointer);
                        }
                        break;

                deafault:
                        //fprintf(stderr,"Err:Invalid instruction `%c`!\n",*ins_pointer);
                        break;
        }       
}

/*  \brief another brainfuck interpreter to script file
 *  \param
 * */
void abi_f(FILE * f){
        /*
        char c = 0;
        while(EOF != (c = fgetc(f))){
                //!< append instruction
                append(ins_buffer,MAX_INS_BUF_LEN,c);
                //!< handle instruction
                abi_op(c, f);
        }
        */

        size_t len = fread(ins_buffer.buf,sizeof(char),sizeof(instruction)-1,f);
        if(0 == len){
                fprintf(stderr,"Err:empty source file.\n");
        }else{
                ins_buffer.buf[len] = '\0';
                ins_buffer.len_valid = len;  //!< count of instructions
                //printf("%s\n",ins_buffer.buf);

                //!< handle instructions in `ins_buffer`
                do{
                        printf("[info]:op==`%c`,offset==`%d`.\n",*ins_pointer,ins_pointer-ins_buffer.buf);
                        abi_op();
                }while(++ins_pointer != ins_buffer.buf+ins_buffer.len_valid);
        }
}

