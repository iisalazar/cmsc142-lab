// Salazar, Ian
// Santos, Justin Roy
// CMSC 142 EF-2L
// Exercise 10 - AVL Deletion
// April 29, 2024

#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#define N 10
#define BALANCED 0
#define LEFT_LEANING 1
#define RIGHT_LEANING 2

typedef struct node_tag{
	int x, height;
	struct node_tag *parent;
	struct node_tag *left;
	struct node_tag *right;
}avl_node;

void deleteNodeWithNoChild(avl_node** rootptr, avl_node* node, int key);
void deleteNodeWithOneChild(avl_node** rootptr, avl_node* node, int key);

int max(int a, int b){
	return(a>b?a:b);
}

void updateheight(avl_node *temp){
	// If the node is not NULL then update its height
	if(temp!=NULL)
		// Set the height of the node to be equal to height of its child node with greater height value + 1
		temp->height = max(temp->left==NULL?-1:temp->left->height,temp->right==NULL?-1:temp->right->height)+1;
}

// rootptr is passed by reference to this function so that this function will be able to edit its value
void left_rotate(avl_node **rootptr){
	// temporary node pointer
	avl_node *temp1;

	if(rootptr!=NULL && *rootptr!=NULL && (*rootptr)->right!=NULL){
		temp1 = (*rootptr)->right;
		
		(*rootptr)->right = temp1->left;
		if(temp1->left!=NULL)
			temp1->left->parent = (*rootptr);

		temp1->parent = (*rootptr)->parent;

		(*rootptr)->parent = temp1;
		temp1->left = (*rootptr);

		if(temp1->parent != NULL){
			if(temp1->parent->right == *rootptr)
				temp1->parent->right = temp1;
			else
				temp1->parent->left = temp1;
		}

		*rootptr = temp1;

		updateheight(temp1->left);
		updateheight(temp1);

	}

}

// **rootptr holds the memory address of the node pointer
void right_rotate(avl_node **rootptr){
	avl_node *temp1;

	if(rootptr!=NULL && *rootptr!=NULL && (*rootptr)->left!=NULL){
		// make temp node point to the left child of root node
		temp1 = (*rootptr)->left;

		// make the right child of the pivot node be the left child of the critical node
		(*rootptr)->left = temp1->right;

		if(temp1->right!=NULL)
			// If the pivot node has a right child,
			// then set the parent field of that right child to be equal to the critical node
			temp1->right->parent = (*rootptr);

		// make the parent of the pivot node be equal to the parent of the critical node
		temp1->parent = (*rootptr)->parent;

		// make pivot node be the parent of the critical node
		(*rootptr)->parent = temp1;
		temp1->right = (*rootptr);

		// if the pivot node, which is now the root of the subtree, has a parent node, or if it is not the root of the whole tree
		if(temp1->parent != NULL){
			// make the pivot node be the left child or right child of the previous parent of the critical node
			// depending on whether the critical node was a left or right child of its previous parent
			if(temp1->parent->left == *rootptr)
				temp1->parent->left = temp1;
			else
				temp1->parent->right = temp1;
		}

		// point the subtree root pointer to temp1 which is now the root of the subtree
		*rootptr = temp1;

		updateheight(temp1->right);
		updateheight(temp1);

	}

}

void insert_fixup(avl_node **rootptr, avl_node *temp){
	// declare int variables
	int current = BALANCED, previous, lh, rh;

	// The fixup will start from the node that is being inserted, all the way up to the root
	do{
		// get the height of the left and right child of the node
		lh= (temp->left==NULL?-1:temp->left->height);
		rh= (temp->right==NULL?-1:temp->right->height);

		previous=current;

		// get the balance status (balanced, right leaning, left leaning) of the node
		current = (lh==rh?BALANCED:(lh>rh?LEFT_LEANING:RIGHT_LEANING));

		// if there is a height imbalance:
		if(abs(lh-rh)>1){
			// note: temp points to the critical node, or to the node with the height imbalance 
			
			// notes:
			// binago order nung rotates sa left-right at right-left leaning kasi parang baliktad sila
			// pinagpalit ko rin statements nung inner if at else statements para ma-cover yung case na balanced yung previous

			if(current==LEFT_LEANING){
				if(previous== RIGHT_LEANING) {
					// left-right leaning fixup

					// bakit parang mali tong original algo
					// baliktad sila
					// right_rotate(&temp);			
					// left_rotate(&(temp->left));

					// left rotate at pivot ata muna
					left_rotate(&(temp->left));
					// then right rotate sa critical node
					right_rotate(&temp);
				}
				else{
					// left-left leaning or left leaning fixup
					right_rotate(&temp);		
				}
			}
			else{
				// current == RIGHT_LEANING
				if(previous==RIGHT_LEANING) {
					// right-left leaning fixup

					// baliktad din tong original algo
					// left_rotate(&temp);
					// right_rotate(&(temp->right));

					// right rotate at pivot muna
					right_rotate(&(temp->right));
					// then left rotate at critical node
					left_rotate(&temp);
				}
				else{
					// right-right leaning or right leaning fixup
					left_rotate(&temp);
				}
			}
		} //end if

		updateheight(temp);

		// make temp be the root node if it does not have a parent node
		if(temp->parent==NULL)
			*rootptr = temp;

		// make the temp node pointer point to the parent of the node
		temp =temp->parent;

	}while(temp!=NULL);
}

