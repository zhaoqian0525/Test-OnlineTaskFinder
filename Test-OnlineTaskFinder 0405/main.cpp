#include <iostream>
#include <utility>
#include <vector>
#include <typeinfo>
#include "json.hpp"
#include <fstream>
#include <sstream>
#include "readVector.h"
#include <time.h>
#include <unordered_set>

using namespace std;
using json = nlohmann::json;

class Edge {
public:
    string front;
    string back;

    Edge() = default;

    explicit Edge(vector<string> input) {
        if (!input.empty() && input.size() == 2) {
            front = input[0];
            back = input[1];
        }
    }

    explicit Edge(string input[2]) {
        front = input[0];
        back = input[1];
    }

    void set(vector<string> input) {
        if (!input.empty() && input.size() == 2) {
            front = input[0];
            back = input[1];
            cout << printVectorToString(input) << endl;
            cout << front << back << endl;
        }
    }

    vector<string> get() {
        vector<string> get = {front, back};
        return get;
    }

    void print() const {
        cout << front << "->" << back << "  ";
    }

    string printEdgeToString(const std::string &separator = " -> ") const {
        std::stringstream s;
        s << front;
        s << separator;
        s << back;
        return s.str();
    }

    bool operator==(const Edge &b) const {
        bool result;
        result = this->front == b.front && this->back == b.back;
        return result;
    }

    bool operator!=(const Edge &b) const {
        bool result;
        result = !(this->front == b.front && this->back == b.back);
        return result;
    }

    bool operator<(const Edge &b) const {
        return this->front < b.front;
    }

    bool operator>(const Edge &b) const {
        return this->front > b.front;
    }
};

template<class T>
int getListSize(T &list) {
    return sizeof(list) / sizeof(list[0]);
}

bool checkTheEdgeOfTheDataBase(vector<string> input, const vector<vector<string>> &dataBase) {
    for (vector<string> oneDataBaseEdge: dataBase) {
        if (input[0] == oneDataBaseEdge[0] && input[1] == oneDataBaseEdge[1]) {
            return true;
        }
    }
    return false;
}

bool canBeFoundInThisVector(const string &object, vector<string> list) {
    auto it = find(list.begin(), list.end(), object);
    if (it != list.end()) {
        return true;
    } else {
        return false;
    }
}

bool checkTheEdgeOfTheDataBase(const Edge &input, const vector<Edge> &dataBase) {
    for (const Edge &oneDataBaseEdge: dataBase) {
        if (input.front == oneDataBaseEdge.front && input.back == oneDataBaseEdge.back) {
            return true;
        }
    }
    return false;
}


class TestOnlineTaskFinder {
public:
    vector<Edge> resultMemoryOfThis;
    vector<string> motionList;
    vector<string> checkedMotion;
    vector<Edge> taskDataBase;
    vector<vector<Edge>> allResultMemory;
    vector<Edge> theBestResult;
    int checkedMotionNum = 0;
    int score = 0;

    TestOnlineTaskFinder(vector<string> inputMotionList, const vector<vector<string>> &inputTaskDataBase) {
        //input data
        motionList = inputMotionList;
        //transform vector<vector<string>> to vector<Edge>
        taskDataBase.reserve(inputTaskDataBase.size());
        for (const auto &edge : inputTaskDataBase) {
            taskDataBase.emplace_back(edge);
        }
        //clean the repeat motion
        auto ite = unique(motionList.begin(), motionList.end());
        motionList.erase(ite, motionList.end());
        cout << "test Motion: " << printVectorToString(motionList) << endl;
        if (allResultMemory.empty()) {
            start();
        }
        cout << " resultmemory after start:" << endl;

        //printMemory();

        for (int i = checkedMotionNum; i < motionList.size(); ++i) {
            cout << "after  " << motionList[i] << "  added" << endl;
            cout << "*************" << endl;
            //cout<<"motionList Size = "<<motionList.size()<<endl;
            refresh(motionList[i]);
            printMemory();
            cout << printVectorToString(checkedMotion) << endl;
        }

        cout << "checked motion list: ";
        cout << printVectorToString(checkedMotion) << endl;
        cout << "the length of the longest result of this memory is  " << theBestResultLength(allResultMemory)
             << endl;
        score = theBestResultLength(allResultMemory);
        getTheBestResult(allResultMemory);
    }

