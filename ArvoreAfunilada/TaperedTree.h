#ifndef _TAPEREDTREE_H_
#define _TAPEREDTREE_H_

    typedef unsigned char bool;

    typedef struct TaperedTree{
        int data;
        struct TaperedTree *left;
        struct TaperedTree *right;
    } TaperedTree;


    extern TaperedTree *insert      (TaperedTree*, int);
    extern TaperedTree *pop         (TaperedTree*, int);
    extern TaperedTree *destruct    (TaperedTree*);
    extern TaperedTree *search      (TaperedTree*, int);
    extern void inOrder             (TaperedTree*);
    extern void show                (TaperedTree*);

#endif