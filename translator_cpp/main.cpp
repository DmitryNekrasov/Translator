#include <stdio.h>
#include "defs.h"
#include "Scanner.h"
#include "Syntax.h"
#include "Tree.h"
#include "LL1.h"

int main() {
    
//    TSyntax *syntax = new TSyntax(new TScanner("input.cpp"));
//    syntax->program();
//    
//    syntax->outTree(false); // true - выводить пустые узлы
//    
//    printf("\nСинтаксических и семантических ошибок не найдено\n");
    
    LL1 *ll1 = new LL1(new TScanner("input.cpp"));
    ll1->LL_1();
    
    printf("\nСинтаксических ошибок не найдено\n");
    
    return 0;
}