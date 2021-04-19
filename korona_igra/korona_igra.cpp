#include <iostream>
#include <time.h>
#include <conio.h>
#include <Windows.h>

using namespace std;


int width, height;
bool quit;
enum dir {STOP, DOWN, DOWNLEFT, DOWNRIGHT};


class Body
{
    int x, y;

public:
    Body(int pos_x, int pos_y)
    {
        x = pos_x;
        y = pos_y;
    }

    void Move_left()
    {
        x--;
    }

    void Move_right()
    {
        x++;
    }

    inline int get_x() { return x; }
    inline int get_y() { return y; }

    //for testing
    friend ostream& operator<< (ostream& os, Body& b)
    {
        return os << "[" << b.x << ", " << b.y << "]";
    }
};


class Virus
{
    int x, y;
    dir direction;

public:
    Virus(int pos_x, int pos_y)
    {
        x = pos_x;
        y = pos_y;
        direction = STOP;
    }

    inline int get_x() { return x; }
    inline int get_y() { return y; }
    inline dir get_direction() { return direction; }

    void Respawn()
    {
        x = (unsigned)((rand() % width - 2) + 1);
        y = 0;
    }

    void Change_direction(dir d)
    {
        direction = d;
    }

    void Random_direction()
    {
        direction = (dir)((rand() % 3) + 1);
    }

    void Move()
    {
        switch (direction)
        {
        case STOP:
            break;
        case DOWN:
            y++;
            break;
        case DOWNLEFT:
            y++; x--;
            break;
        case DOWNRIGHT:
            y++; x++;
            break;
        default:
            break;
        }
    }

    friend ostream& operator<< (ostream& os, Virus& v)
    {
        return os << "[" << v.x << ", " << v.y << "]";
    }
};


class Game
{
    int helth, score;
    Body* body;
    Virus* v1, * v2, * v3;

public:
    Game(int w, int h)
    {
        srand(time(NULL));
        width = w;
        height = h;
        helth = 5;
        score = 0;
        body = new Body(width / 2 - 2, height - 2);
        v1 = new Virus(rand() % width - 2, rand() % 4);
        v2 = new Virus(rand() % width - 2, 0);
        v3 = new Virus(rand() % width - 2, 3);
        v1->Random_direction();
        v2->Random_direction();
        v3->Random_direction();
    }

    ~Game()
    {
        delete body, v1, v2, v3;
    }

    void Input()
    {
        v1->Move();
        v2->Move();
        v3->Move();

        bool pause;

        if (_kbhit()) 
        {
            char press = _getch();
            if (press == 'j')
                if (body->get_x() > 3)
                    body->Move_left();
            if (press == 'l')
                if (body->get_x() < width - 2)
                    body->Move_right();
            if (press == 'q')
                quit = true;
            if (press == 'p')
            {
                v1->Change_direction(STOP);
                v2->Change_direction(STOP);
                v3->Change_direction(STOP);
                pause = true;
            }
            if (pause = true)
            {
                if (press == 'o')
                {
                    v1->Random_direction();
                    v1->Move();
                    v2->Random_direction();
                    v2->Move();
                    v3->Random_direction();
                    v3->Move();
                    pause = false;
                }
            }
        }
    }

    void Draw()
    {
        system("cls");

        int v1_x = v1->get_x();
        int v1_y = v1->get_y();
        int v2_x = v2->get_x();
        int v2_y = v2->get_y();
        int v3_x = v3->get_x();
        int v3_y = v3->get_y();
        int body_x = body->get_x();
        int body_y = body->get_y();

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                if (j == 0)
                    cout << "\xB2";
                else if (v1_x == j && v1_y == i)
                    cout << "K";
                else if (v2_x == j && v2_y == i)
                    cout << "K";
                else if (v3_x == j && v3_y == i)
                    cout << "K";
                else if (body_x == j && body_y == i)
                    cout << "\xDC";
                else if (body_x - 1 == j && body_y == i)
                    cout << "\xDC";
                else if (body_x - 2 == j && body_y == i)
                    cout << "\xDC";
                else if (j == width - 1)
                    cout << "\xB2";
                else
                    cout << " ";
            }
            cout << endl;
        }
        cout << "Helth: " << helth << endl;
        cout << "Score: " << score << endl;
    }

    void Logic()
    {
        if (v1->get_y() == height)
        {
            v1->Respawn();
            v1->Random_direction();
            score += 10;
        }
        if (v2->get_y() == height)
        {
            v2->Respawn();
            v2->Random_direction();
            score += 10;
        }
        if (v3->get_y() == height)
        {
            v3->Respawn();
            v3->Random_direction();
            score += 10;
        }

        for (int i = 0; i < 3; i++)
        {
            if (v1->get_x() == body->get_x() - i && v1->get_y() == body->get_y())
            {
                v1->Respawn();
                helth--;
                Beep(1568, 50);
            }
            if (v2->get_x() == body->get_x() - i && v2->get_y() == body->get_y())
            {
                v2->Respawn();
                helth--;
                Beep(1568, 50);
            }
            if (v3->get_x() == body->get_x() - i && v3->get_y() == body->get_y())
            {
                v3->Respawn();
                helth--;
                Beep(1568, 50);
            }
        }

        if (helth == 0)
        {
            quit = true;
            cout << "You are COVID-19 positive!" << endl;
        }

        if (v1->get_x() == 1)
            v1->Change_direction(DOWNRIGHT);

        if (v1->get_x() == width - 2)
            v1->Change_direction(DOWNLEFT);

        if (v2->get_x() == 1)
            v2->Change_direction(DOWNRIGHT);

        if (v2->get_x() == width - 2)
            v2->Change_direction(DOWNLEFT);

        if (v3->get_x() == 1)
            v3->Change_direction(DOWNRIGHT);

        if (v3->get_x() == width - 2)
            v3->Change_direction(DOWNLEFT);
    }

    void Run()
    {
        while (!quit)
        {
            Draw();
            Input();
            Logic();
        }
    }
};

int main()
{
    Game g(15, 20);
    g.Run();
    /*srand(time(0));
    Virus v((unsigned)(rand() % 9), 0);
    cout << v << endl;
    v.Random_direction();
    cout << v << endl;
    v.Move();
    cout << v << endl;
    v.Move();
    cout << v << endl;*/
}
