#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#define MaxLen 1000
#define MaxIng 100

//function prototypes are created
typedef struct Recipes Recipes;
void addingredient();
int split_ingredients(FILE* ingredientsFile, char localNames [] [1000], double localAmount []);
int compare_ignore_case(const char *a, const char *b);
void suggest_recipe(char input_arr[][1000], int count);
void initRecipes();
void doubleSort(int *Array1, Recipes *Array2, int length);

struct Recipes{
  char name [MaxLen];
  int IngCount;
  char ingredients[MaxIng][MaxLen];
  double amount[MaxIng];
};
Recipes recipes[MaxIng];
int RecipeCount = 0;


int main() {
    //variables are initiated
    FILE *ingredientsFile;
    int count;
    char ingredientNames [1000] [1000];
    double ingredientAmounts [1000];
    int inputChoice; 
    
    initRecipes();
    /*
    
    printf("%s\n", recipes[0].name);
    for (int i = 0; i < 5; i++)
    {
      printf("ingredients nummer %d: %s %.2lf\n", i+1, recipes[0].ingredients[i], recipes[0].amount[i]);
    }
    */
    
    //First part of gui, the program asks for input to decide if the user would like to add an ingredient or to look for one

    //A menu for the user to navigate the program

    while(true) {

    printf("Choose 1: for add ingredient: \nChoose 2: for scan ingredient: \nChoose 3. Find recipe\nChoose 4. EXIT\n");
    scanf("%d",&inputChoice);
    
    switch (inputChoice)
    {
    case 1:
          addingredient();
        break;
    case 2: 
        count = split_ingredients(ingredientsFile, ingredientNames, ingredientAmounts);

        printf("count %d", count);
      for (int i = 0; i < count; i++)
      {
        printf("%s %.2lfg\n", ingredientNames [i], ingredientAmounts [i]);
      }
      break;
    case 3:
      count = split_ingredients(ingredientsFile, ingredientNames, ingredientAmounts);

      suggest_recipe(ingredientNames, count);
      break;
    case 4:
      printf("Exiting program");
      exit(EXIT_SUCCESS);
    default:
        break;
    }
    }
    
    return 0;
}

// Function that splits ingredients and the amount into their own array
 int split_ingredients(FILE* ingredientsFile, char localNames [] [1000], double localAmount [] ){
 
char ingredients[1000];
 int count=0;
 //we open the file "Ingredients.txt" and read it 
 ingredientsFile = fopen("ingredients.txt", "r");

 /*Reads the file and separates the lines into two arrays by char and double,
 the double is found by finding the "," in each line, as the double is right afer*/
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
    //Variables are initiated
    double maengde; 
    char input [1000]; 
    FILE* ingredientsFile;
    //asks for input and appends it to the back of the text file
    printf("Write an ingredient and an amount in grams: \n");
    scanf("%s %lf",&input,&maengde);
    ingredientsFile = fopen("ingredients.txt", "a");
    fprintf(ingredientsFile, "%s, %.2lfg\n",input,maengde);
    fclose(ingredientsFile);

}

void suggest_recipe(char ingredient_arr[][1000], int UserIngCount){
    int recipe_length = RecipeCount;
  int missing_counter = 0;
  int existingCounter = 0;
  int *missingIngredientArr = (int*)malloc (RecipeCount*sizeof(int));
  if(missingIngredientArr == NULL){
    printf("Download more ram\n");
    exit(EXIT_FAILURE);
  }
  for (int j = 0; j < RecipeCount; j++)
  {
    printf("\n\nNow comparing %s\n", recipes[j].name);
    existingCounter = 0;

    for (int i = 0; i < UserIngCount; i++){
      for(int k = 0; k < recipes[j].IngCount; k++){
        if (compare_ignore_case(recipes[j].ingredients[k], ingredient_arr[i])){
          //printf("%s = %s\n", recipes[j].ingredients[k], ingredient_arr[i]);
          existingCounter++;
          break;
        }
        else if( recipes[j].ingredients[k] != "" && ingredient_arr[i] != ""){
          
          //printf("%s /= %s missing: %d\n", recipes[j].ingredients[k], ingredient_arr[i], existingCounter);
        }
      }
    }
    missing_counter = recipes[j].IngCount-existingCounter;
    missingIngredientArr[j]=missing_counter;
    printf("%d\n",missingIngredientArr[j]);
  }
    for (int i = 0; i < RecipeCount; i++){
  printf("%s\n",recipes[i].name);
  printf("%d\n",missingIngredientArr[i]);
  }
  doubleSort(missingIngredientArr,recipes,RecipeCount);
  for (int i = 0; i < RecipeCount; i++){
  printf("%s\n",recipes[i].name);
  printf("%d\n",missingIngredientArr[i]);
  }
  free(missingIngredientArr);
}

int compare_ignore_case(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
            return 0; // not equal
        a++;
        b++;
    }
    return *a == *b;
}
void initRecipes(){
Recipes Pandekager = {
"Pandekager",
5,
{
  "Flour",
  "Egg",
  "Sugar",
  "Milk",
  "Salt"
},
{
  125,
  3,
  50,
  300,
  0.5
},
};
Recipes ChickenCurry = {

  "Chicken Curry",
  6,
{
  "Chicken",
  "Curry",
  "Cream",
  "Rice",
  "Apple",
  "Salt"
},
{
  300,
  15,
  150,
  150,
  0.5
},
};

recipes[RecipeCount++] = Pandekager;
recipes[RecipeCount++] = ChickenCurry; 


}

void doubleSort(int *Array1, Recipes *Array2, int length){
for (int i = 0; i < length; i++){
  for (int j = 0; j < length-i; j++)
  {
    //skidtet går out of bounds
    if(Array1[j]<Array1[j+1]){
      int temp = Array1[j];
      Array1[j] = Array1[j+1];
      Array1[j+1] = temp;

      Recipes temp2 = Array2[j];
      Array2[j] = Array2[j+1];
      Array2[j+1] = temp2;

    }
  }
  
  }
}