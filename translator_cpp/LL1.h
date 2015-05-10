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

struct Operand {
    int operandType;
    OperandValue operandValue;
    
    Operand(int _operandType, TypeLex lex) {
        operandType = _operandType;
        strcpy(operandValue.asOperand, lex);
    }
    
    Operand(int _operandType, int adress) {
        operandType = _operandType;
        operandValue.asAdress = adress;
    }
    
    Operand(int _operandType, OperandValue _operandValue) {
        operandType = _operandType;
        operandValue = _operandValue;
    }
    
    Operand() {}
};

struct Triad {
    int operation;
    Operand *firstOperand, *secondOperand;
    
    Triad(int _operation, Operand* _firstOperand, Operand* _secondOperand) {
        operation = _operation;
        firstOperand = _firstOperand;
        secondOperand = _secondOperand;
    }
};

class LL1 {
private : 
    int mag[MAX_LEN_MAG], z = 0;  // магазин и указатель магазина
    
    Tree* treePointers[MAX_LEN_MAG];  // стек указателей на вершины семантического дерева
    int tpz = 0;  // указатель стека
    
    Operand* operands[MAX_LEN_MAG];  // стек для операндов (R)
    int oz = 0;  // указатель стека
    
    Triad* triads[MAX_LEN_MAG];  // список триад
    int tz = 0;  // указатель списка
    
    TScanner *sc;
    Tree *root;  // корень семантического дерева
    
    int currentType;  // последний отсканированный тип
    TypeLex currentId;  // посдедний отсканированный идентификатор
    TypeLex currentConst;  // последняя отсканированная константа
    int currentConstType;  // тип последней отсканированной константы
    
    int countElements = 0;  // текущее количество перечисляемых констант при присваивании в массив
    
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
            case DELTA5: str = "∆5"; break;
            case DELTA6: str = "∆6"; break;
            case DELTA7: str = "∆7"; break;
            case DELTA8: str = "∆8"; break;
            case DELTA9: str = "∆9"; break;
            case DELTA10: str = "∆10"; break;
                
            case DELTA_GEN_MUL: str = "∆*"; break;
            case DELTA_GEN_DIV: str = "∆/"; break;
            case DELTA_GEN_MOD: str = "∆%"; break;
            case DELTA_GEN_PLUS: str = "∆+"; break;
            case DELTA_GEN_MINUS: str = "∆-"; break;
            case DELTA_GEN_ASSIGNMENT: str = "∆="; break;
            case DELTA_GEN_CMP: str = "∆cmp"; break;
            case DELTA_GEN_INDEX: str = "∆index"; break;
            case DELTA_GEN_CALL: str = "∆call"; break;
                
            case DELTA_WRITE_CONST: str = "∆wConst"; break;
            case DELTA_WRITE_MINUS_ONE: str = "∆w-1"; break;
            case DELTA_WRITE_TOP: str = "∆wTop"; break;
            case DELTA_WRITE_ONE: str = "∆w1"; break;
            case DELTA_WRITE_ZERO: str = "∆w0"; break;
            case DELTA_WRITE_CURRENT_ID: str = "∆curID"; break;
            case DELTA_WRITE_COUNT_ELEM: str = "∆cnt"; break;
                
            case DELTA_REVERSE_LAST_TWO: str = "∆reverse"; break;
            case DELTA_POP: str = "∆pop"; break;
            case DELTA_INIT_ARRAY_STRING: str = "∆str"; break;
                
