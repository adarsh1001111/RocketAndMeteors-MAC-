#include <iostream>
#include <cstdio>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>



using namespace std;

string currentplayerName;
const int screen_width = 80;
const int screen_height = 30;
const char rocket_symbol = '>';
const char meteor1_symbol = '#';
const char powerup_symbol = 'o';
const char drops = 'v';
const int max_leaderboard_size = 10;

class Player
{

private:
    string name;
    int score;

public:
    Player(const string &playerName, int playerScore)
    {
        name = playerName;
        score = playerScore;
    }

    string getName() const
    {
        return name;
    }

    int getScore() const
    {
        return score;
    }
    void setscore(int x){
        score=x;
    }
};

bool compare(Player a,Player b){
    return a.getScore()<b.getScore();
}
int searchinLeaderBoard(Player temp,vector<Player>Leaderboard){
    for(int i=0;i<Leaderboard.size();i++){
        if(temp.getName()==Leaderboard[i].getName()){
            // Leaderboard[i].setscore(temp.getScore());
            return i;
        }
    }
    return -1;
}


class Rocket
{
private:
    int x, y;
    bool isFiring;
    friend class Game;

public:
    Rocket(int startX, int startY)
    {
        x = startX;
        y = startY;
    }

    void draw()
    {
        mvprintw(y, x, ">-<%c", rocket_symbol);
    }
    void draw2()
    {
        if (isFiring)
        {

            mvprintw(y, x, ">>-<%c)", rocket_symbol);
        }
        else
        {
            mvprintw(y, x, ">>-<%c", rocket_symbol);
        }
    }

    void clear()
    {
        mvprintw(y, x, "    ");
    }
    void clear2()
    {
        mvprintw(y, x, "     ");
    }

    void moveUp1()
    {
        clear();
        y--;
        if (y <= 1)
            y = 1;

        draw();
    }
    void moveUp2()
    {
        clear2();
        y--;
        if (y <= 1)
            y = 1;

        draw2();
    }

    void moveDown1()
    {
        clear();
        y++;
        if (y >= screen_height - 2)
            y = screen_height - 2;
        draw();
    }
    void moveDown2()
    {
        clear2();
        y++;
        if (y >= screen_height - 1)
            y = screen_height - 2;
        draw2();
    }

    void moveRight()
    {
        clear();
        x++;
        if (x >= screen_width - 1)
            x = screen_width - 1;

        draw();
    }
    void moveLeft()
    {
        clear();
        x--;
        if (x < 0)
            x = 0;

        draw();
    }
    int getX() const
    {
        return x;
    }

    int getY() const
    {
        return y;
    }
};

class Meteor
{
private:
    int x, y;

public:
    Meteor(int startX, int startY)
    {
        x = startX;
        y = startY;
    }

    void draw()
    {
        mvprintw(y, x, "%c", meteor1_symbol);
    }

    void clear()
    {
        mvprintw(y, x, " ");
    }

    void moveLeft()
    {
        clear();
        x--;
        if (x < 0)
        {
            x = screen_width - 1;
            y = rand() % screen_height;
        }
        draw();
    }

    int getX() const
    {
        return x;
    }

    int getY() const
    {
        return y;
    }
    void setX(int a)
    {
        x = a;
    }
    void setY(int b)
    {
        y = b;
    }
};

class Drop
{
private:
    int x, y;

public:
    Drop(int startX, int startY)
    {
        x = startX;
        y = startY;
    }

    void draw()
    {
        mvprintw(y, x, "%c", drops);
    }

    void clear()
    {
        mvprintw(y, x, " ");
    }

    void moveDown()
    {
        clear();
        y++;
        if (y > screen_height - 1)
        {
            y = 1;
        }
        draw();
    }
    void moveLeft()
    {
        clear();
        x--;
        if (x < 0)
        {
            y = 1;
            x = rand() % screen_width;
        }
        draw();
    }

    int getX() const
    {
        return x;
    }

    int getY() const
    {
        return y;
    }
    void setX(int a)
    {
        x = a;
    }
    void setY(int b)
    {
        y = b;
    }
};

