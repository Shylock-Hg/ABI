//#define NDEBUG

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <abi/abi_bf.h>
#include <abi/internal/abi_tokens.h>

/*! \brief create a instruction
 *  \param token token character
 *  \param count count of token countinue
 *  \retval created brainfuck instruction
 * */
bf_instruction_t* bf_instruction_new(char token, int count) {
    bf_instruction_t* instruction = malloc(sizeof(bf_instruction_t));
    assert(NULL != instruction);
    if (NULL == instruction) return NULL;

    instruction->count = count;
    instruction->token = token;

    return instruction;
}

/*! \brief release brainfuck instruction
 * */
void bf_instruction_release(bf_instruction_t* instruction) {
    free(instruction);
}

/*! \brief create a brainfuck AST node
 *  \param instruction the instruction of AST node
 *  \retval created brainfuck AST node
 * */
bf_ast_node_t* bf_ast_node_new(bf_instruction_t* instruction) {
    if (NULL == instruction) {
        return NULL;
    }
    bf_ast_node_t* node = malloc(sizeof(bf_ast_node_t));
    assert(NULL != instruction && NULL != node);
    if (NULL == node) return NULL;

    node->instruction = instruction;
    node->loop = NULL;
    node->next = NULL;

    return node;
}

/*! \brief release the brainfuck AST node
 * */
void bf_ast_node_release(bf_ast_node_t* node) {
    free(node->instruction);
    free(node);
}

/*! \brief create a brainfuck AST
 *  \param interpreter interpreter of each brainfuck instruction
 *  \retval created brainfuck AST
 * */
bf_ast_t* bf_ast_new(bf_ast_instruction_interpreter_t interpreter) {
    assert(NULL != interpreter);
    if (NULL == interpreter) {
        return NULL;
    }
    bf_ast_t* ast = malloc(sizeof(bf_ast_t));
    assert(NULL != ast);
    if (NULL == ast) return NULL;

    ast->interpreter = interpreter;
    ast->root = NULL;

    return ast;
}

static void bf_ast_release_post(bf_ast_node_t* root) {
    // assert(NULL != root);
    if (NULL == root) return;

    bf_ast_release_post(root->loop);
    bf_ast_release_post(root->next);

    bf_instruction_release(root->instruction);
    free(root);
}

/*! \brief release a brainfuck AST
 *  \param AST to release
 * */
void bf_ast_release(bf_ast_t* ast) {
    assert(NULL != ast);
    if (NULL == ast) return;

    //!< release tree
    bf_ast_release_post(ast->root);

    //!< release handle
    free(ast);
}

/*! \brief add node to a brainfuck AST by token
 *  \param ast instance of brainfuck AST
 *  \param token token of brainfuck instruction
 * */
// void bf_ast_node_add(bf_ast_t * ast, char token);

/*! \brief initialize brainfuck AST from script file
 *  \param ast brainfuck AST instance
 *  \param file brainfuck script file name
 * */
