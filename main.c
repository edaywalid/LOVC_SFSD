// main.c
#define RAYGUI_IMPLEMENTATION
#include "include/raylib.h"
#include "include/raygui.h"
#include "include/abstract.h"
#include <stdbool.h>

File file;

// Declare global variables
const int screenWidth = 800;
const int screenHeight = 800;
int currentPage = 0;
bool editMode1 = false;
bool editMode2 = false;
bool editMode3 = false;
bool editMode4 = false;
bool editMode5 = false;

char id[50];
char name[50];
char avrage[50];

student students[12] = {
    {"Alice", 2, 85.5},
    {"Bob", 1, 92.0},
    {"Alice", 2, 85.5},
    {"Bob", 1, 92.0},
    {"Alice", 2, 85.5},
    {"Bob", 1, 92.0},
    {"Alice", 2, 85.5},
    {"Bob", 1, 92.0},
    {"Alice", 2, 85.5},
    {"Bob", 1, 92.0},
    {"Alice", 2, 85.5},
    {"Bob", 1, 92.0},
    // Add more student data as needed
};

void SaveFormData(student *formData);
void insert_form();
void search_form();
void delete_form();
void DrawStudentTable();
void search_visualisation(int x);

// Function to draw content for Page 0
void home_page()
{
    // Draw content for Page 0
    GuiSetStyle(DEFAULT, TEXT_SIZE, 48);
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    int labelWidth = MeasureText("STUDENT MANAGING SYSTEM", 48);
    int labelX = (screenWidth - labelWidth) / 2;
    int labelY = 50;

    GuiLabel((Rectangle){labelX, labelY, labelWidth, 50}, "STUDENT MANAGING SYSTEM");

    // Button Style
    GuiSetStyle(DEFAULT, TEXT_SIZE, 18);
    int buttonWidth = MeasureText("0000000000", 18);
    int buttonX = (screenWidth - buttonWidth) / 2 - 15;

    // Insert
    if (GuiButton((Rectangle){buttonX, 150, buttonWidth + 15, 25}, "Insert"))
    {
        currentPage = 1;
    }

    // Search
    if (GuiButton((Rectangle){buttonX, 190, buttonWidth + 15, 25}, "Search"))
    {
        currentPage = 2;
    }

    // Delete
    if (GuiButton((Rectangle){buttonX, 230, buttonWidth + 15, 25}, "Delete"))
    {
        currentPage = 3;
    }

    // View List
    if (GuiButton((Rectangle){buttonX, 270, buttonWidth + 15, 25}, "View List"))
    {
        currentPage = 4;
    }
}

// Function to draw content for Insert
void insert_page()
{
    // Draw content for Page 1
    GuiSetStyle(DEFAULT, TEXT_SIZE, 48);
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    int labelWidth = MeasureText("Insert", 48);
    int labelX = (screenWidth - labelWidth) / 2;
    int labelY = 50;

    GuiLabel((Rectangle){labelX, labelY, labelWidth, 50}, "Insert");

    GuiSetStyle(DEFAULT, TEXT_SIZE, 18);
    int buttonWidth = MeasureText("Back", 18);
    int buttonX = (screenWidth - buttonWidth) / 2 - 15;
    if (GuiButton((Rectangle){700, 400, buttonWidth + 15, 25}, "Back"))
    {
        currentPage = 0;
    }
    student std;
    insert_form();
}

// Function to draw content for Delete
void delete_page()
{
    // Draw content for Page 1
    GuiSetStyle(DEFAULT, TEXT_SIZE, 48);
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    int labelWidth = MeasureText("Delete", 48);
    int labelX = (screenWidth - labelWidth) / 2;
    int labelY = 50;

    GuiLabel((Rectangle){labelX, labelY, labelWidth, 50}, "Delete");

    GuiSetStyle(DEFAULT, TEXT_SIZE, 18);
    int buttonWidth = MeasureText("Back", 18);
    int buttonX = (screenWidth - buttonWidth) / 2 - 15;
    if (GuiButton((Rectangle){700, 400, buttonWidth + 15, 25}, "Back"))
    {
        currentPage = 0;
    }

    delete_form();
}

