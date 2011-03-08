// local includes
#include "tictoc.h"
#include "tclap/CmdLine.h"

// c++
#include <iostream>
#include <fstream>
#include <ostream>

// c
#include <stdlib.h>

using namespace std;
using namespace TCLAP;


string path_dfile;
string path_wfile;
string path_names;
string path_distout;


bool flag_debug = false;

int numthreads;

char *db = NULL;
char *w = NULL;


void parseargs(int argc, char **argv) {
    try {
        CmdLine cmd("Command description message", ' ', "0.9");

        // Define a value argument and add it to the command line.
        ValueArg<string> pathDFileArg("D","database_file",
                                    "Signature database (sparse format)",true,
                                    "signdb.bin","string");
        cmd.add( pathDFileArg );

        ValueArg<string> pathWFileArg("w","query_word_vector",
                                    "Query word vector (sparse format)",true,
                                    "words.bin","string");
        cmd.add( pathWFileArg );

        ValueArg<string> pathNameArg("n","names_file",
                              "File containing list of filenames for sig db",
                                    true,
                                    "/iqe/vocabs/signatures","string");
        cmd.add( pathNameArg );

        ValueArg<string> pathDistOutArg("o","dist_out_file",
                                  "Filename to write distance computations to",
                                  true,
                                  "/iqe/querylog/blah/distlist","string");
        cmd.add( pathDistOutArg );



        ValueArg<int> flagNumThreadsArg("t","num_threads",
                   "Amount of parallelism",8,8,"int");
        cmd.add( flagNumThreadsArg );


        SwitchArg flagDebugArg("d","debug",
                                    "Print out lots of stuff",false);
        cmd.add( flagDebugArg );


        // Parse the args.
        cmd.parse( argc, argv );

        // Get the value parsed by each arg.
        path_dfile = pathDFileArg.getValue();
        path_wfile = pathWFileArg.getValue();
        path_names = pathNameArg.getValue();
        path_distout = pathDistOutArg.getValue();

        flag_debug = flagDebugArg.getValue();


        numthreads = flagNumThreadsArg.getValue();
    } catch (ArgException &e) {
        // catch & exit on any exceptions
        cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
        exit(1);
    }
}   
        



char *loadfile(const char *path) {
    ifstream f(path);

    if (! f.is_open() ) {
        cout << "error opening " << path << endl;
        exit(1);
    }
        
    // get the length of the file 
    f.seekg (0, ios::end);
    int64_t length = f.tellg();
    f.seekg (0, ios::beg);

    cout << path << " file length " << length << endl;
            
    // allocate memory:
    char *buffer = new char [length];

    if (buffer == NULL) {
        cout << "could not allocate memory" << endl;
        exit(1);
    }
     
    // read data as a block:
    f.read (buffer,length);
    f.close();

    return buffer;
}

inline uint8_t abs(uint8_t x, uint8_t y) {
    if (x > y) return x - y;
    else return y - x;
}

