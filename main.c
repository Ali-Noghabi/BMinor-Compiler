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
const char operators[] = {'+' , '-' ,'!' ,'*' , '/' , '%' ,'=' ,'|' ,'^' , '<' , '>' , '&' };
const char delimeters[]= {'(' , ')' , '[' , ']' , '{' , '}' , ',' , ';' , ':' , '\'' , '"' , '\\'};
struct Token{
    enum Type type ;
    char* value;
    int line ;
};
void recongnizer(char* word){
}
int isOperator(char c) {

}
int isDelimeters(char c){
}
void stringReader(FILE* file){
}
void operatorReader(char c , FILE* file){
}
void lexical_analysis(FILE* file){
    int line = 1;
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

