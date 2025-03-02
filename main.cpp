#include <iostream>
#include <fstream>
#include <windows.h> // for colors
#include <cstdlib>   // for clearing screen
#include <conio.h>   //for getch();
#include <ctime>
#include <iomanip>
using namespace std;

// Global Constant
const int BLACK = 0;
const int BLUE = 1;
const int GREEN = 2;
const int CYAN = 3;
const int RED = 4;
const int MAGENTA = 5;
const int YELLOW = 6;
const int WHITE = 7;

// Global Variable
int count_card[2];      // no of cards each player have
int PlayerHand[2][108]; // col*10000  + color*1000 + row*100 + type

// Student 1 //
void initializeDeck(int deck[][15]);
void dealCards(int desk[][15]);
void handleSpecialCard(int deck[][15], int specialCardType, int &topCard, int playerCard, int &current_player);
void PrintBoard(int topCard, int current_player);
void saveLoadGameResult(int player1, int player2);

// student 2 //
void shuffleDeck(int deck[][15]);
bool isValidPlay(int playerCard, int topCard);
void playTurn(int deck[][15], int &topCard, int playerCard, int current_player);
void UpdateDiscardPile(int deck[][15], int topCard);
bool callUno();

// display & colors
void clearScreen();
void setColor(int textColor, int bgColor);
void print_cards();

// display
void display_menu(bool card_drawn, int current_player);
int Exit_menu_and_conversion_of_string_input_to_int(int second_player);

// PlayerHand[2][108]
void initializing_no_card_in_PlayerHand();
void adding_card_of_row_col_from_deck_to_PlayerHand(int deck[][15], int row, int col, int player);
void removing_playerCard_from_PlayerHand(int playerCard, int current_player);

// changing deck[8][15]
void Take_1st_Top_card_from_deck(int deck[][15], int &topCard);                    // taking random card from deck
void Penealty_of_card(int deck[][15], int cards, int current_player, int topCard); // how many card do play have to take from deck

// Game option
void start_game(int desk[][15]);
void switch_player(int &current_player);                                          // next player turn
void wild_card(int deck[][15], int &topCard, int playerCard, int current_player); // for the option of wild card ( chose color )
void Draw_Card(int deck[][15], int current_player);                               // for the option to draw a card from deck
void winner();                                                                    // to check if some one has no card left also for saving result

// topCard , playerCard
int color_(int num);
int type_(int num);
int coloumn_(int num);
int row_(int num);

// PlayerHand[][]
int color_arr(int i, int current_player);
int type_arr(int i, int current_player);

// filling
void saveLoadGameResult(int player1, int player2);                       // saving results of game in the file
void display_result_of_file();                                           // display result from file
void storing_previous_winnigs_to_player_1_2(int &player1, int &player2); // storing player 1,2 wining in numbers file
void storing_winner(int win);                                            // increasing number of win of the winner

// for checking
void n_print(int deck[][15]);
void n_print(int topCard);

