#include <iostream>
#include <windows.h>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <stdlib.h>
#include <array>
#include <algorithm>

#pragma comment(lib, "urlmon.lib")

using namespace std;

struct bucketSort {

    vector<int> myVect;
    int myCounties;
    float mySizeOf;
    string directionality;

    bucketSort(vector<int> vect1, int counties, float sizeOf, string ascending) {

        this->myVect = vect1;
        this->myCounties = counties;
        this->mySizeOf = sizeOf;
        this->directionality = ascending;
    }

    float Sort();
    float Sort(int size);

};

float bucketSort::Sort() {

    vector<float> returnVec;
    return 1;

}

float bucketSort::Sort(int sizeBucket) {

    vector<vector<float>> returnVec(sizeBucket);
    vector<vector<float>> finalVec(sizeBucket);
    int numOfBuckets = 0;

    for (int i = 0; i < myVect.size(); i++) {
        int bucketPos = i * myVect.at(sizeBucket);

        if (bucketPos <= 0) {
            returnVec.at(0).push_back(myVect.at(sizeBucket));
        }
        else if (bucketPos > sizeBucket) {
            returnVec.at(sizeBucket).push_back(myVect.at(sizeBucket));
        }
        else {
            returnVec.at(float(bucketPos)).push_back(myVect.at(sizeBucket));
        }

    }

    for (int j = 0; j < returnVec.size(); j++) {
        sort(returnVec.at(j).begin(), returnVec.at(j).end());
    }

    
    for (int k = 0; k < sizeBucket; k++) {
        //for (int l = 0; l < returnVec.at(k).size(); l++) {
            if (returnVec.at(k).at(0) != NULL) {
                finalVec.at(numOfBuckets++) = returnVec.at(k);
            }
        //}
    }

    return finalVec.at(0).at(0);

}

struct riskInformation {
    int numCases; //9 value
    float positivityRatio; //26  ratio [0,1] //prety sparse, not good data it seems
    float caseDensity; //28  ratio [cases/100k people]
    float infectionRate; //30 num cases that arise from a given infection
    float vaccinationsCompleted; //37 ratio [0,1]
    float casePerHundredK; //44 weekly new cases per 100k

    riskInformation(int a, float b, float c, float d, float e, float f) {
        numCases = a;
        positivityRatio = b;
        caseDensity = c;
        infectionRate = d;
        vaccinationsCompleted = e;
        casePerHundredK = f;


    }




};

int ascending(const void* x, const void* y) {
    return (*(float*)(x) * 100000 - *(float*)(y) * 100000);
}

float calculateRiskLevel(riskInformation* r1, riskInformation* r2) {
    float caseRatio = ((float)r2->numCases / (float)r1->numCases);
    float caseGrowthFactor = ((r2->casePerHundredK - r1->casePerHundredK) / (r2->caseDensity - r1->caseDensity + 1));
    float vaccineNormalization = ((r2->vaccinationsCompleted + r1->vaccinationsCompleted) / 2) * (r2->caseDensity - r1->caseDensity);

    // cout << caseRatio << "  " << caseGrowthFactor << "   " << vaccineNormalization << endl;

    float risk = caseRatio + caseGrowthFactor - vaccineNormalization;

    return risk;
}

