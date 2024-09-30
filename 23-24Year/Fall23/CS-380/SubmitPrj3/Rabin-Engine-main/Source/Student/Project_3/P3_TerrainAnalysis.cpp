#include <pch.h>
#include "Terrain/TerrainAnalysis.h"
#include "Terrain/MapMath.h"
#include "Agent/AStarAgent.h"
#include "Terrain/MapLayer.h"
#include "Projects/ProjectThree.h"

#include <iostream>
#include <limits>

static const float sqrt2 = 1.41421356237f;

bool ProjectThree::implemented_fog_of_war() const // extra credit
{
    return false;
}

float distance_to_closest_wall(int row, int col) //Done
{
    /*
        Check the euclidean distance from the given cell to every other wall cell,
        with cells outside the map bounds treated as walls, and return the smallest
        distance.  Make use of the is_valid_grid_position and is_wall member
        functions in the global terrain to determine if a cell is within map bounds
        and a wall, respectively.
    */

    float lowest = std::numeric_limits<float>::max();

    for (int i = -1; i < terrain->get_map_width() + 1; i++)
    {
        for (int j = -1; j < terrain->get_map_height() + 1; j++)
        {
            if (!terrain->is_valid_grid_position({i,j}) || terrain->is_wall({i,j}))
            {
                if ((sqrtf((powf(static_cast<float>(abs(row - static_cast<int>(i))), 2.0f) + 
                    powf(static_cast<float>(abs(col - static_cast<int>(j))), 2.0f)))) < lowest)
                {
                    lowest = (sqrtf((powf(static_cast<float>(abs(row - static_cast<int>(i))), 2.0f) +
                        powf(static_cast<float>(abs(col - static_cast<int>(j))), 2.0f))));
                }
            }
        }
    }

    // WRITE YOUR CODE HERE
    
    return lowest; // REPLACE THIS
}

bool is_clear_path(int row0, int col0, int row1, int col1) //Done
{
    /*
        Two cells (row0, col0) and (row1, col1) are visible to each other if a line
        between their centerpoints doesn't intersect the four boundary lines of every
        wall cell.  You should puff out the four boundary lines by a very tiny amount
        so that a diagonal line passing by the corner will intersect it.  Make use of the
        line_intersect helper function for the intersection test and the is_wall member
        function in the global terrain to determine if a cell is a wall or not.
    */

    Vec2 point1 = { static_cast<float>(row0), static_cast<float>(col0) };
    Vec2 point2 = { static_cast<float>(row1), static_cast<float>(col1) };

    for (int i = std::min(row0, row1); i < std::max(row0, row1) + 1; i++)
    {
        for (int j = std::min(col0, col1); j < std::max(col0, col1) + 1; j++)
        {
            if (terrain->is_wall(i,j))
            {
                Vec2 topLeft = { i - 0.51f, j + 0.51f };
                Vec2 topRight = { i + 0.51f, j + 0.51f };
                Vec2 bottomLeft = { i - 0.51f, j - 0.51f };
                Vec2 bottomRight = { i + 0.51f, j - 0.51f };
                if (line_intersect(point1, point2, topLeft, topRight) || 
                    line_intersect(point1, point2, topRight, bottomRight) || 
                    line_intersect(point1, point2, bottomRight, bottomLeft) || 
                    line_intersect(point1, point2, bottomLeft, topLeft))
                {
                    return false;
                }
            }
        }
    }

    // WRITE YOUR CODE HERE

    return true; // REPLACE THIS
}

void analyze_openness(MapLayer<float> &layer) //Done
{
    /*
        Mark every cell in the given layer with the value 1 / (d * d),
        where d is the distance to the closest wall or edge.  Make use of the
        distance_to_closest_wall helper function.  Walls should not be marked.
    */

    for (int i = 0; i < terrain->get_map_width(); i++)
    {
        for (int j = 0; j < terrain->get_map_height(); j++)
        {
            if (!terrain->is_wall(i,j))
            {
                layer.set_value({ i,j }, 1.0f / (powf(distance_to_closest_wall(i, j), 2.0f)));
            }
        }
    }

    // WRITE YOUR CODE HERE
}

