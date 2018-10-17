// Copyright 2018 Giuseppe Fabiano

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <iostream>
#include "../rtttl.h"

using std::cout;

int main() {
    char hant_house[] = "HauntHouse: d=4,o=5,b=108: 2a4, 2e, 2d#, 2b4, 2a4, 2c, 2d, 2a#4, 2e., e, 1f4, 1a4, 1d#, 2e., d, 2c., b4, 1a4, 1p, 2a4, 2e, 2d#, 2b4, 2a4, 2c, 2d, 2a#4, 2e., e, 1f4, 1a4, 1d#,          2e., d, 2c., b4, 1a4";
 
    RTTTL test(hant_house); 
    printf("%s: %d, %d, %d\n", 
            test.get_title().c_str(),
            test.get_default_octave(),
            test.get_default_duration(),
            test.get_default_bpm());
    
    vector<RTTTL::atomic_note> notes = test.get_vec_notes();
    for(size_t i = 0; i < notes.size(); ++i) {
        cout << "note:" << notes[i].name <<
            ", duration:" << notes[i].duration << ", ottave:"<< notes[i].octave << 
            ", freq:" << notes[i].freq << ", msec:" << notes[i].msec << '\n';
    }
    return 0;
 }

