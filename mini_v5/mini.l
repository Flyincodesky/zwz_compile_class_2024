%{
#include <stdlib.h>
#include "tac.h"
#include "mini.y.h"

%}

%option yylineno

%%

"int"  {  
	yylval.var_type=1;
	return INT;  }

"char" {  
	yylval.var_type=2;
	return CHAR;}

"print"  {  return PRINT;  }

"return"  {  return RETURN;  }

"continue"  {  return CONTINUE;  }

"if"  {  return IF;  }

"then"  {  return THEN;  }

"else"  {  return ELSE;  }

"fi"  {  return FI;  }

"while"  {  return WHILE;  }

"for" { return FOR; }

"do"  {  return DO;  }

"done"  {  return DONE;  }

"break" { return BREAK; }

[A-Za-z]([A-Za-z]|[0-9])*  {  
	yylval.string = strdup(yytext); 
	return IDENTIFIER;
}

[0-9]*	{
	yylval.string = strdup(yytext); 
	return INTEGER;
}

\"[^\"]*\"  {
	yylval.string = strdup(yytext); 
	return TEXT;
}

\'[^\']\'  {
	yylval.character = yytext[1];
	return CHARACTER;
}

"=="  {  return EQ;  }

"!="  {  return NE;  }

"<="  {  return LE;  }

"<"  {  return LT;  }

">="  {  return GE;  }

">"  {  return GT;  }

[ \t\r\n]|#.*

. 	{	return *yytext; }

%%

int yywrap()
{
	return 1;
}