//////////
// Main //
//////////
int main()
{
    srand(time(0));

    char option;

    cout << "\n Main Menu :\n"
         << " 1. To Start Game.\n"
         << " 2. To See Result.\n"
         << " E. To Exit.\n\n"
         << " Enter Option: ";
    option = getch();
    if (option == '1') // 1
    {

        int deck[8][15];

        initializeDeck(deck); // value is given to deck_card (0 to 14) in order

        shuffleDeck(deck); // desk shuffled

        count_card[0] = 0; // nulling every thing
        count_card[1] = 0;

        initializing_no_card_in_PlayerHand(); // both players has no card ( all -1 )

        dealCards(deck); // both player have been given 7 cards

        start_game(deck);
    }
    else if (option == '2') // 2
    {
        clearScreen();
        cout << "\n";
        cout << "-----------------------------\n";
        display_result_of_file();
        cout << "-----------------------------\n\n";
        cout << " Press Any Button to goto Main Menu\n ";
        getch();
        clearScreen();
    }
    else if (option == 'E' || option == 'e') // exit
    {
        char yn;
        cout << "\n Do you want to exit program ( Y for yes / any other button for no ) : ";
        yn = getch();

        if (yn == 'Y' || yn == 'y')
        {
            cout << "\n\n ---PROGRAM EXITED---\n\n";
            exit(0);
        }
        else
        {
            clearScreen();
            main();
        }
    }
    else
    {
        clearScreen();
        cout << " --!! INVALID INPUT !!--\n";
        main();
    }
    main();
    return 0;
}
void start_game(int deck[][15])
{
    clearScreen();

    int topCard = -1;
    Take_1st_Top_card_from_deck(deck, topCard); // one extra random top card from deck

    int playerCard = -1;

    int current_player = 1;

    int option = -1;
    int penealty_player = -1;
    int skiped_player = -1;

    bool card_drawn = false;
    bool said_uno = false;

    int U_current_player = -1;
    int U_second_player = -1;

    while (true)
    {
        card_drawn = false; // if player has drawn a card
    restart_drawn_game:

        // clearScreen();

        int second_player;
        if (current_player == 1)
            second_player = 2;
        else if (current_player == 2)
            second_player = 1;

        bool uno;

        if ( card_drawn==false )
        {
            if (option != -1)
                uno = callUno();

            clearScreen();

            if (uno)
            {
                if (count_card[U_second_player - 1] == 1 && said_uno == false) // if second players calls uno
                {
                    cout << " ---- Player " << U_second_player << " has +2 Penealty ----\n";
                    current_player = U_second_player;
                    Penealty_of_card(deck, 2, current_player, topCard);
                }
                else if (count_card[U_current_player - 1] == 1) // if player calls uno on time
                {
                    cout << " ---- You Have Said UNO On Time So No Penealty ----\n";
                    said_uno = true;
                }
                else // if wrong uno call
                {
                    cout << " ---- Player " << U_current_player << " has +2 Penealty due to wrong call----\n";
                    current_player = U_current_player;
                    Penealty_of_card(deck, 2, current_player, topCard);
                    said_uno = false;
                }
            }
            else
                said_uno = false;
        }
        else
            clearScreen();

        winner();

        UpdateDiscardPile(deck, topCard);

        PrintBoard(topCard, current_player); // print whole cards

        penealty_player = -1;

        display_menu(card_drawn, current_player); // menu play,draw,call,uno

        option = -1;

        U_current_player = current_player;
        U_second_player = second_player;

    reoption:

        option = Exit_menu_and_conversion_of_string_input_to_int(second_player);

        if (option == -1) //  if no is entered after E
            goto restart_drawn_game;

        if (option < 0 || option > count_card[current_player - 1]) // validation (0 to n)
        {
            cout << " --!! Invalid Input type 2 !!-- \n Again";
            goto reoption;
        }

        if (option == 0) // Draw card , Skip Card
        {
            if (card_drawn) // if player has already drawn card ( it will skip )
            {
                switch_player(current_player);
                continue;
            }
            else // if player draw card first time
            {
                Draw_Card(deck, current_player); // for taking a new card from deck
                card_drawn = true;
                goto restart_drawn_game;
            }
        }

        playerCard = PlayerHand[current_player - 1][option - 1]; // card selected from the player

        if (isValidPlay(playerCard, topCard) == false && type_(topCard) != 13 && type_(topCard) != 14)
        {
            cout << " --!! This can not be played !!-- \n Again";
            goto reoption;
        }

        if (1 <= option && option <= count_card[current_player - 1]) // if cards are selected
        {

            if (10 <= type_(playerCard) && type_(playerCard) <= 14) // if special card (+2,S,R,W,+4W)is selected
            {
                handleSpecialCard(deck, type_(playerCard), topCard, playerCard, current_player);
                if (type_(playerCard) == 10)
                {
                    Penealty_of_card(deck, 2, second_player, topCard); // 2 card from deck to oponent
                }
                else if (type_(playerCard) == 14)
                {
                    Penealty_of_card(deck, 4, second_player, topCard); // 4 card from deck to oponent
                }
                continue;
            }
            else if (0 <= type_(playerCard) && type_(playerCard) <= 9) // if numbered card (0-9) are selected
            {
                playTurn(deck, topCard, playerCard, current_player); // reduce selected card from player hand
                switch_player(current_player);                       // next player
                continue;
            }
        }
    }
}
int Exit_menu_and_conversion_of_string_input_to_int(int second_player)
{
    int num = 0;
    string st;

again:
    cout << " Enter Card No : ";
    getline(cin, st);

    if (st == "E" || st == "e") // E  e
    {
        cout << "\n Do you want to goto the Main Menu (Y/N) : ";
        char yn;
        yn = getch();

        while (yn != 'Y' && yn != 'N' && yn != 'y' && yn != 'n')
        {
            cout << "\n --!! Ivalid Input !!--\n Again Enter (Y/N) : ";
            yn = getch();
        }

        if (yn == 'Y' || yn == 'y')
        {
            clearScreen();
            cout << "\n         -----> Player "<<second_player<<" is the winner, As the other player exit the Game <-----\n\n";
            storing_winner(second_player);
            main(); // goto main menu
        }
        else if (yn == 'N' || yn == 'n')
        {
            return -1; // if he want to continue the game
        }
    }

    if (st[0] == '\0')
    {
        cout << " --!! Nothing is Entered !!--\n Again";
        goto again;
    }

    for (int i = 0; st[i] != '\0'; i++) // string to int
    {

        if ('0' <= st[i] && st[i] <= '9')
        {
            num = 10 * num + (st[i] - '0');
        }
        else
        {
            cout << " --!! Invalid Input type 1 !!--\n Again";
            goto again;
        }
    }
    return num;
}
void initializeDeck(int deck[][15]) // deck in order
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if ((i % 2 == 0) || (1 <= j && j <= 12))
            {
                deck[i][j] = j;
            }
            else
            {
                deck[i][j] = -1; // -1 means empty
            }
        }
    }
}

