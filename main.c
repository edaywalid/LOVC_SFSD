// main.c
#define RAYGUI_IMPLEMENTATION
#include "include/raylib.h"
#include "include/raygui.h"
#include "include/abstract.h"
#include <stdbool.h>
#include <time.h>

File file;

// Declare global variables
const int screenWidth = 800;
const int screenHeight = 450;
int currentPage = 0;
bool editMode1 = false;
bool editMode2 = false;
bool editMode3 = false;
bool editMode4 = false;
bool editMode5 = false;

char id[10];
char name[50];
char avrage[10];

// View
float scrollValue = 0.0f;
float scrollSpeed = 5.0f;
// scrollValue = Clamp(scrollValue, 0.0f, (file.Header.lastBloc) * 40.0f);

// visualisation
student searchStudent = {0};
int blocPosition;
bool activeV = false;
int step = 0;
bool trainLeft = false;
bool trainRight = false;
bool trainCenter = false;
bool trainStop = true;

// float speed = 2.5;
// float trainTrans = 0;
// float bolcksTrans = 1;
// float distance = 0;

int speed = 5;
int trainTrans = 0;
int bolcksTrans = 1;
int distance = 0;

float searchTimer = 0.0f;


void reset()
{
    step = 0;
    activeV = false;
    trainTrans = 0;
    trainLeft = false;
    trainRight = false;
    trainCenter = false;
    trainStop = true;
    bolcksTrans = 1;
    distance = 0;
    searchTimer = 0.0f;
}

// Functions

void SaveFormData(student *formData);
void insert_form();
void search_form();
void delete_form();
void DrawStudentTable();
void search_visualisation(int blocNum, student std, bool active);

student stdTest = {"Abderrahmane", 0, 12, 0};

// Pages
//  Function to draw content for Page 0

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
   
   Sound clickSound = LoadSound("Sound/clickv2.wav");
    if (GuiButton((Rectangle){buttonX, 150, buttonWidth + 15, 25}, "Insert"))
    {   
        PlaySound(clickSound);    
        currentPage = 1;
    }

    // Search
    if (GuiButton((Rectangle){buttonX, 190, buttonWidth + 15, 25}, "Search"))
    {   
        PlaySound(clickSound);
        currentPage = 2;
    }

    // Delete
    if (GuiButton((Rectangle){buttonX, 230, buttonWidth + 15, 25}, "Delete"))
    {   
        PlaySound(clickSound); 
        currentPage = 3;
    }

    // View List
    if (GuiButton((Rectangle){buttonX, 270, buttonWidth + 15, 25}, "View List"))
    {   
        PlaySound(clickSound);
        currentPage = 4;
    }
    
}

int insertionStatus = 0;     // 0: No action, 1: Successful, 2: Student doesn't exist
float insertionTimer = 0.0f; // Timer for displaying success or error message
const float insertionDuration = 5.0f;
// Function to draw content for Delete

