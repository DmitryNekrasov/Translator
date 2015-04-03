//
//  Scanner.h
//  translator_cpp
//
//  Created by Дмитрий Некрасов on 02.10.14.
//  Copyright (c) 2014 Дмитрий Некрасов. All rights reserved.
//

#ifndef translator_cpp_Scanner_h
#define translator_cpp_Scanner_h

#include "defs.h"

class TScanner {
private :
    TypeMod t;
    int uk;
    int pos, line, tmpPos, tmpLine;
    
    TypeLex keyWord[MAX_NUM_KEY_WORD] = {"float", "char", "do", "while", "main", "return"};
    int indexKeyWord[MAX_NUM_KEY_WORD] = {TFloat, TChar, TDo, TWhile, TMain, TReturn};
    
    bool isLetter(char ch) {
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
            return true;
        return false;
    }
    
    bool isDigit(char ch) {
        if (ch >= '0' && ch <= '9')
            return true;
        return false;
    }
    
    void newLine() {
        line++;
        pos = 0;
    }
    
public :
    void setUK(int i) {
        line = tmpLine;
        pos = tmpPos;
        uk = i;
    }
    
    int getUK() {
        tmpLine = line;
        tmpPos = pos;
        return uk;
    }
    
    int getPos() {
        return pos;
    }
    
    void setPos(int _pos) {
        pos = _pos;
    }
    
    int getLine() {
        return line;
    }
    
    void setLine(int _line) {
        line = _line;
    }
    
    void printError(int typeError) {
        printf("Ошибка! Строка %d, позиция %d. ", line, pos);
        switch (typeError) {
            case EWrongChar : {
                printf("Недопустимый символ.\n");
            } break;
                
            case EWrongCharConst : {
                printf("Некорректная символьная константа.\n");
            } break;
                
            case EWrongStringConst : {
                printf("Некорректная строковая константа.\n");
            } break;
                
            case ELongIntConst : {
                printf("Слишком длинная целая константа.\n");
            } break;
                
            case ELongStringConst : {
                printf("Слишком длинная строковая константа.\n");
            } break;
        }
        exit(1);
    }
    
    void printError(string error, TypeLex lex) {
        cout << "Ошибка! Строка " << line << ", позиция " << pos << ": " << error << " ( " << lex << " )\n";
        exit(1);
    }
    
    void printError(string error) {
        cout << "Ошибка! Строка " << line << ", позиция " << pos << ": " << error << "\n";
        exit(1);
    }
    
    void printException(string exception, int value) {
        cout << "Исключение! Строка " << line << ", позиция " << pos << ": " << exception << " ( " << value << " )\n";
        exit(1);
    }
    
    void printWarning(int typeWarning) {
        printf("Предупреждение! Строка %d, позиция %d. ", line, pos);
        switch (typeWarning) {
            case WLongId : {
                printf("Слишком длинное имя идентификатора.\n");
            } break;
        }
    }
    
    void getData(char *fileName) {
        FILE *input = fopen(fileName, "r");
        int i = 0;
        while (!feof(input)) {
            fscanf(input, "%c", &t[i++]);
        }
        t[i] = '\0';
        fclose(input);
    }
    
