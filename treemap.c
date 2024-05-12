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

  if (node->left == NULL && node->right == NULL) {
    if (node->parent == NULL) {
      // Si el nodo es la raíz
      tree->root = NULL;
    }
    else {
    // Eliminar referencia del padre al nodo
      if (node->parent->left == node)
        node->parent->left = NULL;
      else
        node->parent->right = NULL;
    }
    free(node); // Liberar memoria del nodo
    return;
  }

  // Caso 2: El nodo tiene un solo hijo
  if (node->left == NULL || node->right == NULL) {
    TreeNode* child = (node->left != NULL) ? node->left : node->right;

    // Actualizar el padre del hijo
    child->parent = node->parent;

    // Actualizar la raíz si es necesario
    if (node->parent == NULL)
      tree->root = child;
    else {
      // Actualizar la referencia del padre al hijo
      if (node->parent->left == node)
        node->parent->left = child;
      else
        node->parent->right = child;
    }
    free(node); // Liberar memoria del nodo
    return;
  }

  // Caso 3: El nodo tiene dos hijos
  // En este caso, necesitas encontrar el sucesor en orden del nodo para reemplazarlo.
  TreeNode* successor = node->right;
  while (successor->left != NULL)
    successor = successor->left;

  // Copiar el par del sucesor al nodo que se está eliminando
  node->pair = successor->pair;

  // Eliminar el sucesor recursivamente
  removeNode(tree, successor);
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
    if(is_equal(tree, key, tree->current->pair->key) == 1){
      return tree->current->pair;
    }
    if(tree->lower_than(key,tree->current->pair->key) == 0)
      tree->current = tree->current->right;
    
    else
      tree->current = tree->current->left;
  }
  
  return NULL;  // Retorna NULL si la clave no se encuentra
}

Pair * upperBound(TreeMap * tree, void* key) {
  if (tree == NULL || tree->root == NULL)
    return NULL;

  TreeNode* current = tree->root;
  TreeNode* result = NULL;

  while (current != NULL) {
    // Si la clave actual es mayor o igual que la clave dada, actualizamos el resultado y exploramos el subárbol izquierdo.
    if (tree->lower_than(current->pair->key, key) == 0) {
      result = current;
      current = current->left;
    } 
    else {
      // Si la clave actual es menor, exploramos el subárbol derecho.
      current = current->right;
    }
  }

  // Devolvemos el resultado encontrado.
  if (result != NULL)
    return result->pair;
  else
    return NULL; // No se encontró ninguna clave mayor o igual que la dada.
}

Pair * firstTreeMap(TreeMap * tree) {
  TreeNode *temp = minimum(tree->root);
  return temp->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
  if (tree == NULL || tree->root == NULL)
    return NULL;

  TreeNode* current = tree->current;

  // Si current es NULL, significa que aún no hemos comenzado el recorrido o hemos llegado al final.
  if (current == NULL) {
    // Buscar el nodo más izquierdo para empezar.
    current = tree->root;
    while (current->left != NULL)
    current = current->left;
  } 
  else {
    // Si current no es NULL, mover al siguiente nodo en orden.
    if (current->right != NULL) {
      // Si hay un hijo derecho, ir a la izquierda más profunda del hijo derecho.
      current = current->right;
      while (current->left != NULL)
                current = current->left;
    } 
    else {
      // Si no hay un hijo derecho, ir hacia arriba hasta que seamos el hijo izquierdo de nuestro padre.
      TreeNode* parent = current->parent;
            while (parent != NULL && current == parent->right) {
                current = parent;
                parent = parent->parent;
            }
          current = parent; // Aquí current puede ser NULL si hemos llegado a la raíz.
    }
  }

  // Actualizar el puntero current en el TreeMap.
  tree->current = current;

  // Devolver el par correspondiente al nodo actual.
  if (current != NULL)
    return current->pair;
  else
    return NULL; // Hemos llegado al final del árbol.
}
