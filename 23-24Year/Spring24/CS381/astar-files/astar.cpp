#include "astar.h"

//////////////////////////////////////////Callback Code//////////////////////////////

template <typename GraphType, typename AStarType>
void AStarCallback<GraphType, AStarType>::OnIteration(const AStarType &astar)
{
}

template <typename GraphType, typename AStarType>
void AStarCallback<GraphType, AStarType>::OnFinish(const AStarType &astar)
{
}

////////////////////////////Open List Code//////////////////////////////////////////

template <typename GraphType, typename Heuristic>
Astar<GraphType, Heuristic>::OpenList::OpenList()
{
    // Constructor implementation
}

template <typename GraphType, typename Heuristic>
Astar<GraphType, Heuristic>::OpenList::~OpenList()
{
    // Destructor implementation
}

template <typename GraphType, typename Heuristic>
void Astar<GraphType, Heuristic>::OpenList::Push(typename GraphType::Vertex *vertex)
{
    // Push implementation
    list.push_back(*vertex);
}

template <typename GraphType, typename Heuristic>
typename GraphType::Vertex *Astar<GraphType, Heuristic>::OpenList::Pop()
{
    // Pop implementation
    typename GraphType::Vertex *vertex = &(list.back());
    list.pop_back();
    return vertex;
}

template <typename GraphType, typename Heuristic>
bool Astar<GraphType, Heuristic>::OpenList::Find(typename GraphType::Vertex *vertex)
{
    // Find implementation
    for (const typename GraphType::Vertex &v : list)
    {
        if (&v == vertex)
            return true;
    }
    return false;
}