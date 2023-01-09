/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 10 "finalAssignment.y"

	#include	<stdio.h>
	#include	<math.h>
	#include	<stdlib.h>
	#include	<string.h>
    #include    <regex.h>
    #include    <time.h>
	
   
    int * count; 
    char* numRegX;
   	extern int yylex();
	int yyerror(char const * s);
	float potenciaConWhile(float numero, int potencia);
	extern char * yytext;
	extern FILE * yyin;
	extern int order;
	extern int line;
    typedef struct arst ARST;
    struct arst{
        unsigned char type;       
        int token;
        char *valStr;
        int valInt;
        float valFlt;
        ARST *right;
        ARST *left;
    };


    ARST * newNode(unsigned char, int, char*, int, float, ARST *, ARST *);
	ARST * treeSearch(ARST *);
    void printARST(ARST *);
    int opInt(ARST*,ARST*, ARST*);
    float opFlt(ARST*,ARST*, ARST*);

float potenciaConWhile(float numero, int potencia)
{
    int resultado = numero;
    while (potencia > 1)
    {
        resultado = resultado * numero;
        potencia--;
    }
    return resultado;
}

int yyerror(char const * s) {
	fprintf(stderr, "%s,%s at [%d,%d]\n", s, yytext,line,order);
	exit(1);
}

int match(const char *string, char *pattern)
{
    int    status;
    regex_t    re;


    if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) {
        return(0);      /* Report error. */
    }
    status = regexec(&re, string, (size_t) 0, NULL, 0);
    regfree(&re);
    if (status != 0) {
        return(0);      /* Report error. */
    }
    return(1);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* REDUCED SYNTAX TREE METHODS */


