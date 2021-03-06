#ifndef _ABI_BF_H_
#define _ABI_BF_H_

#include <stddef.h>
#include <stdint.h>

/*! \brief implement brainfuck interpreter by binary AST
 * */

///! \defgroup abi_bf_prototype
/// @{

/*! \brief brainfuck valid token definition*/
#define BF_TOKEN_MEM_ITEM_INC '+'
#define BF_TOKEN_MEM_ITEM_DEC '-'
#define BF_TOKEN_MEM_PTR_INC '>'
#define BF_TOKEN_MEM_PTR_DEC '<'
#define BF_TOKEN_MEM_ITEM_OUTPUT '.'
#define BF_TOKEN_MEM_ITEM_INPUT ','
#define BF_TOKEN_CTL_LOOP_START '['
#define BF_TOKEN_CTL_LOOP_END ']'

#define BF_TOKEN_PLACEHOLDER 'x'

/*! \brief brainfuck instruction
 *
 * */
typedef struct bf_instruction {
    int count;   //!< count of current instruction
    char token;  //!< type of instruction
} bf_instruction_t;

/*! \brief brainfuck AST node
 * */
typedef struct bf_ast_node {
    bf_instruction_t* instruction;  //!< instruction of current AST node
    struct bf_ast_node* loop;       //!< loop child
    struct bf_ast_node* next;       //!< normal child
} bf_ast_node_t;

/*! \brief brainfuck runtime context
 *
 * */
typedef struct bf_context {
    uint8_t* mem_ptr;  //!< pointer to memory start
    size_t mem_size;   //!< size of memory
    size_t mem_index;  //!< index of current memory
} bf_context_t;

typedef void (*bf_ast_instruction_interpreter_t)(bf_context_t* context,
                                                 bf_instruction_t* instruction);

/*! \brief brainfuck AST */
typedef struct bf_ast {
    //!< root of AST
    bf_ast_node_t* root;
    //!< interpreter for each instruction
    bf_ast_instruction_interpreter_t interpreter;
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
bf_instruction_t* bf_instruction_new(char token, int count);

/*! \brief release brainfuck instruction
 * */
void bf_instruction_release(bf_instruction_t* instruction);

/*! \brief create a brainfuck AST node
 *  \param instruction the instruction of AST node
 *  \retval created brainfuck AST node
 * */
bf_ast_node_t* bf_ast_node_new(bf_instruction_t* instruction);

/*! \brief release the brainfuck AST node
 * */
void bf_ast_node_release(bf_ast_node_t* node);

/*! \brief create a brainfuck AST
 *  \param interpreter interpreter of each brainfuck instruction
 *  \retval created brainfuck AST
 * */
bf_ast_t* bf_ast_new(bf_ast_instruction_interpreter_t interpreter);

/*! \brief release a brainfuck AST
 *  \param AST to release
 * */
void bf_ast_release(bf_ast_t* ast);

/*! \brief traversal AST by pre-order
 *  \param ast instance of AST
 *  \param final loop depth
 * */
void bf_ast_dfs_pre(bf_ast_t* ast);

/*! \brief get loop depth of AST
 *  \param ast instance of AST
 *  \retval -1 for NULL AST, other for loop depth
 * */
int bf_ast_loop_depth(bf_ast_t* ast);

/*! \brief check is a AST can be execute
 *  \param ast instance of AST
 *  \retval bool, true for executable , false for not
 * */
#define bf_ast_executable(ast) \
    ((NULL == ast) ? (0) : (0 == bf_ast_loop_depth(ast)))

/*! \brief initialize brainfuck AST from script file
 *  \param ast brainfuck AST instance
 *  \param file brainfuck script file name
 *  \param root of AST
 * */
void bf_ast_init_4_script(bf_ast_t* ast, const char* script);

/*! \brief initialize brainfuck AST from instruction tokens sting
 *  \parma ast brainfuck AST instance
 *  \param source source code sting of brainfuck
 * */
void bf_ast_init_4_string(bf_ast_t* ast, const char* source);

// bf_ast_node_t * bf_ast_init_4_stream(FILE * stream);
//
//
/*static*/ bf_ast_node_t* bf_ast_tail(bf_ast_node_t* root);

/*! \brief create a context of brainfuck interpreter runtime
 *  \param mem_size size of brianfuck interpreter
 *  \param context of a interpreter
 * */
bf_context_t* bf_context_new(size_t mem_size);

/*! \brief release a context
 *  \parma context to release
 * */
void bf_context_release(bf_context_t* context);

/*! \brief execute brainfuck AST in specify context
 *  \param context current runtime context
 *  \param ast brainfuck AST to execute
 * */
void bf_execute(bf_context_t* context, bf_ast_t* ast);

/*! \brief instruction interpreter for non-control instruction
 *  \param context context of instruction
 *  \param instruction instruction to interprete
 * */
void bf_instruction_interpreter(bf_context_t* context,
                                bf_instruction_t* instruction);

/// @}

#endif  //!< _ABI_BF_H_
