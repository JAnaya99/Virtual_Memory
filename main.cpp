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
       system.UploadProcess(2, 32);
       system.UploadProcess(3, 48);
       system.UploadProcess(4, 63);
       system.FreeProcess(3);
       system.UploadProcess(5, 80);
       system.UploadProcess(109, 1744);
       system.UploadProcess(6, 96);
       system.AccessVirtualMemory(2,16,0);
       system.AccessVirtualMemory(4,63,0);
       system.AccessVirtualMemory(4,62,1);
       system.AccessVirtualMemory(109,1,1);
       system.UploadProcess(61,96);
       system.FreeProcess(2);
       system.AccessVirtualMemory(5,8,0);
       system.AccessVirtualMemory(4,8,0);
       system.FreeProcess(5);
       system.FreeProcess(2);
       system.FreeProcess(4);
       system.FreeProcess(6);
       system.FreeProcess(61);
       system.FreeProcess(109);
       system.RestartMemory();
       system.AccessVirtualMemory(2,2,0);
       system.UploadProcess(2049,2049);
       system.RestartMemory();
    }


    return 0;
}
