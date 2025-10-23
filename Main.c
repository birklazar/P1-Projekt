#include <stdio.h>
FILE *Ingredients;
int main() {
    char input[100];
    double maengde=0;
    printf("Write an ingredient and an amount in grams: \n");
    scanf("%s%lf",&input,&maengde);
   Ingredients = fopen("Ingredients.txt", "a");
   fprintf(Ingredients, "%s, %.2lfg\n",input,maengde);
   fclose(Ingredients);
    return 0;
}
