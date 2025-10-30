#include <stdio.h>
#include <string.h>
void addingredient();
int split_ingredients(FILE* ingredientsFile, char localNames [] [1000], double localAmount []);
int main() {
    FILE *ingredientsFile;

    int count;
    char ingredientNames [1000] [1000];
    double ingredientAmounts [1000];
    int inputChoice = 2; 
    scanf("%d",&inputChoice);
    switch (inputChoice)
    {
    case 1 :
          addingredient();
        break;
    case 2 : 
     count = split_ingredients(ingredientsFile, ingredientNames, ingredientAmounts);

        printf("count %d", count);
      for (int i = 0; i < count; i++)
      {
        printf("%s %.2lfg\n", ingredientNames [i], ingredientAmounts [i]);
      }
      break;
    default:
        break;
    }
   
   
    
    return 0;
}
 int split_ingredients(FILE* ingredientsFile, char localNames [] [1000], double localAmount [] ){
 
char ingredients[1000];
 int count=0;
 ingredientsFile = fopen("ingredients.txt", "r");

    while(fgets(ingredients, sizeof(ingredients), ingredientsFile) && count < 1000){
        char name [1000];
        double amount;

        if(sscanf(ingredients, "%[^,], %lf", name, &amount) == 2) {
        strcpy(localNames [count], name);    
        localAmount [count] = amount;
        count++;
        }
    }
    fclose(ingredientsFile);






 return count;
}

void addingredient() {

    int maengde; 
    char input [1000]; 
    FILE* ingredientsFile;
  printf("Write an ingredient and an amount in grams: \n");
    scanf("%s%lf",&input,&maengde);
    ingredientsFile = fopen("ingredients.txt", "a");
    fprintf(ingredientsFile, "%s, %.2lfg\n",input,maengde);
    fclose(ingredientsFile);

}