int main(int argc, char **argv) {

    parseargs(argc, argv);


    tic();
    db = loadfile(path_dfile.c_str());
    w = loadfile(path_wfile.c_str());
    double time_load = toc();

    cout << "Loaded database and query in " << time_load << " seconds" << endl;


    tic();
    // load the names
    vector<string> names;
    string line;
    int num_names = 0;
    
    ifstream names_file(path_names.c_str());   
    if (! names_file.is_open() ) {
        cout << "could not open names db" << endl;
        exit(1);
    }
    while (! names_file.eof() ) {
        getline(names_file, line);
        if (line != "") names.push_back(line);
        num_names++;
    }
    double time_loadnames = toc();  

    cout << "Loaded " << num_names << " names in " << time_loadnames
         << " seconds" << endl;


    ofstream dist_out(path_distout.c_str());


    if (flag_debug) {
        uint8_t m = 8;
        uint8_t n = 4;
        cout << "test abs " << (int)abs(m,n) << " " << (int)abs(n,m) << endl;
    }


    char *dbuf = db;
    char *wbuf = w;

    int64_t num_active_w = *((int64_t *)wbuf);
    cout << num_active_w << " active bins in input signature" << endl;

    int64_t num_signs = *((int64_t *)dbuf);
    cout << num_signs << " signatures in db" << endl;
    dbuf += sizeof(int64_t);

    tic();

    for (int64_t i = 0 ; i < num_signs ; ++i) {
        int64_t dist = 0;

        int64_t num_active_d = *((int64_t *)dbuf);
        if (flag_debug)
            cout << num_active_d << " active bins in signature " << i << endl;
        dbuf += sizeof(int64_t);

        wbuf = w + sizeof(int64_t);

        int64_t waddr = *((int64_t *)wbuf); wbuf += sizeof(int64_t);
        uint8_t wval = *((uint8_t *)wbuf);  wbuf += sizeof(uint8_t);

        int64_t daddr = *((int64_t *)dbuf); dbuf += sizeof(int64_t);
        uint8_t dval = *((uint8_t *)dbuf);  dbuf += sizeof(uint8_t);

        // the i'th active bin in w and d, respectively
        int iw = 0;
        int id = 0;

        while (iw < num_active_w && id < num_active_d) {

            if (flag_debug) {
                cout << "w " << iw << "/" << num_active_w << " ";
                cout << "d " << id << "/" << num_active_d << " ";
                cout << "waddr " << waddr << " wval " << (int)wval << " "
                     << "daddr " << daddr << " dval " << (int)dval << " ";
                cout << "dist " << dist << endl;
            }

            if (daddr < waddr) {
                if (flag_debug) {
                    cout << "addr " << daddr << " diff "
                         << (int)dval << " dist " << dist << endl;
                }

                dist += dval;
                id++;

                if (id < num_active_d) {
                    daddr = *((int64_t *)dbuf); dbuf += sizeof(int64_t);
                    dval = *((uint8_t *)dbuf);  dbuf += sizeof(uint8_t);
                }
            } else if (waddr < daddr) {
                if (flag_debug) {
                    cout << "addr " << waddr << " diff "
                         << (int)wval << " dist " << dist << endl;
                }

                dist += wval;
                iw++;

                if (iw < num_active_w) {
                    waddr = *((int64_t *)wbuf); wbuf += sizeof(int64_t);
                    wval = *((uint8_t *)wbuf);  wbuf += sizeof(uint8_t);
                }
            } else if (waddr == daddr) {
                if (flag_debug) {
                    cout << "addr " << waddr << " diff "
                         << (int)abs(wval, dval) << " dist " << dist << endl;
                }

                dist += abs(wval, dval);

                iw++;
                id++;

                if (id < num_active_d) {
                    daddr = *((int64_t *)dbuf); dbuf += sizeof(int64_t);
                    dval = *((uint8_t *)dbuf);  dbuf += sizeof(uint8_t);
                }

                if (iw < num_active_w) {
                    waddr = *((int64_t *)wbuf); wbuf += sizeof(int64_t);
                    wval = *((uint8_t *)wbuf);  wbuf += sizeof(uint8_t);
                }

            }

        }

        // add up the remaining active bins
        while (id < num_active_d) {
            dist += dval;

            if (flag_debug) {
                cout << "w " << iw << "/" << num_active_w << " ";
                cout << "d " << id << "/" << num_active_d << " ";
                cout << "waddr " << waddr << " wval " << (int)wval << " "
                     << "daddr " << daddr << " dval " << (int)dval << " "
                     << "dist " << dist << endl;
            }

            id++;

            if (id < num_active_d) {
                daddr = *((int64_t *)dbuf); dbuf += sizeof(int64_t);
                dval = *((uint8_t *)dbuf);  dbuf += sizeof(uint8_t);
            }

        }

        while (iw < num_active_w) {
            dist += wval;

            if (flag_debug) {
                cout << "w " << iw << "/" << num_active_w << " ";
                cout << "d " << id << "/" << num_active_d << " ";
                cout << "waddr " << waddr << " wval " << (int)wval << " "
                     << "daddr " << daddr << " dval " << (int)dval << " "
                     << "dist " << dist << endl;
            }

            iw++;

            if (iw < num_active_w) {
                waddr = *((int64_t *)wbuf); wbuf += sizeof(int64_t);
                wval = *((uint8_t *)wbuf);  wbuf += sizeof(uint8_t);
            }
        }

        if (flag_debug) {
            cout << "w " << iw << "/" << num_active_w << " ";
            cout << "d " << id << "/" << num_active_d << " ";
            cout << "waddr " << waddr << " wval " << (int)wval << " "
                 << "daddr " << daddr << " dval " << (int)dval << " "
                 << "dist " << dist << endl;
        }

        dist_out << names[i] << " " << dist << endl;
    }

    dist_out.close();

    double time_dist = toc();

    cout << "Computed " << num_signs << " distances in " << time_dist
         << " seconds" << endl;

}
