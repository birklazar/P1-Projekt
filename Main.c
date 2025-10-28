#include <stdio.h>

int main() {
    FILE *Ingredients;
    char input[1000];
    char ingredients[1000];
    double maengde=0;


    printf("Write an ingredient and an amount in grams: \n");
    scanf("%s%lf",&input,&maengde);
    Ingredients = fopen("Ingredients.txt", "a");
    fprintf(Ingredients, "%s, %.2lfg\n",input,maengde);
    fclose(Ingredients);

    Ingredients = fopen("Ingredients.txt", "r");

    while(fgets(ingredients, 1000, Ingredients)){
        printf("%s", ingredients);
    }
    fclose(Ingredients);
    return 0;
}
