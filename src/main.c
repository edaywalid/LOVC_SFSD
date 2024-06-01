// main.c
#define RAYGUI_IMPLEMENTATION
#include "../include/abstract.h"
#include "../include/index.h"
#include "../include/raygui.h"
#include "../include/raylib.h"
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

// Font
Font customFont;
Font titleCustomFont;
Font midCustomFont;

// visualisation
student searchStudent = {0};
int blocPosition;
bool activeV = false;
int step = 0;
bool trainLeft = false;
bool trainRight = false;
bool trainCenter = false;
bool trainStop = true;

int speed = 5;
int trainTrans = 0;
int bolcksTrans = 1;
int distance = 0;

float searchTimer = 0.0f;

Texture2D resizedBackGround;

void reset() {
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

void home_page() {
  const char *text = "STUDENT MANAGING SYSTEM";
  Vector2 textSize = MeasureTextEx(titleCustomFont, text, 48, 0);

  int textX = (screenWidth - textSize.x) / 2;
  int textY = 50;

  DrawTextEx(titleCustomFont, text, (Vector2){textX, textY}, 48, 0,
             (Color){119, 119, 119, 255});

  // Button Style
  GuiSetStyle(DEFAULT, TEXT_SIZE, 18);
  int buttonWidth = MeasureText("0000000000", 18);
  int buttonX = (screenWidth - buttonWidth) / 2 - 15;

  // Insert

  if (GuiButton((Rectangle){buttonX, 150, buttonWidth + 15, 25}, "Insert")) {
    Sound clickSound = LoadSound("Sound/clickv2.wav");
    PlaySound(clickSound);
    currentPage = 1;
  }

  // Search
  if (GuiButton((Rectangle){buttonX, 190, buttonWidth + 15, 25}, "Search")) {
    Sound clickSound = LoadSound("Sound/clickv2.wav");

    PlaySound(clickSound);
    currentPage = 2;
  }

  // Delete
  if (GuiButton((Rectangle){buttonX, 230, buttonWidth + 15, 25}, "Delete")) {
    Sound clickSound = LoadSound("Sound/clickv2.wav");

    PlaySound(clickSound);
    currentPage = 3;
  }

  // View List
  if (GuiButton((Rectangle){buttonX, 270, buttonWidth + 15, 25}, "View List")) {
    Sound clickSound = LoadSound("Sound/clickv2.wav");

    PlaySound(clickSound);
    currentPage = 4;
  }
}

int insertionStatus =
    0; // 0: No action, 1: Successful, 2: Student doesn't exist
float insertionTimer = 0.0f; // Timer for displaying success or error message
const float insertionDuration = 5.0f;
// Function to draw content for Delete

void insert_message() {
  if (insertionStatus == 1) {
    DrawText("Successful!",
             screenWidth / 2 - MeasureText("Successful!", 30) / 2,
             screenHeight / 2 + 50, 30, GREEN);
  } else if (insertionStatus == 2) {
    DrawText("Invalid!", screenWidth / 2 - MeasureText("Invalid!", 30) / 2,
             screenHeight / 2 + 50, 30, RED);
  }
  // Update timer
  if (insertionStatus > 0) {
    insertionTimer += GetFrameTime();

    if (insertionTimer >= insertionDuration) {
      insertionStatus = 0; // Reset status after the desired duration
      insertionTimer = 0.0f;
    }
  }
}

// Function to draw content for Insert
void insert_page() {

  const char *text = "Insert";
  Vector2 textSize = MeasureTextEx(titleCustomFont, text, 48, 0);

  int textX = (screenWidth - textSize.x) / 2;
  int textY = 50;

  DrawTextEx(titleCustomFont, text, (Vector2){textX, textY}, 48, 0,
             (Color){119, 119, 119, 255});

  GuiSetStyle(DEFAULT, TEXT_SIZE, 18);
  int buttonWidth = MeasureText("Back", 18);
  int buttonX = (screenWidth - buttonWidth) / 2 - 15;
  if (GuiButton((Rectangle){750, 400, 30, 30}, GuiIconText(ICON_HOUSE, "")))

  {
    Sound clickSound = LoadSound("Sound/clickv2.wav");
    PlaySound(clickSound);
    currentPage = 0;
  }
  student std;
  insert_form();
  insert_message();
}

int deleteStatus = 0; // 0: No action, 1: Successful, 2: Student doesn't exist
float deleteTimer = 0.0f; // Timer for displaying success or error message
const float deleteDuration = 5.0f;
// Function to draw content for Delete

void delete_message() {
  if (deleteStatus == 1)
    DrawText("Successful!",
             screenWidth / 2 - MeasureText("Successful!", 30) / 2,
             screenHeight / 2 + 50, 30, GREEN);
  else if (deleteStatus == 2)
    DrawText("Student Doesn't Exist!",
             screenWidth / 2 - MeasureText("Student Doesn't Exist!", 30) / 2,
             screenHeight / 2 + 50, 30, RED);
  // Update timer
  if (deleteStatus > 0) {
    deleteTimer += GetFrameTime();

    if (deleteTimer >= deleteDuration) {
      deleteStatus = 0; // Reset status after the desired duration
      deleteTimer = 0.0f;
      memset(id, '\0', 10);
    }
  }
}

void delete_page() {
  const char *text = "Delete";
  Vector2 textSize = MeasureTextEx(titleCustomFont, text, 48, 0);

  int textX = (screenWidth - textSize.x) / 2;
  int textY = 50;

  DrawTextEx(titleCustomFont, text, (Vector2){textX, textY}, 48, 0,
             (Color){119, 119, 119, 255});

  GuiSetStyle(DEFAULT, TEXT_SIZE, 18);
  int buttonWidth = MeasureText("Back", 18);
  int buttonX = (screenWidth - buttonWidth) / 2 - 15;
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
void search_page() {
  const char *text = "Search";
  Vector2 textSize = MeasureTextEx(titleCustomFont, text, 48, 0);

  int textX = (screenWidth - textSize.x) / 2;
  int textY = 50;

  DrawTextEx(titleCustomFont, text, (Vector2){textX, textY}, 48, 0,
             (Color){119, 119, 119, 255});

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
void view_page() {

  GuiSetStyle(DEFAULT, TEXT_SIZE, 18);
  int buttonWidth = MeasureText("Back", 18);
  int buttonX = (screenWidth - buttonWidth) / 2 - 15;
  if (GuiButton((Rectangle){750, 400, 30, 30}, GuiIconText(ICON_HOUSE, ""))) {
    Sound clickSound = LoadSound("Sound/clickv2.wav");
    PlaySound(clickSound);
    currentPage = 0;
  }

  DrawStudentTable();
}

// Function to check if the mouse is inside a text box
bool IsMouseInsideTextBox(Rectangle textBox) {
  Vector2 mousePos = GetMousePosition();
  return (
      mousePos.x >= textBox.x && mousePos.x <= (textBox.x + textBox.width) &&
      mousePos.y >= textBox.y && mousePos.y <= (textBox.y + textBox.height));
}

void insert_form() {
  student std;
  int labelWidth = MeasureText("000000:", 20);
  int inputWidth = 150;
  int buttonWidth = 80;

  GuiSetStyle(DEFAULT, TEXT_SIZE, 18);
  GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

  // Check if mouse is inside the first text box
  if (IsMouseInsideTextBox((Rectangle){labelWidth, 120, inputWidth, 20})) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      editMode1 = true;  // Toggle edit mode for TextBox 1
      editMode2 = false; // Ensure TextBox 2 is not in edit mode
      editMode3 = false; // Ensure TextBox 2 is not in edit mode
    }
  }

  // Check if mouse is inside the second text box
  else if (IsMouseInsideTextBox((Rectangle){2 * labelWidth + 10 + inputWidth,
                                            120, inputWidth, 25})) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      editMode2 = true;  // Toggle edit mode for TextBox 2
      editMode1 = false; // Ensure TextBox 1 is not in edit mode
      editMode3 = false; // Ensure TextBox 1 is not in edit mode
    }
  }

  else if (IsMouseInsideTextBox((Rectangle){
               3 * labelWidth + 10 + 2 * inputWidth, 120, inputWidth, 25})) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      editMode3 = true;  // Toggle edit mode for TextBox 2
      editMode1 = false; // Ensure TextBox 1 is not in edit mode
      editMode2 = false; // Ensure TextBox 1 is not in edit mode
    }
  }

  else {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
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

  GuiLabel(rec_label, "Average:");
  GuiTextBox(rec_input, avrage, 10, editMode3);

  // Save button
  if (GuiButton((Rectangle){710, 120, buttonWidth, 20},
                GuiIconText(ICON_FILE_SAVE_CLASSIC, "Save")) ||
      IsKeyPressed(KEY_ENTER)) {
    Sound clickSound = LoadSound("Sound/clickv2.wav");
    PlaySound(clickSound);
    std.name = name;
    std.id = atoi(id);
    std.average = atof(avrage);
    std.LogicallyDeleted = 0;
    if (std.id && std.average) {

      int check = insert(std, &file);
      if (check) {
        Sound clickSound = LoadSound("Sound/REALSUCCES.wav");
        PlaySound(clickSound);
        insertionStatus = 1; // Set status to successful
        insertionTimer = 0.0f;
      }
      // Reset the timere
      else {
        Sound clickSound = LoadSound("Sound/wrong.wav");
        PlaySound(clickSound);
        insertionStatus = 2; // Set status to student doesn't exist
        insertionTimer = 0.0f;
      }
    }

    memset(id, '\0', 10);
    memset(name, '\0', 50);
    memset(avrage, '\0', 10);
  }
}