// Function to draw content for Search
void search_page()
{
    // Draw content for Page 1
    GuiSetStyle(DEFAULT, TEXT_SIZE, 48);
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    int labelWidth = MeasureText("Search", 48);
    int labelX = (screenWidth - labelWidth) / 2;
    int labelY = 50;

    GuiLabel((Rectangle){labelX, labelY, labelWidth, 50}, "Search");

    GuiSetStyle(DEFAULT, TEXT_SIZE, 18);
    int buttonWidth = MeasureText("Back", 18);
    int buttonX = (screenWidth - buttonWidth) / 2 - 15;
    if (GuiButton((Rectangle){700, 400, buttonWidth + 15, 25}, "Back"))
    {
        currentPage = 0;
    }

    search_form();
}
// Function to draw content for View
void view_page()
{
    // Draw content for Page 1
    GuiSetStyle(DEFAULT, TEXT_SIZE, 48);
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    int labelWidth = MeasureText("View List", 48);
    int labelX = (screenWidth - labelWidth) / 2;
    int labelY = 50;

    GuiLabel((Rectangle){labelX, labelY, labelWidth, 50}, "View List");

    GuiSetStyle(DEFAULT, TEXT_SIZE, 18);
    int buttonWidth = MeasureText("Back", 18);
    int buttonX = (screenWidth - buttonWidth) / 2 - 15;
    if (GuiButton((Rectangle){700, 400, buttonWidth + 15, 25}, "Back"))
    {
        currentPage = 0;
    }

    DrawStudentTable();
}

// Function to check if the mouse is inside a text box
bool IsMouseInsideTextBox(Rectangle textBox)
{
    Vector2 mousePos = GetMousePosition();
    return (mousePos.x >= textBox.x && mousePos.x <= (textBox.x + textBox.width) &&
            mousePos.y >= textBox.y && mousePos.y <= (textBox.y + textBox.height));
}

void insert_form()
{
    student std;
    int labelWidth = MeasureText("000000:", 20);
    int inputWidth = 150;
    int buttonWidth = 80;

    GuiSetStyle(DEFAULT, TEXT_SIZE, 18);
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    // Check if mouse is inside the first text box
    if (IsMouseInsideTextBox((Rectangle){labelWidth, 120, inputWidth, 20}))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            editMode1 = !editMode1; // Toggle edit mode for TextBox 1
            editMode2 = false;      // Ensure TextBox 2 is not in edit mode
            editMode3 = false;      // Ensure TextBox 2 is not in edit mode
        }
    }

    // Check if mouse is inside the second text box
    if (IsMouseInsideTextBox((Rectangle){2 * labelWidth + 10 + inputWidth, 120, inputWidth, 25}))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            editMode2 = !editMode2; // Toggle edit mode for TextBox 2
            editMode1 = false;      // Ensure TextBox 1 is not in edit mode
            editMode3 = false;      // Ensure TextBox 1 is not in edit mode
        }
    }

    if (IsMouseInsideTextBox((Rectangle){3 * labelWidth + 10 + 2 * inputWidth, 120, inputWidth, 25}))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            editMode3 = !editMode3; // Toggle edit mode for TextBox 2
            editMode1 = false;      // Ensure TextBox 1 is not in edit mode
            editMode2 = false;      // Ensure TextBox 1 is not in edit mode
        }
    }

    // Draw text boxes
    int start = 0;
    Rectangle rec_label = {start, 120, labelWidth, 20};
    Rectangle rec_input = {start + labelWidth, 120, inputWidth, 20};
    GuiLabel(rec_label, "ID:");
    GuiTextBox(rec_input, id, 50, editMode1);

    start += labelWidth + inputWidth + 10;

    rec_label = (Rectangle){start, 120, labelWidth, 20};
    rec_input = (Rectangle){start + labelWidth, 120, inputWidth, 20};

    GuiLabel(rec_label, "Name:");
    GuiTextBox(rec_input, name, 50, editMode2);

    start += labelWidth + inputWidth + 10;

    rec_label = (Rectangle){start, 120, labelWidth, 20};
    rec_input = (Rectangle){start + labelWidth, 120, inputWidth, 20};

    GuiLabel(rec_label, "Avrage:");
    GuiTextBox(rec_input, avrage, 50, editMode3);

    // Save button
    if (GuiButton((Rectangle){710, 120, buttonWidth, 20}, GuiIconText(ICON_FILE_SAVE_CLASSIC, "Save")))
    {
        std.name = name;
        std.id = atoi(id);
        std.average = atof(avrage);
        // if(std.id  && std.average) {
        insert(std, &file);
        // insert(std, &file);
        SaveFormData(&std);
        // }
    }
}

// Search Form
void search_form()
{
    student std = {0};
    int labelWidth = MeasureText("000000:", 20);
    int inputWidth = 620;
    int buttonWidth = 80;

    GuiSetStyle(DEFAULT, TEXT_SIZE, 18);
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    // Check if mouse is inside the first text box
    if (IsMouseInsideTextBox((Rectangle){labelWidth, 120, inputWidth, 20}))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            editMode4 = !editMode5; // Toggle edit mode for TextBox 1
        }
    }

    // Draw text boxes
    int start = 0;
    Rectangle rec_label = {start, 120, labelWidth, 20};
    Rectangle rec_input = {start + labelWidth, 120, inputWidth, 20};
    GuiLabel(rec_label, "ID:");
    GuiTextBox(rec_input, id, 50, editMode1);

    // Save button
    if (GuiButton((Rectangle){710, 120, buttonWidth, 20}, GuiIconText(ICON_LENS, "")))
    {
        std.id = atoi(id);
        if (std.id && std.average)
        {
            SaveFormData(&std);
            // insert(std);
        }
    }

    search_visualisation(10);
}

