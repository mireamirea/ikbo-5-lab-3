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

TNODE *AddTree(int key);                                    //Выделение памяти под дерево с заданным ключом
void SearchTree(int searchkey, TNODE *root, TNODE **x);     //Поиск в дереве по ключу. Возвращает адрес узла в х
void SearchParent(TNODE *root, TNODE *child, TNODE **x);    //Поиск в дереве по сыну. Возвращает адрес узла в х
void AddChild(TNODE *parent, int key);                      //Добавление потомка
void ShowTree(TNODE *tp, int Space);                        //Вывод дерева
void SaveTree(TNODE *root);                                 //Сохранить дерево в файл
void LoadTree(TNODE **root);                                //Загрузать дерево из файла
void GetNode(TNODE **root, FILE **fp);                      //Получение узла
void SaveNode(TNODE *root, FILE **fp);                      //Сохранение узла
void DeleteTree(TNODE **root);                              //Удаление дерева
void Stats(TNODE *root, int *depth, int *leavesnum, int *nodesnum, int *innodesnum, int *isheap, int flag);     //Получение статистики

int main()
{
    TNODE *tnode = NULL; //Хранит корень
    TNODE *xnode;        //Хранит найденный узел
    TNODE *ynode;        //Хранит второй найденный узел
    char word[255];      //Для хранения команд
    int key,searchkey;   //Для хранения значений ключей
    int depth, leavesnum, nodesnum, innodesnum, isheap, length; //Для статистики
    printf("Enter command: "); //Далее идет реализация интерфейса со множеством условных операторов...
    scanf("%s", &word);
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
                    scanf(" %s", &word);
                    SearchTree(key, tnode, &xnode);
                    if (xnode!=NULL)
                    {
                        printf("Node already exists\n");
                    }
                    else
                    {
                        if (strcmp(word,"parent")==0)
                        {
                            scanf(" %s", &word);
                            if (searchkey=atoi(word))
                            {
                                SearchTree(searchkey, tnode, &xnode);
                                AddChild(xnode,key);
                                xnode=NULL;
                            }
                            else
                            if (strcmp(word,"root")==0)
                            {
                                AddChild(tnode,key);
                            }
                            else {printf("Syntax error\n");fflush(stdin);}
                        }
                        else
                        if (strcmp(word,"sibling")==0)
                        {
                            if (scanf(" %d", &searchkey))
                            {
                                SearchTree(searchkey, tnode, &xnode);
                                SearchParent(tnode, xnode, &ynode);
                                AddChild(ynode, key);
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
                    ShowTree(tnode,1);
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
            scanf(" %s", &word);
            if (strcmp(word,"tree")==0)
                DeleteTree(&tnode);
            else
            if (strcmp(word,"node")==0)
            {
                scanf(" %s", &word);
                if (key=atoi(word))
                {
                    SearchTree(key, tnode, &xnode);
                    if (xnode == NULL) printf("Error. No such node.");
                    else if (tnode==xnode) DeleteTree(&tnode);
                    else
                    {
                        SearchParent(tnode, xnode, &ynode);
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
            printf(" Tree depth %d\n Leaves number %d\n Nodesnum %d\n Innodesnum %d\n Isnot heap %d\n ",depth, leavesnum, nodesnum, innodesnum, isheap);
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
                scanf(" %s", &word);
                if (searchkey=atoi(word))
                {
                    SearchTree(searchkey, tnode, &xnode);
                    scanf(" %s", &word);
                    if (strcmp(word,"to")==0)
                    {
                        scanf(" %s", &word);
                        if (key=atoi(word))
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
        scanf("%s", &word);
    }
    return 0;
}
TNODE *AddTree(int key)//Добавление дерева с данным ключом
{
    TNODE *tnodep;
    tnodep = (TNODE*) malloc (sizeof(TNODE)); //Выделение памяти
    tnodep->key = key;  //присваиваем значение ключа
    tnodep->rtree = tnodep->ltree = NULL;//присваиваем указателям нул
}
void ShowTree(TNODE *tp, int space)//Вывод дерева (с поворотом на 90)
{
    int i;
    if (tp!=NULL) //если узел не нул
    {
        ShowTree(tp->rtree,space+1);//Печать правого дерева (рекурсивно)
        for (i=0; i<space; i++) printf("\t");//Печать отступа
        printf("%d\n",tp->key); //Печать значения узла
        ShowTree(tp->ltree,space+1);//Печать левого дерева (рекурсивно)
    }
}
void AddChild(TNODE *tnode, int key)//Добавление потомка
{
    if (tnode!=NULL) //Если отец существует
    {
        if (tnode->ltree==NULL) tnode->ltree=AddTree(key); //Если есть место, добавляем слева
        else if (tnode->rtree==NULL) tnode->rtree=AddTree(key); //Иначе если есть место добавляем справа
        else printf("Error. No free children\n"); //Иначе выдаем ошибку
    }
    else printf("Error. Can not add to nonexistent node\n");//Если отец нул, ошибка
}
void SearchTree(int searchkey, TNODE *root, TNODE **x)//Поиск в дереве по ключу
{
    if (root!=NULL) //Если узел не нул
    {
        if (root->key==searchkey) (*x)=root;//если нашли, возвращаем х, иначе
        else
        {
            if (root->ltree!=NULL) SearchTree(searchkey, root->ltree, x);//ищем в левом дереве
            if (root->rtree!=NULL) SearchTree(searchkey, root->rtree, x);//ищем в правом дереве
        }
    }
}
void SearchParent(TNODE *root, TNODE *child, TNODE **x)//Ищем отца
{
    if ((child!=NULL)&&(root!=NULL))//Если сын и корень не нул
    {
        if ((root->ltree==child)||(root->rtree==child)) (*x)=root; //если один из сыновей - нужный, возвращаем адрес, иначе
        else
        {
            if  (root->ltree!=NULL) SearchParent(root->ltree, child, x);//ищем в левом
            if  (root->rtree!=NULL) SearchParent(root->rtree, child, x);//ищем в правом
        }
    }
    else printf("Error. Sib node does not exist\n");
}
void SaveTree(TNODE *root) //Сохранение в файл
{
    char fname[255];//Для имени файла
    FILE *fp;//Указатель на файл
    scanf(" %s",fname);//Читаем имя
    fflush(stdin);
    if (fp=fopen(fname, "w"))//Если открылся для записи
    {
        SaveNode(root, &fp);//Функция сохранения узла с рекурсией
        fclose(fp);//Закрытие файла
    }
    else printf("Error. Can not open file\n");
}
void LoadTree(TNODE **root)//Загрузка файла
{
    char fname[255];//Для имени файла
    FILE *fp;//Указатель на файл
    scanf(" %s",fname);//Чтение имени
    fflush(stdin);
    if (fp=fopen(fname, "r"))//Если файл открылся для чтения
    {
        GetNode(root, &fp);//Функция записи узла с рекурсией
        fclose(fp);//Закрытие файла
    }
    else printf("Error. Incorrect filename\n");
}
void GetNode(TNODE **root, FILE **fp)//Функция записи узла с рекурсией
{
    if (!feof(*fp))//Если не конец файла
    {
        (*root)= AddTree(0);//Выделяем память для хранения узла
        fread((*root),sizeof(TNODE),1,*fp);//Читаем структуру из файла
        if ((*root)->ltree!=NULL) GetNode(&((*root)->ltree), fp);//Вызываем для левого дерева
        if ((*root)->rtree!=NULL) GetNode(&((*root)->rtree), fp);//Вызываем для правого дерева
    }
}
void SaveNode(TNODE *root, FILE **fp)//Функция сохранения узла с рекурсией
{
    if (root!=NULL)//Если есть узел
    {
        fwrite(root, sizeof(TNODE),1,*fp);//Запись структуры в файл
        if (root->ltree!=NULL) SaveNode(root->ltree,fp);//Вызываем для левого дерева
        if (root->rtree!=NULL) SaveNode(root->rtree,fp);//Вызываем для правого дерева
    }
}
void DeleteTree(TNODE **root)//Удаление дерева
{
    if (*root!=NULL)//Если дерево есть
    {
        if ((*root)->ltree!=NULL) DeleteTree(&((*root)->ltree));//Если есть левое, идем по нему
        if ((*root)->rtree!=NULL) DeleteTree(&((*root)->rtree));//Если есть правое, идем по нему
        if (((*root)->ltree==NULL) && ((*root)->rtree==NULL))//Если нет потомков
        {
            free((*root)->ltree);//Очистка памяти
            free((*root)->ltree);//Очистка памяти
            (*root)=NULL;//Удаляем значение указателя
        }
    }
    else printf("Tree is already empty\n");
}
void Stats(TNODE *root, int *depth, int *leavesnum, int *nodesnum, int *innodesnum, int *isheap, int flag)//Подсчет статистики
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
