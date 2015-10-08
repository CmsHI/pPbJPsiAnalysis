#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <algorithm>
#include <stdlib.h>

using namespace std;

// Define which y, pT, cent bins are going to be used
// The order of elements will be kept in the final results!

// --------- Rapidity - Array Ordering from forward to backward !

////// for Pbp
////double diff.
//static string str_rap[] = {"-2.4--1.97"}; // 01 : 1.5< y_CM < 1.93
//static string str_rap[] = {"-1.97--1.47"}; //02 : 1 < y_CM < 1.5
//static string str_rap[] = {"-1.47--0.47"}; // 03 : 0 < y_CM < 1
//static string str_rap[] = {"-0.47-0.53"}; //04 : -1 < y_CM < 0
//static string str_rap[] = {"0.53-1.03"}; //05 : -1.5 < y_CM < -1
static string str_rap[] = {"1.03-1.93"}; // 06 : -2.4 < y_CM < -1.5

////// for pPb
////double diff.
//static string str_rap[] = {"1.97-2.4"}; // 01 : 1.5< y_CM < 1.93
//static string str_rap[] = {"1.47-1.97"}; //02 : 1 < y_CM < 1.5
//static string str_rap[] = {"0.47-1.47"}; // 03 : 0 < y_CM < 1
//static string str_rap[] = {"-0.53-0.47"}; // 04 : -1 < y_CM < 0
//static string str_rap[] = {"-1.03--0.53"}; // 05 : -1.5 < y_CM < -1
//static string str_rap[] = {"-1.93--1.03"}; // 06 : -2.4 < y_CM < -1.5

////// for pp
//static string str_rap[] = {"0.00-1.93","-1.93-0.00"}; // rapcoarser

// --------- pT

//static string str_pt[]={"0.0-3.0", "3.0-6.5", "6.5-7.5", "7.5-8.5", "8.5-9.5", "9.5-11.0", "11.0-14.0", "14.0-30.0"}; // for 01
//static string str_pt[]={"6.5-7.5", "7.5-8.5", "8.5-9.5", "9.5-11.0", "11.0-14.0", "14.0-30.0"}; // for 02-05
static string str_pt[]={"3.0-6.5", "6.5-7.5", "7.5-8.5", "8.5-9.5", "9.5-11.0", "11.0-14.0", "14.0-30.0"}; // for 06

// --------- Centrality

static string str_cent[] = {"0.0-100.0"}; // also for pp
//static string str_cent[] = {"50-100", "30-50", "10-30", "0-10"};
//static string str_cent[] = {"70-100", "60-70", "50-60", "40-50", "30-40", "20-30", "10-20", "0-10"}; //KYO

// --------- Ntracks
static string str_ntr[]={"0.0-350.0"};

// --------- ET^{HF}
static string str_et[]={"0.0-120.0"};
//static string str_et[]={"0.0-20.0", "20.0-25.0", "25.0-30.0", "30.0-50.0", "50.0-100.0"};


static std::vector<string> rapidity(str_rap,str_rap+sizeof(str_rap)/sizeof(string));
static std::vector<string> pt(str_pt,str_pt+sizeof(str_pt)/sizeof(string));
static std::vector<string> centrality(str_cent,str_cent+sizeof(str_cent)/sizeof(string));
static std::vector<string> ntracks(str_ntr,str_ntr+sizeof(str_ntr)/sizeof(string));
static std::vector<string> ethf(str_et,str_et+sizeof(str_et)/sizeof(string));
static const int nRap = sizeof(str_rap)/sizeof(string);
static const int nPt = sizeof(str_pt)/sizeof(string);
static const int nCent = sizeof(str_cent)/sizeof(string);
static const int nNtrk = sizeof(str_ntr)/sizeof(string);
static const int nEt = sizeof(str_et)/sizeof(string);

struct eachRow {
  string rap, pt, cent, ntr, et;
  double nsig, nsigerr, npr, nprerr, nnp, nnperr;
  eachRow():rap(str_rap[0]),pt(str_pt[0]),cent(str_cent[0]),ntr(str_ntr[0]),et(str_et[0]),nsig(1),nsigerr(0.1),npr(1),nprerr(0.1),nnp(1),nnperr(0.1){ };
};

