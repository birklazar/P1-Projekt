#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

//function prototypes are created
void addingredient();
int split_ingredients(FILE* ingredientsFile, char localNames [] [1000], double localAmount []);
int compare_ignore_case(const char *a, const char *b);

int main() {
    //variables are initiated
    FILE *ingredientsFile;
    int count;
    char ingredientNames [1000] [1000];
    double ingredientAmounts [1000];
    int inputChoice; 
    void suggest_recipe(char input_arr[][100], int count);
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

void suggest_recipe(char ingredient_arr[][100], int count){
  char recipe_arr[2][5][100] = {
  {"pepper", "salt", "curry", "basil", "apple"},
  {"chicken", "salt", "strawberry", "basil", "salad"}
  };

  int recipe_length = sizeof(recipe_arr)/sizeof(recipe_arr[0][0]);
  int j = 0;
  int k = 0;
  int missing_counter = 0;

  for (int j = 0; j < recipe_length; j++)
  {
    for (int i = 0; i < recipe_length; i++)
    {
      if (compare_ignore_case(recipe_arr[k][j], ingredient_arr[i])){
        printf("%s = %s\n", recipe_arr[k][j], ingredient_arr[i]);
              }else if( recipe_arr[k][j] != "" && ingredient_arr[i] != ""){
        missing_counter++;
        printf("%s /= %s missing: %d\n", recipe_arr[k][j], ingredient_arr[i], missing_counter);
       
      }
      

    }
  }

}
// skriv om
int compare_ignore_case(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
            return 0; // not equal
        a++;
        b++;
    }
    return *a == *b;
}