void shuffleDeck(int deck[][15]) // random suffle
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (deck[i][j] != -1)
            {
                int rand_col = -1;
                do
                {
                    rand_col = rand() % 15; // random column in a row for swapping
                } while (deck[i][rand_col] == -1);

                // swapping
                int temp = deck[i][j];
                deck[i][j] = deck[i][rand_col];
                deck[i][rand_col] = temp;
            }
        }
    }
}

void initializing_no_card_in_PlayerHand() // player cards are empty
{
    for (int i = 0; i < 108; i++)
    {
        PlayerHand[0][i] = -1;
        PlayerHand[1][i] = -1;
    }
}
void dealCards(int deck[][15])
{
    for (int count = 0; count < 7; count++) // 7 cards given
    {
        for (int player = 0; player < 2; player++) // two players
        {
            int rand_row = -1;
            int rand_col = -1;
            do
            {
                rand_row = rand() % 8;
                rand_col = rand() % 15;
            } while (deck[rand_row][rand_col] == -1 ||
                     (50 <= deck[rand_row][rand_col] && deck[rand_row][rand_col] <= 64) ||
                     (-64 <= deck[rand_row][rand_col] && deck[rand_row][rand_col] <= -50));

            adding_card_of_row_col_from_deck_to_PlayerHand(deck, rand_row, rand_col, player);
        }
    }
}
void adding_card_of_row_col_from_deck_to_PlayerHand(int deck[][15], int row, int col, int player)
{
    PlayerHand[player][count_card[player]] = deck[row][col]; // type
    PlayerHand[player][count_card[player]] += row * 100;     // row
    PlayerHand[player][count_card[player]] += col * 10000;   // coloumn

    if (deck[row][col] == 13 || deck[row][col] == 14) // color
        PlayerHand[player][count_card[player]] += BLACK * 1000;
    else
    {
        if (row == 0 || row == 1)
            PlayerHand[player][count_card[player]] += RED * 1000;
        else if (row == 2 || row == 3)
            PlayerHand[player][count_card[player]] += GREEN * 1000;
        else if (row == 4 || row == 5)
            PlayerHand[player][count_card[player]] += BLUE * 1000;
        else if (row == 6 || row == 7)
            PlayerHand[player][count_card[player]] += YELLOW * 1000;
        else
            cout << " error in adding card";
    }

    deck[row][col] += 50;
    count_card[player] += 1;
}
void Take_1st_Top_card_from_deck(int deck[][15], int &topCard) // another random card from deck
{
    int rand_row = -1;
    int rand_col = -1;
    do
    {
        rand_row = rand() % 8;
        rand_col = rand() % 15;
    } while (deck[rand_row][rand_col] == -1 ||
             (10 <= deck[rand_row][rand_col] && deck[rand_row][rand_col] <= 14) ||
             (50 <= deck[rand_row][rand_col] && deck[rand_row][rand_col] <= 64));

    topCard = deck[rand_row][rand_col]; // (0-14) 0ne tens
    topCard += rand_row * 100;          // (0-8) hundred
    topCard += rand_col * 10000;        // (0-14) hendred tousand,ten tousand

    if (deck[rand_row][rand_col] == 13 || deck[rand_row][rand_col] == 14)
        topCard += BLACK * 1000;
    else if (rand_row == 0 || rand_row == 1) //(b=0,r=4,y=6,b=1,g=2) * tousand
        topCard += RED * 1000;
    else if (rand_row == 2 || rand_row == 3)
        topCard += GREEN * 1000;
    else if (rand_row == 5 || rand_row == 4)
        topCard += BLUE * 1000;
    else if (rand_row == 6 || rand_row == 7)
        topCard += YELLOW * 1000;
    else
        cout << "\n ERROR in Take_Top_card_from_deck \n";

    deck[rand_row][rand_col] += 50; // ( -1 ) emptying the deck
    deck[rand_row][rand_col] *= -1;
}
void Penealty_of_card(int deck[][15], int cards, int current_player, int topCard) // taking random card
{

    for (int i = 0; i < cards; i++)
    {
        UpdateDiscardPile(deck, topCard);

        int rand_row = -1;
        int rand_col = -1;
        do
        {
            rand_row = rand() % 8;
            rand_col = rand() % 15;
        } while (deck[rand_row][rand_col] == -1 ||
                 (50 <= deck[rand_row][rand_col] && deck[rand_row][rand_col] <= 64) ||
                 (-64 <= deck[rand_row][rand_col] && deck[rand_row][rand_col] <= -50));

        adding_card_of_row_col_from_deck_to_PlayerHand(deck, rand_row, rand_col, current_player - 1);
    }
}
void UpdateDiscardPile(int deck[][15], int topCard)
{
    for (int i = 0; i < 8; i++) // checking if deck is empty
    {
        for (int j = 0; j < 15; j++)
        {
            if (0 <= deck[i][j] && deck[i][j] <= 14)
                return;
        }
    }

    bool suffle = false;
    for (int i = 0; i < 8; i++) // checking if discard pile is empty
    {
        for (int j = 0; j < 15; j++)
        {
            if (-64 <= deck[i][j] && deck[i][j] <= -50 && row_(topCard) != i && coloumn_(topCard) != j)
            {
                suffle = true;
                cout << "\n -----DISCARD PILE SHUFFLED-----\n ";
            }
        }
    }

    if (suffle == false)
    {
        cout << "\n-----No Card In Discard Pile is Left To Be Suffled-----\n";
        main();
    }

    for (int i = 0; i < 8; i++) //   suffling discard pile in deck
    {
        for (int j = 0; j < 15; j++)
        {
            if (-64 <= deck[i][j] && deck[i][j] <= -50)
            {
                if (row_(topCard) != i && coloumn_(topCard) != j)
                {
                    deck[i][j] *= -1;
                    deck[i][j] -= 50;
                }
            }
        }
    }
}
void clearScreen()
{
    system("CLS");
    // cout << " \n-------cleared------\n";
}
void setColor(int textColor, int bgColor)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
}
void print_cards()
{

    setColor(BLACK, BLUE);
    cout << " -------" << endl;
    cout << " -     -" << endl;
    cout << " -  5  -" << endl;
    cout << " -     -" << endl;
    cout << " -------" << endl;
}

