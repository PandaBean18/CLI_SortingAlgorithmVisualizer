#include <iostream> 
#include <vector>
#include <string>

using namespace std;

class BubbleSortStep
{
    public:
    string val = "BubbleSortStep";
    int currentIndex = 0, sorted = 0;

    void setCurrentIndex(int i)
    {
        currentIndex = i;
    }

    virtual string stepVal() 
    {
        return val;
    }

    string parentType()
    {
        return "BubbleSortStep";
    }
};

class CompareAdjacentElements: public BubbleSortStep
{
	public:
	CompareAdjacentElements()
    {
        val = "CompareAdjacentElements";
    }
};

class SwapAdjacentElements: public BubbleSortStep
{
	public:
	SwapAdjacentElements()
    {
        val = "SwapAdjacentElements";
    }
};

class ReturnResult: public BubbleSortStep 
{
    public:

    ReturnResult()
    {
        val = "ReturnResult";
    }
};

class SelectionSortStep
{
    public: 
    string val = "SelectionSortStep";
    int index_current = 0, index_current_smallest = 0;

    string parentType()
    {
        return "SelectionSortStep";
    }

    virtual string stepVal()
    {
        return val;
    }
};

class FindSmallestNumber : public SelectionSortStep
{
    public:

    FindSmallestNumber(SelectionSortStep* parent)
    {
        val = "FindSmallestNumber";
        index_current = parent->index_current;
        index_current_smallest = parent->index_current_smallest;
    }

    // returns 1 if it found a smaller integer than integer at current index, else it returns 0
    // if it returns 1, then we have to swap the places of the smallest integer and current integer and then updated index_current_smallest to index_current
    int findSmallestNumber(vector<int>* arrayPtr)
    {
        int curr = (*arrayPtr)[index_current];
        int smallest = index_current;
        int updated = 0;
        for (int i = smallest; i < (*arrayPtr).size(); i++) {
            if ((*arrayPtr)[i] < curr) {
                curr = (*arrayPtr)[i];
                smallest = i;
                updated = 1;
            }
        }

        index_current_smallest = smallest;
        return updated;
    }

};

class SwapWithSmallestElement : public SelectionSortStep
{
    public:
    SwapWithSmallestElement(SelectionSortStep *parent)
    {
        val = "SwapWithSmallestElement";
        index_current = parent->index_current;
        index_current_smallest = parent->index_current_smallest;
    }

    void swap(vector<int>* arrayPtr)
    {
        int temp = (*arrayPtr)[index_current_smallest];
        (*arrayPtr)[index_current_smallest] = (*arrayPtr)[index_current];
        (*arrayPtr)[index_current] = temp;
        index_current++;
        index_current_smallest = index_current;
    }
};

class InsertionSortStep
{
    public: 
    int current_index = 1, swap_index = -1;
    string val = "InsertionSortStep";

    string parentType()
    {
        return "InsertionSortStep";
    }

    virtual string stepVal()
    {
        return val;
    }
};

class FindInsertionIndex: public InsertionSortStep
{
    public: 

    FindInsertionIndex(InsertionSortStep *parent)
    {
        val = "FindInsertionIndex";
        current_index = parent->current_index; 
        swap_index = parent->swap_index;
    }

    int findInsertionIndex(vector<int> *arrayPtr)
    {
        int current_val = (*arrayPtr)[current_index];
        int updated = 0;
        for (int i = 0; i < current_index; i++) {
            if ((*arrayPtr)[i] < current_val) {
                swap_index = i;
                updated = 1;
                break;
            }
        }
        return updated;
    }
};

class InsertVal: public InsertionSortStep
{
    public:
    InsertVal(InsertionSortStep *parent)
    {
        val = "InsertVal";
        current_index = parent->current_index;
        swap_index = parent->swap_index;
    }

    void insert(vector<int> *arrayPtr)
    {
        int temp = (*arrayPtr)[current_index];

        for (int i = current_index; i > swap_index; i--) {
            (*arrayPtr)[i] = (*arrayPtr)[i-1];
        }

        (*arrayPtr)[swap_index] = temp;
        current_index++;
        swap_index = -1;
    }
};

template <class T>
class NextStep
{
	
	public:
    T *next; 

    string type()
    {
        return next->stepVal();
	}

    template <typename U>
    NextStep<T> &operator=(const NextStep<U> &other)
    {
        next = other.next;
        return *this;
    }
};

template<class T>
class SingleIterArray
{
	vector<int> current = {7, 2, 8, 1, 6, 9, 5, 5};
	SingleIterArray *left = nullptr, *right = nullptr;

    T parentStep;
    NextStep<T> nextStep;
    CompareAdjacentElements* compareAdjacentElements = nullptr;
    SwapAdjacentElements* swapAdjacentElements = nullptr;
    ReturnResult* returnResult = nullptr;

    FindSmallestNumber* findSmallestNumber = nullptr; 
    SwapWithSmallestElement* swapWithSmallestElement = nullptr;

    FindInsertionIndex* findInsertionIndex = nullptr;
    InsertVal* insertVal = nullptr;

    
    public:
    int sorted = 0;
    int stateChanged = 0;

