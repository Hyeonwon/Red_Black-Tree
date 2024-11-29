#include <stdio.h>
#include <stdlib.h>

typedef enum { RED, BLACK } nodeColor;

typedef struct Node {
    int key;
    nodeColor color;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
} Node;

typedef struct RBTree {
    Node* root;
    Node* nil;
} RBTree;

// 새로운 노드를 생성하는 함수
Node* createNode(RBTree* tree, int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->color = RED;
    node->left = tree->nil;
    node->right = tree->nil;
    node->parent = tree->nil;
    return node;
}

// 빈 레드-블랙 트리를 생성하고 초기화하는 함수
RBTree* createRBTree() {
    RBTree* tree = (RBTree*)malloc(sizeof(RBTree));
    tree->nil = (Node*)malloc(sizeof(Node));
    tree->nil->color = BLACK;
    tree->root = tree->nil;
    return tree;
}

// 서브트리를 왼쪽으로 회전하는 함수
// 시간 복잡도: O(1)
void leftRotate(RBTree* tree, Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left != tree->nil) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == tree->nil) tree->root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

// 서브트리를 오른쪽으로 회전하는 함수
// 시간 복잡도: O(1)
void rightRotate(RBTree* tree, Node* y) {
    Node* x = y->left;
    y->left = x->right;
    if (x->right != tree->nil) x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == tree->nil) tree->root = x;
    else if (y == y->parent->right) y->parent->right = x;
    else y->parent->left = x;
    x->right = y;
    y->parent = x;
}

// 삽입 후 레드-블랙 트리를 수정하는 함수
// 시간 복잡도: O(1)
void insertFixup(RBTree* tree, Node* z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(tree, z->parent->parent);
            }
        } else {
            Node* y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

// 레드-블랙 트리에 노드를 삽입하는 함수
// 시간 복잡도: O(log n)
void insert(RBTree* tree, int key) {
    Node* z = createNode(tree, key);
    Node* y = tree->nil;
    Node* x = tree->root;

    while (x != tree->nil) {
        y = x;
        if (z->key < x->key) x = x->left;
        else x = x->right;
    }

    z->parent = y;
    if (y == tree->nil) tree->root = z;
    else if (z->key < y->key) y->left = z;
    else y->right = z;

    z->left = tree->nil;
    z->right = tree->nil;
    z->color = RED;

    insertFixup(tree, z);
}

// 트리의 한 노드를 다른 노드로 교체하는 함수
// 시간 복잡도: O(1)
void transplant(RBTree* tree, Node* u, Node* v) {
    if (u->parent == tree->nil) tree->root = v;
    else if (u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;
    v->parent = u->parent;
}

// 서브트리에서 최소값을 찾는 함수
// 시간 복잡도: O(log n)
Node* minimum(RBTree* tree, Node* x) {
    while (x->left != tree->nil) x = x->left;
    return x;
}

// 삭제 후 레드-블랙 트리를 수정하는 함수
// 시간 복잡도: O(1)
void deleteFixup(RBTree* tree, Node* x) {
    while (x != tree->root && x->color == BLACK) {
        if (x == x->parent->left) {
            Node* w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(tree, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(tree, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(tree, x->parent);
                x = tree->root;
            }
        } else {
            Node* w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(tree, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(tree, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(tree, x->parent);
                x = tree->root;
            }
        }
    }
    x->color = BLACK;
}

// 레드-블랙 트리에서 노드를 삭제하는 함수
// 시간 복잡도: O(log n)
void deleteNode(RBTree* tree, Node* z) {
    Node* y = z;
    Node* x;
    nodeColor yOriginalColor = y->color;

    if (z->left == tree->nil) {
        x = z->right;
        transplant(tree, z, z->right);
    } else if (z->right == tree->nil) {
        x = z->left;
        transplant(tree, z, z->left);
    } else {
        y = minimum(tree, z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z) x->parent = y;
        else {
            transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (yOriginalColor == BLACK) deleteFixup(tree, x);
}

// 레드-블랙 트리에서 주어진 키의 노드를 삭제하는 함수
// 시간 복잡도: O(log n)
void deleteKey(RBTree* tree, int key) {
    Node* z = tree->root;
    while (z != tree->nil && z->key != key) {
        if (key < z->key) z = z->left;
        else z = z->right;
    }
    if (z != tree->nil) deleteNode(tree, z);
}

// 중위 순회로 노드를 출력하는 재귀 함수
void inorderTraversal(Node* node, Node* nil, FILE* outputFile) {
    if (node != nil) {
        inorderTraversal(node->left, nil, outputFile);
        fprintf(outputFile, "%d ", node->key);
        inorderTraversal(node->right, nil, outputFile);
    }
}

// 레드-블랙 트리의 중위 순회 결과를 출력하는 함수
// 시간 복잡도: O(n)
void printInorder(RBTree* tree, FILE* outputFile) {
    inorderTraversal(tree->root, tree->nil, outputFile);
    fprintf(outputFile, "\n");
}

// 레벨 순회로 노드를 출력하는 함수
// 시간 복잡도: O(n)
void levelOrderTraversal(RBTree* tree, FILE* outputFile) {
    if (tree->root == tree->nil) return;
    Node* queue[100]; // 필요한 경우 크기 조정
    int front = 0, rear = 0;
    queue[rear++] = tree->root;

    while (front < rear) {
        Node* node = queue[front++];
        fprintf(outputFile, "%d ", node->key);
        if (node->left != tree->nil) queue[rear++] = node->left;
        if (node->right != tree->nil) queue[rear++] = node->right;
    }
    fprintf(outputFile, "\n");
}

// 레드-블랙 트리의 레벨 순회 결과를 출력하는 함수
// 시간 복잡도: O(n)
void printLevelorder(RBTree* tree, FILE* outputFile) {
    levelOrderTraversal(tree, outputFile);
}

int main() {
    RBTree* tree = createRBTree();

    FILE* inputFile = fopen("input.txt", "r");
    FILE* outputFile = fopen("output.txt", "w");

    if (inputFile == NULL || outputFile == NULL) {
        printf("파일을 여는 중 오류가 발생했습니다.\n");
        return 1;
    }

    // 노드 삽입
    int key;
    while (fscanf(inputFile, "%d", &key) == 1) {
        insert(tree, key);
        if (fgetc(inputFile) == '\n') break;
    }

    // 삽입 후 순회 결과 출력
    printInorder(tree, outputFile);
    printLevelorder(tree, outputFile);

    // 노드 삭제
    while (fscanf(inputFile, "%d", &key) == 1) {
        deleteKey(tree, key);
    }

    // 삭제 후 순회 결과 출력
    printInorder(tree, outputFile);
    printLevelorder(tree, outputFile);

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
