#include "e-dijkstra.h"
#include <limits>
#include <vector>
#include <iostream>
#include <fstream>

///////////////////////////////////Everything from my H file because we can't submit that for some reason, stupid////////////////////////////////////

class EdgeLogic;
class OpenList;

/**
 * @brief Uses Dijkstra to figure out whether there is a path from vertex1 to vertex2
 *
 * @param vertex1 The city we're starting at
 * @param vertex2 The city we're trying to get to
 * @param openList The car list
 * @param edgeLogic The edge list
 * @return true There is a path
 * @return false There is not a path
 */
bool CanReach(int vertex1, int vertex2, OpenList *openList, EdgeLogic *edgeLogic);

/**
 * @brief The current list that a car is on
 *
 */
enum class ListFlags : unsigned char
{
    Neither = 1,
    Open = 2,
    Closed = 4
};

// These are not used
ListFlags operator|(ListFlags lhs, ListFlags rhs);
ListFlags operator&(ListFlags lhs, ListFlags rhs);

/**
 * @brief The car struct, there is a car for every vertex
 *
 */
struct Car
{
    Car(int position, int range, int chargesLeft);
    Car(int position, int range, int rangeLeft, int chargesLeft, int distanceTraveled);

    /**
     * @brief The vertex this car is at
     *
     */
    int position;

    /**
     * @brief This car's parent, ie what city it came from
     *
     */
    int parent;

    /**
     * @brief The max range of the car
     *
     */
    int range;

    /**
     * @brief The amount of range the car has left
     *
     */
    int rangeLeft;

    /**
     * @brief How many more times the car can charge
     *
     */
    int chargesLeft;

    // Not used
    int distanceTraveled;

    /**
     * @brief Current list this car is on
     *
     */
    ListFlags curList;
};

class EdgeLogic
{

public:
    EdgeLogic(int numEdges);
    ~EdgeLogic();

    /**
     * @brief Struct for an edge between vertices
     *
     */
    struct Edge
    {
        Edge(int cost, int vertex1, int vertex2);

        /**
         * @brief The weight of this edge
         *
         */
        int cost;

        /**
         * @brief What vertices this edge goes between
         *
         */
        std::pair<int, int> vertices;
    };

    /**
     * @brief Get the travel cost from one vertex to another, not used
     *
     * @param vertex1 The first vertex
     * @param vertex2 The second vertex
     * @return int The cost between these two vertices, assuming there is an edge between them
     */
    int GetTravelCost(int vertex1, int vertex2);

    /**
     * @brief Add an edge to the edge list
     *
     * @param newEdge The edge we're adding
     */
    void Add(Edge *newEdge);

    /**
     * @brief Get the edges that go to/come from this vertex
     *
     * @param vertex The vertex we're going to or coming from
     * @return std::vector<Edge *> A vector of all the neighbor edges
     */
    std::vector<Edge *> GetNeighborEdges(int vertex);

private:
    /**
     * @brief List of all edges
     *
     */
    std::vector<Edge *> edges;
};

class OpenList
{
private:
    /**
     * @brief The list of cars, one for each vertex
     *
     */
    std::vector<Car *> carList;

    /**
     * @brief The default range read from file
     *
     */
    int defaultRange;

    /**
     * @brief The default charges read from file
     *
     */
    int defaultCharges;

public:
    OpenList(int numVertices, int defaultRange, int defaultCharges);
    ~OpenList();

    /**
     * @brief Get the car that has traveled the furthest and is on the open list
     *
     * @return Car*
     */
    Car *Pop();

    /**
     * @brief Push a car onto the open list, not used
     *
     * @param vertex
     * @param car
     */
    void Push(int vertex, Car *car);

    /**
     * @brief Replace a car on the open list, not used
     *
     * @param vertex
     * @param car
     */
    void Replace(int vertex, Car *car);

    int GetDefaultRange() { return defaultRange; };

    /**
     * @brief Used to set the car at the vertex we're starting at to the open list
     *
     * @param vertex The vertex we're starting at
     */
    void SetFirstCar(int vertex);

    /**
     * @brief Reset data of all cars
     *
     */
    void Reset();

    /**
     * @brief Check which list this car is on
     *
     * @param vertex The position of the car we're checking
     * @return ListFlags The list it's on
     */
    ListFlags CheckList(int vertex);

    /**
     * @brief Sets all kinds of data on the car given a new parent
     *
     * @param car The position of the car we're setting the data on
     * @param parentCar The new parent car for this position
     * @param edgeTraveled The edge being traveled on, needed for cost
     */
    void SetNewParent(int car, Car *parentCar, EdgeLogic::Edge *edgeTraveled);

    /**
     * @brief Check if this new parent car is a more efficient way to get to this position
     *
     * @param car The position of the car we're checking
     * @param parentCar The potential new parent car
     * @return true Is this path more efficient
     * @return false Is this path less efficient
     */
    bool CheckCloser(int car, Car *parentCar);
};

//////////////////////////////////////List Flags Code///////////////////////////////////////////////////////

