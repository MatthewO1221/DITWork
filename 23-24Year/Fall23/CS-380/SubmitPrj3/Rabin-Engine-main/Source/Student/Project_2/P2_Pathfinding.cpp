#include <pch.h>
#include "Projects/ProjectTwo.h"
#include "P2_Pathfinding.h"



#pragma region Extra Credit
bool ProjectTwo::implemented_floyd_warshall()
{
    return false;
}

bool ProjectTwo::implemented_goal_bounding()
{
    return false;
}

bool ProjectTwo::implemented_jps_plus()
{
    return false;
}
#pragma endregion

bool AStarPather::initialize()
{
    // handle any one-time setup requirements you have

    /*
        If you want to do any map-preprocessing, you'll need to listen
        for the map change message.  It'll look something like this:

        Callback cb = std::bind(&AStarPather::your_function_name, this);
        Messenger::listen_for_message(Messages::MAP_CHANGE, cb);

        There are other alternatives to using std::bind, so feel free to mix it up.
        Callback is just a typedef for std::function<void(void)>, so any std::invoke'able
        object that std::function can wrap will suffice.
    */

    //Create openList and nodeLogic for pathfinding
    openList = new OpenList();
    nodeLogic = new NodeLogic();
   

    //Set precompution callback
    Callback cb = std::bind(&AStarPather::PrecomputeCallback, this);
    Messenger::listen_for_message(Messages::MAP_CHANGE, cb);


    return true; // return false if any errors actually occur, to stop engine initialization
}




void AStarPather::shutdown()
{
    
    /*
        Free any dynamically allocated memory or any other general house-
        keeping you need to do during shutdown.
    */
}

