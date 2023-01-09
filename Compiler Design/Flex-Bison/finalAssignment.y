/* Este archivo contiene un reconocedor sintáctico.

   Para compilar y ejecutar:
   flex finalAssignment.l; bison -d finalAssignment.y; gcc lex.yy.c finalAssignment.tab.c -lfl; ./a.out <filename>

   ejemplo:
   flex finalAssignment.l; bison -d finalAssignment.y; gcc lex.yy.c finalAssignment.tab.c -lfl; ./a.out testCase.txt
   Autor: Nezih Nieto Gutiérrez - Dafne Linette Badillo Campuzano - Arturo Montes de Oca Barrios*/

%{
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
    int workflow(ARST*);

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




%}

/* Los elementos terminales de la gramática. La declaración de abajo se
   convierte en definición de constantes en el archivo Assignment6.tab.h
   que hay que incluir en el archivo de flex. */

%union{
   struct arst * ARST;
   int name;
   int var_int;
   float var_float;
}
%token <name> T_COMMA T_SEMICOL T_OPENKEY T_CLOSEKEY T_OPENBRACK T_CLOSEBRACK T_OPENPAR T_CLOSEPAR T_PLUSOP T_MULTOP T_MINUSOP T_DIVOP T_ASSOP T_EQSYM T_LESSOP T_MOREOP T_LESSOEQOP T_MOREOEQOP T_DOIF T_DOIFELSE T_DOWHILE T_DOUNTIL T_PROGRAM T_BEGIN T_END T_EOF T_MODOP T_POWOP T_COLON T_READ T_PRINT T_ID T_FLT T_INT
%token <var_int> T_INTEGER 
%token <var_float> T_FLOAT
%type <ARST> prog expr term factor expression opt_decls decl_lst decl id_list type opt_stmts stmt_lst stmt

%start start_program