// Search Form
void search_form() {
  int labelWidth = MeasureText("000000:", 20);
  int inputWidth = 620;
  int buttonWidth = 80;

  GuiSetStyle(DEFAULT, TEXT_SIZE, 18);
  GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

  // Check if mouse is inside the first text box
  if (IsMouseInsideTextBox((Rectangle){labelWidth, 120, inputWidth, 20})) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      editMode4 = true; // Toggle edit mode for TextBox 1
    }
  } else {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
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
  if (GuiButton((Rectangle){710, 120, buttonWidth, 20},
                GuiIconText(ICON_LENS, "")) ||
      IsKeyPressed(KEY_ENTER)) {
    Sound clickSound = LoadSound("Sound/clickv2.wav");
    PlaySound(clickSound);
    int ID = atoi(id);

    if (ID) {
      Index index = search(ID, file);
      searchStudent =
          index.bloc == -1
              ? (student){0}
              : getStudentFromBloc(index.bloc, index.position, index.id, file);
      blocPosition = index.bloc;
      if (!activeV) {
        activeV = true;
        step = 1;
      }
    }
  }
  search_visualisation(blocPosition, searchStudent, activeV);
}

// Delete Form
void delete_form() {
  int DeleteId;
  int labelWidth = MeasureText("000000:", 20);
  int inputWidth = 620;
  int buttonWidth = 80;

  GuiSetStyle(DEFAULT, TEXT_SIZE, 18);
  GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

  // Check if mouse is inside the first text box
  if (IsMouseInsideTextBox((Rectangle){labelWidth, 120, inputWidth, 20})) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      editMode5 = true; // Toggle edit mode for TextBox 1
    }
  } else {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      editMode5 = false; // Toggle edit mode for TextBox 1
    }
  }

  // Draw text boxes
  int start = 0;
  Rectangle rec_label = {start, 120, labelWidth, 20};
  Rectangle rec_input = {start + labelWidth, 120, inputWidth, 20};
  GuiLabel(rec_label, "ID:");
  GuiTextBox(rec_input, id, 50,
             editMode5); // Corrected: Use editMode1 for TextBox

  int timer;
  // Save button
  if (GuiButton((Rectangle){710, 120, buttonWidth, 20},
                GuiIconText(ICON_CROSS, ""))) {
    Sound clickSound = LoadSound("Sound/clickv2.wav");
    PlaySound(clickSound);
    int DeleteId = atoi(id);
    if (DeleteId) {
      int check = delete (DeleteId, &file);

      if (check) {
        Sound clickSound = LoadSound("Sound/REALSUCCES.wav");
        PlaySound(clickSound);
        deleteStatus = 1;   // Set status to successful
        deleteTimer = 0.0f; // Reset the timer
      } else {
        Sound clickSound = LoadSound("Sound/wrong.wav");
        PlaySound(clickSound);
        deleteStatus = 2;   // Set status to student doesn't exist
        deleteTimer = 0.0f; // Reset the timer
      }
    }
  }
}
// View
float scrollValue = 0.0f;
float scrollSpeed;
float scrollhandelTrans = 0.0f;
int stopUpdating = 0;
void DrawStudentTable() {

  int numOfRows = 0;

  GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
  GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
  const char *text = "View List";
  Vector2 textSize = MeasureTextEx(titleCustomFont, text, 48, 0);

  int textX = (screenWidth - textSize.x) / 2;
  int textY = 50 - (int)scrollValue;

  DrawTextEx(titleCustomFont, text, (Vector2){textX, textY}, 48, 0,
             (Color){119, 119, 119, 255});

  // Table headers
  GuiLabel((Rectangle){100, 120 - (int)scrollValue, 100, 30}, "ID");
  GuiLabel((Rectangle){200, 120 - (int)scrollValue, 400, 30}, "Name");
  GuiLabel((Rectangle){600, 120 - (int)scrollValue, 100, 30}, "Average");

  int Place = 0;

  if (IsKeyDown(KEY_DOWN) && scrollhandelTrans <= screenHeight - 50) {
    scrollhandelTrans += scrollSpeed;
  } else if (IsKeyDown(KEY_UP) && scrollhandelTrans >= 0) {
    scrollhandelTrans -= scrollSpeed;
  }

  if (GetMouseWheelMove() < 0 && scrollhandelTrans <= screenHeight - 50) {
    scrollhandelTrans += scrollSpeed * 3;
  } else if (GetMouseWheelMove() > 0 && scrollhandelTrans >= 0) {
    scrollhandelTrans -= scrollSpeed * 3;
  }

  indexNode *head = loadIndexToRam(file);
  indexNode *current = head;

  while (current != NULL) {
    int i = 0;
    while (i < current->nb) {
      sprintf(id, "%d", current->indexes[i].id);
      int bloc = current->indexes[i].bloc;
      int pos = current->indexes[i].position;
      student x = getStudentFromBloc(bloc, pos, current->indexes[i].id, file);
      sprintf(avrage, "%.2f", x.average);
      GuiTextBox(
          (Rectangle){100, 120 + (Place + 1) * 40 - (int)scrollValue, 100, 30},
          id, 10, false);
      // Name
      GuiTextBox(
          (Rectangle){200, 120 + (1 + Place) * 40 - (int)scrollValue, 400, 30},
          x.name, 64, false);
      // Average
      GuiTextBox((Rectangle){600, 120 + (1 + Place++) * 40 - (int)scrollValue,
                             100, 30},
                 avrage, 5, false);
      i++;
      numOfRows++;
    }
    current = current->next;
  }

  // Draw the scrollbar
  scrollSpeed = 5.0f;
  Rectangle scrollbar = {screenWidth - 10, 0, 10, screenHeight};

  // Draw the scrollbar
  DrawRectangleRec(scrollbar, LIGHTGRAY);
  DrawRectangleLinesEx(scrollbar, 1, GRAY);

  // Draw the scrollbar handle
  Rectangle scrollbarHandle = {screenWidth - 10, scrollhandelTrans, 10, 50};
  DrawRectangleRec(scrollbarHandle, GRAY);

  scrollValue =
      (40 * ((numOfRows + 3)) / (screenHeight - 50)) * scrollhandelTrans;
  if (scrollValue < 0) {
    scrollValue = 0;
  }

  memset(id, '\0', 10);
  memset(avrage, '\0', 10);
  GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
}

