#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct StructTree{
int Key;
struct StructTree *Left;
struct StructTree *Right;
struct StructTree *Back;
} TypeTree;

int StringCmp(int *TempInt1,int *TempInt2,char filename[]);
void FreeTree(TypeTree *Tree);
TypeTree *AddTree(TypeTree *Tree,int Elem);
TypeTree* TreeSearch(TypeTree* Tree,int SearchNum,int TypeSearch,int flag);
void PrintTree(TypeTree *Tree,int level);
void SaveFile(TypeTree *Tree,FILE **openFile);
void LoadFile(TypeTree **Tree,FILE **openFile);
void Analysis(TypeTree *Tree,int flag);
int CommandCheck(int TempNum1,int TempNum2,int TempNum3,int TempNum4,int *TempInt1,int *TempInt2);
void BackPtr(TypeTree *Tree,TypeTree *ptr);

int StringCmp(int *TempInt1,int *TempInt2,char filename[]){
    char Command[255];
    scanf("%s",Command);
    if (strcmp(Command,"help")==0)
        return 16;
    if (strcmp(Command,"show")==0)
        return 13;
    if (strcmp(Command,"exit")==0)
        return 18;
    if (strcmp(Command,"save")==0){
        scanf("%s",filename);
        return 14;
    }
    if (strcmp(Command,"load")==0){
        scanf("%s",filename);
        return 15;
    }
    if (strcmp(Command,"analysis")==0)
        return 17;
    if (strcmp(Command,"add")==0)
        return CommandCheck(1,2,3,4,TempInt1,TempInt2);
    if (strcmp(Command,"renew")==0)
        return CommandCheck(5,6,7,8,TempInt1,TempInt2);
    if (strcmp(Command,"del")==0){
       scanf("%s",Command);
        if (strcmp(Command,"tree")==0)
            return 9;
        if (strcmp(Command,"left")==0){
            scanf("%s",Command);
            if (strcmp(Command,"parent")==0){
                scanf("%d",TempInt2);
                return 10;
            }
        }
        if (strcmp(Command,"right")==0){
            scanf("%s",Command);
            if (strcmp(Command,"parent")==0){
                scanf("%d",TempInt2);
                return 11;
            }
        }
        if (strcmp(Command,"sibling")==0){
            scanf("%d",TempInt2);
            return 12;
        }
    }
    return -1;
}

int CommandCheck(int TempNum1,int TempNum2,int TempNum3,int TempNum4,int *TempInt1,int *TempInt2){
    char Command[255];
    scanf("%s",Command);
    if (strcmp(Command,"root")==0){
        scanf("%d",TempInt1);
        return TempNum1;
    }
    if (strcmp(Command,"node")==0){
        scanf("%d",TempInt1);
        scanf("%s",Command);
        if (strcmp(Command,"left")==0){
            scanf("%s",Command);
            if (strcmp(Command,"parent")==0){
                scanf("%d",TempInt2);
                return TempNum2;
            }
        }
        if (strcmp(Command,"right")==0){
            scanf("%s",Command);
            if (strcmp(Command,"parent")==0){
                scanf("%d",TempInt2);
                return TempNum3;
            }
        }
        if (strcmp(Command,"sibling")==0){
            scanf("%d",TempInt2);
            return TempNum4;
        }
    }
    return -1;
}


TypeTree *AddTree(TypeTree *Tree,int Elem){
	if (Tree==NULL){
		Tree=(TypeTree*)malloc(sizeof(TypeTree));
		Tree->Key=Elem;
		Tree->Left=NULL;
		Tree->Right=NULL;
	}
	return Tree;
}


void FreeTree(TypeTree *Tree){
	if (Tree!=NULL){
		FreeTree(Tree->Left);
		FreeTree(Tree->Right);
		free(Tree);
	}
}


TypeTree* TreeSearch(TypeTree* Tree,int SearchNum,int TypeSearch,int flag){
    static TypeTree *SearchTree;
    TypeTree *TempTree;
    if (flag==1)
        SearchTree=NULL;
    if (Tree!=NULL){
        if (Tree->Key==SearchNum){
            if ((TypeSearch==1)&&(Tree->Left==NULL))
                SearchTree=Tree;
            if ((TypeSearch==2)&&(Tree->Right==NULL))
                SearchTree=Tree;
            if ((TypeSearch==3)&&((Tree->Back!=NULL)))
                if ((Tree->Back->Left==NULL)||(Tree->Back->Right==NULL))
                    SearchTree=Tree;
            if ((TypeSearch==4)&&(Tree->Left!=NULL))
                SearchTree=Tree;
            if ((TypeSearch==5)&&(Tree->Right!=NULL))
                SearchTree=Tree;
            if ((TypeSearch==6)&&(Tree->Back!=NULL)){
                if ((Tree->Back->Left==Tree)&&(Tree->Back->Right!=NULL))
                    SearchTree=Tree->Back->Right;
                if ((Tree->Back->Right==Tree)&&(Tree->Back->Left!=NULL))
                    SearchTree=Tree->Back->Left;
            }
        }
        TempTree=TreeSearch(Tree->Left,SearchNum,TypeSearch,0);
        TempTree=TreeSearch(Tree->Right,SearchNum,TypeSearch,0);
    }
    if (flag==1)
        return SearchTree;
   return Tree;

}