ARST * newNode(unsigned char type, int token, char* valStr, int valInt, float valFlt, ARST * left, ARST * right) {
   ARST * temp = (ARST *) malloc(sizeof(ARST));
   temp -> token    = token;
   temp -> type     = type;
   temp -> valStr  = valStr;
   temp -> valInt   = valInt;
   temp -> valFlt   = valFlt;
   temp -> left     = left;
   temp -> right    = right;
   return temp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* HASH TABLE METHODS */

#define CAPACITY 50000 // Size of the Hash Table


unsigned long hash_function(char* str) {
    unsigned long i = 0;
    for (int j=0; str[j]; j++)
        i += str[j];
    return i % CAPACITY;
}

typedef struct Ht_item Ht_item;

// Define the Hash Table Item here
struct Ht_item {
    char* key;
    ARST* treeNode; 
};


typedef struct LinkedList LinkedList;

// Define the Linkedlist here
struct LinkedList {
    Ht_item* item; 
    LinkedList* next;
};


typedef struct HashTable HashTable;

// Define the Hash Table here
struct HashTable {
    // Contains an array of pointers
    // to items
    Ht_item** items;
    LinkedList** overflow_buckets;
    int size;
    int count;
};

HashTable* symbolTable;


static LinkedList* allocate_list () {
    // Allocates memory for a Linkedlist pointer
    LinkedList* list = (LinkedList*) malloc (sizeof(LinkedList));
    return list;
};

static LinkedList * linkedlist_insert(LinkedList* list, Ht_item* item) {
    // Inserts the item onto the Linked List
    if (!list) {
        LinkedList* head = allocate_list();
        head->item = item;
        head->next = NULL;
        list = head;
        return list;
    } 
    
    else if (list->next == NULL) {
        LinkedList* node = allocate_list();
        node->item = item;
        node->next = NULL;
        list->next = node;
        return list;
    }

    LinkedList* temp = list;
    while (temp->next->next) {
        temp = temp->next;
    }
    
    LinkedList* node = allocate_list();
    node->item = item;
    node->next = NULL;
    temp->next = node;
    
    return list;
};

static Ht_item* linkedlist_remove(LinkedList* list) {
    // Removes the head from the linked list
    // and returns the item of the popped element
    if (!list)
        return NULL;
    if (!list->next)
        return NULL;
    LinkedList* node = list->next;
    LinkedList* temp = list;
    temp->next = NULL;
    list = node;
    Ht_item* it = NULL;
    memcpy(temp->item, it, sizeof(Ht_item));
    free(temp->item->key);
    free(temp->item->treeNode);
    free(temp->item);
    free(temp);
    return it;
}

static void free_linkedlist(LinkedList* list) {
    LinkedList* temp = list;
    while (list) {
        temp = list;
        list = list->next;
        free(temp->item->key);
        free(temp->item->treeNode);
        free(temp->item);
        free(temp);
    }
}

static LinkedList** create_overflow_buckets(HashTable* table) {
    // Create the overflow buckets; an array of linkedlists
    LinkedList** buckets = (LinkedList**) calloc (table->size, sizeof(LinkedList*));
    for (int i=0; i<table->size; i++)
        buckets[i] = NULL;
    return buckets;
}

static void free_overflow_buckets(HashTable* table) {
    // Free all the overflow bucket lists
    LinkedList** buckets = table->overflow_buckets;
    for (int i=0; i<table->size; i++)
        free_linkedlist(buckets[i]);
    free(buckets);
}


Ht_item* create_item(char* key, ARST* treeNode) {
    // Creates a pointer to a new hash table item
    Ht_item* item = (Ht_item*) malloc (sizeof(Ht_item));
    item->key = (char*) malloc (strlen(key) + 1);
    item->treeNode = (ARST*) malloc (sizeof(treeNode)+1);
   ////////////////////////////////
    strcpy(item->key, key);
    item->treeNode=treeNode;
    return item;
}

HashTable* create_table(int size) {
    // Creates a new HashTable
    HashTable* table = (HashTable*) malloc (sizeof(HashTable));
    table->size = size;
    table->count = 0;
    table->items = (Ht_item**) calloc (table->size, sizeof(Ht_item*));
    for (int i=0; i<table->size; i++)
        table->items[i] = NULL;
    table->overflow_buckets = create_overflow_buckets(table);

    return table;
}

void free_item(Ht_item* item) {
    // Frees an item
    free(item->key);
    free(item->treeNode);
    free(item);
}

void free_table(HashTable* table) {
    // Frees the table
    for (int i=0; i<table->size; i++) {
        Ht_item* item = table->items[i];
        if (item != NULL)
            free_item(item);
    }

    free_overflow_buckets(table);
    free(table->items);
    free(table);
}

void handle_collision(HashTable* table, unsigned long index, Ht_item* item) {
    LinkedList* head = table->overflow_buckets[index];

    if (head == NULL) {
        // We need to create the list
        head = allocate_list();
        head->item = item;
        table->overflow_buckets[index] = head;
        return;
    }
    else {
        // Insert to the list
        table->overflow_buckets[index] = linkedlist_insert(head, item);
        return;
    }
 }

void ht_insert(HashTable* table, char* key, ARST* treeNode) {
    // Create the item
    Ht_item* item = create_item(key, treeNode);

    // Compute the index
    unsigned long index = hash_function(key);

    Ht_item* current_item = table->items[index];
    
    if (current_item == NULL) {
        // Key does not exist.
        if (table->count == table->size) {
            // Hash Table Full
            printf("Insert Error: Hash Table is full\n");
            // Remove the create item
            free_item(item);
            return;
        }
        
        // Insert directly
        table->items[index] = item; 
        table->count++;
    }

    else {
            // Scenario 1: We only need to update value
            if (strcmp(current_item->key, key) == 0) {
                table->items[index]->treeNode=treeNode;
                return;
            }
    
        else {
            // Scenario 2: Collision
            handle_collision(table, index, item);
            return;
        }
    }
}

ARST* ht_search(HashTable* table, char* key) {
    // Searches the key in the hashtable
    // and returns NULL if it doesn't exist
    int index = hash_function(key);
    Ht_item* item = table->items[index];
    LinkedList* head = table->overflow_buckets[index];

    // Ensure that we move to items which are not NULL
    while (item != NULL) {
        if (strcmp(item->key, key) == 0)
            return item->treeNode;
        if (head == NULL)
            return NULL;
        item = head->item;
        head = head->next;
    }
    return NULL;
}

void ht_delete(HashTable* table, char* key) {
    // Deletes an item from the table
    int index = hash_function(key);
    Ht_item* item = table->items[index];
    LinkedList* head = table->overflow_buckets[index];

    if (item == NULL) {
        // Does not exist. Return
        return;
    }
    else {
        if (head == NULL && strcmp(item->key, key) == 0) {
            // No collision chain. Remove the item
            // and set table index to NULL
            table->items[index] = NULL;
            free_item(item);
            table->count--;
            return;
        }
        else if (head != NULL) {
            // Collision Chain exists
            if (strcmp(item->key, key) == 0) {
                // Remove this item and set the head of the list
                // as the new item
                
                free_item(item);
                LinkedList* node = head;
                head = head->next;
                node->next = NULL;
                table->items[index] = create_item(node->item->key, node->item->treeNode);
                free_linkedlist(node);
                table->overflow_buckets[index] = head;
                return;
            }

            LinkedList* curr = head;
            LinkedList* prev = NULL;
            
            while (curr) {
                if (strcmp(curr->item->key, key) == 0) {
                    if (prev == NULL) {
                        // First element of the chain. Remove the chain
                        free_linkedlist(head);
                        table->overflow_buckets[index] = NULL;
                        return;
                    }
                    else {
                        // This is somewhere in the chain
                        prev->next = curr->next;
                        curr->next = NULL;
                        free_linkedlist(curr);
                        table->overflow_buckets[index] = head;
                        return;
                    }
                }
                curr = curr->next;
                prev = curr;
            }

        }
    }
}

void print_search(HashTable* table, char* key) {
    ARST* treeNode;
    if ((treeNode = ht_search(table, key)) == NULL) {
        printf("%s does not exist\n", key);
        return;
    }
    else {
        printf("[NAME]\t[STORED]\n");
        printf("%s\t\n", key);
        printARST(treeNode);
    }
}

void print_table(HashTable* table) {
    int val;
    char* pattern = "^[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)$";
    printf("\n ******************** SYMBOL TABLE ***************************\n");
    printf("---------------------------------------------------------------\n");
    printf("[ID]\t[KEY]\t\t[STORAGED TOKEN]\t\t[TYPE]\t[VALUE]\n");
    for (int i=0; i<table->size; i++) {
        if (table->items[i]){
            val=match(table->items[i]->key,pattern);
            if(val==0){
                printf("%d\t %s\t", i, table->items[i]->key);
                printARST(table->items[i]->treeNode);
                if (table->overflow_buckets[i]) {
                    printf(" => Overflow Bucket => ");
                    LinkedList* head = table->overflow_buckets[i];
                    while (head) {
                        printf("%s\t\n", head->item->key);
                        printARST(head->item->treeNode);
                        head = head->next;
                    }
                }
                            printf("\n"); 
            } 

        }
    }
    printf("---------------------------------------------------------------\n");
};





#line 522 "finalAssignment.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "finalAssignment.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_T_COMMA = 3,                    /* T_COMMA  */
  YYSYMBOL_T_SEMICOL = 4,                  /* T_SEMICOL  */
  YYSYMBOL_T_OPENKEY = 5,                  /* T_OPENKEY  */
  YYSYMBOL_T_CLOSEKEY = 6,                 /* T_CLOSEKEY  */
  YYSYMBOL_T_OPENBRACK = 7,                /* T_OPENBRACK  */
  YYSYMBOL_T_CLOSEBRACK = 8,               /* T_CLOSEBRACK  */
  YYSYMBOL_T_OPENPAR = 9,                  /* T_OPENPAR  */
  YYSYMBOL_T_CLOSEPAR = 10,                /* T_CLOSEPAR  */
  YYSYMBOL_T_PLUSOP = 11,                  /* T_PLUSOP  */
  YYSYMBOL_T_MULTOP = 12,                  /* T_MULTOP  */
  YYSYMBOL_T_MINUSOP = 13,                 /* T_MINUSOP  */
  YYSYMBOL_T_DIVOP = 14,                   /* T_DIVOP  */
  YYSYMBOL_T_ASSOP = 15,                   /* T_ASSOP  */
  YYSYMBOL_T_EQSYM = 16,                   /* T_EQSYM  */
  YYSYMBOL_T_LESSOP = 17,                  /* T_LESSOP  */
  YYSYMBOL_T_MOREOP = 18,                  /* T_MOREOP  */
  YYSYMBOL_T_LESSOEQOP = 19,               /* T_LESSOEQOP  */
  YYSYMBOL_T_MOREOEQOP = 20,               /* T_MOREOEQOP  */
  YYSYMBOL_T_DOIF = 21,                    /* T_DOIF  */
  YYSYMBOL_T_DOIFELSE = 22,                /* T_DOIFELSE  */
  YYSYMBOL_T_DOWHILE = 23,                 /* T_DOWHILE  */
  YYSYMBOL_T_DOUNTIL = 24,                 /* T_DOUNTIL  */
  YYSYMBOL_T_PROGRAM = 25,                 /* T_PROGRAM  */
  YYSYMBOL_T_BEGIN = 26,                   /* T_BEGIN  */
  YYSYMBOL_T_END = 27,                     /* T_END  */
  YYSYMBOL_T_EOF = 28,                     /* T_EOF  */
  YYSYMBOL_T_MODOP = 29,                   /* T_MODOP  */
  YYSYMBOL_T_POWOP = 30,                   /* T_POWOP  */
  YYSYMBOL_T_COLON = 31,                   /* T_COLON  */
  YYSYMBOL_T_READ = 32,                    /* T_READ  */
  YYSYMBOL_T_PRINT = 33,                   /* T_PRINT  */
  YYSYMBOL_T_ID = 34,                      /* T_ID  */
  YYSYMBOL_T_FLT = 35,                     /* T_FLT  */
  YYSYMBOL_T_INT = 36,                     /* T_INT  */
  YYSYMBOL_T_INTEGER = 37,                 /* T_INTEGER  */
  YYSYMBOL_T_FLOAT = 38,                   /* T_FLOAT  */
  YYSYMBOL_YYACCEPT = 39,                  /* $accept  */
  YYSYMBOL_start_program = 40,             /* start_program  */
  YYSYMBOL_prog = 41,                      /* prog  */
  YYSYMBOL_opt_decls = 42,                 /* opt_decls  */
  YYSYMBOL_decl_lst = 43,                  /* decl_lst  */
  YYSYMBOL_decl = 44,                      /* decl  */
  YYSYMBOL_id_list = 45,                   /* id_list  */
  YYSYMBOL_type = 46,                      /* type  */
  YYSYMBOL_stmt = 47,                      /* stmt  */
  YYSYMBOL_opt_stmts = 48,                 /* opt_stmts  */
  YYSYMBOL_stmt_lst = 49,                  /* stmt_lst  */
  YYSYMBOL_expr = 50,                      /* expr  */
  YYSYMBOL_term = 51,                      /* term  */
  YYSYMBOL_factor = 52,                    /* factor  */
  YYSYMBOL_expression = 53                 /* expression  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   85

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  39
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  15
/* YYNRULES -- Number of rules.  */
#define YYNRULES  40
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  95

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   293


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   480,   480,   481,   484,   485,   489,   490,   494,   498,
     499,   503,   504,   508,   509,   510,   511,   512,   513,   514,
     518,   519,   523,   524,   528,   529,   530,   534,   535,   536,
     537,   538,   542,   543,   544,   546,   551,   552,   553,   554,
     555
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "T_COMMA", "T_SEMICOL",
  "T_OPENKEY", "T_CLOSEKEY", "T_OPENBRACK", "T_CLOSEBRACK", "T_OPENPAR",
  "T_CLOSEPAR", "T_PLUSOP", "T_MULTOP", "T_MINUSOP", "T_DIVOP", "T_ASSOP",
  "T_EQSYM", "T_LESSOP", "T_MOREOP", "T_LESSOEQOP", "T_MOREOEQOP",
  "T_DOIF", "T_DOIFELSE", "T_DOWHILE", "T_DOUNTIL", "T_PROGRAM", "T_BEGIN",
  "T_END", "T_EOF", "T_MODOP", "T_POWOP", "T_COLON", "T_READ", "T_PRINT",
  "T_ID", "T_FLT", "T_INT", "T_INTEGER", "T_FLOAT", "$accept",
  "start_program", "prog", "opt_decls", "decl_lst", "decl", "id_list",
  "type", "stmt", "opt_stmts", "stmt_lst", "expr", "term", "factor",
  "expression", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-72)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
       3,     4,    37,   -72,   -17,   -72,   -72,   -72,    22,   -72,
      46,    20,    12,   -17,    23,    49,    50,    51,    52,    28,
      -8,    48,    60,    38,   -72,   -72,    63,   -72,    -8,    -8,
      -8,    -8,   -72,    -8,   -72,   -72,   -72,    -5,    -7,   -72,
      -8,    12,   -72,    23,    36,    57,    58,    59,    61,    14,
      -8,    -8,    -8,    -8,    -8,    -8,    -5,   -72,   -72,    -8,
      -8,    -8,    -8,    -8,    65,    66,    67,    68,   -72,    -7,
      -7,   -72,   -72,   -72,   -72,    -5,    -5,    -5,    -5,    -5,
      12,    12,    12,    12,    62,    69,    70,    71,   -72,    73,
     -72,   -72,    12,    74,   -72
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     2,     5,     1,    12,    11,     0,     4,
       7,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    23,     0,    20,     6,    10,     8,     0,     0,
       0,     0,    19,     0,    33,    35,    34,    18,    26,    31,
       0,     0,     3,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    13,    22,     9,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    32,    24,
      25,    27,    28,    29,    30,    40,    36,    37,    38,    39,
      21,    21,    21,    21,     0,     0,     0,     0,    14,     0,
      17,    16,    21,     0,    15
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -72,   -72,   -72,   -72,    72,   -72,    33,   -72,   -72,   -71,
      40,   -20,   -19,   -38,   -27
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,     3,     8,     9,    10,    27,    11,    22,    23,
      24,    44,    38,    39,    45
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      37,    33,    46,    47,    48,    52,    50,    53,    51,    84,
      85,    86,    87,    49,    71,    72,    73,    74,     6,     7,
      56,    93,    54,    55,    68,    50,    34,    51,     1,    35,
      36,    69,    70,    15,    16,    17,    18,     5,     4,    75,
      76,    77,    78,    79,    19,    20,    21,    50,    12,    51,
      13,    14,    59,    60,    61,    62,    63,    26,    28,    29,
      30,    31,    32,    40,    41,    42,    43,    64,    65,    66,
      88,    67,    80,    81,    82,    83,    58,    89,    90,    91,
      92,    57,    94,     0,     0,    25
};