void insert_message()
{
    if (insertionStatus == 1)
    {
        DrawText("Successful!", screenWidth / 2 - MeasureText("Successful!", 30) / 2, screenHeight / 2 + 50, 30, GREEN);
    }else if (insertionStatus == 2)
    {
        DrawText("Invalid!", screenWidth / 2 - MeasureText("Invalid!", 30) / 2, screenHeight / 2 + 50, 30, RED);
    }
    // Update timer
    if (insertionStatus > 0)
    {
        insertionTimer += GetFrameTime();

        if (insertionTimer >= insertionDuration)
        {
            insertionStatus = 0; // Reset status after the desired duration
            insertionTimer = 0.0f;
        }
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
    // if (GuiButton((Rectangle){700, 400, buttonWidth + 15, 25}, "Back"))
    if (GuiButton((Rectangle){750, 400, 30, 30}, GuiIconText(ICON_HOUSE, "")))

    {   
        Sound clickSound = LoadSound("Sound/clickv2.wav");
        PlaySound(clickSound); 
        currentPage = 0;
    }
    student std;
    insert_form();
    // insert_message();
}

int deleteStatus = 0;     // 0: No action, 1: Successful, 2: Student doesn't exist
float deleteTimer = 0.0f; // Timer for displaying success or error message
const float deleteDuration = 5.0f;
// Function to draw content for Delete

void delete_message()
{
    if (deleteStatus == 1)  
        DrawText("Successful!", screenWidth / 2 - MeasureText("Successful!", 30) / 2, screenHeight / 2 + 50, 30, GREEN);
       else if (deleteStatus == 2)
        DrawText("Student Doesn't Exist!", screenWidth / 2 - MeasureText("Student Doesn't Exist!", 30) / 2, screenHeight / 2 + 50, 30, RED);
    // Update timer
    if (deleteStatus > 0)
    {
        deleteTimer += GetFrameTime();

        if (deleteTimer >= deleteDuration)
        {
            deleteStatus = 0; // Reset status after the desired duration
            deleteTimer = 0.0f;
            memset(id, '\0', 10);
        }
    }
}

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
    // if (GuiButton((Rectangle){700, 400, buttonWidth + 15, 25}, "Back"))
    if (GuiButton((Rectangle){750, 400, 30, 30}, GuiIconText(ICON_HOUSE, "")))

    {   
        Sound clickSound = LoadSound("Sound/clickv2.wav");
        PlaySound(clickSound); 
        currentPage = 0;
    }

    delete_form();
    delete_message();
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
    // if (GuiButton((Rectangle){700, 400, buttonWidth + 15, 25}, "Back"))
    if (GuiButton((Rectangle){750, 400, 30, 30}, GuiIconText(ICON_HOUSE, "")))

    {
        Sound clickSound = LoadSound("Sound/clickv2.wav");
        PlaySound(clickSound); 
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

    // Set up scroll panel
    Rectangle scrollPanel = { 100, 120, 600, 300 }; // Adjust the dimensions as needed
    BeginScissorMode(scrollPanel.x, scrollPanel.y, scrollPanel.width, scrollPanel.height);

    GuiSetStyle(DEFAULT, TEXT_SIZE, 18);
    int buttonWidth = MeasureText("Back", 18);
    int buttonX = (screenWidth - buttonWidth) / 2 - 15;
    // if (GuiButton((Rectangle){700, 400, buttonWidth + 15, 25}, "Back"))
    if (GuiButton((Rectangle){750, 400, 30, 30}, GuiIconText(ICON_HOUSE, "")))
    {
        Sound clickSound = LoadSound("Sound/clickv2.wav");
        PlaySound(clickSound); 
        currentPage = 0;
    }

    DrawStudentTable();

    // End scroll panel
    EndScissorMode();
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
            editMode1 = true;  // Toggle edit mode for TextBox 1
            editMode2 = false; // Ensure TextBox 2 is not in edit mode
            editMode3 = false; // Ensure TextBox 2 is not in edit mode
        }
    }

    // Check if mouse is inside the second text box
    else if (IsMouseInsideTextBox((Rectangle){2 * labelWidth + 10 + inputWidth, 120, inputWidth, 25}))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            editMode2 = true;  // Toggle edit mode for TextBox 2
            editMode1 = false; // Ensure TextBox 1 is not in edit mode
            editMode3 = false; // Ensure TextBox 1 is not in edit mode
        }
    }

    else if (IsMouseInsideTextBox((Rectangle){3 * labelWidth + 10 + 2 * inputWidth, 120, inputWidth, 25}))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            editMode3 = true;  // Toggle edit mode for TextBox 2
            editMode1 = false; // Ensure TextBox 1 is not in edit mode
            editMode2 = false; // Ensure TextBox 1 is not in edit mode
        }
    }

    else
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            editMode3 = false; // Toggle edit mode for TextBox 2
            editMode1 = false; // Ensure TextBox 1 is not in edit mode
            editMode2 = false; // Ensure TextBox 1 is not in edit mode
        }
    }

    // Draw text boxes
    int start = 0;
    Rectangle rec_label = {start, 120, labelWidth, 20};
    Rectangle rec_input = {start + labelWidth, 120, inputWidth, 20};
    GuiLabel(rec_label, "ID:");
    GuiTextBox(rec_input, id, 10, editMode1);

    start += labelWidth + inputWidth + 10;

    rec_label = (Rectangle){start, 120, labelWidth, 20};
    rec_input = (Rectangle){start + labelWidth, 120, inputWidth, 20};

    GuiLabel(rec_label, "Name:");
    GuiTextBox(rec_input, name, 50, editMode2);

    start += labelWidth + inputWidth + 10;

    rec_label = (Rectangle){start, 120, labelWidth, 20};
    rec_input = (Rectangle){start + labelWidth, 120, inputWidth, 20};

    GuiLabel(rec_label, "Avrage:");
    GuiTextBox(rec_input, avrage, 10, editMode3);

    // Save button
    if (GuiButton((Rectangle){710, 120, buttonWidth, 20}, GuiIconText(ICON_FILE_SAVE_CLASSIC, "Save")) || IsKeyPressed(KEY_ENTER))
    {   Sound clickSound = LoadSound("Sound/clickv2.wav");
        PlaySound(clickSound); 
        std.name = name;
        std.id = atoi(id);
        std.average = atof(avrage);
        std.LogicallyDeleted = 0;
        if (std.id && std.average)
        {
            // insert(std, &file);
            insert(std, &file);
            insertionStatus = 1;   // Set status to successful
            insertionTimer = 0.0f; // Reset the timer
        }
        else
        {
            insertionStatus = 2;   // Set status to student doesn't exist
            insertionTimer = 0.0f; // Reset the timer
        }

        memset(id, '\0', 10);
        memset(name, '\0', 50);
        memset(avrage, '\0', 10);
    }
}