    int Scanner(TypeLex lex) {
        memset(lex, '\0', MAX_LEX);
        int i = 0;
        
        //  Игнорируемые символы
        
        while (t[uk] == ' ' || t[uk] == '\n' || t[uk] == '\t' || t[uk] == '/') {
        
            while (t[uk] == ' ' || t[uk] == '\n' || t[uk] == '\t') {
                pos++;
                if (t[uk] == '\n')
                    newLine();
                uk++;
            }
        
            if (t[uk] == '/') {
                if (t[uk + 1] == '/') {
                    uk += 2;
                    while (t[uk] != '\n' && t[uk] != '\0')
                        uk++;
                } else if (t[uk + 1] == '*') {
                    uk += 2;
                    while ((t[uk] != '*' || t[uk + 1] != '/') && t[uk] != '\0') {
                        pos++;
                        if (t[uk] == '\n')
                            newLine();
                        uk++;
                    }
                    uk += 2;
                    pos += 2;
                } else
                    break;
            }
            
        }
        
        
        if (t[uk] == '\0')
            return TEnd;
        
        
        //  Определение лексем
        
        if (isLetter(t[uk])) {
            lex[i++] = t[uk++];
            pos++;
            while (isLetter(t[uk]) || isDigit(t[uk])) {
                if (i != MAX_LEX - 1)
                    lex[i++] = t[uk++];
                else {
                    uk++;
                }
                pos++;
            }
            if (i >= MAX_LEX - 1)
                printWarning(WLongId);
            
            for (int j = 0; j < MAX_NUM_KEY_WORD; j++) {
                if (strcmp(lex, keyWord[j]) == 0)
                    return indexKeyWord[j];
            }
            
            return TId;
            
        } else if (isDigit(t[uk])) {
            lex[i++] = t[uk++];
            pos++;
            while (isDigit(t[uk])) {
                if (i != MAX_LEN_INT)
                    lex[i++] = t[uk++];
                else
                    uk++;
                pos++;
            }
            if (i == MAX_LEN_INT) {
                printError(ELongIntConst);
                return TError;
            }
            return TConstInt;
            
        } else if (t[uk] == '\'') {
            lex[i++] = t[uk++];
            lex[i++] = t[uk++];
            pos += 2;
            if (t[uk] == '\'') {
                lex[i++] = t[uk++];
                pos++;
                return TConstChar;
            }
            else {
                printError(EWrongCharConst);
                return TError;
            }
            
        } else if (t[uk] == '\"') {
            lex[i++] = t[uk++];
            while (t[uk] != '\"' && t[uk] != '\0' && t[uk] != '\n') {
                if (i != MAX_LEX - 2)
                    lex[i++] = t[uk++];
                else
                    uk++;
                pos++;
            }
            
            if (t[uk] == '\"') {
                lex[i++] = t[uk++];
                pos++;
                if (i == MAX_LEX - 1) {
                    printError(ELongStringConst);
                    return TError;
                }
                return TConstString;
            }
            else {
                printError(EWrongStringConst);
                return TError;
            }
            
        } else if (t[uk] == '+') {
            lex[i++] = t[uk++];
            pos++;
            if (t[uk] != '+')
                return TPlus;
            else {
                lex[i++] = t[uk++];
                pos++;
                return TPlusPlus;
            }
            
        } else if (t[uk] == '-') {
            lex[i++] = t[uk++];
            pos++;
            if (t[uk] != '-')
                return TMinus;
            else {
                lex[i++] = t[uk++];
                pos++;
                return TMinusMinus;
            }
            
        } else if (t[uk] == '*') {
            lex[i++] = t[uk++];
            pos++;
            return TMultiply;
            
        } else if (t[uk] == '/') {
            lex[i++] = t[uk++];
            pos++;
            return TDiv;
            
        } else if (t[uk] == '%') {
            lex[i++] = t[uk++];
            pos++;
            return TMod;
            
        } else if (t[uk] == '=') {
            lex[i++] = t[uk++];
            pos++;
            if (t[uk] != '=')
                return TAssignment;
            else {
                lex[i++] = t[uk++];
                pos++;
                return TEquals;
            }
            
        } else if (t[uk] == '>') {
            lex[i++] = t[uk++];
            pos++;
            if (t[uk] != '=')
                return TMore;
            else {
                lex[i++] = t[uk++];
                pos++;
                return TMoreOrEquals;
            }
            
        } else if (t[uk] == '<') {
            lex[i++] = t[uk++];
            pos++;
            if (t[uk] != '=')
                return TLess;
            else {
                lex[i++] = t[uk++];
                pos++;
                return TLessOrEquals;
            }
            
        } else if (t[uk] == '!') {
            lex[i++] = t[uk++];
            pos++;
            if (t[uk] != '=') {
                printError(EWrongChar);
                return TError;
            }
            else {
                lex[i++] = t[uk++];
                pos++;
                return TNoEquals;
            }
            
        } else if (t[uk] == '(') {
            lex[i++] = t[uk++];
            pos++;
            return TOpenRoundBracket;
            
        } else if (t[uk] == ')') {
            lex[i++] = t[uk++];
            pos++;
            return TCloseRoundBracket;
            
        } else if (t[uk] == '{') {
            lex[i++] = t[uk++];
            pos++;
            return TOpenCurlyBracket;
            
        } else if (t[uk] == '}') {
            lex[i++] = t[uk++];
            pos++;
            return TCloseCurlyBracket;
            
        } else if (t[uk] == '[') {
            lex[i++] = t[uk++];
            pos++;
            return TOpenSquareBracket;
            
        } else if (t[uk] == ']') {
            lex[i++] = t[uk++];
            pos++;
            return TCloseSquareBracket;
            
        } else if (t[uk] == ';') {
            lex[i++] = t[uk++];
            pos++;
            return TSemicolon;
            
        } else if (t[uk] == ',') {
            lex[i++] = t[uk++];
            pos++;
            return TComma;
        }
        
        lex[i++] = t[uk++];
        pos++;
        printError(EWrongChar);
        return TError;
    }
    
    TScanner(char *fileName) {
        getData(fileName);
        uk = 0;
        pos = 0;
        line = 1;
    }
    
    ~TScanner() {};
};

#endif