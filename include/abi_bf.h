#ifndef _ABI_BF_H_
#define _ABI_BF_H_

#include <stdint.h>

/*! \brief implement brainfuck interpreter by binary AST
 * */

///! \defgroup abi_bf_prototype
/// @{

/*! \brief brainfuck valid token definition*/
#define BF_TOKEN_MEM_ITEM_INC    '+'
#define BF_TOKEN_MEM_ITEM_DEC    '-'
#define BF_TOKEN_MEM_PTR_INC     '>'
#define BF_TOKEN_MEM_PTR_DEC     '<'
#define BF_TOKEN_MEM_ITEM_OUTPUT '.'
#define BF_TOKEN_MEM_ITEM_INPUT  ','
#define BF_TOKEN_CTL_LOOP_START  '['
#define BF_TOKEN_CTL_LOOP_END    ']'


/*! \brief brainfuck instruction
 *
 * */
typedef struct bf_instruction {
	int count;  //!< count of current instruction	
	char token;  //!< type of instruction
} bf_instruction_t;

/*! \brief brainfuck AST node
 * */
typedef struct bf_ast_node {
	bf_instruction_t * instruction;  //!< instruction of current AST node
	struct bf_ast_node * loop;  //!< loop child
	struct bf_ast_node * next;  //!< normal child
} bf_ast_node_t;

/*! \brief brainfuck runtime context
 *
 * */
typedef struct bf_context {
	uint8_t * mem_ptr;  //!< pointer to memory start
	size_t    mem_size;  //!< size of memory
	size_t    mem_index;  //!< index of current memory
} bf_context_t;

typedef void (*bf_ast_instruction_interpreter_t)(bf_context_t * context, bf_instruction_t * instruction);

/*! \brief brainfuck AST */
typedef struct bf_ast {
	bf_ast_node_t * root;  //!< root of AST 
	bf_ast_instruction_interpreter_t interpreter;  //!< interpreter for each instruction
} bf_ast_t;


/// @}
//
//
///! \defgroup abi_bf_interface
/// @{

/*! \brief create a instruction
 *  \param token token character
 *  \param count count of token countinue
 *  \retval created brainfuck instruction
 * */
bf_instruction_t * bf_instruction_new(char token, int count);

/*! \brief release brainfuck instruction
 * */
void bf_instruction_release(bf_instruction_t * instruction);

/*! \brief create a brainfuck AST node
 *  \param instruction the instruction of AST node
 *  \retval created brainfuck AST node
 * */
bf_ast_node_t * bf_ast_node_new(bf_instruction_t * instruction);

/*! \brief release the brainfuck AST node
 * */
void bf_ast_node_release(bf_ast_node_t * node);

/*! \brief create a brainfuck AST 
 *  \param interpreter interpreter of each brainfuck instruction
 *  \retval created brainfuck AST
 * */
bf_ast_t * bf_ast_new(bf_ast_instruction_interpreter_t interpreter);

/*! \brief release a brainfuck AST
 *  \param AST to release
 * */
void bf_ast_release(bf_ast_t * ast);

/*! \brief add node to a brainfuck AST by token
 *  \param ast instance of brainfuck AST
 *  \param token token of brainfuck instruction
 * */
//void bf_ast_node_add(bf_ast_t * ast, char token);
void bf_ast_dfs(bf_ast_t * ast);

/*! \brief initialize brainfuck AST from script file
 *  \param ast brainfuck AST instance
 *  \param file brainfuck script file name
 *  \param root of AST 
 * */
void bf_ast_init_4_script(bf_ast_t * ast, const char * script);
//bf_ast_node_t * bf_ast_init_4_stream(FILE * stream);

/*! \brief create a context of brainfuck interpreter runtime
 *  \param mem_size size of brianfuck interpreter 
 *  \param context of a interpreter
 * */
bf_context_t * bf_context_new(size_t mem_size);

/*! \brief release a context
 *  \parma context to release
 * */
void bf_context_release(bf_context_t * context);

/*! \brief execute brainfuck AST in specify context
 *  \param context current runtime context 
 *  \param ast brainfuck AST to execute
 * */
void bf_execute(bf_context_t * context, bf_ast_t * ast);

/*! \brief instruction interpreter for non-control instruction
 *  \param context context of instruction
 *  \param instruction instruction to interprete
 * */
void bf_instruction_interpreter(bf_context_t * context, bf_instruction_t * instruction);

/// @}

#endif

