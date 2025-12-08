#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "function.h"
#define MaxLen 1000
#define MaxIng 100

//function prototypes are created
/*void addIngredient();
int split_ingredients(FILE* ingredientsFile, char localNames [] [1000], double localAmount []);
int compare_ignore_case(const char *a, const char *b);
void suggest_recipe(char input_arr[][1000], int count);
void initRecipes();
void initIngredients();
void doubleSort(int *Array1, Recipes *Array2, int length);
*/

struct Recipes{
  char name [MaxLen];
  int IngCount;
  char ingredients[MaxIng][MaxLen];
  double amount[MaxIng];
  int missingIngredients;
};

struct pantry_struct{
  char name [MaxLen];
  char measurement[MaxLen];
  double amount;
};

Recipes recipes[MaxIng];
pantry_struct ingList[MaxLen];
int RecipeCount = 0;


int main() {
    //variables are initiated
    FILE *ingredientsFile;
    int count;
    char ingredientNames [1000] [1000];
    double ingredientAmounts [1000];
    char ingredientlist[MaxIng];
    int inputChoice; 
    
    initRecipes();
    initIngredients();
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

    printf("Choose 1: for add ingredient:\nChoose 2: for remove ingredient: \nChoose 3: for scan ingredient: \nChoose 4. Find recipe\nChoose 5. EXIT\n");
    scanf("%d",&inputChoice);
    
    switch (inputChoice)
    {
    case 1:
        addIngredient();
        break;
    case 2: 
        removeIngredient();
        break;
    case 3:
      count = split_ingredients(ingredientsFile, ingredientNames, ingredientAmounts);

        printf("Total ingredients %d\n", count);
        ingredientsFile = fopen("ingredients.txt", "r");
        while(fgets(ingredientlist, MaxIng, ingredientsFile)){
          
          printf("%s", ingredientlist);
        }
        fclose(ingredientsFile);
        
      break;
    case 4:
      count = split_ingredients(ingredientsFile, ingredientNames, ingredientAmounts);
      suggest_recipe(ingredientNames, count);
      break;
    case 5:
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

        if(sscanf(ingredients, "%s %lf", name, &amount) == 2) {
        strcpy(localNames [count], name);    
        localAmount [count] = amount;
        count++;
        }
    }
    
    fclose(ingredientsFile);

 return count;
}

void addIngredient() {
   //Variables are initiated
    double amount; 
    char input [MaxIng]; 
    FILE* ingredientsFile;
    FILE* tempfile;
    char buffer[MaxLen];
    char target[MaxLen];
    int found = 0;


    printf("Write an ingredient \n");
    scanf("%s",&input);

    //asks for input and appends it to the back of the text file
    int index = -1;
    for (int i = 0; i < 156; i++)
    {
      if(strcasecmp(input, ingList[i].name) == 0) {
      index = i;
      break;
      }
    }

    if (index < 0){
      printf("Didn't find the ingredient");
      return;
    }
    
    ingredientsFile = fopen("Ingredients.txt", "r");
    tempfile = fopen("temp.txt", "w");
    
    if(!ingredientsFile || !tempfile) {
      printf("COULDN'T FIND FILE");
      return;
    }

    printf("Enter the amount: ");
    scanf("%lf", &amount);
    
    double newamount = amount;
    double existingAmount;

    while(fgets(buffer, sizeof(buffer), ingredientsFile)) {
      char validIng_name[MaxIng];
      double validIng_amt;
      char validIng_unit[16];

      if(sscanf(buffer, "%s %lf %s", validIng_name, &validIng_amt, validIng_unit) == 3) {
        if(strcasecmp(validIng_name, ingList[index].name) == 0) {

          newamount = validIng_amt + amount;
          
          fprintf(tempfile, "%s %2.lf %s\n", ingList[index].name, newamount, ingList[index].measurement);

          found = 1;
        continue;
        }
      }
      fputs(buffer, tempfile);
    }

    if (!found) {
      fprintf(tempfile, "%s %2.lf %s\n", ingList[index].name, newamount, ingList[index].measurement);
    }

    fclose(ingredientsFile);
    fclose(tempfile);

    if (remove("Ingredients.txt") != 0) {
      perror("Error deleting Ingredients.txt");
      return;
    }

    if (rename("temp.txt", "Ingredients.txt") != 0) {
      perror("Error renaming temp.txt");
      return;
    }

    printf("Ingredient updated successfully.\n");
}

