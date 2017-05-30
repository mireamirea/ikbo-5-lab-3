#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct treestr TNODE;

struct treestr
{
    int key;
    TNODE *ltree;
    TNODE *rtree;
};

TNODE *AddTree(int key);
void SearchTree(int searchkey, TNODE *root, TNODE **x);
void SearchSource(TNODE *root, TNODE *child, TNODE **x);
void AddBrunch(TNODE *parent, int key);
void Seetree(TNODE *tp, int Space);
void SaveTree(TNODE *root);
void LoadTree(TNODE **root);
void GetNode(TNODE **root, FILE **fp);
void SaveNode(TNODE *root, FILE **fp);
void DeleteTree(TNODE **root);
void Stats(TNODE *root, int *depth, int *leavesnum, int *nodesnum, int *innodesnum, int *isheap, int flag);
TNODE *AddTree(int key)
{
    TNODE *tnodep;
    tnodep = (TNODE*) malloc (sizeof(TNODE));
    if (tnodep)
    {
        tnodep->key = key;
        tnodep->rtree = tnodep->ltree = NULL;
    }
    return tnodep;
}
void Seetree(TNODE *tp, int space)
{
    int i;
    if (tp!=NULL)
    {
        Seetree(tp->rtree,space+1);
        for (i=0; i<space; i++) printf("\t");
        printf("%d\n",tp->key);
        Seetree(tp->ltree,space+1);
    }
}
void AddBrunch(TNODE *tnode, int key)
{
    if (tnode!=NULL)
    {
        if (tnode->ltree==NULL) tnode->ltree=AddTree(key);
        else if (tnode->rtree==NULL) tnode->rtree=AddTree(key);
        else printf("Error. No free children\n");
    }
    else printf("Error. Can not add to nonexistent node\n");
}
void SearchTree(int searchkey, TNODE *root, TNODE **x)
{
    if (root!=NULL)
    {
        if (root->key==searchkey) (*x)=root;
        else
        {
            if (root->ltree!=NULL) SearchTree(searchkey, root->ltree, x);
            if (root->rtree!=NULL) SearchTree(searchkey, root->rtree, x);
        }
    }
}
void SearchSource(TNODE *root, TNODE *child, TNODE **x)
{
    if ((child!=NULL)&&(root!=NULL))
    {
        if ((root->ltree==child)||(root->rtree==child)) (*x)=root;
        else
        {
            if  (root->ltree!=NULL) SearchSource(root->ltree, child, x);
            if  (root->rtree!=NULL) SearchSource(root->rtree, child, x);
        }
    }
    else printf("Error. Sib node does not exist\n");
}
void SaveTree(TNODE *root)
{
    char fname[255];
    FILE *fp;
    scanf(" %s",fname);
    fflush(stdin);
    fp=fopen(fname, "w");
    if (fp)
    {
        SaveNode(root, &fp);
        fclose(fp);
    }
    else printf("Error. Can not open file\n");
}
void LoadTree(TNODE **root)
{
    char fname[255];
    FILE *fp;
    scanf(" %s", fname);
    fflush(stdin);
    fp=fopen(fname, "r");
    if (fp)
    {
        GetNode(root, &fp);
        fclose(fp);
    }
    else printf("Error. Incorrect filename\n");
}
void GetNode(TNODE **root, FILE **fp)
{
    if (!feof(*fp))
    {
        (*root)= AddTree(0);
        fread((*root),sizeof(TNODE),1,*fp);
        if ((*root)->ltree!=NULL) GetNode(&((*root)->ltree), fp);
        if ((*root)->rtree!=NULL) GetNode(&((*root)->rtree), fp);
    }
}
void SaveNode(TNODE *root, FILE **fp)
{
    if (root!=NULL)
    {
        fwrite(root, sizeof(TNODE),1,*fp);
        if (root->ltree!=NULL) SaveNode(root->ltree,fp);
        if (root->rtree!=NULL) SaveNode(root->rtree,fp);
    }
}
void DeleteTree(TNODE **root)
{
    if (*root!=NULL)
    {
        if ((*root)->ltree!=NULL) DeleteTree(&((*root)->ltree));
        if ((*root)->rtree!=NULL) DeleteTree(&((*root)->rtree));
        if (((*root)->ltree==NULL) && ((*root)->rtree==NULL))
        {
            free((*root)->ltree);
            free((*root)->ltree);
            (*root)=NULL;
        }
    }
    else printf("Tree is already empty\n");
}
void Stats(TNODE *root, int *depth, int *leavesnum, int *nodesnum, int *innodesnum, int *isheap, int flag)
{
    static int curdepth;
    if (flag==0) curdepth=(*depth)=(*leavesnum)=(*nodesnum)=(*innodesnum)=(*isheap)=0;
    if (root!=NULL)
    {
        (*nodesnum)++;
        if ((root->ltree==NULL)&&(root->rtree==NULL)) (*leavesnum)++;
        else (*innodesnum)++;
        if (root->ltree!=NULL)
        {
            if (root->ltree->key>root->key) (*isheap)=1;
            curdepth++;
            Stats(root->ltree,depth,leavesnum,nodesnum,innodesnum,isheap,1);
        }
        if (root->rtree!=NULL)
        {
            if (root->rtree->key>root->key) (*isheap)=1;
            curdepth++;
            Stats(root->rtree,depth,leavesnum,nodesnum,innodesnum,isheap,1);
        }
    }
    if (curdepth>(*depth)) (*depth)=curdepth;
    curdepth--;
}

