#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <conio.h>
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

    //First part of gui, the program asks for input to decide if the user would like to add/remove an ingredient or look for a recipe

    //A menu for the user to navigate the program

    while(true) {

    printf("Choose 1: for add ingredient:\nChoose 2: for remove ingredient: \nChoose 3: Display ingredients: \nChoose 4. Find recipe\nChoose 5. EXIT\n");
    scanf("%d",&inputChoice);
    
    switch (inputChoice)
    {
    case 1:
        system("cls");
        addIngredient();
        break;
    case 2: 
        system("cls");
        removeIngredient();
        break;
    case 3:
        count = split_ingredients(ingredientsFile, ingredientNames, ingredientAmounts);
        system("cls");
        printf("Total ingredients %d\n", count);
        ingredientsFile = fopen("Ingredients.txt", "r");
        while(fgets(ingredientlist, MaxIng, ingredientsFile)){
          
          printf("%s", ingredientlist);
        }
        fclose(ingredientsFile);
        printf("\n\n");
      break;
    case 4:
      system("cls");
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

        if(sscanf(ingredients, "%s %lf", name, &amount) == 2) {
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
    for (int i = 0; i < 164; i++)
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
    for (int i = 0; i < 164; i++){
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
  int missingarr[10];
  //Comparison
  for (int j = 0; j < RecipeCount; j++){
    existingCounter = 0;
    for (int i = 0; i < UserIngCount; i++){
      for(int k = 0; k < recipes[j].IngCount; k++){
  //Check that the right ingredient and a sufficient amount is in the saved ingredients
        if (strcasecmp(recipes[j].ingredients[k], ingredient_arr[i]) == 0 && recipes[j].amount[k] <= ingredient_amount[i]){
            existingCounter++;
            recipes[j].missingarr[k] = 1;
            //break;
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
  char unit[MaxLen];
  

if (RecipeCount<10){
  for (int i = 0; i < RecipeCount; i++){
    if(recipes[i].vegetarian == 1) {
    printf(COLOR_GREEN "%d. %s Which is missing %d ingredients (Price: %.2lfDKK)\n",i+1,recipes[i].name, recipes[i].missingIngredients, recipes[i].price);
    printf(COLOR_GREEN "Calories : %.2lf kcal / Proteins : %.2lf g / Fats : %.2lf g / Carbs : %.2lf g\n\n",recipes[i].Calories,recipes[i].Protein,recipes[i].Fat, recipes[i].carbs);
 
}else {
    printf(COLOR_WHITE"%d. %s Which is missing %d ingredients (Price: %.2lfDKK)\n",i+1,recipes[i].name, recipes[i].missingIngredients, recipes[i].price);
    printf(COLOR_WHITE"Calories : %.2lf kcal / Proteins : %.2lf g / Fats : %.2lf g / Carbs : %.2lf g\n\n",recipes[i].Calories,recipes[i].Protein,recipes[i].Fat, recipes[i].carbs);
    }
  }
}else{
  for (int i = 0; i < 10; i++){
    if(recipes[i].vegetarian == 1) {
    printf(COLOR_GREEN "%d. %s Which is missing %d ingredients (Price: %.2lfDKK)\n",i+1,recipes[i].name, recipes[i].missingIngredients, recipes[i].price);
    printf(COLOR_GREEN "Calories : %.2lf kcal / Proteins : %.2lf g / Fats : %.2lf g / Carbs : %.2lf g\n\n",recipes[i].Calories,recipes[i].Protein,recipes[i].Fat, recipes[i].carbs);
}else{
    printf(COLOR_WHITE"%d. %s Which is missing %d ingredients (Price: %.2lfDKK)\n",i+1,recipes[i].name, recipes[i].missingIngredients, recipes[i].price);
    printf(COLOR_WHITE"Calories : %.2lf kcal / Proteins : %.2lf g / Fats : %.2lf g / Carbs : %.2lf g\n\n",recipes[i].Calories,recipes[i].Protein,recipes[i].Fat, recipes[i].carbs);
    }
  }
}

printf(COLOR_WHITE"Choose a recipe by numbers listed above (Green is vegetarian)\n");
scanf("%d",&input);
input--;
system("cls");
printf(COLOR_BOLD"%s :\n",recipes[input].name, COLOR_OFF);
for (int i = 0; i < recipes[input].IngCount; i++)
{
  for (int j = 0; j < 164; j++)
  {
    if(strcasecmp(recipes[input].ingredients[i], ingList[j].name) == 0) {
      strcpy(unit, ingList[j].measurement);
      break;
    }
  }
  
  if(recipes[input].missingarr[i] == 1){
  
  printf(COLOR_WHITE"%s : %.2lf %s\n",recipes[input].ingredients[i],recipes[input].amount[i], unit);
  } else {
    printf(COLOR_RED"%s : %.2lf %s<--- Missing\n",recipes[input].ingredients[i],recipes[input].amount[i], unit);
  }
}
printf(COLOR_WHITE"%s\n",recipes[input].guide);
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
9,
{
  "Vanilla_pod",
  "Cardamom",
  "Sugar",
  "eggs",
  "Milk",
  "Beer",
  "salt",
  "Wheat_flour",
  "butter",
},
{
  0.5,
  5,
  50,
  3,
  300,
  10,
  1,
  125,
  15
},
  0,
  175,
  5,
  7,
  16,
  {},
  100,
  "This is how to make pancakes :",
  1
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
9,
{0},
100,
"This is how to make Chicken curry :",
0
};
Recipes SpinachWaffles= {
    "Spinach Waffles",
    9,
    {
      "Wheat_flour", 
      "Whole_wheat_flour",
      "Baking_powder",    
      "Spinach",   
      "Eggs", 
      "Milk", 
      "Butter", 
      "Salt", 
      "pepper", 
    },
    {
    150.0,  
    75.0, 
    2,  
    125.0, 
    4,
    250.0,
    30.0,
    1.0, 
    1.0, 
    },
    0, 
    1450,     
    68,
    58,  
    165,  
    {0},               
    60.0,      
"Rør hvedemel, fuldkornsmel og bagepulver godt sammen i en skål. Skyl spinaten grundigt og kom derefter spinat i en blender sammen med resten af ingredienserne, inkl. mel. Blend det sammen til en helt ensartet lind dej. Lad gerne dejen hvile i 15-30 minutter, så melet lige kan absorbere lidt af væden og tykne. Fordel lidt olie eller smør i et vaffeljern og steg vaflerne til de er gyldne og gennembagt.",
1,
};

Recipes ChickenBaconOnePot = {

  "Chicken Bacon One Pot",
  12,
{
  "Stock",
  "Chicken",
  "Bacon",
  "Garlic",
  "Onion",
  "Squash",
  "Tomatoes",
  "Thyme",
  "Rosemary",
  "Oregano",
  "Pasta",
  "Salt"
},
{
  1000,
  400,
  100,
  3,
  3,
  1,
  3,
  4,
  2,
  2,
  500,
  1
},
0,
2700,
110,
100,
200,
{0},
185,
"This is how to make One Pot Chicken Bacon Pasta :",
0
};

Recipes FriedRice = {

  "Fried Rice",
  10,
{
  "Vegetable_oil",
  "Shallots",
  "Peas",
  "Carrots",
  "Rice",
  "Soy_sauce",
  "Salt",
  "White_pepper",
  "Eggs",
  "Sesame oil",
},
{
  15,
  2,
  125,
  2,
  500,
  5,
  3,
  2,
  2,
  4,
},
0,
865,
18.7,
12.8,
164.1,
{0},
60,
"Mince 3 garlic cloves. Thinly slice 2 medium scallions crosswise on a slight diagonal, keeping the white parts separate from the light and dark green parts.\nHeat 2 tablespoons of the vegetable oil in a large wok or nonstick frying pan over medium-high heat until shimmering. Add the garlic, scallion whites, and 1 cup frozen peas and carrots. Stir-fry until the carrots and peas are thawed, about 2 minutes.\nAdd 4 cups cold cooked rice, breaking up any clumps of rice with your fingers as you add it in. Toss to combine with the peas and carrots. Add 1 tablespoon soy sauce, 1 1/2 teaspoons kosher salt, and 3/4 teaspoon ground white pepper. Stir to combine.\nPush the fried rice to one side of the pan. Add the remaining 1 teaspoon vegetable oil to the now-empty side of the pan, then crack 2 large eggs into the oil. Scramble into fluffy curds until almost set.\nTurn off the heat and stir the eggs into the fried rice, breaking up any large pieces of egg. Drizzle with 3/4 teaspoon toasted sesame oil and toss to combine. Taste and season with more kosher salt and white pepper as needed. Garnish with the reserved scallion greens.\n",
1
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
204,
7,
4,
30,
{0},
100,
"This is how to make Oatmeal :",
1
};

Recipes PastaPesto = {
  "PastaPesto",
  2,
{
  "Pasta",
  "Green_Pesto"

},
{
  500,
  130,

},
0,
1624,
39,
50,
284,
{0},
30,
"This is how to make PastaPesto : Cook the pasta al dente according to the instructions on the package. Reserve 200 ml of the pasta cooking water, then drain the rest. Return the pasta to the pot and toss it with the pesto over low heat. Adjust the consistency with the pasta water until the dish is nice and creamy. Taste and adjust with more salt and pepper if needed.",
1
};

Recipes Pasta_Alfredo = {
  "Pasta Alfredo",
  8,
  {
  "Pasta",
  "Cream", 
  "Garlic",
  "Butter",
  "Salt",
  "Black_Pepper",
  "Thyme",
  "Permasan",

  },
  {
  400,
  100,
  1,
  5,
  5,
  5,
  25,
  100,
},
  0,
  0,
  0,
  0,
  0,
  {0},
  100,
  "Test",
  1
};

Recipes Cacio_e_pepe = {
  "Cacioepepe",
  4,
{
  "Spaghetti",
  "Parmesan",
  "Butter",
  "Pepper"

},
{
  500,
  250,
  50,
  2,

},
0,
1200,
40,
39,
155,
{0},

80,
"This is how to make Cacio_e_pepe : Crush the peppercorns coarsely in a mortar or using a rolling pin. Toast the pepper in a dry pan large enough to hold all the pasta. Meanwhile, cook the pasta in a pot of generously salted water. After the pepper has toasted for 2 to 3 minutes, add a tablespoon of butter to the pan. Let it melt and foam, then add a couple of large spoonfuls of pasta cooking water. Reduce the heat and let it simmer gently. When the pasta is halfway cooked, transfer it directly from the pot to the pan. Keep adding pasta water little by little while the pasta finishes cooking in the pan. When the pasta is nearly al dente, add the cheese gradually while stirring constantly. Add a bit more pasta water if needed so the sauce ends up silky, creamy, and smooth. Serve immediately with extra cheese on top.",
1
};

recipes[RecipeCount++] = Pandekager;
recipes[RecipeCount++] = SpinachWaffles;
recipes[RecipeCount++] = ChickenCurry;
recipes[RecipeCount++] = ChickenBaconOnePot; 
recipes[RecipeCount++] = FriedRice;
recipes[RecipeCount++] = OatMeal;
recipes[RecipeCount++] = PastaPesto;  
recipes[RecipeCount++] = Cacio_e_pepe;  
recipes[RecipeCount++] = Pasta_Alfredo;


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

      if(i < 39) {
        strcpy(ingList[i].measurement, "Pcs");
      }

      if (i > 40 && i < 139) {
        strcpy(ingList[i].measurement, "g");
      }

      if (i >= 140 && i < 165) {
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
