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
    
//    cout << "\nДерево:\n";
//    ll1->outTree();
//    
//    cout << "\nR:\n";
//    ll1->outOperands();
//    cout << "\n";
    
    cout << "Триады:\n";
    ll1->outTriads();
    
//    ll1->optimization();
//    
//    cout << "\nОптимизированные триады:\n";
//    ll1->outTriads();
    
    cout << "\nASM:\n";
    ll1->generateAsm();
    
    printf("\nСинтаксических и семантических ошибок не найдено\n");
    
    return 0;
}