ListFlags operator|(ListFlags lhs, ListFlags rhs)
{
    using ListFlagsType = std::underlying_type<ListFlags>::type;
    return ListFlags(static_cast<ListFlagsType>(lhs) | static_cast<ListFlagsType>(rhs));
}
ListFlags operator&(ListFlags lhs, ListFlags rhs)
{
    using ListFlagsType = std::underlying_type<ListFlags>::type;
    return ListFlags(static_cast<ListFlagsType>(lhs) & static_cast<ListFlagsType>(rhs));
}

///////////////////////////////////////////////Edge Logic Code/////////////////////////////////////////////////////////

EdgeLogic::EdgeLogic(int numEdges)
    : edges()
{
    // Reserve space to prevent reallocs
    edges.reserve(numEdges);
}

EdgeLogic::~EdgeLogic()
{
    for (int i = 0; i < static_cast<int>(edges.size()); i++)
    {
        delete edges[i];
    }
}

EdgeLogic::Edge::Edge(int cost, int vertex1, int vertex2)
    : cost(cost), vertices(std::make_pair(vertex1, vertex2))
{
}

int EdgeLogic::GetTravelCost(int vertex1, int vertex2)
{

    for (auto edge : edges)
    {
        // 2 if statements used to find the edge between these two vertices
        if (edge->vertices.first == vertex1 || edge->vertices.first == vertex2)
        {
            if (edge->vertices.second == vertex1 || edge->vertices.second == vertex2)
            {
                return edge->cost;
            }
        }
    }

    return 0;
}

void EdgeLogic::Add(Edge *newEdge)
{
    edges.push_back(newEdge);
}

std::vector<EdgeLogic::Edge *> EdgeLogic::GetNeighborEdges(int vertex)
{
    std::vector<EdgeLogic::Edge *> neighbors;

    for (auto edge : edges)
    {
        // Is this edge attached to the given vertex
        if (edge->vertices.first == vertex || edge->vertices.second == vertex)
        {
            // This is a neighbor edge
            neighbors.push_back(edge);
        }
    }

    return neighbors;
}

//////////////////////////////Open List Code/////////////////////////////////////////////////////////

OpenList::OpenList(int numVertices, int defaultRange, int defaultCharges)
    : carList(), defaultRange(defaultRange), defaultCharges(defaultCharges)
{
    // Reserve space to prevent reallocs
    carList.reserve(numVertices);

    // Create a car for each vertex
    for (int i = 0; i < numVertices; i++)
    {
        Car *newCar = new Car(i, defaultRange, defaultCharges);
        carList.push_back(newCar);
    }
}
OpenList::~OpenList()
{
    for (int i = 0; i < static_cast<int>(carList.size()); i++)
    {
        delete carList[i];
    }
}

Car *OpenList::Pop()
{
    Car *carToPop = nullptr;
    int maxDistance = std::numeric_limits<int>().min();

    for (auto car : carList)
    {
        // If this car is on the open list and has traveled the furthest
        if (car && car->curList == ListFlags::Open && car->distanceTraveled > maxDistance)
        {
            carToPop = car;
            maxDistance = car->distanceTraveled;
        }
    }

    // Set this car to the closed list
    if (carToPop)
    {
        carToPop->curList = ListFlags::Closed;
    }

    return carToPop;
}

void OpenList::Push(int vertex, Car *car)
{
    carList[vertex] = car;
    car->curList = ListFlags::Open;
}

void OpenList::Replace(int vertex, Car *car)
{
    carList[vertex] = car;
    car->curList = ListFlags::Open;
}

void OpenList::SetFirstCar(int vertex)
{
    carList[vertex]->curList = ListFlags::Open;
}

void OpenList::Reset()
{
    // Reset all important data on car to prevent cross contamination
    for (auto car : carList)
    {
        car->curList = ListFlags::Neither;
        car->chargesLeft = defaultCharges;
        car->rangeLeft = 0;
        car->distanceTraveled = 0;
        car->parent = -1;
    }
}

ListFlags OpenList::CheckList(int vertex)
{
    return carList[vertex]->curList;
}

void OpenList::SetNewParent(int car, Car *parentCar, EdgeLogic::Edge *edgeTraveled)
{
    // Set the new distance traveled and parent
    carList[car]->distanceTraveled = parentCar->distanceTraveled + edgeTraveled->cost;
    carList[car]->parent = parentCar->position;

    // If we need to recharage before going to this position
    if (parentCar->rangeLeft < edgeTraveled->cost)
    {
        carList[car]->chargesLeft = parentCar->chargesLeft - 1;
        carList[car]->rangeLeft = parentCar->range - edgeTraveled->cost;
    }
    else
    {
        carList[car]->chargesLeft = parentCar->chargesLeft;
        carList[car]->rangeLeft = parentCar->rangeLeft - edgeTraveled->cost;
    }

    carList[car]->curList = ListFlags::Open;
}

bool OpenList::CheckCloser(int car, Car *parentCar)
{
    // Does the parent have more charges left
    if (parentCar->chargesLeft > carList[car]->chargesLeft)
    {
        return true;
    }
    // If it doesn't have more charges then does it have more range left
    else if (parentCar->chargesLeft == carList[car]->chargesLeft && parentCar->rangeLeft > carList[car]->rangeLeft)
    {
        return true;
    }

    return false;
}

