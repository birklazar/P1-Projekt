#include <stdio.h>
#include <string.h>

int main() {
    FILE *ingredientsFile;
    char input[1000];
    char ingredients[1000];
    char Target [1000];
    char ingredientNames [1000] [1000];
    double ingredientAmounts [1000];
    double maengde=0;
    int count=0;

    /*
    printf("Write an ingredient and an amount in grams: \n");
    scanf("%s%lf",&input,&maengde);
    ingredientsFile = fopen("ingredientsFile.txt", "a");
    fprintf(ingredientsFile, "%s, %.2lfg\n",input,maengde);
    fclose(ingredientsFile);
    */

    ingredientsFile = fopen("ingredientsFile.txt", "r");

    while(fgets(ingredients, sizeof(ingredients), ingredientsFile) && count < 1000){
        char name [1000];
        double amount;
        if(sscanf(ingredients, "%[^,], %lf", name, &amount) == 2) {
        strcpy(ingredientNames [count], name);
        ingredientAmounts [count] = amount;
        count++;
        }
    }
    fclose(ingredientsFile);
    printf("count %d", count);
    for (int i = 0; i < count; i++)
    {
        printf("%s %.2lfg\n", ingredientNames [i], ingredientAmounts [i]);
    }
    
    return 0;
}
