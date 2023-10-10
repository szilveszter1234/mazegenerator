#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <stack>
#include <fstream>
#include <sstream>

class Maze
{
public:
    Maze(int height, int width);
    void MakingMaze();
    void printing();
    int getHeight();
    int getWidth();
    std::vector<std::vector<bool>> GetmazeUp();
    std::vector<std::vector<bool>> GetmazeLeft();
private:
    std::vector<std::vector<bool>> mazeUp;
    std::vector<std::vector<bool>> mazeLeft;
    int height;
    int width;
};

class Solving
{
public:
    Solving(Maze& maze);
    std::vector<std::vector<bool>> GetRoute();
    void routeMaker();
    bool MoveLegit(std::pair<int,int> currsquare, std::pair<int,int> nextsquare );
    bool route(std::vector<std::vector<bool>>& M,std::vector<std::vector<bool>> mazeUp, std::vector<std::vector<bool>> mazeLeft, int height, int width, std::pair<int,int> from, std::pair<int,int> to);
private:
    Maze& maze;
    std::vector<std::vector<bool>> RoutVec;

};

Maze::Maze(int height, int width) : height(height), width(width)
{
    mazeUp = std::vector<std::vector<bool>>(height, std::vector<bool>(width, true));
    mazeLeft = std::vector<std::vector<bool>>(height, std::vector<bool>(width, true));
}


int Maze :: getHeight()
{
    return height;
}

int Maze :: getWidth()
{
    return width;
}

std::vector<std::vector<bool>> Maze :: GetmazeUp()
{
    return mazeUp;
}

std::vector<std::vector<bool>> Maze :: GetmazeLeft()
{
    return mazeLeft;
}


void Maze :: MakingMaze()
{
    std::vector<std::vector<bool>> V;
    std::mt19937 gen(static_cast<unsigned int>(std::time(0)));
    mazeUp.resize(height, std::vector<bool>(width, true));
    mazeLeft.resize(height, std::vector<bool>(width, true));
    V.resize(height, std::vector<bool>(width, false));
    bool stop = false;
    std::stack<std::pair<int,int>> path;
    path.push(std::make_pair(0,0));
    V.at(0).at(0) = true;
    while(!stop )
    {
        std::vector<std::pair<int,int>> possibleSquares;
        std::pair<int,int> temp = path.top();
        temp.first++;
        if (temp.first < height)
        {
            if (V.at(temp.first).at(temp.second) == false)
            {
                possibleSquares.push_back(temp);
            }
        }
        temp.first = temp.first - 2;
        if (temp.first > -1)
        {
            if (V.at(temp.first).at(temp.second) == false)
            {
                possibleSquares.push_back(temp);
            }
        }
        temp.first++;
        temp.second++;
        if (temp.second < width )
        {
            if (V.at(temp.first).at(temp.second) == false)
            {
                possibleSquares.push_back(temp);
            }
        }
        temp.second = temp.second - 2;
        if (temp.second > -1)
        {
            if (V.at(temp.first).at(temp.second) == false)
            {
                possibleSquares.push_back(temp);
            }
        }
        temp.second++;
        if (possibleSquares.size() == 0)
        {
            path.pop();
        }
        else
        {

            std::uniform_int_distribution<int> dist(0, possibleSquares.size()-1);
            int random = dist(gen);
            if (possibleSquares.at(random).first > path.top().first)
            {
                mazeUp.at(possibleSquares.at(random).first).at(possibleSquares.at(random).second) = false;
            }
            else if (possibleSquares.at(random).first < path.top().first)
            {
                mazeUp.at(path.top().first).at(possibleSquares.at(random).second) = false;
            }
            else if (possibleSquares.at(random).second > path.top().second)
            {
                mazeLeft.at(possibleSquares.at(random).first).at(possibleSquares.at(random).second) = false;
            }
            else if (possibleSquares.at(random).second < path.top().second)
            {
                mazeLeft.at(possibleSquares.at(random).first).at(path.top().second) = false;
            }
            path.push(possibleSquares.at(random));
            V.at(path.top().first).at(path.top().second) = true;

        }
        int IsAllVisited=0;
        for(int i=0; i<height; i++)
        {
            for(int j=0; j<width; j++)
            {
                if (V.at(i).at(j))
                {
                    IsAllVisited++;
                }

            }
        }

        if (IsAllVisited == height*width)
        {
            stop = true;
        }

    }
}

