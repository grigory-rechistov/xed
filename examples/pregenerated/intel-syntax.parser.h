/* A Bison parser, made by GNU Bison 2.7.12-4996.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_EXAMPLES_PREGENERATED_INTEL_SYNTAX_PARSER_H_INCLUDED
# define YY_YY_EXAMPLES_PREGENERATED_INTEL_SYNTAX_PARSER_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
/* Line 2053 of yacc.c  */
#line 46 "./examples/intel-syntax.y"

#include <stdbool.h>
#include "xed-encode.h"
#include "parse-helpers.h"

/* Define scanner function for lexer */
#define YY_DECL int yylex ( YYSTYPE * yylval_param, YYLTYPE * yylloc_param, \
      yyscan_t yyscanner, xed_encoder_request_t *req, parser_state_t *s)

/* Epilogue for every non-empty action that might signal a parsing error 
   in functions that it calls. Used for location tracking.
   loc points to the first bad token, typically @1 */
#define HANDLE_ERROR(loc) if (s->error_found) { \
                              s->error_position = loc.first_column; \
                              YYABORT; \
                          }



/* Line 2053 of yacc.c  */
#line 66 "./examples/pregenerated/intel-syntax.parser.h"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOK_REPE_PREF = 258,
     TOK_REPNE_PREF = 259,
     TOK_LOCK_PREF = 260,
     TOK_OPSIZE_PREF = 261,
     TOK_OPCODE = 262,
     TOK_GPR = 263,
     TOK_VEC_REG = 264,
     TOK_SEG_REG = 265,
     TOK_FPU_REG = 266,
     TOK_MMX_REG = 267,
     TOK_CONTROL_REG = 268,
     TOK_DEBUG_REG = 269,
     TOK_BOUND_REG = 270,
     TOK_OPMASK_REG = 271,
     TOK_CONSTANT = 272,
     TOK_MEMWIDTH = 273,
     TOK_FAR = 274,
     TOK_COMMA = 275,
     TOK_LSQBR = 276,
     TOK_RSQBR = 277,
     TOK_PLUS = 278,
     TOK_MINUS = 279,
     TOK_MULTI = 280,
     TOK_COLON = 281,
     TOK_LCUBR = 282,
     TOK_RCUBR = 283,
     TOK_ZEROING = 284,
     TOK_BCAST = 285,
     TOK_SAE_ROUNDING = 286,
     TOK_GARBAGE = 287
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2053 of yacc.c  */
#line 81 "./examples/intel-syntax.y"

    xed_reg_enum_t regname;
    struct {
        xed_uint64_t value;
        unsigned int width_bits;
    } literal;

    int rounding_mode;
    char opcode_string[100]; /* saved here for late mangling */


/* Line 2053 of yacc.c  */
#line 125 "./examples/pregenerated/intel-syntax.parser.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void* lexer_state, xed_encoder_request_t *req, parser_state_t *s);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_EXAMPLES_PREGENERATED_INTEL_SYNTAX_PARSER_H_INCLUDED  */