void removeIngredient(){
//Variables are initiated
    double amount; 
    char input [MaxIng]; 
    FILE* ingredientsFile;
    FILE* tempfile;
    char buffer[MaxLen];
    char target[MaxLen];
    int found = 0;


    printf("Write an ingredient \n");
    scanf("%s",&input);

    //asks for input and appends it to the back of the text file
    int index = -1;
    for (int i = 0; i < 156; i++)
    {
      if(strcasecmp(input, ingList[i].name) == 0) {
      index = i;
      break;
      }
    }

    if (index < 0){
      printf("Didn't find the ingredient");
      return;
    }
    
    ingredientsFile = fopen("Ingredients.txt", "r");
    tempfile = fopen("temp.txt", "w");
    
    if(!ingredientsFile || !tempfile) {
      printf("COULDN'T FIND FILE");
      return;
    }

    printf("Enter the amount: ");
    scanf("%lf", &amount);
    
    double newamount = amount;
    double existingAmount;

    while(fgets(buffer, sizeof(buffer), ingredientsFile)) {
      char validIng_name[MaxIng];
      double validIng_amt;
      char validIng_unit[16];

      if(sscanf(buffer, "%s %lf %s", validIng_name, &validIng_amt, validIng_unit) == 3) {
        if(strcasecmp(validIng_name, ingList[index].name) == 0) {

          newamount = validIng_amt - amount;
          
          fprintf(tempfile, "%s %2.lf %s\n", ingList[index].name, newamount, ingList[index].measurement);

          found = 1;
        continue;
        }
      }
      fputs(buffer, tempfile);
    }

    if (!found) {
      printf("Error: Ingredient was not found in your saved ingredients\n\n");
      return;
    }

    fclose(ingredientsFile);
    fclose(tempfile);

    if (remove("Ingredients.txt") != 0) {
      perror("Error deleting Ingredients.txt");
      return;
    }

    if (rename("temp.txt", "Ingredients.txt") != 0) {
      perror("Error renaming temp.txt");
      return;
    }

    printf("Ingredient updated successfully.\n");
}

void suggest_recipe(char ingredient_arr[][1000], int UserIngCount){
    int recipe_length = RecipeCount;
  int missing_counter = 0;
  int existingCounter = 0;
  //Comparison
  for (int j = 0; j < RecipeCount; j++){
    existingCounter = 0;
    for (int i = 0; i < UserIngCount; i++){
      for(int k = 0; k < recipes[j].IngCount; k++){
        if (compare_ignore_case(recipes[j].ingredients[k], ingredient_arr[i])){
          existingCounter++;
          break;
        }
        else if( recipes[j].ingredients[k] != "" && ingredient_arr[i] != ""){
        }
      }
    }
    missing_counter = recipes[j].IngCount-existingCounter;
    recipes[j].missingIngredients=missing_counter;
  }

  //Sort
  doubleSort(recipes,RecipeCount);
  //Print sorted
  for (int i = 0; i < RecipeCount; i++){
  printf("%d. %s Which is missing %d ingredients\n",i+1,recipes[i].name,recipes[i].missingIngredients);
  }
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
  0,
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
0,
};

Recipes ChiaBreakfast = {

  "Chia Breakfast",
  4,
{
  "Chia_Seeds",
  "Yogurt",
  "Berries",
  "Oats"
},
{
  300,
  150,
  50,
  50,
},
0,
};

Recipes OatMeal = {

  "Oatmeal",
  5,
{
  "Oats",
  "Berries",
  "Salt",
  "Sugar",
  "Apples"
},
{
  300,
  50,
  1,
  10,
  100
},
0,
};

recipes[RecipeCount++] = Pandekager;
recipes[RecipeCount++] = ChickenCurry;
recipes[RecipeCount++] = ChiaBreakfast;
recipes[RecipeCount++] = OatMeal; 



}

void initIngredients() {
    char validIng[MaxIng];
    FILE* valid_ingredientsFile;
    valid_ingredientsFile = fopen("validIngredients.txt", "r");

    for (int i = 0; i < 158; i++)
    {
      fgets(validIng, MaxIng, valid_ingredientsFile);
      validIng[strcspn(validIng, "\n")] = 0;
      
      strcpy(ingList[i].name, validIng);

      if(i < 37) {
        strcpy(ingList[i].measurement, "Pcs");
      }

      if (i >= 38 && i < 132) {
        strcpy(ingList[i].measurement, "g");
      }

      if (i >= 133 && i < 158) {
        strcpy(ingList[i].measurement, "ml");
      }



      /*if(strcasecmp(validIng, input) == 0) {

        if(i < 61) {
          if(strcasecmp(input, ))
        printf("Skriv maengde i stykker ");
        scanf("%lf", &maengde);
        fprintf(ingredientsFile, "%s, %.2lf Stk\n",input,maengde);
        }
        if(i >= 63 && i < 133) {
        printf("Skriv maengde i gram: ");
        scanf("%lf", &maengde);
        fprintf(ingredientsFile, "%s, %.2lfg\n",input,maengde);
        }
        if(i >= 135 && i < 158) {
        printf("Skriv maengde i ml: ");
        scanf("%lf", &maengde);
        fprintf(ingredientsFile, "%s, %.2lfml\n",input,maengde);
        }*/
      }
          fclose(valid_ingredientsFile);
    }

void doubleSort(Recipes *Array2, int length) {
    for (int i = 0; i < length - 1; i++) {
        for (int j = 0; j < length - 1 - i; j++) {
          if (Array2[j].missingIngredients > Array2[j + 1].missingIngredients) {
            Recipes temp = Array2[j];
            Array2[j] = Array2[j + 1];
            Array2[j + 1] = temp;
          }
        }
    }
}

