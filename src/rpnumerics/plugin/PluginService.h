
#ifndef _PLUGINSERVICE_H
#define	_PLUGINSERVICE_H

#include "RpnPlugin.h"
#include <sys/types.h>
#include <dirent.h>
#include <dlfcn.h>
#include <iostream>
#include <vector>

using std::cout;
using std::cerr;
using std::string;
using std::vector;

class PluginService {
private:

    string  *libFileName_;
    void * pluginlib_;
    
//    void search();
    
public:

    RpnPlugin * load(const string );
    void unload(RpnPlugin *,const string);
    PluginService(const string );
    virtual ~PluginService();

};



#endif	/* _PLUGINSERVICE_H */

