#ifndef RANGELIST_H
#define RANGELIST_H

// forward declaration
class NodeProxy;

class RangeList
{
public:
  /**
   * @brief Gets the value at position
   * 
   * @param position The index to get the value from
   * @return int The value found at position
   */
  int get(int position) const; // Done

  /**
   * @brief Inserts a given value at a given position
   * 
   * @param position The index to insert at
   * @param value The value to insert
   * @return int Success or Fail
   */
  int insert_value(int position, int value); //Done

  int insert_nonoverlapping_range(int begin, int end, int value); 
                                            // Done
  void printf_elements(char const *format, int dimension, bool do_sanity_check, bool indices = true) const; // Done

  void print_raw() const;                                                                                   // Done

  /**
   * @brief Construct a new Range List object
   * 
   */
  RangeList();

  /**
   * @brief Copy Construct a new Range List object
   * 
   * @param other The list to copy
   */
  RangeList(const RangeList& other);

  /**
   * @brief Destroy the Range List object
   * 
   */
  ~RangeList();

  friend class NodeProxy;

  /**
   * @brief Returns a proxy of the node at position
   * 
   * @param position The position to get
   * @return NodeProxy A proxy version of the node at position
   */
  NodeProxy operator[](unsigned int position);

  /**
   * @brief Returns a proxy of the node at position
   * 
   * @param position The position to get
   * @return NodeProxy A const proxy version of the node at position
   */
  NodeProxy operator[](unsigned int position) const;

  /**
   * @brief Sets one list equal to another
   * 
   * @param other The list to set to
   * @return RangeList& *this
   */
  RangeList& operator=(RangeList other);

  /**
   * @brief Essentially returns the dot product of two lists
   * 
   * @param other The list to dot product with
   * @return int The sum of the products of all indices within the lists
   */
  int operator*(RangeList& other);

  
  /**
   * @brief Makes a list that is the sum of the two given lists
   * 
   * @param other The list to add with
   * @return RangeList A new rangelist where each index is the sum
   * of the indices of the two given lists
   */
  RangeList operator+(RangeList& other);

private:
  struct RangeNode
  {
    int begin;
    int end;
    int value;
    struct RangeNode *next;

    void dump_node() const; // Done

    /**
     * @brief Construct a new Range Node object
     * 
     * @param b The beginning of the node
     * @param e The end of the node
     * @param v The value of the node
     */
    RangeNode(int b, int e, int v);
    
    /**
     * @brief Gets the length of a node
     * 
     * @return int The length i.e. end - begin
     */
    int Length();

    /**
     * @brief Whether a position is within a node
     * 
     * @param position The position in the list to check
     * @return true If the position is within the node
     * @return false If the position is not within the node
     */
    bool WithinRange(int position);
  };
  RangeNode *pHead;

private: // helpers
/**
 * @brief Handles all logic for creating a new node in the list,
 * if you use insert_value with a non-zero value this is called
 * 
 * @param position The position to insert at
 * @param value The value to insert
 * @return int Success or Fail
 */
  int InsertNode(int position, int value);

  /**
   * @brief Iterates through the list and merges any contiguous nodes
   * with the same value
   * 
   */
  void Merger();

  /**
   * @brief Called if we're inserting a value within an existing node
   * 
   * @param position The position to insert at
   * @param value The value to insert
   * @return int Success or Fail
   */
  int InsertSplit(int position, int value);

  /**
   * @brief Handles all logic for deleting a node within the list,
   * if you use insert_value with a zero value this is called
   * 
   * @param position The position to delete at
   * @return int Success or Fail
   */
  int DeleteNode(int position);

  /**
   * @brief This handles all logic for splitting an existing node
   * 
   * @param position The position to split down, example:
   * 1 [3, 6) when given position 4 becomes 1 [3, 4) -> 1 [5, 6)
   */
  void SplitNode(int position);

  /**
   * @brief Deletes all nodes in the list
   * 
   */
  void Clear();

  /**
   * @brief Gets the length of a list in index terms
   * 
   * @return int The end value of the final node in the list
   */
  int ListLength();
};

class NodeProxy
{
  public:
  /**
   * @brief Construct a new Node Proxy object
   * 
   * @param ref A reference to the RangeList
   * @param position The position of this proxy
   */
  NodeProxy(RangeList *ref, int position);

  /**
   * @brief Construct a new const Node Proxy object
   * 
   * @param ref A const reference to the RangeList
   * @param position The position of this proxy
   */
  NodeProxy(const RangeList *ref, int position);

  /**
   * @brief Changes the value at the Proxy's position
   * 
   * @param value The value to set to
   * @return NodeProxy& A reference to this
   */
  NodeProxy& operator=(int value);

  /**
   * @brief Multiplies the value at the Proxy's position by value
   * 
   * @param value The value to multiply by
   * @return RangeList* Just returns listReference
   */
  RangeList* operator*=(int value);

  /**
   * @brief Sums the value at the Proxy's position with value
   * 
   * @param value The value to add to
   * @return RangeList* Just returns listReference
   */
  RangeList* operator+=(int value);

  /**
   * @brief Used for casting a Proxy to an int
   * 
   * @return int The value at the Proxy's position
   */
  operator int() const;

  private:
  /**
   * @brief A reference to the RangeList this Proxy is a part of
   * 
   */
  RangeList* listReference;
  const RangeList* constReference;

  /**
   * @brief The position of this Proxy
   * 
   */
  int position;
  
};
#endif
