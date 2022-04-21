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


//Bucket Sort Implementation

struct bucketSort {

    vector<float> myVect;
    //int myCounties;
    //float mySizeOf;
   // string directionality;

public:

    bucketSort(vector<float> vect1, int counties, string ascending) {

        this->myVect = vect1;
        // this->myCounties = counties;
         //this->mySizeOf = sizeOf;
        // this->directionality = ascending;
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


        int bucketPos = sizeBucket * (myVect.at(i));

        //cout << bucketPos;

        if (bucketPos <= 0) {
            cout << "\ninserting float smaller than bucket: " << myVect.at(i);
            returnVec.at(0).push_back(myVect.at(i));
        }
        else if (bucketPos >= sizeBucket) {
            cout << "\ninserting float bigger than bucket: " << myVect.at(i);
            returnVec.at(sizeBucket - 1).push_back(myVect.at(i));
        }
        else {
            float insertFloat = myVect.at(i);
            cout << "\ninserting float else: " << insertFloat;
            returnVec.at(float(bucketPos)).push_back(insertFloat);
        }

    }

    for (int j = 0; j < returnVec.size(); j++) {
        sort(returnVec.at(j).begin(), returnVec.at(j).end());
    }


    for (int k = 0; k < sizeBucket; k++) {
        //for (int l = 0; l < returnVec.at(k).size(); l++) {
        if (returnVec.at(k).size() > 0) {
            finalVec.at(numOfBuckets++) = returnVec.at(k);
        }
        //}
    }

    return finalVec.at(0).at(0);

}


//Risk Class

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

//Risk calculation

float calculateRiskLevel(riskInformation* r1, riskInformation* r2) {
    float caseRatio = ((float)r2->numCases / (float)r1->numCases);
    float positivityRatioAvg = ((float)r1->positivityRatio + r2->positivityRatio) / 2.0;
    float caseDensityAvg = ((float)r1->caseDensity + r2->caseDensity) / 2.0;
    float vaccineNormalization = ((r2->vaccinationsCompleted + r1->vaccinationsCompleted) / 2.0) * caseDensityAvg;

    //cout << caseRatio << "  " << positivityRatioAvg  <<  "   " << caseDensityAvg<< "  " << vaccineNormalization << endl;

    float risk = caseRatio + positivityRatioAvg + caseDensityAvg - vaccineNormalization;

    if (risk < -1000 || risk > 1000) {
        return 0;
    }
    return risk;
}


//Radix Sort Implementation

