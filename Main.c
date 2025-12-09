#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "function.h"

// The main, containing the switch statement that powers the whole program
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
        ingredientsFile = fopen("Ingredients.txt", "r");
        while(fgets(ingredientlist, MaxIng, ingredientsFile)){
          
          printf("%s", ingredientlist);
        }
        fclose(ingredientsFile);
        
      break;
    case 4:
      count = split_ingredients(ingredientsFile, ingredientNames, ingredientAmounts);
      suggest_recipe(ingredientNames, ingredientAmounts, count);
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
 ingredientsFile = fopen("Ingredients.txt", "r");

 /*Reads the file and separates the lines into two arrays by char and double,
 the double is found by finding the "," in each line, as the double is right afer*/
    while(fgets(ingredients, sizeof(ingredients), ingredientsFile) && count < 1000){
        char name [1000];
        double amount;

        if(sscanf(ingredients, "%s %lf %*s", name, &amount) == 3) {
        strcpy(localNames [count], name);    
        localAmount [count] = amount;
        count++;
        }
    }
    
    fclose(ingredientsFile);

 return count;
}
// Function that asks for and adds an ingredient
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
          
          fprintf(tempfile, "%s %.2lf %s\n", ingList[index].name, newamount, ingList[index].measurement);

          found = 1;
        continue;
        }
      }
      fputs(buffer, tempfile);
    }

    if (!found) {
      fprintf(tempfile, "%s %.2lf %s\n", ingList[index].name, newamount, ingList[index].measurement);
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
// Function that asks for and removes an ingredient
void removeIngredient(){
//Variables are initiated
    double amount; 
    char input [MaxIng]; 
    FILE* ingredientsFile;
    FILE* tempfile;
    char buffer[MaxLen];
    char target[MaxLen];
    int found = 0;

// ask user for ingredient name
    printf("Write an ingredient \n");
    scanf("%s",&input);

// search for ingredient in ingList[]
    int index = -1;
    for (int i = 0; i < 156; i++){
      if(strcasecmp(input, ingList[i].name) == 0) {
      index = i;
      break;
      }
    }
// ingredient not found
    if (index < 0){
      printf("Didn't find the ingredient");
      return;
    }
// open ingredient file to read and temp file to write updated data
    ingredientsFile = fopen("Ingredients.txt", "r");
    tempfile = fopen("temp.txt", "w");
// check that both files opened correctly
    if(!ingredientsFile || !tempfile) {
      printf("COULDN'T FIND FILE");
      return;
    }
// ask user how much to remove
    printf("Enter the amount: ");
    scanf("%lf", &amount);
    
    double newamount = amount;
    double existingAmount;
// read the file line by line
    while(fgets(buffer, sizeof(buffer), ingredientsFile)) {
      char validIng_name[MaxIng];
      double validIng_amt;
      char validIng_unit[16];
// parse: name amount unit
      if(sscanf(buffer, "%s %lf %s", validIng_name, &validIng_amt, validIng_unit) == 3) {
// check if this line is the ingredient we want to update        
        if(strcasecmp(validIng_name, ingList[index].name) == 0) {
// subtract the amount
          newamount = validIng_amt - amount;
          if (newamount < 0) {
          newamount = 0;
          }
// write updated ingredient into temp file
          fprintf(tempfile, "%s %.2lf %s\n", ingList[index].name, newamount, ingList[index].measurement);

          found = 1;
        continue;
        }
      }
      // write unmodified line to temp file
      fputs(buffer, tempfile);
    }
// if ingredient never appeared in file
    if (!found) {
      printf("Error: Ingredient was not found in your saved ingredients\n\n");
      return;
    }
// close both files
    fclose(ingredientsFile);
    fclose(tempfile);
// replace old file with updated one
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
// The algorithm that determines which recipe is missing the least ingredients
void suggest_recipe(char ingredient_arr[][1000],double ingredient_amount[1000], int UserIngCount){
    int recipe_length = RecipeCount;
  int missing_counter = 0;
  int existingCounter = 0;
  //Comparison
  for (int j = 0; j < RecipeCount; j++){
    existingCounter = 0;
    for (int i = 0; i < UserIngCount; i++){
      for(int k = 0; k < recipes[j].IngCount; k++){
  //Check that the right ingredient and a sufficient amount is in the saved ingredients
        if (strcasecmp(recipes[j].ingredients[k], ingredient_arr[i]) == 0 && recipes[j].amount[k] <= ingredient_amount[i]){
            existingCounter++;
            break;
        }
      }
    }
    missing_counter = recipes[j].IngCount-existingCounter;
    recipes[j].missingIngredients=missing_counter;
  }

  //Sort
  boubleSort(recipes,RecipeCount);
  chooseRecipe();
}
// Function used in suggest_recipe that allows the user to select a recipe and determine wether or not to remove the used ingredients
void chooseRecipe(){
  int input = 0;
  char input2;
if (RecipeCount<10){
  for (int i = 0; i < RecipeCount; i++){
    printf("%d. %s Which is missing %d ingredients\n",i+1,recipes[i].name,recipes[i].missingIngredients);
    printf("Calories : %.2lf kcal / Proteins : %.2lf g / Fats : %.2lf g\n\n",recipes[i].Calories,recipes[i].Protein,recipes[i].Fat);
  }
}else{
  for (int i = 0; i < 10; i++){
    printf("%d. %s Which is missing %d ingredients\n",i+1,recipes[i].name,recipes[i].missingIngredients);
    printf("Calories : %.2lf kcal / Proteins : %.2lf g / Fats : %.2lf g\n",recipes[i].Calories,recipes[i].Protein,recipes[i].Fat);
  }
}
printf("Choose a recipe by numbers listed above\n");
scanf("%d",&input);
input--;
printf("%s :\n",recipes[input].name);
for (int i = 0; i < recipes[input].IngCount; i++)
{
  printf("%s : %.2lf\n",recipes[input].ingredients[i],recipes[input].amount[i]);
}
printf("%s\n",recipes[input].guide);
printf("Would you like to remove the ingredients used in the recipe?\ny for yes n for no :  ");
scanf(" %c",&input2);
if (input2 == 'y'){
FILE *ingredientsFile = fopen("Ingredients.txt", "r");
FILE *tempfile = fopen("temp.txt", "w");
char buffer[MaxLen];
double amount = 0;

// check that both files opened correctly
if (!ingredientsFile || !tempfile) {
    printf("COULDN'T FIND FILE");
    return;
}

while (fgets(buffer, sizeof(buffer), ingredientsFile)) {
  char validIng_name[MaxIng];
  double validIng_amt;
  char validIng_unit[16];
  int found = 0;
  if (sscanf(buffer, "%s %lf %s", validIng_name, &validIng_amt, validIng_unit) == 3) {
    for (int i = 0; i < recipes[input].IngCount; i++) {
// get the subtraction amount for this recipe slot
      amount = recipes[input].amount[i];
      if (strcasecmp(validIng_name, recipes[input].ingredients[i]) == 0) {
        double newamount = validIng_amt - amount;
        if (newamount < 0) {
          newamount = 0;
        }
      fprintf(tempfile, "%s %.2lf %s\n",
      recipes[input].ingredients[i],
      newamount,
      validIng_unit);

      found = 1;
      break; // prevent multiple matches writing multiple lines
      }
    }
  }

    // write original line only if no update was written
  if (!found) {
      fputs(buffer, tempfile);
  }
}

// close files
  fclose(ingredientsFile);
  fclose(tempfile);

// replace Ingredients.txt with temp.txt
  if (remove("Ingredients.txt") != 0) {
    perror("Error deleting Ingredients.txt");
    return;
  }
  if (rename("temp.txt", "Ingredients.txt") != 0) {
    perror("Error renaming temp.txt");
    return;
  }

  }
}
// Function that initializes all of the recipes
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
  175,
  5,
  7,
  "This is how to make pancakes :",
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
500,
30,
20,
"This is how to make Chicken curry :",
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
138,
4.7,
8.7,
"This is how to make Chia Breakfast :",
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
300,
8,
2,
"This is how to make Oatmeal :",
};

recipes[RecipeCount++] = Pandekager;
recipes[RecipeCount++] = ChickenCurry;
recipes[RecipeCount++] = ChiaBreakfast;
recipes[RecipeCount++] = OatMeal; 



}
// Function that adds values to all of the ingredients
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
      }
          fclose(valid_ingredientsFile);
    }
// Our boublesort, that we used in suggest_recipe to suggest the recipe with the lowest amount of missing ingredients
void boubleSort(Recipes *Array2, int length) {
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
