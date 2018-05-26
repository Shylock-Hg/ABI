/*! \brief interface of getc&ungetc from source
 *  \author Shylock Hg
 *  \date 2018-05-25
 *  \email tcath2s@gmail.com
 * */

#ifndef _ABI_TOKENS_H_
#define _ABI_TOKENS_H_

#ifdef __cplusplus
	extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>

///! \defgroup bf_ast_tokens_prototype
/// @{

typedef struct {
	char * tokens;  //!< instruction tokens string
	size_t index;  //!< index of current token
} bf_tokens_t;
/// @}

///! \defgroup bf_ast_tokens_interface
/// @{

/*! \brief create AST tokens from source string
 *  \param source source code string
 *  \retval tokens instance
 * */
bf_tokens_t * bf_ast_tokens_new(const char * source);

/*! \brief release instance of AST tokens
 *  \param AST tokens instance
 * */
void bf_ast_tokens_release(bf_tokens_t * tokens);

/*! \brief check the tokens is available
 *  \param tokens tokens of brainfuck source code
 *  \retval true for available , false for not
 * */
bool bf_ast_tokens_is_available(bf_tokens_t * tokens);

/*! \brief get token from source
 *  \param stream stdio.h steam
 *  \param tokens tokens of source code
 *  \retval token got, -1 for tail
 *  \note either stream or tokens must be NULL
 * */
int bf_ast_getc_4_source(FILE * stream, bf_tokens_t * tokens);

/*! \brief unget token to source 
 *  \param c character push back to source
 *  \param stream stdio.h stream
 *  \parma tokens tokens of source code
 *  \retval token got, -1 for tail
 *  \note either stream or tokens must be NULL
 * */
int bf_ast_ungetc_2_source(char c, FILE * stream, bf_tokens_t * tokens);


/// @}

#ifdef __cplusplus
	}
#endif

#endif  //!< _ABI_TOKENS_H_