void n_print(int deck[][15]) // printing block of deck
{
    cout << "\n Deck \n";
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (0 <= deck[i][j] && deck[i][j] <= 14)
                setColor(BLUE, BLACK);
            else if (deck[i][j] == -1)
                setColor(YELLOW, BLACK);
            else if (-64 <= deck[i][j] && deck[i][j] <= -50)
                setColor(RED, BLACK);
            else if (50 <= deck[i][j] && deck[i][j] <= 64)
                setColor(GREEN, BLACK);

            cout << setw(4) << deck[i][j];
            setColor(WHITE, BLACK);
        }
        cout << endl;
    }
    cout << endl;
}
void n_print(int topCard)
{
    cout << " ----------> count 1 = " << count_card[0] << endl;
    cout << " ----------> count 2 = " << count_card[1] << endl;

    // cout << " -------> counter " << current_player << " = " << count_card[current_player - 1] << endl;

    for (int i = 0; i < count_card[0] + 1; i++)
    {
        cout << setw(10) << PlayerHand[0][i];
        if (i % 10 == 0)
            cout << endl;
    }
    cout << endl;
    for (int i = 0; i < count_card[1] + 1; i++)
    {
        cout << setw(10) << PlayerHand[1][i];
        if (i % 10 == 0)
            cout << endl;
    }
    cout << "\n -------> Top Card = " << topCard << endl;
    //      << endl;
}
int color_(int num)
{
    return num / 1000 % 10;
}
int type_(int num)
{
    return num % 100;
}
int row_(int num)
{
    return num / 100 % 10;
}
int coloumn_(int num)
{
    return num / 10000 % 100;
}
int color_arr(int i, int current_player)
{
    return PlayerHand[current_player - 1][i] / 1000 % 10;
}
int type_arr(int i, int current_player)
{
    return PlayerHand[current_player - 1][i] % 100;
}