static bf_ast_node_t* bf_ast_init_4_source(FILE* stream, bf_tokens_t* tokens) {
    // assert(NULL == stream && NULL == tokens);
    // assert(NULL != stream && NULL != tokens);
    if (NULL == stream && NULL == tokens) return NULL;
    if (NULL != stream && NULL != tokens) return NULL;

    char c = 0;
    bf_ast_node_t* root =
        bf_ast_node_new(bf_instruction_new(BF_TOKEN_PLACEHOLDER, 0));
    bf_ast_node_t* node = root;
    bf_ast_node_t* pre = NULL;

    while ((c = bf_ast_getc_4_source(stream, tokens)) != EOF) {
        switch (c) {
            case BF_TOKEN_MEM_ITEM_INC:
                do {
                    node->instruction->token = BF_TOKEN_MEM_ITEM_INC;
                    node->instruction->count++;
                    // fprintf(stderr, "build ast from `%c`\n",
                    // node->instruction->token);
                } while ((c = bf_ast_getc_4_source(stream, tokens)) ==
                         BF_TOKEN_MEM_ITEM_INC);
                bf_ast_ungetc_2_source(c, stream, tokens);
                break;
            case BF_TOKEN_MEM_ITEM_DEC:
                do {
                    node->instruction->token = BF_TOKEN_MEM_ITEM_DEC;
                    node->instruction->count++;

                    // fprintf(stderr, "build ast from `%c`\n",
                    // node->instruction->token);
                } while ((c = bf_ast_getc_4_source(stream, tokens)) ==
                         BF_TOKEN_MEM_ITEM_DEC);
                bf_ast_ungetc_2_source(c, stream, tokens);
                break;
            case BF_TOKEN_MEM_PTR_INC:
                do {
                    node->instruction->token = BF_TOKEN_MEM_PTR_INC;
                    node->instruction->count++;

                    // fprintf(stderr, "build ast from `%c`\n",
                    // node->instruction->token);
                } while ((c = bf_ast_getc_4_source(stream, tokens)) ==
                         BF_TOKEN_MEM_PTR_INC);
                bf_ast_ungetc_2_source(c, stream, tokens);
                break;
            case BF_TOKEN_MEM_PTR_DEC:
                do {
                    node->instruction->token = BF_TOKEN_MEM_PTR_DEC;
                    node->instruction->count++;

                    // fprintf(stderr, "build ast from `%c`\n",
                    // node->instruction->token);
                } while ((c = bf_ast_getc_4_source(stream, tokens)) ==
                         BF_TOKEN_MEM_PTR_DEC);
                bf_ast_ungetc_2_source(c, stream, tokens);
                break;
            case BF_TOKEN_MEM_ITEM_OUTPUT:
                do {
                    node->instruction->token = BF_TOKEN_MEM_ITEM_OUTPUT;
                    node->instruction->count++;

                    // fprintf(stderr, "build ast from `%c`\n",
                    // node->instruction->token);
                } while ((c = bf_ast_getc_4_source(stream, tokens)) ==
                         BF_TOKEN_MEM_ITEM_OUTPUT);
                bf_ast_ungetc_2_source(c, stream, tokens);
                break;
            case BF_TOKEN_MEM_ITEM_INPUT:
                do {
                    node->instruction->token = BF_TOKEN_MEM_ITEM_INPUT;
                    node->instruction->count++;

                    // fprintf(stderr, "build ast from `%c`\n",
                    // node->instruction->token);
                } while ((c = bf_ast_getc_4_source(stream, tokens)) ==
                         BF_TOKEN_MEM_ITEM_INPUT);
                bf_ast_ungetc_2_source(c, stream, tokens);
                break;
            case BF_TOKEN_CTL_LOOP_START:
                node->instruction->token = BF_TOKEN_CTL_LOOP_START;
                node->instruction->count = 1;
                // fprintf(stderr, "build ast from `%c`\n",
                // node->instruction->token); < build loop branch
                node->loop = bf_ast_init_4_source(stream, tokens);
                break;
            case BF_TOKEN_CTL_LOOP_END:
                //< a ast node with invalid instruction
                //< and without children
                node->instruction->token = BF_TOKEN_CTL_LOOP_END;
                node->instruction->count = 1;
                // fprintf(stderr, "build ast from `%c`\n",
                // node->instruction->token); node->next = NULL; default
                // node->loop = NULL;
                return root;
                break;
            default:
                continue;
                // fprintf(stderr, "useless token `%c`\n",c);
        }  //!< end of switch(token)

        //< initialize and scroll to new node
        node->next =
            bf_ast_node_new(bf_instruction_new(BF_TOKEN_PLACEHOLDER, 0));
        pre = node;
        node = node->next;

    }  //!< end of while(! EOF)

    if (BF_TOKEN_PLACEHOLDER == node->instruction->token) {
        bf_ast_node_release(node);
        if (NULL != pre)
            pre->next = NULL;
        else
            root = NULL;
    }

    return root;
}

/*! \brief return pointer to tail node of AST in preorder dfs
 *  \param root node of AST
 *  \retval pointer to tail node of AST
 * */