void Maze :: printing()
{
    Solving solving(*this);

    solving.routeMaker();
    std::vector<std::vector<bool>> M = solving.GetRoute();

    for (int i=0; i<height; i++)
    {
        std::cout<<"+";
        for(int j=0; j<width; j++)
        {
            if (mazeUp.at(i).at(j))
            {
                std::cout<<"---+";
            }
            else
            {
                std::cout<<"   +";
            }
        }
        std::cout<<std::endl;

        for(int k=0; k<width; k++)
        {
            if ((mazeLeft.at(i).at(k)) && (M.at(i).at(k)))
            {
                std::cout<<"| . ";
            }
            else if (M.at(i).at(k))
            {
                std::cout<<"  . ";
            }
            else if (mazeLeft.at(i).at(k))
            {
                std::cout<<"|   ";
            }
            else
            {
                std::cout<<"    ";
            }
        }

        std::cout<<"|"<<std::endl;
    }
    std::cout<<"+";
    for(int l=0; l<width; l++)

    {
        std::cout<<"---+";
    }
    std::cout<<std::endl;
}



Solving::Solving(Maze& maze) : maze(maze) {}

std::vector<std::vector<bool>> Solving::GetRoute()
{
    return RoutVec;

}
void Solving :: routeMaker()
{
    int height = maze.getHeight();
    int width = maze.getWidth();
    std::vector<std::vector<bool>> mazeUp = maze.GetmazeUp();
    std::vector<std::vector<bool>> mazeLeft = maze.GetmazeLeft();
    RoutVec.resize(height, std::vector<bool>(width, false));


    std::pair<int,int> start = std::make_pair(0,0);
    std::pair<int,int> stop = std::make_pair(height-1, width-1);


    route(RoutVec,mazeUp,mazeLeft,height,width,start,stop);
}

bool Solving :: MoveLegit(std::pair<int,int> currsquare, std::pair<int,int> nextsquare )
{

    int height = maze.getHeight();
    int width = maze.getWidth();
    std::vector<std::vector<bool>> mazeUp = maze.GetmazeUp();
    std::vector<std::vector<bool>> mazeLeft = maze.GetmazeLeft();
    if (nextsquare.first < 0 || nextsquare.first > height-1)
    {
        return false;
    }
    if (nextsquare.second < 0 || nextsquare.second > width-1)
    {
        return false;
    }
    if (currsquare.first > nextsquare.first)
    {
        if (mazeUp.at(currsquare.first).at(currsquare.second))
        {
            return false;
        }
    }
    if (currsquare.first < nextsquare.first)
    {
        if (mazeUp.at(nextsquare.first).at(nextsquare.second))
        {
            return false;
        }
    }
    if (currsquare.second > nextsquare.second)
    {
        if (mazeLeft.at(nextsquare.first).at(currsquare.second))
        {
            return false;
        }
    }
    if (currsquare.second < nextsquare.second)
    {
        if (mazeLeft.at(nextsquare.first).at(nextsquare.second))
        {
            return false;
        }
    }


    return true;
}

bool Solving :: route(std::vector<std::vector<bool>>& M,std::vector<std::vector<bool>> mazeUp, std::vector<std::vector<bool>> mazeLeft, int height, int width, std::pair<int,int> from, std::pair<int,int> to)
{
    M.at(from.first).at(from.second)=true;
    if (from == to)
    {
        return true;
    }
    std::vector<std::pair<int,int>> neighbors;
    std::pair<int,int> temp=from;
    temp.first++;
    if (MoveLegit(from, temp))
    {
        neighbors.push_back(temp);
    }
    temp.first--;
    temp.first--;
    if (MoveLegit(from, temp))
    {
        neighbors.push_back(temp);
    }
    temp.first++;
    temp.second++;
    if (MoveLegit(from, temp))
    {
        neighbors.push_back(temp);
    }
    temp.second--;
    temp.second--;
    if (MoveLegit(from, temp))
    {
        neighbors.push_back(temp);
    }
    if (neighbors.size() == 0)
    {
        return false;
    }
    for (int n=0; n<neighbors.size(); n++)
    {
        if (!(M.at(neighbors.at(n).first).at(neighbors.at(n).second)) )
            if (route(M, mazeUp, mazeLeft, height, width, neighbors.at(n), to))
            {
                return true;
            }
    }
    M.at(from.first).at(from.second) = false;
    return false;
}


int main(int argc, char* argv[])
{
    if (argc == 2){
    int height = std::stoi(argv[1]);
    int width = std::stoi(argv[2]);
    Maze thismaze(height, width);
    thismaze.MakingMaze();
    thismaze.printing();
    return 0;}
}
