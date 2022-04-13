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

#pragma comment(lib, "urlmon.lib")

using namespace std;


int ascending(const void* x, const void* y) {
    return (*(float*)(x)*100000 - *(float*)(y)*100000);
}


int main()
{
    std::cout << "Hello World!\n";


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
    map<pair<string, string>, int> data;
    unordered_set<string> counties;

    getline(file, line);

    while (getline(file, line)) {

        stringstream row(line);
        string entry, date, loc;
        int cases, ctr = 0;

        while (getline(row, entry, ',')) {

            ctr++;

            if (ctr == 1) {
                // cout << entry;
                date = entry;
            }
            else if (ctr == 4) {
                //cout << entry << endl;
                counties.insert(entry);
                loc = entry;

            }
            else if (ctr == 9) {
                //  cout << "    " << entry << endl;
                if (!entry.empty()) {
                    cases = stoi(entry);
                    data[make_pair(loc, date)] = cases;
                    //data2[date + loc] = cases;
                }

                break;

            }


        }
        //cout << str << endl;
        rowNum++;

        if (rowNum == 100000) {
            break;
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

    map<pair<string, string>, int>::iterator* iterList = new map<pair<string, string>, int>::iterator[len];
    map < string, map<pair<string, string>, float> > generalRiskLevel;
    //map <  map<pair<string, string>, float>, string > generalRiskLevelInv;


    float* arrRiskLevel = new float[data.size()];

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
                int startDate = iterList[c]->second;
                if (startDate == 0) {
                    startDate++;
                }
                float risk = (float)iter->second / startDate;
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
            cout << superIter-> first << "  "<< i2->first.first << " " << i2->first.second << "  " << i2->second << endl;
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
            int start = numCase1->second, end = numCase2->second;
            if (start == 0) {
                start++;
            }
            float risk = (float)(end) / (float)(start);
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
    cout << endl <<"The county with the lowest risk is : " << riskLevelInv.find(arr1[0])->second << endl;

    return 0;

}
