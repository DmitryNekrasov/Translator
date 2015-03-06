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


class TSyntax {
private :
    TScanner *sc;
public :

    Tree *root;
    
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
        
        do {
            t = sc->Scanner(lex);
            if (t != TId && t != TMain)
                sc->printError("ожидался идентификатор или Main", lex);
            
            copyLex(ident, lex);
            
            tmpUk = sc->getUK(); t = sc->Scanner(lex);
            if (t == TAssignment) {
                Tree *v = root->semInclude(ident, TypeNodeVar, typeData, sc);
                A1();
                tmpUk = sc->getUK(); t = sc->Scanner(lex);
                
            } else if (t == TOpenSquareBracket) {
                Tree *v = root->semInclude(ident, TypeNodeArray, typeData, sc);
                root->semSetSizeArray(v, constant());
                t = sc->Scanner(lex);
                if (t != TCloseSquareBracket)
                    sc->printError("ожидался символ \']\'", lex);
                
                tmpUk = sc->getUK(); t = sc->Scanner(lex);
                if (t == TAssignment) {
                    t = sc->Scanner(lex);
                    if (t == TConstString) {
                        root->controlStringConst(v, lex, sc);
                        tmpUk = sc->getUK(); t = sc->Scanner(lex);
                    } else if (t == TOpenCurlyBracket) {
                        int countElements = 0;
                        do {
                            constant();
                            countElements++;
                            tmpUk = sc->getUK(); t = sc->Scanner(lex);
                        } while (t == TComma);
                        root->controlCountElements(v, countElements, sc);
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
                block();
                root->setCur(v);
                tmpUk = sc->getUK(); t = sc->Scanner(lex);
            } else {
                Tree *v = root->semInclude(ident, TypeNodeVar, typeData, sc);
            }
            
        } while (t == TComma);
        sc->setUK(tmpUk);
        
        t = sc->Scanner(lex);
        if (t != TSemicolon)
            sc->printError("ожидался символ \';\'", lex);
    } // описание
    
    
    void A1() {
        TypeLex lex;
        int t, tmpUk;
        
        A2();
        tmpUk = sc->getUK(); t = sc->Scanner(lex);
        while (t == TMore || t == TLess || t == TMoreOrEquals || t == TLessOrEquals || t == TEquals || t == TNoEquals) {
            A2();
            tmpUk = sc->getUK(); t = sc->Scanner(lex);
        }
        sc->setUK(tmpUk);
    } // выражение
    
    
    void A2() {
        TypeLex lex;
        int t, tmpUk;
        
        tmpUk = sc->getUK(); t = sc->Scanner(lex);
        if (t != TPlus && t != TMinus)
            sc->setUK(tmpUk);
        
        A3();
        tmpUk = sc->getUK(); t = sc->Scanner(lex);
        while (t == TPlus || t == TMinus) {
            A3();
            tmpUk = sc->getUK(); t = sc->Scanner(lex);
        }
        sc->setUK(tmpUk);
    }
    
    
    void A3() {
        TypeLex lex;
        int t, tmpUk;
        
        A4();
        tmpUk = sc->getUK(); t = sc->Scanner(lex);
        while (t == TMultiply || t == TDiv || t == TMod) {
            A4();
            tmpUk = sc->getUK(); t = sc->Scanner(lex);
        }
        sc->setUK(tmpUk);
    }
    
    
    void A4() {
        TypeLex lex;
        int t, tmpUk;
        
        tmpUk = sc->getUK(); t = sc->Scanner(lex);
        if (t == TPlusPlus || t == TMinusMinus) {
            A5();
        } else {
            sc->setUK(tmpUk);
            A5();
            tmpUk = sc->getUK(); t = sc->Scanner(lex);
            if (t != TPlusPlus && t != TMinusMinus)
                sc->setUK(tmpUk);
        }
    }
    
    
    void A5() {
        TypeLex lex;
        int t, tmpUk;
        
        tmpUk = sc->getUK(); t = sc->Scanner(lex);
        if (t == TOpenRoundBracket) {
            A1();
            t = sc->Scanner(lex);
            if (t != TCloseRoundBracket) {
                sc->printError("ожидался символ \')\'", lex);
            }
        } else {
            sc->setUK(tmpUk);
            operand();
        }
        
    }
    
    
    void operand() {
        TypeLex lex;
        int t, tmpUk;
        TypeLex ident;
        tmpUk = sc->getUK(); t = sc->Scanner(lex);
        strcpy(ident, lex);
        if (t == TConstInt || t == TConstChar) {
            sc->setUK(tmpUk);
            constant();
        } else if (t == TMain) {
            sc->setUK(tmpUk);
            functionCall();
        } else if (t == TId) {
            int uk1 = sc->getUK(); t = sc->Scanner(lex);
            if (t == TOpenRoundBracket) {
                sc->setUK(tmpUk);
                functionCall();
            } else if (t == TOpenSquareBracket) {
                Tree *v = root->semGetArray(ident, sc);
                A1();
                t = sc->Scanner(lex);
                if (t != TCloseSquareBracket) {
                    sc->printError("ожидался символ \']\'", lex);
                }
            } else {
                Tree *v = root->semGetType(ident, sc);
                sc->setUK(uk1);
            }
        } else
            sc->printError("ожидалcя идентификатор или константа", lex);
        
    }
    
    
    int constant() {
        TypeLex lex;
        int t;
        
        t = sc->Scanner(lex);
        if (t != TConstInt && t != TConstChar) {
            sc->printError("ожидалась целая или символьная константа", lex);
        }
        return getSizeArray(t, lex);
    }
    
    
    void block() {
        TypeLex lex;
        int t, tmpUk;
        
        t = sc->Scanner(lex);
        if (t != TOpenCurlyBracket) {
            sc->printError("ожидался символ \'{\'", lex);
        }
        Tree *v = root->semIncludeBlock();
        operators();
        t = sc->Scanner(lex);
        if (t != TCloseCurlyBracket) {
            sc->printError("ожидался символ \'}\'", lex);
        }
        root->setCur(v);
    }
    
    
    void operators() {
        TypeLex lex;
        int t, tmpUk;
        
        tmpUk = sc->getUK(); t = sc->Scanner(lex);
        sc->setUK(tmpUk);
        while (t == TFloat || t == TChar || t == TId || t == TDo || t == TOpenCurlyBracket || t == TReturn || t == TMain || t == TSemicolon) {
            if (t == TFloat || t == TChar) {
                description();
            } else {
                oneOperator();
            }
            tmpUk = sc->getUK();
            t = sc->Scanner(lex);
            sc->setUK(tmpUk);
        }
    }
    
    
    void oneOperator() {
        TypeLex lex;
        int t, tmpUk;
        
        tmpUk = sc->getUK(); t = sc->Scanner(lex);
        if (t == TDo) {
            sc->setUK(tmpUk);
            doWhile();
        } else if (t == TOpenCurlyBracket) {
            sc->setUK(tmpUk);
            block();
        } else if (t == TReturn) {
            sc->setUK(tmpUk);
            returnOperator();
        } else if (t == TMain) {
            sc->setUK(tmpUk);
            functionCall();
        } else if (t == TId) {
            t = sc->Scanner(lex);
            if (t == TOpenRoundBracket) {
                sc->setUK(tmpUk);
                functionCall();
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
        if (t == TOpenSquareBracket) {
            Tree *v = root->semGetArray(ident, sc);
            A1();
            t = sc->Scanner(lex);
            if (t != TCloseSquareBracket) {
                sc->printError("ожидался символ \']\'", lex);
            }
        } else {
            Tree *v = root->semGetType(ident, sc);
            sc->setUK(tmpUk);
        }
        
        t = sc->Scanner(lex);
        if (t != TAssignment) {
            sc->printError("ожидался символ \'=\'", lex);
        }
        A1();
        t = sc->Scanner(lex);
        if (t != TSemicolon) {
            sc->printError("ожидался символ \';\'", lex);
        }
        
    } // присваисание
    
    
    void doWhile() {
        TypeLex lex;
        int t;
        
        t = sc->Scanner(lex);
        if (t != TDo) {
            sc->printError("ожидался do", lex);
        }
        
        oneOperator();
        
        t = sc->Scanner(lex);
        if (t != TWhile) {
            sc->printError("ожидался while", lex);
        }
        
        t = sc->Scanner(lex);
        if (t != TOpenRoundBracket) {
            sc->printError("ожидался символ \'(\'", lex);
        }
        
        A1();
        
        t = sc->Scanner(lex);
        if (t != TCloseRoundBracket) {
            sc->printError("ожидался символ \')\'", lex);
        }
        
        t = sc->Scanner(lex);
        if (t != TSemicolon) {
            sc->printError("ожидался символ \';\'", lex);
        }
    }
    
    
    void returnOperator() {
        TypeLex lex;
        int t;
        
        t = sc->Scanner(lex);
        if (t != TReturn) {
            sc->printError("ожидался return", lex);
        }
        
        A1();
        
        t = sc->Scanner(lex);
        if (t != TSemicolon) {
            sc->printError("ожидался символ \';\'", lex);
        }
    }
    
    
    void functionCall() {
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
    }
    
    int getSizeArray(int t, TypeLex lex) {
        if (t == TConstInt)
            return stringToInt(lex);
        return charToInt(lex);
    }
    
    int stringToInt(TypeLex str) {
        size_t len = strlen(str);
        int ans = 0;
        int p = 1;
        for (size_t i = len; i ; i--) {
            int q = (str[i - 1] - '0') * p;
            ans += q;
            p *= 10;
        }
        return ans;
    }
    
    int charToInt(TypeLex str) {
        return str[1];
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