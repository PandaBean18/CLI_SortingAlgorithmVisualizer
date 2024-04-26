#include <iostream> 
#include <vector>
#include <string>
#include <unistd.h>
#include <stdlib.h>

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
            if ((*arrayPtr)[i] > current_val) {
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

class QuickSortStep
{
    public:
    vector<vector<int>> current_array = {};

    string val = "QuickSortStep";

    string parentType()
    {
        return "QucikSortStep";
    }

    virtual string stepVal()
    {
        return val;
    }
};

class DivideArray: public QuickSortStep
{
    public:
    DivideArray(QuickSortStep* parent)
    {
        val = "DivideArray";
        current_array = parent->current_array;
    }

    int divisionPossible()
    {
        for (int i = 0; i < current_array.size(); i++) {
            if (current_array[i].size() >= 3) {
                return 1;
            }
        }
        return 0;
    }

    void divideArray()
    {
        vector<vector<int>> new_array = {};

        for(int i = 0; i < current_array.size(); i++) {
            if (current_array[i].size() >= 3) {
                vector<int> pivot = {current_array[i][0]};
                vector<int> left = {};
                vector<int> right = {};

                for (int j = 1; j < current_array[i].size(); j++) {
                    int ele = current_array[i][j];

                    if (ele < pivot[0]) {
                        left.push_back(ele);
                    } else if(ele > pivot[0]) {
                        right.push_back(ele);
                    } else {
                        pivot.push_back(ele);
                    }

                }

                new_array.push_back(left);
                new_array.push_back(pivot);
                new_array.push_back(right);

            } else {
                new_array.push_back(current_array[i]);
            }
        }
        current_array = new_array;
    }
};

class SortIndividualBlocks: public QuickSortStep
{
    public:
    
    SortIndividualBlocks(QuickSortStep *parent)
    {
        val = "SortIndividualBlocks";
        current_array = parent->current_array;
    }

    int max(vector<int> arr) 
    {
        int m = arr[0];

        for (int i=1; i < arr.size(); i++) {
            if (arr[i] > m) {
                m = arr[i];
            }
        }

        return m;
    }

    int min(vector<int> arr)
    {
        int m = arr[0];

        for (int i = 1; i < arr.size(); i++) {
            if (arr[i] < m) {
                m = arr[i];
            }
        }

        return m;
    }

    // returning 1 indicates that array was updated
    int sort() {
        for (int i = 0; i < current_array.size(); i++) {
            vector<int> curr = current_array[i];

            if (curr.size() == 2) {
                int small = min(curr);
                int big = max(curr);
                curr[0] = small;
                curr[1] = big;
                if (current_array[i] != curr) {
                    current_array[i] = curr;
                    return 1;
                }
            }
        }
        return 0;
    }
};

class MergeArray: public QuickSortStep
{
    public: 
    MergeArray(QuickSortStep *parent) 
    {
        val = "MergeArray";
        current_array = parent->current_array;
    }

    vector<int> merge() 
    {
        vector<int> return_array = {};

        for(int i = 0; i < current_array.size(); i++) {
            for (int j = 0; j < current_array[i].size(); j++) {
                return_array.push_back(current_array[i][j]);
            }
        }

        return return_array;
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

class SorterError
{
    public:
    string errorMessage = "An error occured";

    virtual void printError() {
        cout << errorMessage << endl;
    }
};

template <class T, class U>
class IncompatibleTypeInitialization: public SorterError
{
    public:
    
    IncompatibleTypeInitialization(T *expectedClass, U *givenClass)
    {
        errorMessage = format("SingleIterArray initialized with wrong sorting algorithm. Expected {} and received {}", expectedClass->val, givenClass->val);
    }
};

template<class T>
class SingleIterArray
{
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

    DivideArray* divideArray = nullptr; 
    SortIndividualBlocks* sortIndividualBlocks = nullptr;
    MergeArray* mergeArray = nullptr;

    
    public:
    int sorted = 0;
    int stateChanged = 0;
    vector<int> current = {};

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

    void initializeQuick()
    {
        if (is_same<T, QuickSortStep>::value) {
            parentStep.current_array.push_back(current);
            divideArray = new DivideArray(&parentStep);
            sortIndividualBlocks = new SortIndividualBlocks(&parentStep);
            mergeArray = new MergeArray(&parentStep);
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

    void setNextStepQuick()
    {
        nextStep.next = divideArray;
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

    void executeNextStepQuick()
    {
        stateChanged = 0;
        if (nextStep.type() == "DivideArray") {
            if (divideArray->divisionPossible()) {
                divideArray->divideArray();
                sortIndividualBlocks->current_array = divideArray->current_array;
                mergeArray->current_array = divideArray->current_array;
                stateChanged = 1;
            } else {
                nextStep.next = sortIndividualBlocks;
            }
        } else if (nextStep.type() == "SortIndividualBlocks") {
            stateChanged = sortIndividualBlocks->sort();
            if (stateChanged) {
                divideArray->current_array = sortIndividualBlocks->current_array;
                mergeArray->current_array = sortIndividualBlocks->current_array;
            } else {
                nextStep.next = mergeArray;
            }
            
        } else if (nextStep.type() == "MergeArray") {
            current = mergeArray->merge();
            sorted = 1;
        }
    }

    vector<int> currentStatus() 
    {
        vector<int> a = {};
        for (int i = 0; i < current.size(); i++) {
            a.push_back(current[i]);
        }

        return a;
    }

    vector<int> currentStatusQuick()
    {
        vector<vector<int>> arr = divideArray->current_array;
        vector<int> a = {};
        for (int i = 0; i < arr.size(); i++) {
            for(int j = 0; j < arr[i].size(); j++) {
                a.push_back(arr[i][j]);
            }
        }
        return a;
    }

};

int maximum(vector<int> arr)
{
    int m = arr[0];

    for (int i = 1; i < arr.size(); i++)
    {
        if (arr[i] > m)
        {
            m = arr[i];
        }
    }

    return m;
}

void display(vector<int> arr, string alg)
{
    int m = maximum(arr);
    system("clear");
    cout << "                                                               ALGORITHM: " << alg << endl << endl;
    cout << "TIME COMPLEXITY: ";
    if (alg == "QuickSort") {
        cout << "O(nlogn)" << endl;
    } else {
        cout << "O(n^2)" << endl;
    } 
    cout << endl;
    cout << "INDEX   0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15" << endl;

    for (int i = 0; i < m; i++) {
        if (i < 9) {
            cout << "    " << i+1 << "   ";
        } else {
            cout << "   " << i+1 << "   ";
        }
        for (int j = 0; j < arr.size(); j++) {
            if ((arr[j]-i) > 0) {
                cout << "#   ";
            } else if ((arr[j]-i) == 0) {
                if (arr[j] < 10) {
                    cout << arr[j] << "   ";
                } else {
                    cout << arr[j] << "  ";
                }
            } else {
                cout << "    ";
            }
        }
        cout << endl;
    }
    cout << endl << endl;

    if (alg == "BubbleSort") {
        cout << "Bubble sort is a simple sorting algorithm that repeatedly steps through the list, compares adjacent elements, and swaps them if they are in the wrong order.It continues this process until the entire list is sorted, with larger elements 'bubbling' to the top with each iteration." << endl;
    } else if (alg == "InsertionSort") {
        cout << "Insertion sort is a simple sorting algorithm that builds the final sorted array one element at a time by iteratively inserting each element into its correct position relative to the already sorted elements. It has an average and worst-case time complexity of O(n^2) but performs efficiently on small datasets or nearly sorted arrays." << endl;
    } else if (alg == "SelectionSort") {
        cout << "Selection sort is a simple sorting algorithm that repeatedly finds the minimum element from the unsorted part of the array and swaps it with the element at the beginning of the unsorted part. It continues this process until the entire array is sorted." << endl;
    } else if (alg == "QuickSort") {
        cout << "Quick sort is a highly efficient sorting algorithm that uses a divide-and-conquer approach to recursively partition the array into smaller subarrays based on a pivot element, then sorts each subarray independently. It has an average time complexity of O(nlogn) and is widely used due to its speed and ability to sort large datasets efficiently." << endl;
    }
}

void runBubbleSort(vector<int> a)
{
    SingleIterArray<BubbleSortStep> arr;
    arr.current = a;
    int i = 0;
    arr.initializeBubble();
    arr.setNextStepBubble();
    display(arr.currentStatus(), "BubbleSort");
    sleep(1);
    while (!arr.sorted) {
        if (arr.stateChanged) {
            i++;
            display(arr.currentStatus(), "BubbleSort");
            sleep(1);
        }
        arr.executeNextStepBubble();
    }
    cout << "Total iterations: " << i << endl;
}

void runInsertionSort(vector<int> a)
{
    SingleIterArray<InsertionSortStep> arr;
    arr.current = a;
    int i = 0;
    arr.initializeInsert();
    arr.setNextStepInsert();
    display(arr.currentStatus(), "InsertionSort");
    sleep(1);
    while (!arr.sorted)
    {
        if (arr.stateChanged)
        {
            i++;
            display(arr.currentStatus(), "InsertionSort");
            sleep(1);
        }
        arr.executeNextStepInsert();
    }
    cout << "Total iterations: " << i << endl;
}

void runSelectionSort(vector<int> a)
{
    SingleIterArray<SelectionSortStep> arr;
    int i = 0;
    arr.current = a;
    arr.initializeSelect();
    arr.setNextStepSelect();
    display(arr.currentStatus(), "SelectionSort");
    sleep(1);
    while (!arr.sorted)
    {
        if (arr.stateChanged)
        {
            i++;
            display(arr.currentStatus(), "SelectionSort");
            sleep(1);
        }
        arr.executeNextStepSelect();
    }
    cout << "Total iterations: " << i << endl;
}

void runQuickSort(vector<int> a)
{
    SingleIterArray<QuickSortStep> arr;
    int i = 0;
    arr.current = a;
    arr.initializeQuick();
    arr.setNextStepQuick();
    display(arr.currentStatusQuick(), "QuickSort");
    sleep(1);
    while (!arr.sorted)
    {
        if (arr.stateChanged)
        {
            i++;
            display(arr.currentStatusQuick(), "QuickSort");
            sleep(1);
        }
        arr.executeNextStepQuick();
    }
    cout << "Total iterations: " << i << endl;
}

void compareAlgorithms() 
{
    int first, second;
    cout << "Which two algorthms do you want to compare? " << endl;
    cout << "1. BubbleSort" << endl;
    cout << "2. InsertionSort" << endl;
    cout << "3. SelectionSort" << endl;
    cout << "4. QuickSort" << endl;
    cout << "First: ";
    cin >> first;
    cout << "Second: ";
    cin >> second;

    SingleIterArray<BubbleSortStep> b;
    SingleIterArray<InsertionSortStep> i;
    SingleIterArray<SelectionSortStep> s;
    SingleIterArray<QuickSortStep> q;


}

int main()
{
    cout << " .----------------.  .----------------.  .----------------.  .----------------.  .----------------.  .----------------.  .----------------. " << endl;
    cout << "| .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. |" << endl;
    cout << "| | _____  _____ | || |  _________   | || |   _____      | || |     ______   | || |     ____     | || | ____    ____ | || |  _________   | |" << endl;
    cout << "| ||_   _||_   _|| || | |_   ___  |  | || |  |_   _|     | || |   .' ___  |  | || |   .'    `.   | || ||_   \  /   _|| || | |_   ___  |  | |" << endl;
    cout << "| |  | | /  | |  | || |   | |_  |_|  | || |    | |       | || |  / .'   |_|  | || |  /  .--.  |  | || |  |    V   |  | || |   | |_  |_|  | |" << endl;
    cout << "| |  | |/   | |  | || |   |  _|  _   | || |    | |   _   | || |  | |         | || |  | |    | |  | || |  | |\  /| |  | || |   |  _|  _   | |" << endl;
    cout << "| |  |   ^    |  | || |  _| |___/ |  | || |   _| |__/ |  | || |  | `.___.'|  | || |  |  `--'  /  | || | _| |_ V_| |_ | || |  _| |___/ |  | |" << endl;
    cout << "| |  |__/  |__|  | || | |_________|  | || |  |________|  | || |   `._____.'  | || |   `.____.'   | || ||_____||_____|| || | |_________|  | |" << endl;
    cout << "| |              | || |              | || |              | || |              | || |              | || |              | || |              | |" << endl;
    cout << "| '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' |" << endl;
    cout << "'----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------' " << endl;
    sleep(2);
    vector<int> a = {13, 8, 19, 14, 9, 12, 17, 16, 0, 2, 10, 3, 6, 18, 1, 7};

    while (1) {
        int c;
        cout << "What would you like to do?" << endl;
        cout << "1. Run visualisation with BubbleSort. " << endl;
        cout << "2. Run visualisation with InsertionSort. " << endl;
        cout << "3. Run visualisation with SelectionSort. " << endl;
        cout << "4. Run visualisation with QuickSort. " << endl;
        cout << "5. Exit " << endl;
        cout << "> ";
        cin >> c;

        if (c == 1)
        {
            cout << "The current dataset is: ";
            for (int i = 0; i < a.size(); i++)
            {
                cout << a[i] << " ";
            }
            int b;
            cout << endl;
            cout << "Do you wish to use your own dataset? (0/1)"
                 << "\n> ";
            cin >> b;
            if (b)
            {
                cout << "**NOTE** Numbers greater than 15 will be replaced by mod of 15" << endl;
                for (int i = 0; i <= 15; i++)
                {
                    cout << "Enter the number (" << i + 1 << "/16): ";
                    cin >> a[i];
                }
            }
            runBubbleSort(a);
        }
        else if (c == 2)
        {
            cout << "The current dataset is: ";
            for (int i = 0; i < a.size(); i++)
            {
                cout << a[i] << " ";
            }
            int b;
            cout << endl;
            cout << "Do you wish to use your own dataset? (0/1)"
                 << "\n> ";
            cin >> b;
            if (b)
            {
                cout << "**NOTE** Numbers greater than 15 will be replaced by mod of 15" << endl;
                for (int i = 0; i <= 15; i++)
                {
                    cout << "Enter the number (" << i + 1 << "/16): ";
                    cin >> a[i];
                }
            }
            runInsertionSort(a);
        }
        else if (c == 3)
        {
            cout << "The current dataset is: ";
            for (int i = 0; i < a.size(); i++)
            {
                cout << a[i] << " ";
            }
            int b;
            cout << endl;
            cout << "Do you wish to use your own dataset? (0/1)"
                 << "\n> ";
            cin >> b;
            if (b)
            {
                cout << "**NOTE** Numbers greater than 15 will be replaced by mod of 15" << endl;
                for (int i = 0; i <= 15; i++)
                {
                    cout << "Enter the number (" << i + 1 << "/16): ";
                    cin >> a[i];
                }
            }
            runSelectionSort(a);
        }
        else if (c == 4)
        {
            cout << "The current dataset is: ";
            for (int i = 0; i < a.size(); i++)
            {
                cout << a[i] << " ";
            }
            int b;
            cout << endl;
            cout << "Do you wish to use your own dataset? (0/1)"
                 << "\n> ";
            cin >> b;
            if (b)
            {
                cout << "**NOTE** Numbers greater than 15 will be replaced by mod of 15" << endl;
                for (int i = 0; i <= 15; i++)
                {
                    cout << "Enter the number (" << i + 1 << "/16): ";
                    cin >> a[i];
                }
            }
            runQuickSort(a);
        }
        else
        {
            exit(0);
        }
    }

    return 0;
}
