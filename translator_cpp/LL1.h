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
    int mag[MAX_LEN_MAG], z = 0;  // магазин и указатель магазина
    
    Tree* treePointers[MAX_LEN_MAG];  // стек указателей на вершины семантического дерева
    int tpz = 0;  // указатель стека
    
    TScanner *sc;
    Tree *root;  // корень семантического дерева
    
    int currentType;  // последний отсканированный тип
    TypeLex currentId;  // посдедний отсканированный идентификатор
    TypeLex currentConst;  // последняя отсканированная константа
    int currentConstType;  // тип последней отсканированной константы
    
public :
    
    string codeToString(int code) {
        string str;
        
        switch(code) {
            case netermProgram: str = "Program"; break;
            case netermI: str = "I"; break;
            case netermJ: str = "J"; break;
            case netermType: str = "Type"; break;
            case netermListVar: str = "ListVar"; break;
            case netermH: str = "H"; break;
            case netermListItem: str = "ListItem"; break;
            case netermE: str = "E"; break;
            case netermF: str = "F"; break;
            case netermG: str = "G"; break;
            case netermConstSeq: str = "ConstSeq"; break;
            case netermK: str = "K"; break;
            case netermFunctionName: str = "FunctionName"; break;
            case neterm: str = "neterm"; break;
            case netermBlock: str = "Block"; break;
            case netermOperators: str = "Operators"; break;
            case netermOneOperator: str = "OneOperator"; break;
            case netermD: str = "D"; break;
            case netermAssignment: str = "Assignment"; break;
            case netermItemForAssignment: str = "ItemForAssignment"; break;
            case netermExpression: str = "Expression"; break;
            case netermA1: str = "A1"; break;
            case netermA11: str = "A11"; break;
            case netermA111: str = "A111"; break;
            case netermA2: str = "A2"; break;
            case netermA21: str = "A21"; break;
            case netermA211: str = "A211"; break;
            case netermA3: str = "A3"; break;
            case netermA31: str = "A31"; break;
            case netermA311: str = "A311"; break;
            case netermA4: str = "A4"; break;
            case netermA511: str = "A511"; break;
            case netermA512: str = "A512"; break;
            case netermA5: str = "A5"; break;
            case netermOperand: str = "Operand"; break;
            case netermB: str = "B"; break;
            case netermC: str = "C"; break;
            case netermConst: str = "Const"; break;
            case netermDoWhile: str = "DoWhile"; break;
            case netermReturn: str = "Return"; break;
            case netermFunctionCall: str = "FunctionCall"; break;
                
            case TId: str = "id"; break;
            case TConstInt: str = "constInt"; break;
            case TConstChar: str = "constChar"; break;
            case TConstString: str = "constString"; break;
                
            case TPlus: str = "+"; break;
            case TMinus: str = "-"; break;
            case TMultiply: str = "*"; break;
            case TDiv: str = "/"; break;
            case TMod: str = "%"; break;
            case TAssignment: str = "="; break;
            case TPlusPlus: str = "++"; break;
            case TMinusMinus: str = "--"; break;
            case TMore: str = ">"; break;
            case TLess: str = "<"; break;
            case TMoreOrEquals: str = ">="; break;
            case TLessOrEquals: str = "<="; break;
            case TEquals: str = "=="; break;
            case TNoEquals: str = "!="; break;
                
            case TOpenRoundBracket: str = "("; break;
            case TCloseRoundBracket: str = ")"; break;
            case TOpenCurlyBracket: str = "{"; break;
            case TCloseCurlyBracket: str = "}"; break;
            case TOpenSquareBracket: str = "["; break;
            case TCloseSquareBracket: str = "]"; break;
                
            case TSemicolon: str = ";"; break;
            case TComma: str = ","; break;
                
            case TFloat: str = "float"; break;
            case TChar: str = "char"; break;
            case TDo: str = "do"; break;
            case TWhile: str = "while"; break;
            case TMain: str = "main"; break;
            case TReturn: str = "return"; break;
                
            case TError: str = "TError"; break;
            case TEnd: str = "TEnd"; break;
                
            case DELTA1_FUNCTION: str = "∆1_FUNCTION"; break;
            case DELTA1_VAR: str = "∆1_VAR"; break;
            case DELTA1_ARRAY: str = "∆1_ARRAY"; break;
            case DELTA2_BLOCK: str = "∆2_BLOCK"; break;
            case DELTA3_FUNCTION: str = "∆3_FUNCTION"; break;
            case DELTA3_VAR: str = "∆3_VAR"; break;
            case DELTA3_ARRAY: str = "∆3_ARRAY"; break;
            case DELTA4: str = "∆4"; break;
            case DELTA9: str = "∆9"; break;
                
            default: str = "HZ";
        }
        
        return str;
    }
    
    bool isDelta(int t) {
        if (t <= DELTA1_FUNCTION)
            return true;
        return false;
    }
    
    bool isTerminal(int t) {
        if (t < netermProgram && t >= 0)
            return true;
        return false;
    }
    
    void outMag() {
        for (int i = 0; i <= z; i++)
            cout << codeToString(mag[i]) << " | ";
        cout << "\n";
    }
    
    void outMagInt() {
        for (int i = 0; i <= z; i++)
            printf("%3d ", mag[i]);
        printf("∆\n");
    }
    
    void epsilon() {
//        z--;
    }
    
    // обновляем текущий тип, идентификатор и константу, если надо
    void getCurrents(int t, TypeLex lex) {
        
        if (t == TChar || t == TFloat) {
            currentType = t;
        }
        
        if (t == TId || t == TMain) {
            strcpy(currentId, lex);
        }
        
        if (t == TConstChar || t == TConstInt) {
            strcpy(currentConst, lex);
            currentConstType = t;
        }
    }
    
    int LL_1() {
        int t;
        TypeLex lex;
        int flag = 1;
        
        mag[z] = netermProgram;
        t = sc->Scanner(lex);
        getCurrents(t, lex);
        
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
                        getCurrents(t, lex);
                        
                        z--; // стираем верхушку магазина
                    }
                } else {
                    sc->printError("неверный символ", lex);
                    return -1;
                }
            } else if (isDelta(mag[z])) {
                processingDelta();
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
                            mag[z++] = DELTA9;
                            mag[z++] = netermBlock;
                            mag[z++] = TCloseRoundBracket;
                            mag[z++] = TOpenRoundBracket;
                            mag[z++] = DELTA1_FUNCTION;
                            mag[z++] = TMain;
                        } else {
                            sc->printError("неверный символ", lex);
                            return -1;
                        }
                        break;
                        
                    case netermJ :
                        if (t == TOpenRoundBracket) {
                            mag[z++] = DELTA9;
                            mag[z++] = netermBlock;
                            mag[z++] = TCloseRoundBracket;
                            mag[z++] = TOpenRoundBracket;
                            mag[z++] = DELTA1_FUNCTION;
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
                            mag[z++] = DELTA1_VAR;
                        } else if (t == TOpenSquareBracket) {
                            mag[z++] = netermF;
                            mag[z++] = TCloseSquareBracket;
                            mag[z++] = DELTA4;
                            mag[z++] = netermConst;
                            mag[z++] = TOpenSquareBracket;
                            mag[z++] = DELTA1_ARRAY;
                        } else {
                            mag[z++] = DELTA1_VAR;
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
                            mag[z++] = DELTA3_FUNCTION;
                            mag[z++] = TId;
                        } else if (t == TMain) {
                            mag[z++] = DELTA3_FUNCTION;
                            mag[z++] = TMain;
                        } else {
                            sc->printError("неверный символ", lex);
                            return -1;
                        }
                        break;
                        
                    case netermBlock :
                        if (t == TOpenCurlyBracket) {
                            mag[z++] = DELTA9;
                            mag[z++] = TCloseCurlyBracket;
                            mag[z++] = netermOperators;
                            mag[z++] = TOpenCurlyBracket;
                            mag[z++] = DELTA2_BLOCK;
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
                            mag[z++] = DELTA3_FUNCTION;
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
                            mag[z++] = DELTA3_FUNCTION;
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
                            mag[z++] = DELTA3_FUNCTION;
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
                            mag[z++] = DELTA3_ARRAY;
                        } else {
                            mag[z++] = DELTA3_VAR;
                            epsilon();
                        }
                        break;
                        
                    case netermC :
                        if (t == TOpenRoundBracket) {
                            mag[z++] = TCloseRoundBracket;
                            mag[z++] = TOpenRoundBracket;
                            mag[z++] = DELTA3_FUNCTION;
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
    
    void processingDelta() {
        
        switch (mag[z]) {
                
            // --------------------------------------- Семантика ---------------------------------------
                
            case DELTA1_VAR: {
                root->semInclude(currentId, TypeNodeVar, currentType, sc);
                break;
            }
                
            case DELTA1_FUNCTION: {
                treePointers[tpz++] = root->semInclude(currentId, TypeNodeFunction, currentType, sc);
                break;
            }
                
            case DELTA1_ARRAY: {
                root->semInclude(currentId, TypeNodeArray, currentType, sc);
                break;
            }
                
            case DELTA9: {
                Tree::cur = treePointers[--tpz];
                break;
            }
                
            case DELTA2_BLOCK: {
                treePointers[tpz++] = root->semIncludeBlock();
                break;
            }
                
            case DELTA3_VAR: {
                root->semGetType(currentId, sc);
                break;
            }
                
            case DELTA3_FUNCTION: {
                root->semGetFunc(currentId, sc);
                break;
            }
                
            case DELTA3_ARRAY: {
                root->semGetArray(currentId, sc);
                break;
            }
                
            case DELTA4: {
                int sizeArray = root->getSizeArray(currentConstType, currentConst);
                Tree::cur->node->sizeArray = sizeArray;
                break;
            }
                
        }
        
        z--;
    }
    
    void outTree() {
        root->printTree();
    }
    
    LL1(TScanner *s) {
        sc = s;
        Node *node = new Node();
        strcpy(node->id, "###");
        root = new Tree(NULL, NULL, NULL, node);
        Tree::cur = root;
    }
    
    ~LL1() {};
};

#endif
