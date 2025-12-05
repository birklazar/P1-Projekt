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