    void start() {
        if (motionList.empty()) {
            return;
        }
        if (!canBeFoundInThisVector(motionList[0], checkedMotion)) {
            checkedMotion.push_back(motionList[0]);
        }

        bool foundStart = false;
        for (int i = 0; i < motionList.size(); ++i) {
            if (!canBeFoundInThisVector(motionList[i], checkedMotion)) {
                checkedMotion.push_back(motionList[i]);
            }
            checkedMotionNum = i;

            for (const string &motion1: checkedMotion) {
                Edge edge({motion1, motionList[i]});
                if (checkTheEdgeOfTheDataBase(edge, taskDataBase) && checkIfThisEdgeIsTheStart(edge, taskDataBase)) {
                    cout << "found the start edge :" << edge.printEdgeToString() << endl;
                    vector<Edge> firstpossibility;
                    firstpossibility.push_back(edge);
                    allResultMemory.push_back(firstpossibility);
                    //clean the checkedMotion list
                    vector<string> removeList;
                    for (const auto &motion: checkedMotion) {
                        if (motion == edge.front) {
                            for (const string &removeMotion:removeList) {
                                auto iter = std::remove(checkedMotion.begin(), checkedMotion.end(), removeMotion);
                                checkedMotion.erase(iter, checkedMotion.end());
                            }
                            if (find(checkedMotion.begin(), checkedMotion.end(), motionList[i]) ==
                                checkedMotion.end()) {
                                checkedMotion.push_back(motionList[i]);
                            }
                            break;
                        }
                        if ((motion != edge.front && !checkIfThisMotionIsTheStart(motion, taskDataBase)) ||
                            !checkIfThisMotionExistInDataBase(motion, taskDataBase)) {
                            removeList.push_back(motion);
                        }
                    }
                    cout << "***********" << endl;
                    cout << printVectorToString(checkedMotion) << endl;
                    cout << "***********" << endl;
                    return;
                }
            }
        }
    }

