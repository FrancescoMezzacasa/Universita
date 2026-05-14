#ifndef _TREE_H
#define _TREE_H

#include "nodes.h"

struct _bst {
    t_node root;
};
typedef struct _bst *bst;
bst make_bst (void);
void delete_node_cascade (t_node t);
void delete_bst (bst t);
void bst_insert (bst t, t_node n);

int bst_depth (bst t);

#endif