void PrintTree(TypeTree *Tree,int level){
    int i;
    if(Tree!=NULL){
        PrintTree(Tree->Right,level+1);
        for (i=0;i<level;i++)
            printf("    ");
        printf("%d\n",Tree->Key);
        PrintTree(Tree->Left,level+1);
    }
}


void SaveFile(TypeTree *Tree,FILE **openFile){
    if (Tree!=NULL){
        fwrite(Tree,sizeof(TypeTree),1,*openFile);
        if (Tree->Left!=NULL)
            SaveFile(Tree->Left,openFile);
        if (Tree->Right!=NULL)
            SaveFile(Tree->Right,openFile);
    }
}


void LoadFile(TypeTree **Tree,FILE **openFile){
    if (!feof(*openFile)){
        (*Tree)=(TypeTree*)malloc(sizeof(TypeTree));
        fread(*Tree,sizeof(TypeTree),1,*openFile);
        if ((*Tree)->Left!=NULL)
            LoadFile(&((*Tree)->Left),openFile);
        if ((*Tree)->Right!=NULL)
            LoadFile(&((*Tree)->Right),openFile);
    }
}

void BackPtr(TypeTree *Tree,TypeTree *ptr){
    if (Tree!=NULL){
        Tree->Back=ptr;
        BackPtr(Tree->Left,Tree);
        BackPtr(Tree->Right,Tree);
    }
}


int main(void){
    TypeTree *Tree=0,*TempTree;
    FILE *openFile;
    int True=1,SwitchNum,TempNum1=0,TempNum2=0;
    char filename[255];
    while(True){
        system("CLS");
        printf("Enter command: ");
        printf("(If you want see a HELP(commands) enter show )\n");
        SwitchNum=StringCmp(&TempNum1,&TempNum2,filename);
        fflush(stdin);
        switch (SwitchNum){
        case 1:
            if (Tree==NULL){
                Tree=AddTree(Tree,TempNum1);
                Tree->Back=NULL;
            }
            else printf("Root was made\n");
        break;
        case 2:
            TempTree=TreeSearch(Tree,TempNum2,1,1);
            if (TempTree==NULL){
                printf("Parent is not found or parent is full\n");
                break;
            }
            TempTree->Left=AddTree(TempTree->Left,TempNum1);
            TempTree->Left->Back=TempTree;
        break;
        case 3:
            TempTree=TreeSearch(Tree,TempNum2,2,1);
            if (TempTree==NULL){
                printf("Parent is not found or parent is full\n");
                break;
            }
            TempTree->Right=AddTree(TempTree->Right,TempNum1);
            TempTree->Right->Back=TempTree;
        break;
        case 4:
            TempTree=TreeSearch(Tree,TempNum2,3,1);
            if (TempTree==NULL){
                printf("Free sibling is not found\n");
                break;
            }
            if (TempTree->Back->Left==NULL){
                TempTree->Back->Left=AddTree(TempTree->Back->Left,TempNum1);
                TempTree->Back->Left->Back=TempTree->Back;
            }
            if (TempTree->Back->Right==NULL){
                TempTree->Back->Right=AddTree(TempTree->Back->Right,TempNum1);
                TempTree->Back->Right->Back=TempTree->Back;
            }
        break;
        case 5:
            if (Tree==NULL){
                printf("Root is not made\n");
                break;
            }
            Tree->Key=TempNum1;
        break;
        case 6:
            TempTree=TreeSearch(Tree,TempNum2,4,1);
            if (TempTree==NULL){
                printf("Parent or child are not found\n");
                break;
            }
            TempTree->Left->Key=TempNum1;
        break;
        case 7:
            TempTree=TreeSearch(Tree,TempNum2,5,1);
            if (TempTree==NULL){
                printf("Parent or child is not found\n");
                break;
            }
            TempTree->Right->Key=TempNum1;
        break;
        case 8:
            TempTree=TreeSearch(Tree,TempNum2,6,1);
            if (TempTree==NULL){
                printf("Sibling is not found\n");
                break;
            }
            TempTree->Key=TempNum1;
        break;
        case 9:
            if (Tree==NULL){
                printf("Tree is empty\n");
                break;
            }
            FreeTree(Tree);
            Tree=NULL;
        break;
        case 10:
            PrintTree(Tree,0);
            break;
        case 11:
            openFile=fopen(filename,"w");
            SaveFile(Tree,&openFile);
            fclose(openFile);
            break;
        break;
        case 12:
            FreeTree(Tree);
            openFile=fopen(filename,"r");
            if (openFile!=NULL){
                LoadFile(&Tree,&openFile);
                fclose(openFile);
                BackPtr(Tree,NULL);
            }else printf("Incorrect filename\n");
        break;
        case 13:
            printf("X = number that you insert\n");
            printf("1=add root X\n");
            printf("2=add node X left parent X\n");
            printf("3=add node X right parent X\n");
            printf("4=add node X sibling X\n");
            printf("5=renew root\n");
            printf("6=renew node X left parent X\n");
            printf("7=renew node X right parent X\n");
            printf("8=renew node X sibling X\n");
            printf("9=delete a tree\n");
            printf("10=show\n");
            printf("11=save filename\n");
            printf("12=load filename\n");
            printf("13=help\n");
            printf("14=exit\n");
            system("pause");
        break;
        case 14:
            FreeTree(Tree);
            return 0;
        break;
        default:
            printf("Incorrect command\n");
        }
    }
}