void search_visualisation(int blocNum, student std, bool active) {

  int numOfBlocs = file.Header.lastBloc + 1;
  Rectangle blocs[numOfBlocs];
  Color colorsBlocks[numOfBlocs];

  int blockWidth = 150;
  int arrowWidth = 50;
  int maxlength = std.name == NULL ? numOfBlocs * (blockWidth + arrowWidth)
                                   : (blocNum) * (blockWidth + arrowWidth);

  if (step == 0) {
    reset();
    for (int i = 0; i < numOfBlocs; i++) {

      colorsBlocks[i] = (Color){119, 119, 119, 255};
    }
  }
  if (step == 1) {
    trainLeft = true;
    trainStop = false;
    step = 2;
    for (int i = 0; i < numOfBlocs; i++) {

      colorsBlocks[i] = (Color){119, 119, 119, 255};
    }
  }

  if (activeV) {
    Vector2 point1 = {screenWidth / 2 + trainTrans, screenHeight / 2};
    Vector2 point2 = {screenWidth / 2 - 25 + trainTrans, screenHeight / 2 - 50};
    Vector2 point3 = {screenWidth / 2 + 25 + trainTrans, screenHeight / 2 - 50};
    DrawTriangle(point3, point2, point1, (Color){162, 29, 29, 255});

    if (step == 2) {
      if (trainLeft) {
        if (point1.x == (screenWidth - blockWidth) / 2) {
          trainRight = true;
          trainLeft = false;
        }
        trainTrans -= speed;
      }
      if (trainRight) {
        if (point1.x == (screenWidth + blockWidth) / 2) {
          trainCenter = true;
          trainRight = false;
        }
        trainTrans += speed;
      }
      if (trainCenter) {
        if (point1.x == (screenWidth) / 2) {
          trainStop = true;
          trainCenter = false;
        }
        trainTrans -= speed;
      }
      if (trainStop) {
        step = 3;
      }

      for (int i = 0; i < numOfBlocs; i++) {
        if (distance / (arrowWidth + blockWidth) == i) {
          colorsBlocks[i] = (Color){255, 218, 93, 255};
          continue;
        } else if (distance / (arrowWidth + blockWidth) > i) {
          colorsBlocks[i] = (Color){162, 29, 29, 255};
        } else {
          colorsBlocks[i] = (Color){119, 119, 119, 255};
        }
      }
    }
    for (int i = 0; i < numOfBlocs; i++) {

      if (i != numOfBlocs - 1) {
        Rectangle bodyRect = {(screenWidth - blockWidth) / 2 +
                                  i * (blockWidth + arrowWidth) + blockWidth +
                                  bolcksTrans,
                              283, 40, 4};
        DrawRectangleRec(bodyRect, (Color){119, 119, 119, 255});
        Vector2 point1 = {(screenWidth - blockWidth) / 2 +
                              i * (blockWidth + arrowWidth) + blockWidth + 40 +
                              bolcksTrans,
                          280};
        Vector2 point2 = {(screenWidth - blockWidth) / 2 +
                              i * (blockWidth + arrowWidth) + blockWidth + 40 +
                              bolcksTrans,
                          290};
        Vector2 point3 = {(screenWidth - blockWidth) / 2 +
                              i * (blockWidth + arrowWidth) + blockWidth + 50 +
                              bolcksTrans,
                          285};
        DrawTriangle(point1, point2, point3, (Color){119, 119, 119, 255});
      }

      blocs[i] = (Rectangle){(screenWidth - blockWidth) / 2 +
                                 i * (blockWidth + arrowWidth) + bolcksTrans,
                             250, blockWidth, 70};
    }

    if (step == 3) {
      for (int i = 0; i < numOfBlocs; i++) {
        if (distance / (arrowWidth + blockWidth) >= i) {
          colorsBlocks[i] = (Color){162, 29, 29, 255};
        } else {
          colorsBlocks[i] = (Color){119, 119, 119, 255};
        }
      }
      if ((int)distance % 200 == 0 && distance > 0) {

        if (distance == maxlength) {
          step = 4;
          if (std.name != NULL && blocNum > 0) {
            trainLeft = true;
            trainStop = false;
          } else {
            trainLeft = false;
            trainStop = true;
          }
        } else {
          trainLeft = true;
          trainStop = false;
          step = 1;
        }
        bolcksTrans -= speed;
        distance += speed;
      } else if (!maxlength) {
        step = 4;
        trainLeft = false;
        trainStop = true;
      } else {
        bolcksTrans -= speed;
        distance += speed;
      }
    }

    if (step == 4) {
      for (int i = 0; i < numOfBlocs; i++) {
        if (distance / (arrowWidth + blockWidth) == i) {
          colorsBlocks[i] = (Color){255, 218, 93, 255};
          continue;
        } else if (distance / (arrowWidth + blockWidth) > i) {
          colorsBlocks[i] = (Color){162, 29, 29, 255};
        } else {
          colorsBlocks[i] = (Color){119, 119, 119, 255};
        }
      }

      if (std.name != NULL && blocNum > 0) {

        if (trainLeft) {
          if (point1.x == (screenWidth - blockWidth) / 2) {
            trainRight = true;
            trainLeft = false;
          }
          trainTrans -= speed;
        }
        if (trainRight) {
          if (point1.x == (screenWidth + blockWidth) / 2) {
            trainCenter = true;
            trainRight = false;
          }
          trainTrans += speed;
        }
        if (trainCenter) {
          if (point1.x == (screenWidth) / 2) {
            trainStop = true;

            trainCenter = false;
          }
          trainTrans -= speed;
        }
        if (trainStop) {
          trainTrans = speed;
        }
      }
      if (trainStop)
        step = 5;
    }

    if (step == 5) {

      for (int i = 0; i < numOfBlocs; i++) {
        if (distance / (arrowWidth + blockWidth) == i) {
          colorsBlocks[i] = (Color){80, 208, 80, 255};
        } else if (distance / (arrowWidth + blockWidth) > i) {
          colorsBlocks[i] = (Color){162, 29, 29, 255};
        } else {
          colorsBlocks[i] = (Color){119, 119, 119, 255};
        }
      }
      searchTimer += GetFrameTime();

      if (searchTimer > 3.0f) {
        memset(id, '\0', 10);
        memset(avrage, '\0', 10);
        memset(name, '\0', 10);
        searchTimer = 0.0f;
        reset();
      } else {
        if (std.name != NULL) {

          if (searchTimer > 1.0f) {
            sprintf(avrage, "%.2f", std.average);
            sprintf(id, "%d", std.id);

            DrawTextEx(
                midCustomFont, std.name,
                (Vector2){(screenWidth - MeasureText(std.name, 36)) / 2, 350},
                36, 0, (Color){80, 208, 80, 255});
            DrawTextEx(midCustomFont, "Avrage: ",
                       (Vector2){screenWidth / 2 -
                                     (25 + MeasureText("Avrage: ", 36) / 2),
                                 380},
                       36, 0, (Color){80, 208, 80, 255});
            DrawTextEx(midCustomFont, avrage,
                       (Vector2){screenWidth / 2 + 25, 380}, 36, 0,
                       (Color){80, 208, 80, 255});
          } else {
            DrawTextEx(titleCustomFont, "Successful!",
                       (Vector2){screenWidth / 2 -
                                     MeasureText("Successful!", 48) / 2.7,
                                 360},
                       48, 0, (Color){80, 208, 80, 255});
          }
        } else
          DrawTextEx(
              titleCustomFont, "Not Found!",
              (Vector2){screenWidth / 2 - MeasureText("Not Found!", 48) / 2.7,
                        360},
              48, 0, (Color){162, 29, 29, 255});
      }
    }

    for (int i = 0; i < numOfBlocs; i++) {
      DrawRectangleRounded(blocs[i], 0.05, 10, colorsBlocks[i]);
    }
  }

  Rectangle rect1 = {0, 250, 25, 70};
  Rectangle srcRec1 = {rect1.x, rect1.y, rect1.width, rect1.height};
  Rectangle rect2 = {775, 250, 25, 70};
  Rectangle srcRec2 = {rect2.x, rect2.y, rect2.width, rect2.height};

  DrawTextureRec(resizedBackGround, srcRec1, (Vector2){rect1.x, rect1.y},
                 WHITE);
  DrawTextureRec(resizedBackGround, srcRec2, (Vector2){rect2.x, rect2.y},
                 WHITE);
}

