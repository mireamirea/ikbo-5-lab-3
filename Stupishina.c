#include <stdio.h>
#include <stdlib.h>


typedef struct Tree {
	int value;
	struct Tree *Left;
	struct Tree *Right;
} Tree;


void CreateTree();
void Insert(int value, Tree** tree);
void Write(Tree **p, FILE* output);
Tree* Read(Tree **tree, FILE* fp);
int CountNodes(Tree *node);
int CountLeaves(Tree* tree);
int isBST(Tree* tree);
void Destroy(Tree* Tree);
Tree *Search(Tree** tree, int value);
Tree *SearchLeaves(Tree** tree, int value);
void *SearchParents(Tree** tree, int value);
int Print(Tree *tree, int is_Left, int offset, int depth, char s[20][255]);
void print_t(Tree *tree);

Tree* tree = NULL;
Tree* par = NULL;
Tree* tmp = NULL;



void Insert(int value, Tree** tree) {
	struct Tree *newTree = NULL;
	if (!(*tree)) {
		newTree = (Tree*)malloc(sizeof(Tree));
		newTree->Left = NULL;
		newTree->Right = NULL;
		newTree->value = value;
		*tree = newTree;
		return;
	}
	else printf("\n Root already exists\n");
}

void Write(Tree **p, FILE* output) {
	if (!(*p)) {
		fprintf(output, "# ");
	}
	else {
		fprintf(output, "%d ", (*p)->value);
		Write(&((*p)->Left), output);
		Write(&((*p)->Right), output);
	}
}

Tree* Read(Tree **tree, FILE* fp) {
	char* a;
	int in;
	a = (char *)malloc(sizeof(char) * 10);
	if (fscanf(fp, "%s", a))
		if (a[0] != '#'){
			in = atoi(a);
			Insert(in, tree);
			printf("%d ", (*tree)->value);
			Read(&((*tree)->Left), fp);
			Read(&((*tree)->Right), fp);
		}
	return *tree;
}

int CountNodes(Tree *node){
	int number = 1;
	if (node == NULL)
		return 0;
	else{
		number += CountNodes(node->Left);
		number += CountNodes(node->Right);
		return number;
	}
}

int CountLeaves(Tree* tree){
	if (tree == NULL) {
		return 0;
	}
	if (tree->Left == NULL && tree->Right == NULL) {
		return 1;
	}
	return CountLeaves(tree->Left) + CountLeaves(tree->Right);
}

int isBST(Tree* tree) {
	static struct Tree *prev = NULL;
	if (tree){
		if (!isBST(tree->Left))
			return 0;
		if (prev != NULL && tree->value <= prev->value)
			return 0;
		prev = tree;
		return isBST(tree->Right);
	}
	return 1;
}

void Destroy(Tree* tree) {
	if (tree != 0) {
		Destroy(tree->Left);
		Destroy(tree->Right);
		free(tree);
	}
}

Tree *Search(Tree** tree, int value) {
	if (!(*tree)) {
		return NULL;
	}
	if (value == (*tree)->value) {
		return *tree;
	}
	else
		Search(&((*tree)->Left), value);
	Search(&((*tree)->Right), value);
}

Tree *SearchLeaves(Tree** tree, int value)
{
	if (!(*tree)) {
		return NULL;
	}
	else{
		if (value == (*tree)->value){
			if ((*tree)->Left == NULL){
				par = tmp;
				return *tree;
			}
			if ((*tree)->Right == NULL){
				par = tmp;
				return *tree;
			}
			else{
				tmp = *tree;
				SearchLeaves(&((*tree)->Left), value);
				SearchLeaves(&((*tree)->Right), value);
			}
		}
		else{
			tmp = *tree;
			SearchLeaves(&((*tree)->Left), value);
			SearchLeaves(&((*tree)->Right), value);
		}
	}
}

void *SearchParents(Tree** tree, int value) {
if (!(*tree))
    return NULL;
else {
    if (value == (*tree)->value){
        if (tmp->Left == NULL)
        if (tmp->Right == *tree){
            par = tmp;
            return *tree;
        }
        if ((tmp->Right == NULL) && (tmp->Left == *tree)){
            par = tmp;
            return *tree;
        }

        else {
            tmp = *tree;
            SearchParents(&((*tree)->Left), value);
            SearchParents(&((*tree)->Right), value);
            }
        }
    else {
        tmp = *tree;
        SearchParents(&((*tree)->Left), value);
        SearchParents(&((*tree)->Right), value);
        }
    }
}


