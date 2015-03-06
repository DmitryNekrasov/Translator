//
//  defs.h
//  translator_cpp
//
//  Created by Дмитрий Некрасов on 02.10.14.
//  Copyright (c) 2014 Дмитрий Некрасов. All rights reserved.
//

#ifndef translator_cpp_defs_h
#define translator_cpp_defs_h

#include <stdio.h>
#include <string.h>
#include <iostream>

#define MAX_TEXT 100000 // Максимальная длина исходного модуля
#define MAX_LEX 32 // Максимальная длина лексемы
#define MAX_LEN_INT 10 // Максимальная длина целой константы
#define MAX_NUM_KEY_WORD 6 // максимальное количество ключевых слов
#define MAX_LEN_MAG 5005 // максимальный размер магазина для LL1 - анализатора

// Идентификатор
#define TId 1

// Константы
#define TConstInt 2
#define TConstChar 3
#define TConstString 4

// Знаки операций
#define TPlus 10
#define TMinus 11
#define TMultiply 12
#define TDiv 13
#define TMod 14
#define TAssignment 15
#define TPlusPlus 16
#define TMinusMinus 17
#define TMore 18
#define TLess 19
#define TMoreOrEquals 20
#define TLessOrEquals 21
#define TEquals 22
#define TNoEquals 23

// Скобки
#define TOpenRoundBracket 30
#define TCloseRoundBracket 31
#define TOpenCurlyBracket 32
#define TCloseCurlyBracket 33
#define TOpenSquareBracket 34
#define TCloseSquareBracket 35

// Разделители
#define TSemicolon 40
#define TComma 41

// Ключевые слова
#define TFloat 50
#define TChar 51
#define TDo 52
#define TWhile 53
#define TMain 54
#define TReturn 55

// Ошибочный символ
#define TError 60

// Конец исходного модуля
#define TEnd 61

// Типы ошибок
#define EWrongChar 1
#define EWrongCharConst 2
#define EWrongStringConst 3
#define ELongIntConst 4
#define ELongStringConst 5

// Типы предупреждений
#define WLongId 20

// Тип узла в дереве
#define TypeNodeEmpty 0
#define TypeNodeVar 1
#define TypeNodeArray 2
#define TypeNodeFunction 3

// Тип данных
#define TypeDataFloat 0
#define TypeDataChar 1

// Нетерминалы
#define netermProgram 100
#define netermI 101
#define netermJ 102
#define netermType 103
#define netermListVar 104
#define netermH 105
#define netermListItem 106
#define netermE 107
#define netermF 108
#define netermG 109
#define netermConstSeq 110
#define netermK 111
#define netermFunctionName 112
#define neterm 113
#define netermBlock 114
#define netermOperators 115
#define netermOneOperator 116
#define netermD 117
#define netermAssignment 118
#define netermItemForAssignment 119
#define netermExpression 120
#define netermA1 121
#define netermA11 122
#define netermA111 123
#define netermA2 124
#define netermA21 125
#define netermA211 126
#define netermA3 127
#define netermA31 128
#define netermA311 129
#define netermA4 130
#define netermA511 131
#define netermA512 132
#define netermA5 133
#define netermOperand 134
#define netermB 135
#define netermC 136
#define netermConst 137
#define netermDoWhile 138
#define netermReturn 139
#define netermFunctionCall 140

typedef char TypeMod[MAX_TEXT];
typedef char TypeLex[MAX_LEX];

union DataValue {
    float dataAsFloat;
    char dataAsChar;
};

using namespace std;

#endif