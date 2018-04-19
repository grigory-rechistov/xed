/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 65 "./examples/intel-syntax.y" /* yacc.c:339  */


#include <stdio.h>
#include <stdlib.h>
#include "xed-encode.h"
#include "intel-syntax.parser.h"

#define YY_NO_UNISTD_H // work around flex's attempt to include unistd.h
#include "intel-syntax.lexer.h"

int yylex (YYSTYPE * yylval_param, YYLTYPE * yylloc_param , yyscan_t yyscanner,
           xed_encoder_request_t *req, parser_state_t *s);
static void yyerror(YYLTYPE *locp, void *lexer_state,
            xed_encoder_request_t *req, parser_state_t *state, const char* msg);


#line 83 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "intel-syntax.parser.h".  */
#ifndef YY_YY_EXAMPLES_PREGENERATED_INTEL_SYNTAX_PARSER_H_INCLUDED
# define YY_YY_EXAMPLES_PREGENERATED_INTEL_SYNTAX_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 46 "./examples/intel-syntax.y" /* yacc.c:355  */

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


#line 132 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:355  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
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

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 83 "./examples/intel-syntax.y" /* yacc.c:355  */

    xed_reg_enum_t regname;
    struct {
        xed_uint64_t value;
        unsigned int width_bits;
    } literal;

    int rounding_mode;
    char opcode_string[100]; /* saved here for late mangling */

#line 188 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



int yyparse (void* lexer_state, xed_encoder_request_t *req, parser_state_t *s);

#endif /* !YY_YY_EXAMPLES_PREGENERATED_INTEL_SYNTAX_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 218 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   75

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  33
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  38
/* YYNRULES -- Number of rules.  */
#define YYNRULES  77
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  108

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   287

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   134,   134,   137,   138,   140,   141,   142,   143,   146,
     150,   156,   157,   158,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   176,   181,   187,
     193,   199,   204,   208,   213,   219,   227,   235,   243,   248,
     256,   261,   262,   265,   270,   275,   282,   283,   284,   285,
     286,   289,   290,   291,   292,   293,   294,   298,   304,   311,
     321,   328,   337,   346,   357,   358,   361,   368,   377,   387,
     388,   389,   393,   402,   412,   419,   429,   430
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOK_REPE_PREF", "TOK_REPNE_PREF",
  "TOK_LOCK_PREF", "TOK_OPSIZE_PREF", "TOK_OPCODE", "TOK_GPR",
  "TOK_VEC_REG", "TOK_SEG_REG", "TOK_FPU_REG", "TOK_MMX_REG",
  "TOK_CONTROL_REG", "TOK_DEBUG_REG", "TOK_BOUND_REG", "TOK_OPMASK_REG",
  "TOK_CONSTANT", "TOK_MEMWIDTH", "TOK_FAR", "TOK_COMMA", "TOK_LSQBR",
  "TOK_RSQBR", "TOK_PLUS", "TOK_MINUS", "TOK_MULTI", "TOK_COLON",
  "TOK_LCUBR", "TOK_RCUBR", "TOK_ZEROING", "TOK_BCAST", "TOK_SAE_ROUNDING",
  "TOK_GARBAGE", "$accept", "asmline", "prefixes", "prefix", "opcode",
  "operands", "operand", "general_purpose_register", "vector_register",
  "control_register", "debug_register", "bound_register",
  "fpu_or_mmx_register", "opmask_register", "literal_const",
  "signed_literal", "far_pointer", "lea_spec", "mem_spec",
  "segment_override_mem_spec", "default_segment_mem_spec",
  "segment_override", "agen_expr", "mem_expr", "indirect_addr_gpr",
  "indirect_addr_gpr_plus_offset", "bx_bp_si_di", "base_index_scale",
  "base_index_scale_disp", "vsib_mem_expr", "base_vec_index_scale",
  "base_vec_index_scale_disp", "vec_register_filtered",
  "vec_register_masked", "vec_register_masked_zeroed", "vec_register_sae",
  "sae_modifiers", "broadcast_modifier", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287
};
# endif