int main()
{
    std::cout << "Hello World!\n";

    cout << "The good version \n";

    // the URL to download from 
    string state;

    cout << "Please enter the abbreviation of the state you wish to travel to (ex: CA):" << endl;
    cin >> state;
    string str = "https://api.covidactnow.org/v2/county/" + state + ".timeseries.csv?apiKey=74106da3ec5c421e9ebb34ab44eb0d2e";
    wstring widestr = std::wstring(str.begin(), str.end());
    const wchar_t* srcURL = widestr.c_str();

    // the destination file 
    const wchar_t* destFile = L"StateData.txt";

    // URLDownloadToFile returns S_OK on success 

    if (S_OK == URLDownloadToFileW(NULL, srcURL, destFile, 0, NULL))
    {

        printf("Saved to 'StateData.txt'");

        cout << endl;

    }

    else
    {

        printf("Failed");

        cout << endl;

    }

    ifstream file("StateData.txt");

    string line;
    int rowNum = 0;
    map<pair<string, string>, riskInformation*> data; //class RiskInformation numCses, case dneisty vaacinantiono
    unordered_set<string> counties;

    getline(file, line);

    while (getline(file, line)) {

        stringstream row(line);
        string entry, date, loc;
        int ctr = 0;
        int cases = 1;
        float positivityRatio_ = 0; //26
        float caseDensity_ = 0; //28
        float infectionRate_ = 0; //30
        float vaccinationsCompleted_ = 0; //37
        float casePerHundredK_ = 0; //44
        bool hasData = false;

        while (getline(row, entry, ',')) {

            ctr++;

            if (ctr == 1) {
                date = entry;
            }
            else if (ctr == 4) {
                counties.insert(entry);
                loc = entry;

            }
            else if (ctr == 9) {
                if (!entry.empty()) {
                    cases += stoi(entry);
                    hasData = true;
                }

            }

            else if (ctr == 26) {
                if (!entry.empty()) {
                    positivityRatio_ = stof(entry);
                }
            }
            else if (ctr == 28) {
                if (!entry.empty()) {
                    caseDensity_ = stof(entry);

                }
            }
            else if (ctr == 30) {
                if (!entry.empty()) {
                    infectionRate_ = stof(entry);

                }
            }
            else if (ctr == 37) {
                if (!entry.empty()) {
                    vaccinationsCompleted_ = stof(entry);
                }
            }
            else if (ctr == 44) {
                if (!entry.empty()) {
                    casePerHundredK_ = stof(entry);
                }

                data[make_pair(loc, date)] = new riskInformation(cases, positivityRatio_, caseDensity_, infectionRate_, vaccinationsCompleted_, casePerHundredK_);
                break;
            }

        }
        rowNum++;


        if (rowNum == 100000) {
            //break;
        }

    }

    cout << endl << "Finished taking in input data" << endl << endl;

    cout << rowNum << endl;


    //Checking to see what the containers look like

    /*
    auto iter = data.begin();
    while (iter != data.end()) {
        //cout << iter->first.first << "  " << iter->first.second << "  " << iter->second << endl;
        iter++;
    }
    auto iter2 = data2.begin();
    while (iter2 != data2.end()) {
    //cout << iter->first.first << "  " << iter->first.second << "  " << iter->second << endl;
    iter2++;
    }*/

    cout << data.size() << endl;

    auto countiesIter = counties.begin();

    cout << endl << "The counties that exist in this state are:" << endl;

    while (countiesIter != counties.end()) {
        cout << *countiesIter << endl;
        countiesIter++;
    }



    file.close();

    cout << endl << "For how many days will you travel?" << endl;

    int len;

    cin >> len;

    map<pair<string, string>, riskInformation*>::iterator* iterList = new map<pair<string, string>, riskInformation*>::iterator[len];
    map < string, map<pair<string, string>, float> > generalRiskLevel;
    //map <  map<pair<string, string>, float>, string > generalRiskLevelInv;


    float* arrRiskLevel = new float[data.size() - len];



    auto iter = data.begin();
    int i = 0, arrCounter = 0;
    bool store = false;

    while (iter != data.end()) {
        int c = i;

        if (store)
            c++;

        iterList[i] = iter;
        i++;
        iter++;

        auto temp = iter;
        for (int j = 0; j < len; j++) {
            temp++;
        }

        if (temp == data.end())
            break;

        if (i == len) {
            i = 0;
            c = i;
            store = true;
        }

        if (iterList[c]->first.first.compare(iter->first.first) == 0) {
            if (store) {

                /*
                int startDate = iterList[c]->second; //risk
                if (startDate == 0) {
                    startDate++;
                }
                float risk = (float)iter->second / startDate;*/

                float risk = calculateRiskLevel(iterList[c]->second, iter->second);
                generalRiskLevel[iterList[c]->first.first][make_pair(iterList[c]->first.second, iter->first.second)] = risk;
                arrRiskLevel[arrCounter] = risk;
                arrCounter++;
            }
        }
        else {
            i = 0;
            store = false;
        }

    }


    auto superIter = generalRiskLevel.begin();
    int size = 0;

    while (superIter != generalRiskLevel.end()) {
        auto i2 = superIter->second.begin();

        //cout << endl << superIter->second.size() << endl;
        size += superIter->second.size();

        while (i2 != superIter->second.end()) {
            cout << superIter->first << "  " << i2->first.first << " " << i2->first.second << "  " << i2->second << endl;
            i2++;
        }
        superIter++;

    }

    cout << endl << size << endl;

    //This isnt working properly

    /*
    qsort(arrRiskLevel, data.size(), sizeof(float), ascending);
    for (int i = 0; i < counties.size(); i++)
        cout << arrRiskLevel[i] << " ";
    superIter = generalRiskLevel.begin();
    while (superIter != generalRiskLevel.end()) {
        auto i2 = superIter->second.begin();
        while (i2 != superIter->second.end()) {
            if (i2->second == arrRiskLevel[0]) {
                cout << superIter->first << "  " << i2->first.first << " " << i2->first.second << "  " << i2->second << endl;
            }
            i2++;
        }
        superIter++;
    }*/

    string start, end;

    cout << endl << "Enter Travel dates in YYYY-MM-DD format" << endl;

    cin >> start;
    cin >> end;

    unordered_map<string, float> riskLevel;
    unordered_map<float, string> riskLevelInv;

    float* arr1 = new float[counties.size()];
    float* arr2 = new float[counties.size()];

    int c = 0;
    countiesIter = counties.begin();

    while (countiesIter != counties.end()) {
        pair<string, string> key = make_pair(*countiesIter, start);
        auto numCase1 = data.find(key);

        key = make_pair(*countiesIter, end);
        auto numCase2 = data.find(key);

        if (numCase1 != data.end() && numCase2 != data.end()) {

            /*
            int start = numCase1->second, end = numCase2->second;
            if (start == 0) {
                start++;
            }
            float risk = (float)(end) / (float)(start);*/

            float risk = calculateRiskLevel(numCase1->second, numCase2->second);

            riskLevel[*countiesIter] = risk;
            riskLevelInv[risk] = *countiesIter;
            arr1[c] = risk;
            arr2[c] = risk;
            c++;
        }

        countiesIter++;
    }

    auto iterRL = riskLevel.begin();

    while (iterRL != riskLevel.end()) {
        cout << iterRL->first << "   " << iterRL->second << endl;
        iterRL++;
    }

    qsort(arr1, counties.size(), sizeof(float), ascending);

    /*
    for (int i = 0; i < counties.size(); i++)
        cout << arr1[i] << " ";
    */

    //merge arr2

    cout << endl << "The county with the lowest risk is : " << riskLevelInv.find(arr1[0])->second << endl;

    return 0;

}