class Powerups
{
private:
    int x;
    int y;

public:
    Powerups(int startX, int startY)
    {
        x = startX;
        y = startY;
    }
    void draw()
    {
        mvprintw(y, x, "%c", powerup_symbol);
    }

    void clear()
    {
        mvprintw(y, x, "  ");
    }
    void moveleft()
    {
        clear();
        x--;

        if (x < 0)
        {
            x = screen_width - 1;
            y = rand() % screen_height;
        }

        draw();
    }

    int getx() const
    {
        return x;
    }
    int gety() const
    {
        return y;
    }
    void setx(int a)
    {
        x = a;
    }
    void sety(int b)
    {
        y = b;
    }
};

class Game
{
private:
    Rocket rocket;
    Meteor meteor1;
    Meteor meteor2;
    Powerups powerup;
    Drop drop;
    int score;
    bool gameOver;
    int level;
    bool isFiring;

public:
    Game() : rocket(screen_width / 4, screen_height / 2), meteor1(screen_width - 1, rand() % screen_height), meteor2(screen_width - 1, rand() % screen_height),
             score(0), gameOver(false), powerup(rand() % screen_width, rand() % screen_height), level(1), drop(rand() % screen_width, 1) {}
    int getlevel()
    {
        return level;
    }
    int getScore()
    {
        return score;
    }

    void sortLeaderboard(vector<Player> &leaderboard)
    {
        int n = leaderboard.size();
        for (int i = 0; i < n - 1; ++i)
        {
            for (int j = 0; j < n - i - 1; ++j)
            {
                if (leaderboard[j].getScore() < leaderboard[j + 1].getScore())
                {
                    swap(leaderboard[j], leaderboard[j + 1]);
                }
            }
        }
    }
    void swap(Player &a, Player &b)
    {
        Player temp = a;
        a = b;
        b = temp;
    }
    void savePlayerInfo(const Player &player)
    {
        FILE *file = fopen("leaderboard.txt", "a");
        if (file != NULL)
        {
            fprintf(file, "%s %d\n", player.getName().c_str(), player.getScore());
            fclose(file);
        }
        else
        {
            cout << "Unable to save Player data";
        }
    }

    vector<Player> Leaderboard;

    void updateLeaderboard()
    {
        Leaderboard.clear();
        FILE *file = fopen("leaderboard.txt", "r");

        if (file != NULL)
        {
            char line[100];
            while (fgets(line, sizeof(line), file) != NULL)
            {
                istringstream iss(line);
                string name;
                int score;

                if (iss >> name >> score)
                {
                    Player random(name, score);
                    int index=searchinLeaderBoard(random,Leaderboard);
                    if(index!=-1){
                        if(random.getScore()>Leaderboard[index].getScore()){
                            Leaderboard[index].setscore(random.getScore());
                        }
                        
                        
                    }
                    else{
                    Leaderboard.push_back(random);
                    }
                   
                    sort(Leaderboard.begin(),Leaderboard.end(),compare);
                    // if (Leaderboard.size() < max_leaderboard_size)
                        
                    // else
                    // {
                    //     cout << "error while displaying NICK name due to unspecified format !";
                    // }
                }
            }
            fclose(file);
        }
        else
        {
            cout << "Unable to update leaderboard" << endl;
        }
    }

    void displayLeaderboard()
    {

        string LeaderboardArt = "\n                           *     .--.\n"
                                "                                / /  `\n"
                                "               +               | |\n"
                                "                      '         \\ \\__,\n"
                                "                  *          +   '--'  *\n"
                                "                      +   /\n"
                                "         +              .'  '.   *\n"
                                "                *      /======\\      +\n"
                                "                      ;:.  _   ;\n"
                                "                      |:. (_)  |\n"
                                "                      |:.  _   |\n"
                                "            +         |:. (_)  |          *\n"
                                "                      ;:.      ;\n"
                                "                    .' \\:.    / `.\n"
                                "                   / .-'':._.'`-. \n"
                                "                   |/    /||\\    \\|\n"
                                "                _..--\"\"\"````\"\"\"--.._\n"
                                "           _.-'``                    ``'-._\n"
                                "         -'                                '-\n";

        cout << LeaderboardArt;
        cout << "               LEADERBOARD" << endl;
        cout << "       ==========================" << endl;

        for (size_t i = 0; i < Leaderboard.size() && i < max_leaderboard_size; ++i)
        {
            cout << i + 1 << ". " << Leaderboard[i].getName() << "  " << Leaderboard[i].getScore() << endl;
        }
    }

