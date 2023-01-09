/* Archivo con el reconocedor léxico para la calculadora */
%{
	#include	<stdio.h>
	#include	<stdlib.h>
	#include	<math.h>
	#include	"finalAssignment.tab.h"
	void asignar(char * n);
	int line = 1; 		/* line counter */
	int order = 1; 	/* identation order */
	int reader;
%}
  
/*** Rule Section has three rules, first rule matches with capital letters, second rule matches with any character except newline and  third rule does not take input after the enter ***/
digit 		[0-9]
integer 	{digit}+
float 		({digit}+[.]{digit}*)|({digit}*[.]{digit}+)
id   		(([aA-zZ])[a-z0-9]*)+([A-Z][a-z0-9]*)*
%%
"float"			{return T_FLT;} 
"int"   		{return T_INT;} 
","				{return T_COMMA;}
";"				{return T_SEMICOL;}
"{"				{return T_OPENKEY;}
"}"				{return T_CLOSEKEY;}
"["				{return T_OPENBRACK;}
"]"				{return T_CLOSEBRACK;}
"("				{return T_OPENPAR;}
")"				{return T_CLOSEPAR;}
"+"				{return T_PLUSOP;}
"*"				{return T_MULTOP;}
"-"				{return T_MINUSOP;}
"/"				{return T_DIVOP;}
":="			{return T_ASSOP;}
"="				{return T_EQSYM;}
"<"				{return T_LESSOP;}
">"				{return T_MOREOP;}
"=<"|"<="		{return T_LESSOEQOP;}
"=>"|">="		{return T_MOREOEQOP;}
"do-if" 		{return T_DOIF;}	
"do-ifelse"		{return T_DOIFELSE;}
"do-while" 		{return T_DOWHILE;}
"do-until" 		{return T_DOUNTIL;}
"program"		{return T_PROGRAM;}
"begin"			{return T_BEGIN;}
"end"			{return T_END;}
"%"				{return T_MODOP;}
"^"				{return T_POWOP;}
":"				{return T_COLON;}
"read"			{return T_READ;}
"print"			{return T_PRINT;}
"<<EOF>>"		{return T_END;}
"\n"			{line++;order=1;}
"\t"|" "		{order++;}	
{float} 		{yylval.var_float = atof(yytext);return T_FLOAT;}
{integer}		{yylval.var_int = atoi(yytext);return T_INTEGER;}
{id}*			{yylval.name = (char *) calloc(strlen(yytext) + 1, sizeof(char)); strcpy(yylval.name, yytext);return T_ID;}
.
%%