int main()
{
    TNODE *tnode = NULL;
    TNODE *xnode;
    TNODE *ynode;
    char word[255];
    int key,searchkey;
    int depth, leavesnum, nodesnum, innodesnum, isheap, length;
    printf("Command me: ");
    scanf("%s", word);
    while (strcmp(word,"exit")!=0)
    {
        xnode = NULL;
        ynode = NULL;
        if (strcmp(word,"add")==0)
        {
            scanf(" %s", &word);
            if (strcmp(word,"root")==0)
            {
                if (scanf(" %d", &key))
                    if (tnode==NULL)tnode=AddTree(key);
                    else printf("Root already exists\n");
                else {printf("Syntax error\n");fflush(stdin);}
            }
            else
            if (strcmp(word,"node")==0)
            {
                if (scanf(" %d", &key))
                {
                    scanf(" %s", word);
                    SearchTree(key, tnode, &xnode);
                    if (xnode!=NULL)
                    {
                        printf("Node already exists\n");
                    }
                    else
                    {
                        if (strcmp(word,"parent")==0)
                        {
                            scanf(" %s", word);
                            if (searchkey=atoi(word))
                            {
                                SearchTree(searchkey, tnode, &xnode);
                                AddBrunch(xnode,key);
                                xnode=NULL;
                            }
                            else
                            if (strcmp(word,"root")==0)
                            {
                                AddBrunch(tnode,key);
                            }
                            else {printf("Syntax error\n");fflush(stdin);}
                        }
                        else
                        if (strcmp(word,"sibling")==0)
                        {
                            if (scanf(" %d", &searchkey))
                            {
                                SearchTree(searchkey, tnode, &xnode);
                                SearchSource(tnode, xnode, &ynode);
                                AddBrunch(ynode, key);
                                xnode=ynode=NULL;
                            }
                            else {printf("Syntax error\n");fflush(stdin);}
                        }
                        else {printf("Syntax error\n");fflush(stdin);}
                    }
                }
                else {printf("Syntax error\n");fflush(stdin);}
            }
            else {printf("Syntax error\n");fflush(stdin);}
        }
        else
        if (strcmp(word,"show")==0)
        {
                if (tnode!= NULL)
                {
                    Stats(tnode, &depth, &leavesnum, &nodesnum, &innodesnum, &isheap,0);
                    Seetree(tnode,1);
                    putchar('\n');
                }
                else printf("Tree is empty\n");
        }
        else
        if (strcmp(word,"save")==0)
        {
            SaveTree(tnode);
        }
        else
        if (strcmp(word,"load")==0)
        {
            LoadTree(&tnode);
        }
        else
        if (strcmp(word,"delete")==0)
        {
            scanf(" %s", word);
            if (strcmp(word,"tree")==0)
                DeleteTree(&tnode);
            else
            if (strcmp(word,"node")==0)
            {
                scanf(" %s", word);
                key=atoi(word);
                if (key)
                {
                    SearchTree(key, tnode, &xnode);
                    if (xnode == NULL) printf("Error. No such node.");
                    else if (tnode==xnode) DeleteTree(&tnode);
                    else
                    {
                        SearchSource(tnode, xnode, &ynode);
                        if (ynode->ltree==xnode)
                            ynode->ltree=NULL;
                        else ynode->rtree=NULL;
                        DeleteTree(&xnode);
                    }
                }
                else {printf("Syntax error\n");fflush(stdin);}
            }
            else {printf("Syntax error\n");fflush(stdin);}
        }
        else
        if (strcmp(word,"stats")==0)
        {
            Stats(tnode, &depth, &leavesnum, &nodesnum, &innodesnum, &isheap,0);
            printf(" Tree depth %d\n Leaves number %d\n Nodesnum %d\n Innodesnum %d\n Isnot heap %d\n ",
                   depth, leavesnum, nodesnum, innodesnum, isheap);
        }
        else
        if (strcmp(word,"help")==0)
        {
            printf("List of commands: \nadd root <val>\nadd node <val> parent/sibling <val>"
                   "\nchange <val> to <val>\ndelete tree\ndelete node <val>\nstats\nshow\n"
                   "save <filename>\nload <filename>\n");
        }
        else
        if (strcmp(word,"change")==0)
        {
                scanf(" %s", word);
                searchkey=atoi(word);
                if (searchkey)
                {
                    SearchTree(searchkey, tnode, &xnode);
                    scanf(" %s", word);
                    if (strcmp(word,"to")==0)
                    {
                        scanf(" %s", word);
                        key=atoi(word);
                        if (key)
                        {
                            if (xnode)
                            {
                                SearchTree(key, tnode, &ynode);
                                if (ynode==NULL)
                                    xnode->key=key;
                                else printf("This value already exists\n");
                            }
                        }
                        else {printf("Syntax error\n");fflush(stdin);}
                    }
                    else {printf("Syntax error\n");fflush(stdin);}
                }
                else {printf("Syntax error\n");fflush(stdin);}
        }
        else {printf("Syntax error\n");fflush(stdin);}
        fflush(stdin);
        printf("Press any key\n");
        getchar();
        system("CLS");
        printf("Enter command, help or exit: \n");
        scanf("%s", word);
    }
    return 0;
}
