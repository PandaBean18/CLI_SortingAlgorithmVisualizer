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

class InertionSortStep
{
    public: 
    string val = "InsertionSortStep";
    int index_current = 0, index_current_smallest = 0;

    string parentType()
    {
        return "InsertionSortStep";
    }
};

class FindSmallestNumber: public InertionSortStep
{
    public:


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
	vector<int> current = {5, 4, 3, 2, 1};
	SingleIterArray *left = nullptr, *right = nullptr;

    NextStep<T> nextStep;
    CompareAdjacentElements* compareAdjacentElements = nullptr;
    SwapAdjacentElements* swapAdjacentElements = nullptr;
    ReturnResult* returnResult = nullptr;
    
    public:
    int sorted = 0;
    int stateChanged = 0;

    vector<int> currentArrayState()
    {
        return current;
    }

    void initialize()
    {
        if (nextStep.next->parentType() == "BubbleSortStep") {
            compareAdjacentElements = new CompareAdjacentElements;
            swapAdjacentElements = new SwapAdjacentElements;
            returnResult = new ReturnResult;
        }
    }

    void setNextStep() 
    {
        nextStep.next = compareAdjacentElements;
    }

    void executeNextStep()
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
	SingleIterArray<BubbleSortStep> array;
    array.initialize();
    array.setNextStep();

    while (!array.sorted) {
        if (array.stateChanged)
        {
            array.printCurrentState();
        }
        array.executeNextStep();

    }
    return 0;

}
