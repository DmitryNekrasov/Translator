//
//  Syntax.h
//  translator_cpp
//
//  Created by Дмитрий Некрасов on 16.10.14.
//  Copyright (c) 2014 Дмитрий Некрасов. All rights reserved.
//

#ifndef translator_cpp_Syntax_h
#define translator_cpp_Syntax_h

#include "defs.h"
#include "Scanner.h"
#include "Tree.h"

struct TData {
    int typeData; // тип (float, char)
    DataValue dataValue; // значение
};

class TSyntax {
private :
    TScanner *sc;
public :

    Tree *root;
    
    bool flint = true;
    
    void copyLex(TypeLex ident, TypeLex lex) {
        size_t len = strlen(lex);
        for (int i = 0; i < len; i++)
            ident[i] = lex[i];
        ident[len] = '\0';
    }
    
    void program() {
        TypeLex lex;
        int t, tmpUk;
        
        tmpUk = sc->getUK(); t = sc->Scanner(lex);
        sc->setUK(tmpUk);
        
        if (t != TFloat && t != TChar) {
            sc->printError("ожидался float или char", lex);
        }
        
        while (t == TFloat || t == TChar) {
            description();
            tmpUk = sc->getUK();
            t = sc->Scanner(lex);
            sc->setUK(tmpUk);
        }
        
        if (t != TEnd) {
            sc->printError("ожидался float или char", lex);
        }
    }
    
    
    void description() {
        TypeLex lex;
        int t, tmpUk = 0;
        
        TypeLex ident;
        int typeData;
        
        t = sc->Scanner(lex);
        if (t != TFloat && t != TChar)
            sc->printError("ожидался тип данных float или char", lex);
        
        typeData = t;
        
        bool idIsMain = false;
        
        do {
            t = sc->Scanner(lex);
            if (t != TId && t != TMain) {
                sc->printError("ожидался идентификатор или Main", lex);
            }
            
            if (t == TMain) {
                idIsMain = true;
            }
            
            copyLex(ident, lex);
            
            tmpUk = sc->getUK(); t = sc->Scanner(lex);
            if (t == TAssignment) {
                Tree *v = root->semInclude(ident, TypeNodeVar, typeData, sc);
                
                TData res;
                A1(&res);
                
                // инициализация переменной
                if (flint) {
                    switch (v->node->typeData) {
                        case TypeDataChar :
                            switch(res.typeData) {
                                case TypeDataInt : v->node->dataValue.dataAsChar = (char) res.dataValue.dataAsInt; break;
                                case TypeDataFloat : v->node->dataValue.dataAsChar = (char) res.dataValue.dataAsFloat; break;
                                case TypeDataChar : v->node->dataValue.dataAsChar = res.dataValue.dataAsChar; break;
                                case TypeDataBool : v->node->dataValue.dataAsChar = (char) res.dataValue.dataAsBool; break;
                            }
                            break;
                        
                        case TypeDataFloat :
                            switch(res.typeData) {
                                case TypeDataInt : v->node->dataValue.dataAsFloat = (float) res.dataValue.dataAsInt; break;
                                case TypeDataFloat : v->node->dataValue.dataAsFloat = res.dataValue.dataAsFloat; break;
                                case TypeDataChar : v->node->dataValue.dataAsFloat = (float) res.dataValue.dataAsChar; break;
                                case TypeDataBool : v->node->dataValue.dataAsFloat = (float) res.dataValue.dataAsBool; break;
                            }
                            break;
                    }
                }
                
                tmpUk = sc->getUK(); t = sc->Scanner(lex);
                
            } else if (t == TOpenSquareBracket) {
                Tree *v = root->semInclude(ident, TypeNodeArray, typeData, sc);
                
                TData sizeArray;
                root->semSetSizeArray(v, constant(&sizeArray));
                if (sizeArray.typeData == TypeDataChar) {
                    sizeArray.typeData = TypeDataInt;
                    sizeArray.dataValue.dataAsInt = sizeArray.dataValue.dataAsChar;
                }
                
                // выделение памяти под массив
                if (flint) {
                    switch (v->node->typeData) {
                        case TypeDataChar : v->node->dataValue.arrayDataAsChar = new char[sizeArray.dataValue.dataAsInt]; break;
                        case TypeDataFloat : v->node->dataValue.arrayDataAsFloat = new float[sizeArray.dataValue.dataAsInt]; break;
                    }
                }
                
                t = sc->Scanner(lex);
                if (t != TCloseSquareBracket)
                    sc->printError("ожидался символ \']\'", lex);
                
                tmpUk = sc->getUK(); t = sc->Scanner(lex);
                if (t == TAssignment) {
                    t = sc->Scanner(lex);
                    if (t == TConstString) {
                        root->controlStringConst(v, lex, sc);
                        
                        if (v->node->typeData == TypeDataFloat)
                            sc->printError("невозможно инициализировать массив типа float строкой");
                        
                        // преобразуем строку в массив типа char
                        if (flint) {
                            size_t len = strlen(lex);
                            int i;
                            for (i = 1; i < len - 1; i++) {
                                v->node->dataValue.arrayDataAsChar[i - 1] = lex[i];
                            }
                            v->node->dataValue.arrayDataAsChar[i] = '\0';
                        }
                        
                        tmpUk = sc->getUK(); t = sc->Scanner(lex);
                    } else if (t == TOpenCurlyBracket) {
                        int countElements = 0;
                        do {
                            
                            TData res;
                            constant(&res);
                            
                            countElements++;
                            root->controlCountElements(v, countElements, sc);
                            
                            // a[5] = {1, 2, 3, 4, 5};
                            if (flint) {
                                switch (v->node->typeData) {
                                    case TypeDataChar :
                                        switch (res.typeData) {
                                            case TypeDataInt :
                                                v->node->dataValue.arrayDataAsChar[countElements - 1] = (char) res.dataValue.dataAsInt; break;
                                            case TypeDataFloat :
                                                v->node->dataValue.arrayDataAsChar[countElements - 1] = (char) res.dataValue.dataAsFloat; break;
                                            case TypeDataChar :
                                                v->node->dataValue.arrayDataAsChar[countElements - 1] = res.dataValue.dataAsChar; break;
                                            case TypeDataBool :
                                                v->node->dataValue.arrayDataAsChar[countElements - 1] = (char) res.dataValue.dataAsBool; break;
                                        }
                                        break;
                                    
                                    case TypeDataFloat :
                                        switch (res.typeData) {
                                            case TypeDataInt :
                                                v->node->dataValue.arrayDataAsFloat[countElements - 1] = (float) res.dataValue.dataAsInt; break;
                                            case TypeDataFloat :
                                                v->node->dataValue.arrayDataAsFloat[countElements - 1] = res.dataValue.dataAsFloat; break;
                                            case TypeDataChar :
                                                v->node->dataValue.arrayDataAsFloat[countElements - 1] = (float) res.dataValue.dataAsChar; break;
                                            case TypeDataBool :
                                                v->node->dataValue.arrayDataAsFloat[countElements - 1] = (float) res.dataValue.dataAsBool; break;
                                        }
                                        break;
                                }
                            }
                            
                            tmpUk = sc->getUK(); t = sc->Scanner(lex);
                        } while (t == TComma);
                        sc->setUK(tmpUk);
                        
                        t = sc->Scanner(lex);
                        if (t != TCloseCurlyBracket) {
                            sc->printError("ожидался символ \'}\'", lex);
                        }
                        
                        tmpUk = sc->getUK(); t = sc->Scanner(lex);
                        
                    } else {
                        sc->printError("ожидалась строковая константа или символ \'{\'", lex);
                    }
                }
                
            } else if (t == TOpenRoundBracket) {
                Tree *v = root->semInclude(ident, TypeNodeFunction, typeData, sc);
                t = sc->Scanner(lex);
                if (t != TCloseRoundBracket) {
                    sc->printError("ожидался символ \')\'", lex);
                }
                
                // интерпретация описания функции
                v->node->startFunction = sc->getUK();
                if (!idIsMain) flint = false;
                TData res;
                block(&res);
                if (!idIsMain) flint = true;
                
                root->setCur(v);
                tmpUk = sc->getUK(); t = sc->Scanner(lex);
            } else {
                Tree *v = root->semInclude(ident, TypeNodeVar, typeData, sc);
                
                // инициализация нулём по умолчанию
                if (flint) {
                    switch (v->node->typeData) {
                        case TypeDataChar : v->node->dataValue.dataAsChar = 0; break;
                        case TypeDataFloat : v->node->dataValue.dataAsFloat = 0; break;
                    }
                }
            }
            
        } while (t == TComma);
        sc->setUK(tmpUk);
        
        t = sc->Scanner(lex);
        if (t != TSemicolon)
            sc->printError("ожидался символ \';\'", lex);
    } // описание
    
    
    void A1(TData *res) {
        TypeLex lex;
        int t, tmpUk;
        
        A2(res);
        
        tmpUk = sc->getUK(); t = sc->Scanner(lex);
        TData res1;
        while (t == TMore || t == TLess || t == TMoreOrEquals || t == TLessOrEquals || t == TEquals || t == TNoEquals) {
            
            A2(&res1);
            
            if (flint) {
            
                bool ans = false;
            
                switch (res->typeData) {
                    case TypeDataInt :
                    
                        switch (res1.typeData) {
                            case TypeDataInt :
                                if (t == TMore)
                                    ans = res->dataValue.dataAsInt > res1.dataValue.dataAsInt;
                                else if (t == TLess)
                                    ans = res->dataValue.dataAsInt < res1.dataValue.dataAsInt;
                                else if (t == TMoreOrEquals)
                                    ans = res->dataValue.dataAsInt >= res1.dataValue.dataAsInt;
                                else if (t == TLessOrEquals)
                                    ans = res->dataValue.dataAsInt <= res1.dataValue.dataAsInt;
                                else if (t == TEquals)
                                    ans = res->dataValue.dataAsInt == res1.dataValue.dataAsInt;
                                else if (t == TNoEquals)
                                    ans = res->dataValue.dataAsInt != res1.dataValue.dataAsInt;
                                break;
                            
                            case TypeDataFloat :
                                if (t == TMore)
                                    ans = res->dataValue.dataAsInt > res1.dataValue.dataAsFloat;
                                else if (t == TLess)
                                    ans = res->dataValue.dataAsInt < res1.dataValue.dataAsFloat;
                                else if (t == TMoreOrEquals)
                                    ans = res->dataValue.dataAsInt >= res1.dataValue.dataAsFloat;
                                else if (t == TLessOrEquals)
                                    ans = res->dataValue.dataAsInt <= res1.dataValue.dataAsFloat;
                                else if (t == TEquals)
                                    ans = res->dataValue.dataAsInt == res1.dataValue.dataAsFloat;
                                else if (t == TNoEquals)
                                    ans = res->dataValue.dataAsInt != res1.dataValue.dataAsFloat;
                                break;
                            
                            case TypeDataChar :
                                if (t == TMore)
                                    ans = res->dataValue.dataAsInt > res1.dataValue.dataAsChar;
                                else if (t == TLess)
                                    ans = res->dataValue.dataAsInt < res1.dataValue.dataAsChar;
                                else if (t == TMoreOrEquals)
                                    ans = res->dataValue.dataAsInt >= res1.dataValue.dataAsChar;
                                else if (t == TLessOrEquals)
                                    ans = res->dataValue.dataAsInt <= res1.dataValue.dataAsChar;
                                else if (t == TEquals)
                                    ans = res->dataValue.dataAsInt == res1.dataValue.dataAsChar;
                                else if (t == TNoEquals)
                                    ans = res->dataValue.dataAsInt != res1.dataValue.dataAsChar;
                                break;
                            
                            case TypeDataBool :
                                if (t == TMore)
                                    ans = res->dataValue.dataAsInt > res1.dataValue.dataAsBool;
                                else if (t == TLess)
                                    ans = res->dataValue.dataAsInt < res1.dataValue.dataAsBool;
                                else if (t == TMoreOrEquals)
                                    ans = res->dataValue.dataAsInt >= res1.dataValue.dataAsBool;
                                else if (t == TLessOrEquals)
                                    ans = res->dataValue.dataAsInt <= res1.dataValue.dataAsBool;
                                else if (t == TEquals)
                                    ans = res->dataValue.dataAsInt == res1.dataValue.dataAsBool;
                                else if (t == TNoEquals)
                                    ans = res->dataValue.dataAsInt != res1.dataValue.dataAsBool;
                                break;
                        }
                        break;
                    
                    case TypeDataFloat :
                    
                        switch (res1.typeData) {
                            case TypeDataInt :
                                if (t == TMore)
                                    ans = res->dataValue.dataAsFloat > res1.dataValue.dataAsInt;
                                else if (t == TLess)
                                    ans = res->dataValue.dataAsFloat < res1.dataValue.dataAsInt;
                                else if (t == TMoreOrEquals)
                                    ans = res->dataValue.dataAsFloat >= res1.dataValue.dataAsInt;
                                else if (t == TLessOrEquals)
                                    ans = res->dataValue.dataAsFloat <= res1.dataValue.dataAsInt;
                                else if (t == TEquals)
                                    ans = res->dataValue.dataAsFloat == res1.dataValue.dataAsInt;
                                else if (t == TNoEquals)
                                    ans = res->dataValue.dataAsFloat != res1.dataValue.dataAsInt;
                                break;
                            
                            case TypeDataFloat :
                                if (t == TMore)
                                    ans = res->dataValue.dataAsFloat > res1.dataValue.dataAsFloat;
                                else if (t == TLess)
                                    ans = res->dataValue.dataAsFloat < res1.dataValue.dataAsFloat;
                                else if (t == TMoreOrEquals)
                                    ans = res->dataValue.dataAsFloat >= res1.dataValue.dataAsFloat;
                                else if (t == TLessOrEquals)
                                    ans = res->dataValue.dataAsFloat <= res1.dataValue.dataAsFloat;
                                else if (t == TEquals)
                                    ans = res->dataValue.dataAsFloat == res1.dataValue.dataAsFloat;
                                else if (t == TNoEquals)
                                    ans = res->dataValue.dataAsFloat != res1.dataValue.dataAsFloat;
                                break;
                            
                            case TypeDataChar :
                                if (t == TMore)
                                    ans = res->dataValue.dataAsFloat > res1.dataValue.dataAsChar;
                                else if (t == TLess)
                                    ans = res->dataValue.dataAsFloat < res1.dataValue.dataAsChar;
                                else if (t == TMoreOrEquals)
                                    ans = res->dataValue.dataAsFloat >= res1.dataValue.dataAsChar;
                                else if (t == TLessOrEquals)
                                    ans = res->dataValue.dataAsFloat <= res1.dataValue.dataAsChar;
                                else if (t == TEquals)
                                    ans = res->dataValue.dataAsFloat == res1.dataValue.dataAsChar;
                                else if (t == TNoEquals)
                                    ans = res->dataValue.dataAsFloat != res1.dataValue.dataAsChar;
                                break;
                                
                            case TypeDataBool :
                                if (t == TMore)
                                    ans = res->dataValue.dataAsFloat > res1.dataValue.dataAsBool;
                                else if (t == TLess)
                                    ans = res->dataValue.dataAsFloat < res1.dataValue.dataAsBool;
                                else if (t == TMoreOrEquals)
                                    ans = res->dataValue.dataAsFloat >= res1.dataValue.dataAsBool;
                                else if (t == TLessOrEquals)
                                    ans = res->dataValue.dataAsFloat <= res1.dataValue.dataAsBool;
                                else if (t == TEquals)
                                    ans = res->dataValue.dataAsFloat == res1.dataValue.dataAsBool;
                                else if (t == TNoEquals)
                                    ans = res->dataValue.dataAsFloat != res1.dataValue.dataAsBool;
                                break;
                        }
                        break;
                    
                    case TypeDataChar :
                        switch (res1.typeData) {
                            case TypeDataInt :
                                if (t == TMore)
                                    ans = res->dataValue.dataAsChar > res1.dataValue.dataAsInt;
                                else if (t == TLess)
                                    ans = res->dataValue.dataAsChar < res1.dataValue.dataAsInt;
                                else if (t == TMoreOrEquals)
                                    ans = res->dataValue.dataAsChar >= res1.dataValue.dataAsInt;
                                else if (t == TLessOrEquals)
                                    ans = res->dataValue.dataAsChar <= res1.dataValue.dataAsInt;
                                else if (t == TEquals)
                                    ans = res->dataValue.dataAsChar == res1.dataValue.dataAsInt;
                                else if (t == TNoEquals)
                                    ans = res->dataValue.dataAsChar != res1.dataValue.dataAsInt;
                                break;
                                
                            case TypeDataFloat :
                                if (t == TMore)
                                    ans = res->dataValue.dataAsChar > res1.dataValue.dataAsFloat;
                                else if (t == TLess)
                                    ans = res->dataValue.dataAsChar < res1.dataValue.dataAsFloat;
                                else if (t == TMoreOrEquals)
                                    ans = res->dataValue.dataAsChar >= res1.dataValue.dataAsFloat;
                                else if (t == TLessOrEquals)
                                    ans = res->dataValue.dataAsChar <= res1.dataValue.dataAsFloat;
                                else if (t == TEquals)
                                    ans = res->dataValue.dataAsChar == res1.dataValue.dataAsFloat;
                                else if (t == TNoEquals)
                                    ans = res->dataValue.dataAsChar != res1.dataValue.dataAsFloat;
                                break;
                                
                            case TypeDataChar :
                                if (t == TMore)
                                    ans = res->dataValue.dataAsChar > res1.dataValue.dataAsChar;
                                else if (t == TLess)
                                    ans = res->dataValue.dataAsChar < res1.dataValue.dataAsChar;
                                else if (t == TMoreOrEquals)
                                    ans = res->dataValue.dataAsChar >= res1.dataValue.dataAsChar;
                                else if (t == TLessOrEquals)
                                    ans = res->dataValue.dataAsChar <= res1.dataValue.dataAsChar;
                                else if (t == TEquals)
                                    ans = res->dataValue.dataAsChar == res1.dataValue.dataAsChar;
                                else if (t == TNoEquals)
                                    ans = res->dataValue.dataAsChar != res1.dataValue.dataAsChar;
                                break;
                                
                            case TypeDataBool :
                                if (t == TMore)
                                    ans = res->dataValue.dataAsChar > res1.dataValue.dataAsBool;
                                else if (t == TLess)
                                    ans = res->dataValue.dataAsChar < res1.dataValue.dataAsBool;
                                else if (t == TMoreOrEquals)
                                    ans = res->dataValue.dataAsChar >= res1.dataValue.dataAsBool;
                                else if (t == TLessOrEquals)
                                    ans = res->dataValue.dataAsChar <= res1.dataValue.dataAsBool;
                                else if (t == TEquals)
                                    ans = res->dataValue.dataAsChar == res1.dataValue.dataAsBool;
                                else if (t == TNoEquals)
                                    ans = res->dataValue.dataAsChar != res1.dataValue.dataAsBool;
                                break;
                        }
                        break;
                        
                    case TypeDataBool :
                        
                        switch (res1.typeData) {
                            case TypeDataInt :
                                if (t == TMore)
                                    ans = res->dataValue.dataAsBool > res1.dataValue.dataAsInt;
                                else if (t == TLess)
                                    ans = res->dataValue.dataAsBool < res1.dataValue.dataAsInt;
                                else if (t == TMoreOrEquals)
                                    ans = res->dataValue.dataAsBool >= res1.dataValue.dataAsInt;
                                else if (t == TLessOrEquals)
                                    ans = res->dataValue.dataAsBool <= res1.dataValue.dataAsInt;
                                else if (t == TEquals)
                                    ans = res->dataValue.dataAsBool == res1.dataValue.dataAsInt;
                                else if (t == TNoEquals)
                                    ans = res->dataValue.dataAsBool != res1.dataValue.dataAsInt;
                                break;
                                
                            case TypeDataFloat :
                                if (t == TMore)
                                    ans = res->dataValue.dataAsBool > res1.dataValue.dataAsFloat;
                                else if (t == TLess)
                                    ans = res->dataValue.dataAsBool < res1.dataValue.dataAsFloat;
                                else if (t == TMoreOrEquals)
                                    ans = res->dataValue.dataAsBool >= res1.dataValue.dataAsFloat;
                                else if (t == TLessOrEquals)
                                    ans = res->dataValue.dataAsBool <= res1.dataValue.dataAsFloat;
                                else if (t == TEquals)
                                    ans = res->dataValue.dataAsBool == res1.dataValue.dataAsFloat;
                                else if (t == TNoEquals)
                                    ans = res->dataValue.dataAsBool != res1.dataValue.dataAsFloat;
                                break;
                                
                            case TypeDataChar :
                                if (t == TMore)
                                    ans = res->dataValue.dataAsBool > res1.dataValue.dataAsChar;
                                else if (t == TLess)
                                    ans = res->dataValue.dataAsBool < res1.dataValue.dataAsChar;
                                else if (t == TMoreOrEquals)
                                    ans = res->dataValue.dataAsBool >= res1.dataValue.dataAsChar;
                                else if (t == TLessOrEquals)
                                    ans = res->dataValue.dataAsBool <= res1.dataValue.dataAsChar;
                                else if (t == TEquals)
                                    ans = res->dataValue.dataAsBool == res1.dataValue.dataAsChar;
                                else if (t == TNoEquals)
                                    ans = res->dataValue.dataAsBool != res1.dataValue.dataAsChar;
                                break;
                                
                            case TypeDataBool :
                                if (t == TMore)
                                    ans = res->dataValue.dataAsBool > res1.dataValue.dataAsBool;
                                else if (t == TLess)
                                    ans = res->dataValue.dataAsBool < res1.dataValue.dataAsBool;
                                else if (t == TMoreOrEquals)
                                    ans = res->dataValue.dataAsBool >= res1.dataValue.dataAsBool;
                                else if (t == TLessOrEquals)
                                    ans = res->dataValue.dataAsBool <= res1.dataValue.dataAsBool;
                                else if (t == TEquals)
                                    ans = res->dataValue.dataAsBool == res1.dataValue.dataAsBool;
                                else if (t == TNoEquals)
                                    ans = res->dataValue.dataAsBool != res1.dataValue.dataAsBool;
                                break;
                        }
                        break;
                        
                } // конец первого switch
                
                
                res->typeData = TypeDataBool;
                res->dataValue.dataAsBool = ans;
                
            } // конец ифа на flint
            
            tmpUk = sc->getUK(); t = sc->Scanner(lex);
        }
        sc->setUK(tmpUk);
    } // выражение
    
    
    void A2(TData *res) {
        TypeLex lex;
        int t, tmpUk;
        
        tmpUk = sc->getUK(); t = sc->Scanner(lex);
        if (t != TPlus && t != TMinus)
            sc->setUK(tmpUk);
        
        A3(res);
        
        // унарная операция
        if (flint) {
            if (t == TMinus)
                switch (res->typeData) {
                    case TypeDataInt : res->dataValue.dataAsInt *= -1; break;
                    case TypeDataFloat : res->dataValue.dataAsFloat *= -1; break;
                    case TypeDataChar : res->dataValue.dataAsChar *= -1; break;
                    case TypeDataBool :
                        res->typeData = TypeDataInt;
                        res->dataValue.dataAsInt = res->dataValue.dataAsBool;
                        res->dataValue.dataAsInt *= -1;
                        break;
                }
        }
        
        tmpUk = sc->getUK(); t = sc->Scanner(lex);
        TData res1;
        while (t == TPlus || t == TMinus) {
            
            A3(&res1);
            
            if (flint) {
            
                switch (res->typeData) {
                    case TypeDataInt :
                        
                        switch (res1.typeData) {
                            case TypeDataInt :
                                if (t == TPlus) // int + int = int
                                    res->dataValue.dataAsInt += res1.dataValue.dataAsInt;
                                else if (t == TMinus) // int - int = int
                                    res->dataValue.dataAsInt -= res1.dataValue.dataAsInt;
                                break;
                                
                            case TypeDataFloat :
                                res->typeData = TypeDataFloat; // тип результата изменился
                                res->dataValue.dataAsFloat = res->dataValue.dataAsInt; // сделали интовое значение float
                                if (t == TPlus)
                                    res->dataValue.dataAsFloat += res1.dataValue.dataAsFloat;
                                else if (t == TMinus)
                                    res->dataValue.dataAsFloat -= res1.dataValue.dataAsFloat;
                                break;
                                
                            case TypeDataChar :
                                if (t == TPlus) // int + char = int
                                    res->dataValue.dataAsInt += res1.dataValue.dataAsChar;
                                else if (t == TMinus) // int - char = int
                                    res->dataValue.dataAsInt -= res1.dataValue.dataAsChar;
                                break;
                                
                            case TypeDataBool :
                                if (t == TPlus) // int + bool = int
                                    res->dataValue.dataAsInt += res1.dataValue.dataAsBool;
                                else if (t == TMinus) // int - bool = int
                                    res->dataValue.dataAsInt -= res1.dataValue.dataAsBool;
                                break;
                        }
                        break;
                        
                    case TypeDataFloat :
                        
                        switch (res1.typeData) {
                            case TypeDataInt :
                                if (t == TPlus) // float + int = float
                                    res->dataValue.dataAsFloat += res1.dataValue.dataAsInt;
                                else if (t == TMinus) // float - int = float
                                    res->dataValue.dataAsFloat -= res1.dataValue.dataAsInt;
                                break;
                                
                            case TypeDataFloat :
                                if (t == TPlus) // float + float = float
                                    res->dataValue.dataAsFloat += res1.dataValue.dataAsFloat;
                                else if (t == TMinus) // float - float = float
                                    res->dataValue.dataAsFloat -= res1.dataValue.dataAsFloat;
                                break;
                                
                            case TypeDataChar :
                                if (t == TPlus) // float + char = float
                                    res->dataValue.dataAsFloat += res1.dataValue.dataAsChar;
                                else if (t == TMinus) // float - char = float
                                    res->dataValue.dataAsFloat -= res1.dataValue.dataAsChar;
                                break;
                                
                            case TypeDataBool :
                                if (t == TPlus) // float + bool = float
                                    res->dataValue.dataAsFloat += res1.dataValue.dataAsBool;
                                else if (t == TMinus) // float - bool = float
                                    res->dataValue.dataAsFloat -= res1.dataValue.dataAsBool;
                                break;
                        }
                        break;
                        
                    case TypeDataChar :
                        
                        switch (res1.typeData) {
                            case TypeDataInt :
                                res->typeData = TypeDataInt;
                                res->dataValue.dataAsInt = res->dataValue.dataAsChar;
                                if (t == TPlus)
                                    res->dataValue.dataAsInt += res1.dataValue.dataAsInt;
                                else if (t == TMinus)
                                    res->dataValue.dataAsInt -= res1.dataValue.dataAsInt;
                                break;
                                
                            case TypeDataFloat :
                                res->typeData = TypeDataFloat;
                                res->dataValue.dataAsFloat = res->dataValue.dataAsChar;
                                if (t == TPlus)
                                    res->dataValue.dataAsFloat += res1.dataValue.dataAsFloat;
                                else if (t == TMinus)
                                    res->dataValue.dataAsFloat -= res1.dataValue.dataAsFloat;
                                break;
                                
                            case TypeDataChar :
                                if (t == TPlus)
                                    res->dataValue.dataAsChar += res1.dataValue.dataAsChar;
                                else if (t == TMinus)
                                    res->dataValue.dataAsChar -= res1.dataValue.dataAsChar;
                                break;
                                
                            case TypeDataBool :
                                if (t == TPlus)
                                    res->dataValue.dataAsChar += res1.dataValue.dataAsBool;
                                else if (t == TMinus)
                                    res->dataValue.dataAsChar -= res1.dataValue.dataAsBool;
                                break;
                        }
                        break;
                        
                    case TypeDataBool :
                        
                        switch (res1.typeData) {
                            case TypeDataInt :
                                res->typeData = TypeDataInt;
                                res->dataValue.dataAsInt = res->dataValue.dataAsBool;
                                if (t == TPlus)
                                    res->dataValue.dataAsInt += res1.dataValue.dataAsInt;
                                else if (t == TMinus)
                                    res->dataValue.dataAsInt -= res1.dataValue.dataAsInt;
                                break;
                                
                            case TypeDataFloat :
                                res->typeData = TypeDataFloat;
                                res->dataValue.dataAsFloat = res->dataValue.dataAsBool;
                                if (t == TPlus)
                                    res->dataValue.dataAsFloat += res1.dataValue.dataAsFloat;
                                else if (t == TMinus)
                                    res->dataValue.dataAsFloat -= res1.dataValue.dataAsFloat;
                                break;
                                
                            case TypeDataChar :
                                res->typeData = TypeDataChar;
                                res->dataValue.dataAsChar = res->dataValue.dataAsBool;
                                if (t == TPlus)
                                    res->dataValue.dataAsChar += res1.dataValue.dataAsChar;
                                else if (t == TMinus)
                                    res->dataValue.dataAsChar -= res1.dataValue.dataAsChar;
                                break;
                                
                            case TypeDataBool :
                                res->typeData = TypeDataInt;
                                res->dataValue.dataAsInt = res->dataValue.dataAsBool;
                                if (t == TPlus)
                                    res->dataValue.dataAsInt += res1.dataValue.dataAsBool;
                                else if (t == TMinus)
                                    res->dataValue.dataAsInt -= res1.dataValue.dataAsBool;
                                break;
                        }
                        break;
                        
                } // конец первого switch
                
            }
            
            tmpUk = sc->getUK(); t = sc->Scanner(lex);
        }
        sc->setUK(tmpUk);
    }
    
    
    void A3(TData *res) {
        TypeLex lex;
        int t, tmpUk;
        
        A5(res);
        
        tmpUk = sc->getUK(); t = sc->Scanner(lex);
        TData res1;
        while (t == TMultiply || t == TDiv || t == TMod) {
            
            A5(&res1);
            
            if (flint) {
            
                switch (res->typeData) {
                    case TypeDataInt :
                        
                        switch (res1.typeData) {
                            case TypeDataInt :
                                if (t == TMultiply)
                                    res->dataValue.dataAsInt *= res1.dataValue.dataAsInt;
                                else if (t == TDiv)
                                    res->dataValue.dataAsInt /= res1.dataValue.dataAsInt;
                                else if (t == TMod)
                                    res->dataValue.dataAsInt %= res1.dataValue.dataAsInt;
                                break;
                                
                            case TypeDataFloat :
                                res->typeData = TypeDataFloat;
                                res->dataValue.dataAsFloat = res->dataValue.dataAsInt;
                                if (t ==TMultiply)
                                    res->dataValue.dataAsFloat *= res1.dataValue.dataAsFloat;
                                else if (t == TDiv)
                                    res->dataValue.dataAsFloat /= res1.dataValue.dataAsFloat;
                                else if (t == TMod)
                                    sc->printError("остаток от деления на float");
                                break;
                                
                            case TypeDataChar :
                                if (t == TMultiply)
                                    res->dataValue.dataAsInt *= res1.dataValue.dataAsChar;
                                else if (t == TDiv)
                                    res->dataValue.dataAsInt /= res1.dataValue.dataAsChar;
                                else if (t == TMod)
                                    res->dataValue.dataAsInt %= res1.dataValue.dataAsChar;
                                break;
                                
                            case TypeDataBool :
                                if (t == TMultiply)
                                    res->dataValue.dataAsInt *= res1.dataValue.dataAsBool;
                                else if (t == TDiv)
                                    res->dataValue.dataAsInt /= res1.dataValue.dataAsBool;
                                else if (t == TMod)
                                    res->dataValue.dataAsInt %= res1.dataValue.dataAsBool;
                                break;
                        }
                        break;
                        
                    case TypeDataFloat :
                        
                        switch (res1.typeData) {
                            case TypeDataInt :
                                if (t == TMultiply)
                                    res->dataValue.dataAsFloat *= res1.dataValue.dataAsInt;
                                else if (t == TDiv)
                                    res->dataValue.dataAsFloat /= res1.dataValue.dataAsInt;
                                else if (t == TMod)
                                    sc->printError("остаток от деления на float");
                                break;
                                
                            case TypeDataFloat :
                                if (t == TMultiply)
                                    res->dataValue.dataAsFloat *= res1.dataValue.dataAsFloat;
                                else if (t == TDiv)
                                    res->dataValue.dataAsFloat /= res1.dataValue.dataAsFloat;
                                else if (t == TMod)
                                    sc->printError("остаток от деления на float");
                                break;
                                
                            case TypeDataChar :
                                if (t == TMultiply)
                                    res->dataValue.dataAsFloat *= res1.dataValue.dataAsChar;
                                else if (t == TDiv)
                                    res->dataValue.dataAsFloat /= res1.dataValue.dataAsChar;
                                else if (t == TMod)
                                    sc->printError("остаток от деления на float");
                                break;
                                
                            case TypeDataBool :
                                if (t == TMultiply)
                                    res->dataValue.dataAsFloat *= res1.dataValue.dataAsBool;
                                else if (t == TDiv)
                                    res->dataValue.dataAsFloat /= res1.dataValue.dataAsBool;
                                else if (t == TMod)
                                    sc->printError("остаток от деления на float");
                                break;
                        }
                        break;
                        
                    case TypeDataChar :
                        
                        switch (res1.typeData) {
                            case TypeDataInt :
                                if (t == TMultiply)
                                    res->dataValue.dataAsChar *= res1.dataValue.dataAsInt;
                                else if (t == TDiv)
                                    res->dataValue.dataAsChar /= res1.dataValue.dataAsInt;
                                else if (t == TMod)
                                    res->dataValue.dataAsChar %= res1.dataValue.dataAsInt;
                                break;
                                
                            case TypeDataFloat :
                                res->typeData = TypeDataFloat;
                                res->dataValue.dataAsFloat = res->dataValue.dataAsChar;
                                if (t == TMultiply)
                                    res->dataValue.dataAsFloat *= res1.dataValue.dataAsFloat;
                                else if (t == TDiv)
                                    res->dataValue.dataAsFloat /= res1.dataValue.dataAsFloat;
                                else if (t == TMod)
                                    sc->printError("остаток от деления на float");
                                break;
                                
                            case TypeDataChar :
                                if (t == TMultiply)
                                    res->dataValue.dataAsChar *= res1.dataValue.dataAsChar;
                                else if (t == TDiv)
                                    res->dataValue.dataAsChar /= res1.dataValue.dataAsChar;
                                else if (t == TMod)
                                    res->dataValue.dataAsChar %= res1.dataValue.dataAsChar;
                                break;
                                
                            case TypeDataBool :
                                if (t == TMultiply)
                                    res->dataValue.dataAsChar *= res1.dataValue.dataAsBool;
                                else if (t == TDiv)
                                    res->dataValue.dataAsChar /= res1.dataValue.dataAsBool;
                                else if (t == TMod)
                                    res->dataValue.dataAsChar %= res1.dataValue.dataAsBool;
                                break;
                        }
                        break;
                        
                    case TypeDataBool :
                        
                        switch (res1.typeData) {
                            case TypeDataInt :
                                res->typeData = TypeDataInt;
                                res->dataValue.dataAsInt = res->dataValue.dataAsBool;
                                if (t == TMultiply)
                                    res->dataValue.dataAsInt *= res1.dataValue.dataAsInt;
                                else if (t == TDiv)
                                    res->dataValue.dataAsInt /= res1.dataValue.dataAsInt;
                                else if (t == TMod)
                                    res->dataValue.dataAsInt %= res1.dataValue.dataAsInt;
                                break;
                                
                            case TypeDataFloat :
                                res->typeData = TypeDataFloat;
                                res->dataValue.dataAsFloat = res->dataValue.dataAsBool;
                                if (t == TMultiply)
                                    res->dataValue.dataAsFloat *= res1.dataValue.dataAsFloat;
                                else if (t == TDiv)
                                    res->dataValue.dataAsFloat /= res1.dataValue.dataAsFloat;
                                else if (t == TMod)
                                    sc->printError("остаток от деления на float");
                                break;
                                
                            case TypeDataChar :
                                res->typeData = TypeDataChar;
                                res->dataValue.dataAsChar = res->dataValue.dataAsBool;
                                if (t == TMultiply)
                                    res->dataValue.dataAsChar *= res1.dataValue.dataAsChar;
                                else if (t == TDiv)
                                    res->dataValue.dataAsChar /= res1.dataValue.dataAsChar;
                                else if (t == TMod)
                                    res->dataValue.dataAsChar %= res1.dataValue.dataAsChar;
                                break;
                                
                            case TypeDataBool :
                                res->typeData = TypeDataInt;
                                res->dataValue.dataAsInt = res->dataValue.dataAsBool;
                                if (t == TMultiply)
                                    res->dataValue.dataAsInt *= res1.dataValue.dataAsInt;
                                else if (t == TDiv)
                                    res->dataValue.dataAsInt /= res1.dataValue.dataAsInt;
                                else if (t == TMod)
                                    res->dataValue.dataAsInt %= res1.dataValue.dataAsInt;
                                break;
                        }
                        break;
                        
                } // конец первого switch
                
            } // if flint
            
            tmpUk = sc->getUK(); t = sc->Scanner(lex);
        }
        sc->setUK(tmpUk);
    }
    
    
    void A5(TData *res) {
        TypeLex lex;
        int t, tmpUk;
        
        tmpUk = sc->getUK(); t = sc->Scanner(lex);
        if (t == TOpenRoundBracket) {
            A1(res);
            t = sc->Scanner(lex);
            if (t != TCloseRoundBracket) {
                sc->printError("ожидался символ \')\'", lex);
            }
        } else {
            sc->setUK(tmpUk);
            operand(res);
        }
        
    }
    
    
    void operand(TData *res) {
        TypeLex lex;
        int t, tmpUk;
        TypeLex ident;
        
        bool isPrefPlusPlus = false;
        bool isSuffPlusPlus = false;
        bool isFunctionCall = false;
        
        tmpUk = sc->getUK(); t = sc->Scanner(lex);
        if (t == TPlusPlus || t == TMinusMinus) {
            isPrefPlusPlus = true;
            tmpUk = sc->getUK(); t = sc->Scanner(lex);
        }
        
        strcpy(ident, lex);
        if (t == TConstInt || t == TConstChar) {
            sc->setUK(tmpUk);
            
            // тип и значение константы
            constant(res);
            
        } else if (t == TMain) {
            sc->setUK(tmpUk);
            
            // тип и значение функции main
            functionCall(res);
            
            isFunctionCall = true;
        } else if (t == TId) {
            int uk1 = sc->getUK(); t = sc->Scanner(lex);
            if (t == TOpenRoundBracket) {
                sc->setUK(tmpUk);
                
                // тип и значение фунуции
                functionCall(res);
                
                isFunctionCall = true;
            } else if (t == TOpenSquareBracket) {
                Tree *v = root->semGetArray(ident, sc);
                
                // вычесление индекса массива
                int index = 0;
                TData arrayIndex;
                A1(&arrayIndex);
                if (flint) {
                    switch (arrayIndex.typeData) {
                        case TypeDataFloat : arrayIndex.dataValue.dataAsInt = arrayIndex.dataValue.dataAsFloat; break;
                        case TypeDataChar : arrayIndex.dataValue.dataAsInt = arrayIndex.dataValue.dataAsChar; break;
                        case TypeDataBool : arrayIndex.dataValue.dataAsInt = arrayIndex.dataValue.dataAsBool; break;
                    }
                    arrayIndex.typeData = TypeDataInt;
                    index = arrayIndex.dataValue.dataAsInt;
                    
                    // проверка на выход за границу массива
                    if (index < 0 || index >= v->node->sizeArray)
                        sc->printException("выход за границу массива", index);
                }
                
                t = sc->Scanner(lex);
                if (t != TCloseSquareBracket) {
                    sc->printError("ожидался символ \']\'", lex);
                }
                
                // тип (char или float) и значение элемента массива
                if (flint) {
                    res->typeData = v->node->typeData;
                    switch (res->typeData) {
                        case TypeDataChar : res->dataValue.dataAsChar = v->node->dataValue.arrayDataAsChar[index]; break;
                        case TypeDataFloat : res->dataValue.dataAsFloat = v->node->dataValue.arrayDataAsFloat[index]; break;
                    }
                }
                
            } else {
                Tree *v = root->semGetType(ident, sc);
                
                // тип и значение идентификатора
                if (flint) {
                    res->typeData = v->node->typeData;
                    res->dataValue = v->node->dataValue;
                }
                
                sc->setUK(uk1);
            }
        } else
            sc->printError("ожидалcя идентификатор или константа", lex);
        
        tmpUk = sc->getUK(); t = sc->Scanner(lex);
        if (t == TPlusPlus || t == TMinusMinus) {
            if (isPrefPlusPlus)
                sc->printError("ну так же нельзя", lex);
            else
                isSuffPlusPlus = true;
        } else {
            sc->setUK(tmpUk);
        }
        
        if (isFunctionCall && (isPrefPlusPlus || isSuffPlusPlus)) {
            sc->printError("ожидался идентификатор или элемент одномерного массива");
        }
        
    }
    
    
    int constant(TData *res) {
        TypeLex lex;
        int t;
        
        t = sc->Scanner(lex);
        if (t != TConstInt && t != TConstChar) {
            sc->printError("ожидалась целая или символьная константа", lex);
        }
        
        if (t == TConstInt) {
            if (flint) {
                res->typeData = TypeDataInt;
                res->dataValue.dataAsInt = atoi(lex);
            }
        } else if (t == TConstChar) {
            if (flint) {
                res->typeData = TypeDataChar;
                res->dataValue.dataAsChar = lex[1];
            }
        }
        
        return root->getSizeArray(t, lex);
    }
    
    
    void block(TData *res) {
        TypeLex lex;
        int t, tmpUk;
        
        t = sc->Scanner(lex);
        if (t != TOpenCurlyBracket) {
            sc->printError("ожидался символ \'{\'", lex);
        }
        Tree *v = root->semIncludeBlock();
        operators(res);
        t = sc->Scanner(lex);
        if (t != TCloseCurlyBracket) {
            sc->printError("ожидался символ \'}\'", lex);
        }
        root->setCur(v);
    }
    
    
    void operators(TData *res) {
        TypeLex lex;
        int t, tmpUk;
        
        tmpUk = sc->getUK(); t = sc->Scanner(lex);
        sc->setUK(tmpUk);
        while (t == TFloat || t == TChar || t == TId || t == TDo || t == TOpenCurlyBracket || t == TReturn || t == TMain || t == TSemicolon) {
            if (t == TFloat || t == TChar) {
                description();
            } else {
                oneOperator(res);
            }
            tmpUk = sc->getUK();
            t = sc->Scanner(lex);
            sc->setUK(tmpUk);
        }
    }
    
    
    void oneOperator(TData *res) {
        TypeLex lex;
        int t, tmpUk;
        
        tmpUk = sc->getUK(); t = sc->Scanner(lex);
        if (t == TDo) {
            sc->setUK(tmpUk);
            doWhile();
        } else if (t == TOpenCurlyBracket) {
            sc->setUK(tmpUk);
            TData blockRes;
            block(&blockRes);
        } else if (t == TReturn) {
            sc->setUK(tmpUk);
            returnOperator(res);
        } else if (t == TMain) {
            sc->setUK(tmpUk);
            
            TData res;
            functionCall(&res);
            
        } else if (t == TId) {
            t = sc->Scanner(lex);
            if (t == TOpenRoundBracket) {
                sc->setUK(tmpUk);
                
                TData res;
                functionCall(&res);
                
                t = sc->Scanner(lex);
                if (t != TSemicolon) {
                    sc->printError("ожидался символ \';''", lex);
                }
            } else {
                sc->setUK(tmpUk);
                assignment();
            }
        } else if (t != TSemicolon) {
            sc->printError("ожидался символ \';\'", lex);
        }
    }
    
    
    void assignment() {
        TypeLex lex;
        int t, tmpUk;
        
        TypeLex ident;
        
        t = sc->Scanner(lex);
        strcpy(ident, lex);
        if (t != TId) {
            sc->printError("ожидался идентификатор", lex);
        }
        
        tmpUk = sc->getUK(); t = sc->Scanner(lex);
        Tree *v;
        TData arrayIndex;
        if (t == TOpenSquareBracket) {
            v = root->semGetArray(ident, sc);
            
            A1(&arrayIndex);
            if (flint) {
                switch (arrayIndex.typeData) {
                    case TypeDataFloat : arrayIndex.dataValue.dataAsInt = arrayIndex.dataValue.dataAsFloat; break;
                    case TypeDataChar : arrayIndex.dataValue.dataAsInt = arrayIndex.dataValue.dataAsChar; break;
                    case TypeDataBool : arrayIndex.dataValue.dataAsInt = arrayIndex.dataValue.dataAsBool; break;
                }
                arrayIndex.typeData = TypeDataInt;
                
                int index = arrayIndex.dataValue.dataAsInt;
                if (index < 0 || index >= v->node->sizeArray) sc->printException("выход за границу массива", index);
            }
            
            t = sc->Scanner(lex);
            if (t != TCloseSquareBracket) {
                sc->printError("ожидался символ \']\'", lex);
            }
        } else {
            v = root->semGetType(ident, sc);
            sc->setUK(tmpUk);
        }
        
        t = sc->Scanner(lex);
        if (t != TAssignment) {
            sc->printError("ожидался символ \'=\'", lex);
        }
        
        // результат выражения
        TData res;
        A1(&res);
        
        // записываем результат выражения
        if (flint) {
            if (v->node->typeNode == TypeNodeVar) {
                switch (v->node->typeData) {
                    case TypeDataChar :
                        switch (res.typeData) {
                            case TypeDataInt : v->node->dataValue.dataAsChar = (char) res.dataValue.dataAsInt; break;
                            case TypeDataFloat : v->node->dataValue.dataAsChar = (char) res.dataValue.dataAsFloat; break;
                            case TypeDataChar : v->node->dataValue.dataAsChar = (char) res.dataValue.dataAsChar; break;
                            case TypeDataBool : v->node->dataValue.dataAsChar = (char) res.dataValue.dataAsBool; break;
                        }
                        break;
                        
                    case TypeDataFloat :
                        switch (res.typeData) {
                            case TypeDataInt : v->node->dataValue.dataAsFloat = res.dataValue.dataAsInt; break;
                            case TypeDataFloat : v->node->dataValue.dataAsFloat = res.dataValue.dataAsFloat; break;
                            case TypeDataChar : v->node->dataValue.dataAsFloat = res.dataValue.dataAsChar; break;
                            case TypeDataBool : v->node->dataValue.dataAsFloat = res.dataValue.dataAsBool; break;
                        }
                        break;
                }
            } else if (v->node->typeNode == TypeNodeArray) {
                int index = arrayIndex.dataValue.dataAsInt;
                switch (v->node->typeData) {
                    case TypeDataChar :
                        switch (res.typeData) {
                            case TypeDataInt : v->node->dataValue.arrayDataAsChar[index] = (char) res.dataValue.dataAsInt; break;
                            case TypeDataFloat : v->node->dataValue.arrayDataAsChar[index] = (char) res.dataValue.dataAsFloat; break;
                            case TypeDataChar : v->node->dataValue.arrayDataAsChar[index] = (char) res.dataValue.dataAsChar; break;
                            case TypeDataBool : v->node->dataValue.arrayDataAsChar[index] = (char) res.dataValue.dataAsBool; break;
                        }
                        break;
                        
                    case TypeDataFloat :
                        switch (res.typeData) {
                            case TypeDataInt : v->node->dataValue.arrayDataAsFloat[index] = res.dataValue.dataAsInt; break;
                            case TypeDataFloat : v->node->dataValue.arrayDataAsFloat[index] = res.dataValue.dataAsFloat; break;
                            case TypeDataChar : v->node->dataValue.arrayDataAsFloat[index] = res.dataValue.dataAsChar; break;
                            case TypeDataBool : v->node->dataValue.arrayDataAsFloat[index] = res.dataValue.dataAsBool; break;
                        }
                        break;
                }
            }
        }
        
        t = sc->Scanner(lex);
        if (t != TSemicolon) {
            sc->printError("ожидался символ \';\'", lex);
        }
        
    } // присваивание
    
    
    void doWhile() {
        TypeLex lex;
        int t;
        
        t = sc->Scanner(lex);
        if (t != TDo) {
            sc->printError("ожидался do", lex);
        }
        
        int localFlint = flint;
        int uk1 = sc->getUK();
        
    Start :
        
        TData res;
        oneOperator(&res);
        
        t = sc->Scanner(lex);
        if (t != TWhile) {
            sc->printError("ожидался while", lex);
        }
        
        t = sc->Scanner(lex);
        if (t != TOpenRoundBracket) {
            sc->printError("ожидался символ \'(\'", lex);
        }
        
        A1(&res);
        
        t = sc->Scanner(lex);
        if (t != TCloseRoundBracket) {
            sc->printError("ожидался символ \')\'", lex);
        }
        
        t = sc->Scanner(lex);
        if (t != TSemicolon) {
            sc->printError("ожидался символ \';\'", lex);
        }
        
        bool valueResIsTrue = false;
        switch (res.typeData) {
            case TypeDataInt : valueResIsTrue = (bool) res.dataValue.dataAsInt; break;
            case TypeDataFloat : valueResIsTrue = (bool) res.dataValue.dataAsFloat; break;
            case TypeDataChar : valueResIsTrue = (bool) res.dataValue.dataAsChar; break;
            case TypeDataBool : valueResIsTrue = res.dataValue.dataAsBool; break;
        }
        
        if (flint && valueResIsTrue)
            flint = true;
        else
            flint = false;
        
        if (flint) {
            sc->setUK(uk1);
            goto Start;
        }
        
        flint = localFlint;
    }
    
    
    void returnOperator(TData *res) {
        TypeLex lex;
        int t;
        
        t = sc->Scanner(lex);
        if (t != TReturn) {
            sc->printError("ожидался return", lex);
        }
        
        A1(res);
        
        t = sc->Scanner(lex);
        if (t != TSemicolon) {
            sc->printError("ожидался символ \';\'", lex);
        }
    }
    
    
    void functionCall(TData *res) {
        TypeLex lex;
        int t;
        
        TypeLex ident;
        
        t = sc->Scanner(lex);
        strcpy(ident, lex);
        if (t != TId && t != TMain) {
            sc->printError("ожидался идентификатор или main", lex);
        }
        
        Tree *v = root->semGetFunc(ident, sc);
        
        t = sc->Scanner(lex);
        if (t != TOpenRoundBracket) {
            sc->printError("ожидался символ \'(\'", lex);
        }
        
        t = sc->Scanner(lex);
        if (t != TCloseRoundBracket) {
            sc->printError("ожидался символ \')\'", lex);
        }
        
        int tmpUk = sc->getUK();
        sc->setUK(v->node->startFunction);
        if (flint) {
            block(res);
            v->node->typeData = res->typeData;
            v->node->dataValue = res->dataValue;
        }
        sc->setUK(tmpUk);
        
    }
    
    void outTree(bool printEmpty) {
        root->printTree(printEmpty);
    }
    
    TSyntax(TScanner *s) {
        sc = s;
        Node *node = new Node();
        strcpy(node->id, "###");
        root = new Tree(NULL, NULL, NULL, node);
        Tree::cur = root;
    }
    
    ~TSyntax() {};
};

#endif