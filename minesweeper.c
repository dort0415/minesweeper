#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct Mine {
    int row;
    int column;
} Mine;

typedef struct Field {
    Mine mines[30];
    int numberOfMines;
    int table[10][10];
    int emptyFields;
} Field;

int CheckBombs(Field *field, int row, int column);
int Contains(Mine *mine, int length, int row, int column);
void GenerateBombs(Field *field);
void InitTable(int table[10][10]);
void PrintGame(Field *field, int row, int column);
int GameLogic(Field *field, int row, int column);
void ReadInput(Field *field, int *row, int *column);
int SaveGame(Field *field, char *name);
int LoadGame(Field *field, char *name);

int main(int argc, char **argv){
    
    Field field;
    if(argc == 2){
        field.numberOfMines = atoi(argv[1]);
        field.emptyFields = 100 - field.numberOfMines;
        int vege = 0;
        if(field.numberOfMines < 3 || field.numberOfMines > 30){
            printf("Az aknak szama 3 es 30 kozott lehet!\n");
            return 0;
        }

        GenerateBombs(&field);
        
        InitTable(field.table);

        PrintGame(&field, -1, -1);
        
        printf("\n");

        while(!vege){
            int row, column;
            ReadInput(&field, &row, &column);

            if(CheckBombs(&field, row, column)){
                printf("Vesztettel!\n");
                return 0;
            }

            PrintGame(&field, row, column);

            if(field.emptyFields <= 0){
                vege = 1;
                printf("Gratulalok! Nyertel!\n");
            }
        }

    }else{
        printf("Meg kell adni hogy mennyi aknat akarsz lerakni parameterkent!\n");
    }
    return 0;
}

int CheckBombs(Field *field, int row, int column){
    for(int i = 0; i < field->numberOfMines; i++){
        if(field->mines[i].row == row && field->mines[i].column == column){
            return 1;
        }
    }

    return 0;
}

int Contains(Mine *mine, int length, int row, int column){
    for(int i = 0; i < length; i++){
        if(mine[i].row == row && mine[i].column == column) return 1;
    }
    return 0;
}

void GenerateBombs(Field *field){
    srand(time(0));
    int numberOfMines = field->numberOfMines;
    int mineCount = 0;

    while(mineCount < numberOfMines){
        int randrow = rand() % 10 + 65;
        int randcolumn = rand() % 10;
        if(!Contains(field->mines, mineCount, randrow, randcolumn)){
            field->mines[mineCount].row = randrow;
            field->mines[mineCount].column = randcolumn;
            mineCount++;
        }
    }

    return;
}

void InitTable(int table[10][10]){
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            table[i][j] = -1;
        }
    }

    return;
}

void PrintGame(Field *field, int row, int column){
    //printf("Bombak szama: %d\n", field->numberOfMines);
    for(int i = 0; i < 11; i++){
        for(int j = 0; j < 11; j++){
            if(i == 0){
                if(j == 0){
                    printf("\t");
                }else{
                    printf("%d\t", (j - 1));
                }
            }else{
                if(j == 0){
                    printf("%c\t", (65 + (i - 1)));
                }else{
                    if((i - 1) == (row - 65) && (j - 1) == column){
                        if(field->table[i-1][j-1] == -1) field->emptyFields--;
                        field->table[i-1][j-1] = GameLogic(field, row, column);
                        printf("%d\t", field->table[i-1][j-1]);
                    }else{
                        if(field->table[i-1][j-1] == -1){
                            printf("X\t");
                        }else{
                            printf("%d\t", field->table[i-1][j-1]);
                        }
                    }
                }
            }
        } 
        printf("\n");
    }

    return;    
}

int GameLogic(Field *field, int row, int column){
    int plus = 0;
    if((row - 66) >= 0){
        if(CheckBombs(field, (row - 1), column)){
            plus++;
        }
    }
    if((row - 64) < 10){
        if(CheckBombs(field, (row + 1), column)){
            plus++;
        }
    }
    if((column - 1) >= 0){
        if(CheckBombs(field, row, (column - 1))){
            plus++;
        }
    }
    if((column + 1) < 10){
        if(CheckBombs(field, row, (column + 1))){
            plus++;
        }
    }

    return plus;
}

void ReadInput(Field *field, int *row, int *column){
    char line[256];
    fgets(line, 256, stdin);
    if(strlen(line) == 3){
        *row = (int)line[0];
        *column = line[1] - '0';
    }else if(strlen(line) > 5){
        char command[5];
        char saveFile[256];
        strncpy(command, line, 4);
        command[4] = '\0';
        strncpy(saveFile, line + 5, strlen(line) - 6);
        saveFile[strlen(line) - 6] = '\0';
        if(strcmp(command, "save") == 0){
            if(SaveGame(field, saveFile)){
                printf("Sikeresen mentetted a jatekot!\n");
                ReadInput(field, row, column);
            }else{
                printf("Nem sikerult menteni!\n");
                ReadInput(field, row, column);
            }
        }else if(strcmp(command, "load") == 0){
            if(!LoadGame(field, saveFile)){
                printf("Nem sikerult betolteni a mentest!\n");
                ReadInput(field, row, column);
            }
        }else{
            printf("Hibas bemenet!\n");
            ReadInput(field, row, column);
        }
    }else{
        printf("Hibas bemenet!\n");
        ReadInput(field, row, column);
    }
}

int SaveGame(Field *field, char *name){
    FILE *file;
    file = fopen(name, "w");
    if(file == NULL){
        return 0;
    }

    fwrite(field, sizeof(Field), 1, file);

    fclose(file);
    return 1;
}

int LoadGame(Field *field, char *name){
    FILE *file;
    file = fopen(name, "r");
    if(file == NULL){
        return 0;
    }

    fread(field, sizeof(Field), 1, file);

    fclose(file);
    return 1;
}