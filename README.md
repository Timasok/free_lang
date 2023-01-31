How to execute project?

1. Place source file into folder ./timasok_sources
2. In Terminal enter name of program - ./free_lang input_file output_file(with extention asm)

How does this programming language work?

1. First of all you need to write your program into a source file.
My language requires following constructions:
    -func declaration: 
        def main(to define variables that this function requires) { function's body } ;
    
    -function's body is a sequence of simple lines(with one-step operations such as assignments, funccalls, and their modifications)
    and logical blocks(while, if-else). Both this constructions must end up with a semi colon(;)
        expression ; logical-block ;

    -assignments and declarations: 
        x = 5 ; x = x + 2 ; x = y ;
    
    -funccalls (with our without variables, with or without return value):
        x = foo ( x , y ) ; foo ( 7 ) ;

    -funccalls of funcs declared beforehand(print, print_line, input). 
    Result of this funccalls can't be assigned to any variable:
        print ( x , 4 ) ;     - print a variable or a number, can print multiple values 
        input ( a , b ) ;     - assigns entered values to variables, written below even if they weren't declared or use before
        print_line ( HELLO , BUDDY ) ; - prints line that is name of variable

    -logical constructions (while, if, if-else) - key_word(condition) { block body } ;
        if ( x > 1 ) { print_line ( STAYING , UNDERGROUND ) ; } ;
        if ( 15 ) { print_line ( FLY , AWAY ) ; } else { print ( 47 ) ; } ;
        while ( x < 19 ) { x = x + 1 ; print ( x ) ;}

    -condition is an expression with comparasions ( > , < , <= , >= , == ) or an arithmetic expression 
    that considers false if it's result equals 0. You can use undefined variable here but it value will automaticaly equal 0:
       ( x > 5 ) ; ( x > 9 - y ) ; ( x - 47 ) ;       

2. It initially reads input file and creates an array of tokens.
    What a token is?
    -Token is a small piece on which the whole program can be divided. I have this particular types of tokens: 
    -key_word, variable, operator, number, separator; This tokens can come around in the source file only in following order: 
    types before -> type -> types after. (This rules come from allowed grammatical constructions)
        separator             -> key_word -> separator
        separator or operator -> variable -> separator or operator
        operator              -> number   -> separator or operator
3. Then a special module called syntax analysator creates tree, based on array of tokens and grammatics - rules of tree-building. Here I'll show you my grammar.

NT - nextToken
CT - currentToken


getGeneral        ::= getLangTree assert(tokensend)
    Creates main empty node and calls function that handles program body - getLangTree

getLangTree       ::= l-getAssignment, r-nullptr ?((tokenEnd == false) -> right = createEmpty,getLangTree(right)) linktoPredecessor
    Is a recursive function that calls itself if program body consist next line

getAssignment     ::= if(NT == '='){
    getVariable, getFunc || getExpression, create Assignment node, connect and return
}else{
    KEYWORD || FUNCCALL || EXPRESSION
} 
    Handles assignment - operation with highest priority. It's visibility zone is 2 tokens from the last line's separator - semicolon. As long as every line(program piece separated with semicolon) has at least expression and semicolon itself there is no need to check it.

getKeyWord        ::= DEF?->getDef else-> create node condition-getExpression block-getBlock(for if special function getIfElseBlocks)

getExpression     ::=
getComparison     ::=
getTerm           ::=
getDegree         ::=
getStaples        ::=
getVariable       ::=
getUnarOperation  ::=
getNumber         ::=
getBlock          ::=
getFuncDefinition ::=
getArgumentsNames ::=
getFunc           ::=
getArguments      ::=
getIfElseBlocks   ::=

4.

5.