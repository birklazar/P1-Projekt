typedef struct Recipes Recipes;
typedef struct pantry_struct pantry_struct;

#define MaxLen 1000
#define MaxIng 100

struct Recipes{
  char name [MaxLen];
  int IngCount;
  char ingredients[MaxIng][MaxLen];
  double amount[MaxIng];
  int missingIngredients;
  double Calories;
  double Protein;
  double Fat;
  char guide[MaxLen];
};

struct pantry_struct{
  char name [MaxLen];
  char measurement[MaxLen];
  double amount;
};

Recipes recipes[MaxIng];
pantry_struct ingList[MaxLen];
int RecipeCount = 0;

void addIngredient();
int split_ingredients(FILE* ingredientsFile, char localNames [] [1000], double localAmount []);
void suggest_recipe(char input_arr[][1000],double ingredient_amount[1000], int count);
void initRecipes();
void initIngredients();
void boubleSort(Recipes *Array2, int length);
int isInFile(FILE* fil, int length, char target[]);
void removeIngredient();
void chooseRecipe();