static const yytype_int8 yycheck[] =
{
      20,     9,    29,    30,    31,    12,    11,    14,    13,    80,
      81,    82,    83,    33,    52,    53,    54,    55,    35,    36,
      40,    92,    29,    30,    10,    11,    34,    13,    25,    37,
      38,    50,    51,    21,    22,    23,    24,     0,    34,    59,
      60,    61,    62,    63,    32,    33,    34,    11,    26,    13,
       4,    31,    16,    17,    18,    19,    20,    34,     9,     9,
       9,     9,    34,    15,     4,    27,     3,    10,    10,    10,
       8,    10,     7,     7,     7,     7,    43,     8,     8,     8,
       7,    41,     8,    -1,    -1,    13
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    25,    40,    41,    34,     0,    35,    36,    42,    43,
      44,    46,    26,     4,    31,    21,    22,    23,    24,    32,
      33,    34,    47,    48,    49,    43,    34,    45,     9,     9,
       9,     9,    34,     9,    34,    37,    38,    50,    51,    52,
      15,     4,    27,     3,    50,    53,    53,    53,    53,    50,
      11,    13,    12,    14,    29,    30,    50,    49,    45,    16,
      17,    18,    19,    20,    10,    10,    10,    10,    10,    51,
      51,    52,    52,    52,    52,    50,    50,    50,    50,    50,
       7,     7,     7,     7,    48,    48,    48,    48,     8,     8,
       8,     8,     7,    48,     8
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    39,    40,    41,    42,    42,    43,    43,    44,    45,
      45,    46,    46,    47,    47,    47,    47,    47,    47,    47,
      48,    48,    49,    49,    50,    50,    50,    51,    51,    51,
      51,    51,    52,    52,    52,    52,    53,    53,    53,    53,
      53
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     6,     1,     0,     3,     1,     3,     3,
       1,     1,     1,     3,     7,    10,     7,     7,     2,     2,
       1,     0,     3,     1,     3,     3,     1,     3,     3,     3,
       3,     1,     3,     1,     1,     1,     3,     3,     3,     3,
       3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* start_program: prog  */
#line 480 "finalAssignment.y"
                        {/*treeSearch($1);*/printf("Dopest code ever!!!!\n");}
#line 1617 "finalAssignment.tab.c"
    break;

  case 3: /* prog: T_PROGRAM T_ID opt_decls T_BEGIN opt_stmts T_END  */
#line 481 "finalAssignment.y"
                                                                           {(yyval.ARST)=newNode(0,(yyvsp[-5].name),"program",0,0,(yyvsp[-3].ARST),(yyvsp[-1].ARST));ht_insert(symbolTable,"0",(yyval.ARST));}
#line 1623 "finalAssignment.tab.c"
    break;

  case 4: /* opt_decls: decl_lst  */
#line 484 "finalAssignment.y"
                                   {(yyval.ARST)=newNode(0,(yyvsp[0].ARST),"decl_lst",0,0,NULL,(yyvsp[0].ARST));ht_insert(symbolTable,"-1",(yyval.ARST));}
#line 1629 "finalAssignment.tab.c"
    break;

  case 5: /* opt_decls: %empty  */
#line 485 "finalAssignment.y"
                                  {}
#line 1635 "finalAssignment.tab.c"
    break;

  case 6: /* decl_lst: decl T_SEMICOL decl_lst  */
#line 489 "finalAssignment.y"
                                                  {(yyval.ARST)=newNode(0,(yyvsp[-2].ARST),"decl_list",0,0,(yyvsp[-2].ARST),(yyvsp[0].ARST));ht_insert(symbolTable,"-3",(yyval.ARST));}
#line 1641 "finalAssignment.tab.c"
    break;

  case 7: /* decl_lst: decl  */
#line 490 "finalAssignment.y"
                                       {(yyval.ARST)=newNode(0,(yyvsp[0].ARST),"decl",0,0,NULL,(yyvsp[0].ARST));ht_insert(symbolTable,"-3",(yyval.ARST));}
#line 1647 "finalAssignment.tab.c"
    break;

  case 8: /* decl: type T_COLON id_list  */
#line 494 "finalAssignment.y"
                                               {(yyval.ARST)=newNode(0,(yyvsp[-2].ARST),"id_list",0,0,(yyvsp[-2].ARST),(yyvsp[0].ARST));ht_insert(symbolTable,"-3.5",(yyval.ARST));}
#line 1653 "finalAssignment.tab.c"
    break;

  case 9: /* id_list: T_ID T_COMMA id_list  */
#line 498 "finalAssignment.y"
                                               {(yyval.ARST)=newNode(0,T_ID,"id_list",0,0,NULL,(yyvsp[0].ARST));ht_insert(symbolTable,"-3.75",(yyval.ARST));}
#line 1659 "finalAssignment.tab.c"
    break;

  case 10: /* id_list: T_ID  */
#line 499 "finalAssignment.y"
                                        {(yyval.ARST)=newNode(2,T_ID,"id",0,0,NULL,NULL);ht_insert(symbolTable,"-3.75",(yyval.ARST));}
#line 1665 "finalAssignment.tab.c"
    break;

  case 11: /* type: T_INT  */
#line 503 "finalAssignment.y"
                        {(yyval.ARST)=newNode(0,T_INT,"type_int",0,0,NULL,NULL);ht_insert(symbolTable,"-6",(yyval.ARST));}
#line 1671 "finalAssignment.tab.c"
    break;

  case 12: /* type: T_FLT  */
#line 504 "finalAssignment.y"
                        {(yyval.ARST)=newNode(0,T_FLT,"type_float",0,0,NULL,NULL);ht_insert(symbolTable,"-6",(yyval.ARST));}
#line 1677 "finalAssignment.tab.c"
    break;

  case 13: /* stmt: T_ID T_ASSOP expr  */
#line 508 "finalAssignment.y"
                                    {workflow((yyval.ARST));(yyval.ARST)=newNode(0,T_ASSOP,"assign",0,0,NULL,(yyvsp[0].ARST));ht_insert(symbolTable,"1.75",(yyval.ARST));}
#line 1683 "finalAssignment.tab.c"
    break;

  case 14: /* stmt: T_DOIF T_OPENPAR expression T_CLOSEPAR T_OPENBRACK opt_stmts T_CLOSEBRACK  */
#line 509 "finalAssignment.y"
                                                                                            {workflow((yyval.ARST));(yyval.ARST)=newNode(0,T_DOIF,"do-if",0,0,(yyvsp[-4].ARST),(yyvsp[-1].ARST));ht_insert(symbolTable,"1.75",(yyval.ARST));}
#line 1689 "finalAssignment.tab.c"
    break;

  case 15: /* stmt: T_DOIFELSE T_OPENPAR expression T_CLOSEPAR T_OPENBRACK opt_stmts T_CLOSEBRACK T_OPENBRACK opt_stmts T_CLOSEBRACK  */
#line 510 "finalAssignment.y"
                                                                                                                                   {(yyval.ARST)=newNode(0,T_DOUNTIL,"do-until",0,0,(yyvsp[-7].ARST),(yyvsp[-4].ARST));workflow((yyval.ARST));ht_insert(symbolTable,"1.75",(yyval.ARST));(yyval.ARST)=newNode(2,T_DOUNTIL,"do-until",0,0,(yyvsp[-4].ARST),(yyvsp[-2].name));ht_insert(symbolTable,"1.75",(yyval.ARST));}
#line 1695 "finalAssignment.tab.c"
    break;

  case 16: /* stmt: T_DOUNTIL T_OPENPAR expression T_CLOSEPAR T_OPENBRACK opt_stmts T_CLOSEBRACK  */
#line 511 "finalAssignment.y"
                                                                                               {workflow((yyval.ARST));(yyval.ARST)=newNode(0,T_DOUNTIL,"do-until",0,0,(yyvsp[-4].ARST),(yyvsp[-1].ARST));ht_insert(symbolTable,"1.75",(yyval.ARST));}
#line 1701 "finalAssignment.tab.c"
    break;

  case 17: /* stmt: T_DOWHILE T_OPENPAR expression T_CLOSEPAR T_OPENBRACK opt_stmts T_CLOSEBRACK  */
#line 512 "finalAssignment.y"
                                                                                               {workflow((yyval.ARST));(yyval.ARST)=newNode(0,T_DOWHILE,"do-while",0,0,(yyvsp[-4].ARST),(yyvsp[-1].ARST));ht_insert(symbolTable,"1.75",(yyval.ARST));}
#line 1707 "finalAssignment.tab.c"
    break;

  case 18: /* stmt: T_PRINT expr  */
#line 513 "finalAssignment.y"
                               {(yyval.ARST)=newNode(0,T_PRINT,"print",0,0,NULL,(yyvsp[0].ARST));ht_insert(symbolTable,"1.75",(yyval.ARST));workflow((yyval.ARST));}
#line 1713 "finalAssignment.tab.c"
    break;

  case 19: /* stmt: T_READ T_ID  */
#line 514 "finalAssignment.y"
                               {(yyval.ARST)=newNode(0,T_READ,"read",0,0,NULL,(yyvsp[0].name));ht_insert(symbolTable,"1.75",(yyval.ARST));workflow((yyval.ARST));}
#line 1719 "finalAssignment.tab.c"
    break;

  case 20: /* opt_stmts: stmt_lst  */
#line 518 "finalAssignment.y"
                                   {(yyval.ARST)=newNode(0,(yyvsp[0].ARST),"opt_stmts",0,0,NULL,(yyvsp[0].ARST));ht_insert(symbolTable,"1",(yyval.ARST));}
#line 1725 "finalAssignment.tab.c"
    break;

  case 21: /* opt_stmts: %empty  */
#line 519 "finalAssignment.y"
                                  {}
#line 1731 "finalAssignment.tab.c"
    break;

  case 22: /* stmt_lst: stmt T_SEMICOL stmt_lst  */
#line 523 "finalAssignment.y"
                                                  {(yyval.ARST)=newNode(0,(yyvsp[-1].name),"stmt_lst",0,0,(yyvsp[-2].ARST),(yyvsp[0].ARST));ht_insert(symbolTable,"2",(yyval.ARST));}
#line 1737 "finalAssignment.tab.c"
    break;

  case 23: /* stmt_lst: stmt  */
#line 524 "finalAssignment.y"
                                       {(yyval.ARST)=newNode(0,(yyvsp[0].ARST),"stmt_lst",0,0,NULL,(yyvsp[0].ARST));ht_insert(symbolTable,"2",(yyval.ARST));}
#line 1743 "finalAssignment.tab.c"
    break;

  case 24: /* expr: expr T_PLUSOP term  */
#line 528 "finalAssignment.y"
                                                {(yyval.ARST)=newNode(0,T_PLUSOP,"addTree",0,0,(yyvsp[-2].ARST),(yyvsp[0].ARST));ht_insert(symbolTable,"1.4375",(yyval.ARST));}
#line 1749 "finalAssignment.tab.c"
    break;

  case 25: /* expr: expr T_MINUSOP term  */
#line 529 "finalAssignment.y"
                                                {(yyval.ARST)=newNode(0,T_PLUSOP,"subTree",0,0,(yyvsp[-2].ARST),(yyvsp[0].ARST));ht_insert(symbolTable,"1.4375",(yyval.ARST));}
#line 1755 "finalAssignment.tab.c"
    break;

  case 26: /* expr: term  */
#line 530 "finalAssignment.y"
                               {(yyval.ARST)=newNode(1,T_PLUSOP,"term",0,0,NULL,(yyvsp[0].ARST));ht_insert(symbolTable,"1.4375",(yyval.ARST));}
#line 1761 "finalAssignment.tab.c"
    break;

  case 27: /* term: term T_MULTOP factor  */
#line 534 "finalAssignment.y"
                                                    {(yyval.ARST)=newNode(0,T_MULTOP,"multTree",0,0,(yyvsp[-2].ARST),(yyvsp[0].ARST));ht_insert(symbolTable,"1.40625",(yyval.ARST));}
#line 1767 "finalAssignment.tab.c"
    break;

  case 28: /* term: term T_DIVOP factor  */
#line 535 "finalAssignment.y"
                                                        {(yyval.ARST)=newNode(0,T_DIVOP,"divTree",0,0,(yyvsp[-2].ARST),(yyvsp[0].ARST));ht_insert(symbolTable,"1.40625",(yyval.ARST));}
#line 1773 "finalAssignment.tab.c"
    break;

  case 29: /* term: term T_MODOP factor  */
#line 536 "finalAssignment.y"
                                                        {(yyval.ARST)=newNode(0,T_POWOP,"modTree",0,0,(yyvsp[-2].ARST),(yyvsp[0].ARST));ht_insert(symbolTable,"1.40625",(yyval.ARST));}
#line 1779 "finalAssignment.tab.c"
    break;

  case 30: /* term: term T_POWOP factor  */
#line 537 "finalAssignment.y"
                                                        {(yyval.ARST)=newNode(0,T_POWOP,"powTree",0,0,(yyvsp[-2].ARST),(yyvsp[0].ARST));ht_insert(symbolTable,"1.40625",(yyval.ARST));}
#line 1785 "finalAssignment.tab.c"
    break;

  case 31: /* term: factor  */
#line 538 "finalAssignment.y"
                                 {(yyval.ARST)=newNode(1,(yyvsp[0].ARST),"factor",0,0,NULL,(yyvsp[0].ARST));ht_insert(symbolTable,"1.40625",(yyval.ARST));}
#line 1791 "finalAssignment.tab.c"
    break;

  case 32: /* factor: T_OPENPAR expr T_CLOSEPAR  */
#line 542 "finalAssignment.y"
                                            {(yyval.ARST)=newNode(1,(yyvsp[-1].ARST),"expr",0,0,NULL,(yyvsp[-1].ARST)); ht_insert(symbolTable,"1.4375",(yyval.ARST));}
#line 1797 "finalAssignment.tab.c"
    break;

  case 33: /* factor: T_ID  */
#line 543 "finalAssignment.y"
                       {ht_insert(symbolTable,"1.4375",newNode(1,T_ID,(yyvsp[0].name),0,0,NULL,NULL));}
#line 1803 "finalAssignment.tab.c"
    break;

  case 34: /* factor: T_FLOAT  */
#line 544 "finalAssignment.y"
                          {(yyval.ARST)=NULL;;newNode(1,T_FLOAT,"float",(yyvsp[0].var_float),(yyvsp[0].var_float),NULL,NULL);
                            ht_insert(symbolTable,"1.4375",(yyval.ARST));}
#line 1810 "finalAssignment.tab.c"
    break;

  case 35: /* factor: T_INTEGER  */
#line 546 "finalAssignment.y"
                            {(yyval.ARST)=NULL;;newNode(1,T_INTEGER,"int",(yyvsp[0].var_int),(yyvsp[0].var_int),NULL,NULL);
                            ht_insert(symbolTable,"1.4375",(yyval.ARST));}
#line 1817 "finalAssignment.tab.c"
    break;

  case 36: /* expression: expr T_LESSOP expr  */
#line 551 "finalAssignment.y"
                                                {(yyval.ARST)=newNode(0,T_LESSOP,"compareTree",0,0,(yyvsp[-2].ARST),(yyvsp[0].ARST));ht_insert(symbolTable,"1.375",(yyval.ARST));}
#line 1823 "finalAssignment.tab.c"
    break;

  case 37: /* expression: expr T_MOREOP expr  */
#line 552 "finalAssignment.y"
                                                        {(yyval.ARST)=newNode(0,T_MOREOP,"compareTree",0,0,(yyvsp[-2].ARST),(yyvsp[0].ARST));ht_insert(symbolTable,"1.375",(yyval.ARST));}
#line 1829 "finalAssignment.tab.c"
    break;

  case 38: /* expression: expr T_LESSOEQOP expr  */
#line 553 "finalAssignment.y"
                                                        {(yyval.ARST)=newNode(0,T_LESSOEQOP,"compareTree",0,0,(yyvsp[-2].ARST),(yyvsp[0].ARST));ht_insert(symbolTable,"1.375",(yyval.ARST));}
#line 1835 "finalAssignment.tab.c"
    break;

  case 39: /* expression: expr T_MOREOEQOP expr  */
#line 554 "finalAssignment.y"
                                                        {(yyval.ARST)=newNode(0,T_LESSOEQOP,"compareTree",0,0,(yyvsp[-2].ARST),(yyvsp[0].ARST));ht_insert(symbolTable,"1.375",(yyval.ARST));}
#line 1841 "finalAssignment.tab.c"
    break;

  case 40: /* expression: expr T_EQSYM expr  */
#line 555 "finalAssignment.y"
                                                    {(yyval.ARST)=newNode(0,T_MOREOEQOP,"compareTree",0,0,(yyvsp[-2].ARST),(yyvsp[0].ARST));ht_insert(symbolTable,"1.375",(yyval.ARST));}
#line 1847 "finalAssignment.tab.c"
    break;


#line 1851 "finalAssignment.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

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
                      yytoken, &yylval);
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 558 "finalAssignment.y"