// Delete Form
void delete_form()
{
    student std = {0};
    int labelWidth = MeasureText("000000:", 20);
    int inputWidth = 620;
    int buttonWidth = 80;

    GuiSetStyle(DEFAULT, TEXT_SIZE, 18);
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    // Check if mouse is inside the first text box
    if (IsMouseInsideTextBox((Rectangle){labelWidth, 120, inputWidth, 20}))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            editMode5 = !editMode5; // Toggle edit mode for TextBox 1
        }
    }

    // Draw text boxes
    int start = 0;
    Rectangle rec_label = {start, 120, labelWidth, 20};
    Rectangle rec_input = {start + labelWidth, 120, inputWidth, 20};
    GuiLabel(rec_label, "ID:");
    GuiTextBox(rec_input, id, 50, editMode1);

    // Save button
    if (GuiButton((Rectangle){710, 120, buttonWidth, 20}, GuiIconText(ICON_CROSS, "")))
    {
        // std.id = atoi(id);
        if (std.id && std.average)
        {
            // SaveFormData(&std);
            // delete();
        }
    }
}

void DrawStudentTable()
{
    // Table headers
    GuiLabel((Rectangle){100, 120, 200, 30}, "ID");
    GuiLabel((Rectangle){300, 120, 200, 30}, "Name");
    GuiLabel((Rectangle){500, 120, 200, 30}, "Average");

    // Draw each row of the table
    for (int i = 0; i < 12; i++)
    {
        // ID
        int maxDigits = snprintf(NULL, 0, "%d", students[i].id);
        char id[maxDigits];
        snprintf(id, sizeof(id), "%d", students[i].id);
        char avrage[5];
        sprintf(avrage, "%.2f", students[i].average);
        // printf("%d\n", students[i].id);
        printf("%s\n", id);
        GuiTextBox((Rectangle){100, 120 + (i + 1) * 40, 200, 30}, id, 10, false);
        // Name
        GuiTextBox((Rectangle){300, 120 + (1 + i) * 40, 200, 30}, students[i].name, 64, false);
        // Average
        GuiTextBox((Rectangle){500, 120 + (1 + i) * 40, 200, 30}, avrage, 5, false);
    }
}

void search_visualisation(int x)
{

        // Draw an arrow
        Vector2 point1 = { screenWidth / 2, screenHeight / 2  };
        Vector2 point2 = { screenWidth / 2 - 25, screenHeight / 2 - 50 };
        Vector2 point3 = { screenWidth / 2 + 25, screenHeight / 2 - 50 };
        DrawTriangle(point3, point2, point1, RED);

    for (int i = 0; i < x; i++)
    {
        int blockWidth = 150;
        int arrowWidth = 50;
        
        if(i != x - 1) {  
        Rectangle bodyRect = {25 + i * (blockWidth + arrowWidth) + blockWidth, 273, 40, 4};
        DrawRectangleRec(bodyRect, PINK);
        Vector2 point1 = {25 + i * (blockWidth + arrowWidth) + blockWidth + 40, 270};
        Vector2 point2 = {25 + i * (blockWidth + arrowWidth) + blockWidth + 40, 280};
        Vector2 point3 = {25 + i * (blockWidth + arrowWidth) + blockWidth + 50, 275};
        DrawTriangle(point1, point2, point3, PINK);
        }

        Rectangle rect = {25 + i * (blockWidth + arrowWidth), 250, blockWidth, 50};
        DrawRectangleRec(rect, GRAY);
    }

    DrawRectangleRec((Rectangle){0,250, 25, 50}, LIGHTGRAY);
    DrawRectangleRec((Rectangle){775,250, 25, 50}, LIGHTGRAY);
    
}

void SaveFormData(student *formData)
{

    // In this example, you can implement the logic to save the form data to a file or database
    // For now, we'll just print the data to the console
    printf("Saved Form Data:\n");
    printf("ID: %i\n", formData->id);
    printf("Name: %s\n", formData->name);
    printf("Average: %.2f\n", formData->average);
}

int main()
{
    openFile(&file, 'A');

    // Initialization

    InitWindow(screenWidth, screenHeight, "Page Navigation with raygui");

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw content based on the current page
        switch (currentPage)
        {
        case 1:
            insert_page();
            break;
        case 2:
            search_page();
            break;
        case 3:
            delete_page();
            break;
        case 4:
            view_page();
            break;

        default:
            home_page();
            break;
        }

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}

