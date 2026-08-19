#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

/* List of C keywords */
const char *keywords[] = {
    "auto", "break", "case", "char", "const", "continue",
    "default", "do", "double", "else", "enum", "extern",
    "float", "for", "goto", "if", "int", "long",
    "register", "return", "short", "signed", "sizeof",
    "static", "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while"
};

int keywordCount = 32;

/* Check whether a word is a keyword */
int isKeyword(char *word)
{
    int i;

    for (i = 0; i < keywordCount; i++)
    {
        if (strcmp(word, keywords[i]) == 0)
            return 1;
    }

    return 0;
}

/* Check whether character is a separator */
int isSeparator(char ch)
{
    return (ch == '(' || ch == ')' ||
            ch == '{' || ch == '}' ||
            ch == '[' || ch == ']' ||
            ch == ';' || ch == ',');
}

/* Check whether character is an operator */
int isOperator(char ch)
{
    return (ch == '+' || ch == '-' ||
            ch == '*' || ch == '/' ||
            ch == '=' || ch == '>' ||
            ch == '<' || ch == '%' ||
            ch == '!' || ch == '&' ||
            ch == '|');
}

int main()
{
    FILE *fp;
    char filename[100];

    char ch;
    char token[MAX];
    int i;

    int keywords_count = 0;
    int identifiers = 0;
    int operators = 0;
    int constants = 0;
    int stringLiterals = 0;
    int separators = 0;

    printf("Enter source file name: ");
    scanf("%s", filename);

    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Error: Cannot open file.\n");
        return 1;
    }

    printf("\nTOKEN TYPE\n");
    printf("------------------------------------------------\n");

    while ((ch = fgetc(fp)) != EOF)
    {
        /* Ignore spaces, tabs and new lines */
        if (isspace(ch))
            continue;

        /* Check for comments */
        if (ch == '/')
        {
            char next = fgetc(fp);

            /* Single-line comment */
            if (next == '/')
            {
                while ((ch = fgetc(fp)) != '\n' && ch != EOF)
                    ;

                continue;
            }

            /* Multi-line comment */
            else if (next == '*')
            {
                char prev = 0;

                while ((ch = fgetc(fp)) != EOF)
                {
                    if (prev == '*' && ch == '/')
                        break;

                    prev = ch;
                }

                continue;
            }

            /* Division operator */
            else
            {
                ungetc(next, fp);

                printf("/ Operator\n");
                operators++;
                continue;
            }
        }

        /* String literal */
        if (ch == '"')
        {
            i = 0;
            token[i++] = ch;

            while ((ch = fgetc(fp)) != EOF)
            {
                token[i++] = ch;

                if (ch == '"')
                    break;
            }

            token[i] = '\0';

            printf("%s String Literal\n", token);
            stringLiterals++;

            continue;
        }

        /* Identifier or Keyword */
        if (isalpha(ch) || ch == '_')
        {
            i = 0;
            token[i++] = ch;

            while ((ch = fgetc(fp)) != EOF &&
                   (isalnum(ch) || ch == '_'))
            {
                token[i++] = ch;
            }

            token[i] = '\0';

            if (ch != EOF)
                ungetc(ch, fp);

            if (isKeyword(token))
            {
                printf("%s Keyword\n", token);
                keywords_count++;
            }
            else
            {
                printf("%s Identifier\n", token);
                identifiers++;
            }

            continue;
        }

        /* Numeric constant */
        if (isdigit(ch))
        {
            i = 0;
            token[i++] = ch;

            while ((ch = fgetc(fp)) != EOF &&
                   (isdigit(ch) || ch == '.'))
            {
                token[i++] = ch;
            }

            token[i] = '\0';

            if (ch != EOF)
                ungetc(ch, fp);

            printf("%s Constant\n", token);
            constants++;

            continue;
        }

        /* Separators */
        if (isSeparator(ch))
        {
            printf("%c Separator\n", ch);
            separators++;

            continue;
        }

        /* Operators */
        if (isOperator(ch))
        {
            char next = fgetc(fp);

            /* Two-character operators */
            if ((ch == '=' && next == '=') ||
                (ch == '>' && next == '=') ||
                (ch == '<' && next == '=') ||
                (ch == '!' && next == '=') ||
                (ch == '+' && next == '+') ||
                (ch == '-' && next == '-') ||
                (ch == '&' && next == '&') ||
                (ch == '|' && next == '|'))
            {
                printf("%c%c Operator\n", ch, next);
                operators++;
            }
            else
            {
                if (next != EOF)
                    ungetc(next, fp);

                printf("%c Operator\n", ch);
                operators++;
            }

            continue;
        }

        /* Special symbols */
        printf("%c Special Symbol\n", ch);
    }

    printf("\n------------------------------------------------\n");

    printf("\nToken Count\n");
    printf("Keywords : %d\n", keywords_count);
    printf("Identifiers : %d\n", identifiers);
    printf("Operators : %d\n", operators);
    printf("Constants : %d\n", constants);
    printf("String Literals: %d\n", stringLiterals);
    printf("Separators : %d\n", separators);

    fclose(fp);

    return 0;
}
