#include <stdio.h>

#include "hotpot/hp_platform.h"
#include "hotpot/hp_error.h"
#include "script_y.h"
#include "hotscript/script_parser.h"
#include "script_l.h"
#include "hotscript/hotlex.h"
	
		
static void hotscript_reserved_keyword(char* keyword)
{
	//yyerror(&yylloc, hotscript_parameter, "Cannot use reserved language keyword: \"%s\"\n", keyword);
}

#define YYCTYPE   unsigned char
#define YYFILL(n) 
#define YYCURSOR  sp->yy_cursor
#define YYLIMIT   sp->yy_limit
#define yytext sp->yy_text
#define yyleng sp->yy_leng
#define YYMARKER sp->yy_marker
#define YYGETCONDITION()  sp->yy_state
#define YYSETCONDITION(s) sp->yy_state = s

#define STATE(name)  yyc##name
#define BEGIN(state) YYSETCONDITION(STATE(state))
#define YYSTATE      YYGETCONDITION()


hpint32 script_lex_scan(SCANNER *sp, YYLTYPE *yylloc, YYSTYPE * yylval)
{
restart:
	if(YYCURSOR >= YYLIMIT)
	{
		return 0;
	}
	yylloc->first_line = sp->yylineno;
	yylloc->first_column = sp->yycolumn;
	sp->yy_text = YYCURSOR;
/*!re2c
re2c:yyfill:check = 0;


identifier		([a-zA-Z_][a-zA-Z_0-9]*)
intconstant		([0-9]+)
file_name		("<"[^>]*">")
comment			("//"[^\n]*)
unixcomment		("#"[^\n]*)
sillycomm		("/*""*"*"*/")
multicomm		("/*"[^*]"/"*([^*/]|[^*]"/"|"*"[^/])*"*"*"*/")
symbol			([@#$\[\]\*\{\}])
newline			("\r"|"\n"|"\r\n")
whitespace		([ \n\r\t]+)
literal_begin	(['\"])
any_char		((.|"\n"))

<!*> := yyleng = YYCURSOR - sp->yy_text; scanner_process(sp);

<ST_IN_SCRIPTING>{comment}				{ goto restart;/* do nothing */																}
<ST_IN_SCRIPTING>{sillycomm}			{ goto restart;/* do nothing */																}
<ST_IN_SCRIPTING>{multicomm}			{ goto restart;/* do nothing */																}
<ST_IN_SCRIPTING>{whitespace}			{ goto restart;/* do nothing */																}



<INITIAL>"<%"								{ BEGIN(ST_IN_SCRIPTING);goto restart;										}
<ST_IN_SCRIPTING>"%>"						{ BEGIN(INITIAL);goto restart;												}
<ST_IN_SCRIPTING>{symbol}					{ 
	return yytext[0];															
	}

<INITIAL>{any_char}		     {
	yylval->text.str_len = 1;
	yylval->text.str[0] = yytext[0];
	while(YYCURSOR < YYLIMIT)
	{
		yylval->text.str[yylval->text.str_len] = *YYCURSOR;
		++YYCURSOR;
		++(yylval->text.str_len);
		if(yylval->text.str_len >= 2)
		{
			if((yylval->text.str[yylval->text.str_len - 2] == '<')
				&& (yylval->text.str[yylval->text.str_len - 1] == '%'))
			{					
				yylval->text.str_len -= 2;
				BEGIN(ST_IN_SCRIPTING);
				break;
			}
		}
	}
	return tok_text;
}

<ST_IN_SCRIPTING>"import"					{ return tok_import;										}


<ST_IN_SCRIPTING>{intconstant}			{
	char number[128];
	memcpy(number, yytext, yyleng);
	number[yyleng] = 0;
	yylval->i32 = strtoll(number, NULL, 10);
	return tok_integer;
}
<ST_IN_SCRIPTING>{identifier}			{ 
	memcpy(yylval->identifier, yytext, yyleng);
	yylval->identifier[yyleng] = 0;
	return tok_identifier;
}


<ST_IN_SCRIPTING>"BEGIN"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"END"                { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"__CLASS__"          { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"__DIR__"            { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"__FILE__"           { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"__FUNCTION__"       { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"__LINE__"           { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"__METHOD__"         { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"__NAMESPACE__"      { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"abstract"           { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"alias"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"and"                { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"args"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"as"                 { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"assert"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"begin"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"break"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"catch"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"class"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"clone"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"continue"           { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"declare"            { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"def"                { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"default"            { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"del"                { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"delete"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"do"                 { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"dynamic"            { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"elif"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"else"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"elseif"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"elsif"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"end"                { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"enddeclare"         { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"endfor"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"endforeach"         { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"endif"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"endswitch"          { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"endwhile"           { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"ensure"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"except"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"exec"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"finally"            { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"float"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"for"                { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"foreach"            { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"function"           { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"global"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"goto"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"if"                 { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"implements"         { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"import"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"in"                 { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"inline"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"instanceof"         { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"interface"          { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"is"                 { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"lambda"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"module"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"native"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"new"                { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"next"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"nil"                { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"not"                { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"or"                 { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"pass"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"public"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"print"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"private"            { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"protected"          { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"public"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"raise"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"redo"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"rescue"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"retry"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"register"           { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"return"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"self"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"sizeof"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"static"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"super"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"synchronized"       { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"then"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"this"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"throw"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"transient"          { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"try"                { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"undef"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"union"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"unless"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"unsigned"           { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"until"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"use"                { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"var"                { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"virtual"            { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"volatile"           { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"when"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"while"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"with"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"xor"                { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"yield"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"list"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"required"           { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"optional"           { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"async"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"service"            { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"throws"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"extends"            { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"exception"          { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"binary"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"include"            { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"bool"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"void"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"byte"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"namespace"          { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"union"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"struct"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"if"				  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"const"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"enum"				  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"case"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"typename"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"unique"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"typedef"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"true"				  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"false"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"lower_bound"		  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"upper_bound"		  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"switch"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"vector"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"string"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"int8"				  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"int16"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"int32"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"int64"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"uint8"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"uint16"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"uint32"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"uint64"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"char"				  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"double"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"=="				  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"!="				  { hotscript_reserved_keyword(yytext); }

<ST_IN_SCRIPTING>{literal_begin} {
  char mark = *yytext;
  while(YYCURSOR < YYLIMIT)
  {
    int ch = *YYCURSOR;
    ++YYCURSOR;    
    switch (ch) {
      case '\\':		
		if(YYCURSOR < YYLIMIT)
		{
			ch = *YYCURSOR;
			++YYCURSOR;
			switch (ch) {
			  case 'r':
				yylval->literal.str[(yylval->literal.str_len)++] = '\r';
				continue;
			  case 'n':
				yylval->literal.str[(yylval->literal.str_len)++] = '\n';
				continue;
			  case 't':
				yylval->literal.str[(yylval->literal.str_len)++] = '\t';
				continue;
			  case '"':
				yylval->literal.str[(yylval->literal.str_len)++] = '"';
				continue;
			  case '\'':
				yylval->literal.str[(yylval->literal.str_len)++] = '\'';
				continue;
			  case '\\':
				yylval->literal.str[(yylval->literal.str_len)++] = '\\';
				continue;
			  default:
				//yyerror("bad escape character\n");
				return -1;
			}
        }
        break;
      default:
        if (ch == mark)
        {
			return tok_literal;
        }
		else
		{
          yylval->literal.str[(yylval->literal.str_len)++] = ch;
        }
    }
  }
}

<ST_IN_SCRIPTING>{newline}			     {goto restart;/* do nothing */																}
<ST_IN_SCRIPTING>{any_char}			     {goto restart;/* reutrn error?*/																}
*/
}
