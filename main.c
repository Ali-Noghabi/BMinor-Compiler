#include <ctype.h>
#include <stdio.h>
int sum(int a , int b)
{
    return a+b;
}
enum Type {
    keyWord,
    ID,
    Integer,
    Operator,
    Delimiter
};
const char* keyWords[] = { "array", "boolean", "char", "else", "false", "for", "function", "if",
								"integer", "print", "return", "string", "true", "void", "while"};
int line = 1;
const char operators[] = {'+' , '-' ,'!' ,'*' , '/' , '%' ,'=' ,'|' ,'^' , '<' , '>' , '&' };
const char delimeters[]= {'(' , ')' , '[' , ']' , '{' , '}' , ',' , ';' , ':' , '\'' , '"' , '\\'};
struct Token{
    enum Type type ;
    char* value;
    int line ;
};
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
int isItValidID(char * word){
    if(strlen(word) > 256 ){
        /// error
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
        }else{
            /// error
        }
    }else{
        if(isalpha(word[0]) || word[0] == '_'){
            if(isItValidID(word)){
                struct Token token ;
                token.type = ID;
                token.line = line;
                token.value = (char *) malloc(strlen(word) + 1);
                strcpy(token.value , word);

            }else{
                /// error
            }
        }
        else{
            /// error
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
int isDelimeters(char c){
    for(int i =0 ; i < sizeof(delimeters) / sizeof(char) ; i++){
        if(c == delimeters[i]){
            return 1;
        }
    }
    return 0;
}
void stringReader(FILE* file){
}
void operatorReader(char c , FILE* file){
}
void lexical_analysis(FILE* file){
    char c;
    char* str =NULL;
    while((c = fgetc(file)) != EOF){
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
                token.value = c;
                }
                continue;
            }
            if(isOperator(c)){
                if(str != NULL){
                    recongnizer(str);
                    str = NULL;
                    }
                operatorReader(c , file);
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
    FILE * file = fopen("input.txt" , "r+");
    lexical_analysis(file);
    return 0 ;
}