#define YYPACT_NINF -38

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-38)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -38,    14,    28,   -38,   -38,   -38,   -38,   -38,    21,   -38,
      -8,   -38,   -38,    -7,   -38,   -38,   -38,   -38,   -38,   -38,
      -2,     1,    33,    25,    26,    24,   -38,   -38,   -38,   -38,
     -38,   -38,   -38,   -38,   -38,   -38,   -38,   -38,   -38,   -38,
     -38,   -38,   -38,   -38,   -38,   -14,     6,    19,    38,    27,
       2,    29,   -38,   -38,   -38,   -38,   -38,   -38,   -38,    -8,
      22,   -38,    30,   -38,    32,   -38,     5,    31,   -38,   -38,
     -38,   -38,   -38,   -38,   -38,   -38,    38,     4,    35,   -38,
     -38,    18,   -38,   -38,    10,    34,    37,    36,   -38,   -38,
     -38,    40,   -38,   -38,    34,    39,    43,   -38,    13,    15,
      45,    46,    49,    50,   -38,   -38,   -38,   -38
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,     1,     5,     6,     7,     8,     9,     4,
      11,    10,    27,    28,    32,    33,    29,    30,    31,    34,
      35,     0,     0,     0,     0,     2,    12,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    41,
      42,    26,    69,    70,    71,     0,     0,     0,     0,     0,
      57,     0,    46,    47,    48,    49,    50,    37,    36,     0,
       0,    75,     0,    38,     0,    45,    57,     0,    51,    52,
      53,    54,    55,    56,    64,    65,     0,     0,     0,    40,
      13,    72,    74,    39,     0,    76,     0,    60,    58,    59,
      73,     0,    77,    44,    76,     0,     0,    43,    61,    66,
       0,     0,     0,     0,    62,    63,    67,    68
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -38,   -38,   -38,   -38,   -38,   -38,    -5,   -38,   -38,   -38,
     -38,   -38,   -38,   -38,   -38,   -38,   -38,   -38,   -38,   -38,
     -38,   -38,   -38,   -21,    47,    48,    51,    52,    53,   -38,
     -38,   -38,   -38,   -38,   -38,   -38,   -38,   -37
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,     2,     9,    10,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    49,    51,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    41,    42,    43,    44,    62,    93
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      12,    13,    60,    14,    15,    16,    17,    18,    19,    20,
      21,    47,    87,    22,     3,    23,    24,    61,    87,    91,
      45,    88,    48,    63,    46,    77,    78,    88,    84,    78,
      64,     4,     5,     6,     7,     8,   100,   101,   102,   103,
      11,    50,    57,    58,    59,    65,    66,    90,    76,    83,
      81,    79,    89,    85,    80,    86,    98,    97,    82,    94,
      99,    95,   104,   105,    92,    96,   106,   107,     0,    52,
      53,     0,     0,    54,    55,    56
};

