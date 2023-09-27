#include <stdio.h>
#include <string.h>


int main(int argc,char* argv[])
{
    FILE *fp; 
    int offset = 0;
    int p2 = 0;
    int type = 0;
    int nl = -1;
    int comment = 1;
    int string = 2;
    int keyword = 3;
    //int operate = 4;
    int p = 0;
    int KWindex = 0;
    fp = fopen(argv[1], "r");
    char farray[4016];
    char KeyWord[256];
    char KeyWords[40][10] = {"accessor","and","array","begin","bool","case","character","constant","else","elsif","end",
                            "exit","function","if","in","integer","interface","is","loop","module","mutator","natural",
                            "null","of","or","other","out","positive","procedure","range","return","struct","subtype","then",
                            "type","when","while"};
    char operators[17] = {'.','<','>','(',')','+','-','*','/','|','&',';',',',':','[',']','='};
    char op[2] = {0,0};
    while((farray[p] = fgetc(fp)) != EOF)
    {    
        type = 0;
        KWindex = 0;
        
        for(int i = 0; i < 256; ++i)
        {
            KeyWord[i] = 0;
        }

        if(farray[p] > 47 && farray[p] < 58)
        {
            type = 7;
        }

        if(farray[p] == '/' && type == 0 )
        {
            type = comment;
            
        }

        if((farray[p] == '.' || farray[p] == '<' || farray[p] == '>' || farray[p] == '=' || farray[p] == '*' || farray[p] == ':' || farray[p] == '!') && type == 0)
        {
            op[0] = farray[p];
            op[1] = fgetc(fp);
            if(op[1] == '=' || op[1] == '.' || op[1] == '<' || op[1] == '>' || op[1] == '*')
            {
                type = 4;
            }
            else
            {
                fseek(fp, -1, SEEK_CUR);
            }
        }

        if(type == 0)
        {
            for (int i = 0; i < 17; ++i)
            {   
                if(farray[p] == operators[i] && type == 0)
                {
                    type = 5;
                    break;
                }
            }
        }

        if((farray[p] == '\n' || farray[p] == '\0' || farray[p] == ' ') && type == 0)
        {
            type = nl;
        }

        if(farray[p] == '"' && type == 0)
        {
            type = string;
        }

        if(farray[p] > 96 && farray[p] < 123 && type == 0)
        {
            p2 = p;
            KeyWord[KWindex] = farray[p2];
            p2++;
            farray[p2] = fgetc(fp);
            offset = 1;
            
            
            while(farray[p2] > 96 && farray[p2] < 123)
            {
                ++KWindex;
                KeyWord[KWindex] = farray[p2];
                ++p2;
                farray[p2] = fgetc(fp);
                ++offset;
                
            }
            
            fseek(fp, -1, SEEK_CUR);
            for(int i = 0; i < 39; ++i)
            {
                if(strcmp(KeyWord, KeyWords[i]) == 0)
                {
                    type = keyword;
                    break;
                }
            }

            if(type == 0)
            {
                type = 6;
                fseek(fp, 1, SEEK_CUR);
                while((farray[p2] > 96 && farray[p2] < 123) || (farray[p2] > 47 && farray[p2] < 58) || farray[p2] == '_')
                {
                    ++KWindex;
                    KeyWord[KWindex] = farray[p2];
                    ++p2;
                    farray[p2] = fgetc(fp);
                    ++offset;   
                }
                fseek(fp, -1, SEEK_CUR);
            }
        }


        
        switch(type)
        {
            case 1:
                while(1)
                {
                    printf("%c", farray[p]);
                    p++;
                    farray[(p)] = fgetc(fp);
                    if(farray[(p)] == '/')
                    {
                        printf("%c", farray[p]);
                        printf(" (comment)\n");
                        p++;
                        break;
                    } 
                }
                break;

            case 2:
                while(1)
                {
                    printf("%c", farray[p]);
                    p++;
                    farray[(p)] = fgetc(fp);
                    if(farray[(p)] == '"')
                    {
                        printf("%c", farray[p]);
                        printf(" (string)\n");
                        p++;
                        break;
                    } 
                }
                break;

            case 3:
                printf("%s",KeyWord);
                printf(" (keyword)\n");    
                break;

            case 4:
                printf("%c%c", op[0],op[1]);
                printf(" (operator)\n");
                break; 

            case 5:
                printf("%c",farray[p]);
                printf(" (operator)\n");
                break;

            case 6:
                printf("%s (identifier)\n",KeyWord);
                break;

            case 7:
                printf("%c (numeric literal)\n", farray[p]);
                break;  

            case 0:
                printf("\nUnknow\n");
                break;

            case -1:
                break;
        } 
    }
return 0;
}   
