#include <stdio.h>
#include "defs.h"
#include "Scanner.h"
#include "Syntax.h"
#include "Tree.h"
#include "LL1.h"

int main() {
    
    TSyntax *syntax = new TSyntax(new TScanner("input.txt"));
    syntax->program();
    
    printf("Синтаксических и семантических ошибок не найдено\n");
    
//    LL1 *ll1 = new LL1(new TScanner("input.txt"));
//    ll1->LL_1();
//    
//    printf("Синтаксических ошибок не найдено\n");
    
    return 0;
}