/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

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

#ifndef YY_SPVM_YY_SPVM_YACC_TAB_H_INCLUDED
# define YY_SPVM_YY_SPVM_YACC_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int SPVM_yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    CLASS = 258,
    HAS = 259,
    METHOD = 260,
    OUR = 261,
    ENUM = 262,
    MY = 263,
    USE = 264,
    AS = 265,
    REQUIRE = 266,
    ALIAS = 267,
    ALLOW = 268,
    CURRENT_CLASS = 269,
    MUTABLE = 270,
    DESCRIPTOR = 271,
    MAKE_READ_ONLY = 272,
    IMPLEMENT = 273,
    IF = 274,
    UNLESS = 275,
    ELSIF = 276,
    ELSE = 277,
    FOR = 278,
    WHILE = 279,
    LAST = 280,
    NEXT = 281,
    SWITCH = 282,
    CASE = 283,
    DEFAULT = 284,
    BREAK = 285,
    EVAL = 286,
    NAME = 287,
    VAR_NAME = 288,
    CONSTANT = 289,
    EXCEPTION_VAR = 290,
    UNDEF = 291,
    VOID = 292,
    BYTE = 293,
    SHORT = 294,
    INT = 295,
    LONG = 296,
    FLOAT = 297,
    DOUBLE = 298,
    STRING = 299,
    OBJECT = 300,
    TRUE = 301,
    FALSE = 302,
    END_OF_FILE = 303,
    DOT3 = 304,
    FATCAMMA = 305,
    RW = 306,
    RO = 307,
    WO = 308,
    INIT = 309,
    NEW = 310,
    OF = 311,
    TYPE_COMMENT_START = 312,
    TYPE_COMMENT_END = 313,
    RETURN = 314,
    WEAKEN = 315,
    DIE = 316,
    WARN = 317,
    PRINT = 318,
    CURRENT_CLASS_NAME = 319,
    UNWEAKEN = 320,
    ASSIGN = 321,
    SPECIAL_ASSIGN = 322,
    LOGICAL_OR = 323,
    LOGICAL_AND = 324,
    BIT_OR = 325,
    BIT_XOR = 326,
    BIT_AND = 327,
    NUMEQ = 328,
    NUMNE = 329,
    STREQ = 330,
    STRNE = 331,
    NUMGT = 332,
    NUMGE = 333,
    NUMLT = 334,
    NUMLE = 335,
    STRGT = 336,
    STRGE = 337,
    STRLT = 338,
    STRLE = 339,
    ISA = 340,
    NUMERIC_CMP = 341,
    STRING_CMP = 342,
    SHIFT = 343,
    DIVIDE = 344,
    DIVIDE_UNSIGNED_INT = 345,
    DIVIDE_UNSIGNED_LONG = 346,
    REMAINDER = 347,
    REMAINDER_UNSIGNED_INT = 348,
    REMAINDER_UNSIGNED_LONG = 349,
    LOGICAL_NOT = 350,
    BIT_NOT = 351,
    CREATE_REF = 352,
    DEREF = 353,
    PLUS = 354,
    MINUS = 355,
    CONVERT = 356,
    SCALAR = 357,
    STRING_LENGTH = 358,
    ISWEAK = 359,
    REFCNT = 360,
    REFOP = 361,
    DUMP = 362,
    NEW_STRING_LEN = 363,
    IS_READ_ONLY = 364,
    COPY = 365,
    HAS_IMPLEMENT = 366,
    INC = 367,
    DEC = 368,
    ARROW = 369
  };
#endif

/* Value type.  */



int SPVM_yyparse (SPVM_COMPILER* compiler);

#endif /* !YY_SPVM_YY_SPVM_YACC_TAB_H_INCLUDED  */