void show_horizontal_card(int current_player)
{
    string card_symbol[] = {" 0 ", " 1 ", " 2 ", " 3 ", " 4 ", " 5 ", " 6 ", " 7 ", " 8 ", " 9 ", "+2 ", " S ", " R ", " W ", "+4W", "   "};

    string card_common_design[] = {
        " ------- ",
        " -     - ",
        " - ",
        " -     - ",
        " ------- "};

    int count = count_card[current_player - 1];
    int card_in_row, current_card = 0;

    while (count > 0)
    {
        if (count > 10)
            card_in_row = 10;
        else
            card_in_row = count;

        for (int i = 0; i < 5; i++) // 5 lines
        {
            for (int j = 0; j < card_in_row; j++) // no of card
            {
                if (i != 2)
                {
                    setColor(WHITE, color_arr(current_card + j, current_player));
                    cout << card_common_design[i];
                    setColor(WHITE, BLACK);
                    cout << " ";
                }
                else
                {
                    setColor(WHITE, color_arr(current_card + j, current_player));
                    cout << " - " << card_symbol[type_arr(current_card + j, current_player)] << " - ";
                    setColor(WHITE, BLACK);
                    cout << " ";
                }
            }
            cout << endl;
        }

        for (int j = 1; j <= card_in_row; j++) // card number
        {
            if (j == 1)
                cout << setw(5) << (current_card + j);
            else
                cout << setw(10) << (current_card + j);
        }
        cout << endl;

        count -= card_in_row;
        current_card += card_in_row;
    }
}

void show_face_down_card(int player)
{
    int count = count_card[player - 1];

    string uno_card[] = {
        " ------- ",
        " -     - ",
        " - UNO - ",
        " -     - ",
        " ------- "};

    int card_in_row;
    while (count > 0)
    {
        if (count > 10)
            card_in_row = 10;
        else
            card_in_row = count;

        for (int i = 0; i < 5; i++) // 5 lines
        {
            for (int j = 0; j < card_in_row; j++) // no of cards
            {
                setColor(WHITE, MAGENTA);
                cout << uno_card[i];
                setColor(WHITE, BLACK);
                cout << " ";
            }
            cout << endl;
        }
        cout << endl;
        count -= card_in_row;
    }
}
void show_deck_topCard(int topCard)
{
    string card_symbol[] = {" 0 ", " 1 ", " 2 ", " 3 ", " 4 ", " 5 ", " 6 ", " 7 ", " 8 ", " 9 ", "+2 ", " S ", " R ", " W ", "+4W", "   "};

    string deck_card[] = {" -------- ",
                          " -      - ",
                          " - DECK - ",
                          " -      - ",
                          " -------- "};

    string card_lines[] = {" ------- ",
                           " -     - ",
                           " - ",
                           " -     - ",
                           " ------- "};

    for (int i = 0; i < 5; i++)
    {

        setColor(WHITE, MAGENTA);
        cout << deck_card[i];
        setColor(WHITE, BLACK);
        cout << setw(2) << "";
        if (i != 2)
        {
            setColor(WHITE, color_(topCard));
            cout << card_lines[i];
            setColor(WHITE, BLACK);
            cout << endl;
        }
        else if (i == 2) // between line
        {
            setColor(WHITE, color_(topCard));
            cout << " - " << card_symbol[type_(topCard)] << " - ";
            setColor(WHITE, BLACK);
            cout << endl;
        }
    }
}
void PrintBoard(int topCard, int current_player)
{
    if (current_player == 1)
    {
        cout << "\n Player 2 card :\n";
        show_face_down_card(2);

        cout << "\n\n";

        show_deck_topCard(topCard);

        cout << "\n\n";

        cout << " Player 1 card :\n";
        show_horizontal_card(current_player);
    }
    else if (current_player == 2)
    {
        cout << "\n Player 2 card :\n";
        show_horizontal_card(current_player);

        cout << "\n\n";

        show_deck_topCard(topCard);

        cout << "\n\n";

        cout << " Player 1 card :\n";
        show_face_down_card(1);
    }
}