    void refresh(const string &newMotion) {
        cout << "refresh  " << newMotion << endl;
        if (allResultMemory.empty()) {
            //motionList.push_back(newMotion);
            start();
        }
        if (!actionExistInDataBase(newMotion, taskDataBase)) {
            return;
        }
        vector<Edge> newEdges;
        for (const string &existMotion: checkedMotion) {
            Edge newEdge({existMotion, newMotion});
            newEdges.push_back(newEdge);
        }
        vector<Edge> possibleNewEdges;
        for (const Edge &edge: newEdges) {
            if (checkTheEdgeOfTheDataBase(edge, taskDataBase)) {
                possibleNewEdges.push_back(edge);
            }
        }

        // Three case
        // 1. add at the tail
        // 2. cannot add at the tail but still can add in middle ,so we should creat a new possibility,if it is a kind of start action,also add it into it and creat this added as a new possibility
        // for example, {1->2 2->4}  after 2->5 added, turn to {1->2 2->4} and {1->2 2->4 2->5}
        // example 2 if {1->2 2->4} after 1->3 added, because 1->3 is a kind of start action, we also take it and the result will be {1->2 2->4} and {1->2 2->4 1->3}
        // 3. still not visited, so it may be itself a new possibility
        // example {1->2} after edge 1->3 added ,result will be {1->2} and {1->3},because of case2,result {1->2  1->3} will also be added here
        vector<vector<Edge>> newPossibilities;
        for (const Edge &newEdge: possibleNewEdges) {
            bool case1 = false;
            for (vector<Edge> &oneResultMemory: allResultMemory) {
                //cout << "checking case 1 of " << printVectorToString(newEdge) << endl;
                //cout<< printVectorToString(oneResultMemory.back())<<endl;
                if (oneResultMemory.back().back == newEdge.front &&
                    andConditionCheck(newEdge, oneResultMemory, taskDataBase)) {
                    //cout << "found one  " << printVectorToString(newEdge) << endl;
                    //clean repeat 2023.2.6
                    /*if (find(oneResultMemory.begin(), oneResultMemory.end(), newEdge) != oneResultMemory.end()) {
                        auto iter = remove(oneResultMemory.begin(), oneResultMemory.end(), newEdge);
                        oneResultMemory.erase(iter, oneResultMemory.end());
                    }*/
                    oneResultMemory.push_back(newEdge);
                    case1= true;
                }
            }
            if (case1) {
                continue;
            }

            for (vector<Edge> &oneResultMemory: allResultMemory) {
                //cout << "checking case 2 of " << printVectorToString(newEdge) << endl;
                for (const Edge &recordedEdge: oneResultMemory) {
                    if (recordedEdge.back == newEdge.front && newEdge != oneResultMemory.back() &&
                        andConditionCheck(newEdge, oneResultMemory, taskDataBase)) {
                        // a new possibility should be created
                        vector<Edge> newResult;
                        newResult = oneResultMemory;
                        //clean repeat 2023.2.6
                        if (find(newResult.begin(), newResult.end(), newEdge) != newResult.end()) {
                            auto iter = remove(newResult.begin(), newResult.end(), newEdge);
                            newResult.erase(iter, newResult.end());
                        }
                        newResult.push_back(newEdge);
                        newPossibilities.push_back(newResult);
                        newResult.clear();
                    }

                    if (checkIfThisEdgeIsTheStart(newEdge, taskDataBase) && newEdge != oneResultMemory.back() &&
                        !alreadyIncludeThisEdge(newEdge, oneResultMemory)) {
                        // a new possibility should be created
                        vector<Edge> newResult;
                        newResult = oneResultMemory;
                        newResult.push_back(newEdge);
                        newPossibilities.push_back(newResult);
                        newResult.clear();
                    }
                }
            }

        }
        //this->allResultMemory.insert(this->allResultMemory.end(), newPossibilities.begin(), newPossibilities.end());
        for (const auto& oneNewPossibility:newPossibilities) {
            if (checkIfThisResultIsNew(oneNewPossibility)){
                this->allResultMemory.push_back(oneNewPossibility);
            }
        }


        for (const Edge &edge: possibleNewEdges) {
            //cout << "checking case 3 of " << printVectorToString(edge) << endl;
            if (checkIfThisEdgeIsTheStart(edge, taskDataBase)) {
                vector<Edge> newResult2;
                newResult2.push_back(edge);
                if (checkIfThisResultIsNew(newResult2)){
                    allResultMemory.push_back(newResult2);
                }
            }
        }

        //clean the repeat result memory
        for (vector<Edge> &oneResultMemory: allResultMemory) {
            auto ite = unique(oneResultMemory.begin(), oneResultMemory.end());
            oneResultMemory.erase(ite, oneResultMemory.end());
        }
        sort(allResultMemory.begin(), allResultMemory.end());
        auto ite = unique(allResultMemory.begin(), allResultMemory.end());
        allResultMemory.erase(ite, allResultMemory.end());

        //refresh the Motion list 1.0
        /*if (checkedMotion.back() != newMotion) {
            checkedMotion.push_back(newMotion);
        }*/
        //refresh the Motion list 2.0 not surely work
        if (!canBeFoundInThisVector(newMotion, checkedMotion)) {
            checkedMotion.push_back(newMotion);
        }
        //refresh the score and the BestResult
        score = theBestResultLength(allResultMemory);
        getTheBestResult(allResultMemory);

        cout << "checked motion: ";
        cout << printVectorToString(checkedMotion) << endl;
    }

    static bool
    andConditionCheck(const Edge &edge, const vector<Edge> &oneResultMemory,
                      const vector<Edge> &dataBase) {
        andConditionNumOfThisEdge(edge, dataBase);
        vector<Edge> andConditionRecorder;
        for (const Edge &dataBaseEdge: dataBase) {
            if (dataBaseEdge.back == edge.front) {
                andConditionRecorder.push_back(dataBaseEdge);
            }
        }
        for (const Edge &existEdge: oneResultMemory) {
            for (const Edge &andConditionEdge: andConditionRecorder) {
                if (existEdge == andConditionEdge) {
                    auto iter = remove(andConditionRecorder.begin(), andConditionRecorder.end(), andConditionEdge);
                    andConditionRecorder.erase(iter, andConditionRecorder.end());
                }
            }
        }
        if (andConditionRecorder.empty()) {
            return true;
        }
        return false;
    };

    static int andConditionNumOfThisEdge(const Edge &edge, const vector<Edge> &dataBase) {
        int num = 0;
        for (const Edge &dataBaseEdge: dataBase) {
            if (dataBaseEdge.back == edge.front) {
                num++;
            }
        }
        return num;
    }

