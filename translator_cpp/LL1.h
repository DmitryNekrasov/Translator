//
//  LL1.h
//  translator_cpp
//
//  Created by Дмитрий Некрасов on 28.11.14.
//  Copyright (c) 2014 Дмитрий Некрасов. All rights reserved.
//

#ifndef translator_cpp_LL1_h
#define translator_cpp_LL1_h

#include "defs.h"
#include "Scanner.h"

class LL1 {
private : 
    int mag[MAX_LEN_MAG], z = 0; // магазин и указатель магазина
    TScanner *sc;
    
public :
    
    bool isTerminal(int t) {
        if (t < netermProgram)
            return true;
        return false;
    }
    
    void outMag() {
        for (int i = 0; i <= z; i++)
            printf("%3d ", mag[i]);
        printf("∆\n");
    }
    
    void epsilon() {
//        z--;
    }
    
    int LL_1() {
        int t;
        TypeLex lex;
        int flag = 1;
        
        mag[z] = netermProgram;
        t = sc->Scanner(lex);
        
        while (flag) {
            
            if (z == -1)
                break;
            
            outMag();
            
            if (isTerminal(mag[z])) {
                
                // в верхушке магазина терминал
                
                if (mag[z] == t) { // верхушка совпадает с отсканированным терминалом
                    if (t == TEnd)
                        flag = 0;
                    else {
                        t = sc->Scanner(lex); // сканируем новый символ
                        z--; // стираем верхушку магазина
                    }
                } else {
                    sc->printError("неверный символ", lex);
                    return -1;
                }
            } else {
                
                // в верхушке магазина нетерминал
                
                switch (mag[z]) {
                        
                    case netermProgram :
                        if (t ==TFloat || t == TChar) {
                            mag[z++] = netermProgram;
                            mag[z++] = netermI;
                            mag[z++] = netermType;
                        } else {
                            epsilon();
                        }
                        break;
                        
                    case netermI :
                        if (t == TId) {
                            mag[z++] = netermJ;
                            mag[z++] = TId;
                        } else if (t == TMain) {
                            mag[z++] = netermBlock;
                            mag[z++] = TCloseRoundBracket;
                            mag[z++] = TOpenRoundBracket;
                            mag[z++] = TMain;
                        } else {
                            sc->printError("неверный символ", lex);
                            return -1;
                        }
                        break;
                        
                    case netermJ :
                        if (t == TOpenRoundBracket) {
                            mag[z++] = netermBlock;
                            mag[z++] = TCloseRoundBracket;
                            mag[z++] = TOpenRoundBracket;
                        } else {
                            mag[z++] = TSemicolon;
                            mag[z++] = netermH;
                            mag[z++] = netermE;
                        }
                        break;
                        
                    case netermType :
                        if (t == TFloat) {
                            mag[z++] = TFloat;
                        } else if (t == TChar) {
                            mag[z++] = TChar;
                        } else {
                            sc->printError("неверный символ", lex);
                            return -1;
                        }
                        break;
                        
                    case netermListVar :
                        if (t == TId) {
                            mag[z++] = netermH;
                            mag[z++] = netermE;
                            mag[z++] = TId;
                        } else {
                            sc->printError("неверный символ", lex);
                            return -1;
                        }
                        break;
                        
                    case netermH :
                        if (t == TComma) {
                            mag[z++] = netermListVar;
                            mag[z++] = TComma;
                        } else {
                            epsilon();
                        }
                        break;
                        
                    case netermListItem :
                        if (t == TId) {
                            mag[z++] = netermE;
                            mag[z++] = TId;
                        } else {
                            sc->printError("неверный символ", lex);
                            return -1;
                        }
                        break;
                        
                    case netermE :
                        if (t == TAssignment) {
                            mag[z++] = netermExpression;
                            mag[z++] = TAssignment;
                        } else if (t == TOpenSquareBracket) {
                            mag[z++] = netermF;
                            mag[z++] = TCloseSquareBracket;
                            mag[z++] = netermConst;
                            mag[z++] = TOpenSquareBracket;
                        } else {
                            epsilon();
                        }
                        break;
                        
                    case netermF :
                        if (t == TAssignment) {
                            mag[z++] = netermG;
                            mag[z++] = TAssignment;
                        } else {
                            epsilon();
                        }
                        break;
                        
                    case netermG :
                        if (t == TConstString) {
                            mag[z++] = TConstString;
                        } else if (t == TOpenCurlyBracket) {
                            mag[z++] = TCloseCurlyBracket;
                            mag[z++] = netermConstSeq;
                            mag[z++] = TOpenCurlyBracket;
                        } else {
                            sc->printError("неверный символ", lex);
                            return -1;
                        }
                        break;
                        
                    case netermConstSeq :
                        if (t == TConstInt || t == TConstChar) {
                            mag[z++] = netermK;
                            mag[z++] = netermConst;
                        } else {
                            sc->printError("неверный символ", lex);
                            return -1;
                        }
                        break;
                        
                    case netermK :
                        if (t == TComma) {
                            mag[z++] = netermConstSeq;
                            mag[z++] = TComma;
                        } else {
                            epsilon();
                        }
                        break;
                        
                    case netermFunctionName :
                        if (t == TId) {
                            mag[z++] = TId;
                        } else if (t == TMain) {
                            mag[z++] = TMain;
                        } else {
                            sc->printError("неверный символ", lex);
                            return -1;
                        }
                        break;
                        
                    case netermBlock :
                        if (t == TOpenCurlyBracket) {
                            mag[z++] = TCloseCurlyBracket;
                            mag[z++] = netermOperators;
                            mag[z++] = TOpenCurlyBracket;
                        } else {
                            sc->printError("неверный символ", lex);
                            return -1;
                        }
                        break;
                        
                    case netermOperators :
                        if (t == TId || t == TOpenCurlyBracket || t == TSemicolon || t == TDo || t == TMain || t == TReturn) {
                            mag[z++] = netermOperators;
                            mag[z++] = netermOneOperator;
                        } else if (t == TFloat || t == TChar) {
                            mag[z++] = netermOperators;
                            mag[z++] = TSemicolon;
                            mag[z++] = netermListVar;
                            mag[z++] = netermType;
                        } else {
                            epsilon();
                        }
                        break;
                        
                    case netermOneOperator :
                        if (t == TId) {
                            mag[z++] = netermD;
                            mag[z++] = TId;
                        } else if (t == TMain) {
                            mag[z++] = TCloseRoundBracket;
                            mag[z++] = TOpenRoundBracket;
                            mag[z++] = TMain;
                        } else if (t == TDo) {
                            mag[z++] = netermDoWhile;
                        } else if (t == TOpenCurlyBracket) {
                            mag[z++] = netermBlock;
                        } else if (t == TReturn) {
                            mag[z++] = netermReturn;
                        } else if (t == TSemicolon) {
                            mag[z++] = TSemicolon;
                        } else {
                            sc->printError("неверный символ", lex);
                            return -1;
                        }
                        break;
                        
                    case netermD :
                        if (t == TOpenRoundBracket) {
                            mag[z++] = TCloseRoundBracket;
                            mag[z++] = TOpenRoundBracket;
                        } else {
                            mag[z++] = netermExpression;
                            mag[z++] = TAssignment;
                            mag[z++] = netermB;
                        }
                        break;
                        
                    case netermAssignment :
                        if (t == TId) {
                            mag[z++] = TSemicolon;
                            mag[z++] = netermExpression;
                            mag[z++] = TAssignment;
                            mag[z++] = netermItemForAssignment;
                        } else {
                            sc->printError("неверный символ", lex);
                            return -1;
                        }
                        break;
                        
                    case netermItemForAssignment :
                        if (t == TId) {
                            mag[z++] = netermB;
                            mag[z++] = TId;
                        } else {
                            sc->printError("неверный символ", lex);
                            return -1;
                        }
                        break;
                        
                    case netermExpression :
                        mag[z++] = netermA1;
                        break;
                        
                    case netermA1 :
                        mag[z++] = netermA11;
                        mag[z++] = netermA2;
                        break;
                        
                    case netermA11 :
                        if (t == TMore || t == TLess || t == TEquals || t == TMoreOrEquals || t == TLessOrEquals || t == TNoEquals) {
                            mag[z++] = netermA11;
                            mag[z++] = netermA2;
                            mag[z++] = netermA111;
                        } else {
                            epsilon();
                        }
                        break;
                        
                    case netermA111 :
                        if (t == TMore) {
                            mag[z++] = TMore;
                        } else if (t == TLess) {
                            mag[z++] = TLess;
                        } else if (t == TEquals) {
                            mag[z++] = TEquals;
                        } else if (t == TMoreOrEquals) {
                            mag[z++] = TMoreOrEquals;
                        } else if (t == TLessOrEquals) {
                            mag[z++] = TLessOrEquals;
                        } else if (t == TNoEquals) {
                            mag[z++] = TNoEquals;
                        } else {
                            sc->printError("неверный символ", lex);
                            return -1;
                        }
                        break;
                        
                    case netermA2 :
                        if (t == TPlus) {
                            mag[z++] = netermA21;
                            mag[z++] = netermA3;
                            mag[z++] = TPlus;
                        } else if (t == TMinus) {
                            mag[z++] = netermA21;
                            mag[z++] = netermA3;
                            mag[z++] = TMinus;
                        } else {
                            mag[z++] = netermA21;
                            mag[z++] = netermA3;
                        }
                        break;
                        
                    case netermA21 :
                        if (t == TPlus || t == TMinus) {
                            mag[z++] = netermA21;
                            mag[z++] = netermA3;
                            mag[z++] = netermA211;
                        } else {
                            epsilon();
                        }
                        break;
                        
                    case netermA211 :
                        if (t == TPlus) {
                            mag[z++] = TPlus;
                        } else if (t == TMinus) {
                            mag[z++] = TMinus;
                        } else {
                            sc->printError("неверный символ", lex);
                            return -1;
                        }
                        break;
                        
                    case netermA3 :
                        mag[z++] = netermA31;
                        mag[z++] = netermA4;
                        break;
                        
                    case netermA31 :
                        if (t == TMultiply || t == TDiv || t == TMod) {
                            mag[z++] = netermA31;
                            mag[z++] = netermA4;
                            mag[z++] = netermA311;
                        } else {
                            epsilon();
                        }
                        break;
                        
                    case netermA311 :
                        if (t == TMultiply) {
                            mag[z++] = TMultiply;
                        } else if (t == TDiv) {
                            mag[z++] = TDiv;
                        } else if (t == TMod) {
                            mag[z++] = TMod;
                        } else {
                            sc->printError("неверный символ", lex);
                            return -1;
                        }
                        break;
                        
                    case netermA4 :
                        if (t == TPlusPlus || t == TMinusMinus) {
                            mag[z++] = netermA5;
                            mag[z++] = netermA511;
                        } else {
                            mag[z++] = netermA512;
                            mag[z++] = netermA5;
                        }
                        break;
                        
                    case netermA511 :
                        if (t == TPlusPlus) {
                            mag[z++] = TPlusPlus;
                        } else if (t == TMinusMinus) {
                            mag[z++] = TMinusMinus;
                        } else {
                            sc->printError("неверный символ", lex);
                            return -1;
                        }
                        break;
                        
                    case netermA512 :
                        if (t == TPlusPlus || t == TMinusMinus) {
                            mag[z++] = netermA511;
                        } else {
                            epsilon();
                        }
                        break;
                        
                    case netermA5 :
                        if (t == TOpenRoundBracket) {
                            mag[z++] = TCloseRoundBracket;
                            mag[z++] = netermA1;
                            mag[z++] = TOpenRoundBracket;
                        } else {
                            mag[z++] = netermOperand;
                        }
                        break;
                        
                    case netermOperand :
                        if (t == TId) {
                            mag[z++] = netermC;
                            mag[z++] = TId;
                        } else if (t == TMain) {
                            mag[z++] = TCloseRoundBracket;
                            mag[z++] = TOpenRoundBracket;
                            mag[z++] = TMain;
                        } else {
                            mag[z++] = netermConst;
                        }
                        break;
                        
                    case netermB :
                        if (t == TOpenSquareBracket) {
                            mag[z++] = TCloseSquareBracket;
                            mag[z++] = netermExpression;
                            mag[z++] = TOpenSquareBracket;
                        } else {
                            epsilon();
                        }
                        break;
                        
                    case netermC :
                        if (t == TOpenRoundBracket) {
                            mag[z++] = TCloseRoundBracket;
                            mag[z++] = TOpenRoundBracket;
                        } else {
                            mag[z++] = netermB;
                        }
                        break;
                        
                    case netermConst :
                        if (t == TConstInt) {
                            mag[z++] = TConstInt;
                        } else if (t == TConstChar) {
                            mag[z++] = TConstChar;
                        } else {
                            sc->printError("неверный символ", lex);
                            return -1;
                        }
                        break;
                        
                    case netermDoWhile :
                        mag[z++] = TSemicolon;
                        mag[z++] = TCloseRoundBracket;
                        mag[z++] = netermExpression;
                        mag[z++] = TOpenRoundBracket;
                        mag[z++] = TWhile;
                        mag[z++] = netermOneOperator;
                        mag[z++] = TDo;
                        break;
                        
                    case netermReturn :
                        mag[z++] = TSemicolon;
                        mag[z++] = netermExpression;
                        mag[z++] = TReturn;
                        break;
                        
                    case netermFunctionCall :
                        mag[z++] = TCloseRoundBracket;
                        mag[z++] = TOpenRoundBracket;
                        mag[z++] = netermFunctionName;
                        break;
                        
                } // конец switch
                z--;
            } // конец else

        } // конец while
        
        return 1; // нормальный выход
    }
    
    
    LL1(TScanner *s) {
        sc = s;
    }
    
    ~LL1() {};
};

#endif