void analyze_visibility(MapLayer<float> &layer) //Done
{
    /*
        Mark every cell in the given layer with the number of cells that
        are visible to it, divided by 160 (a magic number that looks good).  Make sure
        to cap the value at 1.0 as well.

        Two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
        helper function.
    */

    int numVisible[40][40] = { 0 };

    for (int i = 0; i < terrain->get_map_width(); i++)
    {
        for (int j = 0; j < terrain->get_map_height(); j++)
        {
            
            if (!terrain->is_wall(i, j))
            {

                for (int k = i; k < terrain->get_map_width(); k++)
                {
                    for (int l = 0; l < terrain->get_map_height(); l++)
                    {
                        if (i != k || j != l)
                        {
                            if (is_clear_path(i, j, k, l))
                            {
                                numVisible[i][j]++;
                                numVisible[k][l]++;
                            }
                        }
                    }
                }
            }

            layer.set_value({ i, j }, std::fmin(static_cast<float>(numVisible[i][j]) / 160.0f, 1.0f));
        }
    }

    // WRITE YOUR CODE HERE
}

void analyze_visible_to_cell(MapLayer<float> &layer, int row, int col) //Done
{
    /*
        For every cell in the given layer mark it with 1.0
        if it is visible to the given cell, 0.5 if it isn't visible but is next to a visible cell,
        or 0.0 otherwise.

        Two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
        helper function.
    */

    for (int i = 0; i < terrain->get_map_width(); i++)
    {
        for (int j = 0; j < terrain->get_map_height(); j++)
        {
            if (!terrain->is_wall(i, j) && is_clear_path(row, col, i, j))
            {
                layer.set_value({ i, j }, 1.0f);
            }
            else if (!terrain->is_wall(i, j))
            {
                layer.set_value(i, j, 0.0f);
            }
        }
    }

    for (int i = 0; i < terrain->get_map_width(); i++)
    {
        for (int j = 0; j < terrain->get_map_height(); j++)
        {
            if (layer.get_value(i,j) == 1.0f)
            {
                for (int k = 0; k < 8; k++)
                {
                    switch (k)
                    {
                    case 0:
                        if (terrain->is_valid_grid_position(i + 1, j)
                            && !terrain->is_wall(i + 1, j)
                            && layer.get_value(i + 1, j) != 1.0f
                            && is_clear_path(i, j, i + 1, j))
                        {
                            layer.set_value(i + 1, j, 0.5f);
                        }
                        break;
                    case 1:
                        if (terrain->is_valid_grid_position(i + 1, j + 1)
                        && !terrain->is_wall(i + 1, j + 1)
                        && layer.get_value(i + 1, j + 1) != 1.0f
                        && is_clear_path(i, j, i + 1, j + 1))
                    {
                        layer.set_value(i + 1, j + 1, 0.5f);
                    }
                          break;
                    case 2:
                        if (terrain->is_valid_grid_position(i, j + 1)
                            && !terrain->is_wall(i, j + 1)
                            && layer.get_value(i, j + 1) != 1.0f
                            && is_clear_path(i, j, i, j + 1))
                        {
                            layer.set_value(i, j + 1, 0.5f);
                        }
                        break;
                    case 3:
                        if ((terrain->is_valid_grid_position(i - 1, j + 1)
                            && !terrain->is_wall(i - 1, j + 1))
                            && layer.get_value(i - 1, j + 1) != 1.0f
                            && is_clear_path(i, j, i - 1, j + 1))
                        {
                            layer.set_value(i - 1, j + 1, 0.5f);
                        }
                        break;
                    case 4:
                        if (terrain->is_valid_grid_position(i - 1, j)
                            && !terrain->is_wall(i - 1, j)
                            && layer.get_value(i - 1, j) != 1.0f
                            && is_clear_path(i, j, i - 1, j))
                        {
                            layer.set_value(i - 1, j, 0.5f);
                        }
                        break;
                    case 5:
                        if (terrain->is_valid_grid_position(i - 1, j - 1)
                            && !terrain->is_wall(i - 1, j - 1)
                            && layer.get_value(i - 1, j - 1) != 1.0f
                            && is_clear_path(i, j, i - 1, j - 1))
                        {
                            layer.set_value(i - 1, j - 1, 0.5f);
                        }
                        break;
                    case 6:
                        if (terrain->is_valid_grid_position(i, j - 1)
                            && !terrain->is_wall(i, j - 1)
                            && layer.get_value(i, j - 1) != 1.0f
                            && is_clear_path(i, j, i, j - 1))
                        {
                            layer.set_value(i, j - 1, 0.5f);
                        }
                        break;
                    case 7:
                        if (terrain->is_valid_grid_position(i + 1, j - 1)
                            && !terrain->is_wall(i + 1, j - 1)
                            && layer.get_value(i + 1, j - 1) != 1.0f
                            && is_clear_path(i, j, i + 1, j - 1))
                        {
                            layer.set_value(i + 1, j - 1, 0.5f);
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
            
        }
    
    }
    

    // WRITE YOUR CODE HERE
}

void analyze_agent_vision(MapLayer<float> &layer, const Agent *agent) //Done
{
    /*
        For every cell in the given layer that is visible to the given agent,
        mark it as 1.0, otherwise don't change the cell's current value.

        You must consider the direction the agent is facing.  All of the agent data is
        in three dimensions, but to simplify you should operate in two dimensions, the XZ plane.

        Take the dot product between the view vector and the vector from the agent to the cell,
        both normalized, and compare the cosines directly instead of taking the arccosine to
        avoid introducing floating-point inaccuracy (larger cosine means smaller angle).

        Give the agent a field of view slighter larger than 180 degrees.

        Two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
        helper function.
    */

    Vec3 forVec = agent->get_forward_vector();
    forVec.y = 0;
    Vec3 posVec = agent->get_position();
    posVec.y = 0;
    
    GridPos agentPos = terrain->get_grid_position(posVec);

    float fovCos = cosf((PI + 0.1) / 2.0f);

    forVec.Normalize();

    for (int i = 0; i < terrain->get_map_width(); i++)
    {
        for (int j = 0; j < terrain->get_map_height(); j++)
        {
            Vec3 worldCell = terrain->get_world_position(i, j);
            Vec3 toCell = worldCell - posVec;
            toCell.Normalize();

            float cos = toCell.Dot(forVec);

            if (cos >= fovCos)
            {
                if (is_clear_path(agentPos.row, agentPos.col, i, j))
                {
                    layer.set_value(i, j, 1.0f);
                }
            }
        }
    }

    // WRITE YOUR CODE HERE
}

void propagate_solo_occupancy(MapLayer<float> &layer, float decay, float growth) //Done
{
    /*
        For every cell in the given layer:

            1) Get the value of each neighbor and apply decay factor
            2) Keep the highest value from step 1
            3) Linearly interpolate from the cell's current value to the value from step 2
               with the growing factor as a coefficient.  Make use of the lerp helper function.
            4) Store the value from step 3 in a temporary layer.
               A float[40][40] will suffice, no need to dynamically allocate or make a new MapLayer.

        After every cell has been processed into the temporary layer, write the temporary layer into
        the given layer;
    */


    float tempLayer[40][40];

    for (int i = 0; i < terrain->get_map_width(); i++)
    {
        for (int j = 0; j < terrain->get_map_height(); j++)
        {
            float highestValue = 0.0f;
            for (int k = 0; k < 8; k++)
            {
                switch (k)
                {
                case 0:
                    if (terrain->is_valid_grid_position(i + 1, j)
                        && !terrain->is_wall(i + 1, j)
                        && is_clear_path(i, j, i + 1, j))
                    {
                        float decayValue = layer.get_value(i + 1, j) * expf(-1 * decay);

                        if (decayValue > highestValue)
                        {
                            highestValue = decayValue;
                        }
                        
                    }
                    break;
                case 1:if (terrain->is_valid_grid_position(i + 1, j + 1)
                    && !terrain->is_wall(i + 1, j + 1)
                    && is_clear_path(i, j, i + 1, j + 1))
                {
                    float decayValue = layer.get_value(i + 1, j + 1) * expf(-sqrt2 * decay);

                    if (decayValue > highestValue)
                    {
                        highestValue = decayValue;
                    }
                }
                      break;
                case 2:
                    if (terrain->is_valid_grid_position(i, j + 1)
                        && !terrain->is_wall(i, j + 1)
                        && is_clear_path(i, j, i, j + 1))
                    {
                        float decayValue = layer.get_value(i, j + 1) * expf(-1 * decay);

                        if (decayValue > highestValue)
                        {
                            highestValue = decayValue;
                        }
                    }
                    break;
                case 3:
                    if ((terrain->is_valid_grid_position(i - 1, j + 1)
                        && !terrain->is_wall(i - 1, j + 1))
                        && is_clear_path(i, j, i - 1, j + 1))
                    {
                        float decayValue = layer.get_value(i - 1, j + 1) * expf(-sqrt2 * decay);

                        if (decayValue > highestValue)
                        {
                            highestValue = decayValue;
                        }
                    }
                    break;
                case 4:
                    if (terrain->is_valid_grid_position(i - 1, j)
                        && !terrain->is_wall(i - 1, j)
                        && is_clear_path(i, j, i - 1, j))
                    {
                        float decayValue = layer.get_value(i - 1, j) * expf(-1 * decay);

                        if (decayValue > highestValue)
                        {
                            highestValue = decayValue;
                        }
                    }
                    break;
                case 5:
                    if (terrain->is_valid_grid_position(i - 1, j - 1)
                        && !terrain->is_wall(i - 1, j - 1)
                        && is_clear_path(i, j, i - 1, j - 1))
                    {
                        float decayValue = layer.get_value(i - 1, j - 1) * expf(-sqrt2 * decay);

                        if (decayValue > highestValue)
                        {
                            highestValue = decayValue;
                        }
                    }
                    break;
                case 6:
                    if (terrain->is_valid_grid_position(i, j - 1)
                        && !terrain->is_wall(i, j - 1)
                        && is_clear_path(i, j, i, j - 1))
                    {
                        float decayValue = layer.get_value(i, j - 1) * expf(-1 * decay);

                        if (decayValue > highestValue)
                        {
                            highestValue = decayValue;
                        }
                    }
                    break;
                case 7:
                    if (terrain->is_valid_grid_position(i + 1, j - 1)
                        && !terrain->is_wall(i + 1, j - 1)
                        && is_clear_path(i, j, i + 1, j - 1))
                    {
                        float decayValue = layer.get_value(i + 1, j - 1) * expf(-sqrt2 * decay);

                        if (decayValue > highestValue)
                        {
                            highestValue = decayValue;
                        }
                    }
                    break;
                default:
                    break;
                }
            }

            float newValue = lerp(layer.get_value(i, j), highestValue, growth);
            tempLayer[i][j] = newValue;
        }
    }

    for (int i = 0; i < terrain->get_map_width(); i++)
    {
        for (int j = 0; j < terrain->get_map_height(); j++)
        {
            layer.set_value(i, j, tempLayer[i][j]);
        }
    }
    
    // WRITE YOUR CODE HERE
}

void propagate_dual_occupancy(MapLayer<float> &layer, float decay, float growth) //Done
{
    /*
        Similar to the solo version, but the values range from -1.0 to 1.0, instead of 0.0 to 1.0

        For every cell in the given layer:

        1) Get the value of each neighbor and apply decay factor
        2) Keep the highest ABSOLUTE value from step 1
        3) Linearly interpolate from the cell's current value to the value from step 2
           with the growing factor as a coefficient.  Make use of the lerp helper function.
        4) Store the value from step 3 in a temporary layer.
           A float[40][40] will suffice, no need to dynamically allocate or make a new MapLayer.

        After every cell has been processed into the temporary layer, write the temporary layer into
        the given layer;
    */

    float tempLayer[40][40];

    for (int i = 0; i < terrain->get_map_width(); i++)
    {
        for (int j = 0; j < terrain->get_map_height(); j++)
        {
            float highestValue = 0.0f;
            bool isNegativeValue = false;
            for (int k = 0; k < 8; k++)
            {
                switch (k)
                {
                case 0:
                    if (terrain->is_valid_grid_position(i + 1, j)
                        && !terrain->is_wall(i + 1, j)
                        && is_clear_path(i, j, i + 1, j))
                    {
                        float decayValue = layer.get_value(i + 1, j) * expf(-1 * decay);

                        if (fabsf(decayValue) > highestValue)
                        {
                            highestValue = fabsf(decayValue);

                            if (decayValue >= 0.0f)
                            {
                                isNegativeValue = false;
                            }
                            else
                            {
                                isNegativeValue = true;
                            }
                        }

                    }
                    break;
                case 1:
                    if (terrain->is_valid_grid_position(i + 1, j + 1)
                    && !terrain->is_wall(i + 1, j + 1)
                    && is_clear_path(i, j, i + 1, j + 1))
                {
                    float decayValue = layer.get_value(i + 1, j + 1) * expf(-sqrt2 * decay);

                    if (fabsf(decayValue) > highestValue)
                    {
                        highestValue = fabsf(decayValue);

                        if (decayValue >= 0.0f)
                        {
                            isNegativeValue = false;
                        }
                        else
                        {
                            isNegativeValue = true;
                        }
                    }
                }
                      break;
                case 2:
                    if (terrain->is_valid_grid_position(i, j + 1)
                        && !terrain->is_wall(i, j + 1)
                        && is_clear_path(i, j, i, j + 1))
                    {
                        float decayValue = layer.get_value(i, j + 1) * expf(-1 * decay);

                        if (fabsf(decayValue) > highestValue)
                        {
                            highestValue = fabsf(decayValue);

                            if (decayValue >= 0.0f)
                            {
                                isNegativeValue = false;
                            }
                            else
                            {
                                isNegativeValue = true;
                            }
                        }
                    }
                    break;
                case 3:
                    if ((terrain->is_valid_grid_position(i - 1, j + 1)
                        && !terrain->is_wall(i - 1, j + 1))
                        && is_clear_path(i, j, i - 1, j + 1))
                    {
                        float decayValue = layer.get_value(i - 1, j + 1) * expf(-sqrt2 * decay);

                        if (fabsf(decayValue) > highestValue)
                        {
                            highestValue = fabsf(decayValue);

                            if (decayValue >= 0.0f)
                            {
                                isNegativeValue = false;
                            }
                            else
                            {
                                isNegativeValue = true;
                            }
                        }
                    }
                    break;
                case 4:
                    if (terrain->is_valid_grid_position(i - 1, j)
                        && !terrain->is_wall(i - 1, j)
                        && is_clear_path(i, j, i - 1, j))
                    {
                        float decayValue = layer.get_value(i - 1, j) * expf(-1 * decay);

                        if (fabsf(decayValue) > highestValue)
                        {
                            highestValue = fabsf(decayValue);

                            if (decayValue >= 0.0f)
                            {
                                isNegativeValue = false;
                            }
                            else
                            {
                                isNegativeValue = true;
                            }
                        }
                    }
                    break;
                case 5:
                    if (terrain->is_valid_grid_position(i - 1, j - 1)
                        && !terrain->is_wall(i - 1, j - 1)
                        && is_clear_path(i, j, i - 1, j - 1))
                    {
                        float decayValue = layer.get_value(i - 1, j - 1) * expf(-sqrt2 * decay);

                        if (fabsf(decayValue) > highestValue)
                        {
                            highestValue = fabsf(decayValue);

                            if (decayValue >= 0.0f)
                            {
                                isNegativeValue = false;
                            }
                            else
                            {
                                isNegativeValue = true;
                            }
                        }
                    }
                    break;
                case 6:
                    if (terrain->is_valid_grid_position(i, j - 1)
                        && !terrain->is_wall(i, j - 1)
                        && is_clear_path(i, j, i, j - 1))
                    {
                        float decayValue = layer.get_value(i, j - 1) * expf(-1 * decay);

                        if (fabsf(decayValue) > highestValue)
                        {
                            highestValue = fabsf(decayValue);

                            if (decayValue >= 0.0f)
                            {
                                isNegativeValue = false;
                            }
                            else
                            {
                                isNegativeValue = true;
                            }
                        }
                    }
                    break;
                case 7:
                    if (terrain->is_valid_grid_position(i + 1, j - 1)
                        && !terrain->is_wall(i + 1, j - 1)
                        && is_clear_path(i, j, i + 1, j - 1))
                    {
                        float decayValue = layer.get_value(i + 1, j - 1) * expf(-sqrt2 * decay);

                        if (fabsf(decayValue) > highestValue)
                        {
                            highestValue = fabsf(decayValue);

                            if (decayValue >= 0.0f)
                            {
                                isNegativeValue = false;
                            }
                            else
                            {
                                isNegativeValue = true;
                            }
                        }
                    }
                    break;
                default:
                    break;
                }
            }

            if (isNegativeValue)
            {
                highestValue = -highestValue;
            }
            float newValue = lerp(layer.get_value(i, j), highestValue, growth);
            tempLayer[i][j] = newValue;
        }
    }

    for (int i = 0; i < terrain->get_map_width(); i++)
    {
        for (int j = 0; j < terrain->get_map_height(); j++)
        {
            layer.set_value(i, j, tempLayer[i][j]);
        }
    }

    // WRITE YOUR CODE HERE
}

void normalize_solo_occupancy(MapLayer<float> &layer) //Done
{
    /*
        Determine the maximum value in the given layer, and then divide the value
        for every cell in the layer by that amount.  This will keep the values in the
        range of [0, 1].  Negative values should be left unmodified.
    */

    float maximumValue = 0.0f;

    for (int i = 0; i < terrain->get_map_width(); i++)
    {
        for (int j = 0; j < terrain->get_map_height(); j++)
        {
            if (layer.get_value(i, j) > maximumValue)
            {
                maximumValue = layer.get_value(i, j);
            }
        }
    }

    if (maximumValue != 0.0f)
    {

    for (int i = 0; i < terrain->get_map_width(); i++)
    {
        for (int j = 0; j < terrain->get_map_height(); j++)
        {
            if (layer.get_value(i, j) >= 0.0f)
            {
                float newValue = layer.get_value(i, j) / maximumValue;
                layer.set_value(i, j, newValue);
            }
        }
    }
    }

    // WRITE YOUR CODE HERE
}

void normalize_dual_occupancy(MapLayer<float> &layer) //Done
{
    /*
        Similar to the solo version, but you need to track greatest positive value AND 
        the least (furthest from 0) negative value.

        For every cell in the given layer, if the value is currently positive divide it by the
        greatest positive value, or if the value is negative divide it by -1.0 * the least negative value
        (so that it remains a negative number).  This will keep the values in the range of [-1, 1].
    */

    float maximumValue = 0.0f;
    float minimumValue = 0.0f;

    for (int i = 0; i < terrain->get_map_width(); i++)
    {
        for (int j = 0; j < terrain->get_map_height(); j++)
        {
            if (layer.get_value(i, j) > maximumValue)
            {
                maximumValue = layer.get_value(i, j);
            }
            else if (layer.get_value(i, j) < minimumValue)
            {
                minimumValue = layer.get_value(i, j);
            }
        }
    }

    

        for (int i = 0; i < terrain->get_map_width(); i++)
        {
            for (int j = 0; j < terrain->get_map_height(); j++)
            {
                if (layer.get_value(i, j) >= 0.0f && maximumValue != 0.0f)
                {
                    float newValue = layer.get_value(i, j) / maximumValue;
                    layer.set_value(i, j, newValue);
                }
                else if (layer.get_value(i, j) < 0.0f && minimumValue != 0.0f)
                {
                    float newValue = layer.get_value(i, j) / minimumValue * -1.0f;
                    layer.set_value(i, j, newValue);
                }

            }
        }
    

    // WRITE YOUR CODE HERE
}

void enemy_field_of_view(MapLayer<float> &layer, float fovAngle, float closeDistance, float occupancyValue, AStarAgent *enemy) //Maybe
{
    /*
        First, clear out the old values in the map layer by setting any negative value to 0.
        Then, for every cell in the layer that is within the field of view cone, from the
        enemy agent, mark it with the occupancy value.  Take the dot product between the view
        vector and the vector from the agent to the cell, both normalized, and compare the
        cosines directly instead of taking the arccosine to avoid introducing floating-point
        inaccuracy (larger cosine means smaller angle).

        If the tile is close enough to the enemy (less than closeDistance),
        you only check if it's visible to enemy.  Make use of the is_clear_path
        helper function.  Otherwise, you must consider the direction the enemy is facing too.
        This creates a radius around the enemy that the player can be detected within, as well
        as a fov cone.
    */



    for (int i = 0; i < terrain->get_map_width(); i++)
    {
        for (int j = 0; j < terrain->get_map_height(); j++)
        {
            if (layer.get_value(i,j) < 0.0f)
            {
                layer.set_value(i, j, 0.0f);
            }
        }
    }

    Vec3 forVec = enemy->get_forward_vector();
    forVec.y = 0;
    Vec3 posVec = enemy->get_position();
    posVec.y = 0;

    GridPos agentPos = terrain->get_grid_position(posVec);

    float fovCos = cosf((fovAngle * (PI/180.0f))/ 2.0f);

    forVec.Normalize();

    for (int i = 0; i < terrain->get_map_width(); i++)
    {
        for (int j = 0; j < terrain->get_map_height(); j++)
        {
            Vec3 worldCell = terrain->get_world_position(i, j);
            Vec3 toCell = worldCell - posVec;
            toCell.Normalize();

            float cos = toCell.Dot(forVec);

            if (cos >= fovCos)
            {
                if (is_clear_path(agentPos.row, agentPos.col, i, j))
                {
                    layer.set_value(i, j, occupancyValue);
                }
            }

            if (powf(closeDistance, 2.0f) > powf(static_cast<float>(agentPos.row - i), 2.0f) + powf(static_cast<float>(agentPos.col - j), 2.0f)
                && is_clear_path(agentPos.row, agentPos.col, i, j))
            {
                layer.set_value(i, j, occupancyValue);
            }
        }
    }

    // WRITE YOUR CODE HERE
}

bool enemy_find_player(MapLayer<float> &layer, AStarAgent *enemy, Agent *player)
{
    /*
        Check if the player's current tile has a negative value, ie in the fov cone
        or within a detection radius.
    */

    const auto &playerWorldPos = player->get_position();

    const auto playerGridPos = terrain->get_grid_position(playerWorldPos);

    // verify a valid position was returned
    if (terrain->is_valid_grid_position(playerGridPos) == true)
    {
        if (layer.get_value(playerGridPos) < 0.0f)
        {
            return true;
        }
    }

    // player isn't in the detection radius or fov cone, OR somehow off the map
    return false;
}

bool enemy_seek_player(MapLayer<float> &layer, AStarAgent *enemy) //Done
{
    /*
        Attempt to find a cell with the highest nonzero value (normalization may
        not produce exactly 1.0 due to floating point error), and then set it as
        the new target, using enemy->path_to.

        If there are multiple cells with the same highest value, then pick the
        cell closest to the enemy.

        Return whether a target cell was found.
    */

    float maximumValue = 0.0f;
    GridPos maxCell = { 0,0};

    GridPos enemyPos = terrain->get_grid_position(enemy->get_position());

    for (int i = 0; i < terrain->get_map_width(); i++)
    {
        for (int j = 0; j < terrain->get_map_height(); j++)
        {
            if (layer.get_value(i,j) > maximumValue)
            {
                maximumValue = layer.get_value(i, j);
                maxCell = { i,j };
            }
            else if (layer.get_value(i, j) == maximumValue 
                && powf(static_cast<float>(i - enemyPos.row), 2.0f) + powf(static_cast<float>(j - enemyPos.col), 2.0f) <
                powf(static_cast<float>(maxCell.row - enemyPos.row), 2.0f) + powf(static_cast<float>(maxCell.col - enemyPos.col), 2.0f))
            {
                maxCell = { i,j };
            }
        }
    }

    if (maximumValue != 0.0f)
    {
        enemy->path_to(terrain->get_world_position(maxCell));
        return true;
    }

    // WRITE YOUR CODE HERE

    return false; // REPLACE THIS
}