int getMax(vector<int> arr) {
    int max = arr[0];
    for (unsigned int i = 1; i < arr.size(); i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

void countSort(vector<int>& arr, int exp) {
    vector<int> newArr; // updated array
    newArr.resize(arr.size());
    int temp[10] = { 0 }; //base 10

    // Store count of occurrences in count[]
    for (int i = 0; i < arr.size(); i++) { // O(n)
        temp[(arr[i] / exp) % 10]++; //generate key using  %10
    }


    for (int i = 1; i < 10; i++) { //O(base) = O(1)
        temp[i] += temp[i - 1]; //update to be stable sort
    }

    for (int i = (arr.size() - 1); i >= 0; i--) { //O(n)
        newArr[temp[(arr[i] / exp) % 10] - 1] = arr[i]; //create sorted array
        temp[(arr[i] / exp) % 10]--;
    }

    for (int i = 0; i < arr.size(); i++) { //O(n)
        arr[i] = newArr[i]; //update arr to be sorted
    }
}

void radixsort(vector<int>& arr) {
    int max = getMax(arr); //O(n)

    for (int exp = 1; max / exp > 0; exp *= 10) { //number of passes based on every digit
        countSort(arr, exp);
    }

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

    //Retrieving all necessary data from API

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
    map <float, pair<pair<string, string>, string>> generalRiskLevelInv;



    vector<int> arrRiskLevelRadix;
    vector<float> arrRiskLevelBucket;

    auto iter = data.begin();
    int i = 0;
    bool store = false;


    //Generation of Risk Level Map for every possible date and county

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

                float risk = calculateRiskLevel(iterList[c]->second, iter->second);
                generalRiskLevel[iterList[c]->first.first][make_pair(iterList[c]->first.second, iter->first.second)] = risk;
                generalRiskLevelInv[risk] = make_pair(make_pair(iterList[c]->first.second, iter->first.second), iterList[c]->first.first);

                if (risk != 0 || true) {
                    arrRiskLevelRadix.push_back(risk * 10000);
                    arrRiskLevelBucket.push_back(risk);
                }

                //cout << risk << endl;

            }
        }
        else {
            i = 0;
            store = false;
        }

    }

    // Prints out the generated Risk Level Map

    auto superIter = generalRiskLevel.begin();
    int size = 0;

    while (superIter != generalRiskLevel.end()) {
        auto i2 = superIter->second.begin();

        cout << endl << superIter->second.size() << endl;
        size += superIter->second.size();

        while (i2 != superIter->second.end()) {
           // cout << superIter->first << "  " << i2->first.first << " " << i2->first.second << "  " << i2->second << endl;
            i2++;
        }
        superIter++;

    }

    cout << endl << size << endl;


    // bucketSort myBucket = bucketSort::bucketSort(arrRiskLevelBucket, counties.size(), "ascending");
     //float ret = myBucket.Sort(10);


    radixsort(arrRiskLevelRadix);

    cout << endl << "BEGIN" << endl;

    for (int i = 0; i < counties.size(); i++) {
        cout << ((float)arrRiskLevelRadix[i] / (float)10000) << " ";
    }

    auto ansRadix = generalRiskLevelInv.find(((float)arrRiskLevelRadix[0] / (float)10000));
    //auto ansBucket = generalRiskLevelInv.find(((float)arrRiskLevelRadix[0] / (float)10000));

    cout << endl << "The dates and county with lowest risk level are  : " << ansRadix->second.second << "   " << ansRadix->second.first.first << "  " << ansRadix->second.first.second << endl;

    // cout << endl << "The county with the lowest risk using radix is : " << ansBucket->second.second << "   " << ansBucket->second.first.first << "  " << ansBucket->second.first.second << endl;



     //----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    string start, end;

    cout << endl << "Enter Travel dates in YYYY-MM-DD format" << endl;

    cin >> start;
    cin >> end;

    unordered_map<string, float> riskLevel;
    unordered_map<float, string> riskLevelInv;

    float* arr1 = new float[counties.size()]; //qsort
    vector<int> radixSortVect;
    radixSortVect.resize(counties.size());
    vector<float> bucketSortVect;

    int c = 0;
    countiesIter = counties.begin();

    bool validDates = true;

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
            float risk = (float)(end) / (float)(start);
            */

            float risk = calculateRiskLevel(numCase1->second, numCase2->second);

            if (risk == 0) {
                validDates = false;
            }


            risk = roundf(risk * 100000) / 100000;  //Round to 5 decimals places

            riskLevel[*countiesIter] = risk;
            riskLevelInv[risk] = *countiesIter;
            arr1[c] = risk;
            radixSortVect[c] = (int)(risk*10000);
            bucketSortVect.push_back(risk);
            c++;
        }

        countiesIter++;
    }

    auto iterRL = riskLevel.begin();

    while (iterRL != riskLevel.end()) {
        cout << iterRL->first << "   " << iterRL->second << endl;
        iterRL++;
    }

  

    bucketSort myBucket = bucketSort::bucketSort(bucketSortVect, counties.size(), "ascending");
    float ret = myBucket.Sort(10);

    qsort(arr1, counties.size(), sizeof(float), ascending);

    radixsort(radixSortVect);


    cout << endl << "The county with the lowest risk using radix is : " << riskLevelInv.find(((float)radixSortVect[0] / (float)10000))->second << endl;

    cout << endl << "The county with the lowest risk using bucket is : " << riskLevelInv.find(ret)->second << endl;

    cout << endl << "The county with the lowest risk using quick is : " << riskLevelInv.find(arr1[0])->second << endl;


    if (!validDates) {


        cout << "However, these specific travel dates have unreliable information. We suggest you choose other dates." << endl;

    }

    return 0;

}