///////////////////////////////Car Code///////////////////////////////////////////////

Car::Car(int position, int range, int chargesLeft)
    : position(position), parent(-1), range(range), rangeLeft(0), chargesLeft(chargesLeft), distanceTraveled(0), curList(ListFlags::Neither)
{
}
Car::Car(int position, int range, int rangeLeft, int chargesLeft, int distanceTraveled)
    : position(position), parent(-1), range(range), rangeLeft(rangeLeft), chargesLeft(chargesLeft), distanceTraveled(distanceTraveled), curList(ListFlags::Neither)
{
}

///////////////////////////////////////////Dijkstra Logic//////////////////////////////////////////////////

bool e_dijkstra(char const *fileName, int range)
{
    OpenList *openList = nullptr;
    EdgeLogic *edgeLogic = nullptr;

    int numVertices;
    int maxCharges;
    int numEdges;

    std::ifstream inputStream;

    inputStream.open(fileName);

    if (inputStream.is_open())
    {
        // Read in the basic info
        if (inputStream >> numVertices >> maxCharges >> numEdges)
        {
            // Create the open list and edge logic from this data
            openList = new OpenList(numVertices, range, maxCharges);
            edgeLogic = new EdgeLogic(numEdges);

            for (int i = 0; i < numEdges; i++)
            {
                int vertex1;
                int vertex2;
                int cost;

                // Read in data for each edge
                if (inputStream >> vertex1 >> vertex2 >> cost)
                {
                    // If the cost of this edge is greater than the car's max range don't bother adding it
                    if (cost <= range)
                    {
                        EdgeLogic::Edge *edge = new EdgeLogic::Edge(cost, vertex1, vertex2);
                        edgeLogic->Add(edge);
                    }
                }
                else
                {
                    std::cerr << "Failed to read edge" << std::endl;
                }
            }
        }
        else
        {
            std::cerr << "Couldn't read base info" << std::endl;
        }
    }
    else
    {
        std::cerr << "Couldn't open file" << std::endl;
    }

    inputStream.close();

    // Iterate through each vertex and the remaining vertices
    for (int i = 0; i < numVertices; i++)
    {
        for (int j = i + 1; j < numVertices; j++)
        {
            // If we can't reach a city then we've failed, just return false
            if (!CanReach(i, j, openList, edgeLogic))
            {
                delete openList;
                delete edgeLogic;
                return false;
            }

            // Reset car data between checks
            openList->Reset();
        }
    }

    // We never failed so every city is reachable
    delete openList;
    delete edgeLogic;
    return true;
}

bool CanReach(int vertex1, int vertex2, OpenList *openList, EdgeLogic *edgeLogic)
{
    /*
    If (request.newRequest) {
      Initialize everything. Clear Open/Closed Lists.
      Push Start Node onto the Open List with cost of f(x) = g(x) + h(x).
}
While (Open List is not empty) {
parentNode = Pop cheapest node off Open List.
If parentNode is the Goal Node, then path found (return PathResult::COMPLETE).
Place parentNode on the Closed List.
For (all neighboring child nodes of parentNode) {
Compute its cost, f(x) = g(x) + h(x)
If child node isn’t on Open or Closed list, put it on Open List.
Else if child node is on Open or Closed List, AND this new one is cheaper,
    then take the old expensive one off both lists and put this new
    cheaper one on the Open List.
}
If taken too much time this frame (or if request.settings.singleStep == true),
     abort search for now and resume next frame (return PathResult::PROCESSING).
}
Open List empty, thus no path possible (return PathResult::IMPOSSIBLE).

    */

    // Put the first car on the open list
    openList->SetFirstCar(vertex1);

    while (true)
    {
        // Get the "cheapest" car
        Car *parent = openList->Pop();

        // If it doesn't exist there are no cars on the open list so we've failed
        if (parent == nullptr)
        {
            return false;
        }

        // If this is the goal
        if (parent->position == vertex2)
        {
            return true;
        }

        std::vector<EdgeLogic::Edge *> neighbors = edgeLogic->GetNeighborEdges(parent->position);

        // Iterate through the valid neighbor edges
        for (auto edge : neighbors)
        {
            // The position we're traveling to on this edge
            int travelingTo = edge->vertices.first == parent->position ? edge->vertices.second : edge->vertices.first;

            // If we can't get to this new position or we're trying to backtrack don't bother
            if ((parent->chargesLeft == 0 && parent->rangeLeft < edge->cost) || travelingTo == parent->parent)
            {
                continue;
            }

            // If this position hasn't been visited yet just add it
            if (openList->CheckList(travelingTo) == ListFlags::Neither)
            {
                openList->SetNewParent(travelingTo, parent, edge);
            }
            // If this position has been visitied and this is a more efficient route add it
            else if (openList->CheckCloser(travelingTo, parent))
            {
                openList->SetNewParent(travelingTo, parent, edge);
            }
        }
    }
}