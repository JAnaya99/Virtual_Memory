//nombres

#include <iostream>
#include <vector>
#include <string>
#include "OS.h"

using namespace std;

int main(){
    cout << "helo " << endl;
    virtual_memory::OS system;
    // Delete this comment.
    //CHECK OUTPUT LIMIT OF MEMORY > 2049
    {
        system.UploadProcess(0, 2048);
        system.UploadProcess(1, 16);
        system.UploadProcess(2, 32);
        system.AccessVirtualMemory(0, 15, 1);
        system.AccessVirtualMemory(0, 21, 0);
        system.AccessVirtualMemory(0, 15, 1);
        system.AccessVirtualMemory(0, 21, 0);
        system.AccessVirtualMemory(0, 15, 1);
        system.AccessVirtualMemory(0, 21, 0);
        system.AccessVirtualMemory(0, 15, 1);
        system.AccessVirtualMemory(0, 21, 0);
    }


    return 0;
}