// this function is just the BST insert
void insert_node(avl_node **rootptr, avl_node *temp){
	// *rootptr accesses the value stored at the address rootptr
	// rootptr contains the memory address of the pointer variable that points to an avl_node
	// *rootpr contains a pointer to an avl_node

	// if the root pointer points to NULL (it does not point to a node)
	// then make the node to be inserted the root of the AVL tree
	if(*rootptr==NULL) *rootptr = temp;
	else{
		// else if root node exists,
		// make the parent of temp node be equal to the root node
		temp->parent = *rootptr;
		// if the value of temp is greater than the root node, then insert temp node to the right subtree
		// else insert it to the left
		if((*rootptr)->x < temp->x)
			insert_node(&((*rootptr)->right),temp);
		else
			insert_node(&((*rootptr)->left),temp);
	}
}

void insert_value(avl_node **rootptr, int x){
	// dynamically create a new node
	avl_node *temp;
	temp = (avl_node *)malloc(sizeof(avl_node));
	// set the values of the fields of that new node
	temp->x = x;
	temp->height = 0;
	temp->parent = temp ->left = temp->right = NULL;
	// insert the node to the tree then do fixup
	insert_node(rootptr, temp);
	insert_fixup(rootptr, temp);
}

void view(avl_node *root, int tabs){
	int i;
	if(root != NULL){
		view(root->right, tabs +1);
		for(i=0; i<tabs; i++) printf("\t");
		printf("%2i\n", root->x);
		view(root->left, tabs +1);
	}
}

void swap(int *a, int *b){
	int temp;
	temp = *a; *a= *b; *b =temp;
}

// -----------------------------------------------------------------------------------------------------------------------------------------
// Answer:

// maximum and minimum functions are needed for finding the successor of a node

// this function finds the node with the largest key in the subtree
avl_node* maximum(avl_node* node) {
    if (node == NULL) return NULL;

    // Traverse the tree by making the temp pointer, that initially points at node n,
    // to keep pointing to the right child of the node that it is currently pointing to
    // until the right child value of a node it is pointing to is equal to NULL
    avl_node* temp = node;

    while (temp->right != NULL) {
        // If the node that temp is currently pointing has a left child
        // then make temp point to that left child
        temp = temp->right;
    }

    // returning the address of the node with the minimum value in the given
    // subtree rooted at node n
    return temp;
}


// this function finds the node with the smallest key in the subtree
avl_node* minimum(avl_node* node)
{
    if (node == NULL) return NULL;

    // Traverse the tree by making the temp pointer, that initially points at node n,
    // to keep pointing to the left child of the node that it is currently pointing to
    // until the left child value of a node it is pointing to is equal to NULL
    avl_node* temp = node;

    while (temp->left != NULL)
    {
        // If the node that temp is currently pointing has a left child
        // then make temp point to that left child
        temp = temp->left;
    }

    if (temp->right != NULL)
      return minimum(temp->right);

    // returning the address of the node with the minimum value in the given
    // subtree rooted at node n
    return temp;
}