/*static*/ bf_ast_node_t* bf_ast_tail(bf_ast_node_t* root) {
    // assert(NULL != root);
    if (NULL == root) return NULL;

    bf_ast_node_t* loop = bf_ast_tail(root->loop);
    bf_ast_node_t* next = bf_ast_tail(root->next);

    if (NULL != next) {
        return next;
    }

    if (NULL != loop) {
        return loop;
    }

    return root;
}

void bf_ast_init_4_script(bf_ast_t* ast, const char* script) {
    assert(NULL != ast && NULL != script);
    if (NULL == ast || NULL == script) return;

    FILE* stream = fopen(script, "r");
    assert(NULL != stream);

    if (NULL != stream) {
        ast->root = bf_ast_init_4_source(stream, NULL);
        fclose(stream);
    }
}

void bf_ast_init_4_string(bf_ast_t* ast, const char* source) {
    assert(NULL != ast && NULL != source);
    if (NULL == ast || NULL == source) return;

    bf_tokens_t* tokens = bf_ast_tokens_new(source);

    if (NULL != tokens) {
        if (NULL == ast->root) {
            ast->root = bf_ast_init_4_source(NULL, tokens);
            while (bf_ast_tokens_is_available(tokens)) {
                bf_ast_node_t* tail = bf_ast_tail(ast->root);
                if (BF_TOKEN_CTL_LOOP_START == tail->instruction->token)
                    tail->loop = bf_ast_init_4_source(NULL, tokens);
                else
                    tail->next = bf_ast_init_4_source(NULL, tokens);
            }
        } else {
            while (bf_ast_tokens_is_available(tokens)) {
                bf_ast_node_t* tail = bf_ast_tail(ast->root);
                if (BF_TOKEN_CTL_LOOP_START == tail->instruction->token)
                    tail->loop = bf_ast_init_4_source(NULL, tokens);
                else
                    tail->next = bf_ast_init_4_source(NULL, tokens);
            }
        }
        bf_ast_tokens_release(tokens);
    }
}

/*! \brief create a context of brainfuck interpreter runtime
 *  \param mem_size size of brianfuck interpreter
 *  \param context of a interpreter
 * */
bf_context_t* bf_context_new(size_t mem_size) {
    uint8_t* mem = malloc(mem_size);
    if (NULL == mem) {
        return NULL;
    }
    memset(mem, 0x00, mem_size);
    bf_context_t* context = malloc(sizeof(bf_context_t));
    assert(NULL != mem && NULL != context);
    if (NULL == context) {
        free(mem);
        return NULL;
    }

    context->mem_ptr = mem;
    context->mem_size = mem_size;
    context->mem_index = 0;

    return context;
}

/*! \brief release a context
 *  \parma context to release
 * */
void bf_context_release(bf_context_t* context) {
    assert(NULL != context);
    if (NULL == context) return;

    free(context->mem_ptr);
    free(context);
}

static void _bf_ast_dfs_pre(bf_ast_node_t* root) {
    if (NULL == root) {
        return;
    }

    // fprintf(stderr, "token-`%c`, count-`%d`.\n", root->instruction->token,
    // root->instruction->count);
    for (int i = 0; i < root->instruction->count; i++)
        fputc(root->instruction->token, stderr);

    if (BF_TOKEN_PLACEHOLDER == root->instruction->token)
        fputc(root->instruction->token, stderr);

    _bf_ast_dfs_pre(root->loop);

    _bf_ast_dfs_pre(root->next);
}

void bf_ast_dfs_pre(bf_ast_t* ast) {
    assert(NULL != ast);
    if (NULL == ast) return;

    _bf_ast_dfs_pre(ast->root);
    fputc('\n', stderr);
}

static int _bf_ast_loop_depth(bf_ast_node_t* root, int loop_depth) {
    if (NULL == root) {
        return loop_depth;
    }

    if (BF_TOKEN_CTL_LOOP_END == root->instruction->token) loop_depth--;

    if (BF_TOKEN_CTL_LOOP_START == root->instruction->token) loop_depth++;

    if (NULL != root->loop)
        loop_depth = _bf_ast_loop_depth(root->loop, loop_depth);
    return _bf_ast_loop_depth(root->next, loop_depth);
}