bool isValidPlay(int playerCard, int topCard) // same type or same card
{
    if (type_(playerCard) == 14 || type_(playerCard) == 13 || (type_(playerCard) == type_(topCard)))
    {
        return true;
    }
    else if (color_(playerCard) == color_(topCard))
    {
        return true;
    }
    return false;
}
void display_menu(bool card_drawn, int current_player) // option menu
{
    cout << "\n\n (Player " << current_player << " turn)\n";
    cout << " Enter : \n";

    if (card_drawn == true) //   Skip or Draw
        cout << "       0. To Skip Play.\n";
    else
        cout << "       0. To Draw Card.\n";

    if (count_card[current_player - 1] != 1) //  1  to   last option card
        cout << "      (1-" << count_card[current_player - 1] << "). To Play Card.\n";
    else if (count_card[current_player - 1] == 1)
        cout << "       1. To Play Card.\n";

    cout << "       E. To Exit.\n";
}
void removing_playerCard_from_PlayerHand(int playerCard, int current_player)
{
    int playercard_no = -1;
    for (int i = 0; i < count_card[current_player - 1]; i++)
    {
        if (PlayerHand[current_player - 1][i] == playerCard)
        {
            playercard_no = i;
            break;
        }
    }

    PlayerHand[current_player - 1][playercard_no] = -1;

    count_card[current_player - 1] -= 1;

    for (int i = playercard_no; i < count_card[current_player - 1]; i++)
    {

        PlayerHand[current_player - 1][i] = PlayerHand[current_player - 1][i + 1];
    }
    PlayerHand[current_player - 1][count_card[current_player - 1]] = -1;
}
void playTurn(int deck[][15], int &topCard, int playerCard, int current_player) //  shift_top card    change curent player 1-14
{
    deck[row_(playerCard)][coloumn_(playerCard)] *= -1;
    topCard = playerCard;                                            //  topcard will change to selected card
    removing_playerCard_from_PlayerHand(playerCard, current_player); // removing this selected card from playerhand
}
void wild_card(int deck[][15], int &topCard, int playerCard, int current_player) // select color
{
    int option, color;
    cout << endl;
    while (true)
    {
        cout << " Enter color ( ";
        setColor(WHITE, RED);
        cout << " 1 ";
        setColor(WHITE, BLACK);
        cout << ",";
        setColor(WHITE, GREEN);
        cout << " 2 ";
        setColor(WHITE, BLACK);
        cout << ",";
        setColor(WHITE, BLUE);
        cout << " 3 ";
        setColor(WHITE, BLACK);
        cout << ",";
        setColor(WHITE, YELLOW);
        cout << " 4 ";
        setColor(WHITE, BLACK);
        cout << " ) : ";
        cin >> option;

        if (option < 1 || option > 4 || cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << " --!! Invalid Input !!-- \n Again";
            continue;
        }

        if (option == 1)
        {
            color = RED;
        }
        else if (option == 2)
        {
            color = GREEN;
        }
        else if (option == 3)
        {
            color = BLUE;
        }
        else if (option == 4)
        {
            color = YELLOW;
        }
        else
        {
            cout << " Error in wild ";
        }
        removing_playerCard_from_PlayerHand(playerCard, current_player);

        deck[row_(playerCard)][coloumn_(playerCard)] *= -1;

        topCard = 15; // blank coloured card

        topCard += color * 1000;
        return;
    }
}
void handleSpecialCard(int deck[][15], int specialCardType, int &topCard, int playerCard, int &current_player)
{
    if (specialCardType == 10) // +2
    {
        playTurn(deck, topCard, playerCard, current_player); // top=player  , -1 player
    }
    else if (specialCardType == 11 || specialCardType == 12) // S or R
    {
        playTurn(deck, topCard, playerCard, current_player); // top=player  , -1 player
    }
    else if (specialCardType == 13) // W
    {
        wild_card(deck, topCard, playerCard, current_player); // select color
        switch_player(current_player);
    }
    else if (specialCardType == 14) //+4W
    {
        wild_card(deck, topCard, playerCard, current_player); // select color
    }
}
void Draw_Card(int deck[][15], int current_player) // taking a random card
{
    int rand_row = -1;
    int rand_col = -1;
    do
    {
        rand_row = rand() % 8;
        rand_col = rand() % 15;
    } while (deck[rand_row][rand_col] == -1 ||
             (50 <= deck[rand_row][rand_col] && deck[rand_row][rand_col] <= 64) ||
             (-64 <= deck[rand_row][rand_col] && deck[rand_row][rand_col] <= -50));

    adding_card_of_row_col_from_deck_to_PlayerHand(deck, rand_row, rand_col, current_player - 1);
}
void switch_player(int &current_player)
{
    if (current_player == 1)
        current_player = 2;
    else if (current_player == 2)
        current_player = 1;
}