// Finding the successor of a node is required for the case #3 of BST delete
avl_node* successor(avl_node* node, avl_node* rootptr) {
    if (node == NULL) return NULL;

    // Case 1: "node" is the node with the largest key in the whole tree
	// If this is the case then "node" does not have a successor
	if (node == maximum(rootptr)) return NULL;
	// though in the case of the AVL delete, this will not happen 
	// as in the Case #3 of AVL delete, the node to be deleted will always have two children
	//  so it will have a right subtree that will contain the successor
    // case 1.5 Node has left sub tree
    if (node->left != NULL)
      return minimum(node->left);
    // Case 2: Node has a right subtree
    // In this case, the successor of "node" is the minimum value in its right subtree
    // Call the minimum() function and pass to it a subtree rooted at the right child of "node"
    // return the node of the right subtree of "node" with the minimum key
    if (node->right != NULL) return minimum(node->right);

    // Case 3: Node has no right subtree, and it is not the max node in the given BST
    // In this case, we need to find the lowest ancestor of "node" whose left child is also an ancestor of x
    // Keep going up sa ancestors/parents until you go right, or you came from a left child path
    avl_node* temp = node;

    // Keep making the temp pointer, that currently points to a node,
    // to point to that node's parent instead
    // until the current node being pointed by temp is the left child of its parent
    while (temp->parent->left != temp)
    {
        // make the temp pointer point to the node above it, or point to its parent
        temp = temp->parent;
    }
    // Return the parent node of the current node being pointed to by temp
    // that is the successor of "node"
    return temp->parent;
}


// This is just BST search
avl_node* search(avl_node* rootptr, int key) {
	if (rootptr == NULL) return NULL;

	// traverse the tree with the help of a temp node pointer
	avl_node* temp = rootptr;

	while (temp != NULL) {
		// node containing the key is found
		// return the value of temp node pointer
		if (key == temp->x) return temp;
		// make temp point to the left child of the current node pointed to by temp	
		else if (key < temp->x) temp = temp->left;
		// make temp point to the right child of the current node pointed to by temp
		else temp = temp->right;
	}

	// return NULL if the key being searched for does not exist in the tree
	return NULL;
}


void delete(avl_node **rootptr, avl_node* ptrToDel, int x) {
	// ptrToDel input parameter will be used in Case #3

	// AVL Deletion:
	// 1. Do BST delete
	// 2. Perform AVL fixup on the parent of the replacement (will be either predecessor or successor) up to the root

	// check if rootptr input is not valid
	if (rootptr == NULL) return;

	// use the BST search function to search for the node in the tree that contains the value equal to x
	if (ptrToDel == NULL) ptrToDel = search(*rootptr, x);

	// if after the search(), the key to be deleted does not exist in the tree
	// then end this function
	if (ptrToDel == NULL) return;

	// else, delete the node containing the key

	// Case 1: The node to be deleted has no child
	if (ptrToDel->left == NULL && ptrToDel->right == NULL) {
		deleteNodeWithNoChild(rootptr, ptrToDel, x);
		return;
	}
	
	// Case 2: The node to be deleted has one child
	if ((ptrToDel->left == NULL && ptrToDel->right != NULL) || (ptrToDel->left != NULL && ptrToDel->right == NULL)) {
		deleteNodeWithOneChild(rootptr, ptrToDel, x);
		return;
	}
	
	// Case 3: Node has two children
    // Find the successor of the node to be deleted:
	avl_node* ptrToSuccessor = successor(ptrToDel, *rootptr);

	// swap the keys of the node to be deleted and its successor:
    ptrToDel->x = ptrToSuccessor->x;
    ptrToSuccessor->x = x;

	  // The key to be deleted is currently stored in the node being pointed to by ptroToSuccessor
    // perform delete operation on that node recursively:
    delete(rootptr, ptrToSuccessor, x);
}


void deleteNodeWithNoChild(avl_node** rootptr, avl_node* node, int key) {
	// If the node to be deleted is the root node, then simply free it:
    if (node->parent == NULL) {
        free(node);
        return;
    }

	// make the parent of the node to be deleted point to null instead of pointing to the node to be deleted
	// identify whether the node to be deleted is a left or right child
	if (node->parent->left != NULL && key == node->parent->left->x) {
		node->parent->left = NULL;
	} else {
		node->parent->right = NULL;
	}

	// do fixup to maintain the height balance of the avl tree
	// this function will also update the height of the tree
	insert_fixup(rootptr, node->parent);
	free(node);
}


