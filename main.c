#include "Lex.h"

int findnum(int t, int in, char a[])
{
    if(a[in] > 47 && a[in] < 58)
        {
            t = 7;
            return t;
        }
    return 0;
}

int findcomment(int t, int in, char a[])
{
    if(a[in] == '/' && t == 0 )
        {
            t = 1;
            return t;
            
        }
    return 0;
}

int findop(int t, int in, char a[], char op[], FILE *fp)
{
    if((a[in] == '.' || a[in] == '<' || a[in] == '>' || a[in] == '=' || a[in] == '*' || a[in] == ':' || a[in] == '!') && t == 0)
        {
            op[0] = a[in];
            op[1] = fgetc(fp);
            if(op[1] == '=' || op[1] == '.' || op[1] == '<' || op[1] == '>' || op[1] == '*')
            {
                t = 4;
                return t;
            }
            else
            {
                fseek(fp, -1, SEEK_CUR);
                return 0;
            }
        }
    return 0;
}

int findop2(int t, int in, char a[], char o[])
{
    if(t == 0)
        {
            for (int i = 0; i < 17; ++i)
            {   
                if(a[in] == o[i] && t == 0)
                {
                    t = 5;
                    return t;
                }
            }
        }
    return 0;
}