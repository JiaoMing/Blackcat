//
//  httpdata.h
//  Blackcat
//
//  Created by haojunhua on 14-4-3.
//
//

#ifndef Blackcat_httpdata_h
#define Blackcat_httpdata_h
#include <string>
using namespace std;
static const char s_dnPrefix[]="http://www.kidsedu.com/";
static const char s_apiPath[]="api/json/";
static string s_getFullPath(const char* request,bool isBlackcatServer=true){
    string fullPath;
    fullPath.append(s_dnPrefix);
    if (isBlackcatServer) {
        fullPath.append("blackcat/");
    }
    fullPath.append(s_apiPath);
    fullPath.append(request);
    return fullPath;
}

#endif