int Print(Tree *tree, int left, int offset, int depth, char s[20][255]){
int i = 0;
char b[20];
int width = 5;
if (!tree) return 0;
sprintf(b, "(%03d)", tree->value);
int Left = Print(tree->Left, 1, offset, depth + 1, s);
int Right = Print(tree->Right, 0, offset + Left + width, depth + 1, s);
for (i = 0; i < width; i++)
    s[depth][offset + Left + i] = b[i];
if (depth && left) {
    for (i = 0; i < width + Right; i++)
        s[depth-1][offset + Left + width / 2 + i] = '-';
    s[depth-1][offset+Left+width/2] = '.';
}
else if (depth && !left) {
    for (i=0; i<Left+width; i++)
        s[depth-1][offset-width/2+i]='-';
    s[depth-1][offset+Left+width/2]='.';
}

return Left + width + Right;
}

void print(Tree *tree){
	int i;
	char s[20][255];
	for (i = 0; i < 20; i++)
		sprintf(s[i], "%80s", " ");
	Print(tree, 0, 0, 0, s);
	for (i = 0; i < 20; i++)
		printf("%s\n", s[i]);
}

int main() {
	int opt, N, M;
	char fname[255];
	Tree *newT = NULL;
	FILE* out;
	while (opt!= 11){
		printf("Options:\n1.Add root\n2.Add parent\n3.Add sibling\n4.Print Tree\n5.Read from file\n6.Save to file\n7.Print tree\n8.Statistics\n9.Search element\n10.Destroy tree\n11.Exit\n");
		scanf("%d", &opt);
		switch (opt){
			case 1: {
				printf("Root value:\n");
				scanf("%d", &N);
				Insert(N, &tree);
				break;
			}
			case 2:
				printf("Tree Value:\n");
				scanf("%d", &N);
				printf("Parent Value:\n");
				scanf("%d", &M);
				par=SearchLeaves(&tree, M);
				if (par!=NULL) {
					if (par->Left == NULL)
						Insert(N, &(tmp->Left));
					else if (par->Right == NULL)
						Insert(N, &(par->Right));
					else
						printf("\n%d Failed to create a new tree.\n", par->value);
				}
				else
					printf("Value not found.\n");
				break;
			case 3:
				printf("Tree Value:\n");
				scanf("%d", &N);
				printf("Sibling Value:\n");
				scanf("%d", &M);
				tmp = NULL;
				par = NULL;
				tmp = tree;
				SearchParents(&tree, M);
				if (par != NULL) {
					if (!par->Left)
						Insert(N, &(par->Left));
					if (!par->Right)
						Insert(N, &(par->Right));
					else
						printf("\n%d Error\n", par->value);
				}
				else
					printf("Error\n");
				break;
			case 4: {
				print(tree);
				break;
			default:
				printf("Error\n");
				break;
			}

			case 5: {
				FILE* File;
				printf ("Enter file name\n");
				scanf("%s", fname);
				File=fopen(fname, "rb");
				if (File== NULL)
				while (File== NULL) {
					printf("File not found.\n");
					scanf("%s", fname);
					File=fopen(fname, "rb");
				}


				Read(&tree, File);
				printf("\n");
				fclose(File);
				break;
			}
			case 6: {
				printf ("Enter file name\n");
				out=fopen(fname, "rb");
				scanf("%s", fname);
				Write(&tree, tree);
				printf("Saved\n");
				fclose(tree);
				break;
			}
			case 7: {
			}
				print(tree);
				break;
			case 8: {
				printf("Number of nodes:%d\n", CountNodes(tree));
				printf("Number of leaves:%d\n", CountLeaves(tree));
				if (isBST(tree))
					printf("This tree is BST\n");
				else
					printf("This tree is not BST\n");
				break;
			}
			case 9: {
				int s;
				printf("\nSearching value is:\n");
				scanf("%d", &s);
				newT = Search(&tree, s);
				if (newT != NULL)
					printf("Value %d is found\n", s);
				else
					printf("Value %d is not found\n", s);
				break;
			}
			case 10: {
				Destroy(tree);
				break;
			}
		}
	}
	return 0;
}
