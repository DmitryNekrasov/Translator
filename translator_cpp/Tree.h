//
//  Tree.h
//  translator_cpp
//
//  Created by Дмитрий Некрасов on 31.10.14.
//  Copyright (c) 2014 Дмитрий Некрасов. All rights reserved.
//

#ifndef translator_cpp_Tree_h
#define translator_cpp_Tree_h

#include "defs.h"
#include "Scanner.h"

struct Node {
    
    int typeNode; // пустой, переменная, массив, функция
    TypeLex id;
    int typeData; // тип (float, char)
    DataValue dataValue; // значение
    
    int sizeArray;
    int startFunction; // начало тела функции
    
    Node() {}
    Node(int _typeNode) {
        typeNode = _typeNode;
    }
};


class Tree {
private :
    
public :
    
    Node *node; // данные таблицы
    Tree *up, *left, *right; // родитель, левый и правый потомок
    
    static Tree *cur; // текущий элемент дерева
    
    
    // ------------------------ Функции обработки бинарного дерева ------------------------
    
    Tree(Tree *_left, Tree *_right, Tree *_up, Node *_node) {
        node = new Node();
        up = _up;
        left = _left;
        right = _right;
        memcpy(node, _node, sizeof(Node));
    }
    
    Tree() {
        node = new Node(TypeNodeEmpty);
        up = NULL;
        left = NULL;
        right = NULL;
        
    }
    
    ~Tree() {}
    
    void setLeft(Node *data) { // создать левого потомка от текущей вершины this
        Tree *a = new Tree(NULL, NULL, this, data);
        left = a;
    }
    
    void setRight(Node *data) { // создать правого потомка от текущей вершины this
        Tree *a = new Tree(NULL, NULL, this, data);
        right = a;
    }
    
    Tree* findUp(Tree *from, TypeLex id) { // поиск данных в дереве от заданной вершины from до корня
        Tree *current = from;
        while (current != NULL && memcmp(id, current->node->id, max(strlen(id), strlen(current->node->id))) != 0) {
            current = current->up;
        }
        return current;
    }
    
    Tree* findUp(TypeLex id) { // поиск данных в дереве от текущей вершины this до корня
        return findUp(this, id);
    }
    
    Tree* findRightLeft(Tree *from, TypeLex id) { // поиск прямых потомков заданной вершины from
        Tree *current = from->right;
        while (current != NULL && memcmp(id, current->node->id, max(strlen(id), strlen(current->node->id))) != 0) {
            current = current->left;
        }
        return current;
    }
    
    Tree* findRightLeft(TypeLex id) { // поиск прямых потомков текущей вершины this
        return findUp(this, id);
    }
    
    Tree* findUpOneLevel(Tree *from, TypeLex id) { // поиск данных в дереве от заданной вершины from на одном уровне вложенности
        Tree *current = from;
        while (current != NULL && current->up != NULL && current->up->right != current) { // добавлен фикс current->up != NULL
            if (memcmp(id, current->node->id, max(strlen(id), strlen(current->node->id))) == 0) {
                return current;
            }
            current = current->up;
        }
        return NULL;
    }
    
    void printTree(bool printEmpty) { // отладочная программа печати дерева для интерпретатора
        
        if (printEmpty || node->typeNode != TypeNodeEmpty) {
        
            printf("Вершина с данными %s ", node->id);
            
            if (node->typeNode == TypeNodeVar || node->typeNode == TypeNodeFunction) {
                printf("(значение ");
                switch (node->typeData) {
                    case TypeDataChar : printf("%d '%c'", node->dataValue.dataAsChar, node->dataValue.dataAsChar); break;
                    case TypeDataFloat : printf("%f", node->dataValue.dataAsFloat); break;
                }
                printf(") ");
            }
            
            if (node->typeNode == TypeNodeArray) {
                printf("(размер массива %d) [", node->sizeArray);
                switch (node->typeData) {
                    case TypeDataChar :
                        for (int i = 0; i < node->sizeArray - 1; i++)
                            printf("%d, ", node->dataValue.arrayDataAsChar[i]);
                        printf("%d", node->dataValue.arrayDataAsChar[node->sizeArray - 1]);
                        
                        printf("] [");
                        for (int i = 0; i < node->sizeArray - 1; i++)
                            printf("'%c', ", node->dataValue.arrayDataAsChar[i]);
                        printf("'%c'", node->dataValue.arrayDataAsChar[node->sizeArray - 1]);
                        
                        break;
                        
                    case TypeDataFloat :
                        for (int i = 0; i < node->sizeArray - 1; i++)
                            printf("%f, ", node->dataValue.arrayDataAsFloat[i]);
                        printf("%f", node->dataValue.arrayDataAsFloat[node->sizeArray - 1]);
                        break;
                }
                printf("] ");
            }
            
            if (printEmpty) {
                printf("---> ");
                
                if (left != NULL) {
                    printf("слева данные %s", left->node->id);
                }
                if (right != NULL) {
                    printf(" справа данные %s", right->node->id);
                }
            }
            
            printf("\n");
            
        }
        
        if (left != NULL)
            left->printTree(printEmpty);
        if (right != NULL)
            right->printTree(printEmpty);
    }
    
