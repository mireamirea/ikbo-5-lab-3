
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "tree.h"


int main(void){
	int key,data;
	char command[255], name[255];
	node *root=NULL,*tresult=NULL;
	stat ts, s0={ 0,0};
	while (1) {
		scanf("%s", command);
		if (!strcmp(command, "show")) {
			printf("Your Tree:\n");
			show(root, 0);
			printf("==========\n");
		}
		if (!strcmp(command, "save")) {
			save_tree(root);
			printf("Tree saved.\n");
		}
		if (!strcmp(command, "load")) {
			printf("Enter file name:\n");
			scanf("%s", name);
			root = load_tree(name);
			printf("File loaded.\n");
		}
		if (!strcmp(command, "exit")) {
			break;
		}
		if (!strcmp(command, "search")) {
			printf("Enter searching element:\n");
			scanf("%d", &key);
			tresult = tree_search(root, key);
			if (tresult)
				printf("Element found\n");
			else
				printf("Element don't found\n");
		}
		if (!strcmp(command, "delete")) {
			scanf("%d", &key);
			delete_elem(root, key);

		}
		if (!strcmp(command, "destroy")) {
			destroy_tree(root);
			printf("I destroy tree");
			root = NULL;
		}
		if (!strcmp(command, "add")) {
			scanf("%s", command);
			if (!strcmp(command, "root")) {
				scanf("%d", &data);
				root = Tcreate(data);
			}
			if (!strcmp(command, "node")) {
				scanf("%d", &data);
				scanf("%s", command);
				if (!strcmp(command, "parent")) {
					scanf("%s", command);
					if (!strcmp(command, "root"))
						add_node(data, root);
					else {
						key = atoi(command);
						add_son(data, key, root);
					}
				}
				if (!strcmp(command, "sibling")) {
					scanf("%d", &key);
					add_sibling(data, key, root);
				}
			}
		}
		if (!strcmp(command,"show_stat")) {
			ts = s0;
			show_stat(root,&ts);
			printf("Number of all nodes:%d\nNumber of internal nodes:%d\n", ts.ALL_node, ts.ALL_node - ts.leaf);
			printf("Number of leaf:%d\n", ts.leaf);
		}
		}
	destroy_tree(root);
	return 1;
	}