    //printActionRemainToBeDone new memory for test
    void printMemory() {
        for (const vector<Edge> &oneResult: allResultMemory) {
            for (const Edge &edge: oneResult) {
                cout << edge.printEdgeToString() << "  ";
            }
            cout << "\n";
        }
    }

    static bool checkIfThisEdgeIsTheStart(const Edge &edge, const vector<Edge> &testTaskData) {
        for (const Edge &dataBaseEdge: testTaskData) {
            if (edge.front == dataBaseEdge.back) {
                return false;
            }
        }
        return true;
    }

    static bool alreadyIncludeThisEdge(const Edge &edge, const vector<Edge> &oneResultMemory) {
        return std::find(oneResultMemory.begin(), oneResultMemory.end(), edge) != oneResultMemory.end();
    }

    static bool checkIfThisMotionIsTheStart(const string &motion, const vector<Edge> &testTaskData) {
        unordered_set<string> endSet;
        for (const Edge &dataBaseEdge: testTaskData) {
            endSet.insert(dataBaseEdge.back);
        }
        return endSet.find(motion) == endSet.end();
    }

    static bool checkIfThisMotionExistInDataBase(const string &motion, const vector<Edge> &testTaskData) {
        unordered_set<string> motionSet;
        for (const Edge &dataBaseEdge : testTaskData) {
            motionSet.insert(dataBaseEdge.front);
            motionSet.insert(dataBaseEdge.back);
        }
        return motionSet.count(motion) > 0;
    }

    static int theBestResultLength(const vector<vector<Edge>> &allResultMemory) {
        int i = 0;
        for (const auto &result: allResultMemory) {
            if (result.size() > i) {
                i = result.size();
            }
        }
        return i;
    }

    void getTheBestResult(const vector<vector<Edge>> &allResults) {
        if (allResults.empty()) {
            return;
        }
        size_t num = 0;
        size_t length = 0;
        for (size_t i = 0; i < allResults.size(); ++i) {
            if (allResults[i].size() > length) {
                length = allResults[i].size();
                num = i;
            }
        }
        theBestResult = allResults[num];
    }

    void printTheBestResult() {
        cout << "Already done actions in this task are :" << endl;
        for (const Edge &edge: theBestResult) {
            cout << edge.printEdgeToString() << "    ";
        }
        cout << endl;
    }

    vector<vector<string>> printTheBestResultToVector() {
        vector<vector<string>> BestResult;
        for (const Edge &edge: theBestResult) {
            BestResult.push_back({edge.front, edge.back});
        }
        cout << endl;
        return BestResult;
    }

    static bool actionExistInDataBase(const string &motion, const vector<Edge> &dataBase) {
        for (const Edge &edge:dataBase) {
            if (motion == edge.front || motion == edge.back) {
                return true;
            }
        }
        return false;
    }

    //new added 01.26
    vector<string> theSingleActions;

//new added 01.26
    bool oneActionChecked() {
        bool check = false;
        for (const string &motion: checkedMotion) {
            if (checkIfThisMotionExistInDataBase(motion, taskDataBase) &&
                checkIfThisMotionIsTheStart(motion, taskDataBase)) {
                theSingleActions.push_back(motion);
                check = true;
            }
        }
        return check;
    }

    bool checkIfThisResultIsNew(const vector<Edge> &newResult) {
        for (const auto &result: allResultMemory) {
            if (result.size() == newResult.size()) {
                int num = newResult.size();
                for (const Edge &newEdge:newResult) {
                    bool haveSame = false;
                    for (const Edge &oldEdge:result) {
                        if (oldEdge == newEdge) {
                            num--;
                            haveSame = true;
                            break;
                        }
                    }
                    if (!haveSame) {
                        break;
                    }
                }
                if (num == 0) {
                    return false;
                }
            }
        }
        return true;
    }
};


