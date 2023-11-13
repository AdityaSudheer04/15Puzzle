#include<iostream>
#include<limits>
#include<string_view>
#include"random.h"
#include<cassert>

constexpr int g_consoleLines{ 20 };

class Tile
{
private:
    int m_num;
public:
    Tile() = default;
    explicit Tile(int num)
        : m_num{num}
    {

    }
    friend std::ostream& operator<<(std::ostream& out,const Tile& t)
   {
        if (t.m_num > 9) // if two digit number
            out << " " << t.m_num << " ";
        else if (t.m_num > 0) // if one digit number
            out << "  " << t.m_num << " ";
        else if (t.m_num == 0) // if empty spot
            out << "    ";
        return out;
    }
    int getNum() const {return m_num;}
    bool isEmpty()
    {
        return (m_num!=0);
    }
    
};


class Direction
{


public:
    enum Type
    {
        up,
        down,
        left,
        right,
        max_directions
    };
    Direction() = default;
    Direction(Type dir)
        :m_dir{dir}
    {
    }
    auto getType() const {return m_dir;}
    Direction operator-() const
    {
        switch (m_dir)
        {
        case up:
            return down;
            break;
        case down:
            return up;
            break;
        case left:
            return right;
            break;
        case right:
            return left;
            break;
        
        default:
            break;
        }
        assert(0 && "Unsupported direction was passed!");
        return Direction{ up };
    }
    friend std::ostream& operator<<(std::ostream& stream,const Direction& dir)
    {
        stream<<"";
        switch(dir.m_dir)
        {
            case Direction::up:     return (stream << "up");
            case Direction::down:   return (stream << "down");
            case Direction::left:   return (stream << "left");
            case Direction::right:  return (stream << "right");
            default:                return (stream << "unknown direction");
        }
        
        // return stream;
    }

    static Direction randomdir()
    {
        Type random{static_cast<Type>(Random::get(0,max_directions - 1))};
        return Direction{random};
    }
private:
    Type m_dir{};

};


namespace UserInput
{
    
    
    bool isValidCommand(char ch)
    {
    return ch == 'w'
        || ch == 'a'
        || ch == 's'
        || ch == 'd'
        || ch == 'q';
    }

    void ignoreLine()
    {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    char getCharacter()
    {
        char operation{};
        std::cin >> operation;
        ignoreLine(); // remove any extraneous input
        return operation;
    }

    char getCommandFromUser()
    {
        char ch{};
        while (!isValidCommand(ch))
            ch = getCharacter();

        return ch;
    }
    
    Direction charToDirection(char ch)
    {
        switch (ch)
        {
        case 'w': return Direction{ Direction::up };
        case 's': return Direction{ Direction::down };
        case 'a': return Direction{ Direction::left };
        case 'd': return Direction{ Direction::right };
        }

        assert(0 && "Unsupported direction was passed!");
        return Direction{ Direction::up };
    }
    
}


class Point
{
public:
    Point(int x, int y)
        : m_x{x},
          m_y{y}
    {
    }
    // friend Board;
    bool comparePoint(const Point& p)
    {
        return(m_x == p.m_x && m_y == p.m_y);
    }
    int getX() const {return m_x;}
    int getY() const {return m_y;}
    Point getAdjacentPoint(const Direction& dir)
    {
        int x{};
        int y{};
        switch (dir.getType())
        {
        case Direction::up: return Point{m_x,m_y-1};
        case Direction::down: return Point{m_x,m_y+1};
        case Direction::left: return Point{m_x-1,m_y};
        case Direction::right: return Point{m_x+1,m_y};
        default: return Point{m_x,m_y};
        }        
    }

    bool operator==(const Point& p)
    {
        return(m_x == p.m_x && m_y == p.m_y);
    }

    bool operator!=(const Point& p)
    {
        return!(m_x == p.m_x && m_y == p.m_y);
    }
private:
    int m_x{};
    int m_y{};
};


class Board
{
private:
    static constexpr int SIZE = 4;
    Tile m_tiles[SIZE][SIZE]
    {
        {Tile{ 1 }, Tile { 2 }, Tile { 3 } , Tile { 4 }},
        {Tile { 5 } , Tile { 6 }, Tile { 7 }, Tile { 8 }},
        {Tile { 9 }, Tile { 10 }, Tile { 11 }, Tile { 12 }},
        {Tile { 13 }, Tile { 14 }, Tile { 15 }, Tile { 0 }} 
    };
public:
    Board() = default;
    

    static void printEmptyLines(int count)
    {
        for (int i = 0; i < count; ++i)
            std::cout << '\n';
    }

    friend std::ostream& operator<<(std::ostream& out,const Board& b)
    {

        printEmptyLines(g_consoleLines);
        out<<"";
        for(int i{0}; i < 4; i++)
        {
            for(int j{0}; j < 4; j++)
            {
                out << b.m_tiles[i][j];
            }
            out<<'\n';
        }
        return out;
    }
    static bool validPoint(const Point& p)
    {
        return (p.getX() < 4 && p.getX() >= 0 && p.getY() < 4 && p.getY() >= 0);
    }

    Point EmptyTile() 
    {
        for (int i{0}; i < 4; i++)
        {
            for(int j{0}; j < 4; j++)
            {
                if (m_tiles[i][j].getNum() == 0)
                {
                    return Point{j,i};
                }
            }
        }
        assert(0 && "There is no empty tile in the board!!!");
        return { -1,-1 };
    }

    auto swap(Point& P1, Point& P2)
    {
        auto temp = m_tiles[P1.getY()][P1.getX()];
        m_tiles[P1.getY()][P1.getX()] = m_tiles[P2.getY()][P2.getX()];
        m_tiles[P2.getY()][P2.getX()] = temp;
    }

    auto operator==(const Board& b)
    {
        for(int i{0}; i < 4; i++)
        {
            for(int j{0}; j < 4; j++)
            {
                if(m_tiles[i][j].getNum() != b.m_tiles[i][j].getNum())
                    return false;
            }
        }
        return true;
    }

    bool moveTile(Direction dir)
    {
        Point P1{EmptyTile()};
        
        Point P2{P1.getAdjacentPoint(-dir)};
        if (!validPoint(P2))
            return false;
        
        swap(P1,P2);
        return true;
    }

    bool playerWon() const
    {
        static Board solved_board{};
        return (solved_board == *this);
    }

    void randomize()
    {
        
        for (int i = 0; i < 1000; ++i)
        {
            bool success = moveTile(Direction::randomdir());
            
            if (!success)
                --i;
        }
    }

};


int main()
{
    std:: cout<< "Rules:\nTo move the tiles:  w : up, s : down, d : right, a : left.\n";
    std::cout<<"The adjacent tile will move in the place of empty box\n";
    std::cout<<"You win if you sort the tiles.\nYou can quit anytime using q.\n";
    Board board{};
    board.randomize();
    std::cout << board;

    while(true)
    {
        char in{UserInput::getCommandFromUser()};
        
        if(in == 'q')
        {
            std::cout<<"\n\nBye!\n\n";
            break;
        }
        Direction dir{ UserInput::charToDirection(in) };
        board.moveTile(dir);
        if (board.playerWon())
        {
            
            std::cout<<"\n\nYou won!\n\n";
            std::cout<<"Press q to quit game: ";
            char choice{};
            std::cin>>choice;
            if(choice == 'Q' || choice == 'q')
                break;
            

            
            
        }
        std::cout<<"You entered direction "<<dir<<'\n';
        std::cout<<board;

        
    }
    
    return 0;
}