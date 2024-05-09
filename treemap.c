#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
  TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
  new->root = NULL;
  new->current = NULL;
  new->lower_than = lower_than;
  return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
  TreeNode* newNode = createTreeNode(key, value);
  TreeNode* current = tree->root;
  TreeNode* parent = NULL;

  while(current != NULL){
    if(is_equal(tree, key, current->pair->key)) return;
    parent = current;
    if(tree->lower_than(key, current->pair->key) == 1){
      current = current->left;
    }
    else current = current->right;
  }

  newNode->parent = parent;
  if(tree->lower_than(key, parent->pair->key) == 1){
    parent->left = newNode;
  }
  else parent->right = newNode;
  tree->current = newNode;
}

TreeNode * minimum(TreeNode * x){
  while(x->left != NULL) {
    x = x->left;
  }
  return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) {
  tree->current = tree->root;
  
  while(tree->current != NULL) {
    if(is_equal(tree, key, tree->current->pair->key) == 0){
      return tree->current->pair;
    }
    else if(tree->lower_than(key,tree->current->pair->key) == 0)
      tree->current = tree->current->right;
    
    else if(tree->lower_than(key,tree->current->pair->key) == 1)
      tree->current = tree->current->left;
  }
  
  return NULL;  // Retorna NULL si la clave no se encuentra
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
  return tree->root->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
  return NULL;
}