void deleteNodeWithOneChild(avl_node** rootptr, avl_node* node, int key) {
	// If the node to be deleted is a root node:
    if (node->parent == NULL) {
        // If the only child of the root node to be deleted is a left child then:
        if (node->left != NULL) {
			// make that left child be the root node
			node->left->parent = NULL;
			*rootptr = node->left;
		}
        // else if the only child of the root node to be deleted is the right child then:
        else {
			// make that rigth child be the root node
			node->right->parent = NULL;
			*rootptr = node->right;
		}

        free(node);
        return;
    }

	// Else, if the node to be deleted is not the root node of the whole tree:

	// If the only child of the node to be deleted is a left child then:
    if (node->left != NULL) {
        // If node to be deleted is a left child of its parent
        if (node->parent->left != NULL && key == node->parent->left->x) {
            // Set left child of parent to be the left child of the node to be deleted
            node->parent->left = node->left;
        }
        else {
            // Else, if the node to be deleted is not a left child of its parent
            // then it has to be its right child
            // Set right child of parent to be the left child of the node to be deleted
            node->parent->right = node->left;
        }
        // Make the parent of the node to be deleted, the parent of the child of the node to be deleted
        node->left->parent = node->parent;
    }
    // Else, if the only child of the node to be deleted is a right child:
    else {
        // If node to be deleted is a left child of its parent
        if (node->parent->left != NULL && key == node->parent->left->x) {
            // Set left child of parent to be the right child of the node to be deleted
            node->parent->left = node->right;
        }
        else {
            // Else, if the node to be deleted is not a left child of its parent
            // then it has to be its right child
            // Set right child of parent to be the right child of the node to be deleted
            node->parent->right = node->right;
        }
        // Make the parent of the node to be deleted, the parent of the child of the node to be deleted
        node->right->parent = node->parent;
    }

	// do fixup to maintain the height balance of the avl tree
	// this function will also update the height of the tree
	insert_fixup(rootptr, node->parent);
    free(node);
}


void freeTree(avl_node *rootptr) {
	// Base case:
    if (rootptr == NULL) return;

    // Postorder tree traversal algo will be sort of used

    // Calling the recursive function on the left child of the current node
    freeTree(rootptr->left);
    // calling the recursive function on the right child of the current node
    freeTree(rootptr->right);

    // After the nodes below the current node have been deleted or cleared
    // only then will we free or delete the current node
    free(rootptr);
}


int main(){
	avl_node *root = NULL;
	int i,n=10;

	// original node insert code
	// for(i=0; i<n; i++){
	// 	insert_value(&root,i+1);
	// 	view(root,0);
	// 	printf("\n----------------------------------------\n");
	// }
	// insert_value(&root,0);	
	// view(root,0);
	// 	printf("\n----------------------------------------\n");

	// delete(&root, NULL, 0);
	// view(root, 0);
	// printf("\n----------------------------------------\n");
	// freeTree(root);
		

	// ----------------------------------------------------------------------------------------------------------------
	// left-right leaning test:
	// test muna kasi parang mali algo sa left-right at right-left leaning nung template code, baliktad yung statements
	// insert_value(&root, 5);
	// view(root, 0);
	// printf("\n----------------------------------------\n");
	// insert_value(&root, 1);
	// view(root, 0);
	// printf("\n----------------------------------------\n");
	// insert_value(&root, 3);
	// view(root, 0);
	// printf("\n----------------------------------------\n");

	// // sample delete tests for those 3 nodes
	// delete(&root, NULL, 3);
	// view(root, 0);
	// printf("\n----------------------------------------\n");

	// delete(&root, NULL, 5);
	// view(root, 0);
	// printf("\n----------------------------------------\n");

	// freeTree(root);


	// ----------------------------------------------------------------------------------------------------------------
	// AVL delete that requires fixup
	insert_value(&root, 3);
	view(root, 0);
	printf("\n----------------------------------------\n");
	insert_value(&root, 2);
	view(root, 0);
	printf("\n----------------------------------------\n");
	insert_value(&root, 5);
	view(root, 0);
	printf("\n----------------------------------------\n");
	insert_value(&root, 1);
	view(root, 0);
	printf("\n----------------------------------------\n");
	insert_value(&root, 4);
	view(root, 0);
	printf("\n----------------------------------------\n");
	insert_value(&root, 6);
	view(root, 0);
	printf("\n----------------------------------------\n");
	insert_value(&root, 7);
	view(root, 0);
	printf("\n----------------------------------------\n");

	delete(&root, NULL, 1);
	view(root, 0);
	printf("\n----------------------------------------\n");

	delete(&root, NULL, 2);
	view(root, 0);
	printf("\n----------------------------------------\n");

	delete(&root, NULL, 5);
	view(root, 0);
	printf("\n----------------------------------------\n");

	delete(&root, NULL, 4);
	view(root, 0);
	printf("\n----------------------------------------\n");
	freeTree(root);
}