bool ComparatorWithArray(eachRow data1, eachRow data2) {
  bool result;
 
  // Check item is in the comparison array 
  std::vector<string>::iterator locRap1 = find(rapidity.begin(),rapidity.end(),data1.rap);
  std::vector<string>::iterator locPt1 = find(pt.begin(),pt.end(),data1.pt);
  std::vector<string>::iterator locCent1 = find(centrality.begin(),centrality.end(),data1.cent);
  std::vector<string>::iterator locNtrk1 = find(ntracks.begin(),ntracks.end(),data1.ntr);
  std::vector<string>::iterator locEt1 = find(ethf.begin(),ethf.end(),data1.et);

  std::vector<string>::iterator locRap2 = find(rapidity.begin(),rapidity.end(),data2.rap);
  std::vector<string>::iterator locPt2 = find(pt.begin(),pt.end(),data2.pt);
  std::vector<string>::iterator locCent2 = find(centrality.begin(),centrality.end(),data2.cent);
  std::vector<string>::iterator locNtrk2 = find(ntracks.begin(),ntracks.end(),data2.ntr);
  std::vector<string>::iterator locEt2 = find(ethf.begin(),ethf.end(),data2.et);

  if ( (locRap1 == rapidity.end()) || (locPt1 == pt.end()) || (locCent1 == centrality.end()) ||
			 (locNtrk1 == ntracks.end()) || (locEt1 == ethf.end()) || 
       (locRap2 == rapidity.end()) || (locPt2 == pt.end()) || (locCent2 == centrality.end()) || 
			 (locNtrk2 == ntracks.end()) || (locEt2 == ethf.end()) ) {
    cerr << "ComparatorWithArray:: one of element isn't found in rapidity/pt/centrality/ntrk/et array!" << std::endl;
    std::cout << "ComparatorWithArray:: one of element isn't found in rapidity/pt/centrality/ntrk/et array!" << std::endl;
    return false;
  }

  // Check found item's index number
  int idxRap1 = distance(rapidity.begin(), locRap1);
  int idxPt1 = distance(pt.begin(), locPt1); 
  int idxCent1 = distance(centrality.begin(), locCent1);
  int idxNtrk1 = distance(ntracks.begin(), locNtrk1);
  int idxEt1 = distance(ethf.begin(), locEt1);
  int idxRap2 = distance(rapidity.begin(), locRap2);
  int idxPt2 = distance(pt.begin(), locPt2); 
  int idxCent2 = distance(centrality.begin(), locCent2);
  int idxNtrk2 = distance(ntracks.begin(), locNtrk2);
  int idxEt2 = distance(ethf.begin(), locEt2);

  // Compare 2 items
  if (idxRap1 < idxRap2) {
    result = true;
  } else if ((idxRap1 == idxRap2)&&(idxPt1 < idxPt2)) {
    result = true;
  } else if ((idxRap1 == idxRap2)&&(idxPt1 == idxPt2)&&(idxCent1 < idxCent2)) {
    result = true;
  } else if ((idxRap1 == idxRap2)&&(idxPt1 == idxPt2)&&(idxCent1 == idxCent2)&&(idxNtrk1 < idxNtrk2)) {
    result = true;
  } else if ((idxRap1 == idxRap2)&&(idxPt1 == idxPt2)&&(idxCent1 == idxCent2)&&(idxNtrk1 == idxNtrk2)&&(idxEt1 < idxEt2)) {
    result = true;
  } else {
    result = false;
  }

  return result;
};

