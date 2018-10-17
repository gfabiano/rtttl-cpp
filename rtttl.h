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


#include <vector>
#include <iostream>

using std::move;
using std::string;
using std::vector;

class RTTTL {
    public:
        RTTTL(const char *t_str): m_rtttl_str{t_str} {
            parse_title();
            parse_defaults();
            parse_notes();
        }

        string get_title() const {
            return m_title;
        }

        int get_default_octave() const {
            return m_octave;
        }

        int get_default_duration() const {
            return m_duration;
        }

        int get_default_bpm() const {
            return m_bpm;
        }

        struct atomic_note {
            char name[3] = {0};
            int duration;
            int octave;
            double freq;
            int msec;
        };

        vector<atomic_note> get_vec_notes() {
            return vec_notes;
        }

    private:
        const string m_rtttl_str;
        uint64_t m_pos{0};
        char m_actual, m_prec;

        string m_title;
        int m_octave{0};
        int m_duration{0};
        int m_bpm{0};
        double m_msec_semibreve{750};

        vector<atomic_note> vec_notes;

        const double m_notes[16] {
            440.0,  // A      ,La
            493.9,  // B or H ,Si
            261.6,  // C      ,Do
            293.7,  // D      ,Re
            329.6,  // E      ,Mi
            349.2,  // F      ,Fa
            392.0,  // G      ,Sol
            0.0,    // pausa

            466.2,  // A#     ,La#
            261.6,  // B# = C ,Si#
            277.2,  // C#     ,Do#
            311.1,  // D#     ,Re#
            349.2,  // E# = F ,Mi#
            370.0,  // F#     ,Fa#
            415.3,  // G#     ,Sol#
            0.0,    // pausa
        };

        char next_char() {
            if (m_pos > m_rtttl_str.size()) {
                m_actual = '\0';
                return m_actual;
            }
            m_prec = m_actual;
            m_actual = tolower(m_rtttl_str[m_pos++]);
            return m_actual;
        }
        
        void parse_title() {
            next_char();
            for (; m_actual != ':' && m_actual != '\0'; next_char()) {
                m_title += m_actual;
            }
        }

        int parse_num() {
            int val = 0;
            do {
                val *= 10;
                val += m_actual- '0';
            } while(isdigit(next_char()));

            return val;
        }
        
        void parse_defaults() {
            char id{' '};
            
            while(' ' == next_char());
            do {
                if (isdigit(m_actual)) {
                    switch(id) {
                       case 'o': m_octave = parse_num(); break;
                       case 'd': m_duration = parse_num(); break;
                       case 'b': m_bpm = parse_num(); break;
                    }
                    continue;
                } else if (isalpha(m_actual)) {
                    id = m_actual;
                }
                next_char();
            } while(':' != m_actual);

            m_msec_semibreve = 240000 / m_bpm;
        }

        void parse_notes() {
            for(;;) { 
                atomic_note a_n;
                while(' ' == next_char() || m_actual == ',');
                if (m_actual == 0) break;

                // d represent the duration
                // d = 1 is a semibreve
                // d = 1/8 means 1/8 semibreve ecc.           
                a_n.duration = [this]() {
                    if (isdigit(m_actual)) {
                        return parse_num();
                    }
                    return m_duration;
                }();    

                // if (c == '|')
                //      return;
                
                // Set the name of note
                a_n.name[0] = m_actual;
                int id_note = [this]() -> int {
                    if (m_actual >= 'a' && m_actual <= 'g') {
                        return m_actual - 'a';
                    } else if (m_actual == 'h') { // H == B
                        return 1;
                    }

                    // Pause
                    return 7;
                }();


                // MODIFIERS
                
                next_char();

                // Check for diesis
                if (m_actual == '#') {
                    id_note += 8;
                    next_char();
                }

                // Check for octave
                a_n.octave = [this]() {
                    if (m_actual >= '4' && m_actual <= '7') {
                        next_char();
                        return m_prec - '0';
                    }
                    return m_octave;
                }();

                // Check duration modifier
                double mult_duration = [this]() {
                    if(m_actual == '.') {
                        next_char();
                        return 1.5;
                    }
                    return 1.0;
                }();


                a_n.freq = m_notes[id_note] * (1 << (a_n.octave - 4));
                a_n.msec = (m_msec_semibreve / a_n.duration) * mult_duration;
    
                a_n.name[1] = (id_note > 7)?'#': '\0';
    
                vec_notes.push_back(move(a_n));
            }
        }
};