int workflow(ARST* root){
     ARST* next = treeSearch(root -> right);
    switch(root->type){
       
        case T_ASSOP:
            printf("WF: Working on assignation...\n");
            break;
        case T_DOIF:
            printf("WF: Working on do-if...\n");
            break;
        case T_DOIFELSE:
            printf("WF: Working on do-ifelse...\n");
            break;
        case T_DOUNTIL:
            printf("WF: Working on do-until...\n");
            break;
        case T_DOWHILE:
            printf("WF: Working on do-while...\n");
            break;
        case T_PRINT:
            /*printf("WF: Working on print...\n");
           
            if((next->valFlt)-(int)(next->valFlt)!=0){printf("%d = %f",ht_search(symbolTable,next->token)->token,next->valFlt);}
            else {printf("%d = %d",ht_search(symbolTable,next->token)->token,next->valInt);}
            break;*/

        case T_READ:
            /*printf("WF: Working on read...\n");
                if((next->valFlt)-(int)(next->valFlt)!=0){ht_insert(symbolTable,ht_search(symbolTable,next->token)->token,ht_search(symbolTable,next->token));}
                else {ht_insert(symbolTable,ht_search(symbolTable,next->token)->token,next->valInt);}
            printf("STORED VALUE AT %d",ht_search(symbolTable,next->token)->token);*/
            break;
        default:

    }
    return 0;
}

