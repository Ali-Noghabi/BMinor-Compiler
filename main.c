#include <ctype.h>
#include <stdio.h>

/*
Structures and enums
*/
enum Type
{
    keyWord,
    ID,
    Integer,
    Operator,
    Delimiter,
    String
};

struct Token
{
    enum Type type;
    char *value;
    int line;
};

struct symbolTableElement
{
    struct Token token;
    int id;
};

/*
Functions prototype
*/
void lexical_analysis(FILE *file);
void insertElement(struct Token token);
int isTokenExists(char *value);
char *getTokenTypeString(enum Type type);
void printToken(struct Token token);
int isItValidInteger(char *word);
int isItKeyWord(char *word);
int isItValidID(char *word);
void recongnizer(char *word);
int isOperator(char c);
int isDoubleOperator(char *c);
int isDelimeters(char c);
void stringReader(FILE *file);
void comment(int i, FILE *file);
char operatorReader(char c, FILE *file);

const char *keyWords[] = {"array", "boolean", "char", "else", "false", "for", "function", "if",
                          "integer", "print", "return", "string", "true", "void", "while"};
const char operators[] = {'+', '-', '!', '*', '/', '%', '=', '|', '^', '<', '>', '&'};
const char *doubleOperators[] = {"++", "--", "!=", "==", "<=", ">=", "||", "&&"};
const char delimeters[] = {'(', ')', '[', ']', '{', '}', ',', ';', ':', '\'', '"', '\\'};
struct symbolTableElement *symbolTable;
int tableMaxSize = 100;
int currentSize = 1;
int line = 1;
int isFirst = 1;

void insertElement(struct Token token)
{
    if (currentSize >= tableMaxSize)
    {
        // If the array is full, double its size using realloc
        tableMaxSize *= 2;
        symbolTable = (struct symbolTableElement *)realloc(symbolTable, tableMaxSize * sizeof(struct symbolTableElement));
        if (symbolTable == NULL)
        {
            fprintf(stderr, "Memory reallocation failed\n");
            exit(1);
        }
    }

    symbolTable[currentSize].token = token;
    symbolTable[currentSize].id = currentSize;
    currentSize++;
}

int isTokenExists(char *value)
{
    for (int i = 0; i < currentSize; ++i)
    {
        if (strcmp(symbolTable[i].token.value, value) == 0)
        {
            return symbolTable[i].id;
        }
    }
    return 0;
}

int main()
{
    symbolTable = (struct symbolTableElement *)malloc(tableMaxSize * sizeof(struct symbolTableElement));
    FILE *file = fopen("..\\input.txt", "r+");
    lexical_analysis(file);
    return 0;
}
char *getTokenTypeString(enum Type type)
{
    switch (type)
    {
    case keyWord:
        return "keyWord";
    case ID:
        return "ID";
    case Integer:
        return "Integer";
    case Operator:
        return "Operator";
    case Delimiter:
        return "Delimiter";
    case String:
        return "String";
    }
}

void printToken(struct Token token)
{

    if (isFirst)
    {
        isFirst = 0;
        printf("%-20s %-20s %-20s\n------------------------------------------------------\n", "token", "token_type", "token_value");

        FILE *outputFile;
        outputFile = fopen("..\\output.txt", "w");
        if (outputFile != NULL)
        {
            fprintf(outputFile, "%-20s %-20s %-20s\n------------------------------------------------------\n", "token", "token_type", "token_value");
        }
        fclose(outputFile);
    }

    char tokenValue[30];
    int result = isTokenExists(token.value);
    if (result)
    {
        sprintf(tokenValue, "%d", result);
    }
    else
    {
        strcpy(tokenValue, token.value);
    }
    printf("%-20s %-20s %-20s\n", token.value, getTokenTypeString(token.type), tokenValue);

    FILE *outputFile;
    outputFile = fopen("..\\output.txt", "a");
    if (outputFile != NULL)
    {
        fprintf(outputFile, "%-20s %-20s %-20s\n", token.value, getTokenTypeString(token.type), tokenValue);
    }
    fclose(outputFile);
}

int isItValidInteger(char *word)
{
    /// be smaller than  Max-64-bit integer
    for (int i = 0; i < strlen(word); i++)
    {
        if (isdigit(word[i]))
        {
            continue;
        }
        return 0;
    }
    return 1;
}

int isItKeyWord(char *word)
{
    for (int i = 0; i < 15; i++)
    {
        if (!strcmp(word, keyWords[i]))
        {
            return 1;
        }
    }
    return 0;
}

int isItValidID(char *word)
{
    if (strlen(word) > 256)
    {
        /// ERROR
        return;
    }
    for (int i = 1; i < strlen(word); i++)
    {
        if (isdigit(word[i]) || isalpha(word[i]) || word[i] == '_')
        {
            continue;
        }
        return 0;
    }
    return 1;
}

