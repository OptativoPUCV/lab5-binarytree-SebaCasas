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
  if (node == NULL)
    return;

  if (node->left == NULL && node->right == NULL){
    if (node->parent == NULL)
      tree->root = NULL;
    else{
      if (node->parent->left == node)
        node->parent->left = NULL;
      else
        node->parent->right = NULL;
    }
    free(node);
    return;
  }

  if (node->left == NULL || node->right == NULL){
    TreeNode* child = (node->left != NULL) ? node->left : node->right;
    child->parent = node->parent;
    if (node->parent == NULL)
      tree->root = child;
    else{
      if (node->parent->left == node)
        node->parent->left = child;
      else
        node->parent->right = child;
    }
    free(node);
    return;
  }

  TreeNode* successor = node->right;
  while (successor->left != NULL)
    successor = successor->left;
  
  node->pair = successor->pair;
  removeNode(tree, successor);
}

void eraseTreeMap(TreeMap * tree, void* key){
  if (tree == NULL || tree->root == NULL) 
    return;
  if (searchTreeMap(tree, key) == NULL) 
    return;
  
  TreeNode* node = tree->current;
  removeNode(tree, node);
}

Pair * searchTreeMap(TreeMap * tree, void* key) {
  tree->current = tree->root;
  
  while(tree->current != NULL){
    if(is_equal(tree, key, tree->current->pair->key) == 1)
      return tree->current->pair;
    
    if(tree->lower_than(key,tree->current->pair->key) == 0)
      tree->current = tree->current->right;
    else tree->current = tree->current->left;
  }
  return NULL;
}

Pair * upperBound(TreeMap * tree, void* key) {
  if (tree == NULL || tree->root == NULL)
    return NULL;

  TreeNode* current = tree->root;
  TreeNode* result = NULL;

  while (current != NULL) {
    if (tree->lower_than(current->pair->key, key) == 0){
      result = current;
      current = current->left;
    } 
    else current = current->right;
  }
  if (result != NULL)
    return result->pair;
  else
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
  TreeNode *temp = minimum(tree->root);
  return temp->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
  if (tree == NULL || tree->root == NULL)
    return NULL;

  TreeNode* current = tree->current;
  if (current == NULL){
    current = tree->root;
    while (current->left != NULL)
      current = current->left;
  } 
  else {
    if (current->right != NULL){
      current = current->right;
      while (current->left != NULL)
        current = current->left;
    }
    else {
      TreeNode* parent = current->parent;
      while (parent != NULL && current == parent->right){
        current = parent;
        parent = parent->parent;
      }
      current = parent;
    }
  }

  tree->current = current;
  if (current != NULL)
    return current->pair;
  else
    return NULL;
}