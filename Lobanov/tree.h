#define SUCCESS 1
#define ELEMENT_NOT_FOUND 2
#define TOO_MANY_CHILDREN 3
#define WRONG_DATA 4
#define NO_MEMORY 5
typedef struct node {
	int data;
	node *left;
	node *right;
};
typedef struct stat {
	int leaf;
	int ALL_node;
};
//выделение памяти на элемент
node* Tcreate(int data) {
	node *NewNode;
	NewNode = (node*)malloc(sizeof(node));
	if (NewNode != NULL) {
		NewNode->data = data;
		NewNode->left = NULL;
		NewNode->right = NULL;
		return(NewNode);
	}
	else
		return(NULL);
}
//добавление узла
int add_node(int data, node *parent) {
	node *NewNode;
	
	if (parent == NULL)
		return WRONG_DATA ;
	
	if (parent->left == NULL) {
		NewNode = Tcreate(data);
		if (!NewNode)
			return NO_MEMORY;
		parent->left = NewNode;
		return SUCCESS;
	}
	
	if (parent->right == NULL) {
		NewNode = Tcreate(data);
		if (!NewNode)
			return NO_MEMORY;
		parent->right = NewNode;
		return SUCCESS;
	}
	
	return TOO_MANY_CHILDREN;

}
//поиск элемента
node* tree_search(node* root, int Key) {
	node  *result;
	if (root == NULL)
		return NULL;
	if (Key == root->data)
		return root;
	if (result = tree_search(root->left, Key))
		return result;
	result = tree_search(root->right, Key);
	return result;
}
//поиск родителя элемента
node* search_parrent(node* root, int key) {
	node *result;
	if (root->left == NULL)
		return NULL;
	if (root->left->data == key) {
		return(root);
	}
	if (root->right == NULL)
		return NULL;
	if (root->right->data == key)
		return(root);
	if (result = search_parrent(root->left, key))
		return result;
	result = search_parrent(root->right, key);
	return result;
}
//уничтожение дерева
void destroy_tree(node *root) {
	if (root == NULL)
		return;
	destroy_tree(root->left);
	destroy_tree(root->right);
	free(root);
}
//уничтожение под дерева
void delete_elem(node *root, int key) {
	node *result;
	result=search_parrent(root, key);
	if (result->left)
		if (result->left->data==key) {
			result->left = NULL;
			destroy_tree(result->left);
		}
	if (result->right)
		if (result->right->data==key) {
			result->right = NULL;
			destroy_tree(result->right);
		}
	return;
}
//добавление ребёнка
int add_son(int NewData,int key,node *root) {
	int result;
	node *parent;
	parent=tree_search(root,key);
	if (parent)
		result = add_node(NewData, parent);
	else
		return ELEMENT_NOT_FOUND;
	return result;
}
//добавление ровестника
int add_sibling(int NewData,int key,node *root) {
	node* parent;
	int result;
	parent = search_parrent(root, key);
	if (parent)
		result = add_node(NewData, parent);
	else
		return ELEMENT_NOT_FOUND;
	return result;

}
//вывод дерева на экран
void show(node *root,int offset) {
	if (root == NULL)
		return;
	if (root->right != NULL)
		show(root->right, offset + 3);
	for (int i = 0; i < offset; i++) 
		printf(" ");
	printf("%d\n", root->data);
	if (root->left != NULL) 
		show(root->left, offset + 3);
}
//функция для вывода команд в файл
void show_command(node *root,FILE *fp,int prev_data) {
	if (root == NULL)
		return;
	fprintf(fp, "add ");
	if (prev_data == 0)
		fprintf(fp, "root %d\n", root->data);
	else
		fprintf(fp, "node %d parent %d\n", root->data, prev_data);
	show_command(root->left, fp, root->data);
	show_command(root->right, fp, root->data);
}
//сохранение дерева в файл с именем
void save_tree(node *root) {
	FILE *fp;
	fp = fopen("tree.txt", "w");
	show_command(root, fp, 0);
	fclose(fp);
	return;
}
//загрузка дерева из файла с именем name(надо переделать)
node* load_tree(char *name){
	FILE *fp;
	char command[255];
	int data,key;
	node *root;
	fp = fopen(name, "r");
	while (fp==NULL) {
		printf("Eror.Can't open file.Try again\n");
		scanf("%s", name);
		fp = fopen(name, "r");
	}
	
	fscanf(fp, "%s", command);
	
	if (strcmp(command, "add")) {
		printf("Eror.Invalid sintax\n");
		return NULL;
	}

	fscanf(fp, "%s", command);
	if (strcmp(command,"root")) {
		printf("Eror.You must add root first\n");
		return NULL;
	}
	else {
		fscanf(fp, "%s", command);
		data = atoi(command);
		root=Tcreate(data);
	}
	while (fscanf(fp,"%s",command)!=EOF){
		if (strcmp(command, "add")) {
			printf("Eror.Invalid sintax\n");
			return NULL;
		}
		fscanf(fp,"%s", command);
		if (strcmp(command, "node")) {
			printf("Eror.Invalid sintax\n");
			return NULL;
		}
		fscanf(fp,"%s", command);
		data = atoi(command);
		fscanf(fp, "%s", command);
		if (!strcmp(command, "parent")) { //нужна проверка на root
			fscanf(fp, "%s", command);
			if (!strcmp(command, "root"))
				add_node(data, root);
			else {
				key = atoi(command);
				add_son(data, key, root);
			}
		}
		if (!strcmp(command, "sibling")) { 
			fscanf(fp, "%s", command);
			key = atoi(command);
			add_sibling(data, key, root);
		}
	
	}
	return root;
}
//статистика
void show_stat(node *root,stat *ts) {
	if (root == NULL)
		return;
	ts->ALL_node++;
	if (root->left == NULL && root->right == NULL) {
		ts->leaf++;
		return;
	}
		
	if (root->left == NULL)
		return;
	else
		show_stat(root->left,ts);
	if (root->right == NULL)
		return;
	else
		show_stat(root->right,ts);
}

//обработчик ошибок(не используется, но пусть будет))
void error_processing(int result) {
	switch (result) {
	case SUCCESS:
		return;
		break;
	case ELEMENT_NOT_FOUND:
		printf("Eror 1.Element not found\n");
		return;
		break;
	case TOO_MANY_CHILDREN:
		printf("Eror 2.Too mane children\n");
		return;
		break;
	case WRONG_DATA:
		printf("Eror 3.Wrong data\n");
		return;
		break;
	case NO_MEMORY:
		printf("Eror 4.No memory\n");
		return;
		break;
	}
}