    void printTree() { // отладочная программа печати дерева
        
        printf("Вершина с данными %s ", node->id);
        
        if (node->typeNode == TypeNodeArray) {
            printf("(размер массива %d) ", node->sizeArray);
        }
    
        printf("---> ");
        
        if (left != NULL) {
            printf("слева данные %s", left->node->id);
        }
        if (right != NULL) {
            printf(" справа данные %s", right->node->id);
        }
        
        printf("\n");
        
        if (left != NULL)
            left->printTree();
        if (right != NULL)
            right->printTree();
    }
    
    
    // ------------------------ Семантические подпрограммы ------------------------
    
    void controlStringConst(Tree *addr, TypeLex str, TScanner *sc) {
        if (addr->node->sizeArray < strlen(str) - 1)
            sc->printError("слишком длинная строковая константа", str);
    }
    
    void controlCountElements(Tree *addr, int countElements, TScanner *sc) {
        if (addr->node->sizeArray < countElements)
            sc->printError("инициализация не соответствует размерности массива", addr->node->id);
    }
    
    void setCur(Tree *a) {
        cur = a;
    }
    
    Tree* getCur() {
        return cur;
    }
    
    int dupControl(Tree *addr, TypeLex id) { // проверка идентификатора на повторное описание внутри блока
        if (findUpOneLevel(addr, id) == NULL)
            return 0;
        return 1;
    }
    
    Tree* semInclude(TypeLex id, int typeNode, int typeData, TScanner *sc) { // занесение идентификатора в таблицу
        if (dupControl(cur, id))
            sc->printError("повторное описание идентификатора", id);
        Tree *v;
        Node node;
        if (typeNode != TypeNodeFunction) {
            memcpy(node.id, id, strlen(id) + 1);
            node.typeNode = typeNode;
            node.typeData = typeData;
            cur->setLeft(&node);
            cur = cur->left;
            return cur;
        } else {
            memcpy(node.id, id, strlen(id) + 1);
            node.typeNode = typeNode;
            node.typeData = typeData;
            cur->setLeft(&node);
            cur = cur->left;
            v = cur; // точка возврата после выхода из функции
            Node emptyNode;
            emptyNode.typeNode = TypeNodeEmpty;
            strcpy(emptyNode.id, "###");
            cur->setRight(&emptyNode);
            cur = cur->right;
            return v;
        }
    }
    
    Tree* semIncludeBlock() {
        Tree *v;
        Node emptyNode1, emptyNode2;
        emptyNode1.typeNode = TypeNodeEmpty;
        emptyNode2.typeNode = TypeNodeEmpty;
        strcpy(emptyNode1.id, "###");
        strcpy(emptyNode2.id, "###");
        cur->setLeft(&emptyNode1);
        cur = cur->left;
        v = cur;
        cur->setRight(&emptyNode2);
        cur = cur->right;
        return v;
    }
    
    void semSetTypeNode(Tree *addr, int typeNode) {
        addr->node->typeNode = typeNode;
    }
    
    void semSetTypeData(Tree *addr, int typeData) {
        addr->node->typeData = typeData;
    }
    
    void semSetSizeArray(Tree *addr, int sizeArray) {
        addr->node->sizeArray = sizeArray;
    }
    
    Tree* semGetType(TypeLex a, TScanner *sc) { // найти в таблице переменную с именем а
        Tree *v = findUp(cur, a);
        if (v == NULL)
            sc->printError("отсутствует описание идентификатора", a);
        if (v->node->typeNode == TypeNodeFunction)
            sc->printError("неверное использование вызова функции", a);
        if (v->node->typeNode == TypeNodeArray)
            sc->printError("идентификатор является элементом массива", a);
        return v;
    }
    
    Tree* semGetArray(TypeLex a, TScanner * sc) {
        Tree *v = findUp(cur, a);
        if (v == NULL)
            sc->printError("отсутствует описание массива", a);
        if (v->node->typeNode != TypeNodeArray)
            sc->printError("идентификатор не является элементом массива", v->node->id);
        return v;
    }
    
    Tree* semGetFunc(TypeLex a, TScanner *sc) { // найти в таблице функцию с именем a
        Tree *v = findUp(cur, a);
        if (v == NULL)
            sc->printError("отсутствует описание функции", a);
        if (v->node->typeNode != TypeNodeFunction)
            sc->printError("не является функцией идентификатор", a);
        return v;
    }
    
    void controlFloatArrayAssignment(TScanner *sc) {
        if (cur->node->typeData == TFloat) {
            sc->printError("нельзя присвваивать в массив типа float");
        }
    }
    
    int getSizeArray(int t, TypeLex lex) {
        int sizeArray;
        if (t == TConstInt)
            sizeArray = stringToInt(lex);
        else
            sizeArray = charToInt(lex);
        return sizeArray;
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
    
};

Tree *Tree::cur;

#endif