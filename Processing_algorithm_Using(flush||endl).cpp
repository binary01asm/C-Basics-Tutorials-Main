#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

int main() {
    // उदाहरण 1: प्रोग्रेस बार
    cout << "Processing: 0%";
    for(int i=1; i<=10; i++) {
        this_thread::sleep_for(chrono::milliseconds(1000));  // 1 सेकंड रुकें
        cout << "\rProcessing: " << i*10 << "%" << flush;
        //cout << "\rProcessing: " << i*10 << "%" << endl;
        // \r करसर को लाइन की शुरुआत में लाता है
        // flush बफर तुरंत खाली करता है
    }
    
    // उदाहरण 2: रियल-टाइम लॉगिंग
    cout << "Starting program..." << endl;  // तुरंत दिखेगा
    // कुछ काम
    cout << "Task 1 completed" << endl;  // तुरंत दिखेगा
    // और काम
    cout << "Task 2 completed" << endl;  // तुरंत दिखेगा
}