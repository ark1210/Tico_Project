
           #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 100

enum operator {
    READ, WRITE, ASSIGN, MOVE, LOAD, STORE, ADD, MINUS, MULT, MOD, EQ, LESS, JUMP, JUMPIF, TERM
};

struct instruction {
    enum operator op;
    unsigned char r1;
    unsigned char r2;
    unsigned char r3;
};

union value {
    unsigned char num;
    struct instruction inst;
};

int flag[256] = { 0 };

char * read_a_line (FILE *code_txt){
    static char buf[256];

    if(feof(code_txt))
        return 0x0 ;

    char * s = 0x0 ;

    s = fgets(buf, MAX_LENGTH, code_txt);

    return s;
}

const char *enumToString(enum operator op) {
    switch (op) {
        case READ: return "READ";
        case WRITE: return "WRITE";
        case ASSIGN: return "ASSIGN";
        case MOVE: return "MOVE";
        case LOAD: return "LOAD";
        case STORE: return "STORE";
        case ADD: return "ADD";
        case MINUS: return "MINUS";
        case MULT: return "MULT";
        case MOD: return "MOD";
        case EQ: return "EQ";
        case LESS: return "LESS";
        case JUMP: return "JUMP";
        case JUMPIF: return "JUMPIF";
        case TERM: return "TERM";
        default: return "UNKNOWN";
    }
}

enum operator toEnum(char *token) {
    if (strcmp(token, "READ") == 0) {
        return READ;
    } else if (strcmp(token, "EQ") == 0) {
        return EQ;
    } else if (strcmp(token, "JUMPIF") == 0) {
        return JUMPIF;
    } else if (strcmp(token, "MOVE") == 0) {
        return MOVE;
    } else if (strcmp(token, "MINUS") == 0) {
        return MINUS;
    } else if (strcmp(token, "JUMP") == 0) {
        return JUMP;
    } else if (strcmp(token, "WRITE") == 0) {
        return WRITE;
    } else
        return TERM;

}int atoi_v2(char * token){
    char * temp;
    int result;

    // token의 길이에 맞춰 동적 메모리 할당
    temp = (char *)malloc(strlen(token) + 1);
    if(temp == NULL){
        printf("Memory allocation failed.\n");
        return -1;
    }

    for(int i = 0; i < strlen(token); i++){
        if(isdigit(token[i])){
            //strcat을 하기 위해 tot 문자열 배열을 선언하고 맨 끝에 null을 넣어줌
            char tot[2] = { token[i], '\0' };
            strcat(temp, tot);
        }
    }

    result = atoi(temp);
    free(temp);
    return result;
}

void load_on_union(char * s, union value *memArray){
    // ':'을 기준으로 union의 인덱스와 명령어를 구분
    char * token;
    char * remain_sentence;
    token = strtok_r(s, ":", &remain_sentence); // '\n'을 추가한 구분자
    int idx = atoi(token);
    token = strtok_r(NULL, " ", &remain_sentence);
    // 명령어가 value인가 instruction인가?
    if (token != NULL && strstr(token, "\"") != NULL) {
        // value
        char * temp;
        sscanf(token, "\"%s\"", temp);
        printf("here : %d\n", atoi_v2(temp));
        memArray[idx].num = atoi_v2(temp);
        flag[idx] = 1;
    }
    else {
        // instruction
        int i = 0;
        while(token){
            if(strcmp(token, "9") > 0){
                // operator
                memArray[idx].inst.op = toEnum(token);
            } else {
                // operand
                int operand = atoi(token);
                if (i == 0) memArray[idx].inst.r1 = operand;
                else if (i == 1) memArray[idx].inst.r2 = operand;
                else if (i == 2) memArray[idx].inst.r3 = operand;
                i++;
            }
            token = strtok_r(NULL, " ", &remain_sentence); // '\n'을 추가한 구분자
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./tico <assembly_file>\n");
        return 1;
    }

    FILE *assemblyFile = fopen(argv[1], "r");

    printf("%s\n",argv[1]);
    if (assemblyFile == NULL) {
        printf("Error: 파일을 열 수 없습니다.\n");
        return 1;
    }

    union value memArray[256] = {0};

    char *line = read_a_line(assemblyFile);
    while (line != NULL) {
        // printf("%s\n", line);
        load_on_union(line, memArray);
        line = read_a_line(assemblyFile);
    }

    fclose(assemblyFile);

      // memArray 배열의 흐름을 출력
    for (int i = 0; i < 256; i++) {
        if(flag[i] == 1){
            printf("memArray[%d].num = %d\n", i, memArray[i].num);
        }else{
            printf("memArray[%d].inst.operator = %u\n", i, memArray[i].inst.op);
            printf("memArray[%d].inst.operand = %d, %d, %d\n", i, memArray[i].inst.r1, memArray[i].inst.r2, memArray[i].inst.r3);
        }
    }

    return 0;
}