    void run()
    {
        initscr();
        noecho();
        curs_set(0);
        keypad(stdscr, true);
        nodelay(stdscr, true);

        srand(time(NULL));

        while (!gameOver)
        {
            char key = getch();

            if (key == 'w' || key == 'W')
            {
                rocket.moveUp1();
            }
            else if (key == 's' || key == 'S')
            {
                rocket.moveDown1();
            }

            else if (key == 'q' || key == 'Q')
                gameOver = true;

            if (level == 3)
            {

                if (key == 'f' || key == 'F')
                {
                    isFiring = true;
                    rocket.isFiring = true;
                }
                else
                {
                    isFiring = false;
                    rocket.isFiring = false;
                }

                if (key == 'd' || key == 'D')
                {
                    rocket.moveRight();
                }
                else if (key == 'a' || key == 'A')
                {
                    rocket.moveLeft();
                }
            }
            meteor1.moveLeft();
            meteor2.moveLeft();
            powerup.moveleft();
            if (level == 3)
            {
                drop.moveDown();
                drop.moveLeft();
            }

            if ((rocket.getX() == meteor1.getX() && rocket.getY() == meteor1.getY()) || ((rocket.getX() == meteor2.getX() && rocket.getY() == meteor2.getY())))
            {
                gameOver = true;
                break;
            }

            if (rocket.getX() == powerup.getx() && rocket.getY() == powerup.gety())
            {
                powerup.clear();
                mvprintw(rocket.getY() - 1, rocket.getX(), "+50");
                score = score + 100;
                powerup.setx(screen_width - 1);
                powerup.sety(rand() % screen_height);
            }
            if (rocket.getY() == meteor1.getY() && isFiring == true && meteor1.getX() > rocket.getX())
            {
                meteor1.clear();
                meteor1.setX(screen_width - 1);
                meteor1.setY(rand() % screen_height);
            }
            if (rocket.getY() == meteor2.getY() && isFiring == true && meteor2.getX() > rocket.getX())
            {
                meteor2.clear();
                meteor2.setX(screen_width - 1);
                meteor2.setY(rand() % screen_height);
            }
            if (rocket.getY() == drop.getY() && level == 3 && rocket.getX() == drop.getX())
            {
                gameOver = true;
                break;
            }

            score++;
            if (score < 500)
            {
                drawGame1();
                napms(100);
            }
            if (score >= 500 && score < 1000)
            {
                level = 2;
                drawGame2();
                napms(50);
            }
            if (score >= 1000)
            {
                level = 3;
                drawGame3();
                napms(30);
            }
        }

        endwin(); 
        Player currentplayer(currentplayerName, score);
        savePlayerInfo(currentplayer);
        cout << endl
             << endl
             << "Game Over!" << endl
             << "Score: " << score << endl;
        updateLeaderboard();
        sortLeaderboard(Leaderboard);
        displayLeaderboard();
    }

