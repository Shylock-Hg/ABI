/*! \brief uniform interface to read source code token from FILE or string
 *  \author Shylock Hg
 *  \date 2018-05-25
 *  \email tcath2s@gmail.com
 * */

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
bf_tokens_t * bf_ast_tokens_new(const char * source){
	assert(NULL != source);
	if(NULL == source)
		return NULL;

	bf_tokens_t * tokens = malloc(sizeof(bf_tokens_t));
	assert(NULL != tokens);
	if(NULL == source)
		return NULL;

	tokens->tokens = calloc(sizeof(char), strlen(source)+1);
	assert(NULL != tokens->tokens);
	if(NULL == tokens->tokens){
		free(tokens);
		return NULL;
	}
	strcpy(tokens->tokens, source);
	tokens->index = 0;

	return tokens;
}

/*! \brief release instance of AST tokens
 *  \param AST tokens instance
 * */
void bf_ast_tokens_release(bf_tokens_t * tokens){
	assert(NULL != tokens);
	if(NULL == tokens)
		return;

	free(tokens->tokens);
	free(tokens);
}

/*! \brief get token from source
 *  \param stream stdio.h steam
 *  \param tokens tokens of source code
 *  \retval token got, EOF for tail
 * */
int bf_ast_getc_4_source(FILE * stream, bf_tokens_t * tokens){
	//assert(NULL == stream && NULL == tokens);
	//assert(NULL != stream && NULL != tokens);
	if(NULL == stream && NULL == tokens)
		return EOF;
	if(NULL != stream && NULL != tokens)
		return EOF;

	if(NULL != stream)
		return fgetc(stream);

	if(NULL != tokens){
		if(NULL != tokens->tokens){
			if('\0' == tokens->tokens[tokens->index])
				return EOF;
			else {
				//fprintf(stderr, "get `%c` \n", tokens->tokens[tokens->index]);
				return tokens->tokens[(tokens->index)++];
			}
		}
	}

	return EOF;
}

/*! \brief unget token to source 
 *  \param stream stdio.h stream
 *  \parma tokens tokens of source code
 *  \retval token got, EOF for tail
 * */
int bf_ast_ungetc_2_source(char c, FILE * stream, bf_tokens_t * tokens){
	//assert(NULL == stream && NULL == tokens);
	//assert(NULL != stream && NULL != tokens);
	if(NULL == stream && NULL == tokens)
		return EOF;
	if(NULL != stream && NULL != tokens)
		return EOF;

	if(NULL != stream){
		return ungetc(c, stream);
	}

	if(NULL != tokens){
		if(NULL != tokens->tokens){
			if(0 < tokens->index){
				//fprintf(stderr, "unget `%c` \n", tokens->tokens[tokens->index-1]);
				tokens->index --;
			}
		}
	}

	return EOF;
}

/// @}