void recongnizer(char *word)
{
    if (isdigit(word[0]))
    {
        if (isItValidInteger(word))
        {
            struct Token token;
            token.type = Integer;
            token.line = line;
            token.value = (char *)malloc(strlen(word) + 1);
            strcpy(token.value, word);
            printToken(token);
        }
        else
        {
            /// ERROR
        }
    }
    else
    {
        if (isalpha(word[0]) || word[0] == '_')
        {
            if (isItKeyWord(word))
            {
                struct Token token;
                token.type = keyWord;
                token.line = line;
                token.value = (char *)malloc(strlen(word) + 1);
                strcpy(token.value, word);
                printToken(token);
                return;
            }
            if (isItValidID(word))
            {
                struct Token token;
                token.type = ID;
                token.line = line;
                token.value = (char *)malloc(strlen(word) + 1);
                if (!isTokenExists(token.value))
                {
                    insertElement(token);
                }
                strcpy(token.value, word);
                printToken(token);
            }
            else
            {
                /// ERROR
            }
        }
        else
        {
            /// ERROR
        }
    }
}

int isOperator(char c)
{
    for (int i = 0; i < sizeof(operators) / sizeof(char); i++)
    {
        if (c == operators[i])
        {
            return 1;
        }
    }
    return 0;
}

int isDoubleOperator(char *c)
{
    for (int i = 0; i < 8; i++)
    {
        if (!strcmp(c, doubleOperators[i]))
        {
            return 1;
        }
    }
    return 0;
}

int isDelimeters(char c)
{
    for (int i = 0; i < 12; i++)
    {
        if (c == delimeters[i])
        {
            return 1;
        }
    }
    return 0;
}

void stringReader(FILE *file)
{
    char *string = (char *)malloc(1);
    string[0] = '\0';
    char c;
    while ((c = fgetc(file)) != '"')
    {
        if (c == EOF)
        {
            /// ERROR
            return;
        }
        if (c == '\n')
        {
            line++;
        }
        int n = strlen(string);
        string = (char *)realloc(string, n + 2);
        string[n + 1] = '\0';
        string[n] = c;
    }
    if (strlen(string) > 256)
    {
        /// ERROR
        return;
    }
    struct Token token;
    token.type = String;
    token.line = line;
    token.value = (char *)malloc(strlen(string) + 1);
    strcpy(token.value, string);
    printToken(token);
}

void comment(int i, FILE *file)
{
    /// i = 1  /* comment */
    /// i = 2  // comment
}

char operatorReader(char c, FILE *file)
{
    char c2 = fgetc(file);
    char *cc2 = (char *)malloc(3);
    cc2[0] = c;
    cc2[1] = c2;
    cc2[2] = '\0';
    if (!strcmp(cc2, "/*"))
    {
        comment(1, file);
        return NULL;
    }
    if (!strcmp(cc2, "//"))
    {
        comment(2, file);
        return NULL;
    }
    if (isDoubleOperator(cc2))
    {
        struct Token token;
        token.type = Operator;
        token.line = line;
        token.value = (char *)malloc(3);
        token.value[0] = c;
        token.value[1] = c2;
        token.value[1] = '\0';
        printToken(token);
        return NULL;
    }
    struct Token token;
    token.type = Operator;
    token.line = line;
    token.value = (char *)malloc(2);
    token.value[0] = c;
    token.value[1] = '\0';
    printToken(token);
    return c2;
}

void lexical_analysis(FILE *file)
{
    char c;
    char b = NULL;
    char *str = NULL;
    while (1)
    {
        if (b == NULL)
        {
            c = fgetc(file);
        }
        else
        {
            c = b;
            b = NULL;
        }
        if (c == EOF)
        {
            break;
        }

        if (isspace(c))
        {
            if (str != NULL)
            {
                recongnizer(str);
                str = NULL;
            }
            if (c == '\n')
            {
                line++;
            }
            continue;
        }
        if (isDelimeters(c))
        {
            if (str != NULL)
            {
                recongnizer(str);
                str = NULL;
            }
            if (c == '"')
            {
                stringReader(file);
            }
            else
            {
                struct Token token;
                token.type = Delimiter;
                token.line = line;
                token.value = (char *)malloc(2);
                token.value[0] = c;
                token.value[1] = '\0';
                printToken(token);
            }
            continue;
        }
        if (isOperator(c))
        {
            if (str != NULL)
            {
                recongnizer(str);
                str = NULL;
            }
            b = operatorReader(c, file);
            continue;
        }
        if (str == NULL)
        {
            str = (char *)malloc(2);
            str[0] = c;
            str[1] = '\0';
        }
        else
        {
            int n = strlen(str);
            str = (char *)realloc(str, n + 2);
            str[n + 1] = '\0';
            str[n] = c;
        }
    }
}