// Search Form
void search_form()
{
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
            editMode4 = true; // Toggle edit mode for TextBox 1
        }
    }
    else
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            editMode4 = false; // Toggle edit mode for TextBox 1
        }
    }

    // Draw text boxes
    int start = 0;
    Rectangle rec_label = {start, 120, labelWidth, 20};
    Rectangle rec_input = {start + labelWidth, 120, inputWidth, 20};
    GuiLabel(rec_label, "ID:");
    GuiTextBox(rec_input, id, 50, editMode4);

    // Save button
    if (GuiButton((Rectangle){710, 120, buttonWidth, 20}, GuiIconText(ICON_LENS, "")) || IsKeyPressed(KEY_ENTER))
    {   Sound clickSound = LoadSound("Sound/clickv2.wav");
        PlaySound(clickSound); 
        int ID = atoi(id);
        int charPosition;
        if (ID)
        {
            printf("ok\n");
            searchStudent = getStudentFromLinkedList(ID, &blocPosition, &charPosition, &file) == "NOT FOUND" ? (student){0} : charToStudent(getStudentFromLinkedList(ID, &blocPosition, &charPosition, &file));
            printf("ok\n");
            if (!activeV)
            {
                activeV = true;
                step = 1;
            }
        }
    }
    search_visualisation(blocPosition, searchStudent, activeV);
}

// Delete Form
void delete_form()
{
    int DeleteId;
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
            editMode5 = true; // Toggle edit mode for TextBox 1
        }
    }
    else
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            editMode5 = false; // Toggle edit mode for TextBox 1
        }
    }

    // Draw text boxes
    int start = 0;
    Rectangle rec_label = {start, 120, labelWidth, 20};
    Rectangle rec_input = {start + labelWidth, 120, inputWidth, 20};
    GuiLabel(rec_label, "ID:");
    GuiTextBox(rec_input, id, 50, editMode5); // Corrected: Use editMode1 for TextBox

    int timer;
    // Save button
    if (GuiButton((Rectangle){710, 120, buttonWidth, 20}, GuiIconText(ICON_CROSS, "")))
    {   Sound clickSound = LoadSound("Sound/clickv2.wav");
        PlaySound(clickSound); 
        int DeleteId = atoi(id);
        if (DeleteId)
        {
            int check = delete (DeleteId, &file);

            if (check)
            {   
                Sound clickSound = LoadSound("Sound/REALSUCCES.wav");
                PlaySound(clickSound);
                deleteStatus = 1;   // Set status to successful
                deleteTimer = 0.0f; // Reset the timer
            }
            else
            {   
                Sound clickSound = LoadSound("Sound/wrong.wav");
                PlaySound(clickSound);
                deleteStatus = 2;   // Set status to student doesn't exist
                deleteTimer = 0.0f; // Reset the timer
            }
        }
    }
}