    vector<int> currentArrayState()
    {
        return current;
    }

    void initializeBubble()
    {
        // update to add error class in case T is not of bubblesort type
        if (is_same<T, BubbleSortStep>::value) {
            compareAdjacentElements = new CompareAdjacentElements;
            swapAdjacentElements = new SwapAdjacentElements;
            returnResult = new ReturnResult;
        }
    }

    void initializeSelect()
    {
        if (is_same<T,SelectionSortStep>::value) {
            findSmallestNumber = new FindSmallestNumber(&parentStep);
            swapWithSmallestElement = new SwapWithSmallestElement(&parentStep);
        }
    }

    void initializeInsert()
    {
        if (is_same<T, InsertionSortStep>::value) {
            findInsertionIndex = new FindInsertionIndex(&parentStep);
            insertVal = new InsertVal(&parentStep);
        }
    }

    void setNextStepBubble()
    {
        nextStep.next = compareAdjacentElements;
    }

    void setNextStepSelect()
    {
        nextStep.next = findSmallestNumber;
    }

    void setNextStepInsert()
    {
        nextStep.next = findInsertionIndex;
    }

    void executeNextStepBubble()
    {
        stateChanged = 0;
        if (nextStep.type() == "SwapAdjacentElements") {
            int temp = current[nextStep.next->currentIndex];
            current[nextStep.next->currentIndex] = current[nextStep.next->currentIndex+1];
            current[nextStep.next->currentIndex+1] = temp;
            int idx = nextStep.next->currentIndex+1;
            nextStep.next = compareAdjacentElements;
            nextStep.next->setCurrentIndex(idx);
            nextStep.next->sorted = 0;
            stateChanged = 1;
        } else if (nextStep.type() == "CompareAdjacentElements") {
            int i = nextStep.next->currentIndex;
            
            if (i == 0) {
                nextStep.next->sorted = 1;
            } else if (i >= (current.size()-1)) {
                if (nextStep.next->sorted) {
                    sorted = 1;
                } else if (nextStep.next->sorted ==  0) {
                    nextStep.next->sorted = 1;
                    nextStep.next->currentIndex = 0;

                }
                return;
            } 
            
            if (current[i] > current[i+1]) {
                nextStep.next = swapAdjacentElements;
                nextStep.next->setCurrentIndex(i);
            } else {
                nextStep.next->setCurrentIndex(i+1);
            }
        } else if (nextStep.type() == "ReturnResult") {
            sorted = 1;
        }
    }

    void executeNextStepSelect()
    {
        stateChanged = 0;
        if (findSmallestNumber->index_current == current.size()) {
            sorted = 1;
        } else if (nextStep.type() == "FindSmallestNumber") {
            int a = findSmallestNumber->findSmallestNumber(&current);

            if (a) {
                swapWithSmallestElement->index_current = findSmallestNumber->index_current;
                swapWithSmallestElement->index_current_smallest = findSmallestNumber->index_current_smallest;
                nextStep.next = swapWithSmallestElement;
            } else {
                findSmallestNumber->index_current++;
                findSmallestNumber->index_current_smallest = findSmallestNumber->index_current;
                swapWithSmallestElement->index_current++;
                swapWithSmallestElement->index_current_smallest = swapWithSmallestElement->index_current;
                nextStep.next = findSmallestNumber;
            }
            
        } else if (nextStep.type() == "SwapWithSmallestElement") {
            swapWithSmallestElement->swap(&current);
            findSmallestNumber->index_current = swapWithSmallestElement->index_current;
            findSmallestNumber->index_current_smallest = swapWithSmallestElement->index_current_smallest;
            nextStep.next = findSmallestNumber;
            stateChanged = 1;
        }
    }

    void executeNextStepInsert() 
    {
        stateChanged = 0;
        if (findInsertionIndex->current_index == current.size()) {
            sorted = 1;
        } else if (nextStep.type() == "FindInsertionIndex") {
            int a = findInsertionIndex->findInsertionIndex(&current);

            if (a) {
                nextStep.next = insertVal;
                insertVal->current_index = findInsertionIndex->current_index;
                insertVal->swap_index = findInsertionIndex->swap_index;
            } else {
                findInsertionIndex->current_index++;
                insertVal->current_index = findInsertionIndex->current_index;
            } 
        } else if (nextStep.type() == "InsertVal") {
            insertVal->insert(&current);
            findInsertionIndex->current_index = insertVal->current_index;
            findInsertionIndex->swap_index = insertVal->swap_index;
            nextStep.next = findInsertionIndex;
            stateChanged = 1;
        }
    }

    void printCurrentState() 
    {
        for (int i = 0; i < current.size(); i++) {
            cout << current[i] << " ";
        }
        cout << endl;
    }

};

int main()
{
	SingleIterArray<InsertionSortStep> array;
    array.initializeInsert();
    array.setNextStepInsert();
    array.printCurrentState();

    while (!array.sorted) {
        if (array.stateChanged)
        {
            array.printCurrentState();
        }
        array.executeNextStepInsert();

    }
    return 0;

}
