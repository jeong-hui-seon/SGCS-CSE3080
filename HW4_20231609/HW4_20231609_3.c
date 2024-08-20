#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

int ** maze, ** mark;
int top;
int EXIT_ROW, EXIT_COL;
FILE * mazeFile; FILE *resultFile;

typedef struct {
    short int row;
    short int col;
    short int dir;
} element;

typedef struct {
    short int vert;
    short int horiz;
} offsets;

offsets move[8] = {
    {-1, 0}, {-1, 1}, {0, 1}, {1, 1},
    {1, 0}, {1, -1}, {0, -1}, {-1, -1}
};

typedef struct node *node_pointer;
typedef struct node {
    node_pointer llink;
    element item;
    node_pointer rlink;
} Node;

node_pointer head, temp;

void dinsert(node_pointer head, node_pointer newnode) {
    if (head -> rlink == head) { // 리스트가 비어 있을 경우
        // 새 노드가 자기 자신을 가리키게 하여 원형 연결 구조를 형성
        newnode->llink = head;
        newnode->rlink = head;
        head->llink = newnode;
        head->rlink = newnode;
    } else { // 리스트에 이미 노드가 있는 경우
        // 기존 로직을 사용하여 새 노드를 리스트의 마지막에 삽입
        node_pointer last_node = head->llink;
        newnode->llink = last_node;
        newnode->rlink = head; // 마지막 노드의 다음 노드가 헤더 노드
        last_node->rlink = newnode;
        head->llink = newnode; // 헤더 노드의 이전 노드를 새 노드로 업데이트
    }
}

element ddelete(node_pointer head) {
    node_pointer deleted = head->llink;
    element tmp;
    tmp.row = deleted->item.row;
    tmp.col = deleted->item.col;
    tmp.dir = deleted->item.dir;

    deleted->llink->rlink = deleted->rlink;
    deleted->rlink->llink = deleted->llink;

    free(deleted);
    return tmp;
}

void printCircularList(node_pointer head, int row, int col, int nextRow, int nextCol) {
    if (head == NULL || head->rlink == head) {
        printf("The list is empty.\n");
        return;
    }

    node_pointer current = head->rlink;
    while (current != head){
        fprintf(resultFile,"%d %d\n", current->item.row, current->item.col);
        current = current->rlink;
    }
    fprintf(resultFile, "%d %d\n", row, col);
    fprintf(resultFile, "%d %d\n", nextRow, nextCol);
}


void path()
{
    int i, row, col, nextRow, nextCol, dir, found = FALSE;
    element position;
    mark[1][1] = 1; top = 0;
    node_pointer new_node=(node_pointer)malloc(sizeof(Node));
    new_node->item.row=1; new_node->item.col=1; new_node->item.dir=1;
    dinsert(head, new_node);

    while (top > -1 && !found) {
        position = ddelete(head);
        row = position.row; col = position.col, dir = position.dir;
        while (dir<8 && !found) {
            nextRow = row + move[dir].vert;
            nextCol = col + move[dir].horiz;
            if (nextRow == EXIT_ROW && nextCol == EXIT_COL){
                found = TRUE;
            }
            else if (!maze[nextRow][nextCol] && !mark[nextRow][nextCol]) {
                mark[nextRow][nextCol] = 1;
                new_node = (node_pointer)malloc(sizeof(Node)); // 항상 새로운 노드 생성
                new_node->item.row = row;
                new_node->item.col = col;
                new_node->item.dir = ++dir; // 새 위치에서 새로 시작
                dinsert(head, new_node);
                row = nextRow; col = nextCol; dir = 0;
            }
            else {
                ++dir;
            }
        }
    }
    if (found) {
        printCircularList(head, row, col, nextRow, nextCol);
    }
    else printf("The maze does not have a path \n");
}

int main(){
    char ch; int digit;
    int rows=0, cols=0, current_cols=0;
    head=(node_pointer)malloc(sizeof(Node));
    head->llink=head; head->rlink=head;
    temp=(node_pointer)malloc(sizeof(Node));
    mazeFile = fopen("maze.txt", "r");
    while(fscanf(mazeFile, "%1d", &digit)){
        current_cols++;
        ch = fgetc(mazeFile);
        if(ch=='\n' || ch==EOF){
            if(current_cols > cols) cols=current_cols;
            current_cols=0;
            rows++;
            if(ch==EOF){
                break;
            }
        }
        else{
            ungetc(ch, mazeFile);
        }
    }
    EXIT_ROW=rows-2; EXIT_COL=cols-2;
    
    maze=(int **)malloc(rows*sizeof(int*));
    for(int i=0; i<rows; i++){
        maze[i]=(int *)malloc(cols*sizeof(int));
    }
    mark=(int **)malloc(rows*sizeof(int*));
    for(int i=0; i<rows; i++){
        mark[i]=(int *)malloc(cols*sizeof(int));
    }
    rewind(mazeFile);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fscanf(mazeFile, "%1d", &maze[i][j]);
            mark[i][j]=0;
        }
    }
    fclose(mazeFile);
    resultFile = fopen("path.txt", "w");
    path(resultFile);
    fclose(resultFile);
    for (int i = 0; i < rows; i++) {
        free(maze[i]);
    }
    free(maze);
    for (int i = 0; i < rows; i++) {
        free(mark[i]);
    }
    free(mark);
    free(head);
    free(temp);
    return 0;
}