void DrawStudentTable()
{
    // Table headers
    GuiLabel((Rectangle){100, 120, 200, 30}, "ID");
    GuiLabel((Rectangle){300, 120, 200, 30}, "Name");
    GuiLabel((Rectangle){500, 120, 200, 30}, "Average");

    int Place = 0;

    readBloc(&file, 0);
    int pos = 0, bloc = 0;
    int i = 0;
    int blocCount = 0;

    // Scrollbar variables
    Rectangle scrollbar = {screenWidth - 20, 0, 20, screenHeight};
    // float scrollValue = 0.0f;
    // float scrollSpeed = 5.0f;

    if (IsKeyDown(KEY_DOWN))
    {
        scrollValue += scrollSpeed;
    }
    else if (IsKeyDown(KEY_UP))
    {
        scrollValue -= scrollSpeed;
    }

    // Clamp scrollValue to keep it within a valid range
    // scrollValue = Clamp(scrollValue, 0.0f, (file.Header.lastBloc) * 40.0f);

    while (blocCount <= (&file)->Header.lastBloc)
    {

        while (blocCount <= (&file)->Header.lastBloc && i < MAX_SIZE && buffer.charArray[i] != '|')
        {

            i++;
            if (i == MAX_SIZE)
            {
                readBloc((&file), ++blocCount);
                i = 0;
            }
        }
        if (blocCount > (&file)->Header.lastBloc)
            break;
        int k = 0;
        i++;
        if (i == MAX_SIZE)
        {
            readBloc((&file), ++blocCount);
            i = 0;
        }

        char *studentID = malloc(100);
        while (i < MAX_SIZE && buffer.charArray[i] != '$')
        {
            studentID[k++] = buffer.charArray[i++];
            if (i == MAX_SIZE)
            {
                readBloc((&file), ++blocCount);
                i = 0;
            }
        }
        studentID[k] = '\0';

        if (i < MAX_SIZE - 1 && buffer.charArray[i + 1] == '1' || i == MAX_SIZE - 1 && buffer.charArray[0] == '1')
        {
            free(studentID);
            continue;
        }

        student x = charToStudent(getStudentFromLinkedList(atoi(studentID), &bloc, &pos, &file));
        snprintf(id, 15, "%d", x.id);
        snprintf(avrage, 50, "%.2f", x.average);

        GuiTextBox((Rectangle){100, 120 + (Place + 1) * 40 - (int)scrollValue, 200, 30}, id, 10, false);
        // Name
        GuiTextBox((Rectangle){300, 120 + (1 + Place) * 40 - (int)scrollValue, 200, 30}, x.name, 64, false);
        // Average
        GuiTextBox((Rectangle){500, 120 + (1 + Place++) * 40 - (int)scrollValue, 200, 30}, avrage, 5, false);

        free(studentID);
        if (i == MAX_SIZE)
            i = 0;
    }

    // Draw the scrollbar
    DrawRectangleRec(scrollbar, LIGHTGRAY);
    DrawRectangleLinesEx(scrollbar, 1, GRAY);

    // Draw the scrollbar handle
    Rectangle scrollbarHandle = {screenWidth - 20, scrollValue, 20, 40};
    DrawRectangleRec(scrollbarHandle, GRAY);

    memset(id, '\0', 10);
}

