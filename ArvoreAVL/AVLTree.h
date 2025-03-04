#ifndef _AVLTREE_H_
#define _AVLTREE_H_

    #include <stdio.h>
    #include <stdlib.h>

    typedef unsigned char boolean;
    
    typedef struct AVLNode {
        int data;
        struct AVLNode *left, *right;
        
    }AVLNode;

    typedef struct AVLTree {
        AVLNode *root;
        
        boolean     (*insert)(int);
        boolean     (*pop)(int);
        void        (*show)();
        void        (*prefix)();
        void        (*infix)();
        void        (*posfix)();
        void        (*destruct)();
    }AVLTree;

    extern AVLTree *new_AVLTree();
    extern void setInstance(AVLTree*);
    
#endif