            default: str = "^_^";
        }
        
        return str;
    }
    
    string codeOperationToString(int code) {
        string str;
        
        switch (code) {
                
            case TRI_MUL: str = "*"; break;
            case TRI_DIV: str = "/"; break;
            case TRI_MOD: str = "%"; break;
            case TRI_PLUS: str = "+"; break;
            case TRI_MINUS: str = "-"; break;
            case TRI_ASSIGNMENT: str = "="; break;
            case TRI_CMP: str = "cmp"; break;
            case TRI_INDEX: str = "index"; break;
            case TRI_CALL: str = "call"; break;
                
            default: str = "^_^";
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
        
        if (t == TConstChar || t == TConstInt || t == TConstString) {
            strcpy(currentConst, lex);
            currentConstType = t;
        }
    }
    
    int LL_1() {
        int t;
        TypeLex lex;
        int flag = 1;
        
        mag[z++] = TEnd;
        mag[z] = netermProgram;
        
        t = sc->Scanner(lex);
        getCurrents(t, lex);
        
        while (flag) {
            
            outMag();
//            outOperands();
            
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
                
                // в верхушке магазина дельта
                
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
                            mag[z++] = DELTA_GEN_ASSIGNMENT;
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
                            mag[z++] = DELTA5;
                        } else {
                            epsilon();
                        }
                        break;
                        
                    case netermG :
                        if (t == TConstString) {
                            mag[z++] = DELTA_INIT_ARRAY_STRING;
                            mag[z++] = DELTA6;
                            mag[z++] = TConstString;
                        } else if (t == TOpenCurlyBracket) {
                            mag[z++] = DELTA8;
                            mag[z++] = TCloseCurlyBracket;
                            mag[z++] = netermConstSeq;
                            mag[z++] = TOpenCurlyBracket;
                            mag[z++] = DELTA7;
                        } else {
                            sc->printError("неверный символ", lex);
                            return -1;
                        }
                        break;
                        
                    case netermConstSeq :
                        if (t == TConstInt || t == TConstChar) {
                            mag[z++] = netermK;
                            mag[z++] = netermConst;
                            mag[z++] = DELTA10;
                            mag[z++] = DELTA_GEN_ASSIGNMENT;
                            mag[z++] = DELTA_WRITE_CONST;
                            mag[z++] = DELTA_GEN_INDEX;
                            mag[z++] = DELTA_WRITE_COUNT_ELEM;
                            mag[z++] = DELTA_WRITE_CURRENT_ID;
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
                            mag[z++] = DELTA_GEN_ASSIGNMENT;
                            mag[z++] = netermExpression;
                            mag[z++] = TAssignment;
                            mag[z++] = netermB;
                        }
                        break;
                        
                    case netermAssignment :
                        if (t == TId) {
                            mag[z++] = TSemicolon;
                            mag[z++] = DELTA_GEN_ASSIGNMENT;
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
                            mag[z++] = DELTA_GEN_CMP;
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
                            mag[z++] = DELTA_GEN_MUL;
                            mag[z++] = netermA3;
                            mag[z++] = DELTA_WRITE_MINUS_ONE;
                            mag[z++] = TMinus;
                        } else {
                            mag[z++] = netermA21;
                            mag[z++] = netermA3;
                        }
                        break;
                        
                    case netermA21 :
                        if (t == TPlus) {
                            mag[z++] = netermA21;
                            mag[z++] = DELTA_GEN_PLUS;
                            mag[z++] = netermA3;
                            mag[z++] = TPlus;
                        } else if (t == TMinus) {
                            mag[z++] = netermA21;
                            mag[z++] = DELTA_GEN_MINUS;
                            mag[z++] = netermA3;
                            mag[z++] = TMinus;
                        } else {
                            epsilon();
                        }
                        break;
                        
                    case netermA3 :
                        mag[z++] = netermA31;
                        mag[z++] = netermA4;
                        break;
                        
                    case netermA31 :
                        if (t == TMultiply) {
                            mag[z++] = netermA31;
                            mag[z++] = DELTA_GEN_MUL;
                            mag[z++] = netermA4;
                            mag[z++] = TMultiply;
                        } else if (t == TDiv) {
                            mag[z++] = netermA31;
                            mag[z++] = DELTA_GEN_DIV;
                            mag[z++] = netermA4;
                            mag[z++] = TDiv;
                        } else if (t == TMod) {
                            mag[z++] = netermA31;
                            mag[z++] = DELTA_GEN_MOD;
                            mag[z++] = netermA4;
                            mag[z++] = TMod;
                        } else {
                            epsilon();
                        }
                        break;
                        
                    case netermA4 :
                        if (t == TPlusPlus) {
                            mag[z++] = DELTA_GEN_ASSIGNMENT;
                            mag[z++] = DELTA_GEN_PLUS;
                            mag[z++] = DELTA_WRITE_ONE;
                            mag[z++] = DELTA_WRITE_TOP;
                            mag[z++] = netermA5;
                            mag[z++] = TPlusPlus;
                        } else if (t == TMinusMinus) {
                            mag[z++] = DELTA_GEN_ASSIGNMENT;
                            mag[z++] = DELTA_GEN_MINUS;
                            mag[z++] = DELTA_WRITE_ONE;
                            mag[z++] = DELTA_WRITE_TOP;
                            mag[z++] = netermA5;
                            mag[z++] = TMinusMinus;
                        } else {
                            mag[z++] = netermA512;
                            mag[z++] = netermA5;
                        }
                        break;
                        
                    case netermA511 :
                        if (t == TPlusPlus) {
                            mag[z++] = DELTA_POP;
                            mag[z++] = DELTA_GEN_ASSIGNMENT;
                            mag[z++] = DELTA_GEN_PLUS;
                            mag[z++] = DELTA_WRITE_ONE;
                            mag[z++] = DELTA_WRITE_TOP;
                            mag[z++] = DELTA_REVERSE_LAST_TWO;
                            mag[z++] = DELTA_GEN_PLUS;
                            mag[z++] = DELTA_WRITE_ZERO;
                            mag[z++] = DELTA_WRITE_TOP;
                            mag[z++] = TPlusPlus;
                        } else if (t == TMinusMinus) {
                            mag[z++] = DELTA_POP;
                            mag[z++] = DELTA_GEN_ASSIGNMENT;
                            mag[z++] = DELTA_GEN_MINUS;
                            mag[z++] = DELTA_WRITE_ONE;
                            mag[z++] = DELTA_WRITE_TOP;
                            mag[z++] = DELTA_REVERSE_LAST_TWO;
                            mag[z++] = DELTA_GEN_PLUS;
                            mag[z++] = DELTA_WRITE_ZERO;
                            mag[z++] = DELTA_WRITE_TOP;
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
                            mag[z++] = DELTA_GEN_CALL;
                            mag[z++] = TCloseRoundBracket;
                            mag[z++] = TOpenRoundBracket;
                            mag[z++] = DELTA3_FUNCTION;
                            mag[z++] = TMain;
                        } else {
                            mag[z++] = DELTA_WRITE_CONST;
                            mag[z++] = netermConst;
                        }
                        break;
                        
                    case netermB :
                        if (t == TOpenSquareBracket) {
                            mag[z++] = TCloseSquareBracket;
                            mag[z++] = DELTA_GEN_INDEX;
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
                            mag[z++] = DELTA_GEN_CALL;
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
                        mag[z++] = DELTA_GEN_CALL;
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
    
    void outOperands() {
        for (int i = 0; i < oz; i++) {
            outOneOperand(operands[i]);
            cout << " ";
        }
        cout << "\n";
    }
    
    void outOneOperand(Operand* operand) {
        if (operand->operandType == TYPE_IS_OPERAND)
            cout << operand->operandValue.asOperand;
        else
            cout << "(" << operand->operandValue.asAdress << ")";
    }
    
    void outOneTriad(Triad* triad) {
        cout << codeOperationToString(triad->operation) << " ";
        outOneOperand(triad->firstOperand);
        cout << " ";
        outOneOperand(triad->secondOperand);
    }
    
    void outTriads() {
        for (int i = 0; i < tz; i++) {
            cout << i << ") ";
            outOneTriad(triads[i]);
            cout << "\n";
        }
    }
    
    void processingDelta() {
        
        switch (mag[z]) {
                
            // --------------------------------------- Семантика ---------------------------------------
                
            case DELTA1_VAR: {
                root->semInclude(currentId, TypeNodeVar, currentType, sc);
                operands[oz++] = new Operand(TYPE_IS_OPERAND, currentId);
                break;
            }
                
            case DELTA1_FUNCTION: {
                treePointers[tpz++] = root->semInclude(currentId, TypeNodeFunction, currentType, sc);
                break;
            }
                
            case DELTA1_ARRAY: {
                root->semInclude(currentId, TypeNodeArray, currentType, sc);
                operands[oz++] = new Operand(TYPE_IS_OPERAND, currentId);
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
                operands[oz++] = new Operand(TYPE_IS_OPERAND, currentId);
                break;
            }
                
            case DELTA3_FUNCTION: {
                root->semGetFunc(currentId, sc);
                operands[oz++] = new Operand(TYPE_IS_OPERAND, currentId);
                break;
            }
                
            case DELTA3_ARRAY: {
                root->semGetArray(currentId, sc);
                operands[oz++] = new Operand(TYPE_IS_OPERAND, currentId);
                break;
            }
                
            case DELTA4: {
                Tree::cur->node->sizeArray = root->getSizeArray(currentConstType, currentConst);
                break;
            }
                
            case DELTA5: {
                root->controlFloatArrayAssignment(sc);
                break;
            }
                
            case DELTA6: {
                root->controlStringConst(Tree::cur, currentConst, sc);
                break;
            }
                
            case DELTA7: {
                countElements = 0;
                break;
            }
                
            case DELTA10: {
                countElements++;
                break;
            }
                
            case DELTA8: {
                root->controlCountElements(Tree::cur, countElements, sc);
                break;
            }
                
                
            // ---------------------------------------- Триады -----------------------------------------
                
            case DELTA_GEN_MUL: {
                generateArithmeticTriad(TRI_MUL);
                break;
            }
                
            case DELTA_GEN_DIV: {
                generateArithmeticTriad(TRI_DIV);
                break;
            }
                
            case DELTA_GEN_MOD: {
                generateArithmeticTriad(TRI_MOD);
                break;
            }
                
            case DELTA_GEN_PLUS: {
                generateArithmeticTriad(TRI_PLUS);
                break;
            }
                
            case DELTA_GEN_MINUS: {
                generateArithmeticTriad(TRI_MINUS);
                break;
            }
                
            case DELTA_GEN_ASSIGNMENT: {
                generateArithmeticTriad(TRI_ASSIGNMENT);
                break;
            }
                
            case DELTA_GEN_CMP: {
                generateArithmeticTriad(TRI_CMP);
                break;
            }
                
            case DELTA_GEN_INDEX: {
                generateArithmeticTriad(TRI_INDEX);
                break;
            }
                
            case DELTA_GEN_CALL: {
                Operand* operand = operands[--oz];
                Triad *triad = new Triad(TRI_CALL, operand, new Operand(TYPE_IS_OPERAND, ""));
                triads[tz++] = triad;
                operands[oz++] = new Operand(TYPE_IS_ADRESS, tz - 1);
                break;
            }
                
                
            // запись в operands (R)
                
            case DELTA_WRITE_CONST: {
                operands[oz++] = new Operand(TYPE_IS_OPERAND, currentConst);
                break;
            }
                
            case DELTA_WRITE_MINUS_ONE: {
                TypeLex minusOne = "-1";
                operands[oz++] = new Operand(TYPE_IS_OPERAND, minusOne);
                break;
            }
                
            case DELTA_WRITE_TOP: {
                Operand *operand = new Operand(operands[oz - 1]->operandType, operands[oz - 1]->operandValue);
                operands[oz++] = operand;
                break;
            }
                
            case DELTA_WRITE_ONE: {
                TypeLex one = "1";
                operands[oz++] = new Operand(TYPE_IS_OPERAND, one);
                break;
            }
                
            case DELTA_WRITE_ZERO: {
                TypeLex one = "0";
                operands[oz++] = new Operand(TYPE_IS_OPERAND, one);
                break;
            }
                
            case DELTA_WRITE_COUNT_ELEM: {
                string s = to_string(countElements);
                TypeLex cnt;
                strcpy(cnt, s.c_str());
                operands[oz++] = new Operand(TYPE_IS_OPERAND, cnt);
                break;
            }
                
            case DELTA_WRITE_CURRENT_ID: {
                operands[oz++] = new Operand(TYPE_IS_OPERAND, currentId);
                break;
            }
                
            case DELTA_REVERSE_LAST_TWO: {
                Operand *tmp = operands[oz - 2];
                operands[oz - 2] = operands[oz - 1];
                operands[oz - 1] = tmp;
                break;
            }
                
            case DELTA_POP: {
                delete operands[oz - 1];
                oz--;
                break;
            }
                
            case DELTA_INIT_ARRAY_STRING: {
                size_t len = strlen(currentConst);
                TypeLex ch = {'\'', '_', '\''};
                for (int i = 1; i < len; i++) {
                    ch[1] = currentConst[i];
                    
                    // концевой 0
                    if (i == len - 1) {
                        strcpy(ch, "\\0");
                    }
                    
                    // положить в стек id
                    operands[oz++] = new Operand(TYPE_IS_OPERAND, currentId);
                    
                    // положить в стек индекс
                    string s = to_string(i - 1);
                    TypeLex index;
                    strcpy(index, s.c_str());
                    operands[oz++] = new Operand(TYPE_IS_OPERAND, index);
                    
                    // взять индекс
                    generateArithmeticTriad(TRI_INDEX);
                    
                    // положить в стек символ
                    operands[oz++] = new Operand(TYPE_IS_OPERAND, ch);
                    
                    // сгенерировать присваивание
                    generateArithmeticTriad(TRI_ASSIGNMENT);
                }
                break;
            }
                
        }
        
        z--;
    }
    
    void generateArithmeticTriad(int operation) {
        Operand* operand2 = operands[--oz];
        Operand* operand1 = operands[--oz];
        Triad *triad = new Triad(operation, operand1, operand2);
        triads[tz++] = triad;
        operands[oz++] = new Operand(TYPE_IS_ADRESS, tz - 1);
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