static const yytype_int8 yycheck[] =
{
       8,     9,    16,    11,    12,    13,    14,    15,    16,    17,
      18,    10,     8,    21,     0,    23,    24,    31,     8,     9,
      27,    17,    21,    17,    26,    23,    24,    17,    23,    24,
      24,     3,     4,     5,     6,     7,    23,    24,    23,    24,
      19,     8,    17,    17,    20,    26,     8,    29,    21,    17,
      28,    22,    17,    22,    59,    76,    17,    94,    28,    22,
      17,    25,    17,    17,    30,    25,    17,    17,    -1,    22,
      22,    -1,    -1,    22,    22,    22
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    34,    35,     0,     3,     4,     5,     6,     7,    36,
      37,    19,     8,     9,    11,    12,    13,    14,    15,    16,
      17,    18,    21,    23,    24,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    65,    66,    67,    68,    27,    26,    10,    21,    54,
       8,    55,    57,    58,    59,    60,    61,    17,    17,    20,
      16,    31,    69,    17,    24,    26,     8,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    21,    23,    24,    22,
      39,    28,    28,    17,    23,    22,    56,     8,    17,    17,
      29,     9,    30,    70,    22,    25,    25,    70,    17,    17,
      23,    24,    23,    24,    17,    17,    17,    17
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    33,    34,    35,    35,    36,    36,    36,    36,    37,
      37,    38,    38,    38,    39,    39,    39,    39,    39,    39,
      39,    39,    39,    39,    39,    39,    39,    40,    41,    42,
      43,    44,    45,    45,    46,    47,    48,    48,    49,    49,
      50,    51,    51,    52,    53,    54,    55,    55,    55,    55,
      55,    56,    56,    56,    56,    56,    56,    57,    58,    58,
      59,    60,    61,    61,    62,    62,    63,    64,    64,    65,
      65,    65,    66,    67,    68,    69,    70,    70
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     0,     2,     1,     1,     1,     1,     1,
       2,     0,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     3,     4,
       3,     1,     1,     6,     5,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     5,     7,     7,     1,     1,     5,     7,     7,     1,
       1,     1,     4,     5,     4,     1,     0,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (&yylloc, lexer_state, req, s, YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location, lexer_state, req, s); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, void* lexer_state, xed_encoder_request_t *req, parser_state_t *s)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  YYUSE (lexer_state);
  YYUSE (req);
  YYUSE (s);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, void* lexer_state, xed_encoder_request_t *req, parser_state_t *s)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, lexer_state, req, s);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, void* lexer_state, xed_encoder_request_t *req, parser_state_t *s)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , lexer_state, req, s);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, lexer_state, req, s); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, void* lexer_state, xed_encoder_request_t *req, parser_state_t *s)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (lexer_state);
  YYUSE (req);
  YYUSE (s);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void* lexer_state, xed_encoder_request_t *req, parser_state_t *s)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);

        yyls = yyls1;
        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex (&yylval, &yylloc, lexer_state, req, s);
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 9:
#line 146 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
        fill_mnemonic_opcode(req, s, (yyvsp[0].opcode_string), sizeof((yyvsp[0].opcode_string)));
        HANDLE_ERROR((yylsp[0]));
}
#line 1487 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 10:
#line 150 "./examples/intel-syntax.y" /* yacc.c:1646  */
    { /* far call/ret/jmp */
        fill_mnemonic_opcode(req, s, (yyvsp[-1].opcode_string), sizeof((yyvsp[-1].opcode_string)));
        s->seen_far_ptr = true;
        HANDLE_ERROR((yylsp[-1]));
}
#line 1497 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 27:
#line 176 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
        fill_register_operand(req, s, (yyvsp[0].regname));
        HANDLE_ERROR((yylsp[0]));
}
#line 1506 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 28:
#line 181 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
        fill_register_operand(req, s, (yyvsp[0].regname));
        deduce_operand_width_vector(req, s, (yyvsp[0].regname));
        HANDLE_ERROR((yylsp[0]));
}
#line 1516 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 29:
#line 187 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
        fill_register_operand(req, s, (yyvsp[0].regname));
        s->seen_cr = true;
        HANDLE_ERROR((yylsp[0]));
}
#line 1526 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 193 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
        fill_register_operand(req, s, (yyvsp[0].regname));
        s->seen_dr = true;
        HANDLE_ERROR((yylsp[0]));
}
#line 1536 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 199 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
        fill_register_operand(req, s, (yyvsp[0].regname));
        HANDLE_ERROR((yylsp[0]));
}
#line 1545 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 204 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
        fill_register_operand(req, s, (yyvsp[0].regname));
        HANDLE_ERROR((yylsp[0]));
}
#line 1554 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 208 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
        fill_register_operand(req, s, (yyvsp[0].regname));
        HANDLE_ERROR((yylsp[0]));
}
#line 1563 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 213 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
        fill_register_operand(req, s, (yyvsp[0].regname));
        HANDLE_ERROR((yylsp[0]));
}
#line 1572 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 219 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
        if (instr_category_uses_rel_branch(s->early_category))
            fill_relative_offset_operand(req, s, (yyvsp[0].literal).value, (yyvsp[0].literal).width_bits);
        else
            fill_immediate_operand(req, s, (yyvsp[0].literal).value, (yyvsp[0].literal).width_bits);
        HANDLE_ERROR((yylsp[0]));
}
#line 1584 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 36:
#line 227 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
        xed_uint64_t negval = -XED_CAST(xed_int64_t, (yyvsp[0].literal).value);
        if (instr_category_uses_rel_branch(s->early_category))
            fill_relative_offset_operand(req, s, negval, (yyvsp[0].literal).width_bits);
        else
            fill_immediate_operand(req, s, negval, (yyvsp[0].literal).width_bits);
        HANDLE_ERROR((yylsp[-1]));
}
#line 1597 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 37:
#line 235 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
        if (instr_category_uses_rel_branch(s->early_category))
            fill_relative_offset_operand(req, s, +(yyvsp[0].literal).value, (yyvsp[0].literal).width_bits);
        else
            fill_immediate_operand(req, s, +(yyvsp[0].literal).value, (yyvsp[0].literal).width_bits);
        HANDLE_ERROR((yylsp[-1]));
}
#line 1609 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 38:
#line 243 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
       fill_far_pointer_operand(req, s, (yyvsp[-2].literal).value, (yyvsp[-2].literal).width_bits, (yyvsp[0].literal).value, (yyvsp[0].literal).width_bits);
       s->seen_far_ptr = true;
       HANDLE_ERROR((yylsp[-2]));
}
#line 1619 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 39:
#line 248 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
       /* Negate the offset */
       xed_uint64_t negval = -XED_CAST(xed_int64_t, (yyvsp[0].literal).value);
       fill_far_pointer_operand(req, s, (yyvsp[-3].literal).value, (yyvsp[-3].literal).width_bits, negval, (yyvsp[0].literal).width_bits);
       s->seen_far_ptr = true;
       HANDLE_ERROR((yylsp[-3]));
}
#line 1631 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 40:
#line 256 "./examples/intel-syntax.y" /* yacc.c:1646  */
    { /* LEA does not have "mem ptr" */
        fill_agen_operand(req, s);
        HANDLE_ERROR((yylsp[-1]));
}
#line 1640 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 43:
#line 265 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
        fill_memory_operand(req, s);
        HANDLE_ERROR((yylsp[-2]));
}
#line 1649 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 44:
#line 270 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
        fill_memory_operand(req, s);
        HANDLE_ERROR((yylsp[-2]));
}
#line 1658 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 45:
#line 275 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
    s->segment_reg = (yyvsp[-1].regname);
    // TODO s->segno ++;
    HANDLE_ERROR((yylsp[-1]));
}
#line 1668 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 57:
#line 298 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
        s->base_reg = (yyvsp[0].regname);
        HANDLE_ERROR((yylsp[0]));
}
#line 1677 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 58:
#line 304 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
        s->base_reg = (yyvsp[-2].regname);
        s->disp_valid = 1;
        s->disp_val = (yyvsp[0].literal).value;
        s->disp_width_bits = (yyvsp[0].literal).width_bits;
        HANDLE_ERROR((yylsp[-2]));
}
#line 1689 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 59:
#line 311 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
        s->base_reg = (yyvsp[-2].regname);
        s->disp_valid = 1;
        s->disp_val = -XED_CAST(xed_int64_t, (yyvsp[0].literal).value); /* Use negative constant */
        s->disp_width_bits = (yyvsp[0].literal).width_bits;
        HANDLE_ERROR((yylsp[-2]));

}
#line 1702 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 60:
#line 321 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
        s->base_reg = (yyvsp[-2].regname);
        s->index_reg = (yyvsp[0].regname);
        HANDLE_ERROR((yylsp[-2]));
}
#line 1712 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 61:
#line 328 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
        s->base_reg = (yyvsp[-4].regname);
        s->index_reg = (yyvsp[-2].regname);
        s->scale_val = (yyvsp[0].literal).value;
        /* TODO: accept only valid numerical scale factors? */
        HANDLE_ERROR((yylsp[-4]));
}
#line 1724 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 62:
#line 337 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
        s->base_reg = (yyvsp[-6].regname);
        s->index_reg = (yyvsp[-4].regname);
        s->scale_val = (yyvsp[-2].literal).value;
        s->disp_valid = 1;
        s->disp_val = (yyvsp[0].literal).value;
        s->disp_width_bits = (yyvsp[0].literal).width_bits;
        HANDLE_ERROR((yylsp[-6]));
}
#line 1738 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 63:
#line 346 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
        s->base_reg = (yyvsp[-6].regname);
        s->index_reg = (yyvsp[-4].regname);
        s->scale_val = (yyvsp[-2].literal).value;
        s->disp_valid = 1;
        s->disp_val = -XED_CAST(xed_int64_t, (yyvsp[0].literal).value); /* Use negative constant */
        s->disp_width_bits = (yyvsp[0].literal).width_bits;
        HANDLE_ERROR((yylsp[-6]));
}
#line 1752 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 66:
#line 361 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
        s->base_reg = (yyvsp[-4].regname);
        s->index_reg = (yyvsp[-2].regname);
        s->scale_val = (yyvsp[0].literal).value;
        HANDLE_ERROR((yylsp[-4]));
}
#line 1763 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 67:
#line 368 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
        s->base_reg = (yyvsp[-6].regname);
        s->index_reg = (yyvsp[-4].regname);
        s->scale_val = (yyvsp[-2].literal).value;
        s->disp_valid = 1;
        s->disp_val = (yyvsp[0].literal).value;
        s->disp_width_bits = (yyvsp[0].literal).width_bits;
        HANDLE_ERROR((yylsp[-6]));
}
#line 1777 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 68:
#line 377 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
        s->base_reg = (yyvsp[-6].regname);
        s->index_reg = (yyvsp[-4].regname);
        s->scale_val = (yyvsp[-2].literal).value;
        s->disp_valid = 1;
        s->disp_val = -XED_CAST(xed_int64_t, (yyvsp[0].literal).value); /* Use negative constant */
        s->disp_width_bits = (yyvsp[0].literal).width_bits;
        HANDLE_ERROR((yylsp[-6]));
}
#line 1791 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 72:
#line 393 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
        /* TODO check that used only once */
        fill_register_operand(req, s, (yyvsp[-3].regname)); // main register
        deduce_operand_width_vector(req, s, (yyvsp[-3].regname));
        fill_register_operand(req, s, (yyvsp[-1].regname)); // opmask register
        HANDLE_ERROR((yylsp[-3])); // TODO separate the action into two to improve location tracking
}
#line 1803 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 73:
#line 402 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
        /* TODO check that used only once */
        fill_register_operand(req, s, (yyvsp[-4].regname)); // main register
        deduce_operand_width_vector(req, s, (yyvsp[-4].regname));
        fill_register_operand(req, s, (yyvsp[-2].regname)); // opmask register
        xed3_set_generic_operand(req, XED_OPERAND_ZEROING, 1);
        HANDLE_ERROR((yylsp[-4])); // TODO separate the action into two to improve location tracking
}
#line 1816 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 74:
#line 412 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
        fill_register_operand(req, s, (yyvsp[-3].regname)); // main register
        deduce_operand_width_vector(req, s, (yyvsp[-3].regname));
        HANDLE_ERROR((yylsp[-3]));
}
#line 1826 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 75:
#line 419 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
        xed3_set_generic_operand(req, XED_OPERAND_SAE, 1);
        if ((yyvsp[0].rounding_mode) >= 0) {
            unsigned xed_rnding = (yyvsp[0].rounding_mode)+1; /* XED shifts it */
            xed3_set_generic_operand(req, XED_OPERAND_ROUNDC, xed_rnding);
        }
        HANDLE_ERROR((yylsp[0]));
}
#line 1839 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;

  case 77:
