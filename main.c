#include <ctype.h>
#include <stdio.h>
enum Type {
    keyWord,
    ID,
    Integer,
    Operator,
    Delimiter,
    String
};
const char* keyWords[] = { "array", "boolean", "char", "else", "false", "for", "function", "if",
								"integer", "print", "return", "string", "true", "void", "while"};
int line = 1;
const char operators[] = {'+' , '-' ,'!' ,'*' , '/' , '%' ,'=' ,'|' ,'^' , '<' , '>' , '&' };
const char* operators2[] = {"++" , "--" , "!=" , "==" , "<=" , ">=" , "||" , "&&"};
const char delimeters[]= {'(' , ')' , '[' , ']' , '{' , '}' , ',' , ';' , ':' , '\'' , '"' , '\\'};
struct Token{
    enum Type type ;
    char* value;
    int line ;
};
void printToken(struct Token token){
    printf("%s   ,   %s" , token.value , token.type);

}
int  isItValidInteger(char * word){
    /// be smaller than  Max-64-bit integer
    for(int i=0; i<strlen(word) ; i++){
        if(isdigit(word[i])){
            continue;
        }
        return 0;
    }
    return 1;
}
int isItKeyWord(char* word){
    for(int i=0 ; i<15 ; i++){
        if(strcmp(word , keyWords[i])){
            return 1;
        }
    }
    return 0;
}
int isItValidID(char * word){
    if(strlen(word) > 256 ){
        /// ERROR
        return;
    }
    for(int i=1; i<strlen(word) ; i++){
        if(isdigit(word[i]) || isalpha(word[i]) || word[i] == '_'){
            continue;
        }
        return 0;
    }
    return 1;
}
void recongnizer(char* word){
    if(isdigit(word[0])){
        if(isItValidInteger(word)){
            struct Token token ;
            token.type = Integer;
            token.line = line;
            token.value = (char *) malloc(strlen(word) + 1);
            strcpy(token.value , word);
            printToken(token);
        }else{
            /// ERROR
        }
    }else{
        if(isalpha(word[0]) || word[0] == '_'){
            if(isItKeyWord(word)){
                struct Token token ;
                token.type = keyWord;
                token.line = line;
                token.value = (char *) malloc(strlen(word) + 1);
                strcpy(token.value , word);
                printToken(token);
                return;
                }
            if(isItValidID(word)){
                struct Token token ;
                token.type = ID;
                token.line = line;
                token.value = (char *) malloc(strlen(word) + 1);
                strcpy(token.value , word);
                printToken(token);

            }else{
                /// ERROR
            }
        }
        else{
            /// ERROR
        }
    }

}
int isOperator(char c) {
    for(int i =0 ; i < sizeof(operators) / sizeof(char) ; i++){
        if(c == operators[i]){
            return 1;
        }
    }
    return 0;
}
int isOperator2(char* c){
    for(int i=0 ; i<8 ; i++){
        if(strcmp(c , operators2[i])){
            return 1;
        }
    }
    return 0;

}
int isDelimeters(char c){
    for(int i =0 ; i < sizeof(delimeters) / sizeof(char) ; i++){
        if(c == delimeters[i]){
            return 1;
        }
    }
    return 0;
}
void stringReader(FILE* file){
    char* string = (char*) malloc(1);
    string[0] = '/0';
    char c;
    while((c = fgetc(file)) != '"'){
        if( c == EOF){
            /// ERROR
            return;
        }
        if(c == '\n'){
            line++;
        }
        int n = strlen(string);
        string = (char *) realloc (string , n + 2);
        string[n+1] = '\0';
        string[n] = c;
    }
    if(strlen(string)> 256){
        /// ERROR
        return;
    }
    struct Token token ;
    token.type = String;
    token.line = line;
    token.value = (char *) malloc(strlen(string) + 1);
    strcpy(token.value , string);
    printToken(token);

}
void comment(int i , FILE* file){
    /// i = 1  /* comment */
    /// i = 2  // comment
}
char  operatorReader(char c , FILE* file){
    char c2 = fgetc(file);
    char* cc2 = (char *) malloc(3);
    cc2[0] = c;
    cc2[1] = c2;
    cc2[2] ='\0';
    if(strcmp(cc2 , "/*")){
        comment(1 , file);
        return NULL;
    }
    if(strcmp(cc2 , "//")){
        comment(2 , file);
        return NULL;
    }
    if(isOperator2(cc2)){
        struct Token token ;
        token.type = Operator;
        token.line = line;
        token.value = (char *) malloc(3);
        token.value[0] = c;
        token.value[1] = c2;
        token.value[1] = '\0';
        printToken(token);
        return NULL;
    }
    struct Token token ;
    token.type = Operator;
    token.line = line;
    token.value = (char *) malloc(2);
    token.value[0] = c;
    token.value[1] = '\0';
    printToken(token);
    return c2;
}
void lexical_analysis(FILE* file){
    char c;
    char b =NULL;
    char* str =NULL;
    while(1){
        if(b == NULL){
            c = fgetc(file);
        }else{
            c = b;
            b = NULL;
        }
        if(c == EOF){
            break;
        }

        if(isspace(c)){
            if(str != NULL){
               recongnizer(str);
               str = NULL;
            }
            if(c == '\n'){
                line++;
            }
            continue;
          }
          if(isDelimeters(c)){
            if(str != NULL){
                recongnizer(str);
                str = NULL;
            }
            if(c == '"'){
                stringReader(file);
            }else{
                struct Token token ;
                token.type = Delimiter;
                token.line = line;
                token.value = (char *) malloc(2);
                token.value[0] = c;
                token.value[1] = '\0';
                printToken(token);
                }
                continue;
            }
            if(isOperator(c)){
                if(str != NULL){
                    recongnizer(str);
                    str = NULL;
                    }
                b = operatorReader(c , file);
                continue;
            }
            if(str == NULL){
                str = (char *) malloc(2);
                str[0] = c;
                str[1] = '\0';
            }else{
                int n = strlen(str);
                str = (char *) realloc (str , n + 2);
                str[n+1] = '\0';
                str[n] = c;

            }

        }
}
int main()
{
    FILE * file = fopen("C:\input.txt" , "r+");
    lexical_analysis(file);
    return 0 ;
}

