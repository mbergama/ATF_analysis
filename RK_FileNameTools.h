#ifndef RK_FileNameTools
#define RK_FileNameTools

#include <TSystem.h>
#include <sstream>
#include <string>
#include <vector>


std::vector<string> CommandList(const char *name=""){
    std::vector<string> OutRawList;
    string the_input = (string) (gSystem->GetFromPipe(name));
    stringstream input_stringstream(the_input);
    string parsed;
    
    //Get the raw list of file
    while(getline(input_stringstream,parsed,'\n')){
        OutRawList.push_back(parsed);
    }
    return OutRawList;
}


//Take a list of files and parse
std::vector<std::vector<string>> ParseInSublists(std::vector<string> ListToParse){
    std::vector<std::vector<string>> OutputList;
    std::vector<string> SameNameList ;
    std::string ActualString ="";
    std::string LastString ;
    //std::string FileNumber ;
    
    for (std::vector<string>::iterator it_str = ListToParse.begin(); it_str != ListToParse.end(); ++it_str){
        std::string strTmp= *it_str ;
        std::string str2 = strTmp.substr (0,strTmp.find(".fit"));
        
        //Separator used before the file number
        std::string key ("_");
        std::size_t found = str2.rfind(key);
        if (found!=std::string::npos){
            ActualString = str2.substr (0,found);
            //FileNumber = str2.substr (found+1, str2.length());
        }
        
        if (ActualString.compare(LastString) == 0){ //If they are equal we have 0
            SameNameList.push_back(*it_str);
        }
        else{
            if(SameNameList.size()>0){ //To avoid problem on first one
                OutputList.push_back(SameNameList);
            }
            SameNameList.clear();
            SameNameList.push_back(*it_str);
        }
        LastString = ActualString;
    }
    return OutputList;
}

void CheckParsedFilenames(std::vector<std::vector<string>> TheList){
    for (std::vector<std::vector<string>>::iterator it_list = TheList.begin(); it_list != TheList.end(); ++it_list){
        cout<<"NEW LISTE"<<endl;
        
        std::vector<string> TmpList = *it_list;
        for (std::vector<string>::iterator it_str = TmpList.begin(); it_str != TmpList.end(); ++it_str){
            cout<<"\t"<<*it_str<<endl;
        }
        
    }
}

#endif