int main()
{

    openFile(&file, 'A');
    student s1 = {"John", 1, 15.5, 0};
    student s2 = {"Mary", 2, 16.5, 0};
    student s3 = {"Peter", 3, 17.5, 0};
    student s4 = {"Paul", 4, 18.5, 0};
    student s5 = {"George", 5, 19.5, 0};
    student s9 = {"George", 11, 19.5, 0};
    student s6 = {"George", 90, 19.5, 0};
    student s7 = {"George", 22, 19.5, 0};
    student s8 = {"George", 13, 19.5, 0};


    insert(s3, &file);
    insert(s2, &file);
    insert(s5, &file);
    insert(s1, &file);
    insert(s4, &file);
    insert(s9, &file);
    insert(s6, &file);
    insert(s7, &file);
    insert(s8, &file);

    InitWindow(screenWidth, screenHeight, "LOVC");

    SetTargetFPS(60);

    InitAudioDevice();

    while (!WindowShouldClose())
    {

        // Initialization

        // Main game loop
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

    closeFile(&file);
    // De-Initialization
    CloseWindow();
    closeFile(&file);

    return 0;
}

void search_visualisation(int blocNum, student std, bool active)
{

    int numOfBlocs = file.Header.lastBloc + 1;
    Rectangle blocs[numOfBlocs];
    Color colorsBlocks[numOfBlocs];

    int blockWidth = 150;
    int arrowWidth = 50;
    int maxlength = std.name == NULL ? numOfBlocs * (blockWidth + arrowWidth) : (blocNum) * (blockWidth + arrowWidth);

    if (step == 0)
    {
        reset();
        for (int i = 0; i < numOfBlocs; i++)
        {

            colorsBlocks[i] = (Color){50, 50, 50, 255};
        }
    }
    if (step == 1)
    {
        trainLeft = true;
        trainStop = false;
        step = 2;
        for (int i = 0; i < numOfBlocs; i++)
        {

            colorsBlocks[i] = (Color){50, 50, 50, 255};
        }
    }

    if (activeV)
    {
        // Draw an arrow
        Vector2 point1 = {screenWidth / 2 + trainTrans, screenHeight / 2};
        Vector2 point2 = {screenWidth / 2 - 25 + trainTrans, screenHeight / 2 - 50};
        Vector2 point3 = {screenWidth / 2 + 25 + trainTrans, screenHeight / 2 - 50};
        DrawTriangle(point3, point2, point1, RED);

        if (step == 2)
        {
            if (trainLeft)
            {
                if (point1.x == (screenWidth - blockWidth) / 2)
                {
                    trainRight = true;
                    trainLeft = false;
                }
                trainTrans -= speed;
            }
            if (trainRight)
            {
                if (point1.x == (screenWidth + blockWidth) / 2)
                {
                    trainCenter = true;
                    trainRight = false;
                }
                trainTrans += speed;
            }
            if (trainCenter)
            {
                if (point1.x == (screenWidth) / 2)
                {
                    trainStop = true;
                    trainCenter = false;
                }
                trainTrans -= speed;
            }
            if (trainStop)
            {
                step = 3;
            }

            for (int i = 0; i < numOfBlocs; i++)
            {
                if (distance / (arrowWidth + blockWidth) == i)
                {
                    colorsBlocks[i] = (Color){0, 0, 255, 255};
                    continue;
                }
                else if (distance / (arrowWidth + blockWidth) > i)
                {
                    colorsBlocks[i] = (Color){255, 0, 0, 255};
                }
                else
                {
                    colorsBlocks[i] = (Color){50, 50, 50, 255};
                }
            }
        }
        for (int i = 0; i < numOfBlocs; i++)
        {

            if (i != numOfBlocs - 1)
            {
                Rectangle bodyRect = {(screenWidth - blockWidth) / 2 + i * (blockWidth + arrowWidth) + blockWidth + bolcksTrans, 273, 40, 4};
                DrawRectangleRec(bodyRect, GRAY);
                Vector2 point1 = {(screenWidth - blockWidth) / 2 + i * (blockWidth + arrowWidth) + blockWidth + 40 + bolcksTrans, 270};
                Vector2 point2 = {(screenWidth - blockWidth) / 2 + i * (blockWidth + arrowWidth) + blockWidth + 40 + bolcksTrans, 280};
                Vector2 point3 = {(screenWidth - blockWidth) / 2 + i * (blockWidth + arrowWidth) + blockWidth + 50 + bolcksTrans, 275};
                DrawTriangle(point1, point2, point3, GRAY);
            }

            blocs[i] = (Rectangle){(screenWidth - blockWidth) / 2 + i * (blockWidth + arrowWidth) + bolcksTrans, 250, blockWidth, 50};
            DrawRectangleRec(blocs[i], colorsBlocks[i]);
        }

        if (step == 3)
        {
            for (int i = 0; i < numOfBlocs; i++)
            {
                if (distance / (arrowWidth + blockWidth) == i)
                {
                    colorsBlocks[i] = (Color){0, 0, 255, 255};
                    continue;
                }
                else if (distance / (arrowWidth + blockWidth) > i)
                {
                    colorsBlocks[i] = (Color){255, 0, 0, 255};
                }
                else
                {
                    colorsBlocks[i] = (Color){50, 50, 50, 255};
                }
            }
            if ((int)distance % 200 == 0 && distance > 0)
            {

                if (distance == maxlength)
                {
                    step = 4;
                    if (std.name != NULL && blocNum > 0)
                    {
                        trainLeft = true;
                        trainStop = false;
                    }
                    else
                    {
                        trainLeft = false;
                        trainStop = true;
                    }
                }
                else
                {
                    trainLeft = true;
                    trainStop = false;
                    step = 1;
                }
                bolcksTrans -= speed;
                distance += speed;
            }
            else if (!maxlength)
            {
                step = 4;
                trainLeft = false;
                trainStop = true;
            }
            else
            {
                bolcksTrans -= speed;
                distance += speed;
            }
        }

        if (step == 4)
        {
            for (int i = 0; i < numOfBlocs; i++)
            {
                if (distance / (arrowWidth + blockWidth) == i)
                {
                    colorsBlocks[i] = (Color){0, 0, 255, 255};
                    continue;
                }
                else if (distance / (arrowWidth + blockWidth) > i)
                {
                    colorsBlocks[i] = (Color){255, 0, 0, 255};
                }
                else
                {
                    colorsBlocks[i] = (Color){50, 50, 50, 255};
                }
            }

            if (std.name != NULL && blocNum > 0)
            {

                if (trainLeft)
                {
                    if (point1.x == (screenWidth - blockWidth) / 2)
                    {
                        trainRight = true;
                        trainLeft = false;
                    }
                    trainTrans -= speed;
                }
                if (trainRight)
                {
                    if (point1.x == (screenWidth + blockWidth) / 2)
                    {
                        trainCenter = true;
                        trainRight = false;
                    }
                    trainTrans += speed;
                }
                if (trainCenter)
                {
                    if (point1.x == (screenWidth) / 2)
                    {
                        trainStop = true;

                        trainCenter = false;
                    }
                    trainTrans -= speed;
                }
                if (trainStop)
                {
                    trainTrans = speed;
                }
            }
            if (trainStop)
                step = 5;
        }

        if (step == 5)
        {

            for (int i = 0; i < numOfBlocs; i++)
            {
                if (distance / (arrowWidth + blockWidth) == i)
                {
                    colorsBlocks[i] = (Color){0, 0, 255, 255};
                    continue;
                }
                else if (distance / (arrowWidth + blockWidth) > i)
                {
                    colorsBlocks[i] = (Color){255, 0, 0, 255};
                }
                else
                {
                    colorsBlocks[i] = (Color){50, 50, 50, 255};
                }
            }
            searchTimer += GetFrameTime();

            if (searchTimer > 3.0f)
            {
                searchTimer = 0.0f;
                reset();
            }
            else
            {
                if (std.name != NULL)
                    DrawText("Successful!", screenWidth / 2 - MeasureText("Successful!", 30) / 2, 350, 30, GREEN);
                else
                    DrawText("Not Found!", screenWidth / 2 - MeasureText("Not Founde!", 30) / 2, 350, 30, RED);
            }
        }

        for (int i = 0; i < numOfBlocs; i++)
        {

            DrawRectangleRec(blocs[i], colorsBlocks[i]);
        }
    }

    DrawRectangleRec((Rectangle){0, 250, 25, 50}, (Color){244, 244, 244, 245});
    DrawRectangleRec((Rectangle){775, 250, 25, 50}, (Color){244, 244, 244, 245});
}