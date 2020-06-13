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
        auto it = system.UploadProcess(0, 2049);
    }


    return 0;
}