#line 430 "./examples/intel-syntax.y" /* yacc.c:1646  */
    {
        xed3_set_generic_operand(req, XED_OPERAND_BCAST, 1);
        HANDLE_ERROR((yylsp[0]));
}
#line 1848 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
    break;


#line 1852 "./examples/pregenerated/intel-syntax.parser.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, lexer_state, req, s, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (&yylloc, lexer_state, req, s, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, lexer_state, req, s);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[1] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp, lexer_state, req, s);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, lexer_state, req, s, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, lexer_state, req, s);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp, lexer_state, req, s);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 435 "./examples/intel-syntax.y" /* yacc.c:1906  */


/* The function is called when YYABORT happened or parser found syntax error */
static void yyerror(YYLTYPE *locp, void *lexer_state,
             xed_encoder_request_t *req, parser_state_t *state, const char* msg)
{
    state->error_found = 1;
    if (locp) {
        fprintf(stderr, "[XED_CLIENT_ERROR] Scanner parsing error:"
                        " %s (symbols [%d:%d])\n",
                        msg,
                        locp->first_column,
                        locp->last_column);
        state->error_position = locp->first_column;
    } else {
        fprintf(stderr, "[XED_CLIENT_ERROR] Scanner parsing error: %s\n", msg);
    }
    (void)lexer_state;
    (void)req;
}