int bf_ast_loop_depth(bf_ast_t* ast) {
    assert(NULL != ast);
    if (NULL == ast) return -1;

    assert(NULL != ast->root);
    if (NULL == ast->root) return -1;

    return _bf_ast_loop_depth(ast->root, 0);
}

/*! \brief execute brainfuck AST by variant pre-order traversal in specify
 * context \param context current runtime context \param ast brainfuck AST to
 * execute
 * */
static void bf_ast_execute(bf_context_t* context, bf_ast_t* ast,
                           bf_ast_node_t* root) {
    assert(NULL != context && NULL != ast && NULL != ast->interpreter);
    if (NULL == context || NULL == ast || NULL == ast->interpreter) return;

    //< execute instruction
    if (NULL == root) return;

    //< execute instruction(excipt control instruction)
    ast->interpreter(context, root->instruction);
#ifndef NDEBUG
    //< trace instruction stream
    // fputc(root->instruction->token, stderr);
#endif

    //< pre-order traversal
    //< loop until point to 0
    if (BF_TOKEN_CTL_LOOP_START == root->instruction->token) {
        while (0 < context->mem_ptr[context->mem_index]) {
            bf_ast_execute(context, ast, root->loop);
        }
    }
    bf_ast_execute(context, ast, root->next);
}

/*! \brief execute brainfuck AST by variant pre-order traversal in specify
 * context \param context current runtime context \param ast brainfuck AST to
 * execute
 * */
void bf_execute(bf_context_t* context, bf_ast_t* ast) {
    assert(NULL != context && NULL != ast);
    if (NULL == context || NULL == ast) return;

    bf_ast_execute(context, ast, ast->root);
}

void bf_instruction_interpreter(bf_context_t* context,
                                bf_instruction_t* instruction) {
    assert(NULL != context && NULL != instruction);
    if (NULL == context || NULL == instruction) return;

    // fputc(root->instruction->token, stderr);
    switch (instruction->token) {
        case BF_TOKEN_MEM_ITEM_INC:
            if (context->mem_index < context->mem_size) {
                context->mem_ptr[context->mem_index] += instruction->count;
            } else {
                fprintf(stderr, "Error:Invalid memory access!\n");
            }
            break;
        case BF_TOKEN_MEM_ITEM_DEC:
            if (context->mem_index < context->mem_size) {
                context->mem_ptr[context->mem_index] -= instruction->count;
            } else {
                fprintf(stderr, "Error:Invalid memory access!\n");
            }
            break;
        case BF_TOKEN_MEM_PTR_INC:
            if (context->mem_size > (context->mem_index + instruction->count)) {
                context->mem_index += instruction->count;
            } else {
                fprintf(stderr, "Error:Invalid memory access!\n");
            }
            break;
        case BF_TOKEN_MEM_PTR_DEC:
            if (0 <= (context->mem_index - instruction->count)) {
                context->mem_index -= instruction->count;
            } else {
                fprintf(stderr, "Error:Invalid memory access!\n");
            }
            break;
        case BF_TOKEN_MEM_ITEM_OUTPUT:
            if (context->mem_index < context->mem_size) {
                for (int i = 0; i < instruction->count; i++) {
                    putchar(context->mem_ptr[context->mem_index]);
                }
            } else {
                fprintf(stderr, "Error:Invalid memory access!\n");
            }
            break;
        case BF_TOKEN_MEM_ITEM_INPUT:
            if (context->mem_index < context->mem_size) {
                for (int i = 0; i < instruction->count; i++) {
                    context->mem_ptr[context->mem_index] = getchar();
                }
            } else {
                fprintf(stderr, "Error:Invalid memory access!\n");
            }
            break;
        //< only execute non control instruction
        case BF_TOKEN_CTL_LOOP_START:
            break;
        case BF_TOKEN_CTL_LOOP_END:
            break;
            // default:
            // fprintf(stderr, "useless instruction->token
            // `%c`\n",instruction->token);
    }
}

/// @}
