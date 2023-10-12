#include <stdio.h>
#include <string.h>
#include "Lex.h"



int main(int argc,char* argv[])
{
    FILE *fp; //file object 
    FILE *ff; //output file object
    int p2 = 0; //second array index
    int p = 0;//main array index
    int type = 0;//switch control
    int nl = -1;
    int string = 2;
    int keyword = 3;
    
    int KWindex = 0; //keyword array index
    fp = fopen(argv[1], "r");//open file
    ff = fopen("output.txt", "w");
    char farray[4016]; //main array
    char KeyWord[256]; //array to search for keywords
    //array of keywords
    char KeyWords[40][10] = {"accessor","and","array","begin","bool","case","character","constant","else","elsif","end",
                            "exit","function","if","in","integer","interface","is","loop","module","mutator","natural",
                            "null","of","or","other","out","positive","procedure","range","return","struct","subtype","then",
                            "type","when","while"};
    //array of operators
    char operators[17] = {'.','<','>','(',')','+','-','*','/','|','&',';',',',':','[',']','='};
    char op[2] = {0,0};//array for double operators
    //start main loop. loop while not at the end of file
    while((farray[p] = fgetc(fp)) != EOF)
    {    
        type = 0; //reinitilize type
        KWindex = 0; //reinitilize keyword index
        //reinitilize keyword search array
        for(int i = 0; i < 256; ++i)
        {
            KeyWord[i] = 0;
        }
        //check for numaric operators
        type = findnum(type, p, farray);
        //check for comments
        if(type == 0)
        {
            type = findcomment(type, p, farray);
        }
        //check for double operators
        if(type == 0)
        {
            type = findop(type, p, farray, op, fp);
        }
        //check for single operators
        if(type == 0)
        {
            type = findop2(type, p, farray, operators);
        }
        //remove newline, space, or null terminator
        if((farray[p] == '\n' || farray[p] == '\0' || farray[p] == ' ') && type == 0)
        {
            type = nl;
        }
        //check for strings
        if(farray[p] == '"' && type == 0)
        {
            type = string;
        }
        //check for keywords and if none found get identifier
        if(farray[p] > 96 && farray[p] < 123 && type == 0)
        {
            p2 = p;
            KeyWord[KWindex] = farray[p2];
            p2++;
            farray[p2] = fgetc(fp);
           
            
            
            while(farray[p2] > 96 && farray[p2] < 123)
            {
                ++KWindex;
                KeyWord[KWindex] = farray[p2];
                ++p2;
                farray[p2] = fgetc(fp);
                
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
                }
                fseek(fp, -1, SEEK_CUR);
            }
        }


//switch statment to control output        
        switch(type)
        {
            case 1:
                while(1)
                {
                    fprintf(ff,"%c", farray[p]);
                    p++;
                    farray[(p)] = fgetc(fp);
                    if(farray[(p)] == '/')
                    {
                        fprintf(ff,"%c", farray[p]);
                        fprintf(ff," (comment)\n");
                        p++;
                        break;
                    } 
                }
                break;

            case 2:
                while(1)
                {
                    fprintf(ff,"%c", farray[p]);
                    p++;
                    farray[(p)] = fgetc(fp);
                    if(farray[(p)] == '"')
                    {
                        fprintf(ff,"%c", farray[p]);
                        fprintf(ff," (string)\n");
                        p++;
                        break;
                    } 
                }
                break;

            case 3:
                fprintf(ff,"%s",KeyWord);
                fprintf(ff," (keyword)\n");    
                break;

            case 4:
                fprintf(ff,"%c%c", op[0],op[1]);
                fprintf(ff," (operator)\n");
                break; 

            case 5:
                fprintf(ff,"%c",farray[p]);
                fprintf(ff," (operator)\n");
                break;

            case 6:
                fprintf(ff,"%s (identifier)\n",KeyWord);
                break;

            case 7:
                farray[p+1]= fgetc(fp);
                farray[p+2]= fgetc(fp);
                 if((farray[p+1]) == '.' && (farray[p+2] > 47 && farray[p+2] < 58))
                 {
                     fprintf(ff,"%c%c%c", farray[p], farray[p+1], farray[p+2]);
                     break;
                 }
                 else
                 {
                     fprintf(ff,"%c (numeric literal)\n", farray[p]);
                     fseek(fp, -2, SEEK_CUR);
                     break;
                 }  

            case 0:
                fprintf(ff,"\nUnknow\n");
                break;

            case -1:
                break;
        } 
    }
    fclose(fp);
    fclose(ff);
return 0;
}   