bool CheckSameEntries(eachRow data1, eachRow data2) {
  // Compare 2 items in the list
  bool result;
  if (data1.rap == data2.rap && data1.pt == data2.pt && data1.cent == data2.cent && data1.ntr == data2.ntr && data1.et == data2.et) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

bool FindMissingElement(std::list<eachRow> data) {
  if (nRap*nPt*nCent*nNtrk*nEt != data.size()) {
    std::cout << "Data size: " << data.size() << "\tReference size: " << nRap*nPt*nCent*nNtrk*nEt << std::endl;
    
    std::list<string> existData, reference;  // rap, pt, cent, dphi(fixed), ntrk, et
    std::list<string>::iterator it;
    std::list<eachRow>::iterator it_eachRow;

    for (int i=0; i<nRap; i++) {
      for (int j=0; j<nPt; j++) {
        for (int k=0; k<nCent; k++) {
        	for (int m=0; m<nNtrk; m++) {
        		for (int n=0; n<nEt; n++) {
		          //reference.push_back(rapidity[i] + "_" + pt[j] + "_" + centrality[k] + "_0.000-1.571_" + ntracks[m] + "_" + ethf[n]);
		          reference.push_back(rapidity[i] + "_" + pt[j] + "_" + centrality[k] + "_" + ntracks[m] + "_" + ethf[n]);
    				}
					}
		    }
      }
    } // Fill up a list with required bins
    
    for (it_eachRow=data.begin(); it_eachRow!=data.end(); it_eachRow++) {
      //existData.push_back((*it_eachRow).rap +"_"+ (*it_eachRow).pt +"_"+ (*it_eachRow).cent+"_0.000-1.571_"+(*it_eachRow).ntr+"_"+(*it_eachRow).et);
      existData.push_back((*it_eachRow).rap +"_"+ (*it_eachRow).pt +"_"+ (*it_eachRow).cent+"_"+(*it_eachRow).ntr+"_"+(*it_eachRow).et);
    } // Fill up a list with data vector

    // Find same entries both on reference and data and remove those entries
    for (it = existData.begin(); it != existData.end(); it++) {
      reference.remove(*it);
    }

    // Prints out not-matched entries
    for (it = reference.begin(); it != reference.end(); it++) {
      std::cout << "FindMissingElement: " << (*it) << std::endl;
    }
    return true;
  } else false;
}

int main(int argc, char *argv[]) {

  // Read fit result summary file
  char *inputFile;
  if (argc > 1) inputFile = argv[1];
  else {
    std::cout << "./a.out [input file] [output file]" << std::endl;
    return -2;
  }
  fstream finput;
  finput.open(inputFile);
  if (!finput.good()) {
    std::cout << "Cannot open input file: " << inputFile << std::endl;
    return -1;
  }

	std::cout << "nRap = " << nRap << std::endl;
	std::cout << "nPt = " << nPt << std::endl;
	std::cout << "nCent = " << nCent << std::endl;
	std::cout << "nNtrk = " << nNtrk << std::endl;
	std::cout << "nEt = " << nEt << std::endl;

  // Read input file's header, column names
  string title; // which dataset/fit method is used for this table?
  getline(finput,title);
  string colNameTmp; // 2nd line contains column names
  getline(finput,colNameTmp);
  istringstream iss(colNameTmp); // Get column names (-> split colName)
  std::vector<string> colName;
  while (iss) {
    string sub;    iss >> sub;
    colName.push_back(sub);
  }
  colName.pop_back(); // Removes the last \n character

  std::list<eachRow> dataValueUseful; // fit results (only useful bins)
  std::list<eachRow>::iterator it_eachRow;

  // Read all lines in the file and store results into the vector
  while (finput.good()) {
    //cout << "XIA : input file is good" << endl;
		// Read a row and parsing it
    string dataline;
    getline(finput,dataline);
    if (dataline.size() == 0) break;  // EOF reached
    istringstream datarow(dataline);
    
    std::vector<string> dataValue; // fit results in each element
    while (datarow) {
      string sub;    datarow >> sub;
      dataValue.push_back(sub);   // Read data 1 line
    }
    if (dataValue.back() == "\n") dataValue.pop_back(); // Removes the last \n character

    // Check this row is useful or not
    std::vector<string>::iterator irap, ipt, icent, intr, iet;
    irap = find(rapidity.begin(),rapidity.end(),dataValue[0]);
    ipt = find(pt.begin(),pt.end(),dataValue[1]);
    icent = find(centrality.begin(),centrality.end(),dataValue[2]);
		intr = find(ntracks.begin(),ntracks.end(),dataValue[4]);
		iet = find(ethf.begin(),ethf.end(),dataValue[5]);
		//cout << "** XIA :: dataValue check= " << dataValue[0] <<" "<<dataValue[1]<<" "<<dataValue[2]<<" "<<dataValue[3]<<" "<< dataValue[4] <<" "<<dataValue[5]<<std::endl;
		//cout << "         "<<dataValue[6]<<" "<<dataValue[7]<<" "<< dataValue[10] <<" "<<dataValue[11]<<" "<<dataValue[12]<<" "<<dataValue[13]<<" "<< std::endl;

    //6,7(NSig,NSigErr), 10,11(PROMPT, PROMPTErr), 12,13(NON-PROMPT, NON-PROMPTErr)
    if (irap != rapidity.end() && ipt != pt.end() && icent != centrality.end() && intr != ntracks.end() && iet != ethf.end() ) {
      eachRow data;
      data.rap = dataValue[0];
      data.pt = dataValue[1];
      data.cent = dataValue[2]; // [3] = dphi
			data.ntr = dataValue[4];
			data.et = dataValue[5];
      data.nsig = atof(dataValue[6].c_str());
      data.nsigerr = atof(dataValue[7].c_str()); // [8]=bkg, [9]=bkgErr
      data.npr = atof(dataValue[10].c_str());
      data.nprerr = atof(dataValue[11].c_str());
      data.nnp = atof(dataValue[12].c_str());
      data.nnperr = atof(dataValue[13].c_str());
      dataValueUseful.push_back(data);
 			//cout << "XIA : data.bin check = " << data.rap <<" "<<data.pt<<" "<< data.cent<<" "<<data.ntr<<" "<<data.et<<endl;
			//cout << "sig, pr, np = "<<data.nsig<<" "<<data.nsigerr<<" "<<data.npr<<" "<<data.nprerr<<" "<<data.nnp<<" "<<data.nnperr<<endl;
    }
  }
  finput.close();
  
  // Remove duplicate entries (It happens when a bin doesn't have a correct fit result)
  dataValueUseful.unique(CheckSameEntries);

  // Sort the final value array with given order (needs to be corrected!!!)
  dataValueUseful.sort(ComparatorWithArray);

  // If there are missing entries in dataValueUseful, find which bin is missing
  if (FindMissingElement(dataValueUseful)) return -3;

  // Write a txt file with sorted results
  char *outputName;
  if (argc > 2) outputName = argv[2];
  else {
    std::cout << "./a.out [input file] [output file]" << std::endl;
    return -2;
  }
  fstream foutput;
  foutput.open(outputName, ios_base::out|ios_base::trunc);
  if (!foutput.good()) {
    std::cout << "Cannot open output file: " << outputName << std::endl;
    return -1;
  }
  for (it_eachRow = dataValueUseful.begin(); it_eachRow != dataValueUseful.end(); it_eachRow++) {
    foutput << (*it_eachRow).nsig << "\t"
      << (*it_eachRow).nsigerr << "\t"
      << (*it_eachRow).npr << "\t"
      << (*it_eachRow).nprerr << "\t"
      << (*it_eachRow).nnp << "\t"
      << (*it_eachRow).nnperr << std::endl;
    std::cout << std::setiosflags(ios::left) 
      << std::setw(14) << (*it_eachRow).rap << "\t" << std::setw(10) << (*it_eachRow).pt << "\t" << std::setw(7) << (*it_eachRow).cent << "\t"
			<< std::setw(7) << (*it_eachRow).ntr << "\t" << std::setw(7) << (*it_eachRow).et << "\t"
      << (*it_eachRow).nsig  << "\t"
      << (*it_eachRow).nsigerr << "\t"
      << (*it_eachRow).npr << "\t"
      << (*it_eachRow).nprerr << "\t"
      << (*it_eachRow).nnp << "\t"
      << (*it_eachRow).nnperr << std::endl;
  }
  foutput.close();

  return 0;
}