PathResult AStarPather::compute_path(PathRequest &request)
{
    /*
        This is where you handle pathing requests, each request has several fields:

        start/goal - start and goal world positions
        path - where you will build the path upon completion, path should be
            start to goal, not goal to start
        heuristic - which heuristic calculation to use
        weight - the heuristic weight to be applied
        newRequest - whether this is the first request for this path, should generally
            be true, unless single step is on

        smoothing - whether to apply smoothing to the path
        rubberBanding - whether to apply rubber banding
        singleStep - whether to perform only a single A* step
        debugColoring - whether to color the grid based on the A* state:
            closed list nodes - yellow
            open list nodes - blue

            use terrain->set_color(row, col, Colors::YourColor);
            also it can be helpful to temporarily use other colors for specific states
            when you are testing your algorithms

        method - which algorithm to use: A*, Floyd-Warshall, JPS+, or goal bounding,
            will be A* generally, unless you implement extra credit features

        The return values are:
            PROCESSING - a path hasn't been found yet, should only be returned in
                single step mode until a path is found
            COMPLETE - a path to the goal was found and has been built in request.path
            IMPOSSIBLE - a path from start to goal does not exist, do not add start position to path
    */
    sizeof(GridPos);
    sizeof(float);
    sizeof(unsigned char);
    sizeof(DirectionFlags);
    sizeof(ListFlags);
    sizeof(NodeLogic::Node);
    // WRITE YOUR CODE HERE

    /*
    
    If (request.newRequest) 
{ 
      Initialize everything. Clear Open/Closed Lists. 
      Push Start Node onto the Open List with cost of f(x) = g(x) + h(x).
}
While (Open List is not empty) 
{
    parentNode = Pop cheapest node off Open List.
    If parentNode is the Goal Node, then path found (return PathResult::COMPLETE).
    Place parentNode on the Closed List.
    For (all valid neighboring child nodes of parentNode) 
    {
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
    
   
    
    NodeLogic::Node* goalNode = nodeLogic->GetNodeAtPos(request.goal);

    //If this is a new request
    if (request.newRequest)
    {
        //Clear both
        nodeLogic->Clear();
        openList->Clear();

        //Reset the start node's given cost and push it onto the open list
        goalNode = nodeLogic->GetNodeAtPos(request.goal);
        nodeLogic->GetNodeAtPos(request.start)->givenCost = 0.0f;
        openList->Push(nodeLogic->GetNodeAtPos(request.start), request.settings.heuristic, terrain->get_grid_position(request.goal), request.settings.weight);
    }

    //While the open list is not empty
    while (!openList->listEmpty)
    {
        //Pop the cheapest node
        NodeLogic::Node* parent = openList->Pop();

       
        //If this node is the goal
        if (parent == goalNode)
        {
            //Clear the old path
            nodeLogic->path.clear();

            //Call get path to get a path from start to finish
            nodeLogic->GetPath(parent, nodeLogic->GetNodeAtPos(request.start));

            //If we're supposed to rubberband
            if (request.settings.rubberBanding && ! request.settings.smoothing)
            {
                nodeLogic->RubberbandPath();
            }
            //If we're supposed to smooth
            else if (request.settings.smoothing && !request.settings.rubberBanding)
            {
                nodeLogic->SplinePath();
            }
            //If we're supposed to do both
            else if (request.settings.rubberBanding && request.settings.smoothing)
            {
                nodeLogic->RubberbandSplinePath();
            }

            //After making our path we need to push it into the request's path output
            for each (auto node in nodeLogic->path)
            {
                request.path.push_back(node);
            }
            return PathResult::COMPLETE;
        }

        //Put the parent on the closed list
        parent->curList = ListFlags::Closed;

        //Set the parents color to reflect closed status
        if (request.settings.debugColoring)
        {
            terrain->set_color(GridPos{parent->posRow, parent->posCol}, Colors::Yellow);
        }

        int curDirection = 1;
        //Loops through each of the 8 directions
        for each (auto neighbor in nodeLogic->neighbors[parent->posRow][parent->posCol])
        {

            if (!static_cast<bool>(static_cast<DirectionFlags>(curDirection) & parent->validNeighbors))
            {
                curDirection <<= 1;
                continue;
            }
        
            //If the node in this direction is a valid neighbor
            //if (static_cast<bool>(parent->validNeighbors & static_cast<DirectionFlags>(i)))
            //{
                //Get the neighbor
                //NodeLogic::Node* neighbor = parent->GetNodeInDirection(static_cast<DirectionFlags>(i), nodeLogic);

                //If it hasn't been on the open or closed list yet
                if (static_cast<bool>(neighbor->curList & ListFlags::Neither))
                {
                    //Set its parent and push it onto the open list
                    neighbor->SetParent(parent, static_cast<DirectionFlags>(curDirection));

                    openList->Push(neighbor, request.settings.heuristic, GridPos{goalNode->posRow, goalNode->posCol}, request.settings.weight);

                    //Color it to reflect open status
                    if (request.settings.debugColoring)
                    {
                        terrain->set_color(GridPos{neighbor->posRow, neighbor->posCol}, Colors::Blue);
                    }
                }
                //If the neighbor is either on the open or closed list see if going this way is cheaper than when it was checked previously
                else if(neighbor->givenCost > neighbor->ComputeCost(parent, static_cast<DirectionFlags>(curDirection)))
                {

                    //If the neighbor is on the closed list
                    if ((request.settings.heuristic == Heuristic::MANHATTAN 
                        ||request.settings.heuristic == Heuristic::INCONSISTENT) 
                        &&static_cast<bool>(neighbor->curList & ListFlags::Closed))
                    {
                        //Set the parent and push it onto the open list
                        neighbor->SetParent(parent, static_cast<DirectionFlags>(curDirection));
                        openList->Push(neighbor, request.settings.heuristic, GridPos{ goalNode->posRow, goalNode->posCol }, request.settings.weight);

                        //Set color to reflect open status
                        if (request.settings.debugColoring)
                        {
                            terrain->set_color(GridPos{ neighbor->posRow, neighbor->posCol }, Colors::Blue);
                        }
                    }
                    //Neighbor is on open list
                    else
                    {
                        //Take it off the open list and push it back on with a new parent
                        //openList->Remove(neighbor, request.settings.heuristic, terrain->get_grid_position(request.goal), request.settings.weight);
                        neighbor->SetParent(parent, static_cast<DirectionFlags>(curDirection));
                        openList->Push(neighbor, request.settings.heuristic, GridPos{ goalNode->posRow, goalNode->posCol }, request.settings.weight);

                        
                    }
                    
                }
                
            //}
                curDirection <<= 1;
        }

        if (request.settings.singleStep == true)
        {
            return PathResult::PROCESSING;
        }
    }

    return PathResult::IMPOSSIBLE;
    
    // Just sample code, safe to delete
    /*
    GridPos start = terrain->get_grid_position(request.start);
    GridPos goal = terrain->get_grid_position(request.goal);
    terrain->set_color(start, Colors::Orange);
    terrain->set_color(goal, Colors::Orange);
    request.path.push_back(request.start);
    request.path.push_back(request.goal);
    return PathResult::COMPLETE;
    */
}

void AStarPather::PrecomputeCallback()
{
    nodeLogic->Precompute();
    
}