%% 

    start_program: prog {treeSearch($1);printf("Dopest code ever!!!!\n");}

	prog 		: T_PROGRAM T_ID opt_decls T_BEGIN opt_stmts T_END {$$=newNode(0,T_PROGRAM,T_ID,0,0,$3,$5);ht_insert(symbolTable,"0",$$);}
	;

	opt_decls 	: decl_lst {$$=newNode(0,$1,"decl_lst",0,0,NULL,$1);ht_insert(symbolTable,"-1",$$);}
				| {}
				//{printf("End of opt_decls\n");}
	;

	decl_lst 	: decl T_SEMICOL decl_lst {$$=newNode(0,$1,"decl_list",0,0,$1,$3);ht_insert(symbolTable,"-3",$$);}
				| decl {$$=newNode(0,$1,"decl",0,0,NULL,$1);ht_insert(symbolTable,"-3",$$);}
				//{printf("End of decl_lst\n");}
	;

	decl 		: type T_COLON id_list {$$=newNode(0,$1,"id_list",0,0,$1,$3);ht_insert(symbolTable,"-3.5",$$);}
				//{printf("End of decl\n");}
	;
	
	id_list 	: T_ID T_COMMA id_list {printf("%s",T_ID);$$=newNode(0,T_COMMA,T_ID,0,0,NULL,$3);ht_insert(symbolTable,$1,$$);}
				| T_ID  {$$=newNode(2,T_ID,"id",0,0,NULL,NULL);ht_insert(symbolTable,"-3.75",$$);}
				//{printf("End of id_list\n");}
	;

    type        : T_INT {$$=newNode(0,T_INT,"type_int",0,0,NULL,NULL);ht_insert(symbolTable,"-6",$$);}
                | T_FLT {$$=newNode(0,T_FLT,"type_float",0,0,NULL,NULL);ht_insert(symbolTable,"-6",$$);}
				//{printf("End of type\n");}
	;

    stmt        : T_ID T_ASSOP expr {$$=newNode(0,T_ASSOP,"assignation",0,0,NULL,$3);ht_insert(symbolTable,$1,$$);}
                | T_DOIF T_OPENPAR expression T_CLOSEPAR T_OPENBRACK opt_stmts T_CLOSEBRACK {$$=newNode(0,T_DOIF,"do-if",0,0,$3,$6);ht_insert(symbolTable,"1.75",$$);}
                | T_DOIFELSE T_OPENPAR expression T_CLOSEPAR T_OPENBRACK opt_stmts T_CLOSEBRACK T_OPENBRACK opt_stmts T_CLOSEBRACK {$$=newNode(0,T_DOUNTIL,"do-until",0,0,$3,$6);ht_insert(symbolTable,"1.75",$$);$$=newNode(2,T_DOUNTIL,"do-until",0,0,$6,$8);ht_insert(symbolTable,"1.75",$$);}
                | T_DOUNTIL T_OPENPAR expression T_CLOSEPAR T_OPENBRACK opt_stmts T_CLOSEBRACK {$$=newNode(0,T_DOUNTIL,"do-until",0,0,$3,$6);ht_insert(symbolTable,"1.75",$$);}
                | T_DOWHILE T_OPENPAR expression T_CLOSEPAR T_OPENBRACK opt_stmts T_CLOSEBRACK {$$=newNode(0,T_DOWHILE,"do-while",0,0,$3,$6);ht_insert(symbolTable,"1.75",$$);}
                | T_PRINT expr {$$=newNode(0,T_PRINT,"print",0,0,NULL,$2);ht_insert(symbolTable,"1.75",$$);}
                | T_READ T_ID  {$$=newNode(0,T_READ,"read",0,0,NULL,$2);ht_insert(symbolTable,"1.75",$$);}
				//{printf("End of stmt\n");}
	;
	
	opt_stmts 	: stmt_lst {$$=newNode(0,$1,"opt_stmts",0,0,NULL,$1);ht_insert(symbolTable,"1",$$);}
				| {}
				//{printf("End of opt_stmts\n");}
	;
	
	stmt_lst 	: stmt T_SEMICOL stmt_lst {$$=newNode(0,$2,"stmt_lst",0,0,$1,$3);ht_insert(symbolTable,"2",$$);}
				| stmt {$$=newNode(0,$1,"stmt_lst",0,0,NULL,$1);ht_insert(symbolTable,"2",$$);}
				//{printf("End of stmt_lst\n");}
	;
	
	expr	 	: expr T_PLUSOP term    {$$=newNode(0,T_PLUSOP,"addTree",0,0,$1,$3);ht_insert(symbolTable,"1.4375",$$);}
	     		| expr T_MINUSOP term   {$$=newNode(0,T_PLUSOP,"subTree",0,0,$1,$3);ht_insert(symbolTable,"1.4375",$$);}
	     		| term {$$=newNode(1,T_PLUSOP,"term",0,0,NULL,$1);ht_insert(symbolTable,"1.4375",$$);}
	     		//{printf("End of expr\n");}
	;

	term 		: term T_MULTOP factor      {$$=newNode(0,T_MULTOP,"multTree",0,0,$1,$3);ht_insert(symbolTable,"1.40625",$$);}
	     		| term T_DIVOP factor		{$$=newNode(0,T_DIVOP,"divTree",0,0,$1,$3);ht_insert(symbolTable,"1.40625",$$);}
	     		| term T_MODOP factor 		{$$=newNode(0,T_POWOP,"modTree",0,0,$1,$3);ht_insert(symbolTable,"1.40625",$$);}
	     		| term T_POWOP factor 		{$$=newNode(0,T_POWOP,"powTree",0,0,$1,$3);ht_insert(symbolTable,"1.40625",$$);}
	     		| factor {$$=newNode(1,$1,"factor",0,0,NULL,$1);ht_insert(symbolTable,"1.40625",$$);}
	     		//{printf("End of term\n");}
	;

    factor      : T_OPENPAR expr T_CLOSEPAR {$$=newNode(1,$2,"expr",0,0,NULL,$2); ht_insert(symbolTable,"1.4375",$$);}
                | T_ID {ht_insert(symbolTable,"1.4375",newNode(1,T_ID,$1,0,0,NULL,NULL));}
                | T_FLOAT {$$=NULL;;newNode(1,T_FLOAT,"float",$1,$1,NULL,NULL);
                            ht_insert(symbolTable,"1.4375",$$);}
                | T_INTEGER {$$=NULL;;newNode(1,T_INTEGER,"int",$1,$1,NULL,NULL);
                            ht_insert(symbolTable,"1.4375",$$);}
                //{printf("End of factor\n");}
	;
	   
	expression 	: expr T_LESSOP expr	{$$=newNode(0,T_LESSOP,"compareTree",0,0,$1,$3);ht_insert(symbolTable,"1.375",$$);}
		   		| expr T_MOREOP expr    {$$=newNode(0,T_MOREOP,"compareTree",0,0,$1,$3);ht_insert(symbolTable,"1.375",$$);}
		   		| expr T_LESSOEQOP expr {$$=newNode(0,T_LESSOEQOP,"compareTree",0,0,$1,$3);ht_insert(symbolTable,"1.375",$$);}
		   		| expr T_MOREOEQOP expr {$$=newNode(0,T_LESSOEQOP,"compareTree",0,0,$1,$3);ht_insert(symbolTable,"1.375",$$);}
		   		| expr T_EQSYM expr {$$=newNode(0,T_MOREOEQOP,"compareTree",0,0,$1,$3);ht_insert(symbolTable,"1.375",$$);}
		   		//{printf("End of factor\n");}
	;
%%

int workflow(ARST* root){
     ARST* next = treeSearch(root -> right);
    switch(root->token){
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
            printf("WF: Working on print...\n");           
            if((next->valFlt)-(int)(next->valFlt)!=0){printf("%d = %f",ht_search(symbolTable,next->token)->token,next->valFlt);}
            else {printf("%d = %d",ht_search(symbolTable,next->token)->token,next->valInt);}
            break;
        case T_READ:
            printf("WF: Working on read...\n");
                if((next->valFlt)-(int)(next->valFlt)!=0){ht_insert(symbolTable,ht_search(symbolTable,next->token)->token,ht_search(symbolTable,next->token));}
                else {ht_insert(symbolTable,ht_search(symbolTable,next->token)->token,next->valInt);}
            printf("STORED VALUE AT %d",ht_search(symbolTable,next->token)->token);
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
    char key=root->valStr;
    if (root == NULL) return 0;
    if (root -> type == 1) { // Se trata de un operando, hay que recorrer sus subárboles
        temp1 = treeSearch(root -> left);
        temp2 = treeSearch(root -> right);
        // Ya tenemos los valores correspondientes, ahora hay que operar y devolver el
        // valor de la operación.
        if((root->valFlt-(int)(root->valFlt))!=0){
            newValFloat = opFlt(root,temp1,temp2);
            ht_insert(symbolTable,key,newNode(root->type,root->token,root->valStr,root->valInt,newValFloat,root->left,root->right));
        }else {
            newValInt = opInt(root,temp1, temp2); 
            ht_insert(symbolTable,key,newNode(root->type,root->token,root->valStr,newValInt,root->valFlt,root->left,root->right));  
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
                workflow(root);
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
                workflow(root);
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