    void drawGame1()
    {
        clear();

        for (int i = 0; i < screen_height; i++)
        {
            for (int j = 0; j < screen_width; j++)
            {
                if (i == 0 || i == screen_height - 1)
                    printw("-");
                else if (i == rocket.getY() && j == rocket.getX())
                    rocket.draw();
                else if (i == meteor1.getY() && j == meteor1.getX())
                {
                    meteor1.draw();
                }
                else if (i == meteor2.getY() && j == meteor2.getX())
                {
                    meteor2.draw();
                }
                else if (i == powerup.gety() && j == powerup.getx())
                {
                    powerup.draw();
                }
                else if (i == 1 && j == 1)
                {
                    mvprintw(1, 1, "%d", level);
                }
                else
                    printw(" ");
            }
            printw("\n");
        }

        printw("Score : %d\n", score);
        refresh();
    }
    void drawGame2()
    {
        clear();

        for (int i = 0; i < screen_height; i++)
        {
            for (int j = 0; j < screen_width; j++)
            {
                if (i == 0 || i == screen_height - 1)
                    printw("-");
                else if (i == rocket.getY() && j == rocket.getX())
                    rocket.draw2();
                else if (i == meteor1.getY() && j == meteor1.getX())
                {
                    meteor1.draw();
                }
                else if (i == meteor2.getY() && j == meteor2.getX())
                {
                    meteor2.draw();
                }
                else if (i == powerup.gety() && j == powerup.getx())
                {
                    powerup.draw();
                }
                else if (i == 1 && j == 1)
                {
                    mvprintw(1, 1, "%d", level);
                }
                else
                    printw(" ");
            }
            printw("\n");
        }

        printw("Score : %d\n", score);
        refresh();
    }
    void drawGame3()
    {
        clear();

        for (int i = 0; i < screen_height; i++)
        {
            for (int j = 0; j < screen_width; j++)
            {
                if (i == 0 || i == screen_height - 1)
                    printw("-");
                else if (i == rocket.getY() && j == rocket.getX())
                    rocket.draw2();
                else if (i == meteor1.getY() && j == meteor1.getX())
                {
                    meteor1.draw();
                }
                else if (i == meteor2.getY() && j == meteor2.getX())
                {
                    meteor2.draw();
                }
                else if (i == powerup.gety() && j == powerup.getx())
                {
                    powerup.draw();
                }
                else if (i == 1 && j == 1)
                {
                    mvprintw(1, 1, "%d", level);
                }
                else if (isFiring && i == rocket.getY() && j > rocket.getX() + 3 && j < screen_width - 1)
                {
                    mvprintw(rocket.getY(), rocket.getX() + 4, ")");
                    mvprintw(rocket.getY(), j, "-");
                }
                else if (i == drop.getY() && j == drop.getX())
                {
                    drop.draw();
                }
                else
                    printw(" ");
            }
            printw("\n");
        }

        printw("Score : %d\n", score);
        refresh();
    }
};

int main()
{

    string rocketAsciiArt =
        "       ^\n"
        "   o  / \\\n"
        "     /___\\\n"
        "    |     |     o             Rules:       \n"
        " o  |  R  |                        1. Control the Rocket with 'W' and 'S' keys to move it Up and Down.  \n"
        "    |  O  |                        2. Avoid the Meteors in the path of the Rocket.\n"
        "    |  C  |  o                     3. Collect as many powerups as you can to boost your score.  \n"
        "    |  K  |                        4. Press 'Q' key to Quit the Game while playing.\n"
        "    |  E  |\n"
        "    |  T  |\n"
        "    |     |\n"
        "    |     |\n"
        "   /|##!##|\\\n"
        "  / |##!##| \\\n";

    // Print the rocket ASCII art
    cout << rocketAsciiArt << endl;
    int n;
    Game game;

    cout << "ENTER your  NICKNAME (only characters and numbers): ";
    cin >> currentplayerName;

    cout << endl
         << "Press 1 to Enter the game " << endl
         << "Press 0 to exit the program" << endl
         << endl;

    cin >> n;
    
    bool invalid = true;
    while (invalid)
    {

        switch (n)
        {
        case 0:
        {
            invalid = false;
            return 0;
            break;
        }
        case 1:
        {
            invalid = false;

            game.run();

            break;
        }
        default:
        {
            cout << endl
                 << "Invalid input !!" << endl
                 << endl;
            invalid = true;
            cout << "Press 1 to Start your cosmos Adventure " << endl
                 << "Press 0 to end" << endl;
            cin >> n;
        }
        }
    }
}