void test() {
    vector<string> nameListOfTestCases;
    ifstream f("../input.json");
    json j = json::parse(f);
    for (auto const &item: j.items()) {
        //cout << item.key() << endl;
        nameListOfTestCases.push_back(item.key());
    }
    cout << "the list of the test cases :" << endl << printVectorToString(nameListOfTestCases) << endl;

    json jsonOutput;
    for (const string &caseName:nameListOfTestCases) {
        cout << endl << endl << caseName << "---------------------------------------------------------------" << endl;
        //read test data
        nlohmann::json task = j.at(caseName);
        vector<string> motionList;
        motionList = task.at("motion_list").get<vector<string>>();
        vector<vector<string>> taskDataBase;
        taskDataBase = task.at("task_edges").get<vector<vector<string>>>();
        //run the program
        TestOnlineTaskFinder bbb(motionList, taskDataBase);
        //output
        nlohmann::json data;
        data["best_result"] = bbb.printTheBestResultToVector();
        data["score"] = bbb.score;
        jsonOutput[caseName] = data;
    }
    f.close();
    ofstream file("../output.json");
    string str1 = jsonOutput.dump(4);
    file << str1 << endl;
    file.close();

    //check the output
    vector<string> notPassedCaseName;
    ifstream f1("../output.json");
    ifstream f2("../referenceOutput.json");
    json j2 = json::parse(f2);
    vector<string> nameListOfTestedCases;
    json j1 = json::parse(f1);
    for (auto const &item: j1.items()) {
        //cout << item.key() << endl;
        nameListOfTestedCases.push_back(item.key());
    }
    cout << "the list of the tested cases :" << endl << printVectorToString(nameListOfTestedCases) << endl;
    bool allCaseAreRight = true;
    int notPassedNum = 0;
    int notFoundNum = 0;
    for (const string &caseName:nameListOfTestCases) {
        vector<vector<string>> resultList;
        resultList = j1.at(caseName).at("best_result").get<vector<vector<string>>>();
        vector<vector<string>> referenceResultList;
        if (!j2.contains(caseName)) {
            cout << caseName << " can not be found in reference output data" << endl;
            allCaseAreRight = false;
            notFoundNum++;
            continue;
        }
        referenceResultList = j2.at(caseName).at("best_result").get<vector<vector<string>>>();
        if (referenceResultList == resultList) {
            cout << caseName << " checked" << endl;
        } else {
            cout << caseName << " check not passed" << endl;
            allCaseAreRight = false;
            notPassedNum++;
            notPassedCaseName.push_back(caseName);
        }
    }
    if (allCaseAreRight) {
        cout << "all test case are checked" << endl;
    } else if (notPassedNum > 0) {
        cout << notPassedNum << " check not passed" << endl;
        cout << "not passed case: " << printVectorToStringWithNoArrows(notPassedCaseName) << endl;
    } else if (notFoundNum > 0) {
        cout << notFoundNum << " check not found" << endl;
    }

}


bool test2() {

    vector<vector<pair<string, string>>> allResultMemory = {{{"1", "2"}, {"2", "3"}},
                                                            {{"1", "2"}, {"2", "3"}, {"3", "4"}}};
    vector<pair<string, string>> newResult={{"1", "2"},{"2", "3"}};

    for (const auto &result: allResultMemory) {
        if (result.size() == newResult.size()) {
            int num = newResult.size();
            for (const auto &newEdge:newResult) {
                bool haveSame = false;
                for (const auto &oldEdge:result) {
                    if (oldEdge == newEdge) {
                        num--;
                        haveSame = true;
                        break;
                    }
                }
                if (!haveSame) {
                    break;
                }
            }
            if (num == 0) {
                cout<<"false"<<endl;
                return false;
            }
        }
    }
    cout<<"true"<<endl;
    return true;


}

int main() {
    //record program runtime
    clock_t start, end;
    start = clock();

    cout << "Hello, World!" << endl;

    //TestOnlineTaskFinder3 aaa;

    cout << "****************" << endl;

    /*aaa.refresh("1");
    aaa.refresh("2");
    aaa.refresh("3");
    aaa.refresh("4");*/

    //aaa.printMemory();
    //

    test();


    cout << "******************" << endl;
    //test for the action remain to be done program
    // vector<vector<string>> alreadyDone={{"1","2"}};
    //TestActionRemainToBeDone aaaa(alreadyDone);
    // vector<string> inputActions={"1","2"};
    // TestActionRemainToBeDone ccc(inputActions);


    end = clock();
    cout << endl << "run time =" << double(end - start) / CLOCKS_PER_SEC << "s" << endl;

    return 0;
}
