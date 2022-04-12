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
#include <cstdlib>

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
                    data[make_pair(date, loc)] = cases;
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

    auto iter = data.begin();

    while (iter != data.end()) {
       // cout << iter->first.first << "  " << iter->first.second << "  " << iter->second << endl;
        iter++;
    }

    cout << data.size() << endl;

    auto countiesIter = counties.begin();

    cout << endl << "The counties that exist in this state are:" << endl;

    while (countiesIter != counties.end()) {
        cout << *countiesIter << endl;
        countiesIter++;
    }

    /*auto iter2 = data2.begin();
    while (iter2 != data2.end()) {
        //cout << iter->first.first << "  " << iter->first.second << "  " << iter->second << endl;
        iter2++;
    }*/

    file.close();

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
        pair<string, string> key = make_pair(start, *countiesIter);
        auto numCase1 = data.find(key);

        key = make_pair(end, *countiesIter);
        auto numCase2 = data.find(key);

        if (numCase1 != data.end() && numCase2 != data.end()) {
            float risk = (float)(numCase2->second) / (float)(numCase1->second);
            riskLevel[*countiesIter] = (float)(numCase2->second) / (float)(numCase1->second);
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

    for (int i = 0; i < counties.size(); i++)
        cout << arr1[i] << " ";

    cout << endl <<"The county with the lowest risk is : " << riskLevelInv.find(arr1[0])->second << endl;



    return 0;

}