bool callUno() // if he has second last card
{
    char option;
    cout << " Enter ( U to say UNO / S to skip ) : ";
    option = getch();

    while (option != 'U' && option != 'u' && option != 'S' && option != 's')
    {
        cout << "\n --!! INVALID INPUT !!--\n Again Enter (U/S) : ";
        option = getch();
    }

    if (option == 'U' || option == 'u')
    {
        return true;
    }
    else if (option == 'S' || option == 's')
    {
        return false;
    }
    return false;
}
void winner()
{
    if (count_card[0] == 0)
    {
        cout << "\n         -----> Player 1 is the winner <-----\n\n";
        storing_winner(1);
        main();
    }
    else if (count_card[1] == 0)
    {
        cout << "\n         -----> Player 2 is the winner <-----\n\n";
        storing_winner(2);
        main();
    }
}

void display_result_of_file()
{
    ifstream in("result.txt"); // opening file for reading
    string line;

    if (!in)
    {
        cout << "Player 1 wins = 0" << endl;
        cout << "Player 2 wins = 0" << endl;
        return;
    }

    while (getline(in, line)) // 1st,2nd line
    {
        cout << line << endl;
    }
    in.close(); // closing file
}

void saveLoadGameResult(int player1, int player2)
{
    ofstream out("result.txt"); // opening file for writing
    if (out)
    {
        out << "Player 1 wins = " << player1 << endl;
        out << "Player 2 wins = " << player2 << endl;
    }
    else
    {
        cout << "Error" << endl;
    }
    out.close(); //  closing file
}



void storing_previous_winnigs_to_player_1_2(int &player1, int &player2)
{
    ifstream in("result.txt");
    if (!in) // if file is not saved
    {
        cout << " New txt file made" << endl;
        return;
    }

    string line;

    while (getline(in, line))
    {
        if (line.rfind("Player 1 wins =", 0) == 0) // finding line
            player1 = stoi(line.substr(16));
        else if (line.rfind("Player 2 wins =", 0) == 0)
            player2 = stoi(line.substr(16));
    }
}
void storing_winner(int win)
{
    int player1 = 0, player2 = 0;

    // previous result
    storing_previous_winnigs_to_player_1_2(player1, player2);

    if (win == 1)
        ++player1;
    else if (win == 2)
        ++player2;

    // saving
    saveLoadGameResult(player1, player2);

    // display
    cout << "-----------------------------\n";
    display_result_of_file();
    cout << "-----------------------------\n\n";

    cout << " Press Any Button to goto Main Menu\n ";
    getch();
    clearScreen();
}