ARST* treeSearch(ARST * root) {
    ARST * temp1;
    ARST * temp2;
    float newValFloat;
    float newValInt;
    if (root == NULL) return 0;
    if (root -> type == 1) { // Se trata de un operando, hay que recorrer sus subárboles
        temp1 = treeSearch(root -> left);
        temp2 = treeSearch(root -> right);
        // Ya tenemos los valores correspondientes, ahora hay que operar y devolver el
        // valor de la operación.
        if((root->valFlt-(int)(root->valFlt))!=0){
            newValFloat = opFlt(root,temp1,temp2);
            ht_insert(symbolTable,root->token,newNode(root->type,root->token,root->valStr,root->valInt,newValFloat,root->left,root->right));
        }else {
            newValInt = opInt(root,temp1, temp2); 
            ht_insert(symbolTable,root->token,newNode(root->type,root->token,root->valStr,newValInt,root->valFlt,root->left,root->right));  
        }
    }
    return root;
}

int opInt( ARST* root, ARST* newRoot1, ARST* newRoot2){
     int temp1 = (int)(newRoot1->valInt);
     int temp2 =(int)(newRoot2->valInt);
            if (root-> token == T_PLUSOP) return temp1 + temp2;
            if (root-> token == T_MINUSOP) return temp1 - temp2;
            if (root-> token == T_MULTOP) return temp1 * temp2;
            if (root-> token == T_DIVOP) return temp1 / temp2;
            if (root-> token == T_MODOP) return temp1 % temp2;
            if (root-> token == T_POWOP) return potenciaConWhile(temp1, temp2);
            if (root-> token == T_EQSYM) return temp2;
            if (root-> token == T_MOREOP) return temp1 > temp2;
            if (root-> token == T_LESSOP) return temp1 < temp2;
            if (root-> token == T_LESSOEQOP) return temp1 <= temp2;
            if (root-> token == T_MOREOEQOP) return temp1 >= temp2;
            else{
                printf("ERROR: UNDECLARED INT VALUE.");
            }
}

