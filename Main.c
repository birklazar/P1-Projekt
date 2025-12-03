#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#define MaxLen 1000
#define MaxIng 100

//function prototypes are created
typedef struct Recipes Recipes;
typedef struct pantry_struct pantry_struct;
void addingredient();
int split_ingredients(FILE* ingredientsFile, char localNames [] [1000], double localAmount []);
int compare_ignore_case(const char *a, const char *b);
void suggest_recipe(char input_arr[][1000], int count);
void initRecipes();
void initIngredients();
void doubleSort(int *Array1, Recipes *Array2, int length);
int isInFile(FILE* fil, int length, char target[]);

struct Recipes{
  char name [MaxLen];
  int IngCount;
  char ingredients[MaxIng][MaxLen];
  double amount[MaxIng];
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

    printf("Choose 1: for add ingredient: \nChoose 2: for scan ingredient: \nChoose 3. Find recipe\nChoose 4. EXIT\n");
    scanf("%d",&inputChoice);
    
    switch (inputChoice)
    {
    case 1:
          addingredient();
        break;
    case 2: 
        count = split_ingredients(ingredientsFile, ingredientNames, ingredientAmounts);

        printf("Total ingredients %d", count);
        ingredientsFile = fopen("ingredients.txt", "r");
        for (size_t i = 0; i < count; i++)
        {
          fgets(ingredientlist, MaxIng, ingredientsFile);
          printf("%s", ingredientlist);
        }
        fclose(ingredientsFile);
        
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
    char input [MaxIng]; 
    FILE* ingredientsFile;


    //asks for input and appends it to the back of the text file
    ingredientsFile = fopen("Ingredients.txt", "a");
    printf("Write an ingredient \n");
    scanf("%s",&input);
    for (int i = 0; i < 156; i++)
    {
      if(strcasecmp(input, ingList[i].name) == 0 && isInFile(ingredientsFile, 6, input) != 1){
        printf("denne eksistere ikke i filen :)");
        printf("Skriv maengde: ");
        scanf("%lf", &maengde);
        fprintf(ingredientsFile, "%s %.2lf %s\n",ingList[i].name, maengde, ingList[i].measurement);
        printf("%s", ingList[i].measurement);
      }else if(strcasecmp(input, ingList[i].name) == 0 && isInFile(ingredientsFile, 6, input) == 1) {
        printf("denne eksistere i filen :)");
        printf("Skriv maengde: ");
        scanf("%lf", &maengde);
        int new_amount = ingList[i].amount+maengde;
        fputs("fuck", ingredientsFile);
      }

    }
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

void initIngredients() {
    char validIng[MaxIng];
    FILE* valid_ingredientsFile;
    valid_ingredientsFile = fopen("validIngredients.txt", "r");

    for (int i = 0; i < 158; i++)
    {
      fgets(validIng, MaxIng, valid_ingredientsFile);
      validIng[strcspn(validIng, "\n")] = 0;
      
      strcpy(ingList[i].name, validIng);

      if(i < 61) {
        strcpy(ingList[i].measurement, "Stk");

      }
      if (i >= 62 && i < 132) {
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

int isInFile(FILE* fil, int length, char target[]) {
  char infile[MaxLen];
  char line[256];

  fil = fopen("ingredients.txt", "r");

  while(fgets(line, sizeof(line), fil)) {
    sscanf(line, "%[^ \n]", infile);
    if(strcasecmp(target, infile) == 0){
        printf("infile: %s = true\n", infile);
        return 1;
      }
  }
    printf("infile: %s = false\n", infile);
    return 0;
    fclose(fil);
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