int main() {

  openFile(&file);

  student s1 = {"Walid", 1, 17.5, 0};
  insert(s1, &file);
  s1 = (student){"Moh", 33, 15, 0};
  insert(s1, &file);
  s1 = (student){"Abderraouf", 44, 17, 0};
  insert(s1, &file);
  s1 = (student){"Abderrahmane", 4, 12, 0};
  insert(s1, &file);
  s1 = (student){"Tarek", 43, 13, 0};
  insert(s1, &file);
  s1 = (student){"Hocine", 22, 9, 0};
  insert(s1, &file);
  s1 = (student){"Adel", 23, 2, 0};
  insert(s1, &file);
  s1 = (student){"Rayan", 66, 13, 0};
  insert(s1, &file);
  s1 = (student){"Wail", 12, 20, 0};
  insert(s1, &file);
  s1 = (student){"Aymen", 99, 19, 0};
  insert(s1, &file);
  s1 = (student){"Imad", 234, 1, 0};
  insert(s1, &file);
  s1 = (student){"Youcef", 989, 0, 0};
  insert(s1, &file);

  memset(name, '\0', 50);

  InitWindow(screenWidth, screenHeight, "LOVC");

  SetTargetFPS(60);

  InitAudioDevice();

  // Load a custom font (you can replace this with your font file)
  customFont = LoadFontEx("fonts/poppins/Poppins-Bold.ttf", 18, 0, 250);
  titleCustomFont = LoadFontEx("fonts/poppins/Poppins-Bold.ttf", 48, 0, 250);
  midCustomFont = LoadFontEx("fonts/poppins/Poppins-Bold.ttf", 36, 0, 250);

  // Set the custom font
  GuiSetFont(customFont);
  Image icon = LoadImage("backGround/icon3.png");

  ImageResize(&icon, 50, 50);

  SetWindowIcon(icon);

  Image backGround = LoadImage("BackGround/images.png");

  ImageResize(&backGround, 800, 450);

  resizedBackGround = LoadTextureFromImage(backGround);

  UnloadImage(backGround);

  while (!WindowShouldClose()) {

    // Initialization

    // Main game loop
    // Update

    // Draw

    BeginDrawing();
    DrawTexture(resizedBackGround, 0, 0, WHITE);

    // Draw content based on the current page
    switch (currentPage) {
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
  closeFile(&file);
  CloseWindow();

  return 0;
}