float opFlt(ARST* root,ARST* newRoot1, ARST* newRoot2){
            float temp1 = newRoot1->valFlt;
            float temp2 = newRoot2->valFlt;
            if (root-> token == T_PLUSOP) return temp1 + temp2;
            if (root-> token == T_MINUSOP) return temp1 - temp2;
            if (root-> token == T_MULTOP) return temp1 * temp2;
            if (root-> token == T_DIVOP) return temp1 / temp2;
            if (root-> token == T_MODOP) return 0;
            if (root-> token == T_POWOP) return potenciaConWhile(temp1, (int)temp2);
            if (root-> token == T_EQSYM) return temp2;
            if (root-> token == T_MOREOP) return temp1 > temp2;
            if (root-> token == T_LESSOP) return temp1 < temp2;
            if (root-> token == T_LESSOEQOP) return temp1 <= temp2;
            if (root-> token == T_MOREOEQOP) return temp1 >= temp2;
            else{
                printf("ERROR: UNDECLARED  FLOAT VALUE.");
            }
}

void printARST(ARST * treeNode){
    unsigned char typeP = treeNode->type;       
    int tokenP = treeNode->token;
    char *valStrP = treeNode->valStr;
    int valIntP = treeNode->valInt;
    float valFltP = treeNode->valFlt;
    if(valFltP-(int)valFltP!=0 && typeP == 1){
    	printf("\t%d:'%s'\t\t\t%s\t%f",tokenP,valStrP, "float",valFltP);
    } else if(typeP == 1) {
    	printf("\t%d:'%s'\t\t\t%s\t%d",tokenP,valStrP, "int",valIntP);
    } else{
    	printf("\t%d:'%s'\t\t\t%s\t%s",tokenP,valStrP, "NULL","NULL");
    }    
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void main(int argc, char * argv[]) {  
    extern HashTable* symbolTable; 
    symbolTable = create_table(CAPACITY);
    count=0;
    extern FILE * yyin;
	if(argc<2){
			yyparse();
	} else{
		yyin = fopen(argv[1],"r");
		yyparse();
	}
    
	print_table(symbolTable);
    free_table(symbolTable);
}

