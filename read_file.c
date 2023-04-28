#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    signed char num;
    struct instruction inst;
};

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
    } else if (strcmp(token, "WRITE") == 0) {
        return WRITE;
    } else if (strcmp(token, "ASSIGN") == 0) {
        return ASSIGN;
    } else if (strcmp(token, "MOVE") == 0) {
        return MOVE;
    } else if (strcmp(token, "LOAD") == 0) {
        return LOAD;
    } else if (strcmp(token, "STORE") == 0) {
        return STORE;
    } else if (strcmp(token, "ADD") == 0) {
        return ADD;
    }else if (strcmp(token, "MINUS") == 0) {
        return MINUS;
    }else if (strcmp(token, "MULT") == 0) {
        return MULT;
    }else if (strcmp(token, "MOD") == 0) {
        return MOD;
    }else if (strcmp(token, "EQ") == 0) {
        return EQ;
    }else if (strcmp(token, "LESS") == 0) {
        return LESS;
    }else if (strcmp(token, "JUMP") == 0) {
        return JUMP;
    }
    else if (strcmp(token, "JUMPIF") == 0) {
        return JUMPIF;
    }else 
            return TERM;
}

void load_on_union(char * s, union value *memArray) {
    if (s == NULL) {
        return;
    }

    char input_line[256];
    strcpy(input_line, s);

    char * token;
    char * remain_sentence;
    token = strtok_r(input_line, ":", &remain_sentence);
    int idx = atoi(token);
    token = strtok_r(NULL, " ", &remain_sentence);
    if (token != NULL && strstr(token, "\"") != NULL) {
        char temp;
        sscanf(token, "\"%c\"", &temp);
        memArray[idx].num = temp;
    }
    else {
        int i = 0;
        while(token){
            if(strcmp(token, "9") > 0){
                memArray[idx].inst.op = toEnum(token);
            } else {
                int operand = atoi(token);
                if (i == 0) memArray[idx].inst.r1 = operand;
                else if (i == 1) memArray[idx].inst.r2 = operand;
                else if (i == 2) memArray[idx].inst.r3 = operand;
                i++;
            }
            token = strtok_r(NULL, " ", &remain_sentence);
        }
    }
}



int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./tico <assembly_file>\n");
        return 1;
    }

    FILE *assemblyFile = fopen(argv[1], "r");

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
    for (int i = 0; i < 25; i++) {
        if (memArray[i].inst.op >= 0 && memArray[i].inst.op <= 14) {
            printf("memArray[%d].inst.op = %s;\n", i, enumToString(memArray[i].inst.op));
            printf("memArray[%d].inst.r1 = %d;\n", i, memArray[i].inst.r1);
            printf("memArray[%d].inst.r2 = %d;\n", i, memArray[i].inst.r2);
            printf("memArray[%d].inst.r3 = %d;\n", i, memArray[i].inst.r3);
        } else if (memArray[i].num != 0) {
            printf("memArray[%d].num = '%c';\n", i, memArray[i].num);
        }
    }

    return 0;
}
