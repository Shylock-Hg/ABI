#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <unistd.h>
#include <getopt.h>

#include "include/abi_bf.h"


//!< command line parameter
#define STR_PARAM  "f:"

#define ABI_MEM_SIZE 30000

#define MAX_LINE_BUF 1024

int main(int argc, char * argv[]){

	int c = 0;  //!< opt character
	/*  \brief mode flag
	 *  \enum MODE_SCRIPT interpret a brainfuck script file
	 *  \enum MODE_INTERACTIVE run the brainfuck interpreter in interactive mode 
	 * */
	enum {MODE_SCRIPT,MODE_INTERACTIVE} mode = MODE_INTERACTIVE;

	char * file = NULL;  //!< script file

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

		bf_ast_t * ast = bf_ast_new(bf_instruction_interpreter);
		bf_ast_init_4_script(ast, file);

		//bf_ast_dfs_pre(ast);

		bf_context_t * context = bf_context_new(ABI_MEM_SIZE);

		//bf_ast_node_t * tail = bf_ast_tail(ast->root);
		/*
		if(NULL != tail)
			fprintf(stderr, "tail->ins->token == `%c`\n", 
					tail->instruction->token);
		*/
		
		if(bf_ast_executable(ast))
			bf_execute(context, ast);

		bf_context_release(context);
		bf_ast_release(ast);
	}else if(MODE_INTERACTIVE == mode){

		char buf[MAX_LINE_BUF] = {0};
		
		bf_ast_t * ast = bf_ast_new(bf_instruction_interpreter);
		bf_context_t * context = bf_context_new(ABI_MEM_SIZE);

		while(1){
			printf(">>>");
			fgets(buf, sizeof(buf), stdin);

			bf_ast_init_4_string(ast, buf);

			//fprintf(stderr, "loop-depth == `%d`\n", bf_ast_loop_depth(ast));

			//bf_ast_dfs_pre(ast);

			if(bf_ast_executable(ast)){
				bf_execute(context, ast);

				bf_ast_release(ast);
				ast = bf_ast_new(bf_instruction_interpreter);
			}
		}
	}else{
		fprintf(stderr,"Unkown mode!\n");
		abort();
	}

	//!< handle brainfuck 
	//abi_f(i_stream);

	//!< close file 
	//fclose(i_stream);

	//printf("ok!\n");
	return 0;
}

