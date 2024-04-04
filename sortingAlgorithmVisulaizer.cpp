#include <iostream> 
#include <vector>
#include <string>

using namespace std;

class BubbleSortStep
{
    public:
    string val = "BubbleSortStep";
    int currentIndex = 0;

    void setCurrentIndex(int i)
    {
        currentIndex = i;
    }

    virtual string stepVal() 
    {
        return val;
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

class SingleIterArray
{
	vector<int> current = {5, 1, 3, 8, 7};
	SingleIterArray *left, *right;
	CompareAdjacentElements* compareAdjacentElements = new CompareAdjacentElements;
    SwapAdjacentElements* swapAdjacentElements = new SwapAdjacentElements;
    ReturnResult* returnResult = new ReturnResult;
    
    public:
    int sorted = 0;
    NextStep<BubbleSortStep> nextStep;
    vector<int> currentArrayState()
    {
        return current;
    }

    void setNextStep() 
    {
        nextStep.next = compareAdjacentElements;
    }

    void executeNextStep()
    {
        if (nextStep.type() == "SwapAdjacentElements") {
            int temp = current[nextStep.next->currentIndex];
            current[nextStep.next->currentIndex] = current[nextStep.next->currentIndex+1];
            current[nextStep.next->currentIndex+1] = temp;
            int idx = nextStep.next->currentIndex+1;
            nextStep.next = compareAdjacentElements;
            nextStep.next->setCurrentIndex(idx);
        } else if (nextStep.type() == "CompareAdjacentElements") {
            int i = nextStep.next->currentIndex;
            if (i >= (current.size()-1)) {
                nextStep.next = returnResult;
            } else if (current[i] > current[i+1]) {
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
	SingleIterArray array;
    array.setNextStep();

    while (!array.sorted) {
        array.printCurrentState();
        array.executeNextStep();